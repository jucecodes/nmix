/*
 *  Copyright 2016 Antonio Lassandro 
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
 
#include "nmix_Viewport.h"
#include "nmix_Colours.h"

nmix::Viewport::Viewport()
{
    setInterceptsMouseClicks(false, true);
    
    selectionInfo.setText("", juce::dontSendNotification);
    selectionInfo.setColour(juce::Label::ColourIds::textColourId, nmix::Colours::White);
    selectionInfo.setInterceptsMouseClicks(false, false);
    selectionInfo.setJustificationType(juce::Justification::centredLeft);
    addAndMakeVisible(selectionInfo);
}

nmix::Viewport::~Viewport()
{
    
}

void nmix::Viewport::resized()
{
    selectionInfo.setBounds(0, getHeight() - 20, getWidth(), 20);
}