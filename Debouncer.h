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
// Digital signal debouncer

#ifndef Debouncer_h
#define Debouncer_h

#include <stdlib.h>
#include "Arduino.h"


class Debouncer {
  private:
    int  pin_;
    int  state_;
    int  debounceCounter_;
    bool isDebouncing_;
    uint debounceLength_;

  public:

    bool risingEdge;
    bool fallingEdge;
    bool value;

    //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
    void setup(int pin, uint debounceLength) {
      pinMode(pin, INPUT);
      this->pin_            = pin;
      this->debounceLength_ = debounceLength;
      state_                = 0;
      debounceCounter_      = 0;
      isDebouncing_         = false;
      risingEdge            = false;
      fallingEdge           = false;
      value                 = false;
    }

    //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
    inline void update () {
      risingEdge  = false;
      fallingEdge = false;

      int pinValue = digitalRead(pin_);

      if (state_ != pinValue) {
        state_ = pinValue;
        isDebouncing_ = true;
        debounceCounter_ = 0;
      } else if (isDebouncing_) {
        if (++debounceCounter_ >= debounceLength_) {
          isDebouncing_ = false;
          debounceCounter_ = 0;
          value = pinValue > 0;
          if (value) {
            risingEdge  = true;
          } else {
            fallingEdge = true;
          }
        }
      }
    }
};

#endif // Debouncer_h
