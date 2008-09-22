// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Gaming_Tools_h_
#define __Gaming_Tools_h_

#include "clients_kernel/Tools.h"
#include "Types.h"

namespace tools
{
    E_PopulationAttitude PopulationAttitudeFromXml( const std::string& type );
    E_ObstacleType       ObstacleTypeFromXml( const std::string& type );
    std::string ToXml( E_ObstacleType type );
    std::string ToXml( E_PopulationAttitude type );
};

#endif // __Gaming_Tools_h_
