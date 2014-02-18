// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Paint_ABC_h_
#define __Paint_ABC_h_

#include "Property_ABC.h"

namespace svg
{
    class References_ABC;

// =============================================================================
/** @class  Paint_ABC
    @brief  Color definition
*/
// Created: AGE 2006-08-14
// =============================================================================
class Paint_ABC : public Property_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Paint_ABC() {};
    virtual ~Paint_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual bool Setup( References_ABC& references, float opacity ) const = 0;
    //@}
};

}

#endif // __Paint_ABC_h_
