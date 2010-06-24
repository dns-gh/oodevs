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
    class KnowledgeGroup_ABC;
}

namespace MsgsSimToClient
{
    class MsgKnowledgeGroupCreation;
}

class KnowledgeGroupFactory_ABC;

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
    explicit KnowledgeGroupsModel( KnowledgeGroupFactory_ABC& factory);
    virtual ~KnowledgeGroupsModel();
    //@}

    //! @name Operations
    //@{
    void Create( const MsgsSimToClient::MsgKnowledgeGroupCreation& message );
    void Delete( unsigned int id );
    void Purge();
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
    KnowledgeGroupFactory_ABC& factory_;
    //@}
};

#endif // __KnowledgeGroupsModel_h_
