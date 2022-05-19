/*
  ==============================================================================

    GainComponent.cpp
    Created: 19 May 2022 1:27:41pm
    Author:  Brian Mendoza

  ==============================================================================
*/

#include <JuceHeader.h>
#include "GainComponent.h"
#include "MainComponent.h"

//==============================================================================
GainComponent::GainComponent()
{
    decibelGainSlider.setRange(-97, -12);
    decibelGainSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    decibelGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    decibelGainSlider.onValueChange = [this] { mainLevel = juce::Decibels::decibelsToGain( (float) decibelGainSlider.getValue(), juce::Decibels::gainToDecibels(-96.0f) ); };   /**did not find a way to change the -96 dB as -INF representation b*/
    decibelGainSlider.setValue(juce::Decibels::gainToDecibels(mainLevel, -96.0f));
    decibelLabel.setText("Level in dB", juce::dontSendNotification);
    
    addAndMakeVisible(decibelGainSlider);
    addAndMakeVisible(decibelLabel);
}

GainComponent::~GainComponent()
{
}

void GainComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);
}

void GainComponent::resized()
{
    decibelGainSlider.setBounds(20, 20, getWidth() - 40, getHeight() - 40);
    decibelLabel.setBounds(30, getHeight() - 40, 120, 20);

}


/**
  Following note couldnt be implemented:
  
 Notes
 The code presented in the demo project for this tutorial assumes that we want to treat a value of -100 dB or lower as -INF dB (that is a linear gain value of zero). This value of -100 dB is the default value used by the Decibels class but you can override this in its calculations. This is achieved by providing an additional argument to each of the functions in the Decibels class specifying which value should be treated as -INF dB. For example, to use -96 dB (and below) as -INF dB when updating our slider value in the MainContentComponent constructor we could do this:

 decibelSlider.setValue (juce::Decibels::gainToDecibels (level, -96.0f));
 But of course we need to ensure that all parts of our application use the same value for -INF dB. There is one potential problem with the code for the demo project since we hard-code -100.0 in our DecibelSlider::getValueFromText() function. If the Decibels class changes its default value (for some reason) then our code would break. Unfortunately, this default value is a private member of the Decibels class, so we can't ask the Decibels class for its default value. Instead, we would need to specify our own default value and use this throughout.

 Exercise
 Modify the demo project to specify your own default value for -INF as -96 dB. You should need to update both the DecibelSlider class and the MainContentComponent class.*/
