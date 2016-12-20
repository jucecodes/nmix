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
#include "nmix_Colours.h"

nmix::Node::Node(nmix::Stage& s) : juce::Component("Unnamed"), stage(s)
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
    
    mouseDownResult = stage.selectedNodes.addToSelectionOnMouseDown(this, e.mods);
    
    for (nmix::Node** n = stage.selectedNodes.begin(); n != stage.selectedNodes.end(); ++n)
    {
        (*n)->currentOpOrigin = (*n)->getPosition();
    }
}

void nmix::Node::mouseDrag(const juce::MouseEvent &e)
{
    if(e.mouseWasDraggedSinceMouseDown())
    {
        
        juce::Point<int> center = juce::Point<int>(stage.getWidth()/2, stage.getHeight()/2);
        
        for (nmix::Node** n = stage.selectedNodes.begin(); n != stage.selectedNodes.end(); ++n)
        {
            switch (stage.status)
            {
                    
            case nmix::Stage::OperationStates::AdjustVolume:
            {
                juce::Point<int> p = (*n)->currentOpOrigin.translated((*n)->getWidth()/2, (*n)->getHeight()/2).translated(-center.x, -center.y);
                
                juce::Point<int> pOffset = juce::Point<int>((*n)->currentOpOrigin.x + e.getDistanceFromDragStartX() - center.x, (*n)->currentOpOrigin.y + e.getDistanceFromDragStartY() - center.y);
                
                double dot = p.getDotProduct(pOffset);
                double len = p.x * p.x + p.y * p.y;
                
                (*n)->setCentrePosition((int)(center.x + (dot * p.x) / len), (int)(center.y + (dot * p.y) / len));
                
                break;
            }
                
            case nmix::Stage::OperationStates::AdjustBalance:
            {
                juce::MouseEvent k = e.getEventRelativeTo(&stage);
                
                juce::Point<int> p = (*n)->currentOpOrigin.translated((*n)->getWidth()/2, (*n)->getHeight()/2);
                
                float opOriginAngle     = center.getAngleToPoint((*n)->currentOpOrigin);
                float mouseOriginAngle  = center.getAngleToPoint(k.getMouseDownPosition());
                float mouseCurrentAngle = center.getAngleToPoint(k.getPosition());
                
                opOriginAngle     += (opOriginAngle < 0)     ? 2 * M_PI : -2 * M_PI;
                mouseOriginAngle  += (mouseOriginAngle < 0)  ? 2 * M_PI : -2 * M_PI;
                mouseCurrentAngle += (mouseCurrentAngle < 0) ? 2 * M_PI : -2 * M_PI;
                
                juce::Point<float> final = center.getPointOnCircumference(center.getDistanceFrom(p), opOriginAngle - (mouseOriginAngle - mouseCurrentAngle));
                
                (*n)->setCentrePosition(final.x, final.y);
                
                break;
            }
                
            default:
                
                (*n)->setTopLeftPosition((*n)->currentOpOrigin.x + e.getDistanceFromDragStartX(), (*n)->currentOpOrigin.y + e.getDistanceFromDragStartY());
                
                break;
            }
            
        }
        
        stage.repaint();
    }
}

void nmix::Node::mouseUp(const juce::MouseEvent &e)
{
    stage.selectedNodes.addToSelectionOnMouseUp(this, e.mods, e.mouseWasDraggedSinceMouseDown(), mouseDownResult);
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
