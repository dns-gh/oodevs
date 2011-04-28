// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __weather_ReadDirections_h_
#define __weather_ReadDirections_h_

#include "MT_Tools/MT_Vector2D.h"

namespace sword
{
    class Heading;
}

namespace weather
{
    MT_Vector2D ReadDirection( double orientation );
    MT_Vector2D ReadDirection( const sword::Heading& message );
}

#endif // __weather_ReadDirections_h_
