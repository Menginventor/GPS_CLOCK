
#include <avr/pgmspace.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TimeLib.h>
#include <EEPROM.h>





#define OLED_RESET -1

Adafruit_SSD1306 display(OLED_RESET);
#define LEFT_BTN_PIN 7
#define MID_BTN_PIN 8
#define RIGHT_BTN_PIN 9
#define SD_SENSE_PIN A0
#define LEFT_BTN 0
#define MID_BTN 1
#define RIGHT_BTN 2
#define PRESS_DEBONCE_TIME 50//MS
#define TIME_ZONE_ADDR 0

char time_zone = 7 ;//7 hour for Thailand;
byte screen_state = 0;
const char* day_name[] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};


byte sate_used = 0;
String speed_str = "--";
String LAT_str = "--";
String LONG_str = "--";
String ALT_str = "--";
byte gps_date,gps_month,gps_year;
bool logging = false;
void setup() {
  // put your setup code here, to run once:
  //setTime(compileTime());
  //adjustTime(-60 * 60 * 7);


  //mySerial.attachInterrupt(&GPS_loop);

  display_init();
  delay(1000);
  pinMode(SD_SENSE_PIN, INPUT_PULLUP); // SD Card check (Active Low)
  pinMode(LEFT_BTN_PIN, INPUT_PULLUP);// Left button
  pinMode(MID_BTN_PIN, INPUT_PULLUP);// Middle button
  pinMode(RIGHT_BTN_PIN, INPUT_PULLUP);// Right button

  //setTime(23, 33, 00, 10, 5, 2018);
  Serial.begin(9600);
  EEPROM.get(TIME_ZONE_ADDR, time_zone);
}

void loop() {

  GPS_loop();
  oled_loop();
  button_loop ();


}


void update_screen_state(byte btn) {

  if (screen_state == 0) {
    if (btn == RIGHT_BTN)screen_state = 1;
    else  if (btn == LEFT_BTN)screen_state = 4;
  }
  else if (screen_state == 1) {
    if (btn == RIGHT_BTN)screen_state = 2;
    else  if (btn == LEFT_BTN)screen_state = 0;
  }
  else if (screen_state == 2) {
    if (btn == RIGHT_BTN)screen_state = 3;
    else  if (btn == LEFT_BTN)screen_state = 1;
  }
  else if (screen_state == 3) {
    if (btn == RIGHT_BTN)screen_state = 4;
    else  if (btn == LEFT_BTN)screen_state = 2;
    else  if (btn == MID_BTN){
      logging = !logging;
      if(logging){
        if(digitalRead(SD_SENSE_PIN) == LOW){//sd in
       
        }
        else logging = false;
      }
    }
  }
  else if (screen_state == 4) {
    if (btn == RIGHT_BTN)screen_state = 0;
    else  if (btn == LEFT_BTN)screen_state = 3;
    else  if (btn == MID_BTN)screen_state = 5;
  }
  else if (screen_state == 5) {
    if (btn == RIGHT_BTN)time_zone++;
    else  if (btn == LEFT_BTN)time_zone--;
    else  if (btn == MID_BTN) {
      screen_state = 4;
      EEPROM.put(TIME_ZONE_ADDR, time_zone);
    }

  }
  else {
    screen_state = 0;
  }
  oled_display();

}
void oled_loop() {
  static time_t prevDisplay;


  if (now() != prevDisplay ) {
    prevDisplay = now();
    oled_display();

  }
}
void oled_display() {

  if (screen_state == 0) {
    display_clock();
  }
  else if (screen_state == 1) {
    display_speedo() ;
  }
  else if (screen_state == 2) {
    display_position() ;
  }
  else if (screen_state == 3) {
    display_SD() ;
  }
  else if (screen_state == 4) {
    display_timezone() ;
  }
  else if (screen_state == 5) {
    setting_timezone() ;
  }

  else {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.print(screen_state);
    display.display();
  }
}

float batt_voltage() {
  return float(analogRead(A3) * 2 * 3.3 / 1023);
}

