// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Client_h_
#define __Client_h_

#include "ClientPublisher_ABC.h"
#include "tools/Client_ABC.h"
#include "game_asn/Asn.h"

namespace dispatcher
{

    // =============================================================================
/** @class  Client
    @brief  Client
*/
// Created: NLD 2006-09-19
// =============================================================================
class Client : public tools::Client_ABC
             , public ClientPublisher_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Client( tools::ObjectMessageService& messageService, DIN::DIN_Link& link );
    virtual ~Client();
    //@}

    //! @name Messages
    //@{
    virtual void Send( const ASN1T_MsgsAuthenticationToClient& msg );
    virtual void Send( const ASN1T_MsgsSimToClient& msg );
    virtual void Send( const ASN1T_MsgsReplayToClient& msg );
    //@}

    //! @name Tools
    //@{
    static Client& GetClientFromLink( const DIN::DIN_Link& link );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Client( const Client& );            //!< Copy constructor
    Client& operator=( const Client& ); //!< Assignement operator
    //@}
};

}

#endif // __Client_h_
