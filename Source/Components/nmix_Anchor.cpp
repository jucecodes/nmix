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

#include "nmix_Anchor.h"
#include "nmix_Node.h"
#include "nmix_Stage.h"
#include "nmix_Colours.h"
#include "nmix_Viewport.h"
#include "nmix_OperationHandler.h"
#include "nmix_Output.h"
#include "nmix_Channel.h"

nmix::Anchor::Anchor(nmix::Stage& s) : currentStage(s)
{
    setSize(8, 8);
}

nmix::Anchor::~Anchor()
{
    
}

void nmix::Anchor::mouseDown(const juce::MouseEvent& e)
{
    if (e.mods.isLeftButtonDown())
    {
        dragger.startDraggingComponent(this, e);
    }
    else if (e.mods.isRightButtonDown())
    {
        currentStage.operationHandler.currentViewport->invokeContextualMenu(e);
    }
}

void nmix::Anchor::mouseDrag(const juce::MouseEvent& e)
{
    if (e.mouseWasDraggedSinceMouseDown() && currentSnap != nullptr)
    {
        currentSnap = nullptr;
    }

    dragger.dragComponent(this, e, nullptr);
    currentStage.repaint();
}

void nmix::Anchor::paint(juce::Graphics& g)
{

    if (currentSnap != nullptr)
    {
        g.setColour(currentSnap->findColour(nmix::Node::backgroundColourId).contrasting());
    }
    else
    {
        g.setColour(nmix::Colours::White);
    }

    g.drawLine(4, 0, 4, 8, 2);
    g.drawLine(0, 4, 8, 4, 2);
}
