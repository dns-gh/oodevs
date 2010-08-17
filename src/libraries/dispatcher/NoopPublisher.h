// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __NoopPublisher_h_
#define __NoopPublisher_h_

#include "protocol/ClientPublisher_ABC.h"
#include "SimulationPublisher_ABC.h"

namespace dispatcher
{
// =============================================================================
/** @class  NoopClientPublisher
    @brief  NoopClientPublisher
*/
// Created: AGE 2007-08-24
// =============================================================================
class NoopClientPublisher : public ClientPublisher_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             NoopClientPublisher() {}
    virtual ~NoopClientPublisher() {}
    //@}

    //! @name Operations
    //@{
    virtual void Send( const MsgsSimToClient::MsgSimToClient& ) {}
    virtual void Send( const MsgsAuthenticationToClient::MsgAuthenticationToClient& ) {}
    virtual void Send( const MsgsReplayToClient::MsgReplayToClient& ) {}
    virtual void Send( const MsgsAarToClient::MsgAarToClient& ) {}
    virtual void Send( const MsgsMessengerToClient::MsgMessengerToClient& ) {}
    virtual void Send( const MsgsDispatcherToClient::MsgDispatcherToClient& ) {}
    //@}

    //! @name Accesors
    //@{
    virtual std::string GetEndpoint() const { return ""; }
    //@}
};

// =============================================================================
/** @class  NoopSimulationPublisher
    @brief  NoopSimulationPublisher
*/
// Created: AGE 2007-08-24
// =============================================================================
class NoopSimulationPublisher : public SimulationPublisher_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             NoopSimulationPublisher() {}
    virtual ~NoopSimulationPublisher() {}
    //@}

    //! @name Operations
    //@{
    virtual void Send( const MsgsClientToSim::MsgClientToSim& ) {}
    virtual void Send( const MsgsDispatcherToSim::MsgDispatcherToSim& ) {}
    //@}
};

}

#endif // __NoopPublisher_h_
