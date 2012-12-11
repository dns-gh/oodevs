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

// =============================================================================
/** @class  FirerIdentifierValue
    @brief  FirerIdentifierValue
*/
// Created: JSR 2012-12-11
// =============================================================================
struct FirerIdentifierValue : public InstantValueIdentifier< NumericValue >
{
    enum { has_parameter = false };
    //! @name Operations
    //@{
    virtual void Receive( const sword::SimToClient& wrapper )
    {
        if( wrapper.message().has_unit_damaged_by_unit_fire() )
            Set( wrapper.message().unit_damaged_by_unit_fire().firer().id() );
    }
    //@}
};

#endif // __FirerIdentifierValue_h_
