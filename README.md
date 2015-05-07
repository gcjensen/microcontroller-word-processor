# LaFortuna-Word-Processor
Simple Word processor for the LaFortuna 8-bit AVR microcontroller board, developed in C.

Controls:

  - Left/right arrows scroll through the keyboard
  - Centre button selects letter/symbol
  - Up arrow writes current text to the SD card
  - Down arrow reads from the SD card and prints to the display
  - _ adds a space
  - < backspaces
  - > moves cursor onto a new line

Built on Klaus-Peter Zauner's FortanaOS, implementing the following libraries:

  - Peter Dannegger's Code for Switch debouncing
  - Steve Gunn's display driver
  - ChanN's FAT File System
  - The RIOS scheduler

Known bugs:

  - Strange symbols sometimes appear when reading from
    the SD card
  - After reading from the SD card the device has to be
    restarted to be able to enter text again
