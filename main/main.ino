//set JP4 to RD4
//set JP1 to 5
//set JP2 to 34
//pin 3 resitor to ground

#include "motor.h"
#include "sensor.h"
#include "switch.h"

#define GEAR_BIG 40
#define GEAR_SMALL 20
#define MOTOR_INDICES 2800
#define NUM 100

#define MIN_PITCH -MOTOR_INDICES/2
#define MAX_PITCH 0
#define IND_PITCH (MAX_PITCH-MIN_PITCH)/NUM

#define MIN_YAW 0
#define MAX_YAW MOTOR_INDICES/GEAR_SMALL*GEAR_BIG/2
#define IND_YAW (MAX_YAW-MIN_YAW)/NUM

#define PITCH_A 2
#define PITCH_B 20
#define PITCH_DIR 4
#define PITCH_PWM 3
#define PITCH_KP 1.2
#define PITCH_KI 6.56
#define PITCH_KD 0.05
#define PITCH_REVERSED false
#define PITCH_ERROR 1

#define YAW_A 7
#define YAW_B 37
#define YAW_DIR 34
#define YAW_PWM 5
#define YAW_KP 1.5
#define YAW_KI 6.59
#define YAW_KD 0.085
#define YAW_REVERSED true
#define YAW_ERROR 10

#define DIST A7

#define ZERO 41

Motor pitch;
Motor yaw;
Sensor dist;
Switch zero;

void setup() {
  Serial.begin(115200);

  Serial.println("Setting up objects.");
  pitch.setup(PITCH_A,PITCH_B,PITCH_DIR,PITCH_PWM,PITCH_KP,PITCH_KI,PITCH_KD,PITCH_REVERSED,PITCH_ERROR);
  yaw.setup(YAW_A,YAW_B,YAW_DIR,YAW_PWM,YAW_KP,YAW_KI,YAW_KD,YAW_REVERSED,YAW_ERROR);
  dist.setup(DIST);
  zero.setup(ZERO);

  Serial.println("Setting up time loop.");
  attachCoreTimerService(time_loop);

  Serial.println("Zeroing the pitch.");
  //zero pitch
  int sp = 0;
  while(!zero.pressed()){
    sp++;
    pitch.set_setpoint(sp);
    pitch.wait_position();
  }
  pitch.set_position(0);
  pitch.set_setpoint(0);
  Serial.println("End setting up.");
}

void loop() {
  Serial.println("Beggining scan.");
  for(int y = MIN_YAW;y <= MAX_YAW; y+=IND_YAW) {
    for(int p = MIN_PITCH; p <= MAX_PITCH; p+=IND_PITCH) {
      yaw.set_setpoint(y);
      pitch.set_setpoint(p);
      yaw.wait_position();
      pitch.wait_position();  
            
      delay(SENSOR_COLLECT_MS);
      
      Serial.print(y);
      Serial.print(",");
      Serial.print(p);
      Serial.print(",");
      Serial.print(yaw.get_position());
      Serial.print(",");
      Serial.print(pitch.get_position());
      Serial.print(",");
      Serial.println(dist.value());
    }
  }
  Serial.print("Done with scan");
  while(1);
}

int counter = 0;//time in us
uint32_t time_loop(uint32_t time) {//time sensitive, low code impact
  pitch.update(counter);
  yaw.update(counter);
  dist.update(counter);

  
  counter+=10;
  return (time+CORE_TICK_RATE/100);
}
