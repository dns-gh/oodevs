// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __KnowledgeGroupHierarchies_h_
#define __KnowledgeGroupHierarchies_h_

#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/EntityHierarchies.h"
#include "clients_kernel/Updatable_ABC.h"
#include "game_asn/Simulation.h"

namespace kernel
{
    class Entity_ABC;
    class KnowledgeGroup_ABC;
}

// =============================================================================
/** @class  KnowledgeGroupHierarchies
    @brief  KnowledgeGroupHierarchies
*/
// Created: AGE 2006-09-20
// =============================================================================
class KnowledgeGroupHierarchies : public kernel::EntityHierarchies< kernel::CommunicationHierarchies >
                                , public kernel::Updatable_ABC< ASN1T_MsgKnowledgeGroupUpdate >
{

public:
    //! @name Constructors/Destructor
    //@{
             KnowledgeGroupHierarchies( kernel::Controller& controller, kernel::Entity_ABC* superior, kernel::KnowledgeGroup_ABC& holder
                                      , tools::Resolver_ABC< kernel::KnowledgeGroup_ABC >& resolver );
    virtual ~KnowledgeGroupHierarchies();
    //@}

    //! @name Operations 
    //@{
    virtual void DoUpdate( const ASN1T_MsgKnowledgeGroupUpdate& updateMessage );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    KnowledgeGroupHierarchies( const KnowledgeGroupHierarchies& );            //!< Copy constructor
    KnowledgeGroupHierarchies& operator=( const KnowledgeGroupHierarchies& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    tools::Resolver_ABC< kernel::KnowledgeGroup_ABC >& resolver_;
    //@}
};

#endif // __KnowledgeGroupHierarchies_h_
