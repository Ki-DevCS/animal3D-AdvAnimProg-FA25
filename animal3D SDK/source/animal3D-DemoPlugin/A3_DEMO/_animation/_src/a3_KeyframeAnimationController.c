/*
	Copyright 2011-2025 Daniel S. Buckstein

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

/*
	animal3D SDK: Minimal 3D Animation Framework
	By Daniel S. Buckstein
	
	a3_KeyframeAnimationController.c
	Implementation of keyframe animation controller.
*/

#include "../a3_KeyframeAnimationController.h"

#include <string.h>

//! Type Definitions so I can figure out exactly what I am working with;
typedef a3f64  animal_DoubleVar;
typedef a3i32  animal_IntVar;
typedef a3ui32 animal_UIntVar;
typedef a3byte animal_ByteVar;


#include <math.h>

//Custom Helper Functions; used to wrap a time value into the range [0, duration)
static inline animal_DoubleVar wrapPositive_Double(animal_DoubleVar time, animal_DoubleVar duration)
{
	animal_DoubleVar remainder = fmod(time, duration);
	if (remainder < (animal_DoubleVar)0.0) remainder += duration;
	return remainder;
}

static inline animal_IntVar wrapPositive_Int(animal_IntVar time, animal_IntVar duration)
{
	if (duration <= 0)
		return 0; // avoid divide-by-zero or nonsense wrapping

	animal_IntVar remainder = time % duration;
	if (remainder < 0)
		remainder += duration;
	return remainder;
}




// macros to help with names
#define A3_CLIPCTRL_DEFAULTNAME		("unnamed clip ctrl")
#define A3_CLIPCTRL_SEARCHNAME		((ctrlName && *ctrlName) ? ctrlName : A3_CLIPCTRL_DEFAULTNAME)


//-----------------------------------------------------------------------------

// initialize clip controller
a3i32 a3clipControllerInit(a3_ClipController* clipCtrl_out, const a3byte ctrlName[a3keyframeAnimation_nameLenMax], const a3_ClipPool* clipPool, const a3ui32 clipIndex_pool, const a3i32 playback_step, const a3f64 playback_stepPerSec)
{
	a3i32 const ret = a3clipControllerSetClip(clipCtrl_out, clipPool, clipIndex_pool, playback_step, playback_stepPerSec);
	if (ret >= 0)
	{
		strncpy(clipCtrl_out->name, ctrlName, a3keyframeAnimation_nameLenMax);
		return ret;
	}
	return -1;
}

// update clip controller
a3i32 a3clipControllerUpdate(a3_ClipController* clipCtrl, a3f64 dt)
{
	if (clipCtrl && clipCtrl->clipPool)
	{
		//-----------------------------------------------------------------------------
		//****TO-DO-ANIM-PROJECT-1: IMPLEMENT ME
		//-----------------------------------------------------------------------------

				//?check that controller; clip; and keyframe exist. If not; error
		if (!(clipCtrl && clipCtrl->clipPool && clipCtrl->clip && clipCtrl->keyframe))
			return -1;

		//?check if DeltaTime is 0 or negative. If true; then the clip shouldn't progress
		if (dt <= (animal_DoubleVar)0.0)
			return 0;

		const animal_DoubleVar timeStepSec =
			dt * clipCtrl->playback_stepPerSec * (animal_DoubleVar)clipCtrl->playback_step;

		clipCtrl->keyframeTime_sec += timeStepSec;   // (1) increment keyframe time (sec)
		clipCtrl->clipTime_sec += timeStepSec;   // (2) increment clip time (sec)

		//Keep Step integer in sync
		if (clipCtrl->playback_secPerStep > (animal_DoubleVar)0.0)
		{
			const animal_DoubleVar asStepsF = timeStepSec / clipCtrl->playback_secPerStep;
			const animal_IntVar    asStepsI = (animal_IntVar)(
				asStepsF >= (animal_DoubleVar)0.0 ? floor(asStepsF) : ceil(asStepsF));
			clipCtrl->keyframeTime_step += asStepsI;
			clipCtrl->clipTime_step += asStepsI;
		}

		//Implement looping behaviour (wrap to [0, duration)
		const animal_DoubleVar clipDurSec = clipCtrl->clip->duration_sec;
		const animal_IntVar    clipDurStp = clipCtrl->clip->duration_step;

		//force stop for bad duration
		if (!(clipDurSec > (animal_DoubleVar)0.0) || !(clipDurStp > 0))
		{
			clipCtrl->clipTime_sec = 0.0;
			clipCtrl->clipTime_step = 0;
			clipCtrl->keyframeTime_sec = 0.0;
			clipCtrl->keyframeTime_step = 0;
			clipCtrl->clipParam = 0.0;
			clipCtrl->keyframeParam = 0.0;
			return 0;
		}

		//Wrap Functions
		clipCtrl->clipTime_sec = wrapPositive_Double(clipCtrl->clipTime_sec, clipDurSec);
		clipCtrl->clipTime_step = wrapPositive_Int(clipCtrl->clipTime_step, clipDurStp);

		//Run Loop
		for (;;)
		{
			const animal_DoubleVar kfDurSec = clipCtrl->keyframe->duration_sec;
			const animal_IntVar    kfDurStp = (animal_IntVar)clipCtrl->keyframe->duration_step;

			// 0) Check once in playback; repeat
			if (!(kfDurSec > (animal_DoubleVar)0.0))
			{
				if (clipCtrl->playback_step >= 0)
				{
					++clipCtrl->keyframeIndex;
					if (clipCtrl->keyframeIndex > clipCtrl->clip->keyframeIndex_final)
						clipCtrl->keyframeIndex = clipCtrl->clip->keyframeIndex_first;
				}
				else
				{
					--clipCtrl->keyframeIndex;
					if (clipCtrl->keyframeIndex < clipCtrl->clip->keyframeIndex_first)
						clipCtrl->keyframeIndex = clipCtrl->clip->keyframeIndex_final;
				}
				clipCtrl->keyframe = clipCtrl->clipPool->keyframe + clipCtrl->keyframeIndex;
				continue;
			}

			// 1) Forward Step
			if (clipCtrl->keyframeTime_sec >= kfDurSec)
			{
				// consume this keyframe's duration
				clipCtrl->keyframeTime_sec -= kfDurSec;

				// keep integer "step" domain coherent
				if (clipCtrl->playback_secPerStep > (animal_DoubleVar)0.0 && kfDurStp > 0)
					clipCtrl->keyframeTime_step = wrapPositive_Int(
						clipCtrl->keyframeTime_step - kfDurStp, kfDurStp);

				// advance to next keyframe (looping)
				++clipCtrl->keyframeIndex;
				if (clipCtrl->keyframeIndex > clipCtrl->clip->keyframeIndex_final)
					clipCtrl->keyframeIndex = clipCtrl->clip->keyframeIndex_first;

				clipCtrl->keyframe = clipCtrl->clipPool->keyframe + clipCtrl->keyframeIndex;
				continue; // re-check with new keyframe
			}

			// 2) Reverse step
			if (clipCtrl->keyframeTime_sec < (animal_DoubleVar)0.0)
			{
				// step back (looping)
				--clipCtrl->keyframeIndex;
				if (clipCtrl->keyframeIndex < clipCtrl->clip->keyframeIndex_first)
					clipCtrl->keyframeIndex = clipCtrl->clip->keyframeIndex_final;
				clipCtrl->keyframe = clipCtrl->clipPool->keyframe + clipCtrl->keyframeIndex;

				// add the (new) current keyframe's duration to bring time into range
				const animal_DoubleVar newDurSec = clipCtrl->keyframe->duration_sec;
				const animal_IntVar    newDurStp = (animal_IntVar)clipCtrl->keyframe->duration_step;

				clipCtrl->keyframeTime_sec += newDurSec;

				if (clipCtrl->playback_secPerStep > (animal_DoubleVar)0.0 && newDurStp > 0)
					clipCtrl->keyframeTime_step = wrapPositive_Int(
						clipCtrl->keyframeTime_step + newDurStp, newDurStp);

				continue;
			}

			// 3) In-range - done resolving
			break;
		}

			//Clip Control Params
			clipCtrl->clipParam = clipCtrl->clipTime_sec / clipDurSec;
			clipCtrl->keyframeParam = clipCtrl->keyframeTime_sec / clipCtrl->keyframe->duration_sec;

		

		// clipCtrl->clipParam = fmod(clipCtrl->clipParam + 0.25, 1.0);
			return 0;

			//-----------------------------------------------------------------------------
			//****END-TO-DO-PROJECT-1
			//-----------------------------------------------------------------------------
		}
		return -1;
}


//-----------------------------------------------------------------------------
