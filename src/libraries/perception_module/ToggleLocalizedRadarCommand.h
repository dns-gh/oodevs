// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PERCEPTION_TOGGLE_LOCALIZED_RADAR_COMMAND_H
#define SWORD_PERCEPTION_TOGGLE_LOCALIZED_RADAR_COMMAND_H

#include "wrapper/Effect.h"
#include <boost/noncopyable.hpp>

namespace sword
{
namespace wrapper
{
    class View;
}

namespace perception
{
    class ModuleFacade;

// =============================================================================
/** @class  ToggleLocalizedRadarCommand
    @brief  Toggle localized radar command
*/
// Created: SLI 2012-03-22
// =============================================================================
class ToggleLocalizedRadarCommand : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ToggleLocalizedRadarCommand( ModuleFacade& module, const wrapper::View& parameters, const wrapper::View& model, size_t identifier );
    virtual ~ToggleLocalizedRadarCommand();
    //@}

    //! @name Operations
    //@{
    void Execute( const wrapper::View& model ) const;
    void ExecutePaused( const wrapper::View& model ) const;
    //@}

private:
    //! @name Member data
    //@{
    wrapper::Effect effect_;
    //@}
};

}
}
#endif // SWORD_PERCEPTION_TOGGLE_LOCALIZED_RADAR_COMMAND_H
