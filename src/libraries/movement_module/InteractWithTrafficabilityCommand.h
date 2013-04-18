// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef SWORD_MOVEMENT_INTERACT_WITH_TRAFFICABILITY_COMMAND_H
#define SWORD_MOVEMENT_INTERACT_WITH_TRAFFICABILITY_COMMAND_H

namespace sword
{
namespace wrapper
{
    class View;
}

namespace movement
{
// =============================================================================
/** @class  InteractWithTrafficabilityCommand
    @brief  InteractWithTrafficabilityCommand
*/
// Created: MMC 2013-04-16
// =============================================================================
class InteractWithTrafficabilityCommand
{
public:
    //! @name Constructors/Destructor
    //@{
    InteractWithTrafficabilityCommand( const wrapper::View& parameters, const wrapper::View& model );
    //@}

    //! @name Operations
    //@{
    void Destroy( const wrapper::View& parameters, const wrapper::View& model ) const;
    void Execute( const wrapper::View& parameters, const wrapper::View& model ) const;
    void ExecutePaused( const wrapper::View& parameters, const wrapper::View& model ) const;
    //@}
};

}
}

#endif // SWORD_MOVEMENT_INTERACT_WITH_TRAFFICABILITY_COMMAND_H
