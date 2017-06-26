// Code provided by Smoke And Wires
// http://www.smokeandwires.co.nz
// This code has been taken from the Adafruit TFT Library and modified
//  by us for use with our TFT Shields / Modules
// For original code / licensing please refer to
// https://github.com/adafruit/TFTLCD-Library

#include <Adafruit_GFX.h>    // Core graphics library
#include "SWTFT.h" // Hardware-specific library
#include <SoftwareSerial.h>

#include <SPI.h>
#include <SD.h>


// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
// double up the pins with the touch screen (see the TFT paint example).
// #define LCD_CS A3 // Chip Select goes to Analog 3
// #define LCD_CD A2 // Command/Data goes to Analog 2
// #define LCD_WR A1 // LCD Write goes to Analog 1
// #define LCD_RD A0 // LCD Read goes to Analog 0

// #define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

// When using the BREAKOUT BOARD only, use these 8 data lines to the LCD:
// For the Arduino Uno, Duemilanove, Diecimila, etc.:
//   D0 connects to digital pin 8  (Notice these are
//   D1 connects to digital pin 9   NOT in order!)
//   D2 connects to digital pin 2
//   D3 connects to digital pin 3
//   D4 connects to digital pin 4
//   D5 connects to digital pin 5
//   D6 connects to digital pin 6
//   D7 connects to digital pin 7
// For the Arduino Mega, use digital pins 22 through 29
// (on the 2-row header at the end of the board).

// Assign human-readable names to some common 16-bit color values:
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

SWTFT tft;
// If using the shield, all control and data lines are fixed, and
// a simpler declaration can optionally be used:
// SWTFT tft;

#define PRIMARY_COLOR     WHITE
#define SECONDARY_COLOR   GREEN
#define ERROR_COLOR       RED

#define RENGLONES 15
#define CARACTERES 27

int whiteIDX = 0;
int greenIDX = 0;
char datoWHITE[CARACTERES];
char datoGREEN[CARACTERES];
char dato;
int i;

int rowCount = 0;


SoftwareSerial  secondSerial(A5, 10);



const int SD_chipSelect = 10;


void setup(void) {
  Serial.begin(9600); 
  secondSerial.begin(9600); 
  tft.reset();
  
  uint16_t identifier = tft.readID();
  Serial.print(F("LCD driver chip: "));
  Serial.println(identifier, HEX);
    
  tft.begin(identifier);
  
  tft.setRotation(1);               // Rotatio 0-3
  tft.fillScreen(BLACK);
  tft.fillScreen(BLACK);


  // SD
  // see if the card is present and can be initialized:
  if (!SD.begin(SD_chipSelect)) {
    tft.setTextColor(ERROR_COLOR); 
    tft.setTextSize(2);
    tft.println("Error ini SD");
    
    // don't do anything more:
    return;
  }
}




void loop(void) {
    if (rowCount == RENGLONES){
      rowCount = 0;
      tft.fillScreen(BLACK);
      tft.setCursor(0, 0);
    }

  
    if (Serial.available()){
      dato = Serial.read();
      if (dato != 0x0D && dato != 0x0A){
        datoWHITE[whiteIDX] = dato;

        whiteIDX++;
        if (whiteIDX == CARACTERES){
          whiteIDX = CARACTERES - 1;
        }
      }

      if (dato == 0x0D && whiteIDX > 0){
        datoWHITE[whiteIDX] = 0;
        i = 0;
        tft.setTextColor(PRIMARY_COLOR); 
        tft.setTextSize(2); 
        while(i < whiteIDX-1){
          tft.print(datoWHITE[i]);
          i++;
        }
        tft.println(datoWHITE[i]);
        whiteIDX = 0;

        rowCount++;

        // Log on SD
        // open the file. note that only one file can be open at a time,
        // so you have to close this one before opening another.
        File dataFile = SD.open("smonitor.txt", FILE_WRITE);
        if (dataFile) {
          // if the file is available, write to it:
          dataFile.println(String(datoWHITE));
          dataFile.close();
        }else {
          // if the file isn't open, pop up an error:
          tft.setTextColor(ERROR_COLOR); 
          tft.setTextSize(2);
          tft.println("Error wrtite SD");
        }
        
      }
    }


    if (secondSerial.available()){
      dato = secondSerial.read();
      if (dato != 0x0D && dato != 0x0A){
        datoGREEN[greenIDX] = dato;

        greenIDX++;
        if (greenIDX == CARACTERES){
          greenIDX = CARACTERES - 1;
        }
      }

      if (dato == 0x0D && greenIDX > 0){
        datoGREEN[greenIDX] = 0;
        i = 0;
        tft.setTextColor(SECONDARY_COLOR); 
        tft.setTextSize(2); 
        while(i < greenIDX-1){
          tft.print(datoGREEN[i]);
          i++;
        }
        tft.println(datoGREEN[i]);
        greenIDX = 0;

        rowCount++;


        // Log on SD
        // open the file. note that only one file can be open at a time,
        // so you have to close this one before opening another.
        File dataFile = SD.open("smonitor.txt", FILE_WRITE);
        if (dataFile) {
          // if the file is available, write to it:
          dataFile.println(String(datoGREEN));
          dataFile.close();
        }else {
          // if the file isn't open, pop up an error:
          tft.setTextColor(ERROR_COLOR); 
          tft.setTextSize(2);
          tft.println("Error wrtite SD");
        }
        
      }
    }
}
