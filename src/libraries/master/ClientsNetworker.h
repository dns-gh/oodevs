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

#include "tools/ServerNetworker.h"

struct ASN1T_MsgsOutMaster;
struct ASN1T_MsgsInMaster;

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
class ClientsNetworker : public tools::ServerNetworker
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
    virtual void ConnectionSucceeded( const std::string& endpoint );
    virtual void ConnectionError    ( const std::string& address, const std::string& error );
    //@}

    //! @name Messages callbacks
    //@{
    void OnReceiveMsgInMaster( const std::string& linkFrom, const ASN1T_MsgsInMaster& message );
    //@}

    //! @name Tools
    //@{
    virtual void DenyConnections ();
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, Client* >   T_Clients;
    typedef T_Clients::const_iterator        CIT_Clients;
    //@}

private:
    Master&   master_;
    T_Clients clients_;
};

}

#endif // __ClientsNetworker_h_
