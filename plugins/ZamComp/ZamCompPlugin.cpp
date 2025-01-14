/*
 * ZamComp mono compressor
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

#include "ZamCompPlugin.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZamCompPlugin::ZamCompPlugin()
    : Plugin(paramCount, 3, 0)
{
    // set default values
    loadProgram(0);
}

// -----------------------------------------------------------------------
// Init

void ZamCompPlugin::initParameter(uint32_t index, Parameter& parameter)
{
    switch (index)
    {
    case paramAttack:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Attack";
        parameter.symbol     = "att";
        parameter.unit       = "ms";
        parameter.ranges.def = 10.0f;
        parameter.ranges.min = 0.1f;
        parameter.ranges.max = 100.0f;
        break;
    case paramRelease:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Release";
        parameter.symbol     = "rel";
        parameter.unit       = "ms";
        parameter.ranges.def = 80.0f;
        parameter.ranges.min = 1.0f;
        parameter.ranges.max = 500.0f;
        break;
    case paramKnee:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Knee";
        parameter.symbol     = "kn";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 8.0f;
        break;
    case paramRatio:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Ratio";
        parameter.symbol     = "rat";
        parameter.unit       = " ";
        parameter.ranges.def = 4.0f;
        parameter.ranges.min = 1.0f;
        parameter.ranges.max = 20.0f;
        break;
    case paramThresh:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Threshold";
        parameter.symbol     = "thr";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -80.0f;
        parameter.ranges.max = 0.0f;
        break;
    case paramMakeup:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Makeup";
        parameter.symbol     = "mak";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 30.0f;
        break;
    case paramSlew:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Slew";
        parameter.symbol     = "slew";
        parameter.unit       = " ";
        parameter.ranges.def = 1.0f;
        parameter.ranges.min = 1.0f;
        parameter.ranges.max = 150.0f;
        break;
    case paramGainRed:
        parameter.hints      = kParameterIsOutput;
        parameter.name       = "Gain Reduction";
        parameter.symbol     = "gr";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 20.0f;
        break;
    case paramOutputLevel:
        parameter.hints      = kParameterIsOutput;
        parameter.name       = "Output Level";
        parameter.symbol     = "outlevel";
        parameter.unit       = "dB";
        parameter.ranges.def = -45.0f;
        parameter.ranges.min = -45.0f;
        parameter.ranges.max = 20.0f;
        break;
    }
}


void ZamCompPlugin::initProgramName(uint32_t index, String& programName)
{
	switch(index) {
	case 0:
		programName = "Zero";
		break;
	case 1:
		programName = "PoppySnare";
		break;
	case 2:
		programName = "VocalLeveller";
		break;
	}
}

void ZamCompPlugin::loadProgram(uint32_t index)
{
	switch(index) {
	case 0:
		attack = 10.0;
		release = 80.0;
		knee = 0.0;
		ratio = 4.0;
		thresdb = 0.0;
		makeup = 0.0;
		gainred = 0.0;
		slewfactor = 1.0;
		outlevel = -45.0;
		break;
	case 1:
		attack = 10.0;
		release = 10.0;
		knee = 1.0;
		ratio = 5.0;
		thresdb = -18.0;
		makeup = 6.0;
		gainred = 0.0;
		slewfactor = 20.0;
		outlevel = -45.0;
		break;
	case 2:
		attack = 50.0;
		release = 400.0;
		knee = 8.0;
		ratio = 5.0;
		thresdb = -16.0;
		makeup = 9.0;
		gainred = 0.0;
		slewfactor = 1.0;
		outlevel = -45.0;
		break;
	}

	activate();
}

// -----------------------------------------------------------------------
// Internal data

float ZamCompPlugin::getParameterValue(uint32_t index) const
{
    switch (index)
    {
    case paramAttack:
        return attack;
        break;
    case paramRelease:
        return release;
        break;
    case paramKnee:
        return knee;
        break;
    case paramRatio:
        return ratio;
        break;
    case paramThresh:
        return thresdb;
        break;
    case paramMakeup:
        return makeup;
        break;
    case paramSlew:
        return slewfactor;
        break;
    case paramGainRed:
        return gainred;
        break;
    case paramOutputLevel:
        return outlevel;
        break;
    default:
        return 0.0f;
    }
}

void ZamCompPlugin::setParameterValue(uint32_t index, float value)
{
    switch (index)
    {
    case paramAttack:
        attack = value;
        break;
    case paramRelease:
        release = value;
        break;
    case paramKnee:
        knee = value;
        break;
    case paramRatio:
        ratio = value;
        break;
    case paramThresh:
        thresdb = value;
        break;
    case paramMakeup:
        makeup = value;
        break;
    case paramSlew:
        slewfactor = value;
        break;
    case paramGainRed:
        gainred = value;
        break;
    case paramOutputLevel:
        outlevel = value;
        break;
    }
}

// -----------------------------------------------------------------------
// Process

void ZamCompPlugin::activate()
{
    gainred = 0.0f;
    outlevel = -45.0f;
    oldL_yl = oldL_y1 = oldL_yg = 0.f;
}

void ZamCompPlugin::run(const float** inputs, float** outputs, uint32_t frames)
{
	float srate = getSampleRate();
	float width = (6.f * knee) + 0.01;
	float slewwidth = 1.8f;
        float cdb=0.f;
        float attack_coeff = exp(-1000.f/(attack * srate));
        float release_coeff = exp(-1000.f/(release * srate));

        int attslew = 0;
        int relslew = 0;
	float max = 0.f;
	float lgaininp = 0.f;
	float rgaininp = 0.f;
	float Lgain = 1.f;
        float Rgain = 1.f;
        float Lxg, Lxl, Lyg, Lyl, Ly1;
        float checkwidth = 0.f;
	uint32_t i;

        for (i = 0; i < frames; i++) {
                relslew = 0;
                attslew = 0;
		Lyg = 0.f;
                Lxg = (inputs[0][i]==0.f) ? -160.f : to_dB(fabs(inputs[0][i]));
                Lxg = sanitize_denormal(Lxg);

                Lyg = Lxg + (1.f/ratio-1.f)*(Lxg-thresdb+width/2.f)*(Lxg-thresdb+width/2.f)/(2.f*width);

		checkwidth = 2.f*fabs(Lxg-thresdb);
                if (2.f*(Lxg-thresdb) < -width) {
                        Lyg = Lxg;
                } else if (checkwidth <= width) {
			Lyg = thresdb + (Lxg-thresdb)/ratio;
			Lyg = sanitize_denormal(Lyg);
			if (checkwidth <= slewwidth) {
				if (Lyg >= oldL_yg) {
					attslew = 1;
				} else {
					relslew = 1;
				}
			}
                } else if (2.f*(Lxg-thresdb) > width) {
                        Lyg = thresdb + (Lxg-thresdb)/ratio;
                        Lyg = sanitize_denormal(Lyg);
                }

                attack_coeff = attslew ? exp(-1000.f/((attack + 2.0*(slewfactor - 1)) * srate)) : attack_coeff;
                // Don't slew on release
		//release_coeff = relslew ? exp(-1000.f/((release + 2.0*(slewfactor - 1)) * srate)) : release_coeff;

                Lxl = Lxg - Lyg;

                oldL_y1 = sanitize_denormal(oldL_y1);
                oldL_yl = sanitize_denormal(oldL_yl);
                Ly1 = fmaxf(Lxl, release_coeff * oldL_y1+(1.f-release_coeff)*Lxl);
		Lyl = attack_coeff * oldL_yl+(1.f-attack_coeff)*Ly1;
                Ly1 = sanitize_denormal(Ly1);
                Lyl = sanitize_denormal(Lyl);

                cdb = -Lyl;
                Lgain = from_dB(cdb);

                gainred = Lyl;

		lgaininp = inputs[0][i] * Lgain;
                outputs[0][i] = lgaininp * from_dB(makeup);

		max = (fabsf(lgaininp) > max) ? fabsf(lgaininp) : sanitize_denormal(max);

                oldL_yl = Lyl;
                oldL_y1 = Ly1;
                oldL_yg = Lyg;
        }
	outlevel = (max == 0.f) ? -45.f : to_dB(max) - thresdb;
    }

// -----------------------------------------------------------------------

Plugin* createPlugin()
{
    return new ZamCompPlugin();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
