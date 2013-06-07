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
    eLogisticMask,

    eProfileUniqueness,
    eProfileUnreadable,
    eProfileUnwritable,
    eProfileNumberTooHigh, // 20
    eProfileNoRole,
    eProfileMask,

    eGhostExistence,
    eGhostConverted,
    eGhostMask,

    eNoCommandPost,
    eSeveralCommandPost,
    eCommandPostMask,

    eLongNameSize,
    eUnknownObjectTypes, // 30
    eUnknownCrowdTypes,
    eUnknownPopulationTypes,
    eNoKnowledgeGroup,
    eScoreError,
    eSuccessFactorError,
    eUnknownInfrastructure,
    eUnknownResourceNetwork,
    eNoOrbat,
    eBadCPExtensions,
    eDiffusionList, // 40
    eDeletedUrbanBlocks,
    eMelmil,
    eDeletedPopulationUrbanBlocks,
    eDiffusionListCleaned,
    eSignature,
    eCityAreaLimitExceeded,
    eUnitOutsideMap,
    eImpossibleObjectCreation,
    eInhabitantBlockRemoved,
    eOthers,
    eOthersMask,
};

#endif // __ConsistencyErrorTypes_h_
