// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PERCEPTION_TOGGLE_RADAR_COMMAND_H
#define SWORD_PERCEPTION_TOGGLE_RADAR_COMMAND_H

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
/** @class  ToggleRadarCommand
    @brief  Toggle radar command
*/
// Created: SLI 2012-03-22
// =============================================================================
class ToggleRadarCommand : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    ToggleRadarCommand( const wrapper::View& parameters, const wrapper::View& model );
    //@}

    //! @name Operations
    //@{
    void Execute( const wrapper::View& parameters, const wrapper::View& model ) const;
    void ExecutePaused( const wrapper::View& parameters, const wrapper::View& model ) const;
    //@}
};

}
}

#endif // SWORD_PERCEPTION_TOGGLE_RADAR_COMMAND_H
