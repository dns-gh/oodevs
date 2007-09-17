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
#include "game_asn/Asn.h"

namespace tools
{
    class MessageSender_ABC;
}

namespace dispatcher
{

// =============================================================================
/** @class  Client
    @brief  Client
*/
// Created: NLD 2006-09-19
// =============================================================================
class Client : public ClientPublisher_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Client( tools::MessageSender_ABC& sender, const std::string& endpoint );
    virtual ~Client();
    //@}

    //! @name Messages
    //@{
    virtual void Send( const ASN1T_MsgsAuthenticationToClient& msg );
    virtual void Send( const ASN1T_MsgsSimToClient& msg );
    virtual void Send( const ASN1T_MsgsReplayToClient& msg );
    virtual void Send( const ASN1T_MsgsAarToClient& msg );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Client( const Client& );            //!< Copy constructor
    Client& operator=( const Client& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    tools::MessageSender_ABC& sender_;
    std::string endpoint_;
    //@}
};

}

#endif // __Client_h_
