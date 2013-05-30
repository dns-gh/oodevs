// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectHierarchies_h_
#define __ObjectHierarchies_h_

#include "SimpleHierarchies.h"
#include "TacticalHierarchies.h"

namespace kernel
{

// =============================================================================
/** @class  ObjectHierarchies
    @brief  ObjectHierarchies
*/
// Created: SBO 2006-10-19
// =============================================================================
class ObjectHierarchies : public kernel::SimpleHierarchies< kernel::TacticalHierarchies >
{
public:
    //! @name Constructors/Destructor
    //@{
             ObjectHierarchies( kernel::Entity_ABC& holder, const kernel::Entity_ABC* superior );
    virtual ~ObjectHierarchies();
    //@}
};

}

#endif // __ObjectHierarchies_h_
