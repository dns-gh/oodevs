// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "StructuralStates.h"

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

// -----------------------------------------------------------------------------
// Name: StructuralStates::Extract
// Created: FPO 2011-05-13
// -----------------------------------------------------------------------------
int StructuralStates::Extract( const sword::SimToClient& wrapper )
{
    int result = 0;
    const sword::UrbanUpdate& update = wrapper.message().urban_update();
    result = update.attributes().structure().state();
    return result;
}
