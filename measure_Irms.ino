
#define CURRENT_PIN A0

float offsetI = 536;

float I_CAL = 0.07; //(bro 0.0695 / robby 0.0732 / io  0.0715)

bool lastVCross, checkVCross;

unsigned long quadCurrent;

float measure_Irms (){

  int sampleI = 0;
  int count = 0 ;
  
  for(int i = 0; i < 1000; i++)
  {
    count++;
    sampleI = analogRead(CURRENT_PIN);
    quadCurrent += sampleI*sampleI; 
    delay(1);   
  }

  float rmsCurrent = (float)sqrt(quadCurrent/count)*(I_CAL);

  return rmsCurrent;
}
