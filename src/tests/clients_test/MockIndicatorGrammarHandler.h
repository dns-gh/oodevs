// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MockIndicatorGrammarHandler_h_
#define __MockIndicatorGrammarHandler_h_

#include "indicators/GrammarHandler_ABC.h"

// =============================================================================
/** @class  MockIndicatorGrammarHandler
    @brief  MockIndicatorGrammarHandler
*/
// Created: SBO 2009-03-16
// =============================================================================
MOCK_BASE_CLASS( MockIndicatorGrammarHandler, indicators::GrammarHandler_ABC )
{
    MOCK_METHOD( HandleNumber, 1 )
    MOCK_METHOD( HandleString, 1 )
    MOCK_METHOD( HandleVariable, 1 )
    MOCK_METHOD( HandleFunctionCall, 2 )
};

#endif // __MockIndicatorGrammarHandler_h_
