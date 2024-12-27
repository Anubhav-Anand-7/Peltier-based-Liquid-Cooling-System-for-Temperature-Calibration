#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#include "OneWire.h"
#include "DallasTemperature.h"

#define CLK_PIN 1
#define DT_PIN 2
#define SW_PIN 3
#define PWM_PELTIER_OUTPUT_PIN 6

// #define PWM_PUMP_OUTPUT_PIN 7

#define i2c_Address 0x3c 
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 
#define OLED_RESET -1  

#define ONE_WIRE_BUS 0 

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
const int min_temperature=10;
const int max_temperature=25;

int CLK_state = LOW;
int prev_CLK_state = LOW;
bool toggle = false;
bool sw_flag = false;
bool button_press = false;

int target_temperature = 17;
float current_temperature;
int scrn_index;
int prev_scrn_index;
bool displayNeedsUpdate=true;

const int n = 8;
int eeprom[n];
int eeprom_sel = 1;
uint8_t eeprom_data = 0;

double dt, time_previous;
double integral, previous, pid_output, error;
double kp, ki, kd;

void setup() {
  Serial.begin(9600);

  scrn_index=0;
  prev_scrn_index=0;

  kp = 0.1;
  ki = 0.01;
  kd = 0.001;
  time_previous = 0;


  display.begin(i2c_Address, true);  
  display.clearDisplay();

  pinMode(CLK_PIN, INPUT);
  pinMode(DT_PIN, INPUT);
  pinMode(SW_PIN, INPUT_PULLUP);

  pinMode(PWM_PELTIER_OUTPUT_PIN, OUTPUT);
  // pinMode(PWM_PUMP_OUTPUT_PIN, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(DT_PIN), rotary, CHANGE);
  attachInterrupt(digitalPinToInterrupt(SW_PIN), rotary_sw, CHANGE);

  //digitalWrite(PWM_PELTIER_IN_PIN_1,HIGH);
  //digitalWrite(PWM_PELTIER_IN_PIN_2,LOW);
}

void loop() {
  if (scrn_index == 0) {screen_0();}
  else if (scrn_index == 1) {screen_1();}
  else if (scrn_index == 2) {screen_2();}
  else if (scrn_index == 3) {screen_3();}
  
}

void array_append(int t){
  for(int i = n - i; i >= 0 ; i--){
    eeprom[i+1] = eeprom[i];
  }
  eeprom[0]=t;
  eeprom_data++;
  if (eeprom_data >= n) {eeprom_data = n;}
}

void rotary() {
  CLK_state = digitalRead(CLK_PIN);
  if (prev_CLK_state == LOW && CLK_state == HIGH) {
    if (digitalRead(DT_PIN) == LOW) {
      if (scrn_index == 2) {
        target_temperature--;
        if (target_temperature < 10) {target_temperature = 10;}
      }
      if (scrn_index == 3) {
        eeprom_sel--;
        if (eeprom_sel < 1) {eeprom_sel = 1;}
      }
    }
    else {
      if (scrn_index == 2) {
        target_temperature++;
        if (target_temperature > 25) {target_temperature = 25;}
      }
      if (scrn_index == 3) {
        eeprom_sel++;
        if (eeprom_sel > eeprom_data) {eeprom_sel = eeprom_data;}
      }
    }
    toggle = !toggle;
    Serial.println(toggle);
  }
  prev_CLK_state = CLK_state;
  displayNeedsUpdate = true;
}

void rotary_sw() {
  sw_flag = !sw_flag;
  if (sw_flag) {
    Serial.println("Button Pressed");
    button_press = true;
    delay(200);
  } 
  displayNeedsUpdate = true;
}

float read_temperature() {
  sensors.requestTemperatures();
  float temperature = sensors.getTempCByIndex(0);
  if (temperature != DEVICE_DISCONNECTED_C){
    Serial.print("Current Temperature is : ");
    Serial.println(temperature);
  }
  else {
    Serial.println("Error: Could not read temperature data");
  }
  return temperature;
}
`
void screen_0() {
    // Update temperature reading each time before display update
    
    // PID Control
    double time_now = millis();
    current_temperature = read_temperature();
    dt = (time_now - time_previous)/1000.00;
    time_previous = time_now;
    error = target_temperature - current_temperature;
    pid_output = map(pid(error, kp, ki, kd), 15, -15, 0, 65535);	
    Serial.print("PWM: "); Serial.println(pid_output);
    if(pid_output>=65535){
      pid_output=65535;
    }
    if(pid_output<=0){
      pid_output=0;
    }
    analogWrite(PWM_PELTIER_OUTPUT_PIN, pid_output);
    // analogWrite(PWM_PUMP_OUTPUT_PIN, pid_output);

    Serial.print(target_temperature);
    Serial.print(",");
    Serial.print(current_temperature);
    Serial.print(",");
    Serial.print(pid_output);
    Serial.println();

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(0, 10);
    display.print("Set Value : ");
    display.println(target_temperature);
    display.setCursor(0, 30);
    display.print("Process Value : ");
    display.println(current_temperature);
    display.display();
    
  if (button_press == true) {
    scrn_index = 1;
    Serial.println("Moved to screen 1");
    button_press = false;
    displayNeedsUpdate = true;
    delay(1000);
  }
}

void screen_1() {
  // Only update display if needed
  if (displayNeedsUpdate) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);

    if (toggle) {
      display.setCursor(0, 10);
      display.println("Set NEW target");
      Serial.println("NEW----");
      display.setCursor(0, 30);
      display.setTextSize(0.5);
      display.println("Set previous target");
    }
   
    else {
      display.setCursor(0, 10);
      display.setTextSize(0.5);
      display.println("Set new target");
      display.setCursor(0, 30);
      display.setTextSize(1);
      display.println("Set PREVIOUS target");
      Serial.println("PREVIOUS----");
    }

    display.display();
    displayNeedsUpdate = false;  // Reset after updating the display
  }

  if (button_press == true) {
    if (toggle == true) {
      scrn_index = 2;
      Serial.println("Moved to screen 2");
    }
    else {
      scrn_index = 3;
      Serial.println("Moved to screen 3");
      eeprom_sel = 1;
    }
    button_press = false;
    displayNeedsUpdate = true;
    delay(1000);
  }
}

void screen_2() {
  // Display update only when needed
  if (displayNeedsUpdate) {
    display.clearDisplay();
    display.setTextColor(SH110X_WHITE);
    display.setCursor(0, 10);
    display.println("Set a new target : ");
    display.setCursor(0, 30);
    display.println(target_temperature);
    display.display();
    displayNeedsUpdate = false;
  }

  if (button_press == true) {
    array_append(target_temperature);
    display.clearDisplay();
    display.setTextColor(SH110X_WHITE);
    display.setCursor(0, 10);
    display.println("New target is set to");
    display.setCursor(0, 30);
    display.println(target_temperature);
    display.display();
    scrn_index = 0;
    button_press = false;
    Serial.println("Moved to screen 0");
    Serial.println(eeprom_data);
    displayNeedsUpdate = true;
    delay(1000);
  }
}

void screen_3() {
  if (eeprom_data == 0) {
    scrn_index = 0;
    Serial.println("Moved to screen 0");
    button_press = false;
    displayNeedsUpdate = true;
    delay(1000);    
  }

  else {
    if (displayNeedsUpdate) {
        display.clearDisplay();
        display.setTextColor(SH110X_WHITE);
        display.setCursor(0, 10);
        display.print("Previous Temperature #");
        display.println(eeprom_sel);
        display.setCursor(0, 30);
        display.println(eeprom[eeprom_sel - 1]);
        display.display();
        displayNeedsUpdate = false;
    }

    if (button_press == true) {
        display.clearDisplay();
        display.setTextColor(SH110X_WHITE);
        display.setCursor(0, 10);
        display.println("New target is set to");
        display.setCursor(0, 30);
        display.println(eeprom[eeprom_sel - 1]);
        target_temperature = eeprom[eeprom_sel - 1];
        array_append(target_temperature);
        display.display();
        scrn_index = 0;
        button_press = false;
        displayNeedsUpdate = true; // Update on returning to screen 0
        Serial.println("Moved to screen 0");
        delay(1000);
    }
  }
}

double pid(double error, double kp, double ki,  double kd) {
  double proportional = error;
  integral += error * dt;
  double derivative = (error - previous) / dt;
  previous = error;
  double output = (kp * proportional) + (ki * integral) + (kd * integral);
  return output; 
}