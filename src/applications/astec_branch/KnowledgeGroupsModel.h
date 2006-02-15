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

#include "Resolver_ABC.h"

class KnowledgeGroup;
class TeamsModel;

// =============================================================================
/** @class  KnowledgeGroupsModel
    @brief  KnowledgeGroupsModel
*/
// Created: AGE 2006-02-15
// =============================================================================
class KnowledgeGroupsModel : public Resolver_ABC< KnowledgeGroup >
{

public:
    //! @name Constructors/Destructor
    //@{
             KnowledgeGroupsModel( TeamsModel& teams );
    virtual ~KnowledgeGroupsModel();
    //@}

    //! @name Operations
    //@{
    virtual KnowledgeGroup* Find( const unsigned long& identifier ) const;
    virtual KnowledgeGroup& Get( const unsigned long& identifier ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    KnowledgeGroupsModel( const KnowledgeGroupsModel& );            //!< Copy constructor
    KnowledgeGroupsModel& operator=( const KnowledgeGroupsModel& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    TeamsModel& teams_;
    //@}
};

#endif // __KnowledgeGroupsModel_h_
