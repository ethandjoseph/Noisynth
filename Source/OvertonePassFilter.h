#pragma once
#include <JuceHeader.h>

class OvertonePassFilter
{
public:
    OvertonePassFilter() {}
	void prepare(const juce::dsp::ProcessSpec& spec);
    void setFrequency(float newFrequency);
    void process(juce::AudioBuffer<float>& buffer);

private:
    float frequency = 20.0f;
    juce::dsp::StateVariableTPTFilter<float> bandPassFilter1;
    juce::dsp::StateVariableTPTFilter<float> bandPassFilter2;
    juce::dsp::StateVariableTPTFilter<float> bandPassFilter3;
    juce::dsp::StateVariableTPTFilter<float> bandPassFilter4;
};