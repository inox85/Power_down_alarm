
void update_clock () {

  clock_timer = (millis() - current_millis);
  minuti = (clock_timer / 60000) % 60;
  ore = (minuti / 60) % 9;

  display.setTextSize(2);
  display.setCursor(0, 50);
  display.print(ore);

  if ((clock_timer / 500) % 2) {
    display.print(':');
  }
  else {
    display.print(' ');
  }

  if (minuti < 10) {
    display.print('0');
    display.print(minuti);
  }
  else {
    display.print(minuti);
  }

}
