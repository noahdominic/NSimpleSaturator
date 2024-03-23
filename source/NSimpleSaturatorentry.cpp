//------------------------------------------------------------------------
// Copyright(c) 2024 Noah Dominic Miranda Silvio.
//------------------------------------------------------------------------

#include "NSimpleSaturatorprocessor.h"
#include "NSimpleSaturatorcontroller.h"
#include "NSimpleSaturatorcids.h"
#include "version.h"

#include "public.sdk/source/main/pluginfactory.h"

#define stringPluginName "NSimpleSaturator"

using namespace Steinberg::Vst;
using namespace NoahDominic;

//------------------------------------------------------------------------
//  VST Plug-in Entry
//------------------------------------------------------------------------

BEGIN_FACTORY_DEF ("Noah Dominic Miranda Silvio", 
			       "github.com/noahdominic", 
			       "mailto:noahdmnc@gmail.com")

	//---First Plug-in included in this factory-------
	// its kVstAudioEffectClass component
	DEF_CLASS2 (INLINE_UID_FROM_FUID(kNSimpleSaturatorProcessorUID),
				PClassInfo::kManyInstances,	// cardinality
				kVstAudioEffectClass,	// the component category (do not changed this)
				stringPluginName,		// here the Plug-in name (to be changed)
				Vst::kDistributable,	// means that component and controller could be distributed on different computers
				NSimpleSaturatorVST3Category, // Subcategory for this Plug-in (to be changed)
				FULL_VERSION_STR,		// Plug-in version (to be changed)
				kVstVersionString,		// the VST 3 SDK version (do not changed this, use always this define)
				NSimpleSaturatorProcessor::createInstance)	// function pointer called when this component should be instantiated

	// its kVstComponentControllerClass component
	DEF_CLASS2 (INLINE_UID_FROM_FUID (kNSimpleSaturatorControllerUID),
				PClassInfo::kManyInstances, // cardinality
				kVstComponentControllerClass,// the Controller category (do not changed this)
				stringPluginName "Controller",	// controller name (could be the same than component name)
				0,						// not used here
				"",						// not used here
				FULL_VERSION_STR,		// Plug-in version (to be changed)
				kVstVersionString,		// the VST 3 SDK version (do not changed this, use always this define)
				NSimpleSaturatorController::createInstance)// function pointer called when this component should be instantiated

	//----for others Plug-ins contained in this factory, put like for the first Plug-in different DEF_CLASS2---

END_FACTORY
