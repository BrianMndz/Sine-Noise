/*
  ==============================================================================

    SynthComponent.cpp
    Created: 19 May 2022 1:25:52pm
    Author:  Brian Mendoza

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SynthComponent.h"

//==============================================================================
SynthComponent::SynthComponent()
{
    addAndMakeVisible(freqSlider);
    freqSlider.setRange(50.0, 5000.0);
    freqSlider.setSkewFactorFromMidPoint(500.0);
    freqSlider.setValue(currentFreq, juce::dontSendNotification);
    /**The lambda function to start only if we have a valid sample Rate */
    freqSlider.onValueChange = [this] {
        targetFreq = freqSlider.getValue();
    };
}

SynthComponent::~SynthComponent()
{
}

void SynthComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::darkgrey);   
}

void SynthComponent::resized()
{
    freqSlider.setBounds(30, getHeight() / 4, getWidth() - 75, getHeight() - 100);
}

void SynthComponent::updateAngleDelta()
{
    auto cyclesPerSample = freqSlider.getValue() / currentSampleRate;   //Calculate the number of cycles to complete for each output sample.
    angleDelta = cyclesPerSample * 2.0 * juce::MathConstants<double>::pi;   //Multiplied by length of a whole sine wave cycle (2pi Rad)
}
