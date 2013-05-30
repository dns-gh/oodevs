// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TacticalLineHierarchies_h_
#define __TacticalLineHierarchies_h_

#include "clients_kernel/TacticalLineHierarchies_ABC.h"


// =============================================================================
/** @class  TacticalLineHierarchies
    @brief  TacticalLineHierarchies
*/
// Created: SBO 2006-10-19
// =============================================================================
class TacticalLineHierarchies : public kernel::TacticalLineHierarchies_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             TacticalLineHierarchies( kernel::Entity_ABC& holder, kernel::Entity_ABC* superior );
    virtual ~TacticalLineHierarchies();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Entity_ABC* superior_;
    //@}
};

#endif // __TacticalLineHierarchies_h_
