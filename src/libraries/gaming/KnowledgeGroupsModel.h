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

#include "clients_kernel/Resolver_ABC.h"

namespace kernel
{
    class KnowledgeGroup_ABC;
}

class TeamsModel;

// =============================================================================
/** @class  KnowledgeGroupsModel
    @brief  KnowledgeGroupsModel
*/
// Created: AGE 2006-02-15
// =============================================================================
class KnowledgeGroupsModel : public kernel::Resolver_ABC< kernel::KnowledgeGroup_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             KnowledgeGroupsModel( TeamsModel& teams );
    virtual ~KnowledgeGroupsModel();
    //@}

    //! @name Operations
    //@{
    void Purge();

    virtual kernel::KnowledgeGroup_ABC* Find( const unsigned long& identifier ) const;
    virtual kernel::KnowledgeGroup_ABC& Get( const unsigned long& identifier ) const;
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
    TeamsModel& teams_;
    //@}
};

#endif // __KnowledgeGroupsModel_h_
