// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
// LTO
// *****************************************************************************

#ifndef __KnowledgeGroupFactory_h_
#define __KnowledgeGroupFactory_h_

#include "clients_kernel/KnowledgeGroupFactory_ABC.h"

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class KnowledgeGroupFactory_ABC;
}

class StaticModel;
class IdManager;

// =============================================================================
/** @class  KnowledgeGroupFactory
    @brief  KnowledgeGroupFactory
*/
// Created: FHD 2009-11-19
// =============================================================================
class KnowledgeGroupFactory : public kernel::KnowledgeGroupFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             KnowledgeGroupFactory( kernel::Controllers& controllers, const StaticModel& staticModel, IdManager& idManager );
    virtual ~KnowledgeGroupFactory();
    //@}

    //! @name Operations
    //@{
    virtual kernel::KnowledgeGroup_ABC* Create( kernel::Team_ABC& team );
    virtual kernel::KnowledgeGroup_ABC* Create( xml::xistream& xis, kernel::Team_ABC& team );
    virtual kernel::KnowledgeGroup_ABC* Create( xml::xistream& xis, kernel::Team_ABC& team, std::string& loadingErrors );

    virtual kernel::KnowledgeGroup_ABC* Create( kernel::KnowledgeGroup_ABC& knowledgeGroup );
    virtual kernel::KnowledgeGroup_ABC* Create( xml::xistream& xis, kernel::KnowledgeGroup_ABC& knowledgeGroup );
    virtual kernel::KnowledgeGroup_ABC* Create( xml::xistream& xis, kernel::KnowledgeGroup_ABC& knowledgeGroup, std::string& loadingErrors );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    KnowledgeGroupFactory( const KnowledgeGroupFactory& );            //!< Copy constructor
    KnowledgeGroupFactory& operator=( const KnowledgeGroupFactory& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    IdManager& idManager_;
    const StaticModel& staticModel_;
    //@}
};

#endif // __KnowledgeGroupFactory_h_
