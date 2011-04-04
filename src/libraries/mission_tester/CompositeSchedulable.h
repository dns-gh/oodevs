// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __CompositeSchedulable_h_
#define __CompositeSchedulable_h_

#include "Schedulable_ABC.h"

namespace mission_tester
{
    class State_ABC;
// =============================================================================
/** @class  CompositeSchedulable
    @brief  CompositeSchedulable
*/
// Created: PHC 2011-03-30
// =============================================================================
class CompositeSchedulable : private Schedulable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             CompositeSchedulable();
    virtual ~CompositeSchedulable();
    //@}

    //! @name Operations
    //@{
    virtual bool Trigger( State_ABC& state );
    //@}
};
}

#endif // __CompositeSchedulable_h_
