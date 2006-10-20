// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FormationHierarchy_h_
#define __FormationHierarchy_h_

#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/DelayedEntityHierarchies.h"
#include "clients_kernel/Updatable_ABC.h"
#include "ASN_Types.h"

namespace kernel {
    class InstanciationComplete;
}

// =============================================================================
/** @class  FormationHierarchy
    @brief  FormationHierarchy
    // $$$$ AGE 2006-10-19: chies
*/
// Created: AGE 2006-10-19
// =============================================================================
class FormationHierarchy : public kernel::DelayedEntityHierarchies< kernel::TacticalHierarchies >
{

public:
    //! @name Constructors/Destructor
    //@{
             FormationHierarchy( kernel::Controller& controller, kernel::Entity_ABC& entity, kernel::Entity_ABC* superior );
    virtual ~FormationHierarchy();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FormationHierarchy( const FormationHierarchy& );            //!< Copy constructor
    FormationHierarchy& operator=( const FormationHierarchy& ); //!< Assignement operator
    //@}
};

#endif // __FormationHierarchy_h_
