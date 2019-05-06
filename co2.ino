#define DC_GAIN (8.5) //Gain of the amplifier
#define MG_PIN (0)
#define ZERO_POINT_VOLTAGE (0.320) // output of the sensor in volts when the concentration of CO2 is 400PPM
#define REACTION_VOLTAGE (0.02) //voltage drop of the sensor when move the sensor from air into 1000ppm CO2

#define TIME_INTERVAL (50)    //define how many samples you are going to take in normal operation
#define READ_TIMES (5)     //define the time interval(in milisecond) between each samples in 
                                                  

int mg_delay = 30000;//Time between samples

float CO2Curve[3]  =  {2.602,ZERO_POINT_VOLTAGE,(REACTION_VOLTAGE/(2.602-3))};   //slope = ( reaction voltage ) / (log350 –log1000) 
void setup()
{
   Serial.begin(9600);
}
 

 
void loop() 
{
    int percentage;
    float volts;
    volts = MqRead(MG_PIN);
    percentage = MqGetPercentage(volts,CO2Curve);
    if (percentage == -1) {
        Serial.print( "<350" );
         Serial.print("\n");
    } else {
        Serial.print(percentage);
         Serial.print("\n");
    }
  
  delay(mg_delay);
}

float MqRead(int mg_pin){//read the output of the sensor
    int i;
    float volts=0;
    
    for (i=0;i<READ_TIMES;i++) {
        volts += analogRead(mg_pin);
        delay(TIME_INTERVAL);
    }
    
    volts = (volts/READ_TIMES) *5/1024 ;
    
    return volts;  
}

int  MqGetPercentage(float volts, float *pcurve){  
//Use the slope ans a point of the line
//we use the power of 10 to convert the result to a non-logarithmic value
   if ((volts/DC_GAIN )>=ZERO_POINT_VOLTAGE) {
      return -1;
   } else { 
    int percentage=pow(10, ((volts/DC_GAIN)-pcurve[1])/pcurve[2]+pcurve[0]);
      return percentage ;
   }
}
