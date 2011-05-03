// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __IdentifierValue_ABC_h_
#define __IdentifierValue_ABC_h_

#include "Values.h"
#include "Types.h"

// =============================================================================
/** @class  IdentifierValue_ABC
    @brief  IdentifierValue_ABC
*/
// Created: FPO 2011 05 03
// =============================================================================
struct IdentifierValue_ABC : public InstantValue< NumericValue >
{
    enum { has_parameter = false };

    //! @name Operations
    //@{
    virtual void Receive( const sword::SimToClient& wrapper ) = 0;
    //@}
};

#endif // __IdentifierValue_ABC_h_
