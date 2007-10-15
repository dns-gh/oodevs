// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __IntelligenceHierarchies_h_
#define __IntelligenceHierarchies_h_

#include "clients_kernel/SimpleHierarchies.h"
#include "clients_kernel/TacticalHierarchies.h"

// =============================================================================
/** @class  IntelligenceHierarchies
    @brief  IntelligenceHierarchies
*/
// Created: SBO 2007-10-15
// =============================================================================
class IntelligenceHierarchies : public kernel::SimpleHierarchies< kernel::TacticalHierarchies >
{

public:
    //! @name Constructors/Destructor
    //@{
             IntelligenceHierarchies( kernel::Entity_ABC& holder, kernel::Entity_ABC* superior );
    virtual ~IntelligenceHierarchies();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    IntelligenceHierarchies( const IntelligenceHierarchies& );            //!< Copy constructor
    IntelligenceHierarchies& operator=( const IntelligenceHierarchies& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Entity_ABC* superior_;
    //@}
};

#endif // __IntelligenceHierarchies_h_
