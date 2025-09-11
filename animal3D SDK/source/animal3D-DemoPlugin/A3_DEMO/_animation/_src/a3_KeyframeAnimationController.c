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

#include "A3_DEMO/a3_Scene_Animation.h"

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

//Custom Helper Functions; used to step Keyframe Forward and Backward
static inline void stepKeyframeForward(a3_ClipController* c)
{
	a3i32 idxFirst = c->clip->keyframeIndex_first;  // signed in header
	a3i32 idxFinal = c->clip->keyframeIndex_final;  // signed in header
	a3i32 idx = (a3i32)c->keyframeIndex;       // work in signed

	++idx;
	if (idx > idxFinal)
		idx = idxFirst;

	c->keyframeIndex = (a3ui32)idx;                 // write back as unsigned
	c->keyframe = c->clipPool->keyframe + c->keyframeIndex;
}

static inline void stepKeyframeBackward(a3_ClipController* c)
{
	a3i32 idxFirst = c->clip->keyframeIndex_first;
	a3i32 idxFinal = c->clip->keyframeIndex_final;
	a3i32 idx = (a3i32)c->keyframeIndex;

	--idx;
	if (idx < idxFirst)
		idx = idxFinal;

	c->keyframeIndex = (a3ui32)idx;
	c->keyframe = c->clipPool->keyframe + c->keyframeIndex;
}


//Custom Helper Function; used to apply the transition
static inline void ApplyTransition(
	a3_ClipController* c,
	a3_ClipTransition const* tr,
	animal_DoubleVar overstep_sec,
	animal_IntVar    overstep_step)
{
	if (!tr) return;

	// direction changes?
	if (tr->flag & a3clip_reverseFlag)
		c->playback_step = -c->playback_step;

	// clip change?
	if (tr->flag & a3clip_clipFlag) {
		a3i32 target = tr->clipIndex;
		if (target >= 0 && target < (a3i32)c->clipPool->clipCount)
			a3clipControllerSetClip(c, c->clipPool, (a3ui32)target, c->playback_step, c->playback_stepPerSec);
	}

	// snap to start or end (we’re about to add overstep/offset)
	if (tr->flag & a3clip_terminusFlag) {
		c->clipTime_sec = c->clip->duration_sec;
		c->clipTime_step = c->clip->duration_step;
	}
	else {
		c->clipTime_sec = 0.0;
		c->clipTime_step = 0;
	}

	// optional offset (in steps)
	if (tr->flag & a3clip_offsetFlag) {
		c->clipTime_step += tr->offset;
		if (c->playback_secPerStep > 0.0)
			c->clipTime_sec += (animal_DoubleVar)tr->offset * c->playback_secPerStep;
	}

	// keep leftover time?
	if (tr->flag & a3clip_overstepFlag) {
		c->clipTime_sec += overstep_sec;
		c->clipTime_step += overstep_step;
	}

	// skip boundary (nudge inside)
	if (tr->flag & a3clip_skipFlag) {
		animal_DoubleVar nudge = (c->playback_secPerStep > 0.0)
			? c->playback_secPerStep * 0.5 : 1.0 / 120.0;
		c->clipTime_sec += (c->playback_step >= 0 ? +nudge : -nudge);
	}

	// wrap and refresh pointers
	c->clipTime_sec = wrapPositive_Double(c->clipTime_sec, c->clip->duration_sec);
	c->clipTime_step = wrapPositive_Int(c->clipTime_step, c->clip->duration_step);
	a3clipControllerRefresh(c, c->clipPool);
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

	// must have controller + pool + clip (keyframe may be null on first tick)
		if (!(clipCtrl && clipCtrl->clipPool && clipCtrl->clip))
			return -1;

		// initialize keyframe pointer on first tick if needed
		if (!clipCtrl->keyframe) {
			clipCtrl->keyframeIndex = (a3ui32)clipCtrl->clip->keyframeIndex_first;
			clipCtrl->keyframe = clipCtrl->clipPool->keyframe + clipCtrl->keyframeIndex;
		}

		// if no time advanced, nothing to do
		if (dt <= (animal_DoubleVar)0.0)
			return 0;

		//DEBUG/SAFETY: ensure playback has sane defaults so time actually advances
		if (clipCtrl->playback_step == 0)                         // paused? force forward for now
			clipCtrl->playback_step = +1;
		if (!(clipCtrl->playback_stepPerSec > (animal_DoubleVar)0.0)) {
			clipCtrl->playback_stepPerSec = (animal_DoubleVar)1.0; // 1 step/sec
			clipCtrl->playback_secPerStep = (animal_DoubleVar)1.0; // reciprocal (avoid div-by-zero)
		}

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

		if (clipCtrl->playback_step >= 0) {
			if (clipCtrl->clipTime_sec >= clipDurSec) {
				// leftover past the end
				animal_DoubleVar over_sec = clipCtrl->clipTime_sec - clipDurSec;
				animal_IntVar    over_stp = 0;
				if (clipCtrl->playback_secPerStep > (animal_DoubleVar)0.0 && clipDurStp > 0)
					over_stp = wrapPositive_Int(clipCtrl->clipTime_step - clipDurStp, clipDurStp);

				ApplyTransition(clipCtrl, clipCtrl->clip->transitionForward, over_sec, over_stp);
			}
		}
		else {
			if (clipCtrl->clipTime_sec < (animal_DoubleVar)0.0) {
				// negative leftover before start
				animal_DoubleVar over_sec = clipCtrl->clipTime_sec;
				animal_IntVar    over_stp = 0;
				ApplyTransition(clipCtrl, clipCtrl->clip->transitionReverse, over_sec, over_stp);
			}
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
				if (clipCtrl->playback_step >= 0) stepKeyframeForward(clipCtrl);
				else                               stepKeyframeBackward(clipCtrl);
				continue;
			}

			// 1) Forward Step
			if (clipCtrl->keyframeTime_sec >= kfDurSec)
			{
				clipCtrl->keyframeTime_sec -= kfDurSec;

				if (clipCtrl->playback_secPerStep > (animal_DoubleVar)0.0 && kfDurStp > 0)
					clipCtrl->keyframeTime_step = wrapPositive_Int(
						clipCtrl->keyframeTime_step - kfDurStp, kfDurStp);

				stepKeyframeForward(clipCtrl);
				continue;
			}


			// 2) Reverse step
			if (clipCtrl->keyframeTime_sec < (animal_DoubleVar)0.0)
			{
				stepKeyframeBackward(clipCtrl);

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
			a3clipControllerRefresh(clipCtrl, clipCtrl->clipPool);


			return 0;

			//-----------------------------------------------------------------------------
			//****END-TO-DO-PROJECT-1
			//-----------------------------------------------------------------------------
		}
		return -1;
}


//-----------------------------------------------------------------------------
