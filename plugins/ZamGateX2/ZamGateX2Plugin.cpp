/*
 * ZamGateX2 stereo gate plugin
 * Copyright (C) 2014  Damien Zammit <damien@zamaudio.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * For a full copy of the GNU General Public License see the doc/GPL.txt file.
 */

#include "ZamGateX2Plugin.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZamGateX2Plugin::ZamGateX2Plugin()
	: Plugin(paramCount, 1, 0) // 1 program, 0 states
{
	// set default values
	loadProgram(0);
}

// -----------------------------------------------------------------------
// Init

void ZamGateX2Plugin::initProgramName(uint32_t index, String& programName)
{
	if (index != 0)
		return;

	programName = "Default";
}

// -----------------------------------------------------------------------
// Internal data

void ZamGateX2Plugin::initParameter(uint32_t index, Parameter& parameter)
{
	switch (index)
	{
	case paramAttack:
		parameter.hints = kParameterIsAutomable;
		parameter.name = "Attack";
		parameter.symbol = "att";
		parameter.unit = "ms";
		parameter.ranges.def = 50.0f;
		parameter.ranges.min = 0.1f;
		parameter.ranges.max = 500.0f;
		break;
	case paramRelease:
		parameter.hints = kParameterIsAutomable;
		parameter.name = "Release";
		parameter.symbol = "rel";
		parameter.unit = "ms";
		parameter.ranges.def = 100.0f;
		parameter.ranges.min = 0.1f;
		parameter.ranges.max = 500.0f;
		break;
	case paramThresh:
		parameter.hints = kParameterIsAutomable;
		parameter.name = "Threshold";
		parameter.symbol = "thr";
		parameter.unit = "dB";
		parameter.ranges.def = -60.0f;
		parameter.ranges.min = -60.0f;
		parameter.ranges.max = 0.0f;
		break;
	case paramMakeup:
		parameter.hints = kParameterIsAutomable;
		parameter.name = "Makeup";
		parameter.symbol = "mak";
		parameter.unit = "dB";
		parameter.ranges.def = 0.0f;
		parameter.ranges.min = -30.0f;
		parameter.ranges.max = 30.0f;
		break;
	case paramGainR:
		parameter.hints = kParameterIsOutput;
		parameter.name = "Gain Reduction";
		parameter.symbol = "gainr";
		parameter.unit = "dB";
		parameter.ranges.def = 0.0f;
		parameter.ranges.min = 0.0f;
		parameter.ranges.max = 40.0f;
		break;
	case paramOutputLevel:
		parameter.hints = kParameterIsOutput;
		parameter.name = "Output Level";
		parameter.symbol = "outlevel";
		parameter.unit = "dB";
		parameter.ranges.def = -45.0f;
		parameter.ranges.min = -45.0f;
		parameter.ranges.max = 20.0f;
		break;
	}
}

// -----------------------------------------------------------------------
// Internal data

float ZamGateX2Plugin::getParameterValue(uint32_t index) const
{
	switch (index)
	{
	case paramAttack:
		return attack;
		break;
	case paramRelease:
		return release;
		break;
	case paramThresh:
		return thresdb;
		break;
	case paramMakeup:
		return makeup;
		break;
	case paramGainR:
		return gainr;
		break;
	case paramOutputLevel:
		return outlevel;
		break;
	default:
		return 0.0f;
	}
}

void ZamGateX2Plugin::setParameterValue(uint32_t index, float value)
{
	switch (index)
	{
	case paramAttack:
		attack = value;
		break;
	case paramRelease:
		release = value;
		break;
	case paramThresh:
		thresdb = value;
		break;
	case paramMakeup:
		makeup = value;
		break;
	case paramGainR:
		gainr = value;
		break;
	case paramOutputLevel:
		outlevel = value;
		break;
	}
}

void ZamGateX2Plugin::loadProgram(uint32_t index)
{
	attack = 50.0;
	release = 100.0;
	thresdb = -60.0;
	gainr = 0.0;
	makeup = 0.0;
	outlevel = -45.0;
	activate();
}

// -----------------------------------------------------------------------
// Process

void ZamGateX2Plugin::activate()
{
	int i;
	gatestatel = 0.f;
	gatestater = 0.f;
	posl = 0;
	posr = 0;
	for (i = 0; i < MAX_GATE; i++) {
		samplesl[i] = 0.f;
		samplesr[i] = 0.f;
	}
}

void ZamGateX2Plugin::pushsamplel(float samples[], float sample)
{
	++posl;
	if (posl >= MAX_GATE)
		posl = 0;
	samples[posl] = sample;
}

void ZamGateX2Plugin::pushsampler(float samples[], float sample)
{
	++posr;
	if (posr >= MAX_GATE)
		posr = 0;
	samples[posr] = sample;
}

float ZamGateX2Plugin::averageabs(float samples[])
{
	int i;
	float average = 0.f;

	for (i = 0; i < MAX_GATE; i++) {
		average += samples[i]*samples[i];
	}
	average /= (float) MAX_GATE;
	return sqrt(average);
}

void ZamGateX2Plugin::run(const float** inputs, float** outputs, uint32_t frames)
{
	uint32_t i;
	float absamplel, absampler, absample;
	float att;
	float rel;
	float gl, gr;
	float ming;
	float fs;
	fs = getSampleRate();
	gl = gatestatel;
	gr = gatestater;
	att = 1000.f / (attack * fs);
	rel = 1000.f / (release * fs);

	for(i = 0; i < frames; i++) {
		pushsamplel(samplesl, inputs[0][i]);
		pushsampler(samplesr, inputs[1][i]);
		absamplel = averageabs(samplesl);
		absampler = averageabs(samplesr);
		absample = std::max(absamplel, absampler);
		if (absample < from_dB(thresdb)) {
			gr -= rel;
			if (gr < 0.f)
				gr = 0.f;
			gl -= rel;
			if (gl < 0.f)
				gl = 0.f;
		} else {
			gr += att;
			if (gr > 1.f)
				gr = 1.f;
			gl += att;
			if (gl > 1.f)
				gl = 1.f;
		}

		gatestatel = gl;
		gatestater = gr;

		outputs[0][i] = gl * from_dB(makeup) * inputs[0][i];
		outputs[1][i] = gr * from_dB(makeup) * inputs[1][i];
		ming = std::max(gr, gl);
		gainr = (ming > 0) ? sanitize_denormal(-to_dB(ming)) : 40.0;
		gainr = std::min(gainr, 40.f);
		outlevel = (absample > 0) ? to_dB(absample) - thresdb : -60.0;
	}
}

// -----------------------------------------------------------------------

Plugin* createPlugin()
{
	return new ZamGateX2Plugin();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
