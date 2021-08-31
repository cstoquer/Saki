// Copyright 2021 Cedric Stoquer.
//
// Author: Cedric Stoquer (cstoquer@gmail.com)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// -----------------------------------------------------------------------------
//
// read and debounce the 8 pattern buttons connected thru a 74HC595 multiplexer


#ifndef MuxButtons_h
#define MuxButtons_h

#include <stdlib.h>
#include "Arduino.h"
#include "hardwareConfig.h"


#define DEBOUNCE_LENGTH 6

class MuxButtons {
  private:
    int   pinStates_[8];       // state values of all 8 button pins
    int   debounceCounter_[8]; // debounce counter values of all 8 pins
    bool  isDebouncing_[8];    // pins currently in debounce mode
    uint  processStep_;

    //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
    inline void debounceValue_ (int i, int value) {
      if (pinStates_[i] != value) {
        pinStates_[i] = value;
        isDebouncing_[i] = !isDebouncing_[i];
        debounceCounter_[i] = 0;
      } else if (isDebouncing_[i]) {
        if (debounceCounter_[i]++ == DEBOUNCE_LENGTH) {
          isDebouncing_[i] = false;
          if (value) {
            pressed  = true;
            buttonId = i;
          }
        }
      }
    }

  public:
    bool pressed  = false; // raising edge
    uint buttonId = 0;     // last button pressed

    //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
    void setup() {
      pinMode(BUTTONS_MUX_LOAD,  OUTPUT);
      pinMode(BUTTONS_MUX_CLOCK, OUTPUT);
      pinMode(BUTTONS_MUX_DATA,  INPUT);
      processStep_ = 0;
    }

    //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
    inline void update () {
      pressed = false;
      switch (processStep_++) {
        case 0:  digitalWrite(BUTTONS_MUX_LOAD, LOW); break;
        case 1:  digitalWrite(BUTTONS_MUX_LOAD, HIGH); break;
    
        case 2:  debounceValue_(7, digitalRead(BUTTONS_MUX_DATA)); break;
        case 5:  debounceValue_(6, digitalRead(BUTTONS_MUX_DATA)); break;
        case 8:  debounceValue_(5, digitalRead(BUTTONS_MUX_DATA)); break;
        case 11: debounceValue_(4, digitalRead(BUTTONS_MUX_DATA)); break;
        case 14: debounceValue_(3, digitalRead(BUTTONS_MUX_DATA)); break;
        case 17: debounceValue_(2, digitalRead(BUTTONS_MUX_DATA)); break;
        case 20: debounceValue_(1, digitalRead(BUTTONS_MUX_DATA)); break;
        case 23: debounceValue_(0, digitalRead(BUTTONS_MUX_DATA)); break;

        case 3:
        case 6:
        case 9:
        case 12:
        case 15:
        case 18:
        case 21:
        case 24: digitalWrite(BUTTONS_MUX_CLOCK, HIGH); break;

        case 4:
        case 7:
        case 10:
        case 13:
        case 16:
        case 19:
        case 22: digitalWrite(BUTTONS_MUX_CLOCK, LOW); break;
        case 25: digitalWrite(BUTTONS_MUX_CLOCK, LOW); processStep_ = 0; break;
      }
    }
};

#endif // MuxButtons_h
