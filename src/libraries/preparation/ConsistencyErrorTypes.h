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
    eUniquenessMask                  = 0x000f,
    eLongNameUniqueness              = 0x0001,
    eTeamNameUniqueness              = 0x0002,
    eObjectNameUniqueness            = 0x0003,
    eLimaNameUniqueness              = 0x0004,
    eLimitNameUniqueness             = 0x0005,

    eLogisticMask                    = 0x000f << 4,
    eStockInitialization             = 0x0001 << 4,
    eStockMaxExceeded                = 0x0002 << 4,
    eLogisticInitialization          = 0x0003 << 4,
    eNoLogisticBase                  = 0x0004 << 4,

    eProfileMask                     = 0x000f << 8,
    eProfileUniqueness               = 0x0001 << 8,
    eProfileUnreadable               = 0x0002 << 8,
    eProfileUnwritable               = 0x0003 << 8,

    eGhostMask                       = 0x000f << 12,
    eGhostExistence                  = 0x0001 << 12,
    eGhostConverted                  = 0x0002 << 12,

    eCommandPostMask                 = 0x000f << 16,
    eNoCommandPost                   = 0x0001 << 16,
    eSeveralCommandPost              = 0x0002 << 16,

    eOthersMask                      = 0x000f << 20,
    eLongNameSize                    = 0x0001 << 20,
    eUnknownObjectTypes              = 0x0002 << 20,
    eUnknownCrowdTypes               = 0x0003 << 20,
    eUnknownPopulationTypes          = 0x0004 << 20,
    eNoKnowledgeGroup                = 0x0005 << 20,
    eScoreError                      = 0x0006 << 20,
    eSuccessFactorError              = 0x0007 << 20,
    eOthers                          = 0x0008 << 20,
};

#endif // __ConsistencyErrorTypes_h_
