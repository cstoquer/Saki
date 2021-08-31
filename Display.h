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
// Update the 8 pattern LEDs connected thru a 74HC165 shift register

#ifndef Display_h
#define Display_h

#include <stdlib.h>
#include "Arduino.h"
#include "hardwareConfig.h"

class Display {
  private:
    uint processStep_;
    uint currentState_;
    uint nextState_;
    uint writingState_;
    uint processingState_;
    bool clean_;

  public:

    //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
    void setup() {
      pinMode(DISPLAY_LATCH, OUTPUT);
      pinMode(DISPLAY_CLOCK, OUTPUT);
      pinMode(DISPLAY_DATA,  OUTPUT);
      processStep_  = 0;
      currentState_ = 0;
      nextState_    = 0;
      clean_        = false;
    }

    //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
    void writeLED (uint value) {
      if (value == currentState_) {
        clean_ = true;
        return;
      }
      nextState_ = value;
      clean_ = false;
    }

    //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
    void displayValue (int value) {
      if (value < 0 || value > 7) {
        writeLED(0);
      } else {
        writeLED(1 << value);
      }
    }

    //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
    inline void update () {
      if (clean_) return;

      switch (processStep_++) {
        case 0:
          digitalWrite(DISPLAY_LATCH, LOW);
          processingState_ = writingState_ = nextState_;
          break;

        case 1:
        case 4:
        case 7:
        case 10:
        case 13:
        case 16:
        case 19:
        case 22:
          digitalWrite(DISPLAY_DATA, (processingState_ & 128) != 0);
          break;

        case 2:
        case 5:
        case 8:
        case 11:
        case 14:
        case 17:
        case 20:
        case 23:
          digitalWrite(DISPLAY_CLOCK, HIGH);
          processingState_ <<= 1;
          break;

        case 3:
        case 6:
        case 9:
        case 12:
        case 15:
        case 18:
        case 21:
        case 24:
          digitalWrite(DISPLAY_CLOCK, LOW);
          break;
        
        // end
        case 25:
          digitalWrite(DISPLAY_LATCH, HIGH);
          currentState_ = writingState_;
          processStep_ = 0;
          clean_ = nextState_ == currentState_;
          break;
      }
    }
};

#endif // Display_h
