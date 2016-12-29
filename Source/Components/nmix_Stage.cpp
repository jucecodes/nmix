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
#include "nmix_Application.h"
#include "nmix_Colours.h"
#include "nmix_Operations.h"
#include "nmix_Node.h"
#include "nmix_Viewport.h"
#include "nmix_OperationHandler.h"

nmix::Stage::Stage(nmix::Viewport& v, nmix::OperationHandler& o) : viewport(v), operationHandler(o)
{
    setColour(backgroundColourId, nmix::Colours::DarkerGrey);
    setColour(foregroundColourId, nmix::Colours::DarkGrey);
    
    setWantsKeyboardFocus(true);
    
    juce::ApplicationCommandManager& commandManager = nmix::Application::getCommandManager();
    commandManager.registerAllCommandsForTarget(this);
    addKeyListener(commandManager.getKeyMappings());
}

nmix::Stage::~Stage()
{

}

juce::ApplicationCommandTarget* nmix::Stage::getNextCommandTarget()
{
    return nullptr;
}

void nmix::Stage::getAllCommands(juce::Array<juce::CommandID> &commands)
{
    const juce::CommandID ids[] =
    {
        nmix::Operation::Escape,
        
        nmix::Operation::SelectAll,
        nmix::Operation::InvertSelection,
        nmix::Operation::DeselectAll,
        
        nmix::Operation::AddNode,
        nmix::Operation::RemoveNode,
        
        nmix::Operation::NudgeSelection,
        nmix::Operation::LockSelection,
        
        nmix::Operation::AdjustX,
        nmix::Operation::AdjustY,
        nmix::Operation::AdjustVolume,
        nmix::Operation::AdjustBalance
    };
    
    commands.addArray(ids, juce::numElementsInArray(ids));
}

void nmix::Stage::getCommandInfo(juce::CommandID commandID, juce::ApplicationCommandInfo &result)
{
    switch (commandID) {
            
        case nmix::Operation::Escape:
            
            result.setInfo("Escape", "Cancel Current Operation", "", 0);
            
            result.addDefaultKeypress(juce::KeyPress::escapeKey, juce::ModifierKeys::noModifiers);
            
            break;
            
        case nmix::Operation::SelectAll:
            
            result.setInfo("Select All", "Select All Nodes", "", 0);
            
            result.addDefaultKeypress('a', juce::ModifierKeys::commandModifier);
            
            break;
            
        case nmix::Operation::InvertSelection:
            
            result.setInfo("Invert Selection", "Invert Selection", "", 0);
            
            result.addDefaultKeypress('i', juce::ModifierKeys::commandModifier);
            
            break;
            
        case nmix::Operation::DeselectAll:
            
            result.setInfo("Deselect All", "Deselect All Nodes", "", 0);
            
            result.addDefaultKeypress('d', juce::ModifierKeys::commandModifier);
            
            break;
            
        case nmix::Operation::AddNode:
            
            result.setInfo("Add Node", "Add a New Node", "", 0);
            
            result.addDefaultKeypress('n', juce::ModifierKeys::commandModifier | juce::ModifierKeys::shiftModifier);
            
            break;
            
        case nmix::Operation::RemoveNode:
            
            result.setInfo("Remove Node", "Remove Selected Nodes", "", 0);
            
            result.addDefaultKeypress(juce::KeyPress::backspaceKey, juce::ModifierKeys::noModifiers);
            result.addDefaultKeypress(juce::KeyPress::deleteKey,    juce::ModifierKeys::noModifiers);
            
            break;
            
        case nmix::Operation::NudgeSelection:
            
            result.setInfo("Nudge", "Nudge Selected Nodes", "", 0);
            
            result.addDefaultKeypress(juce::KeyPress::upKey,    juce::ModifierKeys::noModifiers);
            result.addDefaultKeypress(juce::KeyPress::downKey,  juce::ModifierKeys::noModifiers);
            result.addDefaultKeypress(juce::KeyPress::leftKey,  juce::ModifierKeys::noModifiers);
            result.addDefaultKeypress(juce::KeyPress::rightKey, juce::ModifierKeys::noModifiers);
            
            result.addDefaultKeypress(juce::KeyPress::upKey,    juce::ModifierKeys::shiftModifier);
            result.addDefaultKeypress(juce::KeyPress::downKey,  juce::ModifierKeys::shiftModifier);
            result.addDefaultKeypress(juce::KeyPress::leftKey,  juce::ModifierKeys::shiftModifier);
            result.addDefaultKeypress(juce::KeyPress::rightKey, juce::ModifierKeys::shiftModifier);
            
            break;
            
        case nmix::Operation::LockSelection:
            
            result.setInfo("Lock Selection", "Lock Selected Nodes", "", 0);
            
            result.addDefaultKeypress('l', juce::ModifierKeys::commandModifier);
            
            break;
            
        case nmix::Operation::AdjustX:
            
            result.setInfo("Adjust X", "Adjust Selected Node X Position", "", 0);
            
            result.addDefaultKeypress('x', juce::ModifierKeys::noModifiers);
            
            break;
            
        case nmix::Operation::AdjustY:
            
            result.setInfo("Adjust Y", "Adjust Selected Node Y Position", "", 0);
            
            result.addDefaultKeypress('y', juce::ModifierKeys::noModifiers);
            
            break;
            
        case nmix::Operation::AdjustVolume:
            
            result.setInfo("Adjust Volume", "Adjust Selected Node Volumes", "", 0);
            
            result.addDefaultKeypress('v', juce::ModifierKeys::noModifiers);
            
            break;
            
        case nmix::Operation::AdjustBalance:
            
            result.setInfo("Adjust Balance", "Adjust Selected Node Balances", "", 0);
            
            result.addDefaultKeypress('b', juce::ModifierKeys::noModifiers);
            
        default:
            break;
    }
}

bool nmix::Stage::perform(const juce::ApplicationCommandTarget::InvocationInfo &info)
{
    switch (info.commandID)
    {
        case nmix::Operation::Escape:
            
            if (operationHandler.currentOperation == nmix::Operation::None)
            {
                operationHandler.currentOperation = nmix::Operation::Escape;
            }
            else if (operationHandler.currentOperation == nmix::Operation::Escape)
            {
                break;
            }
            else
            {
                operationHandler.currentOperation = nmix::Operation::None;
            }
            
            break;
            
        case nmix::Operation::SelectAll:
            
            operationHandler.selectAll();
            
            break;
            
        case nmix::Operation::InvertSelection:
        
            operationHandler.invertSelection();
            
            break;
        
            
        case nmix::Operation::DeselectAll:
            
            operationHandler.deselectAll();
            
            break;
            
        case nmix::Operation::AddNode:
        {
            
            operationHandler.addNode();
            
            break;
        }
            
        case nmix::Operation::RemoveNode:
        {
            
            operationHandler.deleteSelection();
            
            break;
        }
            
        case nmix::Operation::NudgeSelection:
        {
            
            operationHandler.nudgeSelection(info.keyPress);
            
            repaint();
            
            break;
        }
            
        case nmix::Operation::LockSelection:
            
            operationHandler.lockSelection();
            
            break;
            
        case nmix::Operation::AdjustX:
            
            operationHandler.currentOperation = nmix::Operation::AdjustX;
            
            break;
            
        case nmix::Operation::AdjustY:
            
            operationHandler.currentOperation = nmix::Operation::AdjustY;
            
            break;
            
        case nmix::Operation::AdjustVolume:
            
            operationHandler.currentOperation = nmix::Operation::AdjustVolume;
            
            break;
            
        case nmix::Operation::AdjustBalance:
            
            operationHandler.currentOperation = nmix::Operation::AdjustBalance;
            
            break;
            
        default:
            return false;
    }
    
    return true;
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
            int nWidth  = (*n)->getWidth();
            int nHeight = (*n)->getHeight();
            
            int radius;
            if (operationHandler.currentOperation == nmix::Operation::AdjustBalance)
            {
                radius = (*n)->currentOpOrigin.translated(nWidth/2, nHeight/2).getDistanceFrom(juce::Point<int>(w/2, h/2));
            }
            else
            {
                radius = (*n)->getPosition().translated(nWidth/2, nHeight/2).getDistanceFrom(juce::Point<int>(w/2, h/2));
            }
            
            g.setColour((*n)->findColour(nmix::Node::backgroundColourId));
            g.drawEllipse((w/2) - radius, (h/2) - radius, radius*2, radius*2, 1);
            g.drawLine(w/2, h/2, (*n)->getX() + nWidth/2, (*n)->getY() + nHeight/2, 1);
        }
    }
}

void nmix::Stage::resized()
{
    
}