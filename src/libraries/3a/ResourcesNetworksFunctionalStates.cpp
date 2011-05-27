// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "ResourcesNetworksFunctionalStates.h"

using namespace extractors;

// -----------------------------------------------------------------------------
// Name: ResourcesNetworksFunctionalStates constructor
// Created: FPO 2011-05-26
// -----------------------------------------------------------------------------
ResourcesNetworksFunctionalStates::ResourcesNetworksFunctionalStates()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourcesNetworksFunctionalStates destructor
// Created: FPO 2011-05-26
// -----------------------------------------------------------------------------
ResourcesNetworksFunctionalStates::~ResourcesNetworksFunctionalStates()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourcesNetworksFunctionalStates constructor
// Created: FPO 2011-05-26
// -----------------------------------------------------------------------------
ResourcesNetworksFunctionalStates::ResourcesNetworksFunctionalStates( xml::xistream& /*xis*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourcesNetworksFunctionalStates::Extract
// Created: FPO 2011-05-26
// -----------------------------------------------------------------------------
float ResourcesNetworksFunctionalStates::Extract( const sword::SimToClient& wrapper )
{
    float result = 0;
    unsigned int count = 0;
    const sword::UrbanAttributes_Infrastructures& infrastructures = wrapper.message().urban_update().attributes().infrastructures();
    for( int i = 0; i < infrastructures.resource_network_size(); ++i )
    {
        const sword::ResourceNetwork& resource_network = infrastructures.resource_network( i );
        result += resource_network.functional_state();
        ++count;
    }
    if( count != 0 ) result /= count;
    return result;
}
