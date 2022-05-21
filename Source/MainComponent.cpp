#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    addAndMakeVisible(gainComponent);
    addAndMakeVisible(synthComponent);
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // No inputs, two outputs
        setAudioChannels (0, 2);
    }
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
/**We need to know the output sample rate. How frequently the samples are being generated */
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    synthComponent.currentSampleRate = sampleRate;
    synthComponent.updateAngleDelta();
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();
    
    auto* leftBuffer  = bufferToFill.buffer->getWritePointer (0, bufferToFill.startSample);
    auto* rightBuffer = bufferToFill.buffer->getWritePointer (1, bufferToFill.startSample);
    
    /**Adding a smoothing change in frequencies */
    auto localTargetFreq = synthComponent.targetFreq;
    
    if(localTargetFreq != synthComponent.currentFreq)
    {
        /**Calculate the required increment per sample */
        auto freqIncrement = (localTargetFreq - synthComponent.currentFreq) / bufferToFill.numSamples;
        
        for(auto sample = 0; sample < bufferToFill.numSamples; ++sample)
        {
            /** For each output sample we calculate the sine function for current angle.
             * then increment the angle for the next sample */
            auto currentSample = (float) std::sin (synthComponent.currentAngle);
            synthComponent.currentFreq += freqIncrement;    //Increment the current frequency
            synthComponent.updateAngleDelta();              //Update the angle based on the new frequency.
            synthComponent.currentAngle += synthComponent.angleDelta;
            auto output = currentSample;
            leftBuffer[sample] = output;
            rightBuffer[sample] = output;
        }
        
        synthComponent.currentFreq = localTargetFreq;
    }
    else
    {
        for(auto sample = 0; sample < bufferToFill.numSamples; ++sample)
        {
            /** For each output sample we calculate the sine function for current angle.
             * then increment the angle for the next sample */
            auto currentSample = (float) std::sin (synthComponent.currentAngle);
            synthComponent.currentAngle += synthComponent.angleDelta;
            auto output = currentSample;
            leftBuffer[sample] = output;
            rightBuffer[sample] = output;
        }
    }
    
    auto localTargetLevel = gainComponent.mainLevel;
    bufferToFill.buffer->applyGainRamp(bufferToFill.startSample, bufferToFill.numSamples, gainComponent.mainLevel, localTargetLevel);
    gainComponent.mainLevel = localTargetLevel;
}

void MainComponent::releaseResources()
{
    
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    gainComponent.setBounds(getWidth() / 2, getHeight() / 2, getWidth() / 2, getHeight() / 2);
    synthComponent.setBounds(0, 0, getWidth(), getHeight() / 2);
}
