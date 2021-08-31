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
// User interface (all buttons and pattern LEDs)


#ifndef Interface_h
#define Interface_h


#include <stdlib.h>
#include "Arduino.h"
#include "hardwareConfig.h"
#include "Display.h"
#include "MuxButtons.h"
#include "CommandButtons.h"
#include "Channel.h"

enum Mode {
  PATTERN_EDITION,
  CHANNEL_SELECTION,
  LENGTH_0_SELECTION,
  LENGTH_8_SELECTION,
  DIV_SELECTION
};


class Interface {
  private:
    uint currentChannel_;
    Mode mode_;


  public:

    MuxButtons     muxButtons;
    CommandButtons commandButtons;
    Display        display;
    Channel        channels[8];

    //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
    void setup() {
      channels[0].setup(CHANNEL_0);
      channels[1].setup(CHANNEL_1);
      channels[2].setup(CHANNEL_2);
      channels[3].setup(CHANNEL_3);
      channels[4].setup(CHANNEL_4);
      channels[5].setup(CHANNEL_5);
      channels[6].setup(CHANNEL_6);
      channels[7].setup(CHANNEL_7);

      commandButtons.setup();
      muxButtons.setup();
      display.setup();

      currentChannel_ = 0;
      mode_ = PATTERN_EDITION;
    }

    //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
    void clockChannels () {
      for (int i = 0; i < 8; ++i) {
        channels[i].tickClock();
      }
    }

    //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
    void resetChannels () {
      for (int i = 0; i < 8; ++i) {
        channels[i].resetClock();
      }
    }

    //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
    void clockFallingEdge () {
      // NOTA: only for the 4 first "trigger" channels.
      for (int i = 0; i < 4; ++i) {
        channels[i].clockFallingEdge();
      }
    }

    //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
    inline void update () {
      commandButtons.update();
      muxButtons.update();
      display.update();

      if (commandButtons.len8.risingEdge) {
        mode_ = LENGTH_8_SELECTION;
        display.displayValue(channels[currentChannel_].getLength() - 9);
      }
 
      if (commandButtons.len0.risingEdge) {
        mode_ = LENGTH_0_SELECTION;
        display.displayValue(channels[currentChannel_].getLength() - 1);
      }

      if (commandButtons.divb.risingEdge) {
        mode_ = DIV_SELECTION;
        display.displayValue(channels[currentChannel_].getDiv() - 1);
      }

      if (commandButtons.chan.risingEdge) {
        mode_ = CHANNEL_SELECTION;
        display.displayValue(currentChannel_);
      }

      if (commandButtons.fallingEdge) {
        mode_ = PATTERN_EDITION;
        display.writeLED(channels[currentChannel_].getState());
      }

      if (!muxButtons.pressed) return;

      switch (mode_) {
        case PATTERN_EDITION:
          display.writeLED(channels[currentChannel_].flipStep(muxButtons.buttonId));
          break;

        case CHANNEL_SELECTION:
          currentChannel_ = muxButtons.buttonId;
          display.writeLED(1 << currentChannel_);
          break;

        case LENGTH_0_SELECTION:
          channels[currentChannel_].setLength(muxButtons.buttonId + 1);
          display.writeLED(1 << muxButtons.buttonId);
          break;

        case LENGTH_8_SELECTION:
          channels[currentChannel_].setLength(muxButtons.buttonId + 9);
          display.writeLED(1 << muxButtons.buttonId);
          break;

        case DIV_SELECTION:
          channels[currentChannel_].setDiv(muxButtons.buttonId + 1);
          display.writeLED(1 << muxButtons.buttonId);
          break;
      }
    }
};

#endif // Interface_h
