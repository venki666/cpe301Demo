// Counts for how long the machine has been running

#define F_CPU 16000000UL

#include <avr/interrupt.h>
#include <avr/io.h>
#include <string.h>
#include <util/delay.h>

#define keypadDirectionRegisterR DDRB
#define keypadPortControlR PORTB
#define keypadPortValueR PINB

#define keypadDirectionRegisterC DDRC
#define keypadPortControlC PORTC
#define keypadPortValueC PINC

#define ctrl PORTD // We are using port D
#define en 2       // enable signal pin 2
#define rw 1       // read/write signal pin 1
#define rs 0       // register select signal pin 0

void lcd_command(unsigned char cmd);
void lcd_init(void);
void lcd_data(unsigned char data);
void lcdCommand(char);
void lcdData(char);
void lcd_print(unsigned char *str);
void lcd_gotoxy(unsigned char x, unsigned char y);
void lcd_update_time(void);
void keypadScan(void);

int main() {
  DDRD = 0xFF; // Setting DDRD to output // setting for port D
  lcd_init();  // initialization of LCD function
  _delay_ms(30);

  lcd_gotoxy(1, 1);         // Go to the location 1,1 of lcd
  lcd_print("Press a key"); // Print the text
  lcd_gotoxy(1, 2);         // Go to the location 1,2 of lcd

  // Keypad initialization
  keypadDirectionRegisterR = (1<<0) | (1<<1) | (1<<2) | (1<<3);
  keypadDirectionRegisterC = (0<<0) | (0<<1) | (0<<2) | (0<<3);

  keypadPortControlR = (0<<0) | (0<<1) | (0<<2) | (0<<3);
  keypadPortControlC = (1<<0) | (1<<1) | (1<<2) | (1<<3);
  
  while (1) {
    keypadScan();
  };

  return 0;
}

void keypadScan(){
  
  // Store value for column
  uint8_t keyPressCodeC = keypadPortValueC;
  
      keypadDirectionRegisterC ^= (1<<0) | (1<<1) | (1<<2) | (1<<3);
      keypadDirectionRegisterR ^= (1<<0) | (1<<1) | (1<<2) | (1<<3); 
  
      keypadPortControlC ^= (1<<0) | (1<<1) | (1<<2) | (1<<3);
      keypadPortControlR ^= (1<<0) | (1<<1) | (1<<2) | (1<<3);
      
      _delay_ms(5);
  
      // Store value for row
      int temp = keypadPortValueR;
      uint8_t keyPressCodeR = temp << 4;
  
      // Add value for column and row
      uint8_t keyPressCode = keyPressCodeC | keyPressCodeR;
  
      uint8_t blinkDuration = 0;
  
      // Comparison and resultant action
  
      // Column one
      if(keyPressCode == 0b11101110){
        lcd_print("a");
      }
      if(keyPressCode == 0b11011110){
        lcd_print("b");
      }
      if(keyPressCode == 0b10111110){
        lcd_print("c");
      }
      if(keyPressCode == 0b01111110){
        lcd_print("d");
      }
  
      // Column two
      if(keyPressCode == 0b11101101){
        lcd_print("e");
      }
      if(keyPressCode == 0b11011101){
        lcd_print("f");
      }
      if(keyPressCode == 0b10111101){
        lcd_print("g");;
      }
      if(keyPressCode == 0b01111101){
        lcd_print("h");
      }
  
      // Column three
      if(keyPressCode == 0b11101011){
        lcd_print("i");
      }
      if(keyPressCode == 0b11011011){
        lcd_print("j");;
      }
      if(keyPressCode == 0b10111011){
        lcd_print("k");
      }
      if(keyPressCode == 0b01111011){
        lcd_print("l");
      }
  
      // Column four
      if(keyPressCode == 0b11100111){
        lcd_print("m");
      }
      if(keyPressCode == 0b11010111){
        lcd_print("n");
      }
      if(keyPressCode == 0b10110111){
        lcd_print("o");
      }
      if(keyPressCode == 0b01110111){
        lcd_print("p");
      }
  
}

// Function moving to a given position on the LCD screen
void lcd_gotoxy(unsigned char x, unsigned char y) {
  unsigned char firstCharAdr[] = {0x80, 0xC0, 0x94, 0xD4};
  lcdCommand(firstCharAdr[y - 1] + x - 1);
  _delay_ms(0.1);
}

void lcd_init(void) {
  lcdCommand(0x02); // To initialize LCD in 4-bit mode.
  _delay_ms(1);
  lcdCommand(0x28); // To initialize LCD in 2 lines, 5X7 dots and 4bit mode.
  _delay_ms(1);
  lcdCommand(0x01); // Clear LCD
  _delay_ms(1);
  lcdCommand(0x0E); // Turn on cursor ON
  _delay_ms(1);
  lcdCommand(0x80); // —8 go to first line and –0 is for 0th position
  _delay_ms(1);
  return;
}

void lcdCommand(char cmd_value) {
  char cmd_value1;
  cmd_value1 = cmd_value & 0xF0;          // Mask lower nibble
                                          // because PD4-PD7 pins are used.
  lcd_command(cmd_value1);                // Send to LCD
  cmd_value1 = ((cmd_value << 4) & 0xF0); // Shift 4-bit and mask
  lcd_command(cmd_value1);                // Send to LCD
}

void lcdData(char data_value) {
  char data_value1;
  data_value1 = data_value & 0xF0;          // Mask lower nibble
  lcd_data(data_value1);                    // because PD4-PD7 pins are used.
  data_value1 = ((data_value << 4) & 0xF0); // Shift 4-bit and mask
  lcd_data(data_value1);                    // Send to LCD
}

void lcd_command(unsigned char cmd) {
  ctrl = cmd;
  ctrl &= ~(1 << rs); // RS = 0 for command
  ctrl &= ~(1 << rw); // RW = 0 for write
  ctrl |= (1 << en);  // EN = 1 for High to Low pulse
  _delay_ms(1);
  ctrl &= ~(1 << en); // EN = 0 for High to Low pulse
  _delay_ms(40);
  return;
}

void lcd_data(unsigned char data) {
  ctrl = data;
  ctrl |= (1 << rs);  // RS = 1 for data
  ctrl &= ~(1 << rw); // RW = 0 for write
  ctrl |= (1 << en);  // EN = 1 for High to Low pulse
  _delay_ms(1);
  ctrl &= ~(1 << en); // EN = 0 for High to Low Pulse
  _delay_ms(40);
  return;
}

void lcd_print(
    unsigned char *str) { // store address value of the string in pointer *str
  int i = 0;
  while (str[i] !=
         '\0') {     // loop will go on till the NULL character in the string
    lcdData(str[i]); // sending data on LCD byte by byte
    i++;
  }
  return;
}
