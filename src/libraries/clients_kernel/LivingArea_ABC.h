// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __LivingArea_ABC_h_
#define __LivingArea_ABC_h_

#include "Extension_ABC.h"

namespace kernel
{
    class LivingAreaVisitor_ABC;

// =============================================================================
/** @class  LivingArea_ABC
    @brief  Living area declaration
*/
// Created: LGY 2011-05-04
// =============================================================================
 class LivingArea_ABC : public kernel::Extension_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             LivingArea_ABC() {}
    virtual ~LivingArea_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual unsigned long GetId() const = 0;
    virtual void Accept( LivingAreaVisitor_ABC& visitor ) const = 0;
    //@}
};

}

#endif // __LivingArea_ABC_h_
