#include "os.h"
#include <string.h>

#define SPACE	6
#define LINE_BUFF_LEN 55


int check_switches(int);
void printKeyboard(int);
void calibrate();
void printSavedLine();
void fillArrays();
void saveLine();
void saveCharacter(char);
void removeCharacter();
void tail(uint8_t lines);   /* Show last lines of a file */

char* uppercaseAlphabet = " A B C D E F G H I J K L M N O P Q R S T U V W X Y Z ";
char* alphabet = "[a]b c d e f g h i j k l m n o p q r s t u v w x y z \n _ < > . , ! ? 1 2 3 4 5 6 7 8 9 0 ";
int selector = 1;
struct cursors cursor;
char currentLine[20];
char lines[20][20];

FIL File;

struct cursors {
  int xPosition;
  int yPosition;
};

void main(void) {
    os_init();
    os_add_task( check_switches,  100, 1);
    sei();
    fillArrays();
    printKeyboard(0);
    for(;;){}
}

void printKeyboard(int direction) {
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

  if (get_switch_press(_BV(SWN))) {
    char nums[] = "0123456789aaaaaaaaaaaaaaaaaaaa";
    f_mount(&FatFs, "", 0);
		if (f_open(&File, "myfile.txt", FA_WRITE | FA_OPEN_ALWAYS) == FR_OK) {
      int i;
      for (i = 0; i < 20; i++) {
        f_puts("                    ", &File);
        f_puts("\r\n", &File);
      }
      f_close(&File);
      if (f_open(&File, "myfile.txt", FA_WRITE | FA_OPEN_ALWAYS) == FR_OK) {
        for (i = 0; i < 20; i++) {
          f_puts(lines[i], &File);
          f_puts("\r\n", &File);
        }
        display_string("Write successful \n");
			  f_close(&File);
      } else {
        display_string("Can't write file! \n");
      }
		} else {
			display_string("Can't write file! \n");
		}
  }

  if (get_switch_press(_BV(SWS))) {
      //printSavedFile();
      tail(20);
  }

  if (get_switch_press(_BV(SWE))) {
      printKeyboard(2);
  }

  if (get_switch_rpt(_BV(SWE))) {
       printKeyboard(2);
  }

  if (get_switch_press(_BV(SWW))) {
      printKeyboard(-2);
  }

  if (get_switch_rpt(_BV(SWW))) {
       printKeyboard(-2);
	}

	if (get_switch_press(_BV(SWS))) {
  }

	if (get_switch_short(_BV(SWC))) {
      if (alphabet[selector] == '_') {
        saveCharacter(' ');
        cursor.xPosition += SPACE;
      } else if (alphabet[selector] == '<') {
        cursor.xPosition -= SPACE;
        removeCharacter(); //implement this
        calibrate();
        display_char(' ');
      } else if (alphabet[selector] == '>') {
        saveLine();
        cursor.xPosition = 0;
        cursor.yPosition += 8;
      } else {
        saveCharacter(alphabet[selector]);
        display_char(alphabet[selector]);
        cursor.xPosition += SPACE;
      }
      calibrate();
	}

  if (get_switch_long(_BV(OS_CD))) {
		display_string("Detected SD card.\n");
	}

	return state;
}

void calibrate() {
  display.x = cursor.xPosition;
  display.y = cursor.yPosition;
}

void printSavedFile() {
  display.x = 0;
  display.y = 0;
  int i;
  for (i = 0; i < 20; i++) {
    display_string(lines[i]);
    display.x = 0;
    display.y += 4;
  }
  cursor.xPosition = display.x;
  cursor.yPosition = display.y;
}

void fillArrays() {
  int i;
  for (i = 0; i < 20; i++){
    currentLine[i] = ' ';
    //lines[i] = " ";
    strcpy(lines[i], " ");

  }
}

void saveLine() {
  int i;
  for (i = 0; i < 20; i++) {
    if (strcmp(lines[i], " ") == 0) {
      //lines[i] = currentLine;
      strcpy(lines[i], currentLine);
      // display_string("\n");
      // display_string(lines[0]);
      // display_string("\n");
      for (i = 0; i < 20; i++){
        currentLine[i] = ' ';
      }
      return;
    }
  }
}

void saveCharacter(char c) {
  int i;
  for (i = 0; i < 20; i++) {
    if (currentLine[i] == ' ') {
      currentLine[i] = c;
      return;
    }
  }
}

void removeCharacter() {
  //implement this
}

void tail(uint8_t n) {
  char line[20][LINE_BUFF_LEN];

	f_mount(&FatFs, "", 0);
	if (f_open(&File, "myfile.txt", FA_READ) == FR_OK) {
    int i;
    for (i = 0; i < 20; i++) {
  	   f_gets(line[i], LINE_BUFF_LEN, &File);
       display_string(line[i]);
       display_string("\n");
		}
		f_close(&File);
	} else {
		display_string("Can't read file! \n");
	}

}
