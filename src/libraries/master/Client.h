// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Client_h_
#define __Client_h_

#include "Publisher_ABC.h"
#include "tools/Client_ABC.h"

struct ASN1T_MsgsOutMaster;
struct ASN1T_MsgsInMaster;
struct ASN1T_MsgAuthenticationRequest;

namespace DIN
{
    class DIN_Input;
}

namespace master
{
    class Profile;
    class Master;

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
             Client( Master& master, tools::ObjectMessageService& messageService, DIN::DIN_Link& link );
    virtual ~Client();
    //@}

    //! @name Messages
    //@{
    void OnReceive( const ASN1T_MsgsInMaster& asnMsg );
    void Send     ( const ASN1T_MsgsOutMaster& asnMsg );
    void Send     ( const ASN1T_MsgsOutMaster& asnMsg, const DIN::DIN_BufferedMessage& dinMsg );    
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
    bool CheckRights( const ASN1T_MsgsOutMaster& asnMsg ) const;
    bool CheckRights( const ASN1T_MsgsInMaster&  asnMsg ) const;
    //@}

    //! @name Messages
    //@{
    void OnReceiveMsgAuthenticationRequest( const ASN1T_MsgAuthenticationRequest& asnMsg );
    //@}

private:
    Master&  master_;
    Profile* pProfile_;
};

}

#endif // __Client_h_
