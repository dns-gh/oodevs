// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PERCEPTION_TOGGLE_ALAT_MONITORING_COMMAND_H
#define SWORD_PERCEPTION_TOGGLE_ALAT_MONITORING_COMMAND_H

#include "wrapper/Effect.h"
#include <boost/noncopyable.hpp>

namespace xml
{
    class xistream;
}

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
/** @class  ToggleAlatMonitoringCommand
    @brief  Toggle Alat monitoring command
*/
// Created: SLI 2012-07-26
// =============================================================================
class ToggleAlatMonitoringCommand : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ToggleAlatMonitoringCommand( ModuleFacade& module, const wrapper::View& parameters, const wrapper::View& model, size_t identifier );
    virtual ~ToggleAlatMonitoringCommand();
    //@}

    //! @name Initialization
    //@{
    static void Initialize( xml::xistream& xis );
    //@}

    //! @name Operations
    //@{
    void Execute( const wrapper::View& model ) const;
    void ExecutePaused( const wrapper::View& model ) const;
    //@}

private:
    //! @name Helpers
    //@{
    struct LoadingWrapper;
    static void ReadAlatTime( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    wrapper::Effect effect_;
    //@}
};

}
}

#endif // SWORD_PERCEPTION_TOGGLE_ALAT_MONITORING_COMMAND_H
