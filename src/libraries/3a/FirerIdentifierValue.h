// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FirerIdentifierValue_h_
#define __FirerIdentifierValue_h_

#include "Values.h"
#include "Types.h"

namespace sword
{
    class SimToClient;
}

// =============================================================================
/** @class  FirerIdentifierValue
    @brief  FirerIdentifierValue
*/
// Created: JSR 2012-12-11
// =============================================================================
struct FirerIdentifierValue : public Value< NumericValue >
{
    enum { has_parameter = false };
    //! @name Operations
    //@{
    void Receive( const sword::SimToClient& wrapper );
    //@}
};

// =============================================================================
/** @class  TargetIdentifierValue
    @brief  TargetIdentifierValue
*/
// Created: JSR 2012-12-11
// =============================================================================
struct TargetIdentifierValue : public Value< NumericValue >
{
    enum { has_parameter = false };
    //! @name Operations
    //@{
    virtual void Receive( const sword::SimToClient& wrapper );
    //@}
};

#endif // __FirerIdentifierValue_h_
