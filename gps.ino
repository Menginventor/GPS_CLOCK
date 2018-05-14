void GPS_loop() {
  static bool receiving = false;
  static String msg = "";
  unsigned long rx_timer = millis();
  boolean data_in = false;

  while (Serial.available()) {
    rx_timer = millis();
    char ch_in = Serial.read();
    //Serial.print(ch_in);
    if ( ch_in == '$') {
      msg = "";
      receiving = true;
    }
    else if  ( ch_in == '\r') {

      GPS_read(msg);
      receiving = false;

    }
    else if (receiving) {
      msg += ch_in;
    }
    data_in = true;
  }
  if (millis() - rx_timer > 100) {
    if (data_in) {


    }
    return;
  }

}

void GPS_read(String &msg) {

  if (msg.substring(2, 5).equals("GGA")) {

    GPS_GGA_read(msg);

  }
  else if (msg.substring(2, 5).equals("RMC")) {

    GPS_RMC_read(msg);
  }
}

void GPS_GGA_read(String &msg) {

  unsigned char comma_arr[14];
  unsigned char comma_i = 0;
  unsigned char packet_checksum = 0;
  unsigned char CRC = msg.charAt(0);
  LAT_str = "--";
  LONG_str = "--";
  ALT_str = "--";
  for (unsigned char i = 0 ; i < msg.length() ; i++) {
    if (comma_i > 14) {
      //Serial.println("ERR");
      //Serial.println(comma_i);
      return;
    }
    if (msg.charAt(i) == ',') {
      comma_arr[comma_i] = i;
      comma_i++;

    }
    if (i > 0 && i < msg.length() - 3) {
      CRC = CRC ^ msg.charAt(i);
    }
    //Serial.print(msg.charAt(i));

  }


  //Serial.println();

  unsigned char CRC_rx = (hex_char_to_byte (msg.charAt(msg.length() - 2)) << 4) | hex_char_to_byte (msg.charAt(msg.length() - 1));
  if (CRC != CRC_rx  ) {
    /*
        Serial.println("CRC invalid!");
        Serial.println(CRC, HEX);
        Serial.println((hex_char_to_byte (msg.charAt(msg.length() - 2)) << 4) | hex_char_to_byte (msg.charAt(msg.length() - 1))  , HEX);
    */
    return;
  }

  if (msg.charAt(comma_arr[5] + 1) == '0') {
    //Serial.println("GGA unavailable");
    return;
  }
  // Serial.print("UTC\t:");//hhmmss.ss

  // Serial.println(msg.substring(comma_arr[0] + 1, comma_arr[1]));
  String GPS_UTC = msg.substring(comma_arr[0] + 1, comma_arr[1]);
  byte gps_hour = GPS_UTC.substring(0, 2).toInt();
  byte gps_minute = GPS_UTC.substring(2, 4).toInt();
  byte gps_second = GPS_UTC.substring(4, 6).toInt();

  setTime(gps_hour, gps_minute, gps_second, gps_date, gps_month, int(gps_year) + 2000);
  adjustTime(int(time_zone) * 60 * 60);

  if (msg.substring(comma_arr[6] + 1, comma_arr[7]).length() != 0) {
    sate_used = msg.substring(comma_arr[6] + 1, comma_arr[7]).toInt();
    //Serial.print("sate_used = ");
    //Serial.println( sate_used );
  }
  //
  /*
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("UTC : " + msg.substring(comma_arr[0] + 1, comma_arr[1]));
    display.setCursor(0, 10);
    display.println("LAT : " + msg.substring(comma_arr[1] + 1, comma_arr[2]) + msg.substring(comma_arr[2] + 1, comma_arr[3]));
    display.setCursor(0, 20);
    display.println("LONG : " + msg.substring(comma_arr[3] + 1, comma_arr[4]) + msg.substring(comma_arr[4] + 1, comma_arr[5]));
    display.display();
  */
  LAT_str = msg.substring(comma_arr[1] + 1, comma_arr[2]) + " " + msg.substring(comma_arr[2] + 1, comma_arr[3]);
  LAT_str = LAT_str.substring(0, 2) + " " + LAT_str.substring(2, LAT_str.length());
  LONG_str = msg.substring(comma_arr[3] + 1, comma_arr[4]) + " " + msg.substring(comma_arr[4] + 1, comma_arr[5]);
  LONG_str =  LONG_str.substring(0, 3) + " " +  LONG_str.substring(3, LONG_str.length());
  ALT_str = msg.substring(comma_arr[8] + 1, comma_arr[9]) + " m";
  /*
    Serial.println("GGA available");

    Serial.print("LAT\t:");
    Serial.print(msg.substring(comma_arr[1] + 1, comma_arr[2]));
    Serial.println(msg.substring(comma_arr[2] + 1, comma_arr[3]));
    Serial.print("LONG\t:");
    Serial.print(msg.substring(comma_arr[3] + 1, comma_arr[4]));
    Serial.println(msg.substring(comma_arr[4] + 1, comma_arr[5]));

    Serial.print("QUAL\t:");
    Serial.println(msg.substring(comma_arr[5] + 1, comma_arr[6]));
    Serial.print("SAT\t:");
    Serial.println(msg.substring(comma_arr[6] + 1, comma_arr[7]));

    Serial.print("ALT\t:");
    Serial.println(msg.substring(comma_arr[8] + 1, comma_arr[9]));
  */

  if (logging) {
    
  }

}

void GPS_RMC_read(String &msg) {
  unsigned char comma_arr[14];
  unsigned char comma_i = 0;
  unsigned char packet_checksum = 0;
  unsigned char CRC = msg.charAt(0);
  speed_str = "--";
  for (unsigned char i = 0 ; i < msg.length() ; i++) {
    if (comma_i > 12)return;
    if (msg.charAt(i) == ',') {
      comma_arr[comma_i] = i;
      comma_i++;

    }
    if (i > 0 && i < msg.length() - 3) {
      CRC = CRC ^ msg.charAt(i);
    }
    //Serial.print(msg.charAt(i));

  }


  //Serial.println();
  unsigned char CRC_rx = (hex_char_to_byte (msg.charAt(msg.length() - 2)) << 4) | hex_char_to_byte (msg.charAt(msg.length() - 1));
  if (CRC != CRC_rx  ) {
    /*
        Serial.println("CRC invalid!");
        Serial.println(CRC, HEX);
        Serial.println((hex_char_to_byte (msg.charAt(msg.length() - 2)) << 4) | hex_char_to_byte (msg.charAt(msg.length() - 1))  , HEX);
    */
    return;
  }

  if (msg.charAt(comma_arr[11] + 1) == 'N') {
    Serial.println("RMC unavailable");
    return;
  }/*
  Serial.print("SPEED\t:");
  Serial.println(msg.substring(comma_arr[6] + 1, comma_arr[7]));
  Serial.print("DATE\t:");
  Serial.println(msg.substring(comma_arr[8] + 1, comma_arr[9]));
*/
  if (msg.substring(comma_arr[6] + 1, comma_arr[7]).length() != 0) {
    speed_str = String(msg.substring(comma_arr[6] + 1, comma_arr[7]).toFloat() * 1.852);
  }
  if (msg.substring(comma_arr[8] + 1, comma_arr[9]).length() != 0) {
    String date_str = msg.substring(comma_arr[8] + 1, comma_arr[9]);
    gps_date = date_str.substring(0, 2).toInt();
    gps_month = date_str.substring(2, 4).toInt();
    gps_year = date_str.substring(4, 6).toInt();
  }


}

unsigned char hex_char_to_byte (char hex_char) {
  if (hex_char >= '0' && hex_char <= '9')return hex_char - '0';
  return (char_upper(hex_char) - 'A') + 10;
}

char char_upper(char ch) {
  if (ch >= 'a' && ch <= 'z') ch = ch - ('a' - 'A');
  return ch;
}
