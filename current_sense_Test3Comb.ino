
//Written for NodeMCU
//by Muhammad Ateeb Aslam
double current=0;
const int currentPin = A0;
const unsigned long sampleTime = 100000UL;                           // sample over 100ms, it is an exact number of cycles for both 50Hz and 60Hz mains
const unsigned long numSamples = 250UL;                               // choose the number of samples to divide sampleTime exactly, but low enough for the ADC to keep up
const unsigned long sampleInterval = sampleTime/numSamples;  // the sampling interval, must be longer than then ADC conversion time
//adc_zero = 760 for 30Amp module
const int adc_zero = 487;                                         // relative digital zero of the arudino input from ACS712 (could make this a variable and auto-adjust it)
double data[250];                                       //(digital zero)511 in case of Arduino UNO 760 in case of NodeMCU
double mili_amp_current=0;

void setup()
{
 Serial.begin(9600);
}

void loop()
{

CurrentSense();                    //currentsense() returns the current rms value in amps
//Serial.println(analogRead(currentPin));
//Serial.println((long)current);
printDouble(current,6); //printdouble functional call for printin current value - type double 
//Serial.println(mili_amp_current +"milliamps");  //printing miliamp current
delay(1000);

}

void CurrentSense()
{
// unsigned long currentAcc = 0;
 unsigned int count = 0;
 unsigned long prevMicros = micros() - sampleInterval ;
 while (count < numSamples)
 {
   if (micros() - prevMicros >= sampleInterval)
   {
     data[count] = analogRead(currentPin) - adc_zero;
     data[count] += (unsigned long)(data[count] * data[count]); //findin squares
     count=count+1;
     prevMicros += sampleInterval;
   }
 }
  double sum=0;
  for(int i=0;i<250;i++){
       sum = sum+data[i];  //findin sum for mean
  }
 double rms = sqrt((double)sum/(double)numSamples) * (50 / 1320.0);   //squareroot //RMS //1024 for Arduino UNO 5000/66~76
rms=rms-0.10;                                                         //in case of 30A module //1320 for NodeMCU
//if (rms<0.20)
//{
//rms=0;
//}
current=rms;
 mili_amp_current = current * 1000;          //converting amp to milliamp
}


//printdouble function - enablin print of double values 

void printDouble( double val, byte precision){

 Serial.print (int(val));  //prints the int part
 if( precision > 0) {
   Serial.print("."); // print the decimal point
   unsigned long frac;
   unsigned long mult = 1;
   byte padding = precision -1;
   while(precision--)
      mult *=10;
      
   if(val >= 0)
     frac = (val - int(val)) * mult;
   else
     frac = (int(val)- val ) * mult;
   unsigned long frac1 = frac;
   while( frac1 /= 10 )
     padding--;
   while(  padding--)
     Serial.print("0");
   Serial.println(frac,DEC) ;
 }
}


