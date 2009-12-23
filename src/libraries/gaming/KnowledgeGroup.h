// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __KnowledgeGroup_h_
#define __KnowledgeGroup_h_

#include "clients_kernel/EntityImplementation.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/KnowledgeGroupType.h"
#include "clients_kernel/Updatable_ABC.h"
#include "game_asn/Simulation.h"

// =============================================================================
/** @class  KnowledgeGroup
    @brief  Represents a gtia.
*/
// Created: AGN 2003-12-22
// =============================================================================
class KnowledgeGroup : public kernel::EntityImplementation< kernel::KnowledgeGroup_ABC >
                     , public kernel::Updatable_ABC< ASN1T_MsgKnowledgeGroupUpdate >
{
public:
    //! @name Constructors/Destructor
    //@{
             KnowledgeGroup( unsigned long nId, kernel::Controller& controller, unsigned long nType); //, const tools::Resolver_ABC< kernel::KnowledgeGroupType >& types );
    virtual ~KnowledgeGroup();
    //@}

    //! @name Operation
    //@{
    virtual void Accept( kernel::ModelVisitor_ABC& visitor ) const;
    virtual void Register( kernel::KnowledgeGroup_ABC& knowledgeGroup );
    virtual void Remove( kernel::KnowledgeGroup_ABC& knowledgeGroup );
    virtual void Register( kernel::Automat_ABC& knowledgeGroup );
    virtual void Remove( kernel::Automat_ABC& knowledgeGroup );
    virtual void DoUpdate( const ASN1T_MsgKnowledgeGroupUpdate& message );
    //@}

    //! @name Accessors
    //@{
    virtual bool IsActivated() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    KnowledgeGroup( const KnowledgeGroup& );
    KnowledgeGroup& operator=( const KnowledgeGroup& );
    //@}

    //! @name Member data
    //@{
    bool isActivated_;
    unsigned long nType_;
//    const tools::Resolver_ABC< kernel::KnowledgeGroupType >& types_;
    //@}
};

#endif // __KnowledgeGroup_h_