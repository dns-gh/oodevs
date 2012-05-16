// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __UrbanHierarchies_h_
#define __UrbanHierarchies_h_

#include "clients_kernel/UrbanTypes.h"
#include "clients_kernel/Hierarchies.h"
#include "clients_kernel/EntityHierarchies.h"
#include <boost/noncopyable.hpp>

// =============================================================================
/** @class  UrbanHierarchies
    @brief  UrbanHierarchies
*/
// Created: JSR 2012-05-14
// =============================================================================
class UrbanHierarchies : public kernel::EntityHierarchies< kernel::Hierarchies >
                       , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanHierarchies( kernel::Controller& controller, kernel::Entity_ABC& entity, kernel::Entity_ABC* superior );
    virtual ~UrbanHierarchies();
    //@}

    //! @name Operations
    //@{
    EUrbanLevel GetLevel() const;
    //@}

private:
    //! @name Member data
    //@{
    EUrbanLevel level_;
    //@}
};

#endif // __UrbanHierarchies_h_
