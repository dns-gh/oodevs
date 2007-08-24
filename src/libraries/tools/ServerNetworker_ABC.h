// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ServerNetworker_ABC_h_
#define __ServerNetworker_ABC_h_

#include <set>
#include <memory>
#include "ObjectMessageService.h"

namespace DIN
{
    template< typename T > class DIN_ConnectionServiceServerUserCbk;
    class DIN_Server;
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
/** @class  ServerNetworker_ABC
    @brief  ServerNetworker_ABC
*/
// Created: NLD 2006-09-19
// =============================================================================
class ServerNetworker_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ServerNetworker_ABC( unsigned int magicCookie, unsigned short port );
    virtual ~ServerNetworker_ABC();
    //@}

    //! @name Operations
    //@{
    void Update();
    //@}

protected:
    //! @name Connection callbacks
    //@{
    virtual void OnConnectionReceived( DIN::DIN_Server& server, DIN::DIN_Link& link );
    virtual void OnConnectionLost    ( DIN::DIN_Server& server, DIN::DIN_Link& link                , const DIN::DIN_ErrorDescription& reason );
    virtual void OnConnectionFailed  ( DIN::DIN_Server& server, const NEK::NEK_Address_ABC& address, const DIN::DIN_ErrorDescription& reason );    
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

    //! @name Operations
    //@{
    virtual void DenyConnections ();
    virtual void AllowConnections();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ServerNetworker_ABC( const ServerNetworker_ABC& );            //!< Copy constructor
    ServerNetworker_ABC& operator=( const ServerNetworker_ABC& ); //!< Assignement operator
    //@}

private:
    std::auto_ptr< DIN::DIN_Engine                                                > dinEngine_;
    std::auto_ptr< DIN::DIN_ConnectionServiceServerUserCbk< ServerNetworker_ABC > > connectionService_;
    std::auto_ptr< ObjectMessageService >                                           messageService_;
    std::auto_ptr< NEK::NEK_AddressINET                                           > serverAddress_;
    DIN::DIN_Server*                                                                pServer_;
};

}

#endif // __ServerNetworker_ABC_h_
