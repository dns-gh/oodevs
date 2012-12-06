// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "geostore_pch.h"
#include "CreateBlockAutoProcess.h"
#include "Database.h"
#include "GeometryFactory.h"
#include "GeoTable.h"
#include "SpatialRequestStatus.h"
#include "terrain/PointProjector_ABC.h"
#include "preparation/UrbanModel.h"
#include "preparation/UrbanFactory_ABC.h"
#include "clients_kernel/UrbanObject_ABC.h"
#include "clients_kernel/UrbanPositions_ABC.h"
#include <math.h>

using namespace geostore;
using namespace geometry;

// -----------------------------------------------------------------------------
// Name: CreateBlockAutoProcess constructor
// Created: AME 2010-08-02
// -----------------------------------------------------------------------------
CreateBlockAutoProcess::CreateBlockAutoProcess( const Database& database, const SpatialIndexer& index, PointProjector_ABC& projector, double roadWidth )
    : database_         ( database )
    , index_            ( index )
    , projector_        ( projector )
    , roadWidth_        ( roadWidth )
    , geometryFactory_  ( new GeometryFactory() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CreateBlockAutoProcess destructor
// Created: AME 2010-08-02
// -----------------------------------------------------------------------------
CreateBlockAutoProcess::~CreateBlockAutoProcess()
{
    // NOTHING
}

void CreateBlockAutoProcess::Run( const geometry::Polygon2f& footprint, std::vector< Polygon2f >& blocks )
{
    gaiaGeomCollPtr poly        = geometryFactory_->CreatePolygonGeometry( footprint , projector_ );
    gaiaGeomCollPtr buildings   = 0;
    gaiaGeomCollPtr urbans      = 0;

    // Compute the road width
    double lat0, lon0, lat1, lon1;
    projector_.Unproject( Point2d( 0.0, 0.0 ), lat0, lon0 );
    projector_.Unproject( Point2d( 0.0 + roadWidth_, 0.0 ), lat1, lon1 );
    roadWidth_ = std::abs( lon1 - lon0 );

    Database::CIT_Tables it;

    // Find intersecting urban areas
    it = database_.tables_.find( "urban" );
    if( it != database_.tables_.end() )
    {
        urbans = GeometryFactory::Validate( it->second->GetFeaturesIntersectingWith( poly ) );
    }

    if( ! urbans )
    {
        return;
    }

    // Find intersecting buildings
    it = database_.tables_.find( "building" );
    if( it != database_.tables_.end() )
    {
        buildings = GeometryFactory::Validate( it->second->GetFeaturesIntersectingWith( poly ) );
    }

    gaiaGeomCollPtr lines = 0;
    gaiaGeomCollPtr areas = 0;
    // Fill areas with surface elements and lines with linear elements
    ExtractTerrainComponents( poly, areas, lines );
    gaiaFreeGeomColl( poly );
    // Clip urban areas with the areas and lines, yielding the discretized urban blocks
    gaiaGeomCollPtr urbanBlocks = SubstractTerrainComponentsFromAreas( urbans, areas, lines );
    gaiaFreeGeomColl( areas );
    gaiaFreeGeomColl( urbans );
    gaiaFreeGeomColl( lines );
    // Remove blocks that intersect buildings
    ClipBlocksWithCollection( urbanBlocks, buildings );
    gaiaFreeGeomColl( buildings );
    // Remove blocks that intersect with existing urban blocks in the index/model
    gaiaGeomCollPtr blocksFromIndex = GeometryFactory::Validate( GetUrbanBlocksInAreaFromIndex( urbanBlocks, footprint ) );
    ClipBlocksWithCollection( urbanBlocks, blocksFromIndex );
    gaiaFreeGeomColl( blocksFromIndex );

    // Populate the resulting polygons vector
    blocks.clear();
    FillPolygonVector( urbanBlocks, blocks );
}

// -----------------------------------------------------------------------------
// Name: CreateBlockAutoProcess::Run
// Created: AME 2010-08-02
// -----------------------------------------------------------------------------
/*void CreateBlockAutoProcess::Run( const Polygon2f& footprint, UrbanModel& model, kernel::UrbanObject_ABC& parent, PointProjector_ABC& projector )
{
    gaiaGeomCollPtr poly        = geometryFactory_->CreatePolygonGeometry( footprint , projector );
    gaiaGeomCollPtr buildings   = 0;
    gaiaGeomCollPtr urbans      = 0;

    // Compute the road width
    double lat0, lon0, lat1, lon1;
    projector.Unproject( Point2d( 0.0, 0.0 ), lat0, lon0 );
    projector.Unproject( Point2d( 0.0 + roadWidth_, 0.0 ), lat1, lon1 );
    roadWidth_ = std::abs( lon1 - lon0 );

    Database::CIT_Tables it;

    // Find intersecting urban areas
    it = database_.tables_.find( "urban" );
    if( it != database_.tables_.end() )
    {
        urbans = GeometryFactory::Validate( it->second->GetFeaturesIntersectingWith( poly ) );
    }

    if( ! urbans )
    {
        return;
    }

    // Find intersecting buildings
    it = database_.tables_.find( "building" );
    if( it != database_.tables_.end() )
    {
        buildings = GeometryFactory::Validate( it->second->GetFeaturesIntersectingWith( poly ) );
    }

    gaiaGeomCollPtr areas = 0;
    gaiaGeomCollPtr lines = 0;
    PrepareTerrainComponents( poly, areas, lines );
    gaiaFreeGeomColl( poly );

    ClipUrbanAreaWithTerrainComponents( lines, areas, urbans );

    gaiaFreeGeomColl( areas );
    gaiaFreeGeomColl( urbans );

    // Update building tables
    UpdateBuildings( buildings );

    // Update urban blocks
    UpdateUrbanModel( model, parent, projector, footprint );
}*/

// -----------------------------------------------------------------------------
// Name: CreateBlockAutoProcess::PrepareTerrainComponent
// Created: AME 2010-08-02
// -----------------------------------------------------------------------------
void CreateBlockAutoProcess::ExtractTerrainComponents( gaiaGeomCollPtr footprint, gaiaGeomCollPtr& areas, gaiaGeomCollPtr& lines )
{
    // Initialize...
    areas = 0;
    lines = 0;

    for( Database::CIT_Tables it = database_.tables_.begin(); it != database_.tables_.end(); ++it )
    {
        if( it->first == "urban" || it->first == "building" )
        {
            continue;
        }

        if( it->second->GetGeometryType() == GeoTable::Polygon ) // Aggreagation des surfaces;
        {
            gaiaGeomCollPtr inter = GeometryFactory::Validate( it->second->GetFeaturesIntersectingWith( footprint ) );
            if( ! inter )
            {
                continue;
            }

            if( ! areas )
            {
                // Set the initial areas.
                areas = inter;
            }
            else
            {
                // Incorporate the intersection in the areas.
                gaiaGeomCollPtr merged = GeometryFactory::Validate( gaiaGeometryUnion( inter, areas ) );
                gaiaFreeGeomColl( inter );
                if( merged )
                {
                    gaiaFreeGeomColl( areas );
                    areas = merged;
                }
            }
        }
        else if( it->second->GetGeometryType() == GeoTable::LineString ) 
        {
            gaiaGeomCollPtr inter = GeometryFactory::Validate( it->second->GetFeaturesIntersectingWith( footprint ) );
            if( ! inter )
            {
                continue;
            }

            gaiaGeomCollPtr temp = gaiaGeomCollBuffer( inter, roadWidth_, 3 );
            gaiaFreeGeomColl( inter );
            if( ! lines )
            {
                // Set the initial buffers
                lines = temp;
            }
            else
            {
                // Merge
                gaiaGeomCollPtr merged = gaiaGeometryUnion( lines, temp );
                gaiaFreeGeomColl( lines );
                gaiaFreeGeomColl( temp );
                lines = merged;
            }
        }
        else
        {
            throw std::runtime_error( __FUNCTION__ );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: CreateBlockAutoProcess::ClippingUrbanAreaWithTerrainComponent
// Created: AME 2010-08-02
// -----------------------------------------------------------------------------
gaiaGeomCollPtr CreateBlockAutoProcess::SubstractTerrainComponentsFromAreas( gaiaGeomCollPtr urbans, gaiaGeomCollPtr areas, gaiaGeomCollPtr lines )
{
    // clipping urban area with buffers
    gaiaGeomCollPtr blocks = lines
        ? gaiaGeometryDifference( urbans, lines )
        : gaiaCloneGeomColl( urbans );

    // clipping blocks with others areas 
    if( gaiaDimension( areas ) == 2 )
    {
        gaiaGeomCollPtr temp = gaiaGeometryDifference( blocks, areas );

        // geometry valid and type POLYGON
        if( gaiaDimension( temp ) == 2 )
        {
            gaiaFreeGeomColl( blocks );
            blocks = temp;
        }
        else
        {
            gaiaFreeGeomColl( temp );
        }
    }

    return blocks;
}

// -----------------------------------------------------------------------------
// Name: CreateBlockAutoProcess::UpdateUrbanModel
// Created: AME 2010-08-02
// -----------------------------------------------------------------------------
/*void CreateBlockAutoProcess::UpdateUrbanModel( UrbanModel& model, kernel::UrbanObject_ABC& parent, PointProjector_ABC& projector, const geometry::Polygon2f& footprint )
{
    // Cas particuliers des polygones à trous. Pas encore traité; créer une médiane pour couper le batiment...
    if( ! blocks_ )
    {
        return;
    }

    gaiaGeomCollPtr getBlocksFromFiles = GeometryFactory::Validate( GetUrbanBlockInArea( model, projector, footprint ) );
    gaiaPolygonPtr block = blocks_->FirstPolygon;
    bool addNewBlock;
    int count = 0;
    while( block )
    {
        addNewBlock = true;
        if( getBlocksFromFiles )
        {
            gaiaGeomCollPtr blockIntersect = geometryFactory_->InitGeometryCollection();
            gaiaPolygonPtr cloneBlock = gaiaClonePolygon( block );
            gaiaInsertPolygonInGeomColl( blockIntersect, cloneBlock->Exterior );
            gaiaMbrGeometry( blockIntersect );
            if( gaiaGeomCollIntersects( blockIntersect, getBlocksFromFiles ) == 1 )
            {
                addNewBlock = false;
            }
            gaiaFreeGeomColl( blockIntersect );
        }
        if( addNewBlock )
        {
            double x, y;
            std::vector< geometry::Point2f > points;
            points.reserve( block->Exterior->Points );
            for( int iv = 0; iv < block->Exterior->Points; ++iv )
            {
                gaiaGetPoint( block->Exterior->Coords, iv, &x, &y );
                geometry::Point2d outPoint( projector.Project( y, x ) );
                points.push_back( geometry::Point2f( static_cast< float >( outPoint.X() ), static_cast< float >( outPoint.Y() ) ) );
            }
            
            const geometry::Polygon2f polygon( points );
            if( model.Create( polygon, &parent ) )
            {
                ++count;
            }
        }
        block = block->Next;
    }
    gaiaFreeGeomColl( getBlocksFromFiles );
}*/

// -----------------------------------------------------------------------------
// Name: CreateBlockAutoProcess::GetUrbanBlockInArea
// Created: AME 2010-08-02
// -----------------------------------------------------------------------------
gaiaGeomCollPtr CreateBlockAutoProcess::GetUrbanBlocksInAreaFromIndex( gaiaGeomCollPtr blocks, const Polygon2f& footprint )
{
    const geometry::Rectangle2f bbox = footprint.BoundingBox();
    float radius = sqrt( ( pow( bbox.Width(), 2 ) + pow( bbox.Height(), 2 ) ) );

    geometry::Point2d centerProjected = projector_.Project( ( blocks->MaxY + blocks->MinY ) / 2 , ( blocks->MaxX + blocks->MinX ) / 2  );
    geometry::Point2f center( static_cast< float >( centerProjected.X() ), static_cast< float >( centerProjected.Y() ) );

    gaiaGeomCollPtr blocksFromIndex = 0;

    std::vector< const kernel::UrbanObject_ABC* > urbanBlocks;
    index_.GetListWithinCircle( center, radius, urbanBlocks ); // to add more blocks (quad tree accuracy not very good)

    for( auto it = urbanBlocks.begin(); it != urbanBlocks.end(); ++it )
    {
        if( const kernel::UrbanPositions_ABC* attribute = ( *it )->Retrieve< kernel::UrbanPositions_ABC >() )
        {
            geometryFactory_->AddPolygonGeometryToCollection( attribute->Polygon(), projector_, blocksFromIndex );
        }
    }

    return blocksFromIndex;
}

// -----------------------------------------------------------------------------
// Name: CreateBlockAutoProcess::UpdateBuildingTable
// Created: AME 2010-08-02
// -----------------------------------------------------------------------------
void CreateBlockAutoProcess::ClipBlocksWithCollection( gaiaGeomCollPtr blocks, gaiaGeomCollPtr collection )
{
    if( ( ! collection ) || ( ! blocks ) )
    {
        return;
    }

    gaiaPolygonPtr block = blocks->FirstPolygon;
    gaiaPolygonPtr prev = 0;

    gaiaGeomCollPtr geom;
    while( block )
    {
        // Create an empty collection
        geom = geometryFactory_->InitGeometryCollection();

        // Insert a copy of the block in that collection
        gaiaInsertPolygonInGeomColl( geom, gaiaCloneRing( block->Exterior ) );
        gaiaMbrGeometry( geom );

        // If the buildings and this block (copy) intersect, remove the block from the collection
        if( 1 == gaiaGeomCollIntersects( geom, collection ) )
        {
            // If it is the first polygon in the geometry set
            if( block == blocks->FirstPolygon )
            {
                blocks->FirstPolygon = block->Next;
            }
            // If it is the last polygon in the geometry set
            // (not "else if" because the set can contain only 1 polygon)
            if( block == blocks->LastPolygon )
            {
                blocks->LastPolygon = prev;
            }
            // If prev is not NULL, update it.
            if( prev )
            {
                prev->Next = block->Next;
            }

            gaiaPolygonPtr next = block->Next;
            // Free the polygon, it is not needed anymore
            gaiaFreePolygon( block );
            block = next;
        }
        else
        {
            // Go on, there was no intersection...
            prev = block;
            block = block->Next;
        }

        gaiaFreeGeomColl( geom );
    }
}

void CreateBlockAutoProcess::FillPolygonVector( gaiaGeomCollPtr blocks, std::vector< geometry::Polygon2f >& vec )
{
    gaiaPolygonPtr block = blocks->FirstPolygon;
    double x, y;
    std::vector< geometry::Point2f > points;
    while( block )
    {
        points.resize( 0 );
        points.reserve( block->Exterior->Points );
        for( int i = 0; i < block->Exterior->Points; ++i )
        {
            gaiaGetPoint( block->Exterior->Coords, i, &x, &y );
            const geometry::Point2d outPoint( projector_.Project( y, x ) );
            points.push_back( geometry::Point2f( static_cast< float >( outPoint.X() ), static_cast< float >( outPoint.Y() ) ) );
        }
        vec.push_back( Polygon2f( points ) );
        block = block->Next;
    }
}
