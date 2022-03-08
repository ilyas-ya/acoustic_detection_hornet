
#include "arduinoFFT.h" // Standard Arduino FFT library https://github.com/kosme/arduinoFFT

arduinoFFT FFT = arduinoFFT();
#define SAMPLES 4096             // Must be a power of 2
#define SAMPLING_FREQUENCY 1000 // Hz, must be 40000 or less due to ADC conversion time. Determines maximum frequency that can be analysed by the FFT Fmax=sampleF/2.
#define amplitude 200            // Depending on your audio source level, you may need to increase this value
unsigned int sampling_period_us;
double vReal1[SAMPLES];
double vImag1[SAMPLES];
double local [10]={0}; // pour sauver les 10 localisations

// voir l'exemple FFT_01 https://github.com/kosme/arduinoFFT/blob/master/Examples/FFT_01/FFT_01.ino
unsigned long newTime, chrono1, chrono2;
int compteur_tour =0;
int Nb_chrono =0;

void setup() {

    Serial.begin(115200);
    pinMode(15, INPUT);  // entrée son sur GPIO 0
    sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQUENCY));
}


void loop(){

    //   prise de son et création de sa FFT
    for (int i = 0; i < SAMPLES; i++) {
        newTime  = micros();         //  lecture du son via un micro mis en GPIO0
        vReal1[i] = analogRead(15);   //  Using pin number for ADC port ici GPIO0
        //Serial.println("avant");
        //Serial.println(vReal1[i]);
        vImag1[i] = 0;
        while ((micros() - newTime) < sampling_period_us) { /* do nothing to wait */ }
    }
    FFT.DCRemoval();
    FFT.Windowing(vReal1, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal1, vImag1, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal1, vImag1, SAMPLES);

    for(int j = 901; j<=983; j++) { //220-240
      //Serial.println(vReal1[j]);
      /*  if(vReal1[j] > 5000) {
          Serial.println("detection frelon");
          }
          */
      if(vReal1[j] > 30000) {
            Serial.println("  ALERTE FRELOOOOOOOOOOOOOOOOOON ");
      }
    }

    // calcul du pic
    //double pic1 = FFT.MajorPeak(vReal1, SAMPLES, SAMPLING_FREQUENCY);
  
    // début des tests de frelons asiatiques
    //pic1 = 220;  // pour test
    //Serial.println(pic1);
   
    
    
    //for (int j=0; j< SAMPLES; j++) {
    //  Serial.println("après");
    //   Serial.println(vReal1[j]);
    //}
      //if (pic1 == vReal1[j]){
      //Serial.println("amplitude");
      //Serial.println(vImag1[j]);
      //}
      //}

    

    // on cherche si le pic = 234 +- 10%
    /*if (pic1 >210  && pic1 <=260) {
        // soupçon de frelon => on garde la trace de la FFT
        // enregistrement de la FFT
        // mise en tableau des 5 parties réelles vecteurs de la FFT
            Serial.print(" pic :  ");
            Serial.println(pic1);
        for (int i = 2; i < bb; i++) { // i = 2 pour éliminer les basses fréquences < 78 Hz
            traceur [Nb_chrono*bb+i] = vReal1 [i];
            Serial.print(traceur [i]);
            Serial.print("  ");
        }
            Serial.println("  ");
        // est-ce que il y a  5 pics en 235 en moins de 30 secondes ?
        chrono2 = millis(); // demarrage du chrono 2
        if (Nb_chrono == 0) {
            chrono1 = chrono2 ;  // remise à l'heure du compteur 1
        }
        unsigned long interval;
        interval = chrono2 - chrono1 ;

        if (interval < 5000 ) {     // les deux pics sont séparés de moins de 5s
            chrono1 = chrono2;
            chrono2 = millis();
            Nb_chrono++;
            delay(100); // attente
        }
        else { Nb_chrono = 0;}   // remise à zero du compteur

        if ( Nb_chrono > 4 ) {     // !!!! FRELON !!!!
            Nb_chrono=0;
            // affichage alerte  FRELON !
            Serial.println("  A L E R T E ");
            Serial.println(" F R E L O N S");
            Serial.println("   /-------!  ");
            Serial.println("   !  o-o  !  ");
            Serial.println("   !  (!)  ! ");
            Serial.println("   !___!___/  ");
        }

        
    }   // fin des tests
    */
}   // fin de LOOP
