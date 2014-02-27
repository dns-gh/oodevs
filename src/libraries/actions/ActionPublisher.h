// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ActionPublisher_h_
#define __ActionPublisher_h_

#include "protocol/ServerPublisher_ABC.h"
#include "clients_kernel/ModesObserver_ABC.h"
#include "tools/Observer_ABC.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Controllers;
    class Time_ABC;
}

namespace actions
{

// =============================================================================
/** @class  ActionPublisher
    @brief  Proxy to Publisher_ABC filtering publication in design mode
*/
// Created: SBO 2010-03-17
// =============================================================================
class ActionPublisher : public Publisher_ABC
                      , public tools::Observer_ABC
                      , public kernel::ModesObserver_ABC
                      , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ActionPublisher( Publisher_ABC& publisher,
                              kernel::Controllers& controllers,
                              const kernel::Time_ABC& time );
    virtual ~ActionPublisher();
    //@}

    //! @name Publisher_ABC implementation
    //@{
    virtual void Send( const sword::ClientToSim& message );
    virtual void Send( const sword::ClientToAuthentication& message );
    virtual void Send( const sword::ClientToReplay& message );
    virtual void Send( const sword::ClientToAar& message );
    virtual void Send( const sword::ClientToMessenger& message );
    virtual void Register( T_SimHandler handler );
    virtual void Register( T_ReplayHandler handler );
    //@}

private:
    //! @name ModesObserver_ABC implementation
    //@{
    virtual void NotifyModeChanged( E_Modes newMode );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    Publisher_ABC& publisher_;
    bool design_;
    const kernel::Time_ABC& simulation_;
    //@}
};

} //! namespace actions

#endif // __ActionPublisher_h_
