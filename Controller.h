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
// Read and manage clock and reset input signals.


#ifndef Controller_h
#define Controller_h


#include <stdlib.h>
#include "Arduino.h"
#include "hardwareConfig.h"
#include "Interface.h"
#include "Debouncer.h"


class Controller {
  private:

    bool      resetRequested_;
    bool      ticking_;
    uint32_t  tick_;
    Debouncer clockDebounce;
    Debouncer resetDebounce;

  public:
    Interface interface;

    //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
    void setup () {
      clockDebounce.setup(CLOCK_INPUT, 8);
      resetDebounce.setup(RESET_INPUT, 8);

      resetRequested_ = false;
      ticking_        = false;
      tick_           = 0;

      interface.setup();
    }

    //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
    inline void update () {
      clockDebounce.update();
      resetDebounce.update();

      if (resetDebounce.risingEdge) resetRequested_ = true; // will be applied on next clock

      if (clockDebounce.risingEdge && !ticking_) {
        ticking_ = true;
        tick_    = 0;
      }

      if (ticking_) {
        ++tick_;

        // delay the clock for few cycles to wait for reset signal
        if (tick_ == 10) {
          if (resetRequested_) {
            globalStep = 0;
            interface.resetChannels();
            resetRequested_ = false;
          } else {
            if (++globalStep == 128) globalStep = 0;
            interface.clockChannels();
          }
        }

        // end of trigger pulse
        if (tick_ == 200) interface.clockFallingEdge();

        // inhibate the clock signal for few more cycles (debouncing & limit bpm)
        if (tick_ == 250) ticking_ = false;
      }

      interface.update();
    }
};

#endif // Controller_h
