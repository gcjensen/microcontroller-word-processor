#include "os.h"

#define SPACE	6


int check_switches(int);
char* uppercaseAlphabet = " A B C D E F G H I J K L M N O P Q R S T U V W X Y Z ";
char* alphabet = "[a]b c d e f g h i j k l m n o p q r s t u v w x y z \n _ < > . , ! ? 1 2 3 4 5 6 7 8 9 0 ";
int selector = 1;
struct cursors cursor;

FIL File;

struct cursors {
  int xPosition;
  int yPosition;
};

void main(void) {
    os_init();
    os_add_task( check_switches,  100, 1);
    sei();
    printKeyboard(0);
    for(;;){}
}

void printKeyboard(int direction) {
  //clear_screen();
  display.x = 0;
  display.y = display.height - 30;
  alphabet[selector - 1] = ' ';
  alphabet[selector + 1] = ' ';
  selector += direction;
  alphabet[selector - 1] = '[';
  alphabet[selector + 1] = ']';
  display_string(alphabet);
  display_string("\n");
  calibrate();
}

int check_switches(int state) {

  if (get_switch_press(_BV(SWE))) {
      printKeyboard(2);
  }

  if (get_switch_rpt(_BV(SWE))) {
       printKeyboard(2);
  }

  if (get_switch_rpt(_BV(SWW))) {
       printKeyboard(-2);
	}

	if (get_switch_press(_BV(SWS))) {
  }

  if (get_switch_press(_BV(SWN))) {
  }

	if (get_switch_short(_BV(SWC))) {
      if (alphabet[selector] == '_') {
        cursor.xPosition += SPACE;
      } else if (alphabet[selector] == '<') {
        cursor.xPosition -= SPACE;
        calibrate();
        display_char(' ');
      } else if (alphabet[selector] == '>') {
        cursor.xPosition = 0;
        cursor.yPosition += 8;
      } else {
        display_char(alphabet[selector]);
        cursor.xPosition += SPACE;
      }
      calibrate();
	}

	return state;
}

void calibrate() {
  display.x = cursor.xPosition;
  display.y = cursor.yPosition;
}
