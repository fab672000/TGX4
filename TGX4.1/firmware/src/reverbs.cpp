#include <EEPROM.h>
#include "reverbs.h"
#include "audioComponents.h"
#include "midi_mapping.h"

#define REVEB_MAX_IDX		(2u)		// max number of reverbs
#define REVERB_PITCH_TABLE_SIZE	(9u)
#define REVERB_PITCH_TABLE_SIZE_F ((float32_t)REVERB_PITCH_TABLE_SIZE + 0.499f)

#if defined (REVERBS_USE_TRAILS)
	#define BP_MODE (2)
#else
	#define BP_MODE (1)
#endif

extern AudioEffectReverbSc_F32			reverbSC;
extern AudioEffectPlateReverb_F32		reverbPL;
extern AudioEffectSpringReverb_F32		reverbSP;

static const char* PROGMEM reverb_names[3] = 
{
	"Plate ", "SC    ", "Spring"
};

static const int8_t semitoneTable[REVERB_PITCH_TABLE_SIZE] = {-12, -7, -5, -3, 0, 3, 5, 7, 12};

active_reverb_e activeReverbIdx = REVERB_SPRING;

float32_t master_vol = 1.0f;
const float32_t masterVolMult = 2.5f;
float32_t mix = 1.0f;
bool freeze_mode = false;
bool reverb_bypass = true;
uint8_t bypassMode = BP_MODE;


const char* reverbs_getName()
{
	return reverb_names[(uint8_t)activeReverbIdx];
}

active_reverb_e reverbs_set(active_reverb_e model, bool bypass)
{
	if (model <= REVEB_MAX_IDX)
	{
		activeReverbIdx = (active_reverb_e) model;
		reverb_bypass = bypass;
		reverb_setFreeze(false);
		switch (activeReverbIdx)
		{
			case REVERB_PLATE:
				reverbSC.bypass_setMode(AudioEffectReverbSc_F32::BYPASS_MODE_PASS);
				reverbSC.bypass_set(true);
				reverbSP.bypass_setMode(AudioEffectSpringReverb_F32::BYPASS_MODE_PASS);
				reverbSP.bypass_set(true);
				reverbPL.bypass_setMode((AudioEffectPlateReverb_F32::bypass_mode_t)bypassMode);
				reverbPL.bypass_set(bypass);
				break;
			case REVERB_SC:
				reverbPL.bypass_setMode(AudioEffectPlateReverb_F32::BYPASS_MODE_PASS);
				reverbPL.bypass_set(true);
				reverbSP.bypass_setMode(AudioEffectSpringReverb_F32::BYPASS_MODE_PASS);
				reverbSP.bypass_set(true);
				reverbSC.bypass_setMode((AudioEffectReverbSc_F32::bypass_mode_t)bypassMode);
				reverbSC.bypass_set(bypass);
				break;
			case REVERB_SPRING:
				reverbPL.bypass_setMode(AudioEffectPlateReverb_F32::BYPASS_MODE_PASS);
				reverbPL.bypass_set(true);
				reverbSC.bypass_setMode(AudioEffectReverbSc_F32::BYPASS_MODE_PASS);
				reverbSC.bypass_set(true);
				reverbSP.bypass_setMode((AudioEffectSpringReverb_F32::bypass_mode_t)bypassMode);
				reverbSP.bypass_set(bypass);
				break;
			default:
				break;	
		}	
	}
	return activeReverbIdx;
}

active_reverb_e reverbs_inc(void)
{
	uint8_t idx = activeReverbIdx;
	idx++;
	if (idx > REVEB_MAX_IDX)
		idx = 0;
	activeReverbIdx = (active_reverb_e)idx;

	reverb_setFreeze(false);
	return reverbs_set(activeReverbIdx, reverb_bypass);
}

active_reverb_e reverbs_getActive()
{
	return activeReverbIdx;
}

void reverb_setBypass(bool state)
{
	reverbs_set(activeReverbIdx, state);
}
bool reverb_getBypass()
{
	return reverb_bypass;
}
bool reverb_tglBypass()
{
	reverb_bypass ^= 1;
	reverbs_set(activeReverbIdx, reverb_bypass);
	return reverb_bypass;
}

int8_t reverb_getPitch(float32_t input)
{
	input = constrain(input, 0.0f, 1.0f);
	float32_t idx = map(input, 0.0f, 1.0f, 0.0f, REVERB_PITCH_TABLE_SIZE_F);
	return semitoneTable[(uint8_t)idx];
}


bool reverb_setFreeze(bool state)
{
	bool reply = false;
	switch (activeReverbIdx)
	{
		case REVERB_PLATE:
			reverbPL.freeze(state);
			reply = true;
			break;
		case REVERB_SC:
			reverbSC.freeze(state);
			reply = true;
			break;
		case REVERB_SPRING:
		default:
			break;	
	}
	return reply;
}

bool reverb_tglFreeze()
{
	bool reply = false;
	switch (activeReverbIdx)
	{
		case REVERB_PLATE:
			reply = reverbPL.freeze_tgl();
			break;
		case REVERB_SC:
			reply = reverbSC.freeze_tgl();
			break;
		case REVERB_SPRING:
			reply = true; // will switch to OFF
		default:
			break;	
	}
	return reply;
}

bool reverb_getFreeze()
{
	bool reply = false;
	switch (activeReverbIdx)
	{
		case REVERB_PLATE:
			reply = reverbPL.freeze_get();
			break;
		case REVERB_SC:
			reply = reverbSC.freeze_get();
			break;
		case REVERB_SPRING:
		default:
			break;	
	}
	return reply;	
}
