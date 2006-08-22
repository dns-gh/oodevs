// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Perception_h_
#define __Perception_h_

#include "Types.h"

namespace kernel
{
    class Agent_ABC;
}

// =============================================================================
/** @class  Perception
    @brief  Perception
*/
// Created: AGE 2006-02-22
// =============================================================================
class Perception
{

public:
    //! @name Constructors/Destructor
    //@{
             Perception( const kernel::Agent_ABC& detected, kernel::E_PerceptionResult level );
    virtual ~Perception();
    //@}

public:
    //! @name Member data
    //@{
    const kernel::Agent_ABC* detected_;
    kernel::E_PerceptionResult level_;
    //@}
};

#endif // __Perception_h_
