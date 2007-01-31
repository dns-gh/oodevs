// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ClientsNetworker_h_
#define __ClientsNetworker_h_

#include "tools/ServerNetworker_ABC.h"

struct ASN1T_MsgsOutMaster;

namespace DIN { class DIN_Input; }

namespace master 
{
class Master;
class Client;

// =============================================================================
/** @class  ClientsNetworker
    @brief  ClientsNetworker
*/
// Created: NLD 2006-09-19
// =============================================================================
class ClientsNetworker : public tools::ServerNetworker_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    ClientsNetworker( Master& master, const std::string& configFile );
    virtual ~ClientsNetworker();
    //@}

    //! @name Main
    //@{
    virtual void Send( const ASN1T_MsgsOutMaster& asnMsg );
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
    virtual void OnConnectionLost    ( DIN::DIN_Server& server, DIN::DIN_Link& link                , const DIN::DIN_ErrorDescription& reason );
    //@}

    //! @name Messages callbacks
    //@{
    void OnReceiveMsgInMaster( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    //@}

    //! @name Tools
    //@{
    virtual void DenyConnections ();
    virtual void AllowConnections();
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< Client* >         T_ClientSet;
    typedef T_ClientSet::const_iterator CIT_ClientSet;
    //@}

private:
    Master&     master_;
    T_ClientSet clients_;
};

}

#endif // __ClientsNetworker_h_
