  // display 1 riga 7 pixel

//Changelog: aggiunto "analogWriteRange(1023)" compatibilità core 3.0

#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3C

#define GRAPH_HEIGHT 33

#define relay1 12
#define relay2 13
#define BUZZER_PIN 14
#define CURRENT_PIN A0
#define flash_button 0

#define ARRAY_LENGHT 3600

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

float averageArray[ARRAY_LENGHT];
float displaySamples[128];
float VOLTS = 230.0;
float instantPower = 0;
float averagePower = 0;

const float MAX_PEAK_POWER = 3.6;
const float MAX_AVERAGE_POWER = 3.3;
int CURRENT_INDEX = 0;


bool LED_STATE = false;


void setup() {

  Serial.begin(115200);

  for(int i = 0; i < ARRAY_LENGHT; i++) averageArray[i] = 0;
  for(int i = 0; i < 128; i++) displaySamples[i] = 0;
  
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(flash_button, INPUT);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  analogWriteRange(1023);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (1);
  }

  display.setTextColor(SSD1306_WHITE);

  display.clearDisplay();

  display.setTextSize(2);
  display.setCursor(7, 0);
  display.print("power down");
  display.setTextSize(1);
  display.setCursor(55, 53);
  display.print("v1.1");
  display.setTextSize(4);
  display.setCursor(8, 20);
  display.print("ALARM");
  display.display();

  delay(500);
  
}

void loop() {

  display.clearDisplay();

  float Irms = measure_Irms();
  

  instantPower = math_round((Irms * VOLTS / 1000.0),2);
  //Serial.println(instantPower);
  addToArray(instantPower);

  float sum = 0;
  for(int index = 0; index < ARRAY_LENGHT; index++) sum += averageArray[index];
  
  if (instantPower > MAX_PEAK_POWER)
  {
    LED_STATE != LED_STATE;
    digitalWrite(LED_BUILTIN,LED_STATE);
    analogWriteFreq((int)(instantPower*(float)200));
    analogWrite(BUZZER_PIN, 512);
  }else
  {
    digitalWrite(LED_BUILTIN,HIGH);
    analogWrite(BUZZER_PIN, 0);
  }

    display.setTextSize(2);
    display.setCursor(0, 0);
    display.print(instantPower);
    display.setCursor(52, 0);
    display.print("kW");
    display.setCursor(0, 15);
    display.print(getAverage());
    display.setCursor(52, 15);
    display.print("kW");
    display.setTextSize(1);
    display.setCursor(77, 22);
    display.print("AVG");

    drawGraphAxis();

    getXAxisValue(instantPower);

    drawGraph();
    
    display.display();
}

void addToArray(float value)
{
    for(int i = 0; i < ARRAY_LENGHT - 1; i++) averageArray[i] = averageArray[i + 1];
    averageArray[ARRAY_LENGHT - 1] = value;
}

float getAverage()
{
    float sum = 0;
    for(int i = 0; i < ARRAY_LENGHT; i++) sum += averageArray[i];
    return sum/ARRAY_LENGHT;
    
}



void drawGraphAxis()
{

  display.drawLine(0, 63-GRAPH_HEIGHT,0 ,63 ,SSD1306_WHITE);
  display.drawLine(0, 63, 127, 63, SSD1306_WHITE);
  
  for(int i = 0; i < 128;i++)
  {
    if(i % 10 == 0)
    {
       display.drawPixel(i,getXAxisValue(1),SSD1306_WHITE);
       display.drawPixel(i,getXAxisValue(2),SSD1306_WHITE);
       display.drawPixel(i,getXAxisValue(3),SSD1306_WHITE);
    }
  }
}

int getXAxisValue(float power)
{
   int p = (int)(power*100);
   int pixelValue = 63-map(p,0,330,0,GRAPH_HEIGHT);
   Serial.print("X: ");
   Serial.println(pixelValue);
   
   return pixelValue;
 
}

void drawGraph()
{
  int index = 0;
  for(int i = 0; i < 128; i++)
  { 
    float sum = 0;
    for(int c = 0; c < 28; c++)
    {
      index++;
      sum += averageArray[index];
    }
    displaySamples[i]=sum/28;
  }
  
  for(int i = 0; i < 127; i++)
  {
    display.drawLine(126-i, getXAxisValue(displaySamples[126-i]), 127-i, getXAxisValue(displaySamples[127-i]), SSD1306_WHITE);
  }
  
//  
//  for(int i = 0; i < 127; i++)
//  {
//    display.drawLine(126-i, getXAxisValue(averageArray[3598-i]), 127-i, getXAxisValue(averageArray[3599-i]), SSD1306_WHITE);
//  }
}
