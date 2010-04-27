// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MockNET_Publisher_ABC_h_
#define __MockNET_Publisher_ABC_h_

#include "Network/NET_Publisher_ABC.h"

MOCK_BASE_CLASS( MockNET_Publisher_ABC, NET_Publisher_ABC )
{
    MOCK_METHOD( Send, 1 );
};

#endif // __MockPublisher_ABC_h_
