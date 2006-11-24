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

#include "ASN_Messages.h"
#include "clients_kernel/SimpleHierarchies.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Resolver_ABC.h"

namespace kernel
{
    class Team_ABC;
}

// =============================================================================
/** @class  ObjectHierarchies
    @brief  Object hierarchies. Duh !
*/
// Created: AGE 2006-10-16
// =============================================================================
class ObjectHierarchies : public kernel::SimpleHierarchies< kernel::TacticalHierarchies >
{

public:
    //! @name Constructors/Destructor
    //@{
             ObjectHierarchies( kernel::Entity_ABC& entity, const kernel::Team_ABC& superior );
    virtual ~ObjectHierarchies();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ObjectHierarchies( const ObjectHierarchies& );            //!< Copy constructor
    ObjectHierarchies& operator=( const ObjectHierarchies& ); //!< Assignement operator
    //@}
};

#endif // __ObjectHierarchies_h_
