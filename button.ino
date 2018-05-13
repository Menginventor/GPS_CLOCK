
void button_loop () {//read switch with debonce
  static byte left_btn_state  = 1;
  static byte mid_btn_state   = 1;
  static byte right_btn_state = 1;
  byte left_btn_read =  digitalRead(LEFT_BTN_PIN);
  byte mid_btn_read =  digitalRead(MID_BTN_PIN);
  byte right_btn_read =  digitalRead(RIGHT_BTN_PIN);
  static unsigned long left_timer;
  static unsigned long mid_timer;
  static unsigned long right_timer;
  if (left_btn_read != left_btn_state) {
    left_btn_state = left_btn_read;
    if (left_btn_state == LOW) { //Pressing
      left_timer = millis();
    }
    else {
      if (millis() - left_timer > PRESS_DEBONCE_TIME) {
        update_screen_state(LEFT_BTN);
      }
    }
  }
  if (mid_btn_read != mid_btn_state) {
    mid_btn_state = mid_btn_read;
    if (mid_btn_state == LOW) { //Pressing
      mid_timer = millis();
    }
    else {
      if (millis() - mid_timer > PRESS_DEBONCE_TIME) {
        update_screen_state(MID_BTN);
      }
    }
  }
  if (right_btn_read != right_btn_state) {
    right_btn_state = right_btn_read;
    if (right_btn_state == LOW) { //Pressing
      right_timer = millis();
    }
    else {
      if (millis() - right_timer > PRESS_DEBONCE_TIME) {
        update_screen_state(RIGHT_BTN);
      }
    }
  }

}

