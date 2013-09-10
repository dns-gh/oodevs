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
    eUniquenessMask         = 0x000f,
    eNNoUniqueness          = 0x0001,
    eEmatUniqueness         = 0x0002,
    eMissionTypeUniqueness  = 0x0004,

    eMissingMask            = 0x000f << 4,
    eMissingNNo             = 0x0001 << 4,
    eMissingEmat            = 0x0002 << 4,
    eMissingPart            = 0x0003 << 4,
    eMissingChoiceComposite = 0x0004 << 4,
    eLowerCaseEmat          = 0x0005 << 4,
    eMissingRepairType      = 0x0006 << 4,

    eOthersMask             = 0x000f << 8,
    eInvalidIdInVector      = 0x0001 << 8,
    eDuplicateSensor        = 0x0002 << 8,
};

#endif // __ADN_ConsistencyErrorTypes_h_
