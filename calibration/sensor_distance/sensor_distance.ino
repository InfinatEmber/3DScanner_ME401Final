#include "sensor.h"
#define S A7

Sensor s;
int last = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("Setting up sensor.");
  s.setup(S);

  attachCoreTimerService(time_loop);
}

void loop() {
  int now = s.value();
  if(now != last){
    Serial.println(now);
    last = now;
  }
}

int counter = 0;//time in us
uint32_t time_loop(uint32_t time) {//time sensitive, low code impact
  s.update(counter);
  
  counter+=10;
  return (time+CORE_TICK_RATE/100);
}
