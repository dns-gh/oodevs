// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __VisionConesToggler_h_
#define __VisionConesToggler_h_

#include "clients_kernel/OptionsObserver_ABC.h"
#include "protocol/ServerPublisher_ABC.h"
#include <tools/ElementObserver_ABC.h>

namespace kernel
{
    class Controllers;
    class OptionVariant;
}

namespace gui
{
    class GLMainProxy;
}

class SimulationController;
class Profile;
class Simulation;

// =============================================================================
/** @class  VisionConesToggler
    @brief  VisionConesToggler
*/
// Created: AGE 2007-07-11
// =============================================================================
class VisionConesToggler : public QObject
                         , public tools::Observer_ABC
                         , public kernel::OptionsObserver_ABC
                         , public tools::ElementObserver_ABC< Profile >
                         , public tools::ElementObserver_ABC< Simulation >
{
public:
    //! @name Constructors/Destructor
    //@{
             VisionConesToggler( kernel::Controllers& controllers,
                                 const SimulationController& simulationController,
                                 const gui::GLMainProxy& mainProxy,
                                 QObject* parent );
    virtual ~VisionConesToggler();
    //@}

protected:
    //! @name Operations
    //@{
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    virtual void NotifyUpdated( const Simulation& );
    virtual void NotifyUpdated( const Profile& );
    //@}

private:
    //! @name Helpers
    //@{
    void SendControlEnableVisionCones();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const SimulationController& simulationController_;
    const gui::GLMainProxy& mainProxy_;
    bool connected_;
    //@}
};

#endif // __VisionConesToggler_h_
