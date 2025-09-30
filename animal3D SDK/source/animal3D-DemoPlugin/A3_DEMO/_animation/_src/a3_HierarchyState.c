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

		for (nodeIndex = 0; nodeIndex < state->hierarchy->numNodes; ++nodeIndex)
		{
			//Inverted objectspace, store in objectspaceINV
			a3real4x4TransformInverse(
				state->objectSpaceInv->hpose_base[nodeIndex].transformMat.m,
				state->objectSpace->hpose_base[nodeIndex].transformMat.m
			);
		}
		return (a3i32)nodeIndex;

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

		enum { kMaxSegmentsScratch = 1024 };
		static a3byte   segChildScratch[kMaxSegmentsScratch][64];
		static a3byte   segParentScratch[kMaxSegmentsScratch][64];
		static a3ui32   segScratchCount = 0;
		static a3boolean segCollecting = 0;

		static a3boolean poseGroupIsReady = 0;
		static a3i32 currentFrameIndex = -1;

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

			if (line[0] == '[')
			{
				// --- if we were in [Segments], finalize what we collected BEFORE switching ---
				if (currentSection == Section_Segments)
				{
					a3ui32 nodeCount = (numSegments > 0) ? numSegments : segScratchCount;
					if (nodeCount > segScratchCount) nodeCount = segScratchCount;

					if (nodeCount == 0) { fclose(file); return -1; }

					hierarchy_out->nodes = (a3_HierarchyNode*)calloc(nodeCount, sizeof(a3_HierarchyNode));
					hierarchy_out->numNodes = nodeCount;

					// names
					for (a3ui32 i = 0; i < nodeCount; ++i)
					{
						strncpy((char*)hierarchy_out->nodes[i].name, (const char*)segChildScratch[i],
							sizeof(hierarchy_out->nodes[i].name) - 1);
						hierarchy_out->nodes[i].name[sizeof(hierarchy_out->nodes[i].name) - 1] = '\0';
						hierarchy_out->nodes[i].parentIndex = -1;
					}
					// parents
					for (a3ui32 i = 0; i < nodeCount; ++i)
					{
						const a3byte* parentNameFromFile = segParentScratch[i];
						if (parentNameFromFile[0] == '\0' ||
							equalsIgnoreCase(parentNameFromFile, (const a3byte*)"NONE") ||
							equalsIgnoreCase(parentNameFromFile, (const a3byte*)"GLOBAL") ||
							equalsIgnoreCase(parentNameFromFile, (const a3byte*)"ROOT") ||
							equalsIgnoreCase(parentNameFromFile, (const a3byte*)"-1"))
						{
							hierarchy_out->nodes[i].parentIndex = -1;
							continue;
						}
						a3i32 parentIndex = -1;
						for (a3ui32 k = 0; k < nodeCount; ++k)
							if (equalsIgnoreCase(hierarchy_out->nodes[k].name, parentNameFromFile)) { parentIndex = (a3i32)k; break; }
						hierarchy_out->nodes[i].parentIndex = parentIndex;
					}

					// reset scratch
					segCollecting = 0;
					segScratchCount = 0;
				}

				// --- now switch section based on header we just read ---
				if (caseInsensitivePrefixMatch((const a3byte*)line, (const a3byte*)"[Header]"))              currentSection = Section_Header;
				else if (caseInsensitivePrefixMatch((const a3byte*)line, (const a3byte*)"[Segments]"))       currentSection = Section_Segments;
				else if (caseInsensitivePrefixMatch((const a3byte*)line, (const a3byte*)"[BasePosition]"))   currentSection = Section_BasePosition;
				else if (caseInsensitivePrefixMatch((const a3byte*)line, (const a3byte*)"[FrameData]"))      currentSection = Section_FrameData;
				else currentSection = Section_None;

				continue; // done with header line
			}

			//4) Parse Sections
			switch (currentSection)
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
			{
				if (!segCollecting) { segScratchCount = 0; segCollecting = 1; }

				//removed header since we put that somewhere else :3
				a3byte child[64], parent[64];
				if (parseTwoCSV(line, child, parent, 63) == 2)
				{
					if (segScratchCount < kMaxSegmentsScratch)
					{
						strncpy((char*)segChildScratch[segScratchCount], (const char*)child, 63);
						segChildScratch[segScratchCount][63] = '\0';
						strncpy((char*)segParentScratch[segScratchCount], (const char*)parent, 63);
						segParentScratch[segScratchCount][63] = '\0';
						++segScratchCount;
					}
				}
			}
			break;

			case Section_BasePosition:
			{

				if (!poseGroup_out->hierarchy)
				{
					// tie the pose group to this hierarchy and allocate storage
					if (a3hierarchyPoseGroupCreate(poseGroup_out, hierarchy_out, numFrames) <= 0)
					{
						fclose(file);
						return -1;
					}

					// Use file’s Euler order for all nodes by default
					for (a3ui32 i = 0; i < hierarchy_out->numNodes; ++i)
						poseGroup_out->order[i] = fileEulerOrder;

					if (line[0] == '[')
						break;
				}

				// 1) Pull the segment name
				a3byte segName[64] = { 0 };
				{
					a3i32 gotName = sscanf((const char*)line, " %63[^,]", segName);
					if (gotName != 1)
						break; //failsafe
					trimWhiteSpace(segName);
				}

				// 2) Find which node this name refers to
				a3i32 nodeIndex = -1;
				for (a3ui32 k = 0; k < hierarchy_out->numNodes; ++k)
				{
					if (equalsIgnoreCase(hierarchy_out->nodes[k].name, segName))
					{
						nodeIndex = (a3i32)k;
						break;
					}
				}

				//Failsafe
				if (nodeIndex < 0)
					break;

				// 3) Parse the 9 numbers after the name
				a3real tx = 0, ty = 0, tz = 0;
				a3real rx = 0, ry = 0, rz = 0;  //Calculating in Degrees
				a3real sx = 1, sy = 1, sz = 1;

				{
					const char* comma = strchr((const char*)line, ',');
					if (!comma) break; // no numbers? skip

					// scan 9 reals: tx ty tz rx ry rz sx sy sz
					// allow signs and decimals
					a3i32 gotNums = sscanf(comma + 1,
						" %f , %f , %f , %f , %f , %f , %f , %f , %f",
						&tx, &ty, &tz, &rx, &ry, &rz, &sx, &sy, &sz);

					if (gotNums < 6)
						break;
					if (gotNums < 9)
					{
						// if scale isn't fully provided, use 1s
						if (gotNums < 7) sx = 1.0f;
						if (gotNums < 8) sy = 1.0f;
						if (gotNums < 9) sz = 1.0f;
					}

				}

				// 4) Write into pose index 0 (the base pose) for this node
				a3_SpatialPose* basePoseArray = poseGroup_out->hpose[0].hpose_base;
				a3_SpatialPose* P = basePoseArray + nodeIndex;

				// translate
				P->translate.x = tx;
				P->translate.y = ty;
				P->translate.z = tz;

				// scale
				P->scale.x = sx;
				P->scale.y = sy;
				P->scale.z = sz;

				// rotation
				P->rotate.x = 0.0f;
				P->rotate.y = 0.0f;
				P->rotate.z = 0.0f;
				P->rotate.w = 1.0f;

				//todo : return to this if time permits
				a3spatialPoseConvert(P, /*channel*/ a3poseChannel_none, poseGroup_out->order[nodeIndex]);
			}
			break;

			case Section_FrameData:

				
				if (!poseGroupIsReady)
				{
					// Safety: need hierarchy_out->numNodes > 0 and numFrames > 0
					if (hierarchy_out->numNodes == 0 || numFrames == 0) {
						fclose(file);
						return -1;
					}
					if (a3hierarchyPoseGroupCreate(poseGroup_out, hierarchy_out, numFrames) < 0) {
						fclose(file);
						return -1;
					}
					// Store per node defaults (if applicable, right now I don't think it is?)
					for (a3ui32 n = 0; n < hierarchy_out->numNodes; ++n) {
						poseGroup_out->channel[n] = a3poseChannel_none;
						poseGroup_out->order[n] = fileEulerOrder;
					}
					poseGroupIsReady = 1;
				}

				// 1) New-frame marker? e.g. "Frame 0" or "Frame: 0"
				if (caseInsensitivePrefixMatch((const a3byte*)line, (const a3byte*)"Frame"))
				{
					a3i32 frameScanned = -1;
					if (sscanf((const char*)line, "Frame %d", &frameScanned) == 1 ||
						sscanf((const char*)line, "Frame: %d", &frameScanned) == 1 ||
						sscanf((const char*)line, "Frame=%d", &frameScanned) == 1)
					{
						// Clamp/sanitize
						if (frameScanned < 0) frameScanned = 0;
						if ((a3ui32)frameScanned >= numFrames) frameScanned = (a3i32)(numFrames - 1);
						currentFrameIndex = frameScanned;
					}

					break;
				}

				// 2) Must be a segment row (bone name followed by 9 numbers)
				if (currentFrameIndex >= 0)
				{
					// Expect: name  tx ty tz   rx ry rz   sx sy sz
					a3byte segName[64] = { 0 };
					a3real tx = 0, ty = 0, tz = 0, rx = 0, ry = 0, rz = 0, sx = 1, sy = 1, sz = 1;

					// Try a few tolerant patterns (space/comma separated)
					int parsed =
						sscanf((const char*)line, " %63s %f %f %f %f %f %f %f %f %f",
							segName, &tx, &ty, &tz, &rx, &ry, &rz, &sx, &sy, &sz);
					if (parsed < 7) /* try comma-delimited */
						parsed = sscanf((const char*)line, " %63[^,], %f ,%f ,%f ,%f ,%f ,%f ,%f ,%f ,%f",
							segName, &tx, &ty, &tz, &rx, &ry, &rz, &sx, &sy, &sz);

					if (parsed >= 7) // we at least have T and R; S may default to 1s
					{
						// Find which node this segment name corresponds to
						a3i32 nodeIndex = -1;
						for (a3ui32 k = 0; k < hierarchy_out->numNodes; ++k)
							if (equalsIgnoreCase(hierarchy_out->nodes[k].name, segName)) { nodeIndex = (a3i32)k; break; }

						if (nodeIndex >= 0)
						{
							// Grab the pose for this frame + node
							a3_SpatialPose* poseHere = poseGroup_out->hpose[currentFrameIndex].hpose_base + nodeIndex;

							// Fill translate and scale directly
							poseHere->translate.x = tx;  poseHere->translate.y = ty;  poseHere->translate.z = tz;
							poseHere->scale.x = sx;  poseHere->scale.y = sy;  poseHere->scale.z = sz;

							// Mark that this node has these channels present
							poseGroup_out->channel[nodeIndex] = a3poseChannel_none;

							// rx, ry, rz are degrees from the file
							a3real4x4 T, R, S, TR;
							a3real4x4SetIdentity(T);
							T[3][0] = tx; T[3][1] = ty; T[3][2] = tz;

							a3real4x4SetNonUnif(S, sx, sy, sz);

							// Build r according to the parsed Euler order
							switch (poseGroup_out->order[nodeIndex])
							{
							case a3poseEulerOrder_xyz:
								a3real4x4SetRotateXYZ(R, rx, ry, rz);
								break;

							case a3poseEulerOrder_zyx:
								a3real4x4SetRotateZYX(R, rx, ry, rz);
								break;


							case a3poseEulerOrder_xzy:
							{
								a3real4x4 Rx, Rz, Ry, Rtemp;
								a3real4x4SetRotateX(Rx, rx);
								a3real4x4SetRotateZ(Rz, rz);
								a3real4x4SetRotateY(Ry, ry);
								a3real4x4Product(Rtemp, Rx, Rz);   // Rx * Rz
								a3real4x4Product(R, Rtemp, Ry);    // (Rx * Rz) * Ry
								break;
							}
							case a3poseEulerOrder_yxz:
							{
								a3real4x4 Ry, Rx, Rz, Rtemp;
								a3real4x4SetRotateY(Ry, ry);
								a3real4x4SetRotateX(Rx, rx);
								a3real4x4SetRotateZ(Rz, rz);
								a3real4x4Product(Rtemp, Ry, Rx);
								a3real4x4Product(R, Rtemp, Rz);
								break;
							}
							case a3poseEulerOrder_yzx:
							{
								a3real4x4 Ry, Rz, Rx, Rtemp;
								a3real4x4SetRotateY(Ry, ry);
								a3real4x4SetRotateZ(Rz, rz);
								a3real4x4SetRotateX(Rx, rx);
								a3real4x4Product(Rtemp, Ry, Rz);
								a3real4x4Product(R, Rtemp, Rx);
								break;
							}
							case a3poseEulerOrder_zxy:
							{
								a3real4x4 Rz, Rx, Ry, Rtemp;
								a3real4x4SetRotateZ(Rz, rz);
								a3real4x4SetRotateX(Rx, rx);
								a3real4x4SetRotateY(Ry, ry);
								a3real4x4Product(Rtemp, Rz, Rx);
								a3real4x4Product(R, Rtemp, Ry);
								break;
							}

							default:
								a3real4x4SetRotateXYZ(R, rx, ry, rz);
								break;
							}

							// Compose: M = T * R * S
							a3real4x4Product(TR, T, R);
							a3real4x4Product(poseHere->transformMat.m, TR, S);

							poseHere->rotate.x = 0.0f;
							poseHere->rotate.y = 0.0f;
							poseHere->rotate.z = 0.0f;
							poseHere->rotate.w = 1.0f;

						}
					}
				}
				break;

			default:
				break;
			}
		}

		fclose(file);
		return 1;
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
