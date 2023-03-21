//Khairun Nisa' Zuqri
//21/473354/TK/52162

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

const int OLED_RESET = -1; 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int upBtnPin = 18;
const int downBtnPin = 19;

int count = 0;

int upBtnState;
int downBtnState;
int lastUpBtnState;
int lastDownBtnState;

unsigned long debounceTime = 60;
unsigned long lastDebounceTime = 0; 
int lastDebounceTime_up;
int lastDebounceTime_down;

void setup() {
  Serial.begin(9600);
  pinMode(upBtnPin, INPUT_PULLUP);
  pinMode(downBtnPin, INPUT_PULLUP);
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(15,15);
  display.print("Counter");  
  display.display();
}

void loop() {
  upBtnState = digitalRead(upBtnPin);
  downBtnState = digitalRead(downBtnPin);
  
  if ((lastUpBtnState != lastDebounceTime_up) || (lastDownBtnState != lastDebounceTime_down)) {
    debounceTime = millis();
    lastDebounceTime_up = lastUpBtnState;
    lastDebounceTime_down = lastDownBtnState;    
  }
  
  if ((millis() - lastDebounceTime) > debounceTime){
    if (upBtnState == LOW && lastUpBtnState == HIGH) {
      count++;
      display.display();      
      display.clearDisplay();
      display.setCursor(15,15);
      display.print("Count:");
      display.setCursor(90,15);
      display.print(count);
      display.display();
    }

    if (downBtnState == LOW && lastDownBtnState == HIGH && count != 0) {
      count--;
      display.display();
      display.clearDisplay();
      display.setCursor(15,15);
      display.print("Count:");
      display.setCursor(90,15);
      display.print(count);
      display.display();
    }    
    
    if (upBtnState == LOW && downBtnState == LOW) {
      count = 0;
      display.display();      
      display.clearDisplay();
      display.setCursor(15,15);
      display.print("Count:");
      display.setCursor(90,15);
      display.print(count);
      display.display();
    }    
  }  

  lastUpBtnState = upBtnState;
  lastDownBtnState = downBtnState;
} 
