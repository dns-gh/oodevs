// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Preparation_Tools_h_
#define __Preparation_Tools_h_

#include "clients_kernel/Tools.h"
#include "clients_kernel/SubTypes.h"

namespace tools
{
    E_PopulationAttitude    PopulationAttitudeFromXml( const std::string& type );
    E_DemolitionTargetType  DemolitionTargetTypeFromXml( const std::string& type );
    std::string ToXml( E_DemolitionTargetType type );
    std::string ToXml( E_PopulationAttitude type );
};

#endif // __Preparation_Tools_h_
