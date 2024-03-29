#include "os.h"
#include <string.h>

#define SPACE	6
#define LINE_BUFF_LEN 55
#define LINE_LENGTH 40

int check_switches(int);
void printKeyboard(int);
void calibrate();
void printSavedLine();
void fillArrays();
void saveLine();
void saveCharacter(char);
void removeCharacter();
void loadFile();
void writeToFile();
int isEven(int);

// [ ] denotes the currently selected letter, so starts on a
char* alphabet = "[a]b c d e f g h i j k l m n o p q r s t u v w x y z \n _ < > . , ! ? 1 2 3 4 5 6 7 8 9 0 ";
int selector = 1;
struct cursors cursor;
char currentLine[LINE_LENGTH];
char lines[LINE_LENGTH][LINE_LENGTH];

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
      saveLine();
      writeToFile();
  }

  if (get_switch_press(_BV(SWS))) {
      loadFile();
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

	if (get_switch_short(_BV(SWC))) {
      if (alphabet[selector] == '_') {
        saveCharacter(' ');
        cursor.xPosition += SPACE;
      } else if (alphabet[selector] == '<') {
        cursor.xPosition -= SPACE;
        removeCharacter();
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
	return state;
}

void calibrate() {
  display.x = cursor.xPosition;
  display.y = cursor.yPosition;
}

void fillArrays() {
  int i;
  for (i = 0; i < LINE_LENGTH; i++){
    currentLine[i] = ' ';
    strcpy(lines[i], " ");
  }
}

void saveLine() {
  int i;
  for (i = 0; i < LINE_LENGTH; i++) {
    if (strcmp(lines[i], " ") == 0) {
      strcpy(lines[i], currentLine);
      for (i = 0; i < LINE_LENGTH; i++){
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
  int i;
  for (i = LINE_LENGTH - 1; i >= 0; i--) {
    if (currentLine[i] != ' ') {
      currentLine[i] = ' ';
      return;
    }
  }
}

void writeToFile() {
  f_mount(&FatFs, "", 0);
  // file contents is initially wiped
  if (f_open(&File, "myfile.txt", FA_WRITE | FA_OPEN_ALWAYS) == FR_OK) {
    int i;
    for (i = 0; i < LINE_LENGTH; i++) {
      f_puts("                    ", &File);
      f_puts("\n", &File);
    }
    f_close(&File);
    if (f_open(&File, "myfile.txt", FA_WRITE | FA_OPEN_ALWAYS) == FR_OK) {
      for (i = 0; i < LINE_LENGTH; i++) {
        f_puts(lines[i], &File);
        f_puts("\n", &File);
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

void loadFile() {
  char line[LINE_LENGTH][LINE_BUFF_LEN];
  int8_t i;
  clear_screen();
  printKeyboard(0);

	f_mount(&FatFs, "", 0);
	if (f_open(&File, "myfile.txt", FA_READ) == FR_OK) {
    display.x = 0;
    display.y = 0;
    for (i = 0; i < LINE_LENGTH; i++) {
      f_gets(line[i], LINE_BUFF_LEN, &File);
      // skips over the strange blank lines in the file
      if (isEven(i)) {
        display_string(line[i]);
        display.x = 0;
      }
    }
    cursor.xPosition = display.x;
    cursor.yPosition = display.y;
		f_close(&File);
	} else {
		display_string("Can't read file! \n");
	}
}

int isEven(int i) {
  return ((i & 1) == 0);
}
