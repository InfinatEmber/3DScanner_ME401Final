#ifndef MOTOR
#define MOTOR

#include <PID_v1.h>
#include <SoftPWMServo.h>

#define MOTOR_PID_UPDATE_US 10000
#define MOTOR_POSITION_UPDATE_US 10

class Motor {
  private:
    int pin_A = 0;
    int pin_B = 0;
    int pin_dir = 0;
    int pin_pwm = 0;

    double kp = 0;
    double ki = 0;
    double kd = 0;

    double input = 0;
    double output = 0;
    double setpoint = 0;

    volatile char last_A;
    volatile char last_B;

    volatile int position = 0;

    bool reverse = false;
    int allowed_error = 0;
    
    PID pid = PID(&input,&output,&setpoint,kp,ki,kd,DIRECT);
  public:
    void setup(int A, int B, int dir, int pwm, double tp, double ti, double td, bool r, int e);
    void update(int us);
    void update_position();
    void update_PID();

    void set_tunings(double tp, double ti, double td);

    void set_setpoint(int p);

    bool at_position() const;

    void wait_position() const;

    void set_position(int p);

    int get_position() const;
};
void Motor::setup(int A, int B, int dir, int pwm, double tp, double ti, double td, bool r, int e){
  pin_A = A;
  pin_B = B;
  pin_dir = dir;
  pin_pwm = pwm;
  reverse = r;
  allowed_error = e;
  set_tunings(tp,ti,td);

  pinMode(pin_A,INPUT);
  pinMode(pin_B,INPUT);
  last_A = digitalRead(pin_A);
  last_B = digitalRead(pin_B);
  pinMode(pin_pwm,OUTPUT);
  pinMode(pin_dir,OUTPUT);
  digitalWrite(pin_pwm,0);
  digitalWrite(pin_dir,0);
  SoftPWMServoPWMWrite(pin_pwm,0);

  pid.SetMode(AUTOMATIC);
  pid.SetSampleTime(MOTOR_PID_UPDATE_US/1000);//us to ms
  pid.SetOutputLimits(-255,255);
}
void Motor::update(int us){
  if(us%MOTOR_POSITION_UPDATE_US == 0) {
    update_position();
  }
  if(us%MOTOR_PID_UPDATE_US == 0) {
    update_PID();
  }
}
void Motor::update_position() {
  char new_A = digitalRead(pin_A);
  char new_B = digitalRead(pin_B);

  position += (new_A^last_B)-(last_A^new_B);

  last_A = new_A;
  last_B = new_B;
}
void Motor::update_PID() {
  input = position;
  pid.Compute();
  bool dir = output > 0;
  if(reverse){dir = !dir;}
  digitalWrite(pin_dir,dir);
  SoftPWMServoPWMWrite(pin_pwm,abs(output));
}
void Motor::set_tunings(double tp, double ti, double td){
  pid.SetTunings(tp,ti,td);
}
void Motor::set_setpoint(int sp) {
  setpoint = sp;
}
bool Motor::at_position() const {
  return abs(position - setpoint) <= allowed_error;
}
void Motor::wait_position() const {
  while(!at_position()){}
  /*
  Serial.print("At position:");
  Serial.print(position);
  Serial.print(",");
  Serial.println(setpoint);
  */
}
void Motor::set_position(int p){
  position = p;
}
int Motor::get_position() const{
  return position;
}
#endif
