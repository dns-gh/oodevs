// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PERCEPTION_VISION_OBJECT_H
#define SWORD_PERCEPTION_VISION_OBJECT_H

namespace sword
{
namespace perception
{
// =============================================================================
/** @enum  VisionObject
    @brief  Vision object
*/
// Created: SLI 2012-07-24
// =============================================================================
// environnement visuel statique
enum E_VisionObject // $$$$ _RC_ SLI 2012-07-24: Copied from simulation_kernel/meteo/rawvisiondata/PHY_RawVisionData.h
{
    eVisionEmpty      = 0x0,
    eVisionForest     = 0x1,
    eVisionUrban      = 0x2,
    eVisionGround     = 0x4
};

}
}

#endif // SWORD_PERCEPTION_VISION_OBJECT_H
