// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ColorNone_h_
#define __ColorNone_h_

#include "Paint_ABC.h"

namespace svg
{

// =============================================================================
/** @class  ColorNone
    @brief  ColorNone
*/
// Created: AGE 2006-08-15
// =============================================================================
class ColorNone : public Paint_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ColorNone();
    virtual ~ColorNone();
    //@}

    //! @name Operations
    //@{
    virtual bool Setup( References_ABC& references, float opacity ) const;
    virtual ColorNone& Clone() const;
    //@}
};

}

#endif // __ColorNone_h_
