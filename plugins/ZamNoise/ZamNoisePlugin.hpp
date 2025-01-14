/*
 * ZamNoise  Noise detection and removal plugin 
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

#ifndef ZAMNOISEPLUGIN_HPP_INCLUDED
#define ZAMNOISEPLUGIN_HPP_INCLUDED

#include "DistrhoPlugin.hpp"
#include "Denoise.hpp"

typedef struct {
	unsigned long cbsize; // size of circular buffer
	float* cbi;
} CircularBuffer;


START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class ZamNoisePlugin : public Plugin
{
public:
    enum Parameters
    {
        paramNoiseToggle,
        paramCount
    };

    ZamNoisePlugin();
    ~ZamNoisePlugin() override;

protected:
    // -------------------------------------------------------------------
    // Information

    const char* getLabel() const noexcept override
    {
        return "ZamNoise";
    }

    const char* getMaker() const noexcept override
    {
        return "Damien Zammit";
    }

    const char* getLicense() const noexcept override
    {
        return "GPL v2+";
    }

    uint32_t getVersion() const noexcept override
    {
        return 0x1000;
    }

    int64_t getUniqueId() const noexcept override
    {
        return d_cconst('Z', 'N', 'O', 'I');
    }

    // -------------------------------------------------------------------
    // Init

    void initParameter(uint32_t index, Parameter& parameter) override;
    void initProgramName(uint32_t index, String& programName) override;

    // -------------------------------------------------------------------
    // Internal data

    float getParameterValue(uint32_t index) const override;
    void  setParameterValue(uint32_t index, float value) override;
    void  loadProgram(uint32_t index) override;

    // -------------------------------------------------------------------
    void activate() override;
    void deactivate() override;
    void run(const float** inputs, float** outputs, uint32_t frames) override;

    // -------------------------------------------------------------------
	float noisetoggle;

	void InstantiateCircularBuffer(CircularBuffer* buffer, unsigned long SampleRate);
	inline void IncrementPointer(CircularBuffer& buffer);	
	void init(float srate);
	int noverlap;
	CircularBuffer buffer;
	Denoise* zamnoise;
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif  // ZAMNOISE_HPP_INCLUDED
