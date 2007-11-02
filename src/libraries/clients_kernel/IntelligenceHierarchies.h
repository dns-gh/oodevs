// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __kernel_IntelligenceHierarchies_h_
#define __kernel_IntelligenceHierarchies_h_

#include "TacticalHierarchies.h"

namespace kernel
{

// =============================================================================
/** @class  IntelligenceHierarchies
    @brief  Intelligence hierarchies
            intelligence -> formation -> team
*/
// Created: SBO 2007-10-18
// =============================================================================
class IntelligenceHierarchies : public Hierarchies, public Symbol_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             IntelligenceHierarchies() {}
    virtual ~IntelligenceHierarchies() {}
    //@}
};

}

#endif // __kernel_IntelligenceHierarchies_h_
