// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __NetworkDef_h__
#define __NetworkDef_h__

//! @name DIN Messages
//@{
static enum  
{
    eMsgOutSim                                 = 0,
    eMsgInSim                                  = 1,

    eMsgOutClient                              = 2,
    eMsgInClient                               = 3,

    eMsgEnableUnitVisionCones                  = 1000,
    eMsgDisableUnitVisionCones                 = 1001,
    eMsgUnitMagicAction                        = 1002,
    eMsgEnableProfiling                        = 1003,
    eMsgDisableProfiling                       = 1004,
    eMsgUnitVisionCones                        = 1005,
    eMsgTrace                                  = 1006,
    eMsgInit                                   = 1007,
    eMsgProfilingValues                        = 1008,
    eMsgUnitInterVisibility                    = 1009,
    eMsgObjectInterVisibility                  = 1010,
    eMsgPopulationConcentrationInterVisibility = 1011,
    eMsgPopulationFlowInterVisibility          = 1012,
    eMsgKnowledgeGroup                         = 1013,
    eMsgArmy                                   = 1014,
    eMsgDebugDrawPoints                        = 1015,
    eMsgEnvironmentType                        = 1016,
    eMsgPopulationCollision                    = 1017
};
//@}


#endif // __NetworkDef_h__
