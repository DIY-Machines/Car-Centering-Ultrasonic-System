#include <SPI.h>
#include <MD_MAX72xx.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

#define DATA_PIN 11
#define CLK_PIN  13
#define CS_PIN   10

MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

// Ultrasonic pins
#define LEFT_TRIG   2
#define LEFT_ECHO   3
#define RIGHT_TRIG  4
#define RIGHT_ECHO  5

const int toleranceMm = 15;
const int minValidMm = 50;
const int maxValidMm = 1500;
const int samples = 5;
const int calibrationOffsetMm = 0;

// 8x8 left arrow
const uint8_t arrowLeft[8] = {
  B00011000,
  B00001100,
  B11111110,
  B11111111,
  B11111111,
  B11111110,
  B00001100,
  B00011000
};

// 8x8 right arrow
const uint8_t arrowRight[8] = {
  B00011000,
  B00110000,
  B01111111,
  B11111111,
  B11111111,
  B01111111,
  B00110000,
  B00011000
};

unsigned long readDistanceMm(uint8_t trigPin, uint8_t echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(3);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  unsigned long duration = pulseIn(echoPin, HIGH, 25000UL);
  if (duration == 0) return 0;

  return (duration * 343UL) / 2000UL;
}

int readAveragedDistanceMm(uint8_t trigPin, uint8_t echoPin) {
  unsigned long total = 0;
  int count = 0;

  for (int i = 0; i < samples; i++) {
    unsigned long d = readDistanceMm(trigPin, echoPin);
    if (d >= minValidMm && d <= maxValidMm) {
      total += d;
      count++;
    }
    delay(25);
  }

  if (count == 0) return -1;
  return total / count;
}

void clearDisplay() {
  mx.clear();
}

// Reverse the whole display horizontally
uint8_t mapCol(uint8_t col) {
  return 31 - col;
}

void setPixelMapped(uint8_t row, uint8_t col, bool state) {
  mx.setPoint(row, mapCol(col), state);
}

void drawBitmap8x8(uint8_t colStart, const uint8_t bitmap[8]) {
  for (uint8_t row = 0; row < 8; row++) {
    for (uint8_t col = 0; col < 8; col++) {
      bool pixelOn = bitRead(bitmap[row], 7 - col);
      setPixelMapped(row, colStart + col, pixelOn);
    }
  }
}

void drawCharAt(uint8_t colStart, char c) {
  uint8_t buffer[8];
  uint8_t width = mx.getChar(c, sizeof(buffer) / sizeof(buffer[0]), buffer);

  for (uint8_t i = 0; i < width; i++) {
    uint8_t thisCol = colStart + i;
    for (uint8_t row = 0; row < 8; row++) {
      bool pixelOn = bitRead(buffer[i], row);
      setPixelMapped(row, thisCol, pixelOn);
    }
  }
}

void drawTextAt(uint8_t colStart, const char* text) {
  while (*text) {
    drawCharAt(colStart, *text);
    colStart += 6; // 5 columns plus 1 space
    text++;
  }
}

void fillEntireDisplay() {
  for (uint8_t row = 0; row < 8; row++) {
    for (uint8_t col = 0; col < 32; col++) {
      setPixelMapped(row, col, true);
    }
  }
}

void drawOuterBorder() {
  for (uint8_t col = 0; col < 32; col++) {
    setPixelMapped(0, col, true);
    setPixelMapped(7, col, true);
  }

  for (uint8_t row = 0; row < 8; row++) {
    setPixelMapped(row, 0, true);
    setPixelMapped(row, 31, true);
  }
}

void showNoCar(bool flashOn) {
  clearDisplay();
  if (flashOn) {
    drawOuterBorder();
  }
  mx.update();
}

void showCentred() {
  clearDisplay();
  fillEntireDisplay();
  mx.update();
}

void showMoveLeft(int mm) {
  clearDisplay();
  drawBitmap8x8(0, arrowLeft);

  char buf[5];
  int shown = mm;
  if (shown > 999) shown = 999;
  itoa(shown, buf, 10);

  drawTextAt(10, buf);
  mx.update();
}

void showMoveRight(int mm) {
  clearDisplay();

  char buf[5];
  int shown = mm;
  if (shown > 999) shown = 999;
  itoa(shown, buf, 10);

  drawTextAt(0, buf);
  drawBitmap8x8(24, arrowRight);
  mx.update();
}

void startupScreen() {
  clearDisplay();
  drawTextAt(1, "START");
  mx.update();
  delay(800);
}

void setup() {
  pinMode(LEFT_TRIG, OUTPUT);
  pinMode(LEFT_ECHO, INPUT);
  pinMode(RIGHT_TRIG, OUTPUT);
  pinMode(RIGHT_ECHO, INPUT);

  mx.begin();
  mx.control(MD_MAX72XX::INTENSITY, 2);
  mx.clear();

  startupScreen();
}

void loop() {
  static bool flashState = false;
  static unsigned long lastFlashToggle = 0;
  const unsigned long flashIntervalMs = 300;

  int leftDist = readAveragedDistanceMm(LEFT_TRIG, LEFT_ECHO);
  int rightDist = readAveragedDistanceMm(RIGHT_TRIG, RIGHT_ECHO);

  if (leftDist < 0 || rightDist < 0) {
    unsigned long now = millis();
    if (now - lastFlashToggle >= flashIntervalMs) {
      flashState = !flashState;
      lastFlashToggle = now;
    }

    showNoCar(flashState);
    delay(50);
    return;
  }

  int diff = (leftDist - rightDist) - calibrationOffsetMm;

  if (abs(diff) <= toleranceMm) {
    showCentred();
  } else if (diff > 0) {
    showMoveLeft(abs(diff));
  } else {
    showMoveRight(abs(diff));
  }

  delay(120);
}