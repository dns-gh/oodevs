// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InhabitantHierarchies_h_
#define __InhabitantHierarchies_h_

#include "clients_kernel/SimpleHierarchies.h"
#include "clients_kernel/TacticalHierarchies.h"

// =============================================================================
/** @class  InhabitantHierarchies
    @brief  InhabitantHierarchies
*/
// Created: SLG 2010-11-24
// =============================================================================
class InhabitantHierarchies : public kernel::SimpleHierarchies< kernel::TacticalHierarchies >
{
public:
    //! @name Constructors/Destructor
    //@{
             InhabitantHierarchies( kernel::Entity_ABC& holder, kernel::Entity_ABC* superior );
    virtual ~InhabitantHierarchies();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Entity_ABC* superior_;
    //@}
};

#endif // __InhabitantHierarchies_h_
