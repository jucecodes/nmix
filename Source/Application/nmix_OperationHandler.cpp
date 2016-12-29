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
    currentStage    = nullptr;
    currentViewport = nullptr;
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

void nmix::OperationHandler::addNode()
{
    nmix::Node* n = new nmix::Node(*currentStage, *this);
    n->setBounds(currentStage->getWidth()/2, currentStage->getHeight()/2, 32, 32);
    currentStage->addAndMakeVisible(n);
    stagedNodes.add(n);
}

void nmix::OperationHandler::deleteSelection()
{
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

void nmix::OperationHandler::positionSelection(const juce::MouseEvent &e)
{
    if(e.mouseWasDraggedSinceMouseDown() && currentOperation != nmix::Operation::Escape)
    {
        juce::MouseEvent k = e.getEventRelativeTo(currentStage);
        
        juce::Point<int> center = juce::Point<int>(currentStage->getWidth()/2, currentStage->getHeight()/2);
        
        for (nmix::Node** n = selectedNodes.begin(); n != selectedNodes.end(); ++n)
        {
            if (!((*n)->status & nmix::Node::StatusIds::Locked))
            {
                
                switch (currentOperation)
                {
                        
                    case nmix::Operation::AdjustVolume:
                    {
                        juce::Point<int> p = (*n)->currentOpOrigin.translated((*n)->getWidth()/2, (*n)->getHeight()/2).translated(-center.x, -center.y);
                        
                        juce::Point<int> pOffset = juce::Point<int>((*n)->currentOpOrigin.x + (*n)->getWidth()/2 + e.getDistanceFromDragStartX() - center.x, (*n)->currentOpOrigin.y + (*n)->getHeight()/2 + e.getDistanceFromDragStartY() - center.y);
                        
                        double dot = p.getDotProduct(pOffset);
                        double len = p.x * p.x + p.y * p.y;
                        
                        (*n)->setCentrePosition((int)(center.x + (dot * p.x) / len), (int)(center.y + (dot * p.y) / len));
                        
                        break;
                    }
                        
                    case nmix::Operation::AdjustBalance:
                    {
                        juce::Point<int> p = (*n)->currentOpOrigin.translated((*n)->getWidth()/2, (*n)->getHeight()/2);
                        
                        float opOriginAngle     = center.getAngleToPoint(p);
                        float mouseOriginAngle  = center.getAngleToPoint(k.getMouseDownPosition());
                        float mouseCurrentAngle = center.getAngleToPoint(k.getPosition());
                        
                        juce::Point<float> final = center.getPointOnCircumference(center.getDistanceFrom(p), opOriginAngle - (mouseOriginAngle - mouseCurrentAngle));
                        
                        (*n)->setCentrePosition(final.x, final.y);
                        
                        break;
                    }
                        
                    case nmix::Operation::AdjustX:
                        
                        (*n)->setTopLeftPosition((*n)->currentOpOrigin.x + e.getDistanceFromDragStartX(), (*n)->currentOpOrigin.y);
                        
                        break;
                        
                    case nmix::Operation::AdjustY:
                        
                        (*n)->setTopLeftPosition((*n)->currentOpOrigin.x, (*n)->currentOpOrigin.y + e.getDistanceFromDragStartY());
                        
                        break;
                        
                    default:
                        
                        (*n)->setTopLeftPosition((*n)->currentOpOrigin.x + e.getDistanceFromDragStartX(), (*n)->currentOpOrigin.y + e.getDistanceFromDragStartY());
                        
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