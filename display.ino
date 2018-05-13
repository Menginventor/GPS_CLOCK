void display_init() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Meng's GPS Syn Clock \nby Meng's Lab");
  display.display();
}
void display_clock() {
  time_t t = now();

  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.print(day_name[weekday(t) - 1]);
  display.print(" ");
  display.print(day(t));
  display.print("/");
  display.print(month(t));
  display.print("/");
  display.print(year(t));
  display.print(" B:");
  display.print(batt_voltage());

  display.setCursor(0, 10);
  display.setTextSize(3);
  if(hour(t)<10)  display.print(" " );
  display.print(hour(t) );
  display.print(":");
   if(minute(t)  <10)  display.print(" " );
  display.print(minute(t) );
  display.setTextSize(2);
  display.print(" ");
   if(second(t) <10)  display.print(" " );
  display.print(second(t) );
  display.display();
}
void display_speedo() {
  time_t t = now();
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.print(hour(t) );
  display.print(":");
  display.print(minute(t) );


  display.print("  Sate:");
  display.print(sate_used);
  display.print("  B:");

  display.print(batt_voltage());

  display.setCursor(0, 10);
  display.setTextSize(3);

  display.print(speed_str);
  display.setTextSize(1);
  display.print(" ");
  display.print("km/h");
  display.display();
}
void display_position() {
  time_t t = now();
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.print(hour(t) );
  display.print(":");
  display.print(minute(t) );


  display.print("  Sate:");
  display.print(sate_used);
  display.print("  B:");

  display.println(batt_voltage());




  display.print("LAT:");
  display.println(LAT_str);
  display.print("LNG:");
  display.println(LONG_str);
  display.print("ALT:");
  display.println(ALT_str);
  display.display();
}
void display_timezone () {
  display.clearDisplay();
   display.setCursor(0, 0);
  display.setTextSize(1);
  display.print("Set Time Zone");
  display.setCursor(30, 10);
  display.setTextSize(3);
  display.print(" ");
  if (time_zone > 0) {
    display.print("+");
  }
  display.print(time_zone, DEC);
  display.display();
}
void setting_timezone () {
  display.clearDisplay();
   display.setCursor(0, 0);
  display.setTextSize(1);
  display.print("Set Time Zone");
  display.setCursor(30, 10);
  display.setTextSize(3);
    display.print("<");
  if (time_zone > 0) {
    display.print("+");
  }
  display.print(time_zone, DEC);
   display.print(">");
  display.display();
}

