#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

// Display constructor
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset= */ U8X8_PIN_NONE);

const int piezoPin = 5;

// RGB light pins
const int redPin = 4;
const int greenPin = 3;
const int bluePin = 2;

const int upPin = 6;      
const int downPin = 7;    
const int leftPin = 8;    
const int rightPin = 9;   
const int centerPin = 10; 


const char passcor[] = "5555"; // Enter your password here
char passins[sizeof(passcor)] = {}; // Password inserted in runtime
byte passhead = 0; // number of last character of inserted password

void setup() {
  pinMode(upPin, INPUT);
  pinMode(downPin, INPUT);
  pinMode(leftPin, INPUT);
  pinMode(rightPin, INPUT);
  pinMode(centerPin, INPUT);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  u8g2.begin(/*Select=*/ 10, /*Right/Next=*/ 9, /*Left/Prev=*/ 8, /*Up=*/ 7, /*Down=*/ 6, /*Home/Cancel=*/ U8X8_PIN_NONE); 
}

// append symbol to the password
void passappend(char symbol) { 
      tone(piezoPin, 3000);
      digitalWrite(bluePin, HIGH);
      delay(250);
      noTone(piezoPin);
      digitalWrite(bluePin, LOW);
      passins[passhead] = symbol;
      passhead = passhead + 1;
}

void loop() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_helvB14_tr);
  u8g2.drawStr(0,15,"PASSYSTEM");
  u8g2.sendBuffer();

  delay(1000);
  tone(piezoPin, 3500); 
  digitalWrite(greenPin, HIGH);
  delay(250);
  tone(piezoPin, 2000);
  digitalWrite(bluePin, HIGH);
  delay(250);
  tone(piezoPin, 5000);
  digitalWrite(redPin, HIGH);
  delay(400);
  noTone(piezoPin);
  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, LOW);
  digitalWrite(redPin, LOW);
  delay(300);
  u8g2.setFont(u8g2_font_7x14_tr);
  u8g2.drawStr(0,42, "Hardware password");
  u8g2.drawStr(0,57, "manager");
  u8g2.sendBuffer();
  delay(700);

  passinput:
  memset(passins, 0, sizeof(passins));
  passhead = 0;

  u8g2.clearBuffer();
  u8g2.drawStr(0,15,"Enter password:");
  u8g2.sendBuffer();
  while (passhead < sizeof(passcor)-1) {
    if (u8g2.getMenuEvent() == "U8X8_MSG_GPIO_MENU_SELECT") {
      passappend('1');
    } else if (u8g2.getMenuEvent() == "U8X8_MSG_GPIO_MENU_NEXT") {
      passappend('2');
    } else if (u8g2.getMenuEvent() == "U8X8_MSG_GPIO_MENU_PREV") {
      passappend('3');     
    } else if (u8g2.getMenuEvent() == "U8X8_MSG_GPIO_MENU_UP") {
      passappend('4');      
    } else if (u8g2.getMenuEvent() == "U8X8_MSG_GPIO_MENU_DOWN") {
      passappend('5');     
    } else if (u8g2.getMenuEvent() == "U8X8_MSG_GPIO_MENU_HOME") {
      passappend('6');      
    } 
  }
  passins[passhead] = '\0';
  if (memcmp(passcor, passins, sizeof(passcor)) == 0) {
    u8g2.clearBuffer();
    u8g2.drawStr(0,15,"Login correct!");
    u8g2.sendBuffer();  

    digitalWrite(greenPin, HIGH);
    tone(piezoPin, 2000);
    delay(100); 
    noTone(piezoPin);
    delay(50); 
    tone(piezoPin, 3000);
    delay(100); 
    digitalWrite(greenPin, LOW);
    noTone(piezoPin);
    delay(500);
  } else {
    u8g2.clearBuffer();
    u8g2.drawStr(0,15,"Login failed.");
    u8g2.drawStr(0,30,"Try again.");
    u8g2.sendBuffer();

    digitalWrite(redPin, HIGH);
    tone(piezoPin, 1000);
    delay(100); 
    digitalWrite(redPin, LOW);
    noTone(piezoPin);
    delay(50); 
    digitalWrite(redPin, HIGH);
    tone(piezoPin, 1000);
    delay(100); 
    digitalWrite(redPin, LOW);
    noTone(piezoPin);
    delay(500);
    goto passinput;
  }

  u8g2.userInterfaceSelectionList("Menu", 1, "abcdef\nghijkl\nmnopqr");
}
