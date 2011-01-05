// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_BurningCells.h"
#include "MIL_FireClass.h"
#include "MIL_Object_ABC.h"
#include "FireAttribute.h"
#include "BurnSurfaceAttribute.h"
#include "simulation_terrain/TER_PathFindManager.h"
#include "simulation_terrain/TER_World.h"
#include "meteo/PHY_Meteo.h"
#include "Tools/MIL_Tools.h"
#include "protocol/Protocol.h"
#include <pathfind/TerrainData.h>
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/cartesian2d.hpp>
#include <boost/geometry/geometries/register/point.hpp>
#include <boost/geometry/multi/multi.hpp>

BOOST_GEOMETRY_REGISTER_POINT_2D(MT_Vector2D, double, cs::cartesian, rX_, rY_)

// -----------------------------------------------------------------------------
// Name: MIL_BurningCells constructor
// Created: BCI 2010-12-20
// -----------------------------------------------------------------------------
MIL_BurningCells::MIL_BurningCells()
: lastCellIndexIncludedInLocalization_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_BurningCells destructor
// Created: BCI 2010-12-20
// -----------------------------------------------------------------------------
MIL_BurningCells::~MIL_BurningCells()
{
	for( BurningCellsByCoordinatesMap::iterator it = burningCellsByCoordinates_.begin(); it != burningCellsByCoordinates_.end(); ++it )
		delete it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_BurningCells::FindCell
// Created: BCI 2010-12-21
// -----------------------------------------------------------------------------
MIL_BurningCells::BurningCell* MIL_BurningCells::FindCell( const BurningCellOrigin& coords ) const
{
	BurningCellsByCoordinatesMap::const_iterator it = burningCellsByCoordinates_.find( coords );
	if( it != burningCellsByCoordinates_.end() )
		return it->second;
	else
		return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_BurningCells::ComputeCellOriginFromPoint
// Created: BCI 2010-12-22
// -----------------------------------------------------------------------------
MIL_BurningCells::BurningCellOrigin MIL_BurningCells::ComputeCellOriginFromPoint( double x, double y, int cellSize )
{
    //Compute cell aligned coordinates
	int alignedX = (int) x;
    int alignedY = (int) y;
	alignedX = alignedX - alignedX % cellSize;
    alignedY = alignedY - alignedY % cellSize;
	return BurningCellOrigin( alignedX, alignedY );
}

// -----------------------------------------------------------------------------
// Name: MIL_BurningCells::StartBurn
// Created: BCI 2010-12-20
// -----------------------------------------------------------------------------
void MIL_BurningCells::StartBurn( MIL_Object_ABC& object )
{
	const MT_Rect& boundingBox = object.GetLocalisation().GetBoundingBox();
	double minX = boundingBox.GetLeft();
	double minY = boundingBox.GetBottom();
	double maxX = boundingBox.GetRight();
	double maxY = boundingBox.GetTop();
	int cellSize = object.GetAttribute< FireAttribute >().GetCellSize();
	BurningCellOrigin minOrigin = ComputeCellOriginFromPoint( minX, minY, cellSize );
	BurningCellOrigin maxOrigin = ComputeCellOriginFromPoint( maxX, maxY, cellSize );
	for( int x = minOrigin.X(); x < maxOrigin.X(); x += cellSize )
		for( int y = minOrigin.Y(); y < maxOrigin.Y(); y += cellSize )
			StartBurn( BurningCellOrigin( x, y ), object );
}

// -----------------------------------------------------------------------------
// Name: MIL_BurningCells::StartBurn
// Created: BCI 2010-12-20
// -----------------------------------------------------------------------------
void MIL_BurningCells::StartBurn( const BurningCellOrigin& cellOrigin, MIL_Object_ABC& object )
{
	int cellSize = object.GetAttribute< FireAttribute >().GetCellSize();

	//init cell and his neighbours
	InitCell( cellOrigin, object, sword::combustion );
	InitCell( BurningCellOrigin( cellOrigin.X() - cellSize, cellOrigin.Y() - cellSize ), object, sword::pre_ignition );
	InitCell( BurningCellOrigin( cellOrigin.X() + cellSize, cellOrigin.Y() + cellSize ), object, sword::pre_ignition );
	InitCell( BurningCellOrigin( cellOrigin.X() - cellSize, cellOrigin.Y() + cellSize ), object, sword::pre_ignition );
	InitCell( BurningCellOrigin( cellOrigin.X() + cellSize, cellOrigin.Y() - cellSize ), object, sword::pre_ignition );
	InitCell( BurningCellOrigin( cellOrigin.X(), cellOrigin.Y() - cellSize ), object, sword::pre_ignition );
	InitCell( BurningCellOrigin( cellOrigin.X(), cellOrigin.Y() + cellSize ), object, sword::pre_ignition );
	InitCell( BurningCellOrigin( cellOrigin.X() - cellSize, cellOrigin.Y() ), object, sword::pre_ignition );
	InitCell( BurningCellOrigin( cellOrigin.X() + cellSize, cellOrigin.Y() ), object, sword::pre_ignition );
}

// -----------------------------------------------------------------------------
// Name: MIL_BurningCells::InitCell
// Created: BCI 2010-12-20
// -----------------------------------------------------------------------------
void MIL_BurningCells::InitCell( const BurningCellOrigin& cellOrigin, MIL_Object_ABC& object, sword::EnumBurningCellPhase phase )
{
	BurningCell* pCell = FindCell( cellOrigin );
	if( pCell )
	{
		//si la cellule existe déjà, on ne peut que la faire brûler si elle ne brûle pas encore
		if( phase != sword::combustion || pCell->phase_ == sword::combustion )
			return;
	}
	else
	{
		//ne brûler que le terrain
		if( !TER_World::GetWorld().GetExtent().IsInside( MT_Vector2D( cellOrigin.X(), cellOrigin.Y() ) ) )
			return;

		//nouvelle cellule
		pCell = new BurningCell( cellOrigin );
		burningCellsByCoordinates_[ cellOrigin ] = pCell;
		burningCellsByObjects_[ &object ].push_back( pCell );
	}

	//initialise la cellule
	double radius = MIL_FireClass::GetCellSize() / 2.;
	pCell->center_ = geometry::Point2d( cellOrigin.X() + radius, cellOrigin.Y() + radius );
	pCell->pObject_ = &object;
	pCell->phase_ = phase;
	TerrainData terrainData = TER_PathFindManager::GetPathFindManager().FindTerrainDataWithinCircle( MT_Vector2D( pCell->center_.X(), pCell->center_.Y() ), (float)radius );
	object.GetAttribute< FireAttribute >().GetSurfaceFirePotentials( terrainData, pCell->ignitionThreshold_, pCell->maxCombustionEnergy_ );
	pCell->ignitionEnergy_ = 0;
	pCell->combustionEnergySum_ = 0;
	pCell->combustionEnergyCount_ = 0;
	pCell->currentHeat_ = phase == sword::combustion ? object.GetAttribute< FireAttribute >().GetInitialHeat() : 0;
	pCell->currentCombustionEnergy_ = 0;
	pCell->lastUpdateTime_ = std::numeric_limits< unsigned int >::max();
	pCell->bUpdated_ = true;
	object.GetAttribute< BurnSurfaceAttribute >().NotifyCellsUpdated();
}

// -----------------------------------------------------------------------------
// Name: MIL_BurningCells::Update
// Created: BCI 2010-12-20
// -----------------------------------------------------------------------------
void MIL_BurningCells::Update( MIL_Object_ABC& object, unsigned int time )
{
	//retrieve cells to update (copy vector to avoid infinite loop)
	BurningCellsVector cellsToUpdate = burningCellsByObjects_[ &object ];

	//update cells
	for( BurningCellsVector::iterator it = cellsToUpdate.begin(); it != cellsToUpdate.end(); ++it )
	{
		BurningCell& cell = **it;
		if( cell.lastUpdateTime_ == std::numeric_limits< unsigned int >::max() )
			cell.lastUpdateTime_ = time;

		switch( cell.phase_ )
		{
		case sword::pre_ignition:
			UpdatePreIgnition( cell, time );
			break;
		case sword::combustion:
			UpdateCombustion( cell );
			break;
		case sword::decline:
			UpdateDecline( cell );
			break;
		}
	}

    //update localisation
    namespace bg = boost::geometry;
    typedef bg::polygon< MT_Vector2D > polygon_2d;
    typedef bg::multi_polygon< polygon_2d > multi_polygon_2d;
    typedef bg::box< MT_Vector2D > box_2d;
    int cellSize = object.GetAttribute< FireAttribute >().GetCellSize();
    const BurningCellsVector& cells = burningCellsByObjects_[ &object ];
    polygon_2d poly;
    bg::assign( poly, object.GetLocalisation().GetPoints() );
    for( ; lastCellIndexIncludedInLocalization_<cells.size(); ++lastCellIndexIncludedInLocalization_ )
    {
        BurningCell& cell = *cells[ lastCellIndexIncludedInLocalization_ ];
        box_2d cellBox( MT_Vector2D( cell.origin_.X(), cell.origin_.Y() ), MT_Vector2D( cell.origin_.X() + cellSize, cell.origin_.Y() + cellSize ) );
        multi_polygon_2d ps;
        bg::union_inserter< polygon_2d >( cellBox, poly, std::back_inserter( ps ) );
        polygon_2d hull;
        bg::convex_hull( ps, hull );
        poly = hull;
    }
    T_PointVector finalPoints( poly.outer().begin(), poly.outer().end() );
    object.UpdateLocalisation( TER_Localisation( TER_Localisation::ePolygon, finalPoints ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_BurningCells::UpdatePreIgnition
// Created: BCI 2010-12-20
// -----------------------------------------------------------------------------
void MIL_BurningCells::UpdatePreIgnition( BurningCell& cell, unsigned int time )
{
	int cellSize = cell.pObject_->GetAttribute< FireAttribute >().GetCellSize();
	unsigned int timeElapsed = time - cell.lastUpdateTime_;
    int oldIgnitionEnergy_ = cell.ignitionEnergy_;
	PropagateIgnition( BurningCellOrigin( cell.origin_.X() - cellSize, cell.origin_.Y() - cellSize ), cell, timeElapsed );
	PropagateIgnition( BurningCellOrigin( cell.origin_.X() + cellSize, cell.origin_.Y() + cellSize ), cell, timeElapsed );
	PropagateIgnition( BurningCellOrigin( cell.origin_.X() - cellSize, cell.origin_.Y() + cellSize ), cell, timeElapsed );
	PropagateIgnition( BurningCellOrigin( cell.origin_.X() + cellSize, cell.origin_.Y() - cellSize ), cell, timeElapsed );
	PropagateIgnition( BurningCellOrigin( cell.origin_.X(), cell.origin_.Y() + cellSize ), cell, timeElapsed );
	PropagateIgnition( BurningCellOrigin( cell.origin_.X(), cell.origin_.Y() - cellSize ), cell, timeElapsed );
	PropagateIgnition( BurningCellOrigin( cell.origin_.X() - cellSize, cell.origin_.Y() ), cell, timeElapsed );
	PropagateIgnition( BurningCellOrigin( cell.origin_.X() + cellSize, cell.origin_.Y() ), cell, timeElapsed );
	if( cell.ignitionEnergy_ >= cell.ignitionThreshold_ )
		StartBurn( cell.origin_, *cell.pObject_ );
    else if( cell.ignitionEnergy_ != oldIgnitionEnergy_ )
    {
        cell.bUpdated_ = true;
        cell.pObject_->GetAttribute< BurnSurfaceAttribute >().NotifyCellsUpdated();
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_BurningCells::PropagateIgnition
// Created: BCI 2010-12-20
// -----------------------------------------------------------------------------
void MIL_BurningCells::PropagateIgnition( const BurningCellOrigin& fromOrigin, BurningCell& cellTo, unsigned int timeElapsed )
{
	if( const BurningCell* pCellFrom = FindCell( fromOrigin ) )
	{
		geometry::Vector2d direction( pCellFrom->center_, cellTo.center_ );
		const weather::PHY_Meteo::sWindData& wind = MIL_Tools::GetWind( MT_Vector2D( pCellFrom->center_.X(), pCellFrom->center_.Y() ) );
		if( wind.rWindSpeed_ > 0.0 )
		{
			double windDirectionScalarProduct = direction.CrossProduct( geometry::Vector2d( wind.vWindDirection_.rX_, wind.vWindDirection_.rY_ ) );
			int ignitionTransfered = int( pCellFrom->currentHeat_ * timeElapsed * std::max( windDirectionScalarProduct, 0.0 ) / ( std::abs( windDirectionScalarProduct )+ wind.rWindSpeed_ ) );
			cellTo.ignitionEnergy_ += ignitionTransfered;
		}
	}
}

namespace
{
    template< typename T >
    T bound( T mini, T value, T maxi )
    {
        return std::max( mini, std::min( value, maxi ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_BurningCells::UpdateCombustion
// Created: BCI 2010-12-20
// -----------------------------------------------------------------------------
void MIL_BurningCells::UpdateCombustion( BurningCell& cell )
{
    if( cell.currentCombustionEnergy_ < cell.maxCombustionEnergy_ )
    {
		FireAttribute& fireAttribute = cell.pObject_->GetAttribute< FireAttribute >();
		cell.currentHeat_ = bound( 0, cell.currentHeat_ + fireAttribute.GetIncreaseRate() - fireAttribute.GetWeatherDecreateRate( *cell.pObject_ ), fireAttribute.GetMaxHeat() );
        cell.combustionEnergySum_ += cell.currentHeat_;
        ++cell.combustionEnergyCount_;
        cell.currentCombustionEnergy_ = cell.combustionEnergySum_ / cell.combustionEnergyCount_;
    }
    else
		cell.phase_ = sword::decline;

	if( cell.currentHeat_ == 0 )
		cell.phase_ = sword::extinguished;

    cell.bUpdated_ = true;
    cell.pObject_->GetAttribute< BurnSurfaceAttribute >().NotifyCellsUpdated();
}

// -----------------------------------------------------------------------------
// Name: MIL_BurningCells::UpdateCombustion
// Created: BCI 2010-12-20
// -----------------------------------------------------------------------------
void MIL_BurningCells::UpdateDecline( BurningCell& cell )
{
	FireAttribute& fireAttribute = cell.pObject_->GetAttribute< FireAttribute >();
	cell.currentHeat_ = bound( 0, cell.currentHeat_ - fireAttribute.GetDecreaseRate() - fireAttribute.GetWeatherDecreateRate( *cell.pObject_ ), fireAttribute.GetMaxHeat() );
    if( cell.currentHeat_ == 0 )
        cell.phase_ = sword::extinguished;

    cell.bUpdated_ = true;
    cell.pObject_->GetAttribute< BurnSurfaceAttribute >().NotifyCellsUpdated();
}

// -----------------------------------------------------------------------------
// Name: MIL_BurningCells::load
// Created: BCI 2010-12-21
// -----------------------------------------------------------------------------
void MIL_BurningCells::load( MIL_CheckPointInArchive&, MIL_Object_ABC&, const unsigned int )
{
	// $$$$ BCI 2010-12-21: todo
}
// -----------------------------------------------------------------------------
// Name: MIL_BurningCells::save
// Created: BCI 2010-12-21
// -----------------------------------------------------------------------------
void MIL_BurningCells::save( MIL_CheckPointOutArchive&, MIL_Object_ABC&, const unsigned int ) const
{
	// $$$$ BCI 2010-12-21: todo
}

// -----------------------------------------------------------------------------
// Name: MIL_BurningCells::SendFullState
// Created: BCI 2010-12-21
// -----------------------------------------------------------------------------
void MIL_BurningCells::SendFullState( sword::ObjectAttributes& asn, MIL_Object_ABC& object ) const
{
	SendState< eFull >( asn, object );
}

// -----------------------------------------------------------------------------
// Name: MIL_BurningCells::SendUpdate
// Created: BCI 2010-12-22
// -----------------------------------------------------------------------------
void MIL_BurningCells::SendUpdate( sword::ObjectAttributes& asn, MIL_Object_ABC& object ) const
{
	SendState< eUpdate >( asn, object );
}

// -----------------------------------------------------------------------------
// Name: template< bool bUpdate > void MIL_BurningCells::SendState
// Created: BCI 2010-12-22
// -----------------------------------------------------------------------------
template< MIL_BurningCells::SendStateMode mode >
void MIL_BurningCells::SendState( sword::ObjectAttributes& asn, MIL_Object_ABC& object ) const
{
	sword::ObjectAttributeBurnSurface& asnBurnSurface = *asn.mutable_burn_surface();
	asnBurnSurface.set_cell_size( object.GetAttribute< FireAttribute >().GetCellSize() );
	BurningCellsByObjectsMap::const_iterator itCells = burningCellsByObjects_.find( &object );
	if( itCells != burningCellsByObjects_.end() )
	{
		const BurningCellsVector& cells = itCells->second;
		for( BurningCellsVector::const_iterator it = cells.begin(); it != cells.end(); ++it )
		{
			BurningCell& cell = **it;
			if( mode == eUpdate )
			{
				if( !cell.bUpdated_ )
					continue;
				cell.bUpdated_ = false;
			}
			sword::ObjectAttributeBurnSurface::BurningCell& asnCell = *asnBurnSurface.add_burningcells();
			asnCell.set_origin_x( cell.origin_.X() );
			asnCell.set_origin_y( cell.origin_.Y() );
			asnCell.set_phase( cell.phase_ );
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
}

// -----------------------------------------------------------------------------
// Name: MIL_BurningCells::WriteODB
// Created: BCI 2010-12-21
// -----------------------------------------------------------------------------
void MIL_BurningCells::WriteODB( xml::xostream&, MIL_Object_ABC& ) const
{
	// $$$$ BCI 2010-12-21: todo
}
