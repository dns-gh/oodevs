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
    NoDamage        = 0,
    SlightDamage    = 1,
    ModerateDamage  = 2,
    Destroyed       = 3
};

enum CamouflageEnum32
{
    UniformPaintScheme  = 0,
    DesertCamouflage    = 1,
    WinterCamouflage    = 2,
    ForestCamouflage    = 3,
    GenericCamouflage   = 4
};

enum StanceCodeEnum32
{
    NotApplicable   = 0,
    UprightStandingStill    = 1,
    UprightWalking  = 2,
    UprightRunning  = 3,
    Kneeling    = 4,
    Prone   = 5,
    Crawling    = 6,
    Swimming    = 7,
    Parachuting     = 8,
    Jumping     = 9,
    Sitting     = 10,
    Squatting   = 11,
    Crouching   = 12,
    Wading  = 13,
    Surrender   = 14,
    Detained    = 15
};

};


#endif // __rpr_Enums_h_
