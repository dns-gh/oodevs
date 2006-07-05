//*****************************************************************************
// Created: JVT 02-07-10
// Last Modified: JVT 02-07-11
//*****************************************************************************

#ifndef __Perception_Def_h_
#define __Perception_Def_h_


enum E_PerceptionType
{
	ePerceptionAgent = 0,
    ePerceptionObject,
    ePerceptionRadio
};

enum E_PerceptionResult
{
    eNotSeen        = 0,
    eDetection      = 1,
    eRecognition    = 2,
	eIdentification = 3,
};

#endif // __Perception_Def_h_