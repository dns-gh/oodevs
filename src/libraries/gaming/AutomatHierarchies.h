// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AutomatHierarchies_h_
#define __AutomatHierarchies_h_

#include "clients_kernel/EntityHierarchies.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Updatable_ABC.h"

namespace sword
{
    class AutomatChangeKnowledgeGroup;
    class AutomatCreation;
}

namespace kernel
{
    class KnowledgeGroup_ABC;
    class PropertiesDictionary;
}

// =============================================================================
/** @class  AutomatHierarchies
    @brief  AutomatHierarchies
*/
// Created: AGE 2006-10-06
// =============================================================================
class AutomatHierarchies : public kernel::EntityHierarchies< kernel::CommunicationHierarchies >
                         , public kernel::Updatable_ABC< sword::AutomatCreation >
                         , public kernel::Updatable_ABC< sword::AutomatChangeKnowledgeGroup >
{
public:
    //! @name Constructors/Destructor
    //@{
             AutomatHierarchies( kernel::Controller& controller, kernel::Entity_ABC& holder,
                                 const tools::Resolver_ABC< kernel::KnowledgeGroup_ABC >& groupResolver,
                                 kernel::PropertiesDictionary& dictionary );
    virtual ~AutomatHierarchies();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AutomatHierarchies( const AutomatHierarchies& );            //!< Copy constructor
    AutomatHierarchies& operator=( const AutomatHierarchies& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual bool CanCommunicate() const;
    virtual bool IsJammed() const;
    virtual void DoUpdate( const sword::AutomatCreation& message );
    virtual void DoUpdate( const sword::AutomatChangeKnowledgeGroup& message );
    virtual void AddSuperiorToDictionary( kernel::PropertiesDictionary& dico, kernel::Entity_ABC* const& superior ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const tools::Resolver_ABC< kernel::KnowledgeGroup_ABC >& groupResolver_;
    QString level_;
    QString symbol_;
    //@}
};

#endif // __AutomatHierarchies_h_
