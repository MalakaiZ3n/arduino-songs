/* 
  Never Gonna Give you Up
  Connect a piezo buzzer or speaker to pin 11 or select a new pin.
                                          
                                              
                                              Robson Couto, 2019
*/
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27,16,2);
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define REST      0
#define TOTAL_NOTES 95


// change this to make the song slower or faster
int tempo = 122;

// change this to whichever pin you want to use
int buzzer = 13;

// the LED outputs
//int yellowLED2 = 2; //Yellow LED to Pin 2
//int greenLED3 = 3; //Green LED to Pin 3
//int blueLED4 = 4; //Blue LED to Pin 4
//int redLED5 = 5; //Red LED to pin 5


// notes of the melody followed by the duration.
// a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
// !!negative numbers are used to represent dotted notes,
// so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!
int melody[] = {

  // Never Gonna Give You Up
  
//  NOTE_D5,-4, NOTE_E5,-4, NOTE_A4,4,//1
//  NOTE_E5,-4, NOTE_FS5,-4, NOTE_A5,16, NOTE_G5,16, NOTE_FS5,8,
//  NOTE_D5,-4, NOTE_E5,-4, NOTE_A4,2,
//  NOTE_A4,16, NOTE_A4,16, NOTE_B4,16, NOTE_D5,8, NOTE_D5,16,
//  NOTE_D5,-4, NOTE_E5,-4, NOTE_A4,4, //repeat from 1
//  NOTE_E5,-4, NOTE_FS5,-4, NOTE_A5,16, NOTE_G5,16, NOTE_FS5,8,
//  NOTE_D5,-4, NOTE_E5,-4, NOTE_A4,2,
//  NOTE_A4,16, NOTE_A4,16, NOTE_B4,16, NOTE_D5,8, NOTE_D5,16,
//  REST,4, NOTE_B4,8, NOTE_CS5,8, NOTE_D5,8, NOTE_D5,8, NOTE_E5,8, NOTE_CS5,-8,
//  NOTE_B4,16, NOTE_A4,2, REST,4, 
//
//  REST,8, NOTE_B4,8, NOTE_B4,8, NOTE_CS5,8, NOTE_D5,8, NOTE_B4,4, NOTE_A4,8, //7
//  NOTE_A5,8, REST,8, NOTE_A5,8, NOTE_E5,-4, REST,4, 
//  NOTE_B4,8, NOTE_B4,8, NOTE_CS5,8, NOTE_D5,8, NOTE_B4,8, NOTE_D5,8, NOTE_E5,8, REST,8,
//  REST,8, NOTE_CS5,8, NOTE_B4,8, NOTE_A4,-4, REST,4,
//  REST,8, NOTE_B4,8, NOTE_B4,8, NOTE_CS5,8, NOTE_D5,8, NOTE_B4,8, NOTE_A4,4,
//  NOTE_E5,8, NOTE_E5,8, NOTE_E5,8, NOTE_FS5,8, NOTE_E5,4, REST,4,

  // Bohemian Rhapsody - Queen
   
  NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5, REST,
  NOTE_E5, NOTE_E5, NOTE_F5, NOTE_E5, NOTE_D5, NOTE_C5, REST,
  NOTE_F5, NOTE_F5, NOTE_F5, NOTE_G5, NOTE_F5,
  NOTE_F4, NOTE_F4, NOTE_F5, NOTE_F5, NOTE_G5, NOTE_F5, NOTE_DS5, NOTE_D5, REST,

  NOTE_D5, NOTE_D5, NOTE_D5, NOTE_D5, NOTE_D5, REST,
  NOTE_D5, NOTE_D5, NOTE_DS5, NOTE_F5, NOTE_AS4, NOTE_G5, REST, REST,
  NOTE_G5, NOTE_G5, NOTE_G5, NOTE_G5, NOTE_G5, REST,
  NOTE_F5, NOTE_F5, NOTE_G5, NOTE_F5, NOTE_DS5, NOTE_C5,
  REST, NOTE_D5, NOTE_DS5,
  
  NOTE_B5, NOTE_B5, NOTE_AS5, REST,
  NOTE_A5, NOTE_A5, NOTE_AS5, REST,
  NOTE_B5, NOTE_B5, NOTE_AS5, REST,
  NOTE_A5, NOTE_A5, NOTE_AS5, REST,

  NOTE_G5, NOTE_G5, NOTE_G5, NOTE_G5, NOTE_F5, NOTE_AS5,
  NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_F5,
  NOTE_C5, NOTE_A4, NOTE_F5, REST,

  NOTE_A4, NOTE_AS4, NOTE_AS4,

  NOTE_G6, NOTE_F6, REST, NOTE_G6, NOTE_F6,
};

// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// there are two values per note (pitch and duration), so for each note there are four bytes
int notes = sizeof(melody) / sizeof(melody[0]) / 2;

// this calculates the duration of a whole note in ms
int wholenote = (60000 * 4) / tempo;

int divider = 0, noteDuration = 0;

void setup() {
  lcd.begin();  //initialize the lcd
  lcd.backlight();  //open the backlight 
  lcd.setCursor(0,0);
  lcd.print("BohemianRhapsody");
  lcd.setCursor(5,1);
  lcd.print("Queen");

 
  // iterate over the notes of the melody.
  // Remember, the array is twice the number of notes (notes + durations)
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {

    // calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(buzzer, melody[thisNote], noteDuration * 0.9);

    // Wait for the specief duration before playing the next note.
    delay(noteDuration);

    // stop the waveform generation before the next note.
    noTone(buzzer);
  }
}

void loop() {
  // no need to repeat the melody.
}
