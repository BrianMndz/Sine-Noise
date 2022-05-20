/*
  ==============================================================================

    GainComponent.h
    Created: 19 May 2022 1:27:41pm
    Author:  Brian Mendoza

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/

/**Combining the decibel gain component with the sine wave tutorial.
    Lets create a class from slider */
class DecibelSlider : public juce::Slider
{
public:
    DecibelSlider() {} /** Default Constructor */
    
    double getValueFromText(const juce::String & text) override
    {
        auto minusInfinitydB = -96.0f;
        /**Strip off the "dB" suffix from the text if present. and trim the text to remove any leading or trailing whitespaces */
        auto decibelText = text.upToFirstOccurrenceOf("dB", false, false).trim();
        /**Check if the remaining text is "-INF" and return -96. Otherwise we convert text to a double value and return it */
        return decibelText.equalsIgnoreCase("-INF") ? minusInfinitydB : decibelText.getDoubleValue();
    }
    
    juce::String getTextFromValue(double value) override
    {
        return juce::Decibels::toString(value);
    }
};

class GainComponent  : public juce::Component
{
public:
    GainComponent();
    ~GainComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    /**Let's check if we can update this float (representing the vol level) with the decibel slider component */
    float mainLevel = 0.05f;
private:
    /**Create the gain stage in the plug in. Lets create it directly in decibel control */
    DecibelSlider decibelGainSlider;
    juce::Label decibelLabel;
    
    juce::Slider gainSlider;
    juce::Label gainLabel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GainComponent)
};
