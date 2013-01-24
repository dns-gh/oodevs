// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Lives_ABC_h_
#define __Lives_ABC_h_

#include "clients_gui/Drawable_ABC.h"
#include "clients_kernel/Extension_ABC.h"

// =============================================================================
/** @class  Lives_ABC
    @brief  Lives declaration
*/
// Created: LGY 2011-03-08
// =============================================================================
class Lives_ABC : public kernel::Extension_ABC
                , public gui::Drawable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Lives_ABC() {};
    virtual ~Lives_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual float GetLife() const = 0;
    //@}
};

#endif // __Lives_ABC_h_
