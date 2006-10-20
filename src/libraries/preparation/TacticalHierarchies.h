// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TacticalHierarchies_h_
#define __TacticalHierarchies_h_

#include "clients_kernel/DelayedEntityHierarchies.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Updatable_ABC.h"

namespace kernel
{
    class Controller;
    class Entity_ABC;
    class InstanciationComplete;
}

// =============================================================================
/** @class  TacticalHierarchies
    @brief  Entity hierarchies
*/
// Created: AGE 2006-09-19
// =============================================================================
class TacticalHierarchies : public kernel::DelayedEntityHierarchies< kernel::TacticalHierarchies >
{

public:
    //! @name Constructors/Destructor
    //@{
             TacticalHierarchies( kernel::Controller& controller, kernel::Entity_ABC& holder, kernel::Entity_ABC* superior );
    virtual ~TacticalHierarchies();
    //@}

    //! @name Operations
    //@{
    void ChangeSuperior( kernel::Entity_ABC& superior );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    TacticalHierarchies( const TacticalHierarchies& );            //!< Copy constructor
    TacticalHierarchies& operator=( const TacticalHierarchies& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const kernel::InstanciationComplete& );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    //@}
};

#endif // __TacticalHierarchies_h_
