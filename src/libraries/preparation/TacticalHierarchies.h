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

#include "clients_gui/EntityHierarchies.h"
#include "clients_kernel/TacticalHierarchies.h"

namespace kernel
{
    class Controller;
    class Entity_ABC;
}

// =============================================================================
/** @class  TacticalHierarchies
    @brief  Entity hierarchies
*/
// Created: AGE 2006-09-19
// =============================================================================
class TacticalHierarchies : public gui::EntityHierarchies< kernel::TacticalHierarchies >
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
    //! @name Copy/Assignment
    //@{
    TacticalHierarchies( const TacticalHierarchies& );            //!< Copy constructor
    TacticalHierarchies& operator=( const TacticalHierarchies& ); //!< Assignment operator
    //@}
};

#endif // __TacticalHierarchies_h_
