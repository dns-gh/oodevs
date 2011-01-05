// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "BurnSurfaceAttribute.h"
#include "protocol/SimulationSenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: BurnSurfaceAttribute constructor
// Created: RFT 2006-09-26
// -----------------------------------------------------------------------------
BurnSurfaceAttribute::BurnSurfaceAttribute( const sword::ObjectAttributes& asnMsg )
: cellSize_( 0 )
{
    DoUpdate( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: BurnSurfaceAttribute destructor
// Created: RFT 2006-09-26
// -----------------------------------------------------------------------------
BurnSurfaceAttribute::~BurnSurfaceAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BurnSurfaceAttribute::Update
// Created: RFT 2006-09-26
// -----------------------------------------------------------------------------
void BurnSurfaceAttribute::Update( const sword::ObjectAttributes& asnMsg )
{
	DoUpdate( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: BurnSurfaceAttribute::Send
// Created: RFT 2006-09-27
// -----------------------------------------------------------------------------
void BurnSurfaceAttribute::Send( sword::ObjectAttributes& asnMsg ) const
{
	asnMsg.mutable_burn_surface()->set_cell_size( cellSize_ );
	for( BurningCellsByCoordinatesMap::const_iterator it = burningCellsByCoordinates_.begin(); it != burningCellsByCoordinates_.end(); ++it )
	{
		const BurningCell& cell = it->second;
		sword::ObjectAttributeBurnSurface::BurningCell& asnCell = *asnMsg.mutable_burn_surface()->add_burningcells();
		asnCell.set_origin_x( it->first.X() );
		asnCell.set_origin_y( it->first.Y() );
		asnCell.set_phase( it->second.phase_ );
		switch( cell.phase_ )
		{
		case sword::pre_ignition:
			{
				asnCell.mutable_pre_ignition()->set_ignition_energy( cell.ignitionEnergy_ );
				asnCell.mutable_pre_ignition()->set_ignition_threshold( cell.ignitionThreshold_ );
				break;
			}
		case sword::combustion:
			{
				asnCell.mutable_combustion()->set_combustion_energy_count( cell.combustionEnergyCount_ );
				asnCell.mutable_combustion()->set_combustion_energy_sum( cell.combustionEnergySum_ );
				asnCell.mutable_combustion()->set_current_heat( cell.currentHeat_ );
				asnCell.mutable_combustion()->set_max_combustion_energy( cell.maxCombustionEnergy_ );
				break;
			}
		case sword::decline:
			{
				asnCell.mutable_decline()->set_current_heat( cell.currentHeat_ );
				break;
			}
		}
	}
}

// -----------------------------------------------------------------------------
// Name: BurnSurfaceAttribute::DoUpdate
// Created: BCI 2011-01-04
// -----------------------------------------------------------------------------
void BurnSurfaceAttribute::DoUpdate( const sword::ObjectAttributes& asnMsg )
{
    if( asnMsg.has_burn_surface() )
    {
		cellSize_ = asnMsg.burn_surface().cell_size();
		for( int i=0, nbCells = asnMsg.burn_surface().burningcells_size(); i<nbCells; ++i )
		{
			const sword::ObjectAttributeBurnSurface::BurningCell& asnCell = asnMsg.burn_surface().burningcells( i );
			BurningCell& cell = burningCellsByCoordinates_[ BurningCellOrigin( asnCell.origin_x(), asnCell.origin_y() ) ];
			cell.phase_ = asnCell.phase();
			if( asnCell.has_pre_ignition() )
			{
				cell.ignitionEnergy_ = asnCell.pre_ignition().ignition_energy();
				cell.ignitionThreshold_ = asnCell.pre_ignition().ignition_threshold();
			}
			if( asnCell.has_combustion() )
			{
				cell.combustionEnergyCount_ = asnCell.combustion().combustion_energy_count();
				cell.combustionEnergySum_ = asnCell.combustion().combustion_energy_sum();
				cell.currentHeat_ = asnCell.combustion().current_heat();
				cell.maxCombustionEnergy_ = asnCell.combustion().max_combustion_energy();
			}
			if( asnCell.has_decline() )
				cell.currentHeat_ = asnCell.decline().current_heat();
		}
    }
}
