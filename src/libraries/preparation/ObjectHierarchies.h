// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PrepaObjectHierarchies_h_
#define __PrepaObjectHierarchies_h_

#include "clients_kernel/ObjectHierarchies.h"
#include <boost/noncopyable.hpp>

// =============================================================================
/** @class  ObjectHierarchies
    @brief  ObjectHierarchies
*/
// Created: JSR 2011-02-22
// =============================================================================
class ObjectHierarchies : public kernel::ObjectHierarchies
{
public:
    //! @name Constructors/Destructor
    //@{
             ObjectHierarchies( kernel::Entity_ABC& holder, const kernel::Entity_ABC* superior );
    virtual ~ObjectHierarchies();
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Entity_ABC* superior_;
    //@}
};

#endif // __PrepaObjectHierarchies_h_
