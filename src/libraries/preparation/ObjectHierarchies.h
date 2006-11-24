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

#include "clients_kernel/SimpleHierarchies.h"
#include "clients_kernel/TacticalHierarchies.h"

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
             ObjectHierarchies( kernel::Controller& controller, kernel::Entity_ABC& holder, kernel::Entity_ABC* superior );
    virtual ~ObjectHierarchies();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ObjectHierarchies( const ObjectHierarchies& );            //!< Copy constructor
    ObjectHierarchies& operator=( const ObjectHierarchies& ); //!< Assignement operator
    //@}
};

#endif // __ObjectHierarchies_h_
