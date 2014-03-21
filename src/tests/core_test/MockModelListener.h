// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef CORE_TEST_MOCK_MODEL_LISTENER_H
#define CORE_TEST_MOCK_MODEL_LISTENER_H

#include "core/ModelListener_ABC.h"

namespace core
{
    MOCK_BASE_CLASS( MockModelListener, ModelListener_ABC )
    {
        MOCK_METHOD( NotifyChanged, 1 )
        MOCK_METHOD( NotifyRemoved, 1 )
    };
}

#endif // CORE_TEST_MOCK_MODEL_LISTENER_H
