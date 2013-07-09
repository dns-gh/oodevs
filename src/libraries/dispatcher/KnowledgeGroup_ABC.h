// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __dispatcher_KnowledgeGroup_ABC_h_
#define __dispatcher_KnowledgeGroup_ABC_h_

#include "Sendable.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"

namespace dispatcher
{
    class Automat_ABC;
    class Team_ABC;

// =============================================================================
/** @class  KnowledgeGroup_ABC
    @brief  KnowledgeGroup_ABC
*/
// Created: SBO 2010-06-03
// =============================================================================
class KnowledgeGroup_ABC : public Sendable< kernel::KnowledgeGroup_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit KnowledgeGroup_ABC( unsigned long id, const QString& name = "" )
        : Sendable< kernel::KnowledgeGroup_ABC >( id, name ) {}
    virtual ~KnowledgeGroup_ABC() {}
    //@}

    //! @name Accessors
    //@{
    virtual const dispatcher::Team_ABC& GetTeam() const = 0;
    //@}

    //! @name Operations
    //@{
    virtual void Register( dispatcher::KnowledgeGroup_ABC& knowledgeGroup ) = 0;
    virtual void Remove( dispatcher::KnowledgeGroup_ABC& knowledgeGroup ) = 0;

    virtual void ChangeSuperior( dispatcher::KnowledgeGroup_ABC* superior ) = 0;
    //@}
};

}

#endif // __dispatcher_KnowledgeGroup_ABC_h_
