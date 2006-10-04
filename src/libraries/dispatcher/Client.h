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

#include "AsnTypes.h"

namespace dispatcher
{

class Dispatcher;

// =============================================================================
/** @class  Client
    @brief  Client
*/
// Created: NLD 2006-09-19
// =============================================================================
class Client : public DIN::DIN_UserData_ABC
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
    void OnReceive( unsigned int nMsgID, DIN::DIN_Input& dinMsg ); //$$$ TMP DIN
    void Send     ( const ASN1T_MsgsInClient& asnMsg );
    void Send     ( const ASN1T_MsgsInClient& asnMsg, const DIN::DIN_BufferedMessage& dinMsg );    
    void Send     ( unsigned int nMsgID, const DIN::DIN_BufferedMessage& dinMsg ); //$$$ TMP DIN
    
    DIN::DIN_BufferedMessage GetDinMsg(); //$$$ TMP DIN    
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
    void OnReceiveMsgCtrlClientAnnouncement( const ASN1T_MsgCtrlClientAnnouncement& asnMsg );
    //@}

private:
    Dispatcher&                  dispatcher_;
    DIN::DIN_MessageService_ABC& messageService_;
    DIN::DIN_Link&               link_;
};

}

#include "Client.inl"

#endif // __Client_h_
