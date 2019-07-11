/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (2, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }

	addAndMakeVisible(freqSlider_);
	initSliders();
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	//sample rate is usually 44100
	maxFreq_ = 12000;
	currFrequency_ = 417;
	minFreq_ = 20;
	phase_ = phaseBufferStart_ = 0;
	wtSize_ = 1024; //1024 samples,, 1024 value array
	sampleRate_ = sampleRate;
	increment_ = currFrequency_ * wtSize_ / sampleRate; // = 10.216780
	amplitude_ = 0.25;

	//one cycle of a sin wave
	for (int i = 0; i < wtSize_; i++)
	{
		//2PI
		//calculate one sin function and insert into the array
		waveTable_.insert(i, sin(2.0  * double_Pi * i / wtSize_)); //double_Pi is just Pi as a double/float
	}
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
	//audio processing code goes here
	                                     //pointer to the left speaker(0) and giving the starting sample index in the array
	float *leftSpeaker = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
	float *rightSpeaker = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample); //right speaker(1)

	//iterate through samples
	for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
	{
		leftSpeaker[sample] = waveTable_[(int)phase_] * amplitude_; //multiply by amplitude(0.25) to make sound quieter by making the amplitude smaller
		rightSpeaker[sample] = waveTable_[(int)phase_] * amplitude_;
		phase_ = fmod((phase_ + increment_), wtSize_); //add increment to phase. If we reach the max wave table size(wtSize)
		//fmod allows us to wrap back around to the beginning
	}
}

void MainComponent::releaseResources()
{
   
}

void MainComponent::paint (Graphics& g)
{
	g.fillAll(Colours::black);
}

void MainComponent::resized()
{
	const int border = 50;
	freqSlider_.setBounds(getLocalBounds().reduced(border));
}

void MainComponent::sliderValueChanged(Slider *slider)
{
	if (slider == &freqSlider_) //check if same address
	{
		currFrequency_ = freqSlider_.getValue(); //update values
		increment_ = currFrequency_ * wtSize_ / sampleRate_; //update increment
		freqSlider_.setValue(currFrequency_);
	}
}

void MainComponent::initSliders()
{
	freqSlider_.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
	freqSlider_.addListener(this);
	freqSlider_.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 20);
	freqSlider_.setTextValueSuffix(" Hz");
	freqSlider_.setRange(minFreq_, maxFreq_);
	freqSlider_.setValue(currFrequency_);
}
