// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __dispatcher_Formation_ABC_h_
#define __dispatcher_Formation_ABC_h_

#include "Sendable.h"
#include "clients_kernel/Formation_ABC.h"
#include "tools/Resolver.h"

namespace kernel
{
    class Team_ABC;
}

namespace dispatcher
{
    class Automat_ABC;

// =============================================================================
/** @class  Formation_ABC
    @brief  Formation_ABC
*/
// Created: SBO 2010-06-03
// =============================================================================
class Formation_ABC : public Sendable< kernel::Formation_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Formation_ABC( unsigned long id, const QString& name = "" )
        : Sendable< kernel::Formation_ABC >( id, name ) {}
    virtual ~Formation_ABC() {}
    //@}

    //! @name Accessors
    //@{
    virtual kernel::Formation_ABC* GetParent() const = 0;
    virtual kernel::Team_ABC& GetTeam() const = 0;
    virtual const tools::Resolver< dispatcher::Formation_ABC >& GetFormations() const = 0;
    virtual const tools::Resolver< dispatcher::Automat_ABC >& GetAutomates() const = 0;
    //@}

    //! @name Operations
    //@{
    virtual void Register( dispatcher::Formation_ABC& formation ) = 0;
    virtual void Remove( dispatcher::Formation_ABC& formation ) = 0;
    virtual void Register( dispatcher::Automat_ABC& automat ) = 0;
    virtual void Remove( dispatcher::Automat_ABC& automat ) = 0;
    //@}
};

}

#endif // __dispatcher_Formation_ABC_h_
