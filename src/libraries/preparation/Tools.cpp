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
// Name: tools::DemolitionTargetTypeFromXml
// Created: AGE 2007-06-19
// -----------------------------------------------------------------------------
E_DemolitionTargetType tools::DemolitionTargetTypeFromXml( const std::string& type )
{
    return ENT_Tr::ConvertToDemolitionTargetType( type );
}

// -----------------------------------------------------------------------------
// Name: tools::ToXml
// Created: AGE 2007-06-19
// -----------------------------------------------------------------------------
std::string tools::ToXml( E_DemolitionTargetType type )
{
    return ENT_Tr::ConvertFromDemolitionTargetType( type, ENT_Tr::eToSim );
}

// -----------------------------------------------------------------------------
// Name: tools::ToXml
// Created: AGE 2007-06-19
// -----------------------------------------------------------------------------
std::string tools::ToXml( E_PopulationAttitude type )
{
    return ENT_Tr::ConvertFromPopulationAttitude( type, ENT_Tr::eToSim );
}
