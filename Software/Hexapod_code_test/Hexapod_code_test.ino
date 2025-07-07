#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// PCA9685 16채널 서보 드라이버
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
#define SERVOMIN   150
#define SERVOMAX   600
#define SERVO_FREQ 50

// 각 그룹 다리 수
const uint8_t GROUP_SIZE = 3;

// 그룹 A: 다리1-3-5 채널 (힙, 무릎, 발목)
const uint8_t HIP_A[GROUP_SIZE]   = {0, 3, 6};
const uint8_t KNEE_A[GROUP_SIZE]  = {1, 4, 7};
const uint8_t ANKLE_A[GROUP_SIZE] = {2, 5, 8};
// 그룹 B: 다리2-4-6 채널
const uint8_t HIP_B[GROUP_SIZE]   = {9, 12, 15};
const uint8_t KNEE_B[GROUP_SIZE]  = {10, 13, 16};
const uint8_t ANKLE_B[GROUP_SIZE] = {11, 14, 17};

// PWM 서보 각도 설정
void setAngle(uint8_t ch, float angle) {
  angle = constrain(angle, 0, 180);
  uint16_t pulse = map((int)angle, 0, 180, SERVOMIN, SERVOMAX);
  pwm.setPWM(ch, 0, pulse);
}

// 그룹 A에 각도 적용
void setGroupA(float hipAng, float kneeAng, float ankleAng) {
  for (uint8_t i = 0; i < GROUP_SIZE; i++) {
    setAngle(HIP_A[i],   hipAng);
    setAngle(KNEE_A[i],  kneeAng);
    setAngle(ANKLE_A[i], ankleAng);
  }
}
// 그룹 B에 각도 적용
void setGroupB(float hipAng, float kneeAng, float ankleAng) {
  for (uint8_t i = 0; i < GROUP_SIZE; i++) {
    setAngle(HIP_B[i],   hipAng);
    setAngle(KNEE_B[i],  kneeAng);
    setAngle(ANKLE_B[i], ankleAng);
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("Hexapod Dual-Group Control Ready");
  Serial.println("Press q: A=90/45/45, B=45/45/45");
  Serial.println("Press w: A=45/45/45, B=90/45/45");

  Wire.begin();
  pwm.begin();
  pwm.setPWMFreq(SERVO_FREQ);
  delay(10);

  // 초기 포즈: A group 90/45/45, B group 45/45/45
  setGroupA(90,45,45);
  setGroupB(45,45,45);
}

void loop() {
  if (!Serial.available()) return;
  char cmd = Serial.read();
  switch (cmd) {
    case 'q': case 'Q':
      setGroupA(90,45,45);
      setGroupB(45,45,45);
      Serial.println("Preset Q applied");
      break;
    case 'w': case 'W':
      setGroupA(45,45,45);
      setGroupB(90,45,45);
      Serial.println("Preset W applied");
      break;
    default:
      // ignore
      break;
  }
}
