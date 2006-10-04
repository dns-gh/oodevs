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

#include "TacticalHierarchies.h"
#include "clients_kernel/Serializable_ABC.h"

// =============================================================================
/** @class  TeamHierarchies
    @brief  TeamHierarchies
*/
// Created: SBO 2006-09-21
// =============================================================================
class TeamHierarchies : public TacticalHierarchies
                      , public kernel::Serializable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             TeamHierarchies( kernel::Controller& controller, kernel::Entity_ABC& holder, kernel::Entity_ABC* superior );
    virtual ~TeamHierarchies();
    //@}

private:
    //! @name Operations
    //@{
    virtual void DoSerialize( xml::xostream& xos ) const;
    //@}
};

#endif // __TeamHierarchies_h_
