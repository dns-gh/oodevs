// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TeamTacticalHierarchies_h_
#define __TeamTacticalHierarchies_h_

#include "clients_kernel/EntityHierarchies.h"
#include "clients_kernel/TacticalHierarchies.h"

// =============================================================================
/** @class  TeamTacticalHierarchies
    @brief  TeamTacticalHierarchies
*/
// Created: AGE 2006-10-20
// =============================================================================
class TeamTacticalHierarchies : public kernel::EntityHierarchies< kernel::TacticalHierarchies >
{

public:
    //! @name Constructors/Destructor
    //@{
             TeamTacticalHierarchies( kernel::Controller& controller, kernel::Entity_ABC& entity );
    virtual ~TeamTacticalHierarchies();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TeamTacticalHierarchies( const TeamTacticalHierarchies& );            //!< Copy constructor
    TeamTacticalHierarchies& operator=( const TeamTacticalHierarchies& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    //@}
};

#endif // __TeamTacticalHierarchies_h_
