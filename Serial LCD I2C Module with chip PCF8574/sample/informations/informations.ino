/*
   Informations used:
   https://www.arduino.cc/en/reference/wire
   https://bitbucket.org/fmalpartida/new-liquidcrystal/wiki/Home
   https://bitbucket.org/fmalpartida/new-liquidcrystal/wiki/schematics#!board-layout
   https://bitbucket.org/fmalpartida/new-liquidcrystal/wiki/schematics#!i2c-connection
   https://bitbucket.org/fmalpartida/new-liquidcrystal/wiki/schematics#!configurable-i2c-address-modules
   https://github.com/arduino/Arduino
   https://github.com/arduino/ArduinoCore-avr
   https://github.com/arduino/ArduinoCore-avr/blob/master/cores/arduino/Print.h
   https://github.com/arduino/ArduinoCore-avr/blob/master/cores/arduino/Print.cpp
   https://www.arduino.cc/reference/en/language/variables/data-types/unsignedchar/
   https://www.arduino.cc/reference/en/language/functions/random-numbers/random/
   https://www.arduino.cc/reference/en/language/functions/random-numbers/randomseed/
   https://www.arduino.cc/en/Reference/LiquidCrystalCreateChar
   http://omerk.github.io/lcdchargen/
   https://www.youtube.com/watch?v=Lkvu3Qug_PM

   Arduino      Module
   A4  (SDA)    SDA
   A5  (SCL)    SCL
   VCC (5V)     VCC
   GND (0V)     GND
*/

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define NUMBER_COLUMNS 16
#define NUMBER_ROWS    2
#define CHAR_SIZE      LCD_5x8DOTS

#define LCD_ADDR       0x3F
#define PIN_EN         2
#define PIN_RW         1
#define PIN_RS         0

#define PIN_D4         4
#define PIN_D5         5
#define PIN_D6         6
#define PIN_D7         7

#define PIN_BACKLIGHT  3

// LiquidCrystal_I2C::LiquidCrystal_I2C( uint8_t LCD_ADDR, uint8_t EN, uint8_t RW, uint8_t RS,
//                                       uint8_t D4, uint8_t D5, uint8_t D6, uint8_t D7,
//                                       uint8_t backlighPin, t_backlightPol pol = POSITIVE )
// Class constructor.
// Initializes class variables and defines the I2C address, back lighi pin and backlight polarity control (POSITIVE, NEGATIVE) of the LCD.
// The constructor does not initialize the LCD.
// Parameters
//     lcd_Addr[in]     I2C address of the IO expansion module. For I2CLCDextraIO, the address can be configured using the on board jumpers.
//     ...
//     backlighPin[in]  pin associated to backlight control.
//     pol[in]          backlight polarity control (POSITIVE, NEGATIVE)

// IMPORTANT: THIS SEQUENCE OF PINS IS
// PIN                  FUNCTION
// VSS ( 1)          -> 0V (Power Supply)
// VDD ( 2)          -> +5V (Power Supply)
// V0  ( 3)          -> for LCD (Power Supply)
// RS  ( 4)[0]       -> Register Select: H:Data Input L:Instruction Input
// RW  ( 5)[1]       -> HIGH=Read / LOW=Write
// E   ( 6)[2]       -> Enable Signal
// D0  ( 7)[0]       -> \
// D1  ( 8)[1]       -> | D0 ... D3
// D2  ( 9)[2]       -> | Data bus used in 8 bit transfer
// D3  (10)[3]       -> /
// D4  (11)[4]       -> \
// D5  (12)[5]       -> | D4 ... D7
// D6  (13)[6]       -> | Data bus for both 4 and 8 bit transfer
// D7  (14)[7]       -> /
// BLA (15)          -> BLACKLIGHT +5V
// BLK (16)          -> BLACKLIGHT 0V
LiquidCrystal_I2C lcd( LCD_ADDR, PIN_EN, PIN_RW, PIN_RS, PIN_D4, PIN_D5, PIN_D6, PIN_D7, PIN_BACKLIGHT, POSITIVE );

byte dataMobile[8] = {
  0b10001,
  0b10011,
  0b10101,
  0b10001,
  0b10001,
  0b10101,
  0b11001,
  0b10001
};

byte note[8] = {
  0b00000,
  0b00011,
  0b00101,
  0b01001,
  0b01001,
  0b01011,
  0b11011,
  0b11000
};

byte notification[8] = {
  0b00000,
  0b00100,
  0b01110,
  0b01110,
  0b01110,
  0b11111,
  0b00000,
  0b00100
};

byte sound[8] = {
  0b00001,
  0b00011,
  0b00111,
  0b11111,
  0b11111,
  0b00111,
  0b00011,
  0b00001
};

byte power[8] = {
  0b01010,
  0b01010,
  0b11111,
  0b10001,
  0b10001,
  0b01110,
  0b00100,
  0b00100
};

void verifyProcess();

void setup(){
  // void LiquidCrystal_I2C::begin ( uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS ) 
  // LCD initialization and associated HW.
  // Initializes the LCD to a given size (col, row) OR (COL, ROW, CHAR_SIZE).
  // This methods initializes the LCD, therefore, it MUST be called prior to using any other method from this class or parent class.
  // The begin method can be overloaded if necessary to initialize any HW that is implemented by a library and can't be done during construction, here we use the Wire class.
  // Parameters
  //     cols[in]      the number of columns that the display has
  //     rows[in]      the number of rows that the display has
  //     charsize[in]  size of the characters of the LCD: LCD_5x8DOTS or LCD_5x10DOTS.
  // Reimplemented from LCD.
  lcd.begin( NUMBER_COLUMNS, NUMBER_ROWS, CHAR_SIZE );

  // void LCD::createChar ( uint8_t location, uint8_t charmap[] )
  // Creates a custom character for use on the LCD.
  // Create a custom character (glyph) for use on the LCD.
  // Most chipsets only support up to eight characters of 5x8 pixels.
  // Therefore, this methods has been limited to locations (numbered 0 to 7).
  // The appearance of each custom character is specified by an array of eight bytes, one for each row.
  // The five least significant bits of each byte determine the pixels in that row.
  // To display a custom character on screen, write()/print() its number, i.e. lcd.print (char(x)); // Where x is 0..7.
  // Parameters
  //     location[in]  LCD memory location of the character to create (0 to 7)
  //     charmap[in]   the bitmap array representing each row of the character.
  lcd.createChar( 0, (uint8_t *)dataMobile );
  lcd.createChar( 1, (uint8_t *)note );
  lcd.createChar( 2, (uint8_t *)notification );
  lcd.createChar( 3, (uint8_t *)sound );
  lcd.createChar( 4, (uint8_t *)power );
  
  // void LiquidCrystal_I2C::setBacklight ( uint8_t value )
  // Switch-on/off the LCD backlight.
  // Switch-on/off the LCD backlight.
  // The setBacklightPin has to be called before setting the backlight for this method to work.
  // See also setBacklightPin.
  // Parameters
  //     value backlight mode (HIGH|LOW)
  // Reimplemented from LCD.
  lcd.setBacklight( HIGH );

  // void LCD::home ( )
  // Sets the cursor to the upper-left corner.
  // Positions the cursor in the upper-left of the LCD.
  // That is, use that location in outputting subsequent text to the display.
  // To also clear the display, use the clear() function instead.
  // This operation is time consuming for the LCD.
  // Parameters
  //     none
  lcd.home();

  // void LCD::clear  (   )
  // Clears the LCD. Clears the LCD screen and positions the cursor in the upper-left corner.
  // This operation is time consuming for the LCD.
  // Parameters
  //     none
  lcd.clear();

  // size_t Print::print( const __FlashStringHelper *ifsh );
  // size_t Print::print( const String &s );
  // size_t Print::print( const char str[] );
  // size_t Print::print( char c );
  // size_t Print::print( unsigned char b, int base=DEC );
  // size_t Print::print( int n, int base=DEC );
  // size_t Print::print( unsigned int n, int base=DEC );
  // size_t Print::print( long n, int base=DEC );
  // size_t Print::print( unsigned long b, int base=DEC );
  // size_t Print::print( double n, int digits=2 );
  // size_t Print::print( const Printable& x );
  lcd.print( "Initializing..." );

  // void LCD::setCursor ( uint8_t col, uint8_t row )
  // Position the LCD cursor.
  // Sets the position of the LCD cursor.
  // Set the location at which subsequent text written to the LCD will be displayed.
  // Parameters
  //      col[in] LCD column
  //      row[in] LCD row - line.
  lcd.setCursor( 0, 1 );
  
  for( unsigned char i=0; i<NUMBER_COLUMNS; i++ ){
    delay( random(1000, 2000) );
    // void LCD::write ( uint8_t value )
    // Writes to the LCD. This method writes character to the LCD in the current cursor position.
    // This is the virtual write method, implemented in the Print class, therefore all Print class methods will end up calling this method.
    // Parameters
    //     value[in] Value to write to the LCD.
    lcd.write( byte(0b11111111) );
  }
  delay( 1000 );

  lcd.home();
  lcd.clear();
  lcd.print( "Process: OK" );
  delay( 1000 );
  
  lcd.home();
  lcd.clear();
  lcd.print( "Welcome to" );
  lcd.setCursor( 0, 1 );
  lcd.print( "everton.mat.br" );
  delay( 3000 );
}

void loop(){
  verifyProcess();
  delay( 3000 );
}

void verifyProcess(){
  lcd.home();
  lcd.clear();
  lcd.print( "Verify System:" );
  lcd.setCursor( 0, 1 );
  lcd.write( byte(0) );
  lcd.print( "  " );
  lcd.write( byte(1) );
  lcd.print( "  " );
  lcd.write( byte(2) );
  lcd.print( "  " );
  lcd.write( byte(3) );
  lcd.print( "  " );
  lcd.write( byte(4) );
  delay( 5000 );
  
  lcd.home();
  lcd.clear();
  lcd.write( byte(0) );
  lcd.setCursor( 0, 1 );
  lcd.print( "Data Mobile: OK" );
  delay( 2000 );

  lcd.home();
  lcd.clear();
  lcd.write( byte(1) );
  lcd.setCursor( 0, 1 );
  lcd.print( "Musics: OK" );
  delay( 2000 );

  lcd.home();
  lcd.clear();
  lcd.write( byte(2) );
  lcd.setCursor( 0, 1 );
  lcd.print( "Notification: OK" );
  delay( 2000 );

  lcd.home();
  lcd.clear();
  lcd.write( byte(3) );
  lcd.setCursor( 0, 1 );
  lcd.print( "Sounds: OK" );
  delay( 2000 );

  lcd.home();
  lcd.clear();
  lcd.write( byte(4) );
  lcd.setCursor( 0, 1 );
  lcd.print( "Power: OK" );
  delay( 2000 );
}
