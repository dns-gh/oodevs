// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ClientObject_h_
#define __ClientObject_h_

#include <tools/Resolver.h>

namespace dispatcher
{
    class ClientPublisher_ABC;
}

namespace sword
{
    class ClientObjectCreationRequest;
    class ClientObjectUpdateRequest;
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
class ClientObjectProperty_ABC;

// =============================================================================
/** @class  ClientObject
    @brief  ClientObject
*/
// Created: JSR 2010-10-14
// =============================================================================
class ClientObject : public tools::StringResolver< ClientObjectProperty_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             ClientObject( unsigned int id, const sword::ClientObjectCreationRequest& message );
             ClientObject( unsigned int id, xml::xistream& xis );
    virtual ~ClientObject();
    //@}

    //! @name Operations
    //@{
    unsigned int GetID() const;
    void Update( const sword::ClientObjectUpdateRequest& message );
    void SendCreation( dispatcher::ClientPublisher_ABC& publisher ) const;
    void SendUpdate( dispatcher::ClientPublisher_ABC& publisher ) const;
    void SendFullState( dispatcher::ClientPublisher_ABC& publisher ) const;
    void SendDestruction( dispatcher::ClientPublisher_ABC& publisher ) const;
    void Write( xml::xostream& xos ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ClientObject( const ClientObject& );            //!< Copy constructor
    ClientObject& operator=( const ClientObject& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadProperty( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    unsigned int id_;
    std::string name_;
    bool persistent_;
    //@}
};

}
}

#endif // __ClientObject_h_
