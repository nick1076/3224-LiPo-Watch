// now.pde
// Prints a snapshot of the current date and time along with the UNIX time
// Modified by Andy Wickert from the JeeLabs / Ladyada RTC library examples
// 5/15/11

#include <Wire.h>
#include <DS3231.h>

DS3231 myRTC;

bool century = false;
bool h12Flag;
bool pmFlag;
byte alarmDay, alarmHour, alarmMinute, alarmSecond, alarmBits;
bool alarmDy, alarmH12Flag, alarmPmFlag;

void setup () {
    Serial.begin(57600);
    Wire.begin();
    delay(500);
    SetClock();
    Serial.println("Nano Ready!");
}

void SetClock(){
    myRTC.setClockMode(false);  // set to 24h
    //setClockMode(true); // set to 12h
    
    myRTC.setYear(25);
    myRTC.setMonth(8);
    myRTC.setDate(14);
    myRTC.setDoW(4);
    myRTC.setHour(14);
    myRTC.setMinute(22);
    myRTC.setSecond(45);

    /*
    // Test of alarm functions
    // set A1 to one minute past the time we just set the clock
    // on current day of week.
    myRTC.setA1Time(dOW, hour, minute+1, second, 0x0, true,
                    false, false);
    // set A2 to two minutes past, on current day of month.
    myRTC.setA2Time(date, hour, minute+2, 0x0, false, false,
                    false);
    // Turn on both alarms, with external interrupt
    myRTC.turnOnAlarm(1);
    myRTC.turnOnAlarm(2);*/
}

void loop () {

    
    delay(1000);/*
  Serial.print(myRTC.getYear(), DEC);
  Serial.print(' ');
  
  // then the month
  Serial.print(myRTC.getMonth(century), DEC);
  Serial.print(" ");
  
  // then the date
  Serial.print(myRTC.getDate(), DEC);
  Serial.print(" ");
  
  // and the day of the week
  Serial.print(myRTC.getDoW(), DEC);
  Serial.print(" ");
  */
  // Finally the hour, minute, and second
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

  /*
 
  // Display the temperature
  Serial.print("T=");
  Serial.print(myRTC.getTemperature(), 2);
  
  // Tell whether the time is (likely to be) valid
  if (myRTC.oscillatorCheck()) {
    Serial.print(" O+");
  } else {
    Serial.print(" O-");
  }
 
  // Indicate whether an alarm went off
  if (myRTC.checkIfAlarm(1)) {
    Serial.print(" A1!");
  }
 
  if (myRTC.checkIfAlarm(2)) {
    Serial.print(" A2!");
  }
 
  // New line on display
  Serial.println();
  
  // Display Alarm 1 information
  Serial.print("Alarm 1: ");
  myRTC.getA1Time(alarmDay, alarmHour, alarmMinute, alarmSecond, alarmBits, alarmDy, alarmH12Flag, alarmPmFlag);
  Serial.print(alarmDay, DEC);
  if (alarmDy) {
    Serial.print(" DoW");
  } else {
    Serial.print(" Date");
  }
  Serial.print(' ');
  Serial.print(alarmHour, DEC);
  Serial.print(' ');
  Serial.print(alarmMinute, DEC);
  Serial.print(' ');
  Serial.print(alarmSecond, DEC);
  Serial.print(' ');
  if (alarmH12Flag) {
    if (alarmPmFlag) {
      Serial.print("pm ");
    } else {
      Serial.print("am ");
    }
  }
  if (myRTC.checkAlarmEnabled(1)) {
    Serial.print("enabled");
  }
  Serial.println();
 
  // Display Alarm 2 information
  Serial.print("Alarm 2: ");
  myRTC.getA2Time(alarmDay, alarmHour, alarmMinute, alarmBits, alarmDy, alarmH12Flag, alarmPmFlag);
  Serial.print(alarmDay, DEC);
  if (alarmDy) {
    Serial.print(" DoW");
  } else {
    Serial.print(" Date");
  }
  Serial.print(" ");
  Serial.print(alarmHour, DEC);
  Serial.print(" ");
  Serial.print(alarmMinute, DEC);
  Serial.print(" ");
  if (alarmH12Flag) {
    if (alarmPmFlag) {
      Serial.print("pm");
    } else {
      Serial.print("am");
    }
  }
  if (myRTC.checkAlarmEnabled(2)) {
    Serial.print("enabled");
  }
 
  // display alarm bits
  Serial.println();
  Serial.print("Alarm bits: ");
  Serial.println(alarmBits, BIN);

  Serial.println();
  */
}
