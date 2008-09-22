// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MiscEvents_h_
#define __MiscEvents_h_

#include "Events.h"

namespace script
{
    namespace events
    {
        DECLARE_EVENT( TickEnded, int, tick );
        DECLARE_EVENT( TimeFlowed, float, delta );
        DECLARE_EVENT( PhaseChanged, std::string, phase );

        DECLARE_EVENT2( ClientJoined, std::string, client, std::string, profile );
        DECLARE_EVENT(  ClientLeft, std::string, client );

        struct Reset {};
    }

}

#endif // __MiscEvents_h_
