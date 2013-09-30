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
#include "clients_gui/EntityHierarchies.h"
#include "clients_kernel/Updatable_ABC.h"
#include "protocol/SimulationSenders.h"

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
class KnowledgeGroupHierarchies : public gui::EntityHierarchies< kernel::CommunicationHierarchies >
                                , public kernel::Updatable_ABC< sword::KnowledgeGroupUpdate >
{
public:
    //! @name Constructors/Destructor
    //@{
             KnowledgeGroupHierarchies( kernel::Controller& controller, kernel::Entity_ABC* superior, kernel::KnowledgeGroup_ABC& holder
                                      , tools::Resolver_ABC< kernel::KnowledgeGroup_ABC >& resolver, bool jam );
    virtual ~KnowledgeGroupHierarchies();
    //@}

    //! @name Operations
    //@{
    virtual bool CanCommunicate() const;
    virtual bool CanReceive() const;
    virtual bool IsJammed() const;
    virtual void DoUpdate( const sword::KnowledgeGroupUpdate& updateMessage );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    KnowledgeGroupHierarchies( const KnowledgeGroupHierarchies& );            //!< Copy constructor
    KnowledgeGroupHierarchies& operator=( const KnowledgeGroupHierarchies& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    tools::Resolver_ABC< kernel::KnowledgeGroup_ABC >& resolver_;
    kernel::Controller& controller_;
    bool jam_;
    //@}
};

#endif // __KnowledgeGroupHierarchies_h_
