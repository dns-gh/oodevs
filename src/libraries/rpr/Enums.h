// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __rpr_Enums_h_
#define __rpr_Enums_h_

namespace rpr
{

enum DamageState32
{
    damageState_NoDamage        = 0,
    damageState_SlightDamage    = 1,
    damageState_ModerateDamage  = 2,
    damageState_Destroyed       = 3
};

enum CamouflageEnum32
{
    camouflage_UniformPaintScheme  = 0,
    camouflage_DesertCamouflage    = 1,
    camouflage_WinterCamouflage    = 2,
    camouflage_ForestCamouflage    = 3,
    camouflage_GenericCamouflage   = 4
};

enum StanceCodeEnum32
{
    stance_NotApplicable   = 0,
    stance_UprightStandingStill    = 1,
    stance_UprightWalking  = 2,
    stance_UprightRunning  = 3,
    stance_Kneeling    = 4,
    stance_Prone   = 5,
    stance_Crawling    = 6,
    stance_Swimming    = 7,
    stance_Parachuting     = 8,
    stance_Jumping     = 9,
    stance_Sitting     = 10,
    stance_Squatting   = 11,
    stance_Crouching   = 12,
    stance_Wading  = 13,
    stance_Surrender   = 14,
    stance_Detained    = 15
};

enum BreachedStatusEnum8
{
    breachedStatus_NoBreaching = 0,
    breachedStatus_SlightBreaching     = 1,
    breachedStatus_ModerateBreaching   = 2,
    breachedStatus_Cleared     = 3
};

}

#endif // __rpr_Enums_h_
