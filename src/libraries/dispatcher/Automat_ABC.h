// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __dispatcher_Automat_ABC_h_
#define __dispatcher_Automat_ABC_h_

#include "Sendable.h"
#include "clients_kernel/Automat_ABC.h"
#include "tools/Resolver.h"

namespace kernel
{
    class Formation_ABC;
    class KnowledgeGroup_ABC;
}

namespace dispatcher
{
    class Agent_ABC;
    class Team_ABC;

// =============================================================================
/** @class  Automat_ABC
    @brief  Automat_ABC
*/
// Created: SBO 2010-06-03
// =============================================================================
class Automat_ABC : public Sendable< kernel::Automat_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Automat_ABC( unsigned long id, const QString& name = "" )
        : Sendable< kernel::Automat_ABC >( id, name ) {}
    virtual ~Automat_ABC() {}
    //@}

    //! @name Accessors
    //@{
    virtual bool IsEngaged() const = 0;
    virtual dispatcher::Team_ABC& GetTeam() const = 0;
    virtual kernel::KnowledgeGroup_ABC& GetKnowledgeGroup() const = 0;
    virtual const tools::Resolver< dispatcher::Agent_ABC >& GetAgents() const = 0;
    virtual const tools::Resolver< dispatcher::Automat_ABC >& GetAutomats() const = 0;
    virtual kernel::Automat_ABC* GetParentAutomat() const = 0;
    virtual kernel::Formation_ABC* GetFormation() const = 0;
    //@}

    //! @name Operations
    //@{
    virtual void Register( dispatcher::Automat_ABC& automat ) = 0;
    virtual void Remove( dispatcher::Automat_ABC& automat ) = 0;
    virtual void Register( dispatcher::Agent_ABC& automat ) = 0;
    virtual void Remove( dispatcher::Agent_ABC& automat ) = 0;
    //@}
};

}

#endif // __dispatcher_Automat_ABC_h_
