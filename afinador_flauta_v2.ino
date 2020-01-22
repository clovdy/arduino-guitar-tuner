int led_verde = 6;
int led_amarillo_1 = 2;
int led_amarillo_2 = 3;
int led_rojo_1 = 4;
int led_rojo_2 = 7;
int sensorPin= A0;
int sensorValue= 0;

#include "arduinoFFT.h"
 
#define SAMPLES 128             //Must be a power of 2
#define SAMPLING_FREQUENCY 10000 //Hz, must be less than 10000 due to ADC
#define SIZE 20
 
arduinoFFT FFT = arduinoFFT();
 
unsigned int sampling_period_us;
unsigned long microseconds;
unsigned long tend;
unsigned long freqreal;
long a;
int j=0;
double vReal[SAMPLES];
double vImag[SAMPLES];
float delta_mas;
float delta_menos;
float notas[SIZE];

void notasGeneration()
{
   int x;
   for(x = 0; x < SIZE; x++)
   {
       notas[x] = 650*(pow( pow(2, x) , (1/12.0) ));
       }
}



 
void setup() {
   notasGeneration();
   Serial.begin(9600);
   pinMode(led_verde, OUTPUT);
   pinMode(led_amarillo_1, OUTPUT);
   pinMode(led_amarillo_2, OUTPUT);
   pinMode(led_rojo_1, OUTPUT);
   pinMode(led_rojo_2, OUTPUT);
    sampling_period_us = round(1000000*(1.0/SAMPLING_FREQUENCY));
}
 
void loop() {
    unsigned long tstart = micros();
    /*SAMPLING*/
    
    for(int i=0; i<SAMPLES; i++)
    {
        microseconds = micros();    //Overflows after around 70 minutes!
     
        vReal[i] = analogRead(0);
        vImag[i] = 0;

        while(micros() < (microseconds + sampling_period_us)){
        }
    }
        tend = micros();
  //      Serial.println(tend-tstart);
 
    /*FFT*/
    FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
    double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);
 
    /*PRINT RESULTS*/
    //Serial.println(peak*0.82485698);     //Print out what frequency is the most dominant.
 
//    for(int i=0; i<(SAMPLES/2); i++)
//    {
//        /*View all these three lines in serial terminal to see which frequencies has which amplitudes*/
//         
//        //Serial.print((i * 1.0 * SAMPLING_FREQUENCY) / SAMPLES, 1);
//        //Serial.print(" ");
//        Serial.println(vReal[i], 1);    //View only this line in serial plotter to visualize the bins
//    }
 
    delay(10);  //Repeat the process every second OR:
    //while(1);       //Run code once
    
freqreal = (peak/1.2124032334099413-1.2);


a = freqreal - notas[0];


for(int i = 1; i < SIZE; i++)
{
  if (pow(freqreal-notas[i], 2) < pow(a,2))
  {
    a=freqreal-notas[i];
    j = i;
  }
  else{
    break;}
}

//Serial.println(j);


Serial.print(notas[j]);
Serial.print(" ");
Serial.print(j);
Serial.print(" ");
Serial.print(freqreal);
Serial.print(" ");
Serial.print(notas[j]*pow(0.5, 1.0/72.0));
Serial.print(" ");
Serial.println(notas[j]*pow(2, 1.0/72.0));





if (freqreal > (notas[j]*pow(0.5, 1.0/72.0)) && freqreal<(notas[j]*pow(2, 1.0/72.0))){  
     digitalWrite(led_rojo_1, LOW);
     digitalWrite(led_rojo_2, LOW);
     digitalWrite(led_amarillo_1, LOW);
     digitalWrite(led_amarillo_2, LOW);
     digitalWrite(led_verde, HIGH);// PRENDE VERDE
  }

if (freqreal >= (notas[j]*pow(2.0, 1.0/72.0)) && freqreal<=(notas[j]*pow(2.0, 2.0/72.0)))  {  
     digitalWrite(led_rojo_2, LOW);
     digitalWrite(led_rojo_1, LOW);
     digitalWrite(led_amarillo_1, HIGH);// PRENDE AMARILLO ALTO
     digitalWrite(led_amarillo_2, LOW); 
     digitalWrite(led_verde, LOW);
   }  
if (freqreal > (notas[j]*pow(2.0, 2.0/72.0)) && freqreal < (notas[j]*pow(2.0, 3.0/72.0))) {  
     digitalWrite(led_rojo_1, HIGH); //PRENDE ROJO ALTO
     digitalWrite(led_rojo_2, LOW);
     digitalWrite(led_amarillo_2, LOW);
     digitalWrite(led_amarillo_1, LOW);
     digitalWrite(led_verde, LOW);
  }
 if (freqreal < (notas[j]*pow(0.5, 1.0/72.0)) && freqreal >= (notas[j]*pow(0.5, 2.0/72.0))){  
     digitalWrite(led_rojo_1, LOW); 
     digitalWrite(led_rojo_2, LOW); 
     digitalWrite(led_amarillo_1, LOW); 
     digitalWrite(led_amarillo_2, HIGH);//PRENDE AMARILLO BAJO
     digitalWrite(led_verde, LOW);
 }  

    if (freqreal < (notas[j]*pow(0.5, 2.0/72.0)) && freqreal > (notas[j]*pow(0.5, 3.0/72.0))) {  
     digitalWrite(led_rojo_1, LOW);
     digitalWrite(led_rojo_2, HIGH);// PRENDE ROJO BAJO     
     digitalWrite(led_amarillo_1, LOW);
     digitalWrite(led_amarillo_2, LOW);
     digitalWrite(led_verde, LOW);
  }
}
