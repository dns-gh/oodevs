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

namespace core
{
    class Facade;
}

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
    static void Initialize( core::Facade& facade );
    //@}
};

}

#endif // SWORD_MOVEMENT_HOOKS_H
