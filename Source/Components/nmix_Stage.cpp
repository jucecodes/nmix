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

#include "nmix_Stage.h"
#include "nmix_Colours.h"
#include "nmix_Operations.h"
#include "nmix_Node.h"
#include "nmix_OperationHandler.h"
#include "nmix_Viewport.h"
#include "nmix_Channel.h"
#include "nmix_Output.h"

nmix::Stage::Stage(nmix::OperationHandler& o) : operationHandler(o)
{
    setColour(backgroundColourId, nmix::Colours::DarkerGrey);
    setColour(foregroundColourId, nmix::Colours::DarkGrey);

    nodeSize = 32;
}

nmix::Stage::~Stage()
{

}

void nmix::Stage::mouseDown(const juce::MouseEvent &e)
{
    if (e.mods.isLeftButtonDown())
    {
        addAndMakeVisible(lasso);
        lasso.beginLasso(e, this);
    }
}

void nmix::Stage::mouseDrag(const juce::MouseEvent &e)
{
    if (e.mouseWasDraggedSinceMouseDown() && e.mods.isLeftButtonDown())
    {
        lasso.dragLasso(e);
        lasso.toFront(true);
    }
}

void nmix::Stage::mouseUp(const juce::MouseEvent &e)
{
    if (e.mods.isLeftButtonDown())
    {
        lasso.endLasso();
        removeChildComponent(&lasso);
        if (!e.mouseWasDraggedSinceMouseDown())
        {
            operationHandler.deselectAll();
        }
    }

    if(e.mods.isRightButtonDown())
    {
        operationHandler.currentViewport->invokeContextualMenu(e);
    }
}

void nmix::Stage::findLassoItemsInArea(juce::Array<nmix::Node *>& results, const juce::Rectangle<int> &area)
{
    for (nmix::Node** n = operationHandler.stagedNodes.begin(); n != operationHandler.stagedNodes.end(); ++n)
    {
        if ((*n)->getBounds().intersects(area))
        {
            results.add(*n);
        }
    }
}

juce::SelectedItemSet<nmix::Node*>& nmix::Stage::getLassoSelection()
{
    return operationHandler.selectedNodes;
}

void nmix::Stage::paint(juce::Graphics& g)
{
    int w = getWidth();
    int h = getHeight();
    
    g.fillAll(findColour(backgroundColourId));
    
    g.setColour(findColour(foregroundColourId));
    g.drawVerticalLine  (w/2, 0, h);
    g.drawHorizontalLine(h/2, 0, w);
    
    for (int i = 0; i <= 3; i++)
    {
        g.drawEllipse((w/8)*i, (h/8)*i, w - (w/4 * i), h - (h/4 * i), 0.5f);
    }
    
    if (operationHandler.selectedNodes.getNumSelected() > 0)
    {
        for (nmix::Node** n = operationHandler.selectedNodes.begin(); n != operationHandler.selectedNodes.end(); ++n)
        {
            if ((*n) != master)
            {
                int nWidth  = (*n)->getWidth();
                int nHeight = (*n)->getHeight();

                juce::Point<int> masterCenter = juce::Point<int>(master->getX() + master->getWidth()/2, master->getY() + master->getHeight()/2);

                int radius;
                if (operationHandler.currentOperation == nmix::Operation::PositionSelectionAzimuth)
                {
                    radius = (*n)->currentOpOrigin.translated(nWidth/2, nHeight/2).getDistanceFrom(masterCenter);
                }
                else
                {
                    radius = (*n)->getPosition().translated(nWidth/2, nHeight/2).getDistanceFrom(masterCenter);
                }

                g.setColour((*n)->findColour(nmix::Node::backgroundColourId));
                g.drawEllipse(masterCenter.x - radius, masterCenter.y - radius, radius*2, radius*2, 1);
                g.drawLine(masterCenter.x, masterCenter.y, (*n)->getX() + nWidth/2, (*n)->getY() + nHeight/2, 1);
            }
        }
    }
}

void nmix::Stage::resized()
{
    
}