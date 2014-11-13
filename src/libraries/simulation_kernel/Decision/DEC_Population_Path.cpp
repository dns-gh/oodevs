// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2002 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Population_Path.h"
#include "DEC_Population_PathClass.h"
#include "DEC_Population_PathfinderRule.h"
#include "DEC_PopulationContext.h"
#include "Decision/DEC_PathComputer.h"
#include "Decision/DEC_PathType.h"
#include "Entities/Populations/MIL_Population.h"
#include "simulation_terrain/TER_PathSection.h"
#include <boost/make_shared.hpp>

//-----------------------------------------------------------------------------
// Name: DEC_Population_Path constructor
// Created: JSR 2014-01-16
//-----------------------------------------------------------------------------
DEC_Population_Path::DEC_Population_Path( const MIL_Population& population,
        const T_PointVector& points )
    : DEC_PathResult( DEC_PathType::movement_, population.GetID() )
    , context_( new DEC_PopulationContext( population, points ) )
{
    if( points.empty() )
        throw MASA_EXCEPTION( "List of points is empty in population path initialization" );
    for( auto it = points.begin(); it != points.end() - 1; ++it )
    {
        std::unique_ptr< TerrainRule_ABC > rule( new DEC_Population_PathfinderRule( context_ ) );
        GetComputer().RegisterPathSection( *new TER_PathSection( std::move( rule ), *it, *(it + 1), false, false ) );
    }
}

//-----------------------------------------------------------------------------
// Name: DEC_Population_Path destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
DEC_Population_Path::~DEC_Population_Path()
{
    // NOTHING
}
