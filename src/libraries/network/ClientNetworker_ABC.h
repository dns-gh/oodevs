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

#include <string>
#include <memory>

namespace DIN
{
    class DIN_MessageService_ABC;
    template< typename T > class DIN_MessageServiceUserCbk;
    template< typename T > class DIN_ConnectionServiceClientUserCbk;
    class DIN_Link;
    class DIN_Engine;
    class DIN_ErrorDescription;
}

namespace NEK
{
    class NEK_Address_ABC;
    class NEK_AddressINET;
}

namespace network
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
    explicit ClientNetworker_ABC( const std::string& host );
    virtual ~ClientNetworker_ABC();
    //@}

    //! @name Operations
    //@{
    void Update();
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
    DIN::DIN_MessageServiceUserCbk< ClientNetworker_ABC >& GetMessageService() const;
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
    std::auto_ptr< DIN::DIN_MessageServiceUserCbk         < ClientNetworker_ABC > > messageService_;
    std::auto_ptr< NEK::NEK_AddressINET                                           > serverAddress_;
};

}

#endif // __ClientNetworker_ABC_h_
