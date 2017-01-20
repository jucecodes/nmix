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

#include "nmix_OperationHandler.h"
#include "nmix_Colours.h"
#include "nmix_Viewport.h"
#include "nmix_Stage.h"
#include "nmix_Node.h"
#include "nmix_Channel.h"
#include "nmix_Output.h"

nmix::OperationHandler::OperationHandler()
{
    currentOperation = nmix::Operation::None;
    currentEditMode  = nmix::EditMode::Parallel;
    currentModifiers = juce::ModifierKeys::noModifiers;
    selectedNodes.addChangeListener(this);
}

nmix::OperationHandler::~OperationHandler()
{
    stagedNodes.clear();
}

void nmix::OperationHandler::cycleEditMode()
{
    if (currentEditMode == nmix::EditMode::Parallel)
    {
        currentEditMode = nmix::EditMode::Inverse;
        currentViewport->operationInfo.setText("Inverse", juce::dontSendNotification);
    }
    else
    {
        currentEditMode = nmix::EditMode::Parallel;
        currentViewport->operationInfo.setText("Parallel", juce::dontSendNotification);
    }
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
    nmix::Node* n = new nmix::Channel(*currentStage, *this);

    juce::Point<int> placement;

    if (fromKeyPress)
    {
        placement = juce::Point<int>(currentStage->getWidth()/2, currentStage->getHeight()/2);
    } else {
        placement = mouseOpOrigin;
    }

    n->setCentrePosition(placement.x, placement.y);

    currentStage->addAndMakeVisible(n);
    stagedNodes.add(n);
}

void nmix::OperationHandler::deleteSelection()
{
    if (!selectedNodes.isSelected(currentOpSource))
    {
        if (currentOpSource != currentStage->master && !(currentOpSource->status & nmix::Node::Locked))
        {
            selectedNodes.deselect(currentOpSource);
            stagedNodes.removeObject(currentOpSource);
            currentOpSource = nullptr;
        }
        else
        {
            // warning
            return;
        }
    }
    else
    {
        selectedNodes.deselect(currentStage->master);

        juce::Array<nmix::Node*> nodesToDelete;

        for (int i = 0; i < selectedNodes.getNumSelected(); ++i)
        {
            if (!(selectedNodes.getSelectedItem(i)->status & nmix::Node::Locked))
            {
                nodesToDelete.add(selectedNodes.getSelectedItem(i));
            }
        }

        for (nmix::Node** n = nodesToDelete.begin(); n != nodesToDelete.end(); ++n)
        {
            selectedNodes.deselect((*n));
            stagedNodes.removeObject((*n));
        }
    }

    currentStage->repaint();
}

void nmix::OperationHandler::lockSelection()
{

    if (!selectedNodes.isSelected(currentOpSource))
    {
        currentOpSource->status |= nmix::Node::Locked;
        selectedNodes.deselect(currentOpSource);
        currentOpSource->repaint();
    }
    else
    {
        for (nmix::Node** n = selectedNodes.begin(); n != selectedNodes.end(); ++n)
        {
            (*n)->status |= nmix::Node::Locked;
        }

        selectedNodes.deselectAll();
    }
}

void nmix::OperationHandler::unlockSelection()
{
    if (!selectedNodes.isSelected(currentOpSource))
    {
        currentOpSource->status &= ~nmix::Node::Locked;
        currentOpSource->repaint();
    }
    else
    {
        for (nmix::Node** n = selectedNodes.begin(); n != selectedNodes.end(); ++n)
        {
            (*n)->status &= ~nmix::Node::Locked;
            (*n)->repaint();
        }
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

    if (!selectedNodes.isSelected(currentOpSource))
    {
        currentOpSource->setCentrePosition(centre.x, centre.y);
    }
    else
    {
        for (nmix::Node** n = selectedNodes.begin(); n != selectedNodes.end(); ++n)
        {
            if (!((*n)->status & nmix::Node::StatusIds::Locked))
            {
                (*n)->setCentrePosition(centre.x, centre.y);
            }
        }
    }

    currentStage->repaint();
}

void nmix::OperationHandler::positionSelection(const juce::MouseEvent &e)
{
    if (currentOperation != nmix::Operation::Escape)
    {
        juce::MouseEvent k = e.getEventRelativeTo(currentStage);

        float precision  = (currentModifiers.isShiftDown()) ? 0.25 : 1;
        int   proportion = 1;

        for (nmix::Node** n = selectedNodes.begin(); n != selectedNodes.end(); ++n)
        {

            if ((*n) != currentOpSource)
            {
                if (currentEditMode == nmix::EditMode::Inverse)
                {
                    proportion = -1;
                }

                if (currentModifiers.isCommandDown())
                {
                    continue;
                }
            }

            if (!((*n)->status & nmix::Node::StatusIds::Locked))
            {

                juce::Point<int> centre = currentStage->anchor->getPosition().translated(4, 4);
                
                switch (currentOperation)
                {
                        
                    case nmix::Operation::PositionSelectionDistance:
                    {

                        if (currentStage->anchor->currentSnap == (*n)) { continue; }

                        juce::Point<int> p = (*n)->currentModOrigin.translated((*n)->getWidth()/2, (*n)->getHeight()/2).translated(-centre.x, -centre.y);
                        
                        juce::Point<int> pOffset = juce::Point<int>((*n)->currentModOrigin.x + (*n)->getWidth()/2 + ((k.x - mouseModOrigin.x) * precision * proportion) - centre.x, (*n)->currentModOrigin.y + (*n)->getHeight()/2 + ((k.y - mouseModOrigin.y) * precision * proportion) - centre.y);
                        
                        double dot = p.getDotProduct(pOffset);
                        double len = p.x * p.x + p.y * p.y;
                        
                        (*n)->setCentrePosition((int)(centre.x + (dot * p.x) / len), (int)(centre.y + (dot * p.y) / len));

                        break;
                    }
                        
                    case nmix::Operation::PositionSelectionAzimuth:
                    {

                        if (currentStage->anchor->currentSnap == (*n)) { continue; }

                        juce::Point<int> p = (*n)->currentModOrigin.translated((*n)->getWidth()/2, (*n)->getHeight()/2);
                        
                        float opOriginAngle     = centre.getAngleToPoint(p);
                        float mouseOriginAngle  = centre.getAngleToPoint(mouseModOrigin);
                        float mouseCurrentAngle = centre.getAngleToPoint(k.getPosition());
                        
                        juce::Point<float> final = centre.getPointOnCircumference(centre.getDistanceFrom(p), opOriginAngle - ((mouseOriginAngle - mouseCurrentAngle) * precision * proportion));
                        
                        (*n)->setCentrePosition(final.x, final.y);

                        break;
                    }
                        
                    case nmix::Operation::PositionSelectionX:
                        
                        (*n)->setTopLeftPosition((*n)->currentModOrigin.x + (k.x - mouseModOrigin.x) * precision * proportion, (*n)->currentOpOrigin.y);
                        
                        break;
                        
                    case nmix::Operation::PositionSelectionY:
                        
                        (*n)->setTopLeftPosition((*n)->currentModOrigin.x, (*n)->currentModOrigin.y + (k.y - mouseModOrigin.y) * precision * proportion);
                        
                        break;
                        
                    default:
                        
                        (*n)->setTopLeftPosition((*n)->currentModOrigin.x + (k.x - mouseModOrigin.x) * precision * proportion, (*n)->currentModOrigin.y + (k.y - mouseModOrigin.y) * precision * proportion);
                        
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

void nmix::OperationHandler::setAnchor()
{
    currentStage->anchor->setCentrePosition(mouseOpOrigin.x, mouseOpOrigin.y);
    currentStage->repaint();
}

void nmix::OperationHandler::snapAnchor()
{
    nmix::Node* n = currentOpSource;
    currentStage->anchor->currentSnap = n;
    currentStage->anchor->setCentrePosition(n->getX() + currentStage->nodeSize/2, n->getY() + currentStage->nodeSize/2);
    currentStage->repaint();
}

void nmix::OperationHandler::resetAnchor()
{
    juce::Point<int> centre = currentStage->master->getPosition().translated(currentStage->nodeSize/2, currentStage->nodeSize/2);
    currentStage->anchor->setCentrePosition(centre.x, centre.y);
    currentStage->anchor->currentSnap = currentStage->master;
    currentStage->repaint();
}

void nmix::OperationHandler::centreAnchor()
{
    juce::Point<int> centre = juce::Point<int>(currentStage->getWidth()/2, currentStage->getHeight()/2);
    currentStage->anchor->setCentrePosition(centre.x, centre.y);
    currentStage->anchor->currentSnap = nullptr;
    currentStage->repaint();
}

void nmix::OperationHandler::modifierKeysChanged(const juce::ModifierKeys &mods)
{
    if (juce::Desktop::getInstance().getNumDraggingMouseSources())
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
