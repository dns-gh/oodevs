// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __IdentifierValue_h_
#define __IdentifierValue_h_

#include "Types.h"
#include "Values.h"

namespace sword
{
    class SimToClient;
}

// =============================================================================
/** @class  IdentifierValue
    @brief  IdentifierValue
*/
// Created: AGE 2007-08-28
// =============================================================================
struct IdentifierValue : public Value< NumericValue >
{
    enum { has_parameter = false };

    //! @name Operations
    //@{
    virtual void Receive( const sword::SimToClient& wrapper );
    //@}
};

#endif // __IdentifierValue_h_
