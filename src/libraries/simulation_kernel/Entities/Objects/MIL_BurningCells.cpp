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
#include "MIL_AgentServer.h"
#include "MIL_FireClass.h"
#include "MIL_Object_ABC.h"
#include "FireAttribute.h"
#include "BurnSurfaceAttribute.h"
#include "FirePropagationModifierCapacity.h"
#include "simulation_terrain/TER_Analyzer.h"
#include "simulation_terrain/TER_World.h"
#include "meteo/Meteo.h"
#include "Tools/MIL_Tools.h"
#include "protocol/Protocol.h"
#include "CheckPoints/MIL_CheckPointInArchive.h"
#include "CheckPoints/MIL_CheckPointOutArchive.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Units/WoundEffectsHandler_ABC.h"
#include "Entities/Populations/MIL_PopulationElement_ABC.h"
#include "Entities/MIL_EntityManager.h"
#include "Meteo/PHY_MeteoDataManager.h"
#include "Meteo/RawVisionData/ElevationGrid.h"
#include "Meteo/RawVisionData/PHY_RawVisionData.h"
#include <spatialcontainer/TerrainData.h>
#pragma warning( push, 0 )
#pragma warning( disable: 4702 )
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/geometry/multi/geometries/multi_geometries.hpp>
#include <boost/geometry/algorithms/union.hpp>
#include <boost/geometry/geometries/register/point.hpp>
#pragma warning( pop )
#include <boost/range/algorithm_ext/erase.hpp>

BOOST_GEOMETRY_REGISTER_POINT_2D(MT_Vector2D, double, cs::cartesian, rX_, rY_)

// $$$$ BCI 2011-01-04: todo faire plusieurs classes en fonction des phases...
class MIL_BurningCell : private boost::noncopyable
{
public:
    void serialize( MIL_CheckPointOutArchive& ar, unsigned int )
    {
        ar << origin_.X()
           << origin_.Y()
           << phase_
           << ignitionThreshold_
           << maxCombustionEnergy_
           << ignitionEnergy_
           << currentHeat_
           << currentCombustionEnergy_
           << lastUpdateTime_;
    }
    void serialize( MIL_CheckPointInArchive& ar, unsigned int )
    {
        int x, y;
        ar >> x
           >> y
           >> phase_
           >> ignitionThreshold_
           >> maxCombustionEnergy_
           >> ignitionEnergy_
           >> currentHeat_
           >> currentCombustionEnergy_
           >> lastUpdateTime_;

        origin_.Set( x, y );
        double radius = MIL_FireClass::GetCellSize() / 2.;
        center_ = geometry::Point2d( x + radius, y + radius );
        bUpdated_ = false;
        bRequested_ = false;
    }

    MIL_BurningCellOrigin origin_;
    geometry::Point2d center_; // $$$$ BCI 2011-01-11: c'est qui le plus fort? geometry::Point2d ou MT_Vector2D?
    MIL_Object_ABC* pObject_;
    sword::EnumBurningCellPhase phase_;
    int ignitionThreshold_;
    int maxCombustionEnergy_;
    int ignitionEnergy_;
    int currentHeat_;
    int currentCombustionEnergy_;
    unsigned int lastUpdateTime_;
    bool bUpdated_;
    bool bRequested_;
};

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
MIL_BurningCell* MIL_BurningCells::FindCell( const MIL_BurningCellOrigin& coords ) const
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
MIL_BurningCellOrigin MIL_BurningCells::ComputeCellOriginFromPoint( double x, double y )
{
    //Compute cell aligned coordinates
    int alignedX = (int) x;
    int alignedY = (int) y;
    int cellSize = MIL_FireClass::GetCellSize();
    alignedX = alignedX - alignedX % cellSize;
    alignedY = alignedY - alignedY % cellSize;
    return MIL_BurningCellOrigin( alignedX, alignedY );
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
    int cellSize = MIL_FireClass::GetCellSize();
    MIL_BurningCellOrigin minOrigin = ComputeCellOriginFromPoint( minX, minY );
    MIL_BurningCellOrigin maxOrigin = ComputeCellOriginFromPoint( maxX, maxY );
    for( int x = minOrigin.X(); x < maxOrigin.X(); x += cellSize )
        for( int y = minOrigin.Y(); y < maxOrigin.Y(); y += cellSize )
            StartBurn( MIL_BurningCellOrigin( x, y ), object );
}

// -----------------------------------------------------------------------------
// Name: MIL_BurningCells::StartBurn
// Created: BCI 2010-12-20
// -----------------------------------------------------------------------------
void MIL_BurningCells::StartBurn( const MIL_BurningCellOrigin& cellOrigin, MIL_Object_ABC& object )
{
    int cellSize = MIL_FireClass::GetCellSize();

    //init cell and his neighbours
    InitCell( cellOrigin, object, sword::combustion );
    InitCell( MIL_BurningCellOrigin( cellOrigin.X() - cellSize, cellOrigin.Y() - cellSize ), object, sword::pre_ignition );
    InitCell( MIL_BurningCellOrigin( cellOrigin.X() + cellSize, cellOrigin.Y() + cellSize ), object, sword::pre_ignition );
    InitCell( MIL_BurningCellOrigin( cellOrigin.X() - cellSize, cellOrigin.Y() + cellSize ), object, sword::pre_ignition );
    InitCell( MIL_BurningCellOrigin( cellOrigin.X() + cellSize, cellOrigin.Y() - cellSize ), object, sword::pre_ignition );
    InitCell( MIL_BurningCellOrigin( cellOrigin.X(), cellOrigin.Y() - cellSize ), object, sword::pre_ignition );
    InitCell( MIL_BurningCellOrigin( cellOrigin.X(), cellOrigin.Y() + cellSize ), object, sword::pre_ignition );
    InitCell( MIL_BurningCellOrigin( cellOrigin.X() - cellSize, cellOrigin.Y() ), object, sword::pre_ignition );
    InitCell( MIL_BurningCellOrigin( cellOrigin.X() + cellSize, cellOrigin.Y() ), object, sword::pre_ignition );
}

// -----------------------------------------------------------------------------
// Name: MIL_BurningCells::InitCell
// Created: BCI 2010-12-20
// -----------------------------------------------------------------------------
void MIL_BurningCells::InitCell( const MIL_BurningCellOrigin& cellOrigin, MIL_Object_ABC& object, sword::EnumBurningCellPhase phase )
{
    MIL_BurningCell* pCell = FindCell( cellOrigin );
    bool isNew;
    if( pCell )
    {
        //si la cellule existe déjà, on ne peut que la faire brûler si elle ne brûle pas encore
        if( phase != sword::combustion || pCell->phase_ == sword::combustion )
            return;

        isNew = false;
    }
    else
    {
        //ne brûler que le terrain
        if( !TER_World::GetWorld().GetExtent().IsInside( MT_Vector2D( cellOrigin.X(), cellOrigin.Y() ) ) )
            return;

        //nouvelle cellule
        pCell = new MIL_BurningCell();
        pCell->origin_ = cellOrigin;
        isNew = true;
    }

    //initialise la cellule
    double radius = MIL_FireClass::GetCellSize() / 2.;
    pCell->center_ = geometry::Point2d( cellOrigin.X() + radius, cellOrigin.Y() + radius );
    pCell->pObject_ = &object;
    pCell->phase_ = phase;
    TerrainData terrainData;
    FindTerrainData( pCell->center_, (float) radius, terrainData );
    const FireAttribute& fireAttribute = object.GetAttribute< FireAttribute >();
    fireAttribute.GetSurfaceFirePotentials( terrainData, pCell->ignitionThreshold_, pCell->maxCombustionEnergy_ );
    for( auto it = propagationModifierObjects_.begin(); it != propagationModifierObjects_.end(); ++it )
    {
        if( (*it)->GetLocalisation().IsInside( MT_Vector2D( pCell->center_.X(), pCell->center_.Y() ) ) )
            (*it)->Get< FirePropagationModifierCapacity >().Modify( fireAttribute.GetClass(), pCell->ignitionThreshold_, pCell->maxCombustionEnergy_ );
    }

    // $$$$ BCI 2011-04-05: avoid useless cell creation
    if( isNew )
    {
        if( pCell->maxCombustionEnergy_ > 0 )
        {
            burningCellsByCoordinates_[ cellOrigin ] = pCell;
            burningCellsByObjects_[ object.GetID() ].push_back( pCell );
        }
        else
        {
            delete pCell;
            return;
        }
    }

    pCell->ignitionEnergy_ = 0;
    pCell->currentHeat_ = phase == sword::combustion ? object.GetAttribute< FireAttribute >().GetInitialHeat() : 0;
    pCell->currentCombustionEnergy_ = 0;
    pCell->lastUpdateTime_ = std::numeric_limits< unsigned int >::max();
    pCell->bUpdated_ = true;
    pCell->bRequested_ = false;
    object.GetAttribute< BurnSurfaceAttribute >().NotifyCellsUpdated();
}

// -----------------------------------------------------------------------------
// Name: MIL_BurningCells::FindTerrainData
// Created: BCI 2011-03-04
// -----------------------------------------------------------------------------
void MIL_BurningCells::FindTerrainData( const geometry::Point2d& center, float radius, TerrainData& data )
{
    data = TER_Analyzer::GetAnalyzer().FindTerrainDataWithinCircle( MT_Vector2D( center.X(), center.Y() ), radius );

    // $$$$ BCI 2011-04-05: grosse bidouille pour trouver un semblant de type de terrain quand le pathfind n'assure pas...
    const TerrainData unknow;
    if( data == unknow )
    {
        envBits e = MIL_AgentServer::GetWorkspace().GetMeteoDataManager().GetRawVisionData()( center.X(), center.Y() ).GetEnv();
        if( e & PHY_RawVisionData::eVisionForest )
            data.Merge( data.Forest() );
        if( e & PHY_RawVisionData::eVisionUrban )
            data.Merge( data.Urban() );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_BurningCells::Update
// Created: BCI 2010-12-20
// -----------------------------------------------------------------------------
void MIL_BurningCells::Update( MIL_Object_ABC& object, unsigned int time )
{
    //retrieve cells to update (copy vector to avoid infinite loop)
    BurningCellsVector cellsToUpdate = burningCellsByObjects_[ object.GetID() ];

    //update cells
    for( BurningCellsVector::iterator it = cellsToUpdate.begin(); it != cellsToUpdate.end(); ++it )
    {
        MIL_BurningCell& cell = **it;
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
    typedef bg::model::polygon< MT_Vector2D > polygon_2d;
    typedef bg::model::multi_polygon< polygon_2d > multi_polygon_2d;
    typedef bg::model::box< MT_Vector2D > box_2d;
    int cellSize = MIL_FireClass::GetCellSize();
    const BurningCellsVector& cells = burningCellsByObjects_[ object.GetID() ];
    polygon_2d poly;
    bg::assign_points( poly, object.GetLocalisation().GetPoints() );
    for( ; lastCellIndexIncludedInLocalization_<cells.size(); ++lastCellIndexIncludedInLocalization_ )
    {
        MIL_BurningCell& cell = *cells[ lastCellIndexIncludedInLocalization_ ];
        box_2d cellBox( MT_Vector2D( cell.origin_.X(), cell.origin_.Y() ), MT_Vector2D( cell.origin_.X() + cellSize, cell.origin_.Y() + cellSize ) );
        multi_polygon_2d ps;
        bg::union_( cellBox, poly, ps );
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
void MIL_BurningCells::UpdatePreIgnition( MIL_BurningCell& cell, unsigned int time )
{
    int cellSize = MIL_FireClass::GetCellSize();
    unsigned int timeElapsed = time - cell.lastUpdateTime_;
    PropagateIgnition( MIL_BurningCellOrigin( cell.origin_.X() - cellSize, cell.origin_.Y() - cellSize ), cell, timeElapsed );
    PropagateIgnition( MIL_BurningCellOrigin( cell.origin_.X() + cellSize, cell.origin_.Y() + cellSize ), cell, timeElapsed );
    PropagateIgnition( MIL_BurningCellOrigin( cell.origin_.X() - cellSize, cell.origin_.Y() + cellSize ), cell, timeElapsed );
    PropagateIgnition( MIL_BurningCellOrigin( cell.origin_.X() + cellSize, cell.origin_.Y() - cellSize ), cell, timeElapsed );
    PropagateIgnition( MIL_BurningCellOrigin( cell.origin_.X(), cell.origin_.Y() + cellSize ), cell, timeElapsed );
    PropagateIgnition( MIL_BurningCellOrigin( cell.origin_.X(), cell.origin_.Y() - cellSize ), cell, timeElapsed );
    PropagateIgnition( MIL_BurningCellOrigin( cell.origin_.X() - cellSize, cell.origin_.Y() ), cell, timeElapsed );
    PropagateIgnition( MIL_BurningCellOrigin( cell.origin_.X() + cellSize, cell.origin_.Y() ), cell, timeElapsed );
    if( cell.ignitionEnergy_ >= cell.ignitionThreshold_ )
        StartBurn( cell.origin_, *cell.pObject_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_BurningCells::PropagateIgnition
// Created: BCI 2010-12-20
// -----------------------------------------------------------------------------
void MIL_BurningCells::PropagateIgnition( const MIL_BurningCellOrigin& fromOrigin, MIL_BurningCell& cellTo, unsigned int timeElapsed )
{
    if( const MIL_BurningCell* pCellFrom = FindCell( fromOrigin ) )
    {
        geometry::Vector2d direction( pCellFrom->center_, cellTo.center_ );
        const weather::WindData& wind = MIL_Tools::GetWind( MT_Vector2D( pCellFrom->center_.X(), pCellFrom->center_.Y() ) );
        if( wind.rSpeed_ > 0.0 )
        {
            double windDirectionScalarProduct = direction.DotProduct( geometry::Vector2d( wind.vDirection_.rX_, wind.vDirection_.rY_ ) );
            int ignitionTransfered = int( pCellFrom->currentHeat_ * timeElapsed * std::max( windDirectionScalarProduct, 0.0 ) / ( std::abs( windDirectionScalarProduct )+ wind.rSpeed_ ) );
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
void MIL_BurningCells::UpdateCombustion( MIL_BurningCell& cell )
{
    if( cell.currentCombustionEnergy_ < cell.maxCombustionEnergy_ )
    {
        FireAttribute& fireAttribute = cell.pObject_->GetAttribute< FireAttribute >();
        cell.currentHeat_ = bound( 0, cell.currentHeat_ + fireAttribute.GetIncreaseRate() - fireAttribute.GetWeatherDecreateRate( *cell.pObject_ ), fireAttribute.GetMaxHeat() );
        cell.currentCombustionEnergy_ += cell.currentHeat_;
    }
    else
    {
        cell.phase_ = sword::decline;
        cell.bUpdated_ = true;
        cell.pObject_->GetAttribute< BurnSurfaceAttribute >().NotifyCellsUpdated();
    }

    if( cell.currentHeat_ == 0 )
    {
        cell.phase_ = sword::extinguished;
        cell.bUpdated_ = true;
        cell.pObject_->GetAttribute< BurnSurfaceAttribute >().NotifyCellsUpdated();
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_BurningCells::UpdateCombustion
// Created: BCI 2010-12-20
// -----------------------------------------------------------------------------
void MIL_BurningCells::UpdateDecline( MIL_BurningCell& cell )
{
    FireAttribute& fireAttribute = cell.pObject_->GetAttribute< FireAttribute >();
    cell.currentHeat_ = bound( 0, cell.currentHeat_ - fireAttribute.GetDecreaseRate() - fireAttribute.GetWeatherDecreateRate( *cell.pObject_ ), fireAttribute.GetMaxHeat() );
    if( cell.currentHeat_ == 0 )
    {
        cell.phase_ = sword::extinguished;
        cell.bUpdated_ = true;
        cell.pObject_->GetAttribute< BurnSurfaceAttribute >().NotifyCellsUpdated();
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_BurningCells::load
// Created: BCI 2011-01-07
// -----------------------------------------------------------------------------
void MIL_BurningCells::load( MIL_CheckPointInArchive& ar )
{
    ar >> lastCellIndexIncludedInLocalization_;
}

// -----------------------------------------------------------------------------
// Name: MIL_BurningCells::save
// Created: BCI 2011-01-07
// -----------------------------------------------------------------------------
void MIL_BurningCells::save( MIL_CheckPointOutArchive& ar ) const
{
    ar << lastCellIndexIncludedInLocalization_;
}

// -----------------------------------------------------------------------------
// Name: MIL_BurningCells::finalizeLoad
// Created: BCI 2011-01-07
// -----------------------------------------------------------------------------
void MIL_BurningCells::finalizeLoad( MIL_EntityManager& entityManager )
{
    for( BurningCellsByObjectsMap::iterator it = burningCellsByObjects_.begin(); it != burningCellsByObjects_.end(); ++it )
    {
        MIL_Object_ABC* pObject = entityManager.FindObject( it->first );
        if( !pObject )
            throw MASA_EXCEPTION( "cannot find object with id " + boost::lexical_cast< std::string >( it->first ) );

        BurningCellsVector& cells = it->second;
        for( BurningCellsVector::iterator itCell = cells.begin(); itCell != cells.end(); ++itCell )
            (*itCell)->pObject_ = pObject;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_BurningCells::load
// Created: BCI 2010-12-21
// -----------------------------------------------------------------------------
void MIL_BurningCells::load( MIL_CheckPointInArchive& ar, unsigned int objectId, const unsigned int )
{
    BurningCellsVector& cells = burningCellsByObjects_[ objectId ];
    ar >> cells;
    for( BurningCellsVector::iterator it = cells.begin(); it != cells.end(); ++it )
    {
        MIL_BurningCell& cell = **it;
        burningCellsByCoordinates_[ cell.origin_ ] = &cell;
    }

}
// -----------------------------------------------------------------------------
// Name: MIL_BurningCells::save
// Created: BCI 2010-12-21
// -----------------------------------------------------------------------------
void MIL_BurningCells::save( MIL_CheckPointOutArchive& ar, unsigned int objectId, const unsigned int ) const
{
    BurningCellsByObjectsMap::const_iterator it = burningCellsByObjects_.find( objectId );
    if( it != burningCellsByObjects_.end() )
    {
        const BurningCellsVector& cells = it->second;
        ar << cells;
    }
    else
    {
        BurningCellsVector emptyVector;
        ar << emptyVector;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_BurningCells::SendFullState
// Created: BCI 2010-12-21
// -----------------------------------------------------------------------------
void MIL_BurningCells::SendFullState( sword::ObjectAttributes& asn, MIL_Object_ABC& object ) const
{
    SendState( asn, object, eFull );
}

// -----------------------------------------------------------------------------
// Name: MIL_BurningCells::SendUpdate
// Created: BCI 2010-12-22
// -----------------------------------------------------------------------------
void MIL_BurningCells::SendUpdate( sword::ObjectAttributes& asn, MIL_Object_ABC& object ) const
{
    SendState( asn, object, eUpdate );
}

// -----------------------------------------------------------------------------
// Name: template< bool bUpdate > void MIL_BurningCells::SendState
// Created: BCI 2010-12-22
// -----------------------------------------------------------------------------
void MIL_BurningCells::SendState( sword::ObjectAttributes& asn, MIL_Object_ABC& object, MIL_BurningCells::SendStateMode mode ) const
{
    sword::ObjectAttributeBurnSurface& asnBurnSurface = *asn.mutable_burn_surface();
    asnBurnSurface.set_cell_size( object.GetAttribute< FireAttribute >().GetCellSize() );
    BurningCellsByObjectsMap::const_iterator itCells = burningCellsByObjects_.find( object.GetID() );
    if( itCells != burningCellsByObjects_.end() )
    {
        const BurningCellsVector& cells = itCells->second;
        for( BurningCellsVector::const_iterator it = cells.begin(); it != cells.end(); ++it )
        {
            MIL_BurningCell& cell = **it;
            if( mode == eUpdate )
            {
                if( !cell.bUpdated_ )
                    continue;
                cell.bUpdated_ = false;
            }
            sword::ObjectAttributeBurnSurface::BurningCell& asnCell = *asnBurnSurface.add_burning_cells();
            asnCell.set_origin_x( cell.origin_.X() );
            asnCell.set_origin_y( cell.origin_.Y() );
            asnCell.set_phase( cell.phase_ );

            if( cell.bRequested_ )
            {
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
                        asnCell.mutable_combustion()->set_combustion_energy( cell.currentCombustionEnergy_ );
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
                cell.bRequested_ = false;
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

// -----------------------------------------------------------------------------
// Name: MIL_BurningCells::ComputePathCost
// Created: BCI 2011-01-05
// -----------------------------------------------------------------------------
double MIL_BurningCells::ComputePathCost( const MT_Vector2D& from, const MT_Vector2D& to ) const
{
    if( IsTrafficable( from, to ) )
        return std::numeric_limits< double >::min();
    else
        return -1.;
}

// -----------------------------------------------------------------------------
// Name: MIL_BurningCells::IsTrafficable
// Created: BCI 2011-01-06
// -----------------------------------------------------------------------------
bool MIL_BurningCells::IsTrafficable( const MT_Vector2D& from, const MT_Vector2D& to ) const
{
    if( burningCellsByCoordinates_.empty() )
        return true;
    const MT_Line line( from, to );
    int cellSize = MIL_FireClass::GetCellSize();
    MIL_BurningCellOrigin minOrigin = ComputeCellOriginFromPoint( std::min( from.rX_, to.rX_ ), std::min( from.rY_, to.rY_ ) );
    MIL_BurningCellOrigin maxOrigin = ComputeCellOriginFromPoint( std::max( from.rX_, to.rX_ ), std::max( from.rY_, to.rY_ ) );
    for( int x = minOrigin.X(); x <= maxOrigin.X(); x += cellSize )
    {
        for( int y = minOrigin.Y(); y <= maxOrigin.Y(); y += cellSize )
        {
            BurningCellsByCoordinatesMap::const_iterator it = burningCellsByCoordinates_.find( MIL_BurningCellOrigin( x, y ) );
            if( it != burningCellsByCoordinates_.end() )
            {
                if( it->second->phase_ == sword::combustion || it->second->phase_ == sword::decline )
                {
                    MT_Rect rect( x, y, x + cellSize, y + cellSize );
                    if( rect.Intersect2D( line ) )
                        return false;
                }
            }
        }
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_BurningCells::BurnAgent
// Created: BCI 2011-01-06
// -----------------------------------------------------------------------------
void MIL_BurningCells::BurnAgent( MIL_Object_ABC& object, MIL_Agent_ABC& agent )
{
    MT_Vector2D position = agent.GetRole< PHY_RoleInterface_Location >().GetPosition();
    BurningCellsByCoordinatesMap::const_iterator it = burningCellsByCoordinates_.find( ComputeCellOriginFromPoint( position.rX_, position.rY_ ) );
    if( it != burningCellsByCoordinates_.end() )
    {
        if( it->second->phase_ == sword::combustion || it->second->phase_ == sword::decline )
        {
            MIL_BurnEffectManipulator manipulator( object.GetAttribute< FireAttribute >().GetBurnEffect() );
            agent.Apply( &WoundEffectsHandler_ABC::ApplyEffect, manipulator );
        }
    }
}
// -----------------------------------------------------------------------------
// Name: MIL_BurningCells::BurnPopulation
// Created: BCI 2011-01-06
// -----------------------------------------------------------------------------
void MIL_BurningCells::BurnPopulation( MIL_Object_ABC& object, MIL_PopulationElement_ABC& population )
{
    const MT_Rect& boundingBox = population.GetLocation().GetBoundingBox();
    double minX = boundingBox.GetLeft();
    double minY = boundingBox.GetBottom();
    double maxX = boundingBox.GetRight();
    double maxY = boundingBox.GetTop();
    int cellSize = MIL_FireClass::GetCellSize();
    MIL_BurningCellOrigin minOrigin = ComputeCellOriginFromPoint( minX, minY );
    MIL_BurningCellOrigin maxOrigin = ComputeCellOriginFromPoint( maxX, maxY );
    for( int x = minOrigin.X(); x <= maxOrigin.X(); x += cellSize )
    {
        for( int y = minOrigin.Y(); y <= maxOrigin.Y(); y += cellSize )
        {
            BurningCellsByCoordinatesMap::const_iterator it = burningCellsByCoordinates_.find( MIL_BurningCellOrigin( x, y ) );
            if( it != burningCellsByCoordinates_.end() )
            {
                if( it->second->phase_ == sword::combustion || it->second->phase_ == sword::decline )
                {
                    population.ApplyBurn( object.GetAttribute< FireAttribute >().GetBurnEffect() );
                    return;
                }
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_BurningCells::OnRequest
// Created: BCI 2011-01-10
// -----------------------------------------------------------------------------
void MIL_BurningCells::OnRequest( double x, double y )
{
    MIL_BurningCellOrigin origin = ComputeCellOriginFromPoint( x, y );
    BurningCellsByCoordinatesMap::const_iterator it = burningCellsByCoordinates_.find( origin );
    if( it != burningCellsByCoordinates_.end() )
    {
        MIL_BurningCell& cell = *it->second;
        cell.bUpdated_ = true;
        cell.bRequested_ = true;
        cell.pObject_->GetAttribute< BurnSurfaceAttribute >().NotifyCellsUpdated();
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_BurningCells::StartModifyBurn
// Created: BCI 2011-01-11
// -----------------------------------------------------------------------------
void MIL_BurningCells::StartModifyBurn( MIL_Object_ABC& object )
{
    propagationModifierObjects_.push_back( &object );
}

// -----------------------------------------------------------------------------
// Name: MIL_BurningCells::StopModifyBurn
// Created: BCI 2011-01-11
// -----------------------------------------------------------------------------
void MIL_BurningCells::StopModifyBurn( MIL_Object_ABC& object )
{
    boost::remove_erase( propagationModifierObjects_, &object );
}
