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

#include "nmix_OperationHandler.h"
#include "nmix_Colours.h"
#include "nmix_Viewport.h"
#include "nmix_Stage.h"
#include "nmix_Node.h"

nmix::OperationHandler::OperationHandler()
{
    currentOperation = nmix::Operation::None;
    selectedNodes.addChangeListener(this);
}

nmix::OperationHandler::~OperationHandler()
{
    stagedNodes.clear();
}

void nmix::OperationHandler::selectAll()
{
    for (nmix::Node** n = stagedNodes.begin(); n != stagedNodes.end(); ++n)
    {
        selectedNodes.addToSelection(*n);
    }
}

void nmix::OperationHandler::invertSelection()
{
    for (nmix::Node** n = stagedNodes.begin(); n != stagedNodes.end(); ++n)
    {
        (selectedNodes.isSelected(*n)) ? selectedNodes.deselect(*n) : selectedNodes.addToSelection(*n);
    }
}

void nmix::OperationHandler::deselectAll()
{
    selectedNodes.deselectAll();
}

void nmix::OperationHandler::addNode(bool fromKeyPress)
{
    nmix::Node* n = new nmix::Node(*currentStage, *this);

    juce::Point<int> placement;

    if (fromKeyPress)
    {
        placement = juce::Point<int>(currentStage->getWidth()/2, currentStage->getHeight()/2);
    } else {
        placement = currentStage->getLocalPoint(nullptr, juce::Desktop::getLastMouseDownPosition());
    }

    n->setCentrePosition(placement.x, placement.y);

    stagedNodes.add(n);
}

void nmix::OperationHandler::deleteSelection()
{

    selectedNodes.deselect(currentStage->master);

    while (selectedNodes.getNumSelected() > 0)
    {
        Node* n = selectedNodes.getSelectedItem(0);
        selectedNodes.deselect(n);
        stagedNodes.removeObject(n);
    }

    currentStage->repaint();
}

void nmix::OperationHandler::lockSelection()
{
    for (nmix::Node** n = selectedNodes.begin(); n != selectedNodes.end(); ++n)
    {
        (*n)->status ^= nmix::Node::Locked;
        (*n)->repaint();
    }
}

void nmix::OperationHandler::nudgeSelection(const juce::KeyPress& k)
{
    currentOperation = nmix::Operation::NudgeSelection;
    int nudgeValue = (k.getModifiers().isShiftDown()) ? 10 : 1;
    
    int deltaX =
      (k.getKeyCode() == juce::KeyPress::upKey)    ? 0
    : (k.getKeyCode() == juce::KeyPress::downKey)  ? 0
    : (k.getKeyCode() == juce::KeyPress::leftKey)  ? -nudgeValue
    : (k.getKeyCode() == juce::KeyPress::rightKey) ? nudgeValue
    : 0;
    
    int deltaY =
      (k.getKeyCode() == juce::KeyPress::upKey)    ? -nudgeValue
    : (k.getKeyCode() == juce::KeyPress::downKey)  ? nudgeValue
    : (k.getKeyCode() == juce::KeyPress::leftKey)  ? 0
    : (k.getKeyCode() == juce::KeyPress::rightKey) ? 0
    : 0;
    
    for (nmix::Node** n = selectedNodes.begin(); n != selectedNodes.end(); ++n)
    {
        (*n)->setTopLeftPosition((*n)->getPosition().translated(deltaX, deltaY));
    }
    
    currentStage->repaint();
}

void nmix::OperationHandler::centreSelection()
{
    juce::Point<int> centre = juce::Point<int>(currentStage->getWidth()/2, currentStage->getHeight()/2);

    for (nmix::Node** n = selectedNodes.begin(); n != selectedNodes.end(); ++n)
    {
        (*n)->setCentrePosition(centre.x, centre.y);
    }
}

void nmix::OperationHandler::positionSelection(const juce::MouseEvent &e)
{
    if(e.mouseWasDraggedSinceMouseDown() && currentOperation != nmix::Operation::Escape)
    {
        juce::MouseEvent k = e.getEventRelativeTo(currentStage);
        
        juce::Point<int> center = juce::Point<int>(currentStage->master->getPosition().translated(currentStage->nodeSize/2, currentStage->nodeSize/2));

        float precision = (currentModifiers.isShiftDown()) ? 0.25 : 1;

        for (nmix::Node** n = selectedNodes.begin(); n != selectedNodes.end(); ++n)
        {
            if (!((*n)->status & nmix::Node::StatusIds::Locked))
            {
                
                switch (currentOperation)
                {
                        
                    case nmix::Operation::PositionSelectionDistance:
                    {
                        if ((*n) != currentStage->master)
                        {
                            juce::Point<int> p = (*n)->currentModOrigin.translated((*n)->getWidth()/2, (*n)->getHeight()/2).translated(-center.x, -center.y);
                            
                            juce::Point<int> pOffset = juce::Point<int>((*n)->currentModOrigin.x + (*n)->getWidth()/2 + ((k.x - mouseModOrigin.x) * precision) - center.x, (*n)->currentModOrigin.y + (*n)->getHeight()/2 + ((k.y - mouseModOrigin.y) * precision) - center.y);
                            
                            double dot = p.getDotProduct(pOffset);
                            double len = p.x * p.x + p.y * p.y;
                            
                            (*n)->setCentrePosition((int)(center.x + (dot * p.x) / len), (int)(center.y + (dot * p.y) / len));
                        }
                        break;
                    }
                        
                    case nmix::Operation::PositionSelectionAzimuth:
                    {
                        if ((*n) != currentStage->master)
                        {
                            juce::Point<int> p = (*n)->currentModOrigin.translated((*n)->getWidth()/2, (*n)->getHeight()/2);
                            
                            float opOriginAngle     = center.getAngleToPoint(p);
                            float mouseOriginAngle  = center.getAngleToPoint(mouseModOrigin);
                            float mouseCurrentAngle = center.getAngleToPoint(k.getPosition());
                            
                            juce::Point<float> final = center.getPointOnCircumference(center.getDistanceFrom(p), opOriginAngle - ((mouseOriginAngle - mouseCurrentAngle) * precision));
                            
                            (*n)->setCentrePosition(final.x, final.y);
                        }
                        break;
                    }
                        
                    case nmix::Operation::PositionSelectionX:
                        
                        (*n)->setTopLeftPosition((*n)->currentModOrigin.x + (k.x - mouseModOrigin.x) * precision, (*n)->currentOpOrigin.y);
                        
                        break;
                        
                    case nmix::Operation::PositionSelectionY:
                        
                        (*n)->setTopLeftPosition((*n)->currentModOrigin.x, (*n)->currentModOrigin.y + (k.y - mouseModOrigin.y) * precision);
                        
                        break;
                        
                    default:
                        
                        (*n)->setTopLeftPosition((*n)->currentModOrigin.x + (k.x - mouseModOrigin.x) * precision, (*n)->currentModOrigin.y + (k.y - mouseModOrigin.y) * precision);
                        
                        break;
                }
            }
        }
    }
    else
    {
        for (nmix::Node** n = selectedNodes.begin(); n != selectedNodes.end(); ++n)
        {
            (*n)->setTopLeftPosition((*n)->currentOpOrigin);
        }
    }
    
    currentStage->repaint();
}

void nmix::OperationHandler::modifierKeysChanged(const juce::ModifierKeys &mods)
{
    if (mods.isShiftDown() && juce::Desktop::getInstance().getNumDraggingMouseSources())
    {
        for (nmix::Node** n = selectedNodes.begin(); n != selectedNodes.end(); ++n)
        {
            (*n)->currentModOrigin = (*n)->getPosition();
        }

        mouseModOrigin = currentStage->getLocalPoint(nullptr, juce::Desktop::getMousePosition());
    }

    currentModifiers = mods;
}

void nmix::OperationHandler::changeListenerCallback(juce::ChangeBroadcaster *source)
{
    for (nmix::Node** n = stagedNodes.begin(); n != stagedNodes.end(); ++n)
    {
        if (selectedNodes.isSelected(*n))
        {
            (*n)->status |= nmix::Node::Selected;
        }
        else
        {
            (*n)->status &= ~nmix::Node::Selected;
        }
        
        (*n)->repaint();
        currentStage->repaint();
    }
    
    if (selectedNodes.getNumSelected() > 1)
    {
        currentViewport->selectionInfo.setText(juce::String(selectedNodes.getNumSelected()) + " nodes selected", juce::dontSendNotification);
        currentViewport->selectionInfo.setColour(juce::Label::ColourIds::textColourId, nmix::Colours::White);
    }
    else if (selectedNodes.getNumSelected() == 1)
    {
        nmix::Node* n = selectedNodes.getSelectedItem(0);
        currentViewport->selectionInfo.setText(n->getName(), juce::dontSendNotification);
        currentViewport->selectionInfo.setColour(juce::Label::ColourIds::textColourId, n->findColour(nmix::Node::backgroundColourId));
    }
    else
    {
        currentViewport->selectionInfo.setText("", juce::dontSendNotification);
        currentViewport->selectionInfo.setColour(juce::Label::ColourIds::textColourId, nmix::Colours::White);
    }
}