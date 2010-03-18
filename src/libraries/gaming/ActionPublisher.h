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

#include "protocol/Publisher_ABC.h"
#include "clients_kernel/OptionsObserver_ABC.h"

// =============================================================================
/** @class  ActionPublisher
    @brief  Proxy to Publisher_ABC filtering publication in design mode
*/
// Created: SBO 2010-03-17
// =============================================================================
class ActionPublisher : public Publisher_ABC
                      , public tools::Observer_ABC
                      , public kernel::OptionsObserver_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionPublisher( Publisher_ABC& publisher, kernel::Controllers& controllers );
    virtual ~ActionPublisher();
    //@}

    //! @name Proxy methods
    //@{
    virtual void Send( const MsgsClientToSim::MsgClientToSim& message );
    virtual void Send( const MsgsClientToAuthentication::MsgClientToAuthentication& message );
    virtual void Send( const MsgsClientToReplay::MsgClientToReplay& message );
    virtual void Send( const MsgsClientToAar::MsgClientToAar& message );
    virtual void Send( const MsgsClientToMessenger::MsgClientToMessenger& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionPublisher( const ActionPublisher& );            //!< Copy constructor
    ActionPublisher& operator=( const ActionPublisher& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    Publisher_ABC& publisher_;
    bool design_;
    //@}
};

#endif // __ActionPublisher_h_
