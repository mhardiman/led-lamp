//fading.ino by Mark Hardiman
//Used to control PWM output to LEDs and read in two ADC inputs to control brightness(amplitude) and frequency
void setup() {
 // Serial.begin(9600);

}
float max_freq = 4.0f;
int ledPin = 3;
float val = 0;
float incr = 1;
float amp = 0;
float freq = 0;
float rate = 0;
float amps[10] = {0,0,0,0};//FIFO to smooth ADC readings
float freqs[10] = {0,0,0,0};//FIFO to smooth ADC readings
int i = 0;
float freqAvg = 0;
float ampAvg = 0;
float hyst = .15; //hysteresis threshold
void loop() {
  amps[i] = analogRead(A4)/1023.0f;
  freqs[i] = (1023-analogRead(A3))*max_freq/1023.0f; //reversed pot
  //computer buffer averages
  ampAvg = (amps[0]+amps[1]+amps[2]+amps[3])/4.0f;
  freqAvg = (freqs[0]+freqs[1]+freqs[2]+freqs[3])/4.0f;
  if (freqAvg > hyst)
  {
    
    rate = incr*5.12f*freqAvg*ampAvg;//512*.01 = DAC units to increment over (256 up and down)* delay
    val += rate;
    //Serial.println("\nfreq = ");
    //Serial.println(rate);
   
    if (val >= 255*ampAvg)
    {
      val = round(255*ampAvg);
      incr = -1;
    }
    else if (val <= 0)
    {
      val = 0;
      incr = 1;
    }
    hyst = .15;
    analogWrite(ledPin, round(val));
  }
  else
  {
    analogWrite(ledPin, ampAvg*255);
    hyst = .5;
  }

    delay(10);
    i++;
    if (i==10)
      i = 0;
}
