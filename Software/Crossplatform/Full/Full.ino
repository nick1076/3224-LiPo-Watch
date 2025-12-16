//Headers
#include <Adafruit_GFX.h> //Graphics library for display
#include <Adafruit_SSD1306.h> //Display library
#include <Wire.h> //For custom i2c
#include <DS3231.h> //Library for RTC IC

//Vars
#define SCREEN_WIDTH 128 //Width of display used
#define SCREEN_HEIGHT 64 //Height of display used
#define WAKE_INPUT 1 //Wake Button Input
#define SET_INPUT 9 //Set Button Input

bool awake = false; //Tracks if screen is currently awake
bool setMode = false; //Tracks if watch is currently in SET mode (for setting time)
bool initialized = false; //Tracks if screen initialized properly

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); //Screen
DS3231 myRTC; //RTC IC

//RTC Vars
bool century = false;
bool h12Flag;
bool pmFlag;
byte alarmDay, alarmHour, alarmMinute, alarmSecond, alarmBits;
bool alarmDy, alarmH12Flag, alarmPmFlag;

//Other Watch control variables
byte dispCounter = 0;
int tick = 0;
int parallelTick = 0;
byte setIndex = 0;

byte currentSetHour = 0;
byte currentSetMinute = 0;

//Time (in increments of 10ms) between each uptick in current number
int setMode_increaseTimeCooldown = 5;

//Time (in increments of 10ms) between each index (hour to minute to exiting set mode)
int setMode_switchSetIndexCooldown = 8;

//Pixel offset of displayed data if hour time is double digits
int xDualDecimalHourOffset = -10;

void setup () {
    Serial.begin(57600);
    Wire.begin();

    pinMode(WAKE_INPUT, INPUT_PULLUP);
    pinMode(SET_INPUT, INPUT_PULLUP);

    delay(500);
    
    Serial.println("Initializing...");

    //Check if display has failed to begin
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
      Serial.println(F("SSD1306 allocation failed"));
      return;
    }
    else{
      //Otherwise display has started correctly
      Serial.println("Initialized.");
      initialized = true;
      Clear();
    }

    delay(500);

    SetClock(12, 0);
    Serial.println("Watch Ready!");
}

//Display info on watch screen
void DisplayText(String text, int x, int y, int size, bool clear = true){
  if (clear){
    display.clearDisplay();
  }
  display.setTextSize(size);
  display.setTextColor(WHITE);
  display.setCursor(x, y);
  display.println(text);
  display.display(); 
}

//Display time on OLED
void DisplayTime(int x, int y, int size, bool showDots, bool updateDisplay = true){
  display.clearDisplay();
  display.setTextSize(size);
  display.setTextColor(WHITE);

  if (myRTC.getHour(h12Flag, pmFlag) > 12){
    //Check if double digit hour, if so, move cursor X to the left some more
    if (myRTC.getHour(h12Flag, pmFlag)-12>9){
      display.setCursor(x+xDualDecimalHourOffset, y);
    }
    else{
      display.setCursor(x, y);
    }
  }
  else{
    if (myRTC.getHour(h12Flag, pmFlag) > 9){
      display.setCursor(x+xDualDecimalHourOffset, y);
    }
    else{
      display.setCursor(x, y);
    }
  }

  if (myRTC.getHour(h12Flag, pmFlag) > 12){
    display.print(myRTC.getHour(h12Flag, pmFlag)-12, DEC);
  }
  else{
    display.print(myRTC.getHour(h12Flag, pmFlag), DEC);
  }
  if (showDots){
    display.print(":");
  }
  else{
    display.print(" ");
  }
  if (myRTC.getMinute() < 10){
    display.print("0");
    display.println(myRTC.getMinute(), DEC);
  }
  else{
    display.println(myRTC.getMinute(), DEC);
  }

  if (!updateDisplay) { return; }
  display.display(); 
}

void loop () {
  delay(10); //10 ms delay per tick
  tick++;

  if (digitalRead(SET_INPUT) == LOW && !setMode){
    setMode = true;
    setIndex = 0;
    parallelTick = 0;
    tick=0;
    currentSetHour = myRTC.getHour(h12Flag, pmFlag);
    currentSetMinute = myRTC.getMinute();
    return;
  }

  if (setMode){
    DisplayTime(30, 20, 3, true, false);

    int xOffset = 0;

    if (myRTC.getHour(h12Flag, pmFlag) > 12){
      //Check if double digit hour, if so, move cursor X to the left some more
      if (myRTC.getHour(h12Flag, pmFlag)-12>9){
        xOffset+=xDualDecimalHourOffset;
      }
    }
    else{
      if (myRTC.getHour(h12Flag, pmFlag) > 9){
        xOffset+=xDualDecimalHourOffset;
      }
    }
    
    if (setIndex == 0){
      DisplayText("H", 50+xOffset, 50, 2, false);
    }
    else{
      DisplayText("M", 50+xOffset, 50, 2, false);
    }
    
    parallelTick++;
    awake = false;
    if (digitalRead(WAKE_INPUT) == LOW && parallelTick >= setMode_increaseTimeCooldown){
      parallelTick = 0;

      //Hour slot
      if (setIndex == 0){
        currentSetHour++;
        
        if (currentSetHour > 24){
          currentSetHour = 1;
        }
        
        myRTC.setHour(currentSetHour);
      }

      //Minutes slot
      else if (setIndex == 1){
        currentSetMinute++;
        
        if (currentSetMinute > 59){
          currentSetMinute = 0;
        }
        
        myRTC.setMinute(currentSetMinute);
      }
    }

    //If user hits set button again after cooldown
    if (digitalRead(SET_INPUT) == LOW && tick >= setMode_switchSetIndexCooldown){
      tick=0;
      setIndex++;

      //Exit set mode
      if (setIndex >= 2){
        tick = 0;
        Clear();
        setMode = false;
        delay(500);
        return;
      }
    }
  }

  if (setMode){
    return;
  }
  
  if (!awake && digitalRead(WAKE_INPUT) == LOW){
    dispCounter = 0;
    awake = true;
  }
  else if (!awake){
    Clear();
    return;
  }
  else if (awake){
    if (dispCounter >= 10){
      awake = false;
      dispCounter = 0;
      Clear();
      return;
    }
  }
  
  if (tick < 50){
    return;
  }
  tick=0;
  
  if (dispCounter % 2 == 0){
    DisplayTime(30, 20, 3, true);
  }
  else{
    DisplayTime(30, 20, 3, false);
  }
  dispCounter++;
  if (dispCounter >= 200){
    dispCounter=0;
  }
  if (myRTC.getHour(h12Flag, pmFlag) > 12){
    Serial.print(myRTC.getHour(h12Flag, pmFlag)-12, DEC);
  }
  else{
    Serial.print(myRTC.getHour(h12Flag, pmFlag), DEC);
  }
  Serial.print(":");
  Serial.print(myRTC.getMinute(), DEC);
  Serial.print(":");
  Serial.print(myRTC.getSecond(), DEC);
 
  // Add AM/PM indicator
  if (h12Flag) {
    if (pmFlag) {
      Serial.print(" PM ");
    } else {
      Serial.print(" AM ");
    }
  } else {
    Serial.println(" 24h");
  }
}

//Set current clock hour and minute
void SetClock(byte hour, byte minute){
    myRTC.setClockMode(false);  // set to 24h
    
    myRTC.setYear(0);
    myRTC.setMonth(0);
    myRTC.setDate(0);
    myRTC.setDoW(0);
    myRTC.setHour(hour);
    myRTC.setMinute(minute);
    myRTC.setSecond(0);
}

//Clear watch screen
void Clear(){
  display.clearDisplay();
  display.display(); 
}
