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
#include "clients_kernel/KnowledgeGroupFactory_ABC.h"

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
    virtual tools::Iterator< const kernel::KnowledgeGroup_ABC& > CreateIterator() const;
    void Create( kernel::Team_ABC& parent ); // LTO
    void Create( xml::xistream& xis, kernel::Team_ABC& parent, Model& model, std::string& loadingErrors );
    void CreateSubKnowledgeGroup( kernel::KnowledgeGroup_ABC& parent ); // LTO
    void CreateSubKnowledgeGroup( xml::xistream& xis, kernel::KnowledgeGroup_ABC& parent, Model& model, std::string& loadingErrors );
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
