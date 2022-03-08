
#include "arduinoFFT.h" 

arduinoFFT FFT = arduinoFFT();
#define SAMPLES 4096             
#define SAMPLING_FREQUENCY 1000 
#define amplitude 200         
#define NB_TUPLE 3   
unsigned int sampling_period_us;
double vReal1[SAMPLES];
double vImag1[SAMPLES];
unsigned long newTime;

struct tuple {
    int f0;
    int f1;
    int seuil;
    bool valide;
};


tuple tuple_arr [NB_TUPLE]= {{220,240,20000,false},{370,380,20000,false}, {620,640,20000,false}};

void setup() {

    Serial.begin(115200);
    pinMode(15, INPUT);  // entrée son sur GPIO 0
    sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQUENCY));
}


void loop(){

    Read_microphone();

    //   prise de son et création de sa FFT

    FFT.DCRemoval();
    FFT.Windowing(vReal1, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal1, vImag1, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal1, vImag1, SAMPLES);

    
    check_all_tuple(tuple_arr);
}

void check_all_tuple (tuple* tuple_array){
    static int cpt =0;
    for (int i=0; i<NB_TUPLE; i++) {
    check_tuple(&tuple_array[i]);
    }

    for (int j=0; j<NB_TUPLE; j++) {
        Serial.println("//");
        Serial.println("tuple");
        Serial.println(j);
        Serial.println("validité");
        Serial.println(tuple_array[j].valide);
        if(tuple_array[j].valide == true) {
            cpt++;
            tuple_array[j].valide = false;
            
        }
    }
    Serial.println("compteur=");
    Serial.println(cpt);

    if (cpt > 2*NB_TUPLE/3) {
        Serial.println("  ALERTE FRELON ");
        cpt=0;
    }
    else {
      cpt = 0;
      }

}


void check_tuple(tuple* tuple_f) {

    for(int j = round(tuple_f->f0/0.244) ; j<= round(tuple_f->f1/0.244) ; j++) {
      if(vReal1[j] > tuple_f->seuil) {
            tuple_f->valide = true ;
            Serial.println("verifiee");
      }
    }
}

void Read_microphone() {
     for (int i = 0; i < SAMPLES; i++) {
        newTime  = micros();         //  lecture du son via un micro mis en GPIO0
        vReal1[i] = analogRead(15);   //  Using pin number for ADC port ici GPIO0
        vImag1[i] = 0;
        while ((micros() - newTime) < sampling_period_us) { /* do nothing to wait */ }
    }
}
