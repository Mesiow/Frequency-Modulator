/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public AudioAppComponent, public Slider::Listener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;

	void sliderValueChanged(Slider *slider)override;


private:
	void initSliders();

private:
	Array<float> waveTable_; //juce array, similar to a vector
	double wtSize_;
	double maxFreq_;
	double currFrequency_;
	double minFreq_;
	double phase_; //position within 1 cycle of the wave table
	double phaseBufferStart_;
	double increment_; //how much to move to the next value
	double amplitude_;
	double sampleRate_;
	Random rand_;

	Slider freqSlider_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
