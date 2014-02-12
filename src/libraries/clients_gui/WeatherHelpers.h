// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __WeatherHelpers_h_
#define __WeatherHelpers_h_

#include "clients_kernel/Rectangle.h"

namespace gui
{
    struct WeatherParameters
    {
        E_WeatherType type_;
        float temperature_;
        float windSpeed_;
        int windDirection_;
        float cloudFloor_;
        float cloudCeiling_;
        float cloudDensity_;
    };
    struct LocalWeatherParameters
    {
        WeatherParameters globalParams_;
        QDateTime startTime_;
        QDateTime endTime_;
        kernel::Rectangle location_;
        unsigned int id_;
    };
}

#endif // __WeatherHelpers_h_
