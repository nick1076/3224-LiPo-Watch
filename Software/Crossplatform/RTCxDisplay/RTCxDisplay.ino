//Headers
#include <Adafruit_GFX.h> //Graphics library for display
#include <Adafruit_SSD1306.h> //Display library

//Vars
#define SCREEN_WIDTH 128 //Width of display used
#define SCREEN_HEIGHT 64 //Height of display used

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

void setup () {
    Serial.begin(57600);
    Wire.begin();
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
    }
    delay(500);
    SetClock(14, 46);
    Serial.println("Nano Ready!");
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

void loop () {
  delay(500);
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

void DisplayTime(int x, int y, int size, bool showDots){
  display.clearDisplay();
  display.setTextSize(size);
  display.setTextColor(WHITE);
  display.setCursor(x, y);


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
  display.println(myRTC.getMinute(), DEC);
  
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
