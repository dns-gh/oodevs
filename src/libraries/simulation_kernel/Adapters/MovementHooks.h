// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_MOVEMENT_HOOKS_H
#define SWORD_MOVEMENT_HOOKS_H

namespace sword
{
// =============================================================================
/** @class  MovementHooks
    @brief  Movement hooks
*/
// Created: SLI 2012-03-13
// =============================================================================
class MovementHooks
{
public:
    //! @name Constructors/Destructor
    //@{
    static void Initialize(); // $$$$ _RC_ SLI 2012-10-19: Only to avoid compiler to remove hooks
    //@}
};

}

#endif // SWORD_MOVEMENT_HOOKS_H
