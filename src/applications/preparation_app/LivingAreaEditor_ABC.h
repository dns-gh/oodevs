// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LivingAreaEditor_ABC_h_
#define __LivingAreaEditor_ABC_h_

#include "tools/Observer_ABC.h"

// =============================================================================
/** @class  LivingAreaEditor_ABC
    @brief  Living area editor declaration
*/
// Created: LGY 2012-01-12
// =============================================================================
class LivingAreaEditor_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             LivingAreaEditor_ABC() {}
    virtual ~LivingAreaEditor_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Select( const geometry::Point2f& point ) = 0;
    //@}
};

#endif // __LivingAreaEditor_ABC_h_
