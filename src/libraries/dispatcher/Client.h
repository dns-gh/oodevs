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

#include "Publisher_ABC.h"
#include "game_asn/Asn.h"
#include "tools/Client_ABC.h"

namespace DIN { class DIN_Input; }

namespace dispatcher
{
    class Profile;
    class Dispatcher;

// =============================================================================
/** @class  Client
    @brief  Client
*/
// Created: NLD 2006-09-19
// =============================================================================
class Client : public tools::Client_ABC
             , public Publisher_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Client( Dispatcher& dispatcher, DIN::DIN_MessageService_ABC& messageService, DIN::DIN_Link& link );
    virtual ~Client();
    //@}

    //! @name Messages
    //@{
            void OnReceive( const ASN1T_MsgsOutClient& asnMsg );
            void OnReceive( unsigned int nMsgID, DIN::DIN_Input& dinMsg );
    virtual void Send     ( const ASN1T_MsgsInClient& asnMsg );
            void Send     ( const ASN1T_MsgsInClient& asnMsg, const DIN::DIN_BufferedMessage& dinMsg );    
            void Send     ( unsigned int nMsgID, const DIN::DIN_BufferedMessage& dinMsg );
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

    //! @name Rights management
    //@{
    bool CheckRights( const ASN1T_MsgsOutClient& asnMsg ) const;
    bool CheckRights( const ASN1T_MsgsInClient&  asnMsg ) const;
    //@}

    //! @name Messages
    //@{
    void OnReceiveMsgAuthenticationRequest    ( const ASN1T_MsgAuthenticationRequest&     asnMsg );
    void OnReceiveMsgProfileCreationRequest   ( const ASN1T_MsgProfileCreationRequest&    asnMsg );
    void OnReceiveMsgProfileUpdateRequest     ( const ASN1T_MsgProfileUpdateRequest&      asnMsg );
    void OnReceiveMsgProfileDestructionRequest( const ASN1T_MsgProfileDestructionRequest& asnMsg );
    //@}

private:
    Dispatcher& dispatcher_;
    Profile*    pProfile_;
};

}

#endif // __Client_h_
