#ifndef SENSOR
#define SENSOR

#define FILTER_LENGTH 1000
#define SENSOR_UPDATE_US 500

class Sensor {
  private:
    int pin = 0;

    int value_array[FILTER_LENGTH] = {0};
    int sorted_array[FILTER_LENGTH] = {0};

    void add_value(int value);
    void sort_array();
    int get_median();
   public:
    void setup(int s);
    void update(int us);

    int value();
};
void Sensor::add_value(int value){
  for(int i = 0; i < FILTER_LENGTH-1;i++) {
    value_array[i] = value_array[i+1];
  }
  value_array[FILTER_LENGTH-1] = value;
}
void Sensor::sort_array(){
  int value = 0;
  int hold = 0;
  for(int i = 0; i < FILTER_LENGTH; i++) {
    sorted_array[i] = 0;
  }
  for(int i = 0; i < FILTER_LENGTH;i++) {
    value = value_array[i];
    for(int j = FILTER_LENGTH-1;j>= 0; j--) {
      if(value == 0) {//0 lowest
        break;
      }
      if(value > sorted_array[j]){
        hold = value;
        value = sorted_array[j];
        sorted_array[j] = hold;
      }
    }
  }
}
int Sensor::get_median(){
  return sorted_array[(int) (FILTER_LENGTH/2)];//no averaging, just seems easier, biased low, not true median
}
void Sensor::setup(int s) {
  pin = s;
  pinMode(pin,INPUT);
}
void Sensor::update(int us){
  if(us%SENSOR_UPDATE_US == 0) {
    add_value(analogRead(pin));
  }
}
int Sensor::value() {
  sort_array();
  return get_median();
}
#endif
