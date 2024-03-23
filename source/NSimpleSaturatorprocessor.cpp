//------------------------------------------------------------------------
// Copyright(c) 2024 Noah Dominic Miranda Silvio.
//------------------------------------------------------------------------

#include "NSimpleSaturatorprocessor.h"
#include "NSimpleSaturatorcids.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "public.sdk/source/vst/vstaudioprocessoralgo.h"


using namespace Steinberg;

namespace NoahDominic {
//------------------------------------------------------------------------
// NSimpleSaturatorProcessor
//------------------------------------------------------------------------
NSimpleSaturatorProcessor::NSimpleSaturatorProcessor ()
{
	//--- set the wanted controller for our processor
	setControllerClass (kNSimpleSaturatorControllerUID);
}

//------------------------------------------------------------------------
NSimpleSaturatorProcessor::~NSimpleSaturatorProcessor ()
{}

//------------------------------------------------------------------------
tresult PLUGIN_API NSimpleSaturatorProcessor::initialize (FUnknown* context)
{
	// Here the Plug-in will be instantiated
	
	//---always initialize the parent-------
	tresult result = AudioEffect::initialize (context);
	// if everything Ok, continue
	if (result != kResultOk)
	{
		return result;
	}

	//--- create Audio IO ------
	addAudioInput (STR16 ("Stereo In"), Steinberg::Vst::SpeakerArr::kStereo);
	addAudioOutput (STR16 ("Stereo Out"), Steinberg::Vst::SpeakerArr::kStereo);

	/* If you don't need an event bus, you can remove the next line */
	addEventInput (STR16 ("Event In"), 1);

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API NSimpleSaturatorProcessor::terminate ()
{
	// Here the Plug-in will be de-instantiated, last possibility to remove some memory!
	
	//---do not forget to call parent ------
	return AudioEffect::terminate ();
}

//------------------------------------------------------------------------
tresult PLUGIN_API NSimpleSaturatorProcessor::setActive (TBool state)
{
	//--- called when the Plug-in is enable/disable (On/Off) -----
	return AudioEffect::setActive (state);
}

//------------------------------------------------------------------------
tresult PLUGIN_API NSimpleSaturatorProcessor::process (Vst::ProcessData& data)
{
	//--- First : Read inputs parameter changes-----------

	if (data.inputParameterChanges)
	{
		int32 numParamsChanged = data.inputParameterChanges->getParameterCount ();
		for (int32 index = 0; index < numParamsChanged; index++)
		{
			if (auto* paramQueue = data.inputParameterChanges->getParameterData (index))
			{
				Vst::ParamValue value;
				int32 sampleOffset;
				int32 numPoints = paramQueue->getPointCount ();
				switch (paramQueue->getParameterId ())
				{
				case NSimpleSaturatorParams::kParamPreGainId:
					if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
						mPreGain = value;
					break;
				case NSimpleSaturatorParams::kParamSaturationId:
					if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
						mSaturation = value;
					break;
				case NSimpleSaturatorParams::kParamPostGainId:
					if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
						mPostGain = value;
					break;
				case NSimpleSaturatorParams::kParamMixId:
					if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
						mMix = value;
					break;
				}
			}
		}
	}
	
	//--- Here you have to implement your processing

	if (data.numSamples > 0)
	{
		//--- ------------------------------------------
		// here as example a default implementation where we try to copy the inputs to the outputs:
		// if less input than outputs then clear outputs
		//--- ------------------------------------------
		
		int32 minBus = std::min (data.numInputs, data.numOutputs);
		for (int32 i = 0; i < minBus; i++)
		{
			int32 minChan = std::min (data.inputs[i].numChannels, data.outputs[i].numChannels);
			for (int32 c = 0; c < minChan; c++)
			{
				int32 samples = data.numSamples;
				Vst::Sample32* ptrIn = data.inputs[i].channelBuffers32[c];
				Vst::Sample32* ptrOut = data.outputs[i].channelBuffers32[c];
				Vst::Sample32 temp;
				Vst::Sample32 raw;

				while (--samples >= 0)
				{
					temp = (*ptrIn++);
					raw = temp;


					temp *= mPreGain * 2;
					temp = mSaturation == 0 ? temp : tanhf(temp * mSaturation * 10)/(mSaturation*10);
					temp *= mPostGain * 2;
					temp = temp * mMix + raw * (1.0 - mMix);

					(*ptrOut++) = temp;
				}
			}
			data.outputs[i].silenceFlags = data.inputs[i].silenceFlags;
				
			// clear the remaining output buffers
			for (int32 c = minChan; c < data.outputs[i].numChannels; c++)
			{
				// clear output buffers
				memset (data.outputs[i].channelBuffers32[c], 0,
						data.numSamples * sizeof (Vst::Sample32));

				// inform the host that this channel is silent
				data.outputs[i].silenceFlags |= ((uint64)1 << c);
			}
		}
		// clear the remaining output buffers
		for (int32 i = minBus; i < data.numOutputs; i++)
		{
			// clear output buffers
			for (int32 c = 0; c < data.outputs[i].numChannels; c++)
			{
				memset (data.outputs[i].channelBuffers32[c], 0,
						data.numSamples * sizeof (Vst::Sample32));
			}
			// inform the host that this bus is silent
			data.outputs[i].silenceFlags = ((uint64)1 << data.outputs[i].numChannels) - 1;
		}
	}

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API NSimpleSaturatorProcessor::setupProcessing (Vst::ProcessSetup& newSetup)
{
	//--- called before any processing ----
	return AudioEffect::setupProcessing (newSetup);
}

//------------------------------------------------------------------------
tresult PLUGIN_API NSimpleSaturatorProcessor::canProcessSampleSize (int32 symbolicSampleSize)
{
	// by default kSample32 is supported
	if (symbolicSampleSize == Vst::kSample32)
		return kResultTrue;

	// disable the following comment if your processing support kSample64
	/* if (symbolicSampleSize == Vst::kSample64)
		return kResultTrue; */

	return kResultFalse;
}

//------------------------------------------------------------------------
tresult PLUGIN_API NSimpleSaturatorProcessor::setState (IBStream* state)
{
	if (!state)
		return kResultFalse;

	// called when we load a preset, the model has to be reloaded
	IBStreamer streamer (state, kLittleEndian);
	
	float savedParams[] = { 0.f, 0.f, 0.f, 0.f };
	if (streamer.readFloatArray(savedParams, 4) == false)
	{
		return kResultFalse;
	}
	mPreGain = savedParams[0];
	mSaturation = savedParams[1];
	mPostGain = savedParams[2];
	mMix = savedParams[3];

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API NSimpleSaturatorProcessor::getState (IBStream* state)
{
	// here we need to save the model
	float toSaveParams[]{ mPreGain, mSaturation, mPostGain, mMix };
	IBStreamer streamer (state, kLittleEndian);
	streamer.writeFloatArray(toSaveParams, 4);


	return kResultOk;
}

//------------------------------------------------------------------------
} // namespace NoahDominic
