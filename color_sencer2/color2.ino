#include <FastLED.h>
#define NUM_LEDS 1  //燈條數量 6顆燈
#define DATA_PIN 7  //燈條腳位

CRGB leds[NUM_LEDS];
CRGB ledshow[NUM_LEDS];

//存入顏色的陣列
int Red[1];
int Green[1];
int Blue[1];
int lightTime = 200;  //點亮LED的時間

//設定6顆燈RGB的黑白基準值
int whiteR[1];
int whiteG[1];
int whiteB[1];
int blackR[1];
int blackG[1];
int blackB[1];

void setup() {
  Serial.begin(115200);

  LEDS.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
  LEDS.addLeds<WS2812B, 8, RGB>(ledshow, NUM_LEDS);
  FastLED.setBrightness(255);

  pinMode(2, INPUT_PULLUP);  //黑色基準值的按鈕
  pinMode(3, INPUT_PULLUP);  //白色基準值的按鈕
  pinMode(4, INPUT_PULLUP);  //顏色館子校正
  pinMode(5, INPUT_PULLUP);  //顏色印出
}
void loop() {

  //如果白色基準值的按鈕壓下去
  if (digitalRead(3) == LOW) {

    //亮燈 將讀取到的數值存入白色基準值
    ledG();
    delay(lightTime);
    for (int i = 0; i < NUM_LEDS; i++)
      whiteG[i] = analogRead(A0);

    ledR();
    delay(lightTime);
    for (int i = 0; i < NUM_LEDS; i++)
      whiteR[i] = analogRead(A0);

    ledB();
    delay(lightTime);
    for (int i = 0; i < NUM_LEDS; i++)
      whiteB[i] = analogRead(A0);
    delay(lightTime);
  }

  //如果黑色基準值的按鈕壓下去
  if (digitalRead(2) == LOW) {

    //亮燈 將讀取到的數值存入黑色基準值
    ledG();
    delay(lightTime);
    for (int i = 0; i < NUM_LEDS; i++)
      blackG[i] = analogRead(A0);

    ledR();
    delay(lightTime);
    for (int i = 0; i < NUM_LEDS; i++)
      blackR[i] = analogRead(A0);

    ledB();
    delay(lightTime);
    for (int i = 0; i < NUM_LEDS; i++)
      blackB[i] = analogRead(A0);

    delay(lightTime);
  }

  ledR();
  delay(lightTime);
  for (int i = 0; i < NUM_LEDS; i++)
    Red[i] = constrain(map(analogRead(A0)-blackR[i],0 , whiteR[i]-blackR[i], 0, 255), 0, 255);


  ledG();
  delay(lightTime);
  for (int i = 0; i < NUM_LEDS; i++)
    Green[i] = constrain(map(analogRead(A0)-blackG[i], 0 , whiteG[i]-blackG[i], 0, 255), 0, 255);


  ledB();
  delay(lightTime);
  for (int i = 0; i < NUM_LEDS; i++)
    Blue[i] = constrain(map(analogRead(A0)-blackB[i], 0, whiteB[i]-blackB[i], 0, 255), 0, 255);



  ledOFF();  //關燈
  delay(500);
  Serial.print('r');
  Serial.print(Red[0]);
  Serial.print('g');
  Serial.print(Green[0]);
  Serial.print('b');
  Serial.print(Blue[0]);
  Serial.print('s');
  if (digitalRead(5) == LOW) {
    Serial.print(1);
  } else
    Serial.print(0);
  delay(500);


}
//顯示綠色燈
void ledG() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(255, 0, 0);
  }
  FastLED.show();
}

//顯示紅色燈
void ledR() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(0, 255, 0);
  }
  FastLED.show();
}

//顯示藍色燈
void ledB() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(0, 0, 255);
  }
  FastLED.show();
}

//關燈
void ledOFF() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(0, 0, 0);
  }
  FastLED.show();
}