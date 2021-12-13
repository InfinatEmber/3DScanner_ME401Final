#ifndef SWITCH
#define SWITCH

class Switch {
  private:
  int pin = 0;
  public:
  void setup(int p);
  void update(int us);
  bool pressed() const;
};
void Switch::setup(int p){
  pin = p;
  pinMode(pin,INPUT);
}
bool Switch::pressed() const {
  return digitalRead(pin);
}
#endif
