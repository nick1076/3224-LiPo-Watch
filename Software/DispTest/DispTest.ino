//Headers
#include <Wire.h> //Custom i2c pinout library
#include <Adafruit_GFX.h> //Graphics library for display
#include <Adafruit_SSD1306.h> //Display library

//Vars
#define SCREEN_WIDTH 128 //Width of display used
#define SCREEN_HEIGHT 64 //Height of display used

bool initialized = false; //Tracks if screen initialized properly

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); //Screen

void setup() 
{
  //Device / pin setup
  Wire.begin(); //Setup custom i2c line on these pins

  //Serial setup
  Serial.begin(9600); //Start serial
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
    Display("Hello", 20, 20, 3);
    delay(5000);
    Clear();
    display.ssd1306_command(SSD1306_DISPLAYOFF);
  }
}

void loop() 
{
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
