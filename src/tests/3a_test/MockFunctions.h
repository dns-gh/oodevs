// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MockFunctions_h_
#define __MockFunctions_h_

#include "3a/Functions.h"

// =============================================================================
/** @class  MockFunction1
    @brief  Mock Function 1
*/
// Created: LGY 2010-08-09
// =============================================================================
#ifndef TYPE
#define TYPE Function1_ABC< K, A >
template< typename K, typename A >
MOCK_BASE_CLASS( MockFunction1, TYPE )
{
    MOCK_METHOD_EXT_TPL( BeginTick, 0, void(), BeginTick );
    MOCK_METHOD_EXT_TPL( SetKey, 1, void( const K& ), SetKey );
    MOCK_METHOD_EXT_TPL( Apply, 1, void( const A& ), Apply );
    MOCK_METHOD_EXT_TPL( EndTick, 0, void(), EndTick );
};
#endif

// =============================================================================
/** @class  MockFunction2
    @brief  Mock Function 2
*/
// Created: LGY 2010-08-09
// =============================================================================
#ifndef TYPE2
#define TYPE2 Function2_ABC< K, A1, A2 >
template< typename K, typename A1, typename A2 >
MOCK_BASE_CLASS( MockFunction2, TYPE2 )
{
    MOCK_METHOD_EXT_TPL( BeginTick, 0, void(), BeginTick );
    MOCK_METHOD_EXT_TPL( SetKey, 1, void( const K& ), SetKey );
    MOCK_METHOD_EXT_TPL( Apply, 2, void( const A1&, const A2& ), Apply );
    MOCK_METHOD_EXT_TPL( EndTick, 0, void(), EndTick );
};
#endif

#endif // __MockFunctions_h_
