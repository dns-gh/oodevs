// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_MOVEMENT_FOLLOW_COMMAND_H
#define SWORD_MOVEMENT_FOLLOW_COMMAND_H

namespace sword
{
namespace wrapper
{
    class View;
}

namespace movement
{
// =============================================================================
/** @class  FollowCommand
    @brief  FollowCommand
*/
// Created: SLI 2012-10-23
// =============================================================================
class FollowCommand
{
public:
    //! @name Constructors/Destructor
    //@{
    FollowCommand( const wrapper::View& parameters, const wrapper::View& model );
    //@}

    //! @name Operations
    //@{
    void Execute( const wrapper::View& parameters, const wrapper::View& model ) const;
    void ExecutePaused( const wrapper::View& parameters, const wrapper::View& model ) const;
    //@}
};

}
}

#endif // SWORD_MOVEMENT_FOLLOW_COMMAND_H
