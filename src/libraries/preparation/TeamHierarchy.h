// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TeamHierarchy_h_
#define __TeamHierarchy_h_

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "clients_kernel/ElementObserver_ABC.h"

namespace kernel
{
    class Team_ABC;
    class Controller;
    class Hierarchies;
    class Entity_ABC;
}

// =============================================================================
/** @class  TeamHierarchy
    @brief  TeamHierarchy
*/
// Created: SBO 2006-09-22
// =============================================================================
class TeamHierarchy : public kernel::Extension_ABC
                    , public kernel::Observer_ABC
                    , public kernel::ElementObserver_ABC< kernel::Hierarchies >
{

public:
    //! @name Constructors/Destructor
    //@{
             TeamHierarchy( kernel::Controllers& controllers, const kernel::Team_ABC& team, const kernel::Entity_ABC& holder );
    virtual ~TeamHierarchy();
    //@}

    //! @name Operations
    //@{
    const kernel::Team_ABC& GetTeam() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    TeamHierarchy( const TeamHierarchy& );            //!< Copy constructor
    TeamHierarchy& operator=( const TeamHierarchy& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const kernel::Hierarchies& hierarchy );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::SafePointer< kernel::Team_ABC > team_;
    const kernel::Entity_ABC& holder_;
    //@}
};

#endif // __TeamHierarchy_h_
