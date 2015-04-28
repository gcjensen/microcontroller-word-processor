#include "os.h"

//int blink(int);
//int update_dial(int);
//int collect_delta(int);
int check_switches(int);
char* uppercaseAlphabet = " A B C D E F G H I J K L M N O P Q R S T U V W X Y Z ";
char* alphabet = "[a]b c d e f g h i j k l m n o p q r s t u v w x y z \n _ < ";
int position = 1;
char text[100];
int textPosition = 0;


FIL File;  						/* FAT File */

volatile int8_t delta;



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
  //alphabet[position] = staticAlphabet[position];
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

	if (get_switch_press(_BV(SWN))) {
			display_string("Up\n");
	}

	if (get_switch_press(_BV(SWE))) {
      printKeyboard(2);
	}

	if (get_switch_press(_BV(SWS))) {
			display_string("Down\n");
	}

	if (get_switch_press(_BV(SWW))) {
      printKeyboard(-2);
	}

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

  if (get_switch_long(_BV(SWC))) {
      if (alphabet[position] == '_') {
        text[textPosition] = ' ';
        textPosition++;
      } else if (alphabet[position] == '<') {
        textPosition--;
        text[textPosition] = ' ';
      } else {
        text[textPosition] = uppercaseAlphabet[position];
        textPosition++;
      }
      display.y = 0;
      printKeyboard(0);

  }


	// if (get_switch_rpt(_BV(SWN))) {
	// 		display_string("[R] North\n");
	// }
  //
	// if (get_switch_rpt(_BV(SWE))) {
	// 		display_string("[R] East\n");
	// }
  //
	// if (get_switch_rpt(_BV(SWS))) {
	// 		display_string("[R] South\n");
	// }
  //
	// if (get_switch_rpt(_BV(SWW))) {
	// 		display_string("[R] West\n");
	// }
  //
	// if (get_switch_rpt(SWN)) {
	// 		display_string("[R] North\n");
	// }
  //
  //
	// if (get_switch_long(_BV(OS_CD))) {
	// 	display_string("Detected SD card.\n");
	// }

	return state;
}

// ISR( TIMER0_COMPA_vect ) {
//     static int8_t last;
//     int8_t new, diff;
//     uint8_t wheel;
//
//
//     /*
//       Scan rotary encoder
//       ===================
//       This is adapted from Peter Dannegger's code available at:
//       http://www.mikrocontroller.net/articles/Drehgeber
//     */
//
//     wheel = PINE;
//     new = 0;
//     if( wheel  & _BV(PE4) ) new = 3;
//     if( wheel  & _BV(PE5) )
//     new ^= 1;                  /* convert gray to binary */
//     diff = last - new;         /* difference last - new  */
//     if( diff & 1 ){            /* bit 0 = value (1) */
//        last = new;                /* store new as next last  */
//        delta = (diff & 2) - 1;   /* bit 1 = direction (+/-) */
//        if (delta < 0) {
//          printKeyboard(2);
//        } else {
//          printKeyboard(-2);
//        }
//     }
// }
