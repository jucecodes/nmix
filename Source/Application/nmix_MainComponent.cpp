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

#include "nmix_MainComponent.h"
#include "nmix_Stage.h"
#include "nmix_Node.h"
#include "nmix_Viewport.h"
#include "nmix_Application.h"
#include "nmix_OperationHandler.h"
#include "nmix_Channel.h"
#include "nmix_Output.h"

nmix::MainComponent::MainComponent()
{
    viewport = new nmix::Viewport();
    addAndMakeVisible(viewport);
    
    nmix::OperationHandler& op = nmix::Application::getOperationHandler();
    
    stage = new nmix::Stage(op);
    addAndMakeVisible(stage);
    
    op.currentStage    = stage;
    op.currentViewport = viewport;
    
    setSize (675, 675);
    setAudioChannels (0, 2);

    stage->master = new nmix::Output(*stage, op);
    stage->master->setCentrePosition(stage->getWidth()/2, stage->getHeight()/2);
    stage->addAndMakeVisible(stage->master);
    op.stagedNodes.add(stage->master);
}

nmix::MainComponent::~MainComponent()
{
    viewport = nullptr;
    stage    = nullptr;
    shutdownAudio();
}

void nmix::MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    
}

void nmix::MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo &bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();
}

void nmix::MainComponent::releaseResources()
{
    
}

void nmix::MainComponent::paint(juce::Graphics &g)
{
    g.fillAll(juce::Colours::black);
}

void nmix::MainComponent::resized()
{
    stage->setBounds(getBounds());
    viewport->setBounds(getBounds());
    viewport->toFront(false);
}