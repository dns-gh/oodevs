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
class TeamHierarchies : public kernel::EntityHierarchies
{

public:
    //! @name Constructors/Destructor
    //@{
             TeamHierarchies( kernel::Controller& controller, const kernel::Team_ABC& holder );
    virtual ~TeamHierarchies();
    //@}

    //! @name Operations
    //@{
    virtual const kernel::Entity_ABC* GetSuperior() const;
    virtual const kernel::Entity_ABC& GetEntity() const;
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
    const kernel::Team_ABC& holder_;
    //@}
};

#endif // __TeamHierarchies_h_
