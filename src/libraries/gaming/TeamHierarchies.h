// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TeamHierarchies_h_
#define __TeamHierarchies_h_

#include "ASN_Types.h"
#include "clients_kernel/EntityHierarchies.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Updatable_ABC.h"

namespace kernel
{
    class Controller;
    class Team_ABC;
}
class KnowledgeGroupFactory_ABC;

// =============================================================================
/** @class  TeamHierarchies
    @brief  TeamHierarchies
*/
// Created: AGE 2006-09-20
// =============================================================================
class TeamHierarchies : public kernel::EntityHierarchies< kernel::CommunicationHierarchies >
                      , public kernel::Updatable_ABC< ASN1T_MsgKnowledgeGroupCreation >
{

public:
    //! @name Constructors/Destructor
    //@{
             TeamHierarchies( kernel::Controller& controller, kernel::Team_ABC& holder, KnowledgeGroupFactory_ABC& factory );
    virtual ~TeamHierarchies();
    //@}

private:
    //! @name Operations
    //@{
    virtual void DoUpdate( const ASN1T_MsgKnowledgeGroupCreation& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TeamHierarchies( const TeamHierarchies& );            //!< Copy constructor
    TeamHierarchies& operator=( const TeamHierarchies& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    kernel::Team_ABC& holder_;
    KnowledgeGroupFactory_ABC& factory_;
    //@}
};

#endif // __TeamHierarchies_h_
