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
    eOthersMask
};

#endif // __ADN_ConsistencyErrorTypes_h_
