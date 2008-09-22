// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "Tools.h"
#include "ENT/ENT_Tr.h"

// -----------------------------------------------------------------------------
// Name: tools::PopulationAttitudeFromXml
// Created: AGE 2007-06-19
// -----------------------------------------------------------------------------
E_PopulationAttitude tools::PopulationAttitudeFromXml( const std::string& type )
{
    return ENT_Tr::ConvertToPopulationAttitude( type  );
}

// -----------------------------------------------------------------------------
// Name: tools::ObstacleTypeFromXml
// Created: AGE 2007-06-19
// -----------------------------------------------------------------------------
E_ObstacleType tools::ObstacleTypeFromXml( const std::string& type )
{
    return ENT_Tr::ConvertToObstacleType( type );
}

// -----------------------------------------------------------------------------
// Name: tools::ToXml
// Created: AGE 2007-06-19
// -----------------------------------------------------------------------------
std::string tools::ToXml( E_ObstacleType type )
{
    return ENT_Tr::ConvertFromObstacleType( type, ENT_Tr::eToSim );
}

// -----------------------------------------------------------------------------
// Name: tools::ToXml
// Created: AGE 2007-06-19
// -----------------------------------------------------------------------------
std::string tools::ToXml( E_PopulationAttitude type )
{
    return ENT_Tr::ConvertFromPopulationAttitude( type, ENT_Tr::eToSim );
}
