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

#ifndef NMIX_CHANNEL_H
#define NMIX_CHANNEL_H

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Components/nmix_Node.h"

namespace nmix
{

struct Channel : public nmix::Node
{

    Channel(nmix::Stage& s, nmix::OperationHandler& o);
    ~Channel();

    void moved() override;

    float distance;
    float azimuth;

};


}



#endif  // NMIX_CHANNEL_H
