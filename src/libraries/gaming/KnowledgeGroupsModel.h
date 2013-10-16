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

#include <tools/Resolver.h>
#include <boost/noncopyable.hpp>

namespace kernel
{
    class KnowledgeGroup_ABC;
    class Team_ABC;
}

namespace sword
{
    class KnowledgeGroupCreation;
}

class KnowledgeGroupFactory_ABC;

// =============================================================================
/** @class  KnowledgeGroupsModel
    @brief  KnowledgeGroupsModel
*/
// Created: AGE 2006-02-15
// =============================================================================
class KnowledgeGroupsModel : public tools::Resolver< kernel::KnowledgeGroup_ABC >
                           , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit KnowledgeGroupsModel( KnowledgeGroupFactory_ABC& factory);
    virtual ~KnowledgeGroupsModel();
    //@}

    //! @name Operations
    //@{
    void Create( const sword::KnowledgeGroupCreation& message );
    void Delete( unsigned int id );
    void Purge();
    kernel::KnowledgeGroup_ABC* FindCrowdKnowledgeGroup( const kernel::Team_ABC& team ) const;
    //@}

private:
    //! @name Member data
    //@{
    KnowledgeGroupFactory_ABC& factory_;
    //@}
};

#endif // __KnowledgeGroupsModel_h_
