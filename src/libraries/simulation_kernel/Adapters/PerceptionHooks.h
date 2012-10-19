// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PERCEPTION_HOOKS_H
#define SWORD_PERCEPTION_HOOKS_H

namespace sword
{
// =============================================================================
/** @class  PerceptionHooks
    @brief  Perception hooks
*/
// Created: SLI 2012-05-22
// =============================================================================
class PerceptionHooks
{
public:
    //! @name Constructors/Destructor
    //@{
    static void Initialize(); // $$$$ _RC_ SLI 2012-10-19: Only to avoid compiler to remove hooks
    //@}
};

}

#endif // SWORD_PERCEPTION_HOOKS_H
