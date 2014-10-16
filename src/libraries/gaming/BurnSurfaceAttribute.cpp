// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "BurnSurfaceAttribute.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Tools.h"
#include "protocol/ServerPublisher_ABC.h"
#include "protocol/SimulationSenders.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: BurnSurfaceAttribute constructor
// Created: BCI 2010-12-21
// -----------------------------------------------------------------------------
BurnSurfaceAttribute::BurnSurfaceAttribute( Controller& controller, Publisher_ABC& publisher, kernel::Entity_ABC& entity )
    : controller_( controller )
    , cellSize_( 0 )
    , publisher_( publisher )
    , entity_( entity )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BurnSurfaceAttribute destructor
// Created: BCI 2010-12-21
// -----------------------------------------------------------------------------
BurnSurfaceAttribute::~BurnSurfaceAttribute()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: BurnSurfaceAttribute::UpdateData
// Created: BCI 2010-12-21
// -----------------------------------------------------------------------------
template< typename T >
void BurnSurfaceAttribute::UpdateData( const T& message )
{
    if( message.has_burn_surface()  )
    {
        cellSize_ = message.burn_surface().cell_size();
        for( int i=0, nbCells = message.burn_surface().burning_cells_size(); i<nbCells; ++i )
            UpdateBurningCellData( message.burn_surface().burning_cells( i ) );
        controller_.Update( *(BurnSurfaceAttribute_ABC*)this );
    }
}

// -----------------------------------------------------------------------------
// Name: BurnSurfaceAttribute::UpdateBurningCellData
// Created: BCI 2010-12-21
// -----------------------------------------------------------------------------
template< typename T >
void BurnSurfaceAttribute::UpdateBurningCellData( const T& asnCell )
{
    BurningCell& cell = burningCellsByCoordinates_[ BurningCellOrigin( asnCell.origin_x(), asnCell.origin_y() ) ];
    cell.phase_ = asnCell.phase();
    if( asnCell.has_pre_ignition() )
    {
        cell.ignitionEnergy_ = asnCell.pre_ignition().ignition_energy();
        cell.ignitionThreshold_ = asnCell.pre_ignition().ignition_threshold();
    }
    if( asnCell.has_combustion() )
    {
        cell.combustionEnergy_ =  asnCell.combustion().combustion_energy();
        cell.currentHeat_ = asnCell.combustion().current_heat();
        cell.maxCombustionEnergy_ = asnCell.combustion().max_combustion_energy();
    }
    if( asnCell.has_decline() )
        cell.currentHeat_ = asnCell.decline().current_heat();
}

// -----------------------------------------------------------------------------
// Name: BurnSurfaceAttribute::DoUpdate
// Created: BCI 2010-12-21
// -----------------------------------------------------------------------------
void BurnSurfaceAttribute::DoUpdate( const sword::ObjectKnowledgeUpdate& message )
{
    UpdateData( message.attributes() );
}

// -----------------------------------------------------------------------------
// Name: BurnSurfaceAttribute::DoUpdate
// Created: BCI 2010-12-21
// -----------------------------------------------------------------------------
void BurnSurfaceAttribute::DoUpdate( const sword::ObjectUpdate& message )
{
    UpdateData( message.attributes() );
}

// -----------------------------------------------------------------------------
// Name: BurnSurfaceAttribute::Draw
// Created: BCI 2010-12-21
// -----------------------------------------------------------------------------
void BurnSurfaceAttribute::Draw( const geometry::Point2f& /*where*/, const gui::Viewport_ABC& /*viewport*/, gui::GLView_ABC& /*tools*/ ) const
{
    // $$$$ BCI 2011-01-07: todo ne dessiner que ce qui est visible
    for( BurningCellsByCoordinatesMap::const_iterator it = burningCellsByCoordinates_.begin(); it != burningCellsByCoordinates_.end(); ++it )
    {
        const BurningCell& cell = it->second;
        switch( cell.phase_ )
        {
        case sword::pre_ignition:
            glColor4ub( 255, 255, 128, 128 );
            break;
        case sword::combustion:
            glColor4ub( 255, 0, 0, 128 );
            break;
        case sword::decline:
            glColor4ub( 255, 128, 0, 128 );
            break;
        case sword::extinguished:
            glColor4ub( 128, 64, 64, 128 );
            break;
        }
        const BurningCellOrigin& origin = it->first;
        glBegin( GL_QUADS );
        glVertex2i( origin.X(), origin.Y() );
        glVertex2i( origin.X(), origin.Y() + cellSize_ );
        glVertex2i( origin.X() + cellSize_, origin.Y() + cellSize_ );
        glVertex2i( origin.X() + cellSize_, origin.Y() );
        glEnd();
    }

}

// -----------------------------------------------------------------------------
// Name: BurnSurfaceAttribute::TerrainPicked
// Created: BCI 2011-01-04
// -----------------------------------------------------------------------------
void BurnSurfaceAttribute::TerrainPicked( const geometry::Point2f& terrainCoordinates, QStringList& outputInfos )
{
    BurningCellsByCoordinatesMap::const_iterator it = burningCellsByCoordinates_.find( ComputeCellOriginFromPoint( terrainCoordinates ) );
    if( it != burningCellsByCoordinates_.end() )
    {
        const BurningCell& cell = it->second;

        //format current data
        switch( cell.phase_ )
        {
        case sword::pre_ignition:
            if( cell.ignitionEnergy_ >= 0 && cell.ignitionThreshold_ >= 0 )
                outputInfos.push_back( tools::translate( "Object", "Cell ignition:  %L1/%L2" ).arg( cell.ignitionEnergy_ ).arg( cell.ignitionThreshold_ ) );
            break;
        case sword::combustion:
            if( cell.combustionEnergy_ >= 0 && cell.maxCombustionEnergy_ >= 0 && cell.currentHeat_ >= 0 )
                outputInfos.push_back( tools::translate( "Object", "Cell combustion: %L1/%L2, heat %L3" ).arg( cell.combustionEnergy_ ).arg( cell.maxCombustionEnergy_ ).arg( cell.currentHeat_ ) );
            break;
        case sword::decline:
            if( cell.currentHeat_ >= 0 )
                outputInfos.push_back( tools::translate( "Object", "Cell decline: heat %L1" ).arg( cell.currentHeat_ ) );
            break;
        case sword::extinguished:
            outputInfos.push_back( tools::translate( "Object", "Cell extinguished" ) );
            break;
        }

        simulation::BurningCellRequest message;
        message().mutable_object()->set_id( entity_.GetId() );
        message().set_x( terrainCoordinates.X() );
        message().set_y( terrainCoordinates.Y() );
        message.Send( publisher_ );
    }
}

// -----------------------------------------------------------------------------
// Name: BurnSurfaceAttribute::Display
// Created: BCI 2011-01-04
// -----------------------------------------------------------------------------
void BurnSurfaceAttribute::Display( kernel::Displayer_ABC& displayer ) const
{
    displayer.Clear();
    Displayer_ABC& burnSurfaceGroup = displayer.Group( tools::translate( "Object", "Burn surface" ) );
    burnSurfaceGroup.Display( tools::translate( "Object", "Nb burning cells:" ), burningCellsByCoordinates_.size() );
}

// -----------------------------------------------------------------------------
// Name: BurnSurfaceAttribute::DisplayInSummary
// Created: BCI 2011-01-04
// -----------------------------------------------------------------------------
void BurnSurfaceAttribute::DisplayInSummary( kernel::Displayer_ABC& displayer ) const
{
    Display( displayer );
}

// -----------------------------------------------------------------------------
// Name: BurnSurfaceAttribute::ComputeCellOriginFromPoint
// Created: BCI 2011-01-04
// -----------------------------------------------------------------------------
BurnSurfaceAttribute::BurningCellOrigin BurnSurfaceAttribute::ComputeCellOriginFromPoint( const geometry::Point2f& terrainCoordinates ) const
{
    //Compute cell aligned coordinates
    int alignedX = (int) terrainCoordinates.X();
    int alignedY = (int) terrainCoordinates.Y();
    alignedX = alignedX - alignedX % cellSize_;
    alignedY = alignedY - alignedY % cellSize_;
    return BurningCellOrigin( alignedX, alignedY );
}
