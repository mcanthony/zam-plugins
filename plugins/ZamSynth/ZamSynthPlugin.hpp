/*
 * ZamSynth mono compressor 
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

#ifndef ZAMSYNTHPLUGIN_HPP_INCLUDED
#define ZAMSYNTHPLUGIN_HPP_INCLUDED

#include "DistrhoPlugin.hpp"
#define MAX_VOICES 64

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class ZamSynthPlugin : public Plugin
{
public:
    enum Parameters
    {
        paramAttack = 0,
        paramCount
    };

    ZamSynthPlugin();
    ~ZamSynthPlugin() override;

protected:
    // -------------------------------------------------------------------
    // Information

    const char* d_getLabel() const noexcept override
    {
        return "ZamSynth";
    }

    const char* d_getMaker() const noexcept override
    {
        return "Damien Zammit";
    }

    const char* d_getLicense() const noexcept override
    {
        return "GPL v2+";
    }

    uint32_t d_getVersion() const noexcept override
    {
        return 0x1000;
    }

    long d_getUniqueId() const noexcept override
    {
        return d_cconst('Z', 'C', 'M', 'P');
    }

    // -------------------------------------------------------------------
    // Init

    void d_initParameter(uint32_t index, Parameter& parameter) ;
    void d_initProgramName(uint32_t index, d_string& programName) ;

    // -------------------------------------------------------------------
    // Internal data

    float d_getParameterValue(uint32_t index) const override;
    void  d_setParameterValue(uint32_t index, float value) override;
    void  d_setProgram(uint32_t index) ;

    // -------------------------------------------------------------------
    // Process

	static inline float
	sanitize_denormal(float v) {
	        if(!std::isnormal(v))
	                return 0.f;
	        return v;
	}

	static inline float
	from_dB(float gdb) {
	        return (exp(gdb/20.f*log(10.f)));
	}

	static inline float
	to_dB(float g) {
	        return (20.f*log10(g));
	}

    void d_activate() override;
    void d_deactivate() override;
    void d_run(float** inputs, float** outputs, uint32_t frames,
    		const MidiEvent* midievent, uint32_t midicount) override;

    // -------------------------------------------------------------------

private:
    float attack;
    float old_yl, old_y1;
    float rampstate[128], rampfreq[128], amp[128];
    int noteon[128];
    int noteoff[128];
    int voice[128];
    int totalvoices;
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif  // ZAMSYNTH_HPP_INCLUDED