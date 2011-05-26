// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __KnowledgeGroupFactory_ABC_h_
#define __KnowledgeGroupFactory_ABC_h_

namespace kernel
{
    class KnowledgeGroup_ABC;
    class Team_ABC;
}

namespace xml
{
    class xistream;
}

namespace kernel
{
// =============================================================================
/** @class  KnowledgeGroupFactory_ABC
    @brief  KnowledgeGroupFactory_ABC
*/
// Created: AGE 2006-02-15
// =============================================================================
class KnowledgeGroupFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             KnowledgeGroupFactory_ABC() {};
    virtual ~KnowledgeGroupFactory_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual kernel::KnowledgeGroup_ABC* Create( kernel::Team_ABC& team ) = 0;
    virtual kernel::KnowledgeGroup_ABC* Create( xml::xistream& xis, kernel::Team_ABC& team, std::string& loadingErrors ) = 0;

    virtual kernel::KnowledgeGroup_ABC* Create( kernel::KnowledgeGroup_ABC& knowledgeGroup ) = 0;
    virtual kernel::KnowledgeGroup_ABC* Create( xml::xistream& xis, kernel::KnowledgeGroup_ABC& knowledgeGroup, std::string& loadingErrors ) = 0;

    //@}
};

}

#endif // __KnowledgeGroupFactory_ABC_h_
