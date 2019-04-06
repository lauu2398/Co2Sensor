const int MQ_PIN = A0;      // Pin del sensor
const int RL_VALUE = 5;      // Resistencia RL del modulo en Kilo ohms
const int R0 = 10;          // Resistencia R0 del sensor en Kilo ohms
 
// Datos para lectura multiple
const int READ_SAMPLE_INTERVAL = 100;    // how many samples you are going to take
const int READ_SAMPLE_TIMES = 5;       // time interval in ms between each sample
 
// Calibration 
const float X0 = 200; //min value sensor can measure in ppm
const float Y0 = 1.7; //min value in volts
const float X1 = 10000; //max value sensor can measure in ppm
const float Y1 = 0.28; //max value sensor can measure in volts
 
// points of the calibration curve
const float point0[] = { log10(X0), log10(Y0) };
const float point1[] = { log10(X1), log10(Y1) };
 
// slope and coords
const float scope = (point1[1] - point0[1]) / (point1[0] - point0[0]);
const float coord = point0[1] - point0[0] * scope;
int mg_delay = 30000;//time between samples 30 seconds

void setup()
{
   Serial.begin(9600);
}
 
void loop()
{
   float volts= readMQ(MQ_PIN);      // Obtain volts
   float concentration = getConcentration(volts/R0);   // Obtain the cc of CO2
  
   Serial.println(concentration); //show the value of the concentration of C02 in ppm
     delay(mg_delay);
}
 
// To obtain the average of READ_SAMPLE_TIMES samples
float readMQ(int mq_pin)
{
   float v = 0;
   for (int i = 0;i<READ_SAMPLE_TIMES;i++) {
      v += getMQResistance(analogRead(mq_pin));
      delay(READ_SAMPLE_INTERVAL);
   }
   return v / READ_SAMPLE_TIMES;
}
 
// Resistence from analog reading
float getMQResistance(int raw_adc)
{
   return (((float)RL_VALUE / 1000.0*(1023 - raw_adc) / raw_adc));
}
 
// convert analog reading in cc(ppm)
float getConcentration(float volts_ratio)
{
   return pow(10, coord + scope * log(volts_ratio));
}
