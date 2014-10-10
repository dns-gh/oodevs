// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Kernel_Types_h_
#define __Kernel_Types_h_

#undef max
#undef min
#include <geometry/Types.h>

typedef std::vector< geometry::Point2f > T_PointVector;
typedef T_PointVector::iterator         IT_PointVector;
typedef T_PointVector::const_iterator  CIT_PointVector;

#include "ENT/ENT_Enums.h"

namespace kernel
{
    struct Nothing {};

enum E_TroopHealthState // $$$$ ABR 2011-08-30: Here to preserve old troops display (cf TroopsCompatibilityVersion)
{
    eTroopHealthStateTotal = 0,
    eTroopHealthStateOperational,
    eTroopHealthStateDead,
    eTroopHealthStateWounded,
    eTroopHealthStateMentalWounds,
    eTroopHealthStateContaminated,
    eTroopHealthStateInTreatment,
    eTroopHealthStateUsedForMaintenance,
    eTroopHealthStateWoundedNotEvacuated,
    eTroopHealthStateNbrStates
};

enum E_PerceptionResult
{
    eNotSeen        = 0,
    eDetection      = 1,
    eRecognition    = 2,
    eIdentification = 3,
};

typedef std::vector< geometry::Point2d >               T_TessellatedPoints;
typedef std::pair< unsigned int, T_TessellatedPoints > T_TessellatedData;
typedef std::vector< T_TessellatedData >               T_TessellatedPolygon;
}

#endif // __Kernel_Types_h_
