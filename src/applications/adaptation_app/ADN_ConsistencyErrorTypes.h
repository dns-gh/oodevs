// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ADN_ConsistencyErrorTypes_h_
#define __ADN_ConsistencyErrorTypes_h_

enum E_ConsistencyCheck
{
    eMissionTypeUniqueness,
    eObjectTypeUniqueness,
    eUniquenessMask,

    eMissingPart,
    eMissingChoiceComposite,
    eMissingSymbols,
    eMissionAttachmentInvalid,
    eMissingRepairType,
    eInvalidTowCapacity,
    eInvalidMaxMassCarried,
    eInvalidMaxVolumeCarried,
    eInvalidCargoTransportCapacity,
    eInvalidCrowdTransportCapacity,
    eMissingConvoy,
    eMissingConvoyMission,
    eMissingBreakdown,
    eMissingGeometry,
    eMissingDecisionalModel,
    eMissingUnitOnAutomat,
    eMissingPCOnAutomat,
    eMissingArmor,
    eMissingDisaster,
    eMissingObjectParameter,
    eMissingMask,

    eNoCrew,
    eInvalidCrossedRef,
    eInvalidPtrInVector,
    eInvalidIdInVector,
    eDuplicateTranslation,
    eInvalidProbe,
    eNonLogUnitInLogAutomat,
    eLogUnitInNonLogAutomat,
    eBadBreakdownSum,
    eInhabitantSchedule,
    eInvalidSupplyClass,
    eInvalidResourceNature,
    eInvalidFireInjuries,
    eInvalidPoisoningData,
    eBadFloodInjuries,
    eInvalidRoof,
    eInvalidVolume,
    eIndirectFireRange,
    eRepartitionError,
    eDuplicateEnumeration,
    eOthersMask
};

#endif // __ADN_ConsistencyErrorTypes_h_
