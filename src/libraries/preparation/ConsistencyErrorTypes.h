// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ConsistencyErrorTypes_h_
#define __ConsistencyErrorTypes_h_

enum E_ConsistencyCheck
{
    eLongNameUniqueness,
    eTeamNameUniqueness,
    eObjectNameUniqueness,
    eLimaNameUniqueness,
    eLimitNameUniqueness,
    eUniquenessMask,

    eStockInitialization,
    eStockMaxExceeded,
    eStockMissing,
    eLogisticInitialization,
    eNoLogisticBase,
    eNoLogisticFormation,
    eLogisticBaseNotSameTeam,
    eStockInvalidDotation,
    eBadLogisticSubordinate,
    eBadQuotas,
    eLogisticMask,

    eProfileUniqueness,
    eProfileUnreadable,
    eProfileUnwritable,
    eProfileMask,

    eGhostExistence,
    eGhostConverted,
    eGhostMask,

    eNoCommandPost,
    eSeveralCommandPost,
    eCommandPostMask,

    eLongNameSize,
    eUnknownObjectTypes,
    eUnknownCrowdTypes,
    eUnknownPopulationTypes,
    eNoKnowledgeGroup,
    eScoreError,
    eSuccessFactorError,
    eUnknownInfrastructure,
    eUnknownResourceNetwork,
    eNoOrbat,
    eDeletedUrbanBlocks,
    eDeletedPopulationUrbanBlocks,
    eSignature,
    eUnitOutsideMap,
    eEquipmentState,
    eOthers,
    eImpossibleObjectCreation,
    eUnknownMaterial,
    eUnknownRoofShape,
    eUnknownResource,
    eUnknownEquipment,
    eOthersMask,
};

#endif // __ConsistencyErrorTypes_h_
