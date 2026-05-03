// Board checker. Run this, if there is no blinking, your board is ded (T_T)[jk, could just be a blown led or loose connection]
/*fine LED_PIN 2   

void setup() {
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  delay(1000);                
  digitalWrite(LED_PIN, LOW);  
  delay(1000);                 
}*/


//PWM Testing
/*
#define PWM_PIN 34 

volatile uint32_t startTime = 0;
volatile uint32_t pulseWidth = 0;

void IRAM_ATTR handlePWM() {
  if (digitalRead(PWM_PIN) == HIGH) {
    startTime = micros();
  } else {
    pulseWidth = micros() - startTime;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(PWM_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(PWM_PIN), handlePWM, CHANGE);
}

void loop() {
  Serial.print("Pulse width: ");
  Serial.println(pulseWidth);
  delay(100);
}*/


//MPU6050 testing(do you see graphs?)

#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

void setup() {
  Serial.begin(115200);

  
  Wire.begin(21, 22);
  Wire.setClock(400000);

  mpu.initialize();

 
}

void loop() {
  int16_t ax, ay, az, gx, gy, gz;

  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Convert accl to g
  float ax_g = ax / 16384.0;
  float ay_g = ay / 16384.0;
  float az_g = az / 16384.0;

 
  Serial.print(ax_g);
  Serial.print(" ");
  Serial.print(ay_g);
  Serial.print(" ");
  Serial.println(az_g);

  delay(20);
}
