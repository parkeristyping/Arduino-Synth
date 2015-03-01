// WILLIAM THE SYNTHESIZER 
// By Parker Lawrence
// 
// Based on the Illutron synth library
// http://www.instructables.com/id/Turn-your-Arduino-into-a-4-voice-wavetable-synth-w/
//

#include <synth.h>
#include "MIDI_parser.h"

synth william;      // Make a synth
midiParser parser;  // Make a MIDI parser

void setup()
{
  Serial.begin(31250); // MIDI BAUD rate
  william.begin();     // Init synth
}

void loop()
{
  unsigned char voice;
  while(Serial.available())
  {
    if(parser.update(Serial.read())) // Feed MIDI stream to parser and execute commands
    {
      switch(parser.midi_cmd)
      {
        //*******************
        // Handle MIDI notes
        //*******************
        case 0x90: // Channel 1 (voice 0)
        case 0x91: // Channel 2 (voice 1)
        case 0x92: // Channel 3 (voice 2)
        case 0x93: // Channel 4 (voice 3)
        
          voice = parser.midi_cmd-0x90;
          if(parser.midi_2nd) // Velocity not zero (could implement NOTE_OFF here)
            william.mTrigger(voice,parser.midi_1st);
          break;
          
        //*************************
        // Handle MIDI controllers
        //*************************
        
        case 0xb0: // Channel 1 (voice 0)
        case 0xb1: // Channel 2 (voice 1)
        case 0xb2: // Channel 3 (voice 2)
        case 0xb3: // Channel 4 (voice 3)
        
          voice = parser.midi_cmd-0xb0;
          switch(parser.midi_1st) // Controller number
          {
          case 16: // Controller 16
            william.setWave(voice,parser.midi_2nd/21);
            break;
          case 17: // Controller 17
            william.setEnvelope(voice,parser.midi_2nd/32);
            break;
          case 18: // Controlelr 18
            william.setLength(voice,parser.midi_2nd);
            break;
          case 10: // Controller 10
            william.setMod(voice,parser.midi_2nd);
            break;
          }
          break;
      }
    }
  }
}
