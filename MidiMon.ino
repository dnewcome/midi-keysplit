/*
 Arduino MIDI monitor
 Copyright 2018 Dan Newcome
 Provided under the BSD license

 Uses a Hitachi HD44780 16x2 LCD display.

 Conections:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
*/

#include <LiquidCrystal.h>
#include <MIDI.h>
#include <RotaryEncoder.h>

MIDI_CREATE_DEFAULT_INSTANCE();
RotaryEncoder encoder(A2, A3);

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

enum mode_type {
  setLo,
  setHi,
  split,
  disp
};

int pos = 0;
int chan_lo = 0;
int chan_hi = 1;
mode_type mode = disp;

void handleNoteOn(byte channel, byte pitch, byte velocity)
{
    if (pitch > pos) {
      lcd.setCursor(0, 1);
      lcd.print("To ch" + String(channel + 1));
      MIDI.sendNoteOn(pitch, velocity, channel + 1);    // Send a Note (pitch 42, velo 127 on channel 1)
      
    }
    else {
      lcd.setCursor(0, 1);
      lcd.print("To ch" + String(channel));
      MIDI.sendNoteOn(pitch, velocity, channel);    // Send a Note (pitch 42, velo 127 on channel 1)
    }
}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
    if (pitch > pos) {
      lcd.setCursor(0, 1);
      lcd.print("To ch" + String(channel + 1));
      MIDI.sendNoteOff(pitch, velocity, channel + 1);    // Send a Note (pitch 42, velo 127 on channel 1)  
    }
    else {
      lcd.setCursor(0, 1);
      lcd.print("To ch" + String(channel));
      MIDI.sendNoteOff(pitch, velocity, channel);    // Send a Note (pitch 42, velo 127 on channel 1)
    }
}

// note character that I made up
byte dan_note[8] = {
  B00001,
  B00001,
  B00001,
  B11101,
  B11111,
  B01111,
  B00110,
};

// this is the glyph used by Access
byte virus_note[8] = {
  B00001,
  B00001,
  B00001,
  B00001,
  B00111,
  B01111,
  B01110,
};


void setup() {
  // set up the LCD's number of columns and rows:
  lcd.createChar(0, dan_note);
  lcd.createChar(1, virus_note);
  lcd.begin(16, 2);
  lcd.print("MIDI monitor");
  pinMode(A1,INPUT_PULLUP);

  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.begin(MIDI_CHANNEL_OMNI);
}

void loop() {
  
  
  MIDI.read();
  encoder.tick();
  int newPos = encoder.getPosition(); 
  
  if (pos != newPos) {
    lcd.setCursor(10, 1);
    lcd.print("    ");
    lcd.setCursor(10, 1);
    lcd.print(newPos);
    pos = newPos;
  }
  
  lcd.setCursor(14, 1);
  
  if(digitalRead(A1) == HIGH) {
    lcd.write(byte(0));
    lcd.setCursor(15, 1);
    lcd.write(byte(1));
  }
  else {
    lcd.print("L");
  }
  
}

