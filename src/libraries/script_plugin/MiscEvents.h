// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __MiscEvents_h_
#define __MiscEvents_h_

#include "Events.h"

namespace plugins
{
namespace script
{
    namespace events
    {
        DECLARE_EVENT( SimulationTimeChanged, std::string, time );
        DECLARE_EVENT2( TickEnded, int, tick, unsigned int, duration );
        DECLARE_EVENT( TimeFlowed, float, delta );
        DECLARE_EVENT( PhaseChanged, std::string, phase );

        DECLARE_EVENT2( ClientJoined, std::string, client, std::string, profile );
        DECLARE_EVENT( ClientLeft, std::string, client );

        DECLARE_EVENT2( IndicatorChanged, std::string, name, double, value );

        struct Reset {};
    }

}
}

#endif // __MiscEvents_h_
