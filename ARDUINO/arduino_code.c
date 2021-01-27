#include <LiquidCrystal.h>


#define MAX_MESSAGE 30


// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


void setup()
{
  Serial.begin(115200);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");

  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
}

void loop()
{
  static char buffer[MAX_MESSAGE];
  static unsigned char index = 0;
  char inch;
 
  while (Serial.available() > 0) {
    
    inch = Serial.read();
    if (inch == '\n') {
      Serial.print("You entered: ");
      Serial.println(buffer);
      lcd.print(buffer);
      buffer[0] = 0;
      index = 0;
    } else {       
      if (index < MAX_MESSAGE-1) {
        buffer[index++] = inch;
        buffer[index] = 0;
      }
    }
    
  }
}
