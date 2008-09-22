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

#include "ClientPublisher_ABC.h"
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
             NoopClientPublisher() {};
    virtual ~NoopClientPublisher() {};
    //@}

    //! @name Operations
    //@{
    virtual void Send( const ASN1T_MsgsSimToClient&               ) {};
    virtual void Send( const ASN1T_MsgsAuthenticationToClient&    ) {};
    virtual void Send( const ASN1T_MsgsReplayToClient&            ) {};
    virtual void Send( const ASN1T_MsgsAarToClient& ) {};
    virtual void Send( const ASN1T_MsgsMessengerToClient& ) {};
    virtual void Send( const ASN1T_MsgsDispatcherToClient& ) {};
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
             NoopSimulationPublisher() {};
    virtual ~NoopSimulationPublisher() {};
    //@}

    //! @name Operations
    //@{
    virtual void Send( const ASN1T_MsgsClientToSim& ) {};
    virtual void Send( const ASN1T_MsgsDispatcherToSim& ) {};
    //@}
};

}

#endif // __NoopPublisher_h_
