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
// Created: SBO 2006-11-08
// =============================================================================
class PopulationHierarchies : public kernel::SimpleHierarchies< kernel::TacticalHierarchies >
{
public:
    //! @name Constructors/Destructor
    //@{
             PopulationHierarchies( kernel::Entity_ABC& holder, kernel::Entity_ABC* superior );
    virtual ~PopulationHierarchies();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PopulationHierarchies( const PopulationHierarchies& );            //!< Copy constructor
    PopulationHierarchies& operator=( const PopulationHierarchies& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Entity_ABC* superior_;
    //@}
};

#endif // __PopulationHierarchies_h_
