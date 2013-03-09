// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MockValueHandler_h_
#define __MockValueHandler_h_

#include "3a/ValueHandler_ABC.h"

// =============================================================================
/** @class  MockValueHandler
    @brief  Mock Value Handler
*/
// Created: LGY 2010-08-09
// =============================================================================
template< typename T >
MOCK_BASE_CLASS( MockValueHandler, ValueHandler_ABC< T > )
{
    MOCK_METHOD( BeginTick, 0, void() );
    MOCK_METHOD_TPL( Handle, 1, void( const T& ) );
    MOCK_METHOD( EndTick, 0, void() );
};

#endif // __MockValueHandler_h_
