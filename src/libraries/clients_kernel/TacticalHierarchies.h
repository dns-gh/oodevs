// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __kernel_TacticalHierarchies_h_
#define __kernel_TacticalHierarchies_h_

#include "Hierarchies.h"

namespace kernel
{

// =============================================================================
/** @class  TacticalHierarchies
    @brief  Tactical hierarchies
            agent -> formations -> team
*/
// Created: AGE 2006-10-04
// =============================================================================
class TacticalHierarchies : public Hierarchies
{

public:
    //! @name Constructors/Destructor
    //@{
             TacticalHierarchies() {};
    virtual ~TacticalHierarchies() {};
    //@}

    //! @name Operations
    //@{
    virtual std::string GetSymbol() const { return ""; };
    virtual void UpdateSymbol() {};
    //@}
};

}

#endif // __kernel_TacticalHierarchies_h_
