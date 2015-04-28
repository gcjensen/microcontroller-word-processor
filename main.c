#include "os.h"

int check_switches(int);
char* uppercaseAlphabet = " A B C D E F G H I J K L M N O P Q R S T U V W X Y Z ";
char* alphabet = "[a]b c d e f g h i j k l m n o p q r s t u v w x y z \n _ < ";
int position = 1;
char text[100];
int textPosition = 0;

FIL File;  						/* FAT File */

void main(void) {
    os_init();
    os_add_task( check_switches,  100, 1);

    sei();
    display.y = display.height - 20;
    display_string(alphabet);
    display_string("\n");
    for(;;){}

}

void printKeyboard(int direction) {
  clear_screen();
  display.y = display.height - 20;
  alphabet[position - 1] = ' ';
  alphabet[position + 1] = ' ';
  position = position + direction;
  alphabet[position - 1] = '[';
  alphabet[position + 1] = ']';
  display_string(alphabet);
  display_string("\n");
  display.y = 0;
  display_string(text);
}

int check_switches(int state) {

  if (get_switch_rpt(_BV(SWE))) {
       printKeyboard(2);
  }

  if (get_switch_rpt(_BV(SWW))) {
       printKeyboard(-2);
	}

	if (get_switch_press(_BV(SWS))) {}

  if (get_switch_press(_BV(SWN))) {}

	if (get_switch_short(_BV(SWC))) {
      if (alphabet[position] == '_') {
        text[textPosition] = ' ';
        textPosition++;
      } else if (alphabet[position] == '<') {
        textPosition--;
        text[textPosition] = ' ';
      } else {
        text[textPosition] = alphabet[position];
        textPosition++;
      }
      display.y = 0;
      printKeyboard(0);
	}

	return state;
}
