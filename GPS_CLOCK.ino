

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


int offset = 7 * 60 * 60; //7 hour for Thailand;
byte screen_state = 0;
const char* day_name[] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
byte sate_used = 0;
String speed_str = "--";
String LAT_str = "--";
String LONG_str = "--";
String ALT_str = "--";

void setup() {
  // put your setup code here, to run once:
  setTime(compileTime());
  adjustTime(-60 * 60 * 7);


  //mySerial.attachInterrupt(&GPS_loop);

  display_init();
  delay(1000);
  pinMode(SD_SENSE_PIN, INPUT_PULLUP); // SD Card check (Active Low)
  pinMode(LEFT_BTN_PIN, INPUT_PULLUP);// Left button
  pinMode(MID_BTN_PIN, INPUT_PULLUP);// Middle button
  pinMode(RIGHT_BTN_PIN, INPUT_PULLUP);// Right button

  //setTime(23, 33, 00, 10, 5, 2018);
  Serial.begin(9600);
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
  }
  else if (screen_state == 4) {
    if (btn == RIGHT_BTN)screen_state = 0;
    else  if (btn == LEFT_BTN)screen_state = 3;
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


time_t compileTime(void) {
#define FUDGE 25        //fudge factor to allow for compile time (seconds, YMMV)

  char *compDate = __DATE__, *compTime = __TIME__, *months = "JanFebMarAprMayJunJulAugSepOctNovDec";
  char chMon[3], *m;
  int d, y;
  tmElements_t tm;
  time_t t;

  strncpy(chMon, compDate, 3);
  chMon[3] = '\0';
  m = strstr(months, chMon);
  tm.Month = ((m - months) / 3 + 1);

  tm.Day = atoi(compDate + 4);
  tm.Year = atoi(compDate + 7) - 1970;
  tm.Hour = atoi(compTime);
  tm.Minute = atoi(compTime + 3);
  tm.Second = atoi(compTime + 6);
  t = makeTime(tm);
  return t + FUDGE;        //add fudge factor to allow for compile time
}