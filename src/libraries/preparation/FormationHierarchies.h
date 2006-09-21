// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FormationHierarchies_h_
#define __FormationHierarchies_h_

#include "EntityHierarchies.h"
#include "clients_kernel/Serializable_ABC.h"

// =============================================================================
/** @class  FormationHierarchies
    @brief  FormationHierarchies
*/
// Created: SBO 2006-09-21
// =============================================================================
class FormationHierarchies : public EntityHierarchies
                           , public kernel::Serializable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             FormationHierarchies( kernel::Controller& controller, kernel::Entity_ABC& holder, kernel::Entity_ABC* superior );
    virtual ~FormationHierarchies();
    //@}

private:
    //! @name Operations
    //@{
    virtual void DoSerialize( xml::xostream& xos ) const;
    //@}
};

#endif // __FormationHierarchies_h_
