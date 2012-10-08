// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __dispatcher_Team_ABC_h_
#define __dispatcher_Team_ABC_h_

#include "Sendable.h"
#include "clients_kernel/Team_ABC.h"

namespace kernel
{
    class Karma;
}

namespace dispatcher
{
    class Formation_ABC;
    class Inhabitant_ABC;
    class KnowledgeGroup_ABC;
    class Object_ABC;
    class Population_ABC;

// =============================================================================
/** @class  Team_ABC
    @brief  Team_ABC
*/
// Created: SBO 2010-06-03
// =============================================================================
class Team_ABC : public Sendable< kernel::Team_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Team_ABC( unsigned long id, const QString& name = "" )
        : Sendable< kernel::Team_ABC >( id, name )
    {}
    virtual ~Team_ABC()
    {}
    //@}

    //! @name Accessors
    //@{
    virtual const kernel::Karma& GetKarma() const = 0;
    virtual bool GetExtension( const std::string& key, std::string& result ) const = 0;
    //@}

    //! @name Operations
    //@{
    virtual void Register( dispatcher::Formation_ABC& formation ) = 0;
    virtual void Remove( dispatcher::Formation_ABC& formation ) = 0;
    virtual void Register( dispatcher::Population_ABC& population ) = 0;
    virtual void Remove( dispatcher::Population_ABC& population ) = 0;
    virtual void Register( dispatcher::Inhabitant_ABC& inhabitant ) = 0;
    virtual void Remove( dispatcher::Inhabitant_ABC& inhabitant ) = 0;
    virtual void Register( dispatcher::Object_ABC& object ) = 0;
    virtual void Remove( dispatcher::Object_ABC& object ) = 0;
    virtual void Register( dispatcher::KnowledgeGroup_ABC& knGroup ) = 0;
    virtual void Remove( dispatcher::KnowledgeGroup_ABC& knGroup ) = 0;
    //@}
};

}

#endif // __Team_ABC_h_
