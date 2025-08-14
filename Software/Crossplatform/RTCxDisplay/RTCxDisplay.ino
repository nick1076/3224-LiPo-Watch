//Headers
#include <Adafruit_GFX.h> //Graphics library for display
#include <Adafruit_SSD1306.h> //Display library

//Vars
#define SCREEN_WIDTH 128 //Width of display used
#define SCREEN_HEIGHT 64 //Height of display used
#define WAKE_INPUT 1 //Wake Button Input
#define SET_INPUT 9 //Set Button Input

bool awake = false;
bool setMode = false;
bool initialized = false; //Tracks if screen initialized properly

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); //Screen

#include <Wire.h>
#include <DS3231.h>

DS3231 myRTC;

bool century = false;
bool h12Flag;
bool pmFlag;
byte alarmDay, alarmHour, alarmMinute, alarmSecond, alarmBits;
bool alarmDy, alarmH12Flag, alarmPmFlag;

byte dispCounter = 0;
int tick = 0;
int parallelTick = 0;
byte setIndex = 0;

byte currentSetHour = 0;
byte currentSetMinute = 0;

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
    SetClock(14, 46);
    Serial.println("Nano Ready!");
}

void loop () {
  delay(10);
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
    DisplayTime(30, 20, 3, true);
    parallelTick++;
    awake = false;
    if (digitalRead(WAKE_INPUT) == LOW && parallelTick >= 10){
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
    if (digitalRead(SET_INPUT) == LOW && tick >= 25){
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

void DisplayTime(int x, int y, int size, bool showDots){
  display.clearDisplay();
  display.setTextSize(size);
  display.setTextColor(WHITE);
  display.setCursor(x, y);


  if (myRTC.getHour(h12Flag, pmFlag) > 12){
    display.print(myRTC.getHour(h12Flag, pmFlag)-12, DEC);
  }
  else{
    if (myRTC.getHour(h12Flag, pmFlag) > 10){
      display.print(" ");
      display.print(myRTC.getHour(h12Flag, pmFlag), DEC);
    }
    else{
      display.print(myRTC.getHour(h12Flag, pmFlag), DEC);
    }
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
  
  display.display(); 
}

void Display(String text, int x, int y, int size){
  display.clearDisplay();
  display.setTextSize(size);
  display.setTextColor(WHITE);
  display.setCursor(x, y);
  display.println(text);
  display.display(); 
}

void Clear(){
  display.clearDisplay();
  display.display(); 
}
