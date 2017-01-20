/*
 *  Copyright 2016 - 2017 Antonio Lassandro 
 *
 *  nmix is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  nmix is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "nmix_Output.h"
#include "nmix_Stage.h"
#include "nmix_OperationHandler.h"
#include "nmix_Colours.h"

nmix::Output::Output(nmix::Stage& s, nmix::OperationHandler& o) : nmix::Node(s, o)
{
    setName("Output");
    status ^= Locked;
    setColour(backgroundColourId, nmix::Colours::Yellow);
}

nmix::Output::~Output()
{

}

void nmix::Output::moved()
{
    if (stage.anchor->currentSnap == this)
    {
        stage.anchor->setCentrePosition(getX() + getWidth()/2, getY() + getHeight()/2);
    }
}
