// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __KnowledgeGroupsModel_h_
#define __KnowledgeGroupsModel_h_

#include "clients_kernel/KnowledgeGroupFactory_ABC.h"
#include <tools/Resolver.h>
#include <tools/ElementObserver_ABC.h>

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class KnowledgeGroup_ABC;
    class KnowledgeGroupFactory_ABC;
    class Team_ABC;
}

namespace xml
{
    class xistream;
    class xostream;
}

class Model;

// =============================================================================
/** @class  KnowledgeGroupsModel
    @brief  KnowledgeGroupsModel
*/
// Created: AGE 2006-02-15
// =============================================================================
class KnowledgeGroupsModel : public tools::Resolver< kernel::KnowledgeGroup_ABC >
                           , public tools::Observer_ABC
                           , public tools::ElementObserver_ABC< kernel::KnowledgeGroup_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit KnowledgeGroupsModel( kernel::Controllers& controllers, kernel::KnowledgeGroupFactory_ABC& knowledgeGroupFactory ); // LTO
    virtual ~KnowledgeGroupsModel();
    //@}

    //! @name Operations
    //@{
    void Purge();
    kernel::KnowledgeGroup_ABC* Create( kernel::Team_ABC& parent ); // LTO
    void Create( xml::xistream& xis, kernel::Team_ABC& parent, Model& model );
    void CreateSubKnowledgeGroup( kernel::KnowledgeGroup_ABC& parent ); // LTO
    void CreateSubKnowledgeGroup( xml::xistream& xis, kernel::KnowledgeGroup_ABC& parent, Model& model );
    //@}

private:
    //! @name Operations
    //@{
    virtual void NotifyDeleted( const kernel::KnowledgeGroup_ABC& group );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    KnowledgeGroupsModel( const KnowledgeGroupsModel& );            //!< Copy constructor
    KnowledgeGroupsModel& operator=( const KnowledgeGroupsModel& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_; // LTO
    kernel::KnowledgeGroupFactory_ABC& knowledgeGroupFactory_; // LTO
    //@}
};

#endif // __KnowledgeGroupsModel_h_
