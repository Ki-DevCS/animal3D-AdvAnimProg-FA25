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
	
	a3_HierarchyState.c
	Implementation of transform hierarchy state.
*/

#include "../a3_HierarchyState.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


//-----------------------------------------------------------------------------

// initialize pose set given an initialized hierarchy and key pose count
a3i32 a3hierarchyPoseGroupCreate(a3_HierarchyPoseGroup *poseGroup_out, const a3_Hierarchy *hierarchy, const a3ui32 poseCount)
{
	// validate params and initialization states
	//	(output is not yet initialized, hierarchy is initialized)
	if (poseGroup_out && hierarchy && !poseGroup_out->hierarchy && hierarchy->nodes)
	{
		// determine memory requirements
		a3ui32 const nodeCount = hierarchy->numNodes;
		a3ui32 const hposeCount = poseCount, hposeSpace = sizeof(a3_HierarchyPose) * hposeCount;
		a3ui32 const sposeCount = hposeCount * nodeCount, sposeSpace = sizeof(a3_SpatialPose) * sposeCount;
		a3ui32 const channelSpace = sizeof(a3_SpatialPoseChannel) * nodeCount;
		a3ui32 const orderSpace = sizeof(a3_SpatialPoseEulerOrder) * nodeCount;
		a3ui32 const memreq = hposeSpace + sposeSpace + channelSpace + orderSpace;
		a3index i;

		// allocate everything (one malloc)
		poseGroup_out->hpose = (a3_HierarchyPose*)malloc(memreq);
		poseGroup_out->hpose->hpose_base = poseGroup_out->pose = (a3_SpatialPose*)(poseGroup_out->hpose + hposeCount);
		poseGroup_out->channel = (a3_SpatialPoseChannel*)(poseGroup_out->pose + sposeCount);
		poseGroup_out->order = (a3_SpatialPoseEulerOrder*)(poseGroup_out->channel + nodeCount);

		// set pointers
		for (i = 1; i < hposeCount; ++i)
		{
			poseGroup_out->hpose[i].hpose_base = poseGroup_out->hpose[i - 1].hpose_base + nodeCount;
			poseGroup_out->hpose[i].hpose_index = i * nodeCount;
		}

		// reset all data
		a3hierarchyPoseReset(poseGroup_out->hpose, sposeCount);
		memset(poseGroup_out->channel, a3poseChannel_none, channelSpace);
		memset(poseGroup_out->order, a3poseEulerOrder_xyz, orderSpace);
		poseGroup_out->hierarchy = hierarchy;
		poseGroup_out->hposeCount = hposeCount;
		poseGroup_out->poseCount = sposeCount;

		// done
		return 1;
	}
	return -1;
}

// release pose set
a3i32 a3hierarchyPoseGroupRelease(a3_HierarchyPoseGroup *poseGroup)
{
	// validate param exists and is initialized
	if (poseGroup && poseGroup->hierarchy)
	{
		// release everything (one free)
		free(poseGroup->hpose);

		// reset pointers
		poseGroup->hierarchy = 0;
		poseGroup->hpose = 0;
		poseGroup->pose = 0;
		poseGroup->channel = 0;
		poseGroup->order = 0;

		// done
		return 1;
	}
	return -1;
}

// load binary
a3i32 a3hierarchyPoseGroupLoadBinary(a3_HierarchyPoseGroup* poseGroup, a3_FileStream const* fileStream)
{
	FILE* fp;
	a3ui32 ret = 0;
	a3ui32 dataSize = 0;
	if (poseGroup && fileStream)
	{
		if (poseGroup->hierarchy && !poseGroup->pose)
		{
			fp = fileStream->stream;
			if (fp)
			{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-OPTIONAL: IMPLEMENT ME
//-----------------------------------------------------------------------------
				
	//todo : if time permits PoseGroupLoadBinary

//-----------------------------------------------------------------------------
//****END-TO-DO-OPTIONAL
//-----------------------------------------------------------------------------
			}
			return ret;
		}
	}
	return -1;
}

// save binary
a3i32 a3hierarchyPoseGroupSaveBinary(a3_HierarchyPoseGroup const* poseGroup, a3_FileStream const* fileStream)
{
	FILE* fp;
	a3ui32 ret = 0;
	if (poseGroup && fileStream)
	{
		if (poseGroup->hierarchy && poseGroup->pose)
		{
			fp = fileStream->stream;
			if (fp)
			{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-OPTIONAL: IMPLEMENT ME
//-----------------------------------------------------------------------------
				
	//todo : if Time Permits poseGroupSaveBinary

//-----------------------------------------------------------------------------
//****END-TO-DO-OPTIONAL
//-----------------------------------------------------------------------------
			}
			return ret;
		}
	}
	return -1;
}


//-----------------------------------------------------------------------------

// initialize hierarchy state given an initialized hierarchy
a3i32 a3hierarchyStateCreate(a3_HierarchyState *state_out, const a3_Hierarchy *hierarchy)
{
	// validate params and initialization states
	//	(output is not yet initialized, hierarchy is initialized)
	if (state_out && hierarchy && !state_out->hierarchy && hierarchy->nodes)
	{
		// determine memory requirements
		a3ui32 const nodeCount = hierarchy->numNodes;
		a3ui32 const hposeCount = sizeof(state_out->hpose) / sizeof(a3_HierarchyPose);
		a3ui32 const sposeCount = hposeCount * nodeCount;
		a3ui32 const memreq = sizeof(a3_SpatialPose) * sposeCount;
		a3index i;

		// allocate everything (one malloc)
		state_out->hpose->hpose_base = (a3_SpatialPose*)malloc(memreq);

		// set pointers
		state_out->hierarchy = hierarchy;
		for (i = 1; i < hposeCount; ++i)
		{
			state_out->hpose[i].hpose_base = state_out->hpose[i - 1].hpose_base + nodeCount;
			state_out->hpose[i].hpose_index = i * nodeCount;
		}

		// reset all data
		a3hierarchyPoseReset(state_out->hpose, sposeCount);

		// done
		return 1;
	}
	return -1;
}

// release hierarchy state
a3i32 a3hierarchyStateRelease(a3_HierarchyState *state)
{
	// validate param exists and is initialized
	if (state && state->hierarchy)
	{
		a3ui32 const hposeCount = sizeof(state->hpose) / sizeof(a3_HierarchyPose);
		a3ui32 i;

		// release everything (one free)
		free(state->hpose->hpose_base);

		// reset pointers
		state->hierarchy = 0;
		for (i = 0; i < hposeCount; ++i)
		{
			state->hpose[i].hpose_base = 0;
			state->hpose[i].hpose_index = 0;
		}

		// done
		return 1;
	}
	return -1;
}


//-----------------------------------------------------------------------------

// update inverse local-space matrices
a3i32 a3hierarchyStateUpdateLocalInverse(const a3_HierarchyState* state)
{
	if (state && state->hierarchy)
	{
		a3ui32 nodeIndex = 0;
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
//-----------------------------------------------------------------------------
		
		for (nodeIndex = 0; nodeIndex < state->hierarchy->numNodes; ++nodeIndex)
		{
			a3real4x4TransformInverse(
				state->localSpaceInv->hpose_base[nodeIndex].transformMat.m,
				state->localSpace->hpose_base[nodeIndex].transformMat.m
			);
		}

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
		return (a3i32)nodeIndex;
	}
	return -1;
}

// update inverse object-space matrices
a3i32 a3hierarchyStateUpdateObjectInverse(const a3_HierarchyState* state)
{
	if (state && state->hierarchy)
	{
		a3index nodeIndex = 0;
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
//-----------------------------------------------------------------------------

		for(nodeIndex = 0; nodeIndex < state->hierarchy->numNodes; ++nodeIndex)
		{
			a3real4x4TransformInverse
			(
				state->objectSpaceInv->hpose_base[nodeIndex].transformMat.m,
				state->objectSpaceInv->hpose_base[nodeIndex].transformMat.m
			);
		}

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
		return (a3i32)nodeIndex;
	}
	return -1;
}

// update inverse object-space bind-to-current matrices
a3i32 a3hierarchyStateUpdateObjectBindToCurrent(const a3_HierarchyState* state, const a3_HierarchyState* state_bind)
{
	if (state && state->hierarchy && state_bind && state_bind->hierarchy)
	{
		a3ui32 count = 0;
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
//-----------------------------------------------------------------------------
		
		for (count = 0; count < state->hierarchy->numNodes; ++count)
		{
			a3real4x4Product
			(
				state->objectSpaceBindToCurrent->hpose_base[count].transformMat.m,
				state->objectSpace->hpose_base[count].transformMat.m,
				state_bind->objectSpaceInv->hpose_base[count].transformMat.m
			);
		}
//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
		return (a3i32)count;
	}
	return -1;
}


//-----------------------------------------------------------------------------

//Helper Function to Remove spaces, tabs, and newlines from the beginning and end of a string
static inline void trimWhiteSpace(a3byte* text)
{
	//Trim Left
	a3byte* firstVisibleChar = text;

	while
		(
			*firstVisibleChar == ' ' ||
			*firstVisibleChar == '\t' ||
			*firstVisibleChar == '\r' ||
			*firstVisibleChar == '\n'
		)
	{
		++firstVisibleChar;
	}

	if (firstVisibleChar != text)
	{
		memmove(text, firstVisibleChar, strlen(firstVisibleChar) + 1);
	}

	//Trim Right
	size_t length = strlen(text);

	while (length > 0 &&
		(
			text[length - 1] == ' ' ||
			text[length - 1] == '\t' ||
			text[length - 1] == '\r' ||
			text[length - 1] == '\n')
		)
	{
		text[--length] = '\0';
	}

}

//Helper function to identify if the input string has a prefix regardless of capitalisation
static inline a3i32 caseInsensitivePrefixMatch
(
	const a3byte* text,
	const a3byte* prefix
)
{
	while (*prefix && *text)
	{
		a3byte currentCharacter = *text;
		a3byte expectedCharacter = *prefix;

		//force both to lower case
		if (currentCharacter >= 'A' && currentCharacter <= 'Z')
			currentCharacter += 'a' - 'A';

		if (expectedCharacter >= 'A' && expectedCharacter <= 'Z')
			expectedCharacter += 'a' - 'A';

		if (currentCharacter != expectedCharacter)
			return 0; //identify mismatch

		++text;
		++prefix;
	}
	return (*prefix == '\0');
}

//Helper function to parse Euler Order
static a3_SpatialPoseEulerOrder parseEulerOrder(const a3byte* text)
{
	//failstate
	if (!text)
		return a3poseEulerOrder_xyz;

	//Getting the X, Y, and Z variables regardless of case
	a3byte first =
		(text[0] >= 'a' && text[0] <= 'z')
		?
		(text[0] - 'a' + 'A') : text[0];

	a3byte second =
		(text[1] >= 'a' && text[1] <= 'z')
		?
		(text[1] - 'a' + 'A') : text[1];

	a3byte third =
		(text[2] >= 'a' && text[2] <= 'z')
		?
		(text[2] - 'a' + 'A') : text[2];

	//Choosing the Euler Order
	if (first == 'X' && second == 'Y' && third == 'Z')
		return a3poseEulerOrder_xyz; //!

	if (first == 'Z' && second == 'Y' && third == 'X')
		return a3poseEulerOrder_zyx; //!

	if (first == 'Y' && second == 'X' && third == 'Z')
		return a3poseEulerOrder_yxz; //!

	if (first == 'Z' && second == 'X' && third == 'Y')
		return a3poseEulerOrder_zxy; //!

	if (first == 'X' && second == 'Z' && third == 'Y')
		return a3poseEulerOrder_xzy; //!

	if (first == 'Y' && second == 'Z' && third == 'X')
		return a3poseEulerOrder_yzx;

	//Default Fallback
	return a3poseEulerOrder_xyz;
}

//Helper function to compares strings regardless of case
static inline a3i32 equalsIgnoreCase(const a3byte* firstString, const a3byte* secondString)
{
	// Walk through both strings one character at a time
	while (*firstString && *secondString)
	{
		a3byte charA = *firstString;
		a3byte charB = *secondString;

		// Convert uppercase A–Z to lowercase a–z
		if (charA >= 'A' && charA <= 'Z')
			charA += 'a' - 'A';
		if (charB >= 'A' && charB <= 'Z')
			charB += 'a' - 'A';

		// If characters don’t match, strings are not equal
		if (charA != charB)
			return 0;

		// Advance to the next characters
		++firstString;
		++secondString;
	}

	// Both must end at the same time for a full match
	return (*firstString == '\0' && *secondString == '\0');
}

//Helper function to parse a line of text in Field 1 and Field 2
static inline a3i32 parseTwoCSV(const a3byte* line, a3byte* outField1, a3byte* outField2, a3i32 maxLen)
{
	a3i32 fieldsRead = sscanf(
		(const char*)line,
		" %63[^,] , %63[^,\r\n]",
		outField1,
		outField2
	);

	// If both fields were found, clean up any surrounding whitespace
	if (fieldsRead == 2)
	{
		trimWhiteSpace(outField1);
		trimWhiteSpace(outField2);
	}

	// Return the number of successfully parsed fields (0, 1, or 2)
	return fieldsRead;
}

// load HTR file, read and store complete pose group and hierarchy
a3i32 a3hierarchyPoseGroupLoadHTR(a3_HierarchyPoseGroup* poseGroup_out, a3_Hierarchy* hierarchy_out, const a3byte* resourceFilePath)
{
	if (poseGroup_out && !poseGroup_out->hierarchy && hierarchy_out && !hierarchy_out->nodes && resourceFilePath && *resourceFilePath)
	{
		//-----------------------------------------------------------------------------
		//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
		//-----------------------------------------------------------------------------

				//Fail Condition
		if (!(poseGroup_out && !poseGroup_out->hierarchy &&
			hierarchy_out && !hierarchy_out->nodes &&
			resourceFilePath && *resourceFilePath))
			return -1;

		printf("<LoadHTR> All Files Exist\n");

		//File IO
		FILE* file = fopen((const char*)resourceFilePath, "r");
		if (file == NULL)
		{
			// Could not open the file, return error
			return -1;
		}

		printf("<LoadHTR> File IO Successfully opened\n");

		//Sectioning
		enum Section
		{
			Section_None = 0,
			Section_Header,
			Section_Segments,
			Section_BasePosition,
			Section_FrameData
		}
		currentSection = Section_None;

		//Spec Variables
		a3ui32 numSegments = 0;
		a3ui32 numFrames = 1;
		a3real frameRate = 30.0f; //simple default, can be overriden
		a3_SpatialPoseEulerOrder fileEulerOrder = a3poseEulerOrder_xyz; //safe default

		//buffer
		a3byte line[1024];
		//(void)line; //allow build

		while (fgets(line, sizeof(line), file))
		{
			//1) Trim White Space
			trimWhiteSpace(line);

			if (!line[0])
				continue;

			//2) Skip Comments
			if
			(
				line[0] == '#' ||
				line[0] == ';' ||
				line[0] == '/' &&
				line[1] == '/'
			)
				continue;

			//3) Detect Section Headers
			if (line[0] == '[')
			{
				if
				(
					caseInsensitivePrefixMatch
					(
						(const a3byte*)line,
						(const a3byte*)"[Header]"
					)
				)
					currentSection = Section_Header;

				else if
					(
						caseInsensitivePrefixMatch
						(
							(const a3byte*)line,
							(const a3byte*)"[Segments]"
						)
					)
					currentSection = Section_Segments;

				else if
					(
						caseInsensitivePrefixMatch
						(
							(const a3byte*)line,
							(const a3byte*)"[BasePosition]"
						)
					)
					currentSection = Section_BasePosition;

				else if
					(
						caseInsensitivePrefixMatch
						(
							(const a3byte*)line,
							(const a3byte*)"[FrameData]"
						)
					)
					currentSection = Section_FrameData;

				else
					currentSection = Section_None;

				continue;

				//4) Parse Sections
				switch(currentSection)
				{
					case Section_Header:
						if (caseInsensitivePrefixMatch((const a3byte*)line, (const a3byte*)"NumSegments")) 
						{
							sscanf(line, "%*[^0-9]%u", &numSegments);
						}
						else if (caseInsensitivePrefixMatch((const a3byte*)line, (const a3byte*)"NumFrames")) 
						{
							sscanf(line, "%*[^0-9]%u", &numFrames);
						}
						else if (caseInsensitivePrefixMatch((const a3byte*)line, (const a3byte*)"DataFrameRate"))
						{
							sscanf(line, "%*[^0-9.-]%f", &frameRate);
						}
						else if (caseInsensitivePrefixMatch((const a3byte*)line, (const a3byte*)"EulerRotationOrder")) 
						{
							a3byte orderString[8] = { 0 };
							if (sscanf(line, "%*[^A-Za-z]%7s", orderString) == 1)
								fileEulerOrder = parseEulerOrder(orderString);
						}
						break;

					case Section_Segments:
						break;

					case Section_BasePosition:
						break;

					case Section_FrameData:
							break;

					default:
						break;
				}
			}
		}

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
	}
	return -1;
}

// load BVH file, read and store complete pose group and hierarchy
a3i32 a3hierarchyPoseGroupLoadBVH(a3_HierarchyPoseGroup* poseGroup_out, a3_Hierarchy* hierarchy_out, const a3byte* resourceFilePath)
{
	if (poseGroup_out && !poseGroup_out->hierarchy && hierarchy_out && !hierarchy_out->nodes && resourceFilePath && *resourceFilePath)
	{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-OPTIONAL: IMPLEMENT ME
//-----------------------------------------------------------------------------

		//todo : If Time Permits; GroupLoadBPV

//-----------------------------------------------------------------------------
//****END-TO-DO-OPTIONAL
//-----------------------------------------------------------------------------
	}
	return -1;
}

// save HTR file, read and store complete pose group and hierarchy
a3i32 a3hierarchyPoseGroupSaveHTR(const a3_HierarchyPoseGroup* poseGroup_in, const a3_Hierarchy* hierarchy_in, const a3byte* resourceFilePath)
{
	if (poseGroup_in && poseGroup_in->hierarchy && hierarchy_in && hierarchy_in->nodes && resourceFilePath && *resourceFilePath)
	{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-OPTIONAL: IMPLEMENT ME
//-----------------------------------------------------------------------------

			//todo : If Time Permits; GroupLoadHTR

//-----------------------------------------------------------------------------
//****END-TO-DO-OPTIONAL
//-----------------------------------------------------------------------------
	}
	return -1;
}

// save BVH file, read and store complete pose group and hierarchy
a3i32 a3hierarchyPoseGroupSaveBVH(const a3_HierarchyPoseGroup* poseGroup_in, const a3_Hierarchy* hierarchy_in, const a3byte* resourceFilePath)
{
	if (poseGroup_in && poseGroup_in->hierarchy && hierarchy_in && hierarchy_in->nodes && resourceFilePath && *resourceFilePath)
	{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-OPTIONAL: IMPLEMENT ME
//-----------------------------------------------------------------------------

			//todo : If Time Permits; GroupLoadBVH

//-----------------------------------------------------------------------------
//****END-TO-DO-OPTIONAL
//-----------------------------------------------------------------------------
	}
	return -1;
}


//-----------------------------------------------------------------------------
