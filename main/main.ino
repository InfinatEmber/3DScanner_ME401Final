#include "motor.h"
#include "sensor.h"

#define PITCH_A 2
#define PITCH_B 20
#define PITCH_DIR 4
#define PITCH_PWM 3
#define PITCH_KP 1
#define PITCH_KI 0
#define PITCH_KD 0

#define YAW_A
#define YAW_B
#define YAW_DIR 34
#define YAW_PWM 5
#define YAW_KP 1
#define YAW_KI 0
#define YAW_KD 0

#define DIST A7

Motor pitch;
Motor yaw;
Sensor dist;

void setup() {
  Serial.begin(115200);

  pitch.setup(PITCH_A,PITCH_B,PITCH_DIR,PITCH_PWM,PITCH_KP,PITCH_KI,PITCH_KD);
  yaw.setup(YAW_A,YAW_B,YAW_DIR,YAW_PWM,YAW_KP,YAW_KI,YAW_KD)
  dist.setup(DIST);

  attatchCoreTimerService(time_loop);
}

void loop() {
  // put your main code here, to run repeatedly:

}

int counter = 0;//time in us
uint32_t time_loop(uint32_t time) {//time sensitive, low code impact
  pitch.update(counter);
  yaw.update(counter);
  dist.update(counter);

  
  counter+=10;
  return (time+CORE_TICK_RATE/100);
}
