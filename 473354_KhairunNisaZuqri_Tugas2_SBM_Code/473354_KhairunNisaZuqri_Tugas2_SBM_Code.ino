#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

const int OLED_RESET = -1; 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define ROWS 4
#define COLS 4

char rowPins[ROWS] = {33, 32, 25, 17};
char colPins[COLS] = {18, 5, 4, 15};

char alphaChars[10][7] = {
  {'0', '0', '0', '0', '0', '0', '\0'}, 
  {'.', ',', '?', '!', '&', '1', '\0'}, 
  {'/', 'a', 'b', 'c', '2', ' ', '\0'}, 
  {'+', 'd', 'e', 'f', '3', ' ', '\0'}, 
  {'-', 'g', 'h', 'i', '4', ' ', '\0'}, 
  {'=', 'j', 'k', 'l', '5', ' ', '\0'}, 
  {'*', 'm', 'n', 'o', '6', ' ', '\0'}, 
  {'%', 'p', 'q', 'r', 's', '7', '\0'}, 
  {'@', 't', 'u', 'v', '8', ' ', '\0'}, 
  {'~', 'w', 'x', 'y', 'z', '9', '\0'}  
};

char keyMap[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

unsigned long lastPressed[ROWS][COLS] = {
  {0, 0, 0, 0},
  {0, 0, 0, 0},
  {0, 0, 0, 0},
  {0, 0, 0, 0}
};

const int MAX_STRING_LENGTH = 20;
char currentString[MAX_STRING_LENGTH]; 

int len = 0;
int indeks = 0;
bool capslock = false;

unsigned long debounceTime = 100;
unsigned long lastPressTime = 0;

void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display(); 
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(15,15);
  display.print("Keypad");  
  display.display();

  // Inisialisasi keypad
  for (char i = 0; i < ROWS; i++) {
    pinMode(rowPins[i], INPUT_PULLUP);
  }
  for (char j = 0; j < COLS; j++) {
    pinMode(colPins[j], OUTPUT);
    digitalWrite(colPins[j], HIGH);
  }
}

void loop() {
  char key = readKeypad();
  char i, j;
  char ch;

  for (char i = 0; i < 4; i++) {
    for (char j = 0; j < 4; j++) {
      if (keyMap[i][j] == key) {
      }
    }
  }
  
  if (key != '\0' && millis() - lastPressTime >= debounceTime) {
    lastPressTime = millis();
    if (key >= '0' && key <= '9') { 
      int click = key - '0';   
      if (millis() - lastPressed[i][j] < 700) {  
        if (indeks == 0){
          indeks++;
          ch = alphaChars[click][indeks];
        } else if (indeks == 1){
          indeks++;
          ch = alphaChars[click][indeks];  
        } else if (indeks == 2){
          indeks++;
          ch = alphaChars[click][indeks];  
        } else if (indeks == 3){
          indeks++;
          ch = alphaChars[click][indeks];  
        } else if (indeks == 4){
          if ((click == 1) || (click == 7) || (click == 9)){
            indeks++;
            ch = alphaChars[click][indeks];
          } else {
            indeks = 0;
            ch = alphaChars[click][indeks];  
          }         
        } else if (indeks == 5){
          indeks = 0;
          ch = alphaChars[click][indeks];  
        }
        if (capslock) {
          ch = toupper(ch);        
        }
        currentString[len - 1] = ch;   
      } else {   
        if (capslock) {
          ch = toupper(ch);        
        }
        ch = alphaChars[click][0];    
        currentString[len] = ch;
        len++;        
      }
      lastPressed[i][j] = millis();
      if (len < MAX_STRING_LENGTH) {        
        display.clearDisplay();
        display.setCursor(0, 0);
        display.print("Enter text:");
        display.print(currentString);
        display.display();
      }
      if (capslock) {
        ch = toupper(ch);        
      }
    } else if (key == '*') {
      // Reset 
      memset(currentString, 0, sizeof(currentString));
      len = 0;
      display.clearDisplay();
      display.setCursor(0, 0);
      display.print("Enter text:");
      display.display();
    } 
  }
}

char readKeypad() {
  char currentKey = '\0';
  for (char j = 0; j < COLS; j++) {
    digitalWrite(colPins[j], LOW);
    for (char i = 0; i < ROWS; i++) {
      if (digitalRead(rowPins[i]) == LOW) {                  
        while (digitalRead(rowPins[i]) == LOW){}
        // CapsLock 
        if (keyMap[i][j] == 'A') {
          capslock = !capslock;
        } 
        // Space 
        else if (keyMap[i][j] == 'B' && len < MAX_STRING_LENGTH) {
          currentString[len] = ' ';
          len++;
          currentString[len] = '\0';
        }
        // Enter 
        else if (keyMap[i][j] == 'C') {
          currentString[len] = '\n';
          len++;
          currentString[len] = '\0';
        }
        // Backspace 
        else if (keyMap[i][j] == 'D') {
          if (len > 0) {
            len--;
            currentString[len] = '\0';
            display.clearDisplay();
            display.setCursor(0, 0);
            display.print("Enter text:");
            display.print(currentString);
            display.display();
          }
        }
        else {
          currentKey = keyMap[i][j];
          currentKey = capslock ? toupper(keyMap[i][j]) : keyMap[i][j];
        }
      }
    }
    digitalWrite(colPins[j], HIGH);
  }
  return currentKey;
}