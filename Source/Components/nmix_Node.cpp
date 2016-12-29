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

#include "nmix_Node.h"
#include "nmix_Stage.h"
#include "nmix_Operations.h"
#include "nmix_OperationHandler.h"
#include "nmix_Colours.h"

nmix::Node::Node(nmix::Stage& s, nmix::OperationHandler& o) : juce::Component("Unnamed"), stage(s), operationHandler(o)
{
    status = 0;
    
    setColour(backgroundColourId, nmix::Colours::Blue);
    setColour(highlightColourId,  nmix::Colours::White);
    
    setRepaintsOnMouseActivity(true);
}

nmix::Node::~Node()
{

}

void nmix::Node::mouseEnter(const juce::MouseEvent &e)
{
    status |= Hovered;
}

void nmix::Node::mouseExit(const juce::MouseEvent &e)
{
    status &= ~Hovered;
}

void nmix::Node::mouseDown(const juce::MouseEvent &e)
{
    toFront(true);
    
    mouseDownResult = operationHandler.selectedNodes.addToSelectionOnMouseDown(this, e.mods);
    
    for (nmix::Node** n = operationHandler.selectedNodes.begin(); n != operationHandler.selectedNodes.end(); ++n)
    {
        (*n)->currentOpOrigin = (*n)->getPosition();
    }
}

void nmix::Node::mouseDrag(const juce::MouseEvent &e)
{
    
    operationHandler.positionSelection(e);
//    if(e.mouseWasDraggedSinceMouseDown() && stage.currentOperation != nmix::Operation::Escape)
//    {
//        juce::MouseEvent k = e.getEventRelativeTo(&stage);
//        
//        juce::Point<int> center = juce::Point<int>(stage.getWidth()/2, stage.getHeight()/2);
//        
//        for (nmix::Node** n = stage.selectedNodes.begin(); n != stage.selectedNodes.end(); ++n)
//        {
//            if (!((*n)->status & StatusIds::Locked))
//            {
//            
//                switch (stage.currentOperation)
//                {
//                        
//                case nmix::Operation::AdjustVolume:
//                {
//                    juce::Point<int> p = (*n)->currentOpOrigin.translated((*n)->getWidth()/2, (*n)->getHeight()/2).translated(-center.x, -center.y);
//                    
//                    juce::Point<int> pOffset = juce::Point<int>((*n)->currentOpOrigin.x + (*n)->getWidth()/2 + e.getDistanceFromDragStartX() - center.x, (*n)->currentOpOrigin.y + (*n)->getHeight()/2 + e.getDistanceFromDragStartY() - center.y);
//                    
//                    double dot = p.getDotProduct(pOffset);
//                    double len = p.x * p.x + p.y * p.y;
//                    
//                    (*n)->setCentrePosition((int)(center.x + (dot * p.x) / len), (int)(center.y + (dot * p.y) / len));
//                    
//                    break;
//                }
//                    
//                case nmix::Operation::AdjustBalance:
//                {
//                    juce::Point<int> p = (*n)->currentOpOrigin.translated((*n)->getWidth()/2, (*n)->getHeight()/2);
//                    
//                    float opOriginAngle     = center.getAngleToPoint(p);
//                    float mouseOriginAngle  = center.getAngleToPoint(k.getMouseDownPosition());
//                    float mouseCurrentAngle = center.getAngleToPoint(k.getPosition());
//                    
//                    juce::Point<float> final = center.getPointOnCircumference(center.getDistanceFrom(p), opOriginAngle - (mouseOriginAngle - mouseCurrentAngle));
//                    
//                    (*n)->setCentrePosition(final.x, final.y);
//                    
//                    break;
//                }
//                    
//                case nmix::Operation::AdjustX:
//                    
//                    (*n)->setTopLeftPosition((*n)->currentOpOrigin.x + e.getDistanceFromDragStartX(), (*n)->currentOpOrigin.y);
//                    
//                    break;
//                
//                case nmix::Operation::AdjustY:
//                        
//                    (*n)->setTopLeftPosition((*n)->currentOpOrigin.x, (*n)->currentOpOrigin.y + e.getDistanceFromDragStartY());
//                        
//                    break;
//                        
//                default:
//                    
//                    (*n)->setTopLeftPosition((*n)->currentOpOrigin.x + e.getDistanceFromDragStartX(), (*n)->currentOpOrigin.y + e.getDistanceFromDragStartY());
//                    
//                    break;
//                }
//            }
//        }
//    }
//    else
//    {
//        for (nmix::Node** n = stage.selectedNodes.begin(); n != stage.selectedNodes.end(); ++n)
//        {
//            (*n)->setTopLeftPosition((*n)->currentOpOrigin);
//        }
//    }
    
    stage.repaint();
}

void nmix::Node::mouseUp(const juce::MouseEvent &e)
{
    operationHandler.selectedNodes.addToSelectionOnMouseUp(this, e.mods, e.mouseWasDraggedSinceMouseDown(), mouseDownResult);
    operationHandler.currentOperation = nmix::Operation::None;
    stage.repaint();
}

void nmix::Node::paint(juce::Graphics &g)
{
    int w = getWidth();
    int h = getHeight();
    
    juce::Colour body      = findColour(backgroundColourId);
    juce::Colour highlight = findColour(highlightColourId).withAlpha(0.0f);
    
    if (status & Hovered)
    {
        body = body.brighter();
    }
    
    if (status & Selected)
    {
        body      = body.brighter();
        highlight = highlight.withAlpha(1.0f);
    }
    
    if (status & Locked)
    {
        body = body.darker();
    }
    
    g.setColour(body);
    g.fillEllipse(0, 0, w, h);
    
    g.setColour(highlight);
    g.drawEllipse(1, 1, w-2, h-2, 2);
}

void nmix::Node::resized()
{
    
}
