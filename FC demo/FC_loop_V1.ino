#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

// PWM inp
#define PWM_PIN 34
volatile uint32_t startTime = 0;
volatile uint32_t pulseWidth = 1500;

void IRAM_ATTR pwmISR() {
  if (digitalRead(PWM_PIN)) {
    startTime = micros();
  } else {
    pulseWidth = micros() - startTime;
  }
}

//LED PWM SETUP (ESP32 v3.x) 
#define M1 32
#define M2 12
#define M3 15
#define M4 18

void setupPWM() {
  ledcAttach(M1, 5000, 8);
  ledcAttach(M2, 5000, 8);
  ledcAttach(M3, 5000, 8);
  ledcAttach(M4, 5000, 8);
}

// IMU VARIABLES 
float pitch = 0, roll = 0;
unsigned long prevTime = 0;

//  SETUP 
void setup() {
  Serial.begin(115200);

  // I2C
  Wire.begin(21, 22);
  Wire.setClock(400000);
  mpu.initialize();

  // PWM input
  pinMode(PWM_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(PWM_PIN), pwmISR, CHANGE);

  // LED PWM
  setupPWM();

  // Time init
  prevTime = micros();
}

//MAIN LOOP 
void loop() {

  //TIME STEP 
  float dt = (micros() - prevTime) / 1000000.0;
  prevTime = micros();

  //IMU read
  int16_t ax, ay, az, gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  //Convert accel
  float ax_g = ax / 16384.0;
  float ay_g = ay / 16384.0;
  float az_g = az / 16384.0;

  //Convert gyro
  float gx_dps = gx / 131.0;
  float gy_dps = gy / 131.0;

  //Accl. angles
  float pitch_acc = atan2(ay_g, az_g) * 180 / PI;
  float roll_acc  = atan2(-ax_g, az_g) * 180 / PI;

  //COMPLEMENTARY FILTER 
  pitch = 0.98 * (pitch + gx_dps * dt) + 0.02 * pitch_acc;
  roll  = 0.98 * (roll  + gy_dps * dt) + 0.02 * roll_acc;

  //Throttle inp 
  int throttle = map(pulseWidth, 1000, 2000, 0, 255);
  throttle = constrain(throttle, 0, 255);

  //STABILIZATION (P-CONTROL) 
  float Kp = 2.0;

  int pitch_correction = Kp * pitch;
  int roll_correction  = Kp * roll;

  //motor mixing
  int m1 = throttle - pitch_correction + roll_correction;
  int m2 = throttle - pitch_correction - roll_correction;
  int m3 = throttle + pitch_correction - roll_correction;
  int m4 = throttle + pitch_correction + roll_correction;

  //Constrain outputs
  m1 = constrain(m1, 0, 255);
  m2 = constrain(m2, 0, 255);
  m3 = constrain(m3, 0, 255);
  m4 = constrain(m4, 0, 255);

  //OUTPUT TO LEDS
  ledcWrite(M1, m1);
  ledcWrite(M2, m2);
  ledcWrite(M3, m3);
  ledcWrite(M4, m4);

  //DEBUG
  Serial.print(pitch); Serial.print(" ");
  Serial.print(roll);  Serial.print(" ");
  Serial.print(m1); Serial.print(" ");
  Serial.print(m2); Serial.print(" ");
  Serial.print(m3); Serial.print(" ");
  Serial.println(m4);

  delay(10);
}