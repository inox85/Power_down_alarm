  // display 1 riga 7 pixel

//Changelog: aggiunto "analogWriteRange(1023)" compatibilità core 3.0

#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3C

#define relay1 12
#define relay2 13
#define buzzer 14
#define current_pin A0
#define flash_button 0

#define ARRAY_LENGHT 3600

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

float averageArray[ARRAY_LENGHT];
float VOLTS = 230.0;
float instantPower = 0;
float averagePower = 0;

const float MAX_PEAK_POWER = 4.0;
const float MAX_AVERAGE_POWER = 3.3;
int CURRENT_INDEX = 0;

int state;
unsigned int current_millis;
unsigned int current_millis_set;

bool avg_state = 0;

bool on_buzzer = 0;
bool beep = 0;
bool on_led = 0;

unsigned int clock_timer;
unsigned int minuti;
unsigned int ore;

unsigned long samples;
int wait = 0;

void setup() {

  Serial.begin(115200);

  for(int i = 0; i < ARRAY_LENGHT; i++) averageArray[i] = 0;
  
  pinMode(buzzer, OUTPUT);
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

  delay(1500);
  
}

void loop() {

  display.clearDisplay();

  float Irms = measure_Irms();

  instantPower = math_round((Irms * VOLTS / 1000.0),2);

  addToArray(instantPower);

  float sum = 0;
  for(int index = 0; index < ARRAY_LENGHT; index++) sum += averageArray[index];

  if (instantPower > MAX_PEAK_POWER)
  {
    avg_state = 1;
  }

    display.setTextSize(2);
    display.setCursor(0, 0);
    display.print(instantPower);
    display.setCursor(0, 30);
    display.print(averagePower);
    display.setCursor(52, 30);
    display.print("kW");
    display.setTextSize(1);
    display.setCursor(77, 37);
    display.print("AVG");

    
//  }
//
//  else {
//    current_millis = millis();
//    samples = 0;
//    on_led = 0;
//    sum_kW = 0;
//    kW_avg = 0;
//  }
//
//  if (kW > LIMIT_POWER && minuti < 180) {
//    wait++;
//    if (wait > 10) {
//      on_buzzer = 1;
//      on_led = 1;
//    }
//  }
//  else if (kW < LIMIT_POWER && minuti < 180) {
//    on_buzzer = 0;
//    wait = 0;
//  }
//
//  if (kW < 0.05) {
//    on_led = 0;
//    on_buzzer = 0;
//  }
//
//  if (on_led) {
//    state = digitalRead(LED_BUILTIN);
//    digitalWrite(LED_BUILTIN, !state);
//  }
//  else digitalWrite(LED_BUILTIN, HIGH);
//
//  if (on_buzzer && beep == 0) {
//    analogWriteFreq(2800);
//    analogWrite(buzzer, 512);
//    beep = 1;
//  }
//  else if (on_buzzer && beep == 1) {
//    analogWriteFreq(1100);
//    analogWrite(buzzer, 512);
//    beep = 0;
//  }
//  else analogWrite(buzzer, 0);
//
//
//  display.setTextSize(3);
//  display.setCursor(0, 0);
//  display.print(kW);
//  display.setCursor(80, 0);
//  display.print("kW");
//
//  display.display();

}

void addToArray(float value)
{
    for(int i = 0; i < ARRAY_LENGHT - 1; i++) averageArray[i] = averageArray[i + 1];
    averageArray[ARRAY_LENGHT - 1] = value;
}
