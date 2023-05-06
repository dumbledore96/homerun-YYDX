#include <AccelStepper.h>

// 馬達腳位常數
const int enablePin = 8; // 驅動版啟用引腳
const int xdirPin = 5;   // x方向控制引腳
const int xstepPin = 2;  // x步進控制引腳
const int ydirPin = 6;   // y方向控制引腳
const int ystepPin = 3;  // y步進控制引腳
const int zdirPin = 7;   // z方向控制引腳
const int zstepPin = 4;  // z步進控制引腳
const int adirPin = 13;  // a方向控制引腳
const int astepPin = 12; // a步進控制引腳

// 步進馬達變數
AccelStepper stepper1(1, xstepPin, xdirPin);  
AccelStepper stepper2(1, ystepPin, ydirPin);  
AccelStepper stepper3(1, zstepPin, zdirPin);  
AccelStepper stepper4(1, astepPin, adirPin);  

void setup() {
  Serial.begin(115200);
  pinMode(enablePin, OUTPUT);  
  pinMode(xstepPin, OUTPUT);   
  pinMode(xdirPin, OUTPUT);    
  pinMode(ystepPin, OUTPUT);   
  pinMode(ydirPin, OUTPUT);    
  pinMode(zstepPin, OUTPUT);   
  pinMode(zdirPin, OUTPUT);    
  pinMode(astepPin, OUTPUT);   
  pinMode(adirPin, OUTPUT);    

  digitalWrite(enablePin, HIGH);  //將控制引腳設為低電位

  // 設置馬達最大速度300、初始速度0
  stepper1.setMaxSpeed(300.0);  
  stepper1.setSpeed(0.0);
  stepper2.setMaxSpeed(300.0);  
  stepper2.setSpeed(0.0);
  stepper3.setMaxSpeed(300.0);  
  stepper3.setSpeed(0.0);
  stepper4.setMaxSpeed(300.0);  
  stepper4.setSpeed(0.0);
}

double motorspeed = -150.0;
bool getsi = 0; // 馬達運轉標記
int circ, cirm, ciry, cirk; // 馬達圈數變數

void loop() {
  int r, g, b, s;
  char rc, gc, bc, pc, sc;

  // 接收主端數值
  while (!Serial.available()) {};
  if (Serial.available()) {
    rc = Serial.read();
    while (rc!='r') {
      rc=Serial.read();
    }
    r = Serial.parseInt();
    gc = Serial.read();
    g = Serial.parseInt();
    bc = Serial.read();
    b = Serial.parseInt();
    sc = Serial.read();
    s = Serial.parseInt();
    if (s == 1)
      getsi = 1;
    
    // rgb轉cmyk
    double R = r * 1.0 / 255.0, G = g * 1.0 / 255.0, B = b * 1.0 / 255.0;
    double M = max(max(R, G), B);
    double k = 1.0 - M, c = 1.0 - R * 1.0 / M, m = 1.0 - G * 1.0 / M, y = 1.0 - B * 1.0 / M;
    c *= 100.0;
    m *= 100.0;
    y *= 100.0;
    k *= 100.0;

    // 輸出數值
    Serial.println("rgb code:");
    Serial.print("r: ");
    Serial.print(rc);
    Serial.println(r);
    Serial.print("g: ");
    Serial.print(gc);
    Serial.println(g);
    Serial.print("b: ");
    Serial.print(bc);
    Serial.println(b);

    Serial.println("cmyk code:");
    Serial.print("c: ");
    Serial.println(c);
    Serial.print("m: ");
    Serial.println(m);
    Serial.print("y: ");
    Serial.println(y);
    Serial.print("k: ");
    Serial.println(k);
    Serial.println();

    // 接收到運轉訊號
    if (getsi == 1) {
      Serial.println("Start color pumping.");
      double sum = c + y + m + k;
      circ = c * 20.0 / sum;
      cirm = m * 20.0 / sum;
      ciry = y * 20.0 / sum;
      cirk = k * 20.0 / sum;
      Serial.print("color c cycles: ");
      Serial.println(circ);
      Serial.print("color m cycles: ");
      Serial.println(cirm);
      Serial.print("color y cycles: ");
      Serial.println(ciry);
      Serial.print("color k cycles: ");
      Serial.println(cirk);

      // 抽取顏料
      digitalWrite(enablePin, LOW);
      stepper1.setCurrentPosition(0);
      while (stepper1.currentPosition() <= 200 * circ) {
        stepper1.setSpeed((-1)*motorspeed);
        stepper1.runSpeed();
      }
      stepper1.setSpeed(0.0);

      stepper2.setCurrentPosition(0);
      while (stepper2.currentPosition() <= 200 * cirm) {
        stepper2.setSpeed((-1)*motorspeed);
        stepper2.runSpeed();
      }
      stepper2.setSpeed(0.0);

      stepper3.setCurrentPosition(0);
      while (stepper3.currentPosition() >= -200 * ciry) {
        stepper3.setSpeed(motorspeed);
        stepper3.runSpeed();
      }
      stepper3.setSpeed(0.0);

      stepper4.setCurrentPosition(0);
      while (stepper4.currentPosition() >= -200 * cirk) {
        stepper4.setSpeed(motorspeed);
        stepper4.runSpeed();
      }
      stepper4.setSpeed(0.0);

      Serial.println("pumping complete.");
    }
    digitalWrite(enablePin, HIGH);
    getsi = 0;
  }
}