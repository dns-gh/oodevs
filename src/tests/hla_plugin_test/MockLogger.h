// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef MockLogger_h
#define MockLogger_h

#include "dispatcher/Logger_ABC.h"

namespace dispatcher
{
// =============================================================================
/** @class  MockLogger
    @brief  Mock simulation publisher
*/
// Created: VPR 2011-09-06
// =============================================================================
MOCK_BASE_CLASS( MockLogger, Logger_ABC )
{
    MOCK_METHOD( LogInfo, 1 )
    MOCK_METHOD( LogError, 1 )
    MOCK_METHOD( LogWarning, 1 )
};

}

#endif // MockLogger_h
