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

#include "clients_kernel/EntityHierarchies.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/CommunicationHierarchies.h"

namespace kernel
{
    class Controller;
    class Team_ABC;
    class KnowledgeGroup_ABC;
    class InstanciationComplete;
}

// =============================================================================
/** @class  KnowledgeGroupHierarchies
    @brief  KnowledgeGroupHierarchies
*/
// Created: AGE 2006-09-20
// =============================================================================
class KnowledgeGroupHierarchies : public kernel::EntityHierarchies< kernel::CommunicationHierarchies >
                                , public kernel::Updatable_ABC< kernel::InstanciationComplete >
{

public:
    //! @name Constructors/Destructor
    //@{
             KnowledgeGroupHierarchies( kernel::Controller& controller, kernel::Team_ABC& team, kernel::KnowledgeGroup_ABC& holder );
    virtual ~KnowledgeGroupHierarchies();
    //@}

    //! @name Operations
    //@{
    virtual const kernel::Entity_ABC* GetSuperior() const;
    virtual const kernel::Entity_ABC& GetEntity() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    KnowledgeGroupHierarchies( const KnowledgeGroupHierarchies& );            //!< Copy constructor
    KnowledgeGroupHierarchies& operator=( const KnowledgeGroupHierarchies& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const kernel::InstanciationComplete& );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    kernel::Entity_ABC& superior_;
    kernel::KnowledgeGroup_ABC& holder_;
    //@}
};

#endif // __KnowledgeGroupHierarchies_h_
