// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Helpers_h_
#define __Helpers_h_

#include "protocol/ClientSenders.h"

// =============================================================================
/** @class  Helpers
    @brief  Helpers
*/
// Created: SBO 2010-06-09
// =============================================================================
namespace sword
{
    inline bool operator==( const sword::SimToClient& lhs, const sword::SimToClient& rhs )
    {
        BOOST_CHECK_EQUAL( lhs.DebugString(), rhs.DebugString() );
        return true;
    }
}

#endif // __Helpers_h_
