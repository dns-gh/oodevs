// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PROPAGATION_FLOOD_COMMAND_H
#define SWORD_PROPAGATION_FLOOD_COMMAND_H

namespace sword
{
namespace wrapper
{
    class View;
}
namespace propagation
{
// =============================================================================
/** @class  FloodCommand
    @brief  Flood command
*/
// Created: LGY 2012-06-12
// =============================================================================
class FloodCommand
{
public:
    //! @name Constructors/Destructor
    //@{
    FloodCommand( const wrapper::View& parameters, const wrapper::View& model, size_t identifier );
    //@}

public:
    //! @name Operations
    //@{
    void Execute( const wrapper::View& model ) const;
    void ExecutePaused( const wrapper::View& model ) const;
    //@}
};

}
}

#endif // SWORD_PROPAGATION_FLOOD_COMMAND_H
