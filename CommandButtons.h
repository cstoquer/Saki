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
// Read and debounce the four command buttons (len8, len, div chan)


#ifndef CommandButtons_h
#define CommandButtons_h


#include <stdlib.h>
#include "Arduino.h"
#include "hardwareConfig.h"
#include "Debouncer.h"


class CommandButtons {
  private:
    uint processStep_;
    

  public:
    Debouncer chan;
    Debouncer len8;
    Debouncer len0;
    Debouncer divb;
    bool      fallingEdge;
    

    //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
    void setup () {
      processStep_ = 0;
      fallingEdge = false;
      chan.setup(CHAN_BUTTON,  60);
      len8.setup(LEN_8_BUTTON, 60);
      len0.setup(LEN_0_BUTTON, 60);
      divb.setup(DIV_BUTTON,    60);
    }

    //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
    inline void update () {
      switch (processStep_++) {
        case 0: chan.update(); fallingEdge = chan.fallingEdge; break;
        case 1: len8.update(); fallingEdge = len8.fallingEdge; break;
        case 2: len0.update(); fallingEdge = len0.fallingEdge; break;
        case 3: divb.update(); fallingEdge = divb.fallingEdge; processStep_ = 0; break;
      }
    }
};

#endif // CommandButtons_h
