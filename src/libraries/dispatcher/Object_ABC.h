// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __dispatcher_Object_ABC_h_
#define __dispatcher_Object_ABC_h_

#include "Sendable.h"
#include "Observable.h"
#include "clients_kernel/Object_ABC.h"

namespace sword
{
    class ObjectUpdate;
}

namespace dispatcher
{
    class Team_ABC;
    class Localisation;
    class ObjectAttributeContainer;

// =============================================================================
/** @class  Object_ABC
    @brief  Object_ABC
*/
// Created: SBO 2010-06-07
// =============================================================================
class Object_ABC
    : public Sendable< kernel::Object_ABC >
    , public Observable< sword::ObjectUpdate >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Object_ABC( unsigned long id, const QString& name = "" )
        : Sendable< kernel::Object_ABC >( id, name ) {}
    virtual ~Object_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual const dispatcher::Team_ABC& GetTeam() const = 0;
    virtual const Localisation& GetLocalisation() const = 0;
    virtual bool GetExtension( const std::string& key, std::string& result ) const = 0;
    virtual const ObjectAttributeContainer& GetAttributes() const = 0;
    //@}
};

}

#endif // __dispatcher_Object_ABC_h_
