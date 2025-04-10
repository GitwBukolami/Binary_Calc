
#include <Keypad.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

// TFT LCD pins
#define TFT_CS 10
#define TFT_DC 9
#define TFT_RST 8

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// Keypad config
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  { '1', '2', '3', '+' },
  { '4', '5', '6', '-' },
  { '7', '8', '9', 'C' },
  { '*', '0', '=', '/' }
};
byte rowPins[ROWS] = { A0, A1, A2, A3 };
byte colPins[COLS] = { 5, 4, 3, 2 };

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Variables
String input1 = "";
String input2 = "";
char operation = 0;
bool isSecond = false;

void setup() {
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_GREEN);
  tft.setTextSize(2);
  tft.setCursor(0, 0);
  tft.println("BinaryCalc");

  Serial.begin(9600);
}

void loop() {
  char key = keypad.getKey();
  // --- Serial Input Handling ---
  if (Serial.available()) {
    char sKey = Serial.read();

    if (sKey == 'C') {
      input1 = "";
      input2 = "";
      operation = 0;
      isSecond = false;
      displayText("Cleared");
      Serial.println("Cleared");
    } else if (sKey == '+' || sKey == '-' || sKey == '*' || sKey == '/') {
      if (!input1.equals("")) {
        operation = sKey;
        isSecond = true;
        displayText(String(operation));
        Serial.println(String("Op: ") + operation);
      }
    } else if (sKey == '=') {
      if (!input1.equals("") && !input2.equals("") && operation != 0) {
        long num1 = strtol(input1.c_str(), NULL, 2);
        long num2 = strtol(input2.c_str(), NULL, 2);
        long result = 0;
        bool valid = true;

        switch (operation) {
          case '+': result = num1 + num2; break;
          case '-': result = num1 - num2; break;
          case '*': result = num1 * num2; break;
          case '/':
            if (num2 != 0) result = num1 / num2;
            else valid = false;
            break;
        }

        if (valid) {
          String resStr = String(result, BIN);
          displayText("= " + resStr);
          Serial.println(String("= ") + resStr);
          input1 = resStr;
        } else {
          displayText("Err: Div0");
          Serial.println("Error: Divide by 0");
          input1 = "";
        }

        input2 = "";
        operation = 0;
        isSecond = false;
      }
    } else if (sKey == '0' || sKey == '1') {
      if (!isSecond) {
        input1 += sKey;
        displayText(input1);
        Serial.println("Num1: " + input1);
      } else {
        input2 += sKey;
        displayText(input2);
        Serial.println("Num2: " + input2);
      }
    }
  }

  if (key) {
    if (key == 'C') {
      input1 = "";
      input2 = "";
      operation = 0;
      isSecond = false;
      displayText("Cleared");
    } else if (key == '+' || key == '-' || key == '*' || key == '/') {
      if (!input1.equals("")) {
        operation = key;
        isSecond = true;
        displayText(String(operation));
      }
    } else if (key == '=') {
      if (!input1.equals("") && !input2.equals("") && operation != 0) {
        long num1 = strtol(input1.c_str(), NULL, 2);
        long num2 = strtol(input2.c_str(), NULL, 2);
        long result = 0;
        bool valid = true;

        switch (operation) {
          case '+': result = num1 + num2; break;
          case '-': result = num1 - num2; break;
          case '*': result = num1 * num2; break;
          case '/':
            if (num2 != 0) result = num1 / num2;
            else valid = false;
            break;
        }

        if (valid) {
          String resStr = String(result, BIN);
          displayText("= " + resStr);
          input1 = resStr;
        } else {
          displayText("Err: Div0");
          input1 = "";
        }

        input2 = "";
        operation = 0;
        isSecond = false;
      }
    } else if (key >= '0' && key <= '1') {
      if (!isSecond) {
        input1 += key;
        displayText(input1);
      } else {
        input2 += key;
        displayText(input2);
      }
    }
  }
}

void displayText(String text) {
  tft.fillRect(0, 30, 128, 40, ST77XX_BLACK);
  tft.setCursor(0, 30);
  tft.print(text);
}
