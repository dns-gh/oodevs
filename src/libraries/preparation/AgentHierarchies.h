// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentHierarchies_h_
#define __AgentHierarchies_h_

#include "TacticalHierarchies.h"

// =============================================================================
/** @class  AgentHierarchies
    @brief  AgentHierarchies
*/
// Created: AGE 2006-09-20
// =============================================================================
class AgentHierarchies : public TacticalHierarchies
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentHierarchies( kernel::Controller& controller, kernel::Entity_ABC& holder, kernel::Entity_ABC* superior );
    virtual ~AgentHierarchies();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AgentHierarchies( const AgentHierarchies& );            //!< Copy constructor
    AgentHierarchies& operator=( const AgentHierarchies& ); //!< Assignement operator
    //@}
};

#endif // __AgentHierarchies_h_
