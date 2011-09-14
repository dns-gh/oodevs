// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __AutomatProfileHierarchies_h_
#define __AutomatProfileHierarchies_h_

#include "clients_kernel/EntityHierarchies.h"
#include "ProfileHierarchies_ABC.h"

// =============================================================================
/** @class  ProfileHierarchies
    @brief  Automat profile hierarchies
*/
// Created: LGY 2011-09-13
// =============================================================================
class ProfileHierarchies : public kernel::EntityHierarchies< ProfileHierarchies_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             ProfileHierarchies( kernel::Controller& controller, kernel::Entity_ABC& holder, kernel::Entity_ABC* superior );
    virtual ~ProfileHierarchies();
    //@}
};

#endif // __AutomatProfileHierarchies_h_
