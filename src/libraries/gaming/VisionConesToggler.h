// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __VisionConesToggler_h_
#define __VisionConesToggler_h_

#include "clients_kernel/OptionsObserver_ABC.h"
#include "tools/ElementObserver_ABC.h"
#include "protocol/ServerPublisher_ABC.h"

namespace kernel
{
    class Controllers;
    class OptionVariant;
}

class Publisher_ABC;
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
             VisionConesToggler( kernel::Controllers& controllers, Publisher_ABC& publisher, QObject* parent );
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
    Publisher_ABC& publisher_;
    bool displayCones_;
    bool displaySurfaces_;
    bool displayFog_;
    bool connected_;
    //@}
};

#endif // __VisionConesToggler_h_
