// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __KnowledgeGroupsModel_h_
#define __KnowledgeGroupsModel_h_

#include "tools/Resolver.h"

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class KnowledgeGroup_ABC;
    class Team_ABC;
}

namespace xml
{ 
    class xistream;
    class xostream;
}

class KnowledgeGroupFactory_ABC;
class Model;

// =============================================================================
/** @class  KnowledgeGroupsModel
    @brief  KnowledgeGroupsModel
*/
// Created: AGE 2006-02-15
// =============================================================================
class KnowledgeGroupsModel : public tools::Resolver< kernel::KnowledgeGroup_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit KnowledgeGroupsModel( kernel::Controllers& controllers, KnowledgeGroupFactory_ABC& knowledgeGroupFactory );
    virtual ~KnowledgeGroupsModel();
    //@}

    //! @name Operations
    //@{
    void Purge();
    virtual tools::Iterator< const kernel::KnowledgeGroup_ABC& > CreateIterator() const;
        void Create( kernel::Team_ABC& parent );
        void Create( xml::xistream& xis, kernel::Team_ABC& parent, Model& model );
        void CreateSubKnowledgeGroup( kernel::KnowledgeGroup_ABC& parent );
        void CreateSubKnowledgeGroup( xml::xistream& xis, kernel::KnowledgeGroup_ABC& parent, Model& model );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    KnowledgeGroupsModel( const KnowledgeGroupsModel& );            //!< Copy constructor
    KnowledgeGroupsModel& operator=( const KnowledgeGroupsModel& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    KnowledgeGroupFactory_ABC& knowledgeGroupFactory_;
    //@}
};

#endif // __KnowledgeGroupsModel_h_
