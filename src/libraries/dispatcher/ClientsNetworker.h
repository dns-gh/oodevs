// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ClientsNetworker_h_
#define __ClientsNetworker_h_

#include "Publisher_ABC.h"
#include "tools/ServerNetworker_ABC.h"

struct ASN1T_MsgsInClient;

namespace DIN
{
    class DIN_Input;
}

namespace dispatcher 
{
class Dispatcher;
class Replayer;
class Client;
class Config;

// =============================================================================
/** @class  ClientsNetworker
    @brief  ClientsNetworker
*/
// Created: NLD 2006-09-19
// =============================================================================
class ClientsNetworker : public tools::ServerNetworker_ABC
                       , public Publisher_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ClientsNetworker( Dispatcher& dispatcher, const Config& config );
             ClientsNetworker( Replayer&   replayer  , const Config& config );
    virtual ~ClientsNetworker();
    //@}

    //! @name Main
    //@{
    virtual void DenyConnections ();
    virtual void AllowConnections();

    virtual void Send( const ASN1T_MsgsSimToClient&    asnMsg );
    virtual void Send( const ASN1T_MsgsMiddleToClient& asnMsg );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ClientsNetworker( const ClientsNetworker& );            //!< Copy constructor
    ClientsNetworker& operator=( const ClientsNetworker& ); //!< Assignement operator
    //@}

    //! @name Connection callbacks
    //@{
    virtual void OnConnectionReceived( DIN::DIN_Server& server, DIN::DIN_Link& link );
    virtual void OnConnectionLost    ( DIN::DIN_Server& server, DIN::DIN_Link& link, const DIN::DIN_ErrorDescription& reason );
    //@}

    //! @name Messages callbacks
    //@{
    void OnReceiveMsgClientToSim   ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    void OnReceiveMsgClientToMiddle( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< Client* >         T_ClientSet;
    typedef T_ClientSet::const_iterator CIT_ClientSet;
    //@}

private:
    Dispatcher* dispatcher_;
    Replayer*   replayer_;
    T_ClientSet clients_;
};

}

#endif // __ClientsNetworker_h_
