// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationHierarchies_h_
#define __PopulationHierarchies_h_

#include "clients_kernel/SimpleHierarchies.h"
#include "clients_kernel/TacticalHierarchies.h"
#include <boost/noncopyable.hpp>

// =============================================================================
/** @class  PopulationHierarchies
    @brief  PopulationHierarchies
*/
// Created: SBO 2006-11-08
// =============================================================================
class PopulationHierarchies : public kernel::SimpleHierarchies< kernel::TacticalHierarchies >
                            , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             PopulationHierarchies( kernel::Entity_ABC& holder, kernel::Entity_ABC* superior );
    virtual ~PopulationHierarchies();
    //@}

public:
    //! @name Operations
    //@{
    void ChangeSuperior( kernel::Entity_ABC& superior );
    //@}
};

#endif // __PopulationHierarchies_h_
