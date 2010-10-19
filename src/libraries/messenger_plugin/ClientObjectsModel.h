// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ClientObjectsModel_h_
#define __ClientObjectsModel_h_

#include "tools/resolver.h"

namespace dispatcher
{
    class ClientPublisher_ABC;
}

namespace MsgsClientToMessenger
{
    class MsgClientObjectCreationRequest;
    class MsgClientObjectDestructionRequest;
    class MsgClientObjectUpdateRequest;
}

namespace xml
{
    class xistream;
    class xostream;
}

namespace plugins
{
namespace messenger
{
class ClientObject;
class IdManager;

// =============================================================================
/** @class  ClientObjectsModel
    @brief  ClientObjectsModel
*/
// Created: JSR 2010-10-14
// =============================================================================
class ClientObjectsModel : public tools::Resolver< ClientObject >
{
public:
    //! @name Constructors/Destructor
    //@{
             ClientObjectsModel( dispatcher::ClientPublisher_ABC& clients, IdManager& idManager );
    virtual ~ClientObjectsModel();
    //@}

    //! @name Operations
    //@{
    void HandleRequest( dispatcher::ClientPublisher_ABC& publisher, const MsgsClientToMessenger::MsgClientObjectCreationRequest& message );
    void HandleRequest( dispatcher::ClientPublisher_ABC& publisher, const MsgsClientToMessenger::MsgClientObjectDestructionRequest& message );
    void HandleRequest( dispatcher::ClientPublisher_ABC& publisher, const MsgsClientToMessenger::MsgClientObjectUpdateRequest& message );
    void SendStateToNewClient( dispatcher::ClientPublisher_ABC& publisher ) const;
    void Write( xml::xostream& xos ) const;
    void ReadClientObject( xml::xistream& xis );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ClientObjectsModel( const ClientObjectsModel& );            //!< Copy constructor
    ClientObjectsModel& operator=( const ClientObjectsModel& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::ClientPublisher_ABC& clients_;
    IdManager& idManager_;
    //@}
};

}
}

#endif // __ClientObjectsModel_h_
