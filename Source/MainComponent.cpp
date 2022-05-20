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
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
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
    
    auto currentLevel = gainComponent.mainLevel;
    
    auto* localBuffer = bufferToFill.buffer;
    auto numChannels = localBuffer->getNumChannels();
    auto numSamples = localBuffer->getNumSamples();
    
    for(auto chan = 0; chan < numChannels; ++chan)
    {
        auto* writePointer = localBuffer->getWritePointer(chan);
        
        for(auto sample = 0; sample < numSamples; ++sample)
        {
            /** For each output sample we calculate the sine function for current angle.
             * then increment the angle for the next sample */
            auto currentSample = (float) std::sin (synthComponent.currentAngle);
            synthComponent.currentAngle += synthComponent.angleDelta;
            auto output = currentSample * currentLevel;
            writePointer[sample] = output;
            //Code commented to produce noise
            //auto output = randomGen.nextFloat() * currentLevel;*/
        }
    }
}

void MainComponent::releaseResources()
{
    
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    gainComponent.setBounds(getWidth() / 2, getHeight() / 2, getWidth() / 2, getHeight() / 2);
    synthComponent.setBounds(0, 0, getWidth(), getHeight() / 2);
}
