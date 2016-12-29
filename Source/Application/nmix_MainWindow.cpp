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

#include "nmix_MainWindow.h"
#include "nmix_Operations.h"
#include "nmix_OperationHandler.h"
#include "nmix_Application.h"
#include "nmix_MainComponent.h"

nmix::MainWindow::MainWindow(juce::String name) : juce::DocumentWindow(name, juce::Colours::lightgrey, DocumentWindow::allButtons)
{
    setUsingNativeTitleBar (true);
    setContentOwned (new nmix::MainComponent(), true);
    setResizable (false, true);
    
    juce::ApplicationCommandManager& commandManager = nmix::Application::getCommandManager();
    commandManager.registerAllCommandsForTarget(this);
    addKeyListener(commandManager.getKeyMappings());
    
    setWantsKeyboardFocus(true);
    
    centreWithSize (getWidth(), getHeight());
    setVisible (true);
}

juce::ApplicationCommandTarget* nmix::MainWindow::getNextCommandTarget()
{
    return nullptr;
}

void nmix::MainWindow::getAllCommands(juce::Array<juce::CommandID> &commands)
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
        
        nmix::Operation::PositionSelectionX,
        nmix::Operation::PositionSelectionY,
        nmix::Operation::PositionSelectionDistance,
        nmix::Operation::PositionSelectionAzimuth
    };
    
    commands.addArray(ids, juce::numElementsInArray(ids));
}

void nmix::MainWindow::getCommandInfo(juce::CommandID commandID, juce::ApplicationCommandInfo &result)
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
            
        case nmix::Operation::PositionSelectionX:
            
            result.setInfo("Adjust X", "Adjust Selected Node X Position", "", 0);
            
            result.addDefaultKeypress('x', juce::ModifierKeys::noModifiers);
            
            break;
            
        case nmix::Operation::PositionSelectionY:
            
            result.setInfo("Adjust Y", "Adjust Selected Node Y Position", "", 0);
            
            result.addDefaultKeypress('y', juce::ModifierKeys::noModifiers);
            
            break;
            
        case nmix::Operation::PositionSelectionDistance:
            
            result.setInfo("Adjust Volume", "Adjust Selected Node Volumes", "", 0);
            
            result.addDefaultKeypress('v', juce::ModifierKeys::noModifiers);
            
            break;
            
        case nmix::Operation::PositionSelectionAzimuth:
            
            result.setInfo("Adjust Balance", "Adjust Selected Node Balances", "", 0);
            
            result.addDefaultKeypress('b', juce::ModifierKeys::noModifiers);
            
        default:
            break;
    }
}

bool nmix::MainWindow::perform(const juce::ApplicationCommandTarget::InvocationInfo &info)
{
    
    nmix::OperationHandler& operationHandler = nmix::Application::getOperationHandler();
    
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
            
        case nmix::Operation::PositionSelectionX:
            
            operationHandler.currentOperation = nmix::Operation::PositionSelectionX;
            
            break;
            
        case nmix::Operation::PositionSelectionY:
            
            operationHandler.currentOperation = nmix::Operation::PositionSelectionY;
            
            break;
            
        case nmix::Operation::PositionSelectionDistance:
            
            operationHandler.currentOperation = nmix::Operation::PositionSelectionDistance;
            
            break;
            
        case nmix::Operation::PositionSelectionAzimuth:
            
            operationHandler.currentOperation = nmix::Operation::PositionSelectionAzimuth;
            
            break;
            
        default:
            return false;
    }
    
    return true;
}

void nmix::MainWindow::closeButtonPressed()
{
    juce::JUCEApplication::getInstance()->systemRequestedQuit();
}