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

// =============================================================================
/** @class  PopulationHierarchies
    @brief  PopulationHierarchies
*/
// Created: AGE 2006-11-24
// =============================================================================
class PopulationHierarchies : public kernel::SimpleHierarchies< kernel::TacticalHierarchies >
{
public:
    //! @name Constructors/Destructor
    //@{
             PopulationHierarchies( const kernel::Entity_ABC& holder, const kernel::Entity_ABC& superior );
    virtual ~PopulationHierarchies();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PopulationHierarchies( const PopulationHierarchies& );            //!< Copy constructor
    PopulationHierarchies& operator=( const PopulationHierarchies& ); //!< Assignment operator
    //@}
};

#endif // __PopulationHierarchies_h_
