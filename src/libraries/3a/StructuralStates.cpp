// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "StructuralStates.h"
#include "protocol/Simulation.h"

using namespace extractors;

// -----------------------------------------------------------------------------
// Name: StructuralStates constructor
// Created: FPO 2011-05-13
// -----------------------------------------------------------------------------
StructuralStates::StructuralStates()
    : hasInfrastructure_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StructuralStates destructor
// Created: FPO 2011-05-13
// -----------------------------------------------------------------------------
StructuralStates::~StructuralStates()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StructuralStates constructor
// Created: FPO 2011-05-13
// -----------------------------------------------------------------------------
StructuralStates::StructuralStates( xml::xistream& /*xis*/ )
    : hasInfrastructure_( false )
{
    // NOTHING
}

bool StructuralStates::HasValue( const sword::SimToClient& wrapper ) const
{
    if( wrapper.message().has_urban_update() && wrapper.message().urban_update().has_attributes() )
    {
        if( !hasInfrastructure_ )
        {
            const sword::UrbanAttributes& attributes = wrapper.message().urban_update().attributes();
            if( attributes.has_infrastructures() && attributes.infrastructures().has_infrastructure() )
                hasInfrastructure_ = true;
        }

        return ( hasInfrastructure_ && wrapper.message().urban_update().attributes().has_structure() );
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: StructuralStates::Extract
// Created: FPO 2011-05-13
// -----------------------------------------------------------------------------
int StructuralStates::Extract( const sword::SimToClient& wrapper )
{
    return wrapper.message().urban_update().attributes().structure().state();
}
