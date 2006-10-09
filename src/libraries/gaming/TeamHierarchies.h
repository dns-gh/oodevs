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

#include "clients_kernel/EntityHierarchies.h"
#include "clients_kernel/CommunicationHierarchies.h"

namespace kernel
{
    class Controller;
    class Team_ABC;
}

// =============================================================================
/** @class  TeamHierarchies
    @brief  TeamHierarchies
*/
// Created: AGE 2006-09-20
// =============================================================================
class TeamHierarchies : public kernel::EntityHierarchies< kernel::CommunicationHierarchies >
{

public:
    //! @name Constructors/Destructor
    //@{
             TeamHierarchies( kernel::Controller& controller, kernel::Team_ABC& holder );
    virtual ~TeamHierarchies();
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
    //@}
};

#endif // __TeamHierarchies_h_
