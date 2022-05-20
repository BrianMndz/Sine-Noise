#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    addAndMakeVisible(gainComponent);
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
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
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
            auto output = randomGen.nextFloat() * currentLevel;
            writePointer[sample] = output;
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
}
