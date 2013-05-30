// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationHierarchies_h_
#define __PopulationHierarchies_h_

#include "clients_kernel/SimpleHierarchies.h"
#include "clients_kernel/TacticalHierarchies.h"

// =============================================================================
/** @class  PopulationHierarchies
    @brief  PopulationHierarchies
*/
// Created: AGE 2006-11-24
// =============================================================================
class PopulationHierarchies : public kernel::SimpleHierarchies< kernel::TacticalHierarchies >
{
public:
    //! @name Constructors/Destructor
    //@{
             PopulationHierarchies( const kernel::Entity_ABC& holder, const kernel::Entity_ABC& superior );
    virtual ~PopulationHierarchies();
    //@}
};

#endif // __PopulationHierarchies_h_
