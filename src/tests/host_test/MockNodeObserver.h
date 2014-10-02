// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef MOCK_NODE_OBSERVER_H
#define MOCK_NODE_OBSERVER_H

#include "host/NodeObserver_ABC.h"

namespace mocks
{
    MOCK_BASE_CLASS( MockNodeObserver, host::NodeObserver_ABC )
    {
        MOCK_METHOD( Notify, 1 );
    };
};

#endif // MOCK_NODE_OBSERVER_H
