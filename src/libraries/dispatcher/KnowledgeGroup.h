 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __KnowledgeGroup_h_
#define __KnowledgeGroup_h_

#include "game_asn/Simulation.h"
#include "SimpleEntity.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "tools/Resolver.h"

namespace kernel
{
    class Automat_ABC;
    class ModelVisitor_ABC;
    class Team_ABC;
}

namespace dispatcher
{
    class Model;
    class ClientPublisher_ABC;

// =============================================================================
/** @class  KnowledgeGroup
    @brief  KnowledgeGroup
*/
// Created: NLD 2006-09-19
// =============================================================================
class KnowledgeGroup : public SimpleEntity< kernel::KnowledgeGroup_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             KnowledgeGroup( Model& model, const ASN1T_MsgKnowledgeGroupCreation& msg );
    virtual ~KnowledgeGroup();
    //@}

    //! @name Operations
    //@{
    void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    void SendDestruction( ClientPublisher_ABC& publisher ) const;
    void Accept         ( kernel::ModelVisitor_ABC& visitor ) const;
//    void SendChangeParent( ClientPublisher_ABC& publisher ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    KnowledgeGroup( const KnowledgeGroup& );            //!< Copy constructor
    KnowledgeGroup& operator=( const KnowledgeGroup& ); //!< Assignement operator
    //@}

public:
    //! @name Member data
    //@{
    kernel::Team_ABC& team_;
    KnowledgeGroup*      parent_;
    tools::Resolver< KnowledgeGroup > knowledgeGroups_;
    tools::Resolver< kernel::Automat_ABC > automats_;
    //@}
};

}

#endif // __KnowledgeGroup_h_
