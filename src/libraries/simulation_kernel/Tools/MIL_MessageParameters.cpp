// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
#include "MIL_MessageParameters.h"

#include "protocol/Simulation.h"

const DEC_Model_ABC* parameters::GetModel( const sword::MissionParameters& params, const ModelFinder& finder )
{
    if( !params.elem_size() )
        return nullptr;
    const std::string name = protocol::GetString( params, 0 );
    auto model = finder( name );
    protocol::Check( model, "must be a valid model", 0 );
    return model;
}
