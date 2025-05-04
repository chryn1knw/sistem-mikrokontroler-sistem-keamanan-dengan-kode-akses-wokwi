#include <Keypad.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

const uint8_t ROWS = 4;
const uint8_t COLS = 3;

char keys[ROWS][COLS] = {
  { '1', '2', '3' },
  { '4', '5', '6'},
  { '7', '8', '9' },
  { '*', '0', '#' }
};

uint8_t colPins[COLS] = { 5, 4, 3 }; // Pins connected to C1, C2, C3, C4
uint8_t rowPins[ROWS] = { 9, 8, 7, 6 }; // Pins connected to R1, R2, R3, R4

LiquidCrystal_I2C lcd(0x27, 16, 2); // Ganti 0x27 jika alamat berbeda

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

Servo doorServo; // Create a "Servo" object called "doorServo"
float openAngle = 90;  // Sudut pintu terbuka
float closeAngle = 0;  // Sudut pintu tertutup

String inputCode = "";
String validCode = "1234";

void setup() {
  // put your setup code here, to run once:
  lcd.init();       
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Input Code:");

  doorServo.attach(11);
  doorServo.write(closeAngle);
}

void loop() {
  char key = keypad.getKey();

  if (key != NO_KEY) {
    if (key == '*') {
      if (inputCode.length() > 0) {
        inputCode.remove(inputCode.length() - 1);
        lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor(0, 1);
        lcd.print(inputCode);
      }
    }
    else if (key == '#') {
      lcd.clear();
      if (inputCode == validCode) {
        lcd.setCursor(0, 0);
        lcd.print("Access Granted");
        doorServo.write(openAngle);
        delay(2100);
        doorServo.write(closeAngle);
      } 
      else {
        lcd.setCursor(0, 0);
        lcd.print("Access Denied ");
        doorServo.write(closeAngle);
      }

      delay(1800);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Input Code:");
      inputCode = "";
      lcd.setCursor(0, 1);
      lcd.print("                ");
    }
    else {
      inputCode += key;
      lcd.setCursor(0, 1);
      lcd.print(inputCode);
    }
  }
}
