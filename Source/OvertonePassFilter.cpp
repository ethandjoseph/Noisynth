#include "OvertonePassFilter.h"

void OvertonePassFilter::prepare(const juce::dsp::ProcessSpec& spec)
{
	bandPassFilter1.prepare(spec);
	bandPassFilter2.prepare(spec);
	bandPassFilter3.prepare(spec);
	bandPassFilter4.prepare(spec);
	bandPassFilter1.reset();
	bandPassFilter2.reset();
	bandPassFilter3.reset();
	bandPassFilter4.reset();
	bandPassFilter1.setType(juce::dsp::StateVariableTPTFilterType::bandpass);
	bandPassFilter2.setType(juce::dsp::StateVariableTPTFilterType::bandpass);
	bandPassFilter3.setType(juce::dsp::StateVariableTPTFilterType::bandpass);
	bandPassFilter4.setType(juce::dsp::StateVariableTPTFilterType::bandpass);
}

void OvertonePassFilter::setFrequency(float newFrequency)
{
    frequency = newFrequency;
}

void OvertonePassFilter::process(juce::AudioBuffer<float>& buffer)
{
	juce::AudioBuffer<float> tempBuffer1, tempBuffer2, tempBuffer3, tempBuffer4;

	tempBuffer1.makeCopyOf(buffer);
	tempBuffer2.makeCopyOf(buffer);
	tempBuffer3.makeCopyOf(buffer);
	tempBuffer4.makeCopyOf(buffer);

	bool filterBypass1, filterBypass2, filterBypass3, filterBypass4 = false;

	if ((8 * frequency) > 20000.)
	{
		filterBypass4 = true;
		tempBuffer4.clear();
		bandPassFilter4.reset();
	}
	else
	{
		filterBypass4 = false;
	}

	if ((4 * frequency) > 20000.)
	{
		filterBypass3 = true;
		tempBuffer3.clear();
		bandPassFilter3.reset();
	}
	else
	{
		filterBypass3 = false;
	}

	if ((2 * frequency) > 20000.)
	{
		filterBypass2 = true;
		tempBuffer2.clear();
		bandPassFilter2.reset();
	}
	else
	{
		filterBypass2 = false;
	}

	if (frequency > 20000.)
	{
		filterBypass1 = true;
		tempBuffer1.clear();
		bandPassFilter1.reset();
	}
	else
	{
		filterBypass1 = false;
	}

	if (!filterBypass1)
	{
		bandPassFilter1.setCutoffFrequency(frequency);
		bandPassFilter1.setResonance(8.0);//(resonance);
	}

	if (!filterBypass2)
	{
		bandPassFilter2.setCutoffFrequency(2 * frequency);
		bandPassFilter2.setResonance(8.0);//(resonance);

	}

	if (!filterBypass3)
	{
		bandPassFilter3.setCutoffFrequency(4 * frequency);
		bandPassFilter3.setResonance(8.0);//(resonance);

	}

	if (!filterBypass4)
	{
		bandPassFilter4.setCutoffFrequency(8 * frequency);
		bandPassFilter4.setResonance(8.0);//(resonance);
	}

	auto audioBlock1 = juce::dsp::AudioBlock<float>(tempBuffer1);
	auto audioBlock2 = juce::dsp::AudioBlock<float>(tempBuffer2);
	auto audioBlock3 = juce::dsp::AudioBlock<float>(tempBuffer3);
	auto audioBlock4 = juce::dsp::AudioBlock<float>(tempBuffer4);

	auto context1 = juce::dsp::ProcessContextReplacing<float>(audioBlock1);
	auto context2 = juce::dsp::ProcessContextReplacing<float>(audioBlock2);
	auto context3 = juce::dsp::ProcessContextReplacing<float>(audioBlock3);
	auto context4 = juce::dsp::ProcessContextReplacing<float>(audioBlock4);

	bandPassFilter1.process(context1);
	bandPassFilter2.process(context2);
	bandPassFilter3.process(context3);
	bandPassFilter4.process(context4);

	float filterGain1 = juce::Decibels::decibelsToGain(0.);
	float filterGain2 = juce::Decibels::decibelsToGain(-2.);
	float filterGain3 = juce::Decibels::decibelsToGain(-4.);
	float filterGain4 = juce::Decibels::decibelsToGain(-6.);

	for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
	{
		auto* mainBuffer = buffer.getWritePointer(channel);
		auto* data1 = tempBuffer1.getReadPointer(channel);
		auto* data2 = tempBuffer2.getReadPointer(channel);
		auto* data3 = tempBuffer3.getReadPointer(channel);
		auto* data4 = tempBuffer4.getReadPointer(channel);

		for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
		{
			mainBuffer[sample] = data1[sample] + filterGain2 * data2[sample] + filterGain3 * data3[sample] + filterGain4 * data4[sample];
		}
	}

	float gainCompensation = juce::Decibels::decibelsToGain(-18.0);
	for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
	{
		auto* channelData = buffer.getWritePointer(channel);

		for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
		{
			channelData[sample] *= gainCompensation;
		}
	}
}