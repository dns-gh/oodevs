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

#include "ENT/ENT_Enums_Gen.h"

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

enum E_DayLightingType
{
    eDayLightingJourSansNuage,
    eDayLightingJourPeuNuageux,
    eDayLightingJourMoyennementNuageux,
    eDayLightingJourAssezNuageux,
    eDayLightingJourTresNuageux,

    eNbrDayLightingType
};

enum E_NightLightingType
{
    eNightLightingNuitPleineLune,
    eNightLightingNuitTroisQuartDeLune,
    eNightLightingNuitDemiLune,
    eNightLightingNuitQuartDeLune,
    eNightLightingNuitNouvelleLune,

    eNbrNightLightingType
};

enum E_FuncLimaType
{
    eLimaFuncLD     = 0, // Ligne de débouché
    eLimaFuncLCA    = 1, // Ligne de changement d'attitude
    eLimaFuncLC     = 2, // Ligne de coordination
    eLimaFuncLI     = 3, // Ligne à interdire
    eLimaFuncLO     = 4, // Ligne d'objectif
    eLimaFuncLCAR   = 5, // Ligne de coup d'arrêt
    eLimaFuncLR     = 6, // Ligne de recueil
    eLimaFuncLDM    = 7, // Ligne de début de mission
    eLimaFuncLFM    = 8, // Ligne de fin de mission
    eLimaFuncLIA    = 9, // Ligne d'identification acceuil
    eLimaFuncNbr         // Keep this at the end of the enum! No !
};

}

#endif // __Kernel_Types_h_
