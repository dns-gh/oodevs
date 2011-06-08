// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MockLogisticEntity_h_
#define __MockLogisticEntity_h_

#include "dispatcher/LogisticEntity_ABC.h"

// =============================================================================
/** @class  MockLogisticEntity
    @brief  MockLogisticEntity
*/
// Created: SBO 2010-06-02
// =============================================================================
MOCK_BASE_CLASS( MockLogisticEntity, dispatcher::LogisticEntity_ABC )
{
    MOCK_METHOD( GetLogisticLevel, 0 );
    MOCK_METHOD( Send, 1 );
};


#endif // __MockLogisticEntity_h_
