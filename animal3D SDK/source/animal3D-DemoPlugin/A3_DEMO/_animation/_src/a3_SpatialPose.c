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
	
	a3_SpatialPose.c
	Implementation of spatial pose.
*/

#include "../a3_SpatialPose.h"


//-----------------------------------------------------------------------------

// convert single node pose to matrix
a3i32 a3spatialPoseConvert(a3_SpatialPose* spatialPose, const a3_SpatialPoseChannel channel, const a3_SpatialPoseEulerOrder order)
{
	if (spatialPose)
	{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
//-----------------------------------------------------------------------------
		
		//todo : implement channel and order if time permits

		//shh warnings :3
		(void)channel;
		(void)order;

		//Matrix definition
		a3real4x4 scaleMatrix;
		a3real4x4 rotationMatrix;
		a3real4x4 translationMatrix;
		a3real4x4 TR_temp; 
		a3real4x4 localMatrix;

		//Matrix identity set
		a3real4x4SetIdentity(scaleMatrix);
		a3real4x4SetIdentity(rotationMatrix);
		a3real4x4SetIdentity(translationMatrix);

		//Build Scale Matrix
		a3real4x4SetNonUnif(scaleMatrix,
			spatialPose->scale.x,
			spatialPose->scale.y,
			spatialPose->scale.z);

		//Build Rotation Matrix
		a3quatConvertToMat4(rotationMatrix, spatialPose->rotate.v);

		//Build Translation Matrix
		a3real4x4SetIdentity(translationMatrix);
		translationMatrix[3][0] = spatialPose->translate.x;  // tx
		translationMatrix[3][1] = spatialPose->translate.y;  // ty
		translationMatrix[3][2] = spatialPose->translate.z;  // tz

		//matrix calculation
		a3real4x4Product(TR_temp, translationMatrix, rotationMatrix);
		a3real4x4Product(localMatrix, TR_temp, scaleMatrix);

		//return transformed matrix
		a3real4x4SetReal4x4(spatialPose->transformMat.m, localMatrix);

		return 1; //Success <3



//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
	}
	return -1;
}

// restore single node pose from matrix
a3i32 a3spatialPoseRestore(a3_SpatialPose* spatialPose, const a3_SpatialPoseChannel channel, const a3_SpatialPoseEulerOrder order)
{
	if (spatialPose)
	{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-3: IMPLEMENT ME
//-----------------------------------------------------------------------------
		


//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-3
//-----------------------------------------------------------------------------
	}
	return -1;
}

// copy operation for single node pose
a3i32 a3spatialPoseCopy(a3_SpatialPose* spatialPose_out, const a3_SpatialPose* spatialPose_in)
{
	if (spatialPose_out && spatialPose_in)
	{
		*spatialPose_out = *spatialPose_in;
		return 1;
	}
	return -1;
}

// concat
a3i32 a3spatialPoseConcat(a3_SpatialPose* spatialPose_out, const a3_SpatialPose* spatialPose_lhs, const a3_SpatialPose* spatialPose_rhs)
{
	if (spatialPose_out && spatialPose_lhs && spatialPose_rhs)
	{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
//-----------------------------------------------------------------------------
		
		a3real4Sum
		(
			spatialPose_out->rotate.v,
			spatialPose_lhs->rotate.v,
			spatialPose_rhs->rotate.v
		);

		a3real4Sum
		(
			spatialPose_out->translate.v,
			spatialPose_lhs->translate.v,
			spatialPose_rhs->translate.v
		);

		a3real4ProductComp
		(
			spatialPose_out->scale.v,
			spatialPose_lhs->scale.v,
			spatialPose_rhs->scale.v
		);

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
	}
	return -1;
}

// deconcat
a3i32 a3spatialPoseDeconcat(a3_SpatialPose* spatialPose_out, const a3_SpatialPose* spatialPose_lhs, const a3_SpatialPose* spatialPose_rhs)
{
	if (spatialPose_out && spatialPose_lhs && spatialPose_rhs)
	{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
//-----------------------------------------------------------------------------
		
		a3real4Diff
		(
			spatialPose_out->rotate.v,
			spatialPose_lhs->rotate.v,
			spatialPose_rhs->rotate.v
		);

		a3real4Diff
		(
			spatialPose_out->translate.v,
			spatialPose_lhs->translate.v,
			spatialPose_rhs->translate.v
		);

		a3real4QuotientComp
		(
			spatialPose_out->scale.v,
			spatialPose_lhs->scale.v,
			spatialPose_rhs->scale.v
		);

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
	}
	return -1;
}

// lerp
a3i32 a3spatialPoseLerp(a3_SpatialPose* spatialPose_out, const a3_SpatialPose* spatialPose_0, const a3_SpatialPose* spatialPose_1, const a3real u)
{
	if (spatialPose_out && spatialPose_0 && spatialPose_1)
	{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
//-----------------------------------------------------------------------------
		
		a3quatSlerp
		(
			spatialPose_out->rotate.v,
			spatialPose_0->rotate.v,
			spatialPose_1->rotate.v,
			u
		);

		a3real3Lerp
		(
			spatialPose_out->translate.v,
			spatialPose_0->translate.v,
			spatialPose_1->translate.v,
			u
		);

		a3real3Lerp
		(
			spatialPose_out->scale.v,
			spatialPose_0->scale.v,
			spatialPose_1->scale.v,
			u
		);

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
	}
	return -1;
}


//-----------------------------------------------------------------------------

a3ui8 a3basisAxisIndex(a3_BasisAxis const axis)
{
	return (axis & 0x0F);
}

a3ui8 a3basisAxisSign(a3_BasisAxis const axis)
{
	return (axis & 0xF0) >> 4;
}

a3boolean a3basisAxisValid(a3_BasisAxis const axis)
{
	return (a3basisAxisIndex(axis) < 3) && (a3basisAxisSign(axis) < 2);
}

a3_Basis a3basisInit(a3_BasisAxis const fwd, a3_BasisAxis const up)
{
	if (!a3basisAxisValid(fwd) || !a3basisAxisValid(up))
		return basis_invalid;
	if (a3basisAxisIndex(fwd) == a3basisAxisIndex(up))
		return basis_invalid;
	return ((a3_Basis)(fwd & 0xFF)) | ((a3_Basis)(up & 0xFF) << 8);
}

a3boolean a3basisExtract(a3_BasisAxis* fwd_out, a3_BasisAxis* up_out, a3_Basis const basis)
{
	a3_BasisAxis fwd, up;
	if (!fwd_out || !up_out)
		return false;
	*fwd_out = basis_invalid;
	*up_out = basis_invalid;
	fwd = (basis) & 0xFF;
	up = (basis >> 8) & 0xFF;
	if (a3basisInit(fwd, up) != basis)
		return false;
	*fwd_out = fwd;
	*up_out = up;
	return true;
}

a3real3x3r a3basisToMat3(a3real3x3 m_out, a3_Basis const basis)
{
	a3ui32 i_fwd, i_up;
	a3_BasisAxis fwd, up;
	if (!m_out)
		return 0;
	if (!a3basisExtract(&fwd, &up, basis))
		return 0;
	a3real3x3SetNonUnif(m_out, 0, 0, 0);
	i_fwd = fwd & 0x0F;
	i_up = up & 0x0F;
	m_out[1][(i_fwd) % 3] = (fwd & 0xF0) ? -a3real_one : +a3real_one;
	m_out[2][(i_up) % 3] = (up & 0xF0) ? -a3real_one : +a3real_one;
	a3real3Cross(m_out[0], m_out[1], m_out[2]);
	return m_out;
}

a3real4x4r a3basisToMat4(a3real4x4 m_out, a3_Basis const basis)
{
	a3ui32 i_fwd, i_up;
	a3_BasisAxis fwd, up;
	if (!m_out)
		return 0;
	if (!a3basisExtract(&fwd, &up, basis))
		return 0;
	a3real4x4SetNonUnif(m_out, 0, 0, 0);
	i_fwd = fwd & 0x0F;
	i_up = up & 0x0F;
	m_out[1][(i_fwd) % 3] = (fwd & 0xF0) ? -a3real_one : +a3real_one;
	m_out[2][(i_up) % 3] = (up & 0xF0) ? -a3real_one : +a3real_one;
	a3real3Cross(m_out[0], m_out[1], m_out[2]);
	return m_out;
}


//-----------------------------------------------------------------------------
