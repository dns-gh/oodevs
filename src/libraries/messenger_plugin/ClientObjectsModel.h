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

#include <tools/Resolver.h>

namespace dispatcher
{
    class ClientPublisher_ABC;
}

namespace sword
{
    class ClientObjectCreationRequest;
    class ClientObjectDestructionRequest;
    class ClientObjectUpdateRequest;
}

namespace xml
{
    class xistream;
    class xostream;
}

namespace tools
{
    class IdManager;
}

namespace plugins
{
namespace messenger
{
class ClientObject;

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
             ClientObjectsModel( dispatcher::ClientPublisher_ABC& clients, tools::IdManager& idManager );
    virtual ~ClientObjectsModel();
    //@}

    //! @name Operations
    //@{
    void HandleRequest( dispatcher::ClientPublisher_ABC& publisher, const sword::ClientObjectCreationRequest& message );
    void HandleRequest( dispatcher::ClientPublisher_ABC& publisher, const sword::ClientObjectDestructionRequest& message );
    void HandleRequest( dispatcher::ClientPublisher_ABC& publisher, const sword::ClientObjectUpdateRequest& message );
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
    tools::IdManager& idManager_;
    //@}
};

}
}

#endif // __ClientObjectsModel_h_
