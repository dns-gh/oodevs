// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ClientNetworker_ABC_h_
#define __ClientNetworker_ABC_h_

#include "ObjectMessageService.h"

#include <string>
#include <memory>

namespace DIN
{
    template< typename T > class DIN_ConnectionServiceClientUserCbk;
    class DIN_ErrorDescription;
}

namespace NEK
{
    class NEK_Address_ABC;
    class NEK_AddressINET;
}

namespace tools
{

// =============================================================================
/** @class  ClientNetworker_ABC
    @brief  ClientNetworker_ABC
*/
// Created: NLD 2006-09-19
// =============================================================================
class ClientNetworker_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ClientNetworker_ABC( unsigned int magicCookie, const std::string& host = "" );
    virtual ~ClientNetworker_ABC();
    //@}

    //! @name Operations
    //@{
    bool Connect    ( const std::string& host );
    bool Disconnect ();
    void Update     ();
    bool IsConnected() const;
    //@}

protected:
    //! @name Connection callbacks
    //@{
    virtual void OnConnected       ( DIN::DIN_Link& link );
    virtual void OnConnectionFailed( DIN::DIN_Link& link, const DIN::DIN_ErrorDescription& reason );
    virtual void OnConnectionLost  ( DIN::DIN_Link& link, const DIN::DIN_ErrorDescription& reason );
    //@}

    //! @name Messages callbacks
    //@{
    virtual bool OnErrorReceivingMessage( DIN::DIN_Link &link, const DIN::DIN_ErrorDescription& info );
    //@}

    //! @name Accessors
    //@{
    ObjectMessageService& GetMessageService() const;
    template< typename C, typename T >
    void RegisterMessage( C& instance, void (C::*callback)( DIN::DIN_Link &link, const T& object ) )
    {
        GetMessageService().RegisterMessage( instance, callback );
    }
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ClientNetworker_ABC( const ClientNetworker_ABC& );            //!< Copy constructor
    ClientNetworker_ABC& operator=( const ClientNetworker_ABC& ); //!< Assignement operator
    //@}

private:
    std::auto_ptr< DIN::DIN_Engine                                                > dinEngine_;
    std::auto_ptr< DIN::DIN_ConnectionServiceClientUserCbk< ClientNetworker_ABC > > connectionService_;
    std::auto_ptr< ObjectMessageService >                                           messageService_;
    std::auto_ptr< NEK::NEK_AddressINET                                           > serverAddress_;
    DIN::DIN_Link*                                                                  session_;
};

}

#endif // __ClientNetworker_ABC_h_
