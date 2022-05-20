/*
  ==============================================================================

    SynthComponent.h
    Created: 19 May 2022 1:25:52pm
    Author:  Brian Mendoza

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class SynthComponent  : public juce::Component
{
public:
    SynthComponent();
    ~SynthComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    /**Added a function to change the angle*/
    void updateAngleDelta();
    
    double currentSampleRate = 0.0, currentAngle = 0.0, angleDelta = 0.0;
    /**Adding component to remove artifacts */
    double currentFreq = 500.0, targetFreq = 500.0;

private:
    juce::Slider freqSlider;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthComponent)
};
