//------------------------------------------------------------------------
// Copyright(c) 2024 Noah Dominic Miranda Silvio.
//------------------------------------------------------------------------

#pragma once

#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/vst/vsttypes.h"

namespace NoahDominic {
//------------------------------------------------------------------------
static const Steinberg::FUID kNSimpleSaturatorProcessorUID (0x9E66FA67, 0x818C5E25, 0xAD9CFCA0, 0x85E38588);
static const Steinberg::FUID kNSimpleSaturatorControllerUID (0x32E8FB1D, 0x459A56DB, 0xA737CAF6, 0x3863DF25);

#define NSimpleSaturatorVST3Category "Fx"

enum NSimpleSaturatorParams : Steinberg::Vst::ParamID
{
	kParamPreGainId,
	kParamSaturationId,
	kParamPostGainId,
	kParamMixId,
};

//------------------------------------------------------------------------
} // namespace NoahDominic
