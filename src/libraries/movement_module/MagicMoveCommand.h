// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_MOVEMENT_MAGIC_MOVE_COMMAND_H
#define SWORD_MOVEMENT_MAGIC_MOVE_COMMAND_H

namespace sword
{
namespace wrapper
{
    class View;
}

namespace movement
{
// =============================================================================
/** @class  MagicMoveCommand
    @brief  MagicMoveCommand
*/
// Created: SLI 2012-10-23
// =============================================================================
class MagicMoveCommand
{
public:
    //! @name Constructors/Destructor
    //@{
    MagicMoveCommand( const wrapper::View& parameters, const wrapper::View& model );
    //@}

    //! @name Operations
    //@{
    void Execute( const wrapper::View& parameters, const wrapper::View& model ) const;
    void ExecutePaused( const wrapper::View& parameters, const wrapper::View& model ) const;
    //@}
};

}
}

#endif // SWORD_MOVEMENT_MAGIC_MOVE_COMMAND_H
