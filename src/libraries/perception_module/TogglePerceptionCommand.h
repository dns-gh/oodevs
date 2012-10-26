// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PERCEPTION_TOGGLE_PERCEPTION_COMMAND_H
#define SWORD_PERCEPTION_TOGGLE_PERCEPTION_COMMAND_H

#include <boost/noncopyable.hpp>
#include <string>

namespace sword
{
namespace wrapper
{
    class View;
}

namespace perception
{
// =============================================================================
/** @class  TogglePerceptionCommand
    @brief  Toggle perception command
*/
// Created: SLI 2012-03-20
// =============================================================================
class TogglePerceptionCommand : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    TogglePerceptionCommand( const wrapper::View& parameters, const wrapper::View& model );
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

#endif // SWORD_PERCEPTION_TOGGLE_PERCEPTION_COMMAND_H
