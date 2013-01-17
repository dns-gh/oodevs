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
#include "SpatialIndexer.h"
#include "terrain/PointProjector_ABC.h"
#include "clients_kernel/UrbanObject_ABC.h"
#include "clients_kernel/UrbanPositions_ABC.h"

using namespace geostore;

// -----------------------------------------------------------------------------
// Name: CreateBlockAutoProcess constructor
// Created: AME 2010-08-02
// -----------------------------------------------------------------------------
CreateBlockAutoProcess::CreateBlockAutoProcess( const Database& database, const SpatialIndexer& index, PointProjector_ABC& projector, double roadWidth )
    : database_( database )
    , index_( index )
    , projector_( projector )
    , roadWidth_( roadWidth )
    , geometryFactory_( new GeometryFactory() )
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

// -----------------------------------------------------------------------------
// Name: CreateBlockAutoProcess::Run
// Created: AME 2010-08-02
// -----------------------------------------------------------------------------
void CreateBlockAutoProcess::Run( const geometry::Polygon2f& footprint, std::vector< geometry::Polygon2f >& blocks )
{
    gaiaGeomCollPtr poly = geometryFactory_->CreatePolygonGeometry( footprint , projector_ );
    gaiaGeomCollPtr buildings = 0;
    gaiaGeomCollPtr urbans = 0;

    blocks.clear();

    // Find intersecting urban areas
    auto it = database_.tables_.find( "urban" );
    if( it != database_.tables_.end() )
        urbans = GeometryFactory::Validate( it->second->GetFeaturesIntersectingWith( poly ) );

    if( !urbans )
        return;

    // Find intersecting buildings
    it = database_.tables_.find( "building" );
    if( it != database_.tables_.end() )
        buildings = GeometryFactory::Validate( it->second->GetFeaturesIntersectingWith( poly ) );

    // Compute the road width
    double lat0, lon0, lat1, lon1;
    projector_.Unproject( geometry::Point2d( 0, 0 ), lat0, lon0 );
    projector_.Unproject( geometry::Point2d( 0 + roadWidth_, 0 ), lat1, lon1 );
    roadWidth_ = std::abs( lon1 - lon0 );

    // Fill areas with surface elements and lines with linear elements
    gaiaGeomCollPtr lines = 0;
    gaiaGeomCollPtr areas = 0;
    ExtractTerrainComponents( poly, areas, lines );
    gaiaFreeGeomColl( poly );

    // Clip urban areas with the areas and lines, yielding the discretized urban blocks
    gaiaGeomCollPtr urbanBlocks = SubstractTerrainComponentsFromAreas( urbans, areas, lines );
    gaiaFreeGeomColl( areas );
    gaiaFreeGeomColl( urbans );
    gaiaFreeGeomColl( lines );

    // Remove blocks that intersect buildings
    gaiaGeomCollPtr temp = ClipBlocksWithCollection( urbanBlocks, buildings );
    gaiaFreeGeomColl( urbanBlocks );
    gaiaFreeGeomColl( buildings );
    urbanBlocks = temp;

    // Remove blocks that intersect with existing urban blocks in the index/model
    gaiaGeomCollPtr blocksFromIndex = GeometryFactory::Validate( GetUrbanBlocksInAreaFromIndex( footprint ) );
    temp = ClipBlocksWithCollection( urbanBlocks, blocksFromIndex );
    gaiaFreeGeomColl( urbanBlocks );
    gaiaFreeGeomColl( blocksFromIndex );
    urbanBlocks = temp;

    // Populate the resulting polygons vector
    FillPolygonVector( urbanBlocks, blocks );
    gaiaFreeGeomColl( urbanBlocks );
}

// -----------------------------------------------------------------------------
// Name: CreateBlockAutoProcess::PrepareTerrainComponent
// Created: AME 2010-08-02
// -----------------------------------------------------------------------------
void CreateBlockAutoProcess::ExtractTerrainComponents( gaiaGeomCollPtr footprint, gaiaGeomCollPtr& areas, gaiaGeomCollPtr& lines )
{
    for( auto it = database_.tables_.begin(); it != database_.tables_.end(); ++it )
    {
        if( it->first == "urban" || it->first == "building" )
            continue;

        if( it->second->GetGeometryType() == GeoTable::Polygon ) // Aggregation des surfaces;
        {
            gaiaGeomCollPtr inter = GeometryFactory::Validate( it->second->GetFeaturesIntersectingWith( footprint ) );
            if( !inter )
                continue;

            if( !areas )
                areas = inter; // Set the initial areas.
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
            if( !inter )
                continue;

            // Upgrade the geometry with the width of the roads...
            gaiaGeomCollPtr temp = gaiaGeomCollBuffer( inter, roadWidth_, 3 );
            gaiaFreeGeomColl( inter );
            inter = temp;

            if( !lines )
                lines = inter; // Set the initial buffers
            else
            {
                // Merge
                gaiaGeomCollPtr merged = GeometryFactory::Validate( gaiaGeometryUnion( inter, lines ) );
                gaiaFreeGeomColl( inter );
                if( merged )
                {
                    gaiaFreeGeomColl( lines );
                    lines = merged;
                }
            }
        }
        else
        {
            // Unsupported geometry...
            throw MASA_EXCEPTION( "Unsupported geometry." );
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
            gaiaFreeGeomColl( temp );
    }
    return blocks;
}

// -----------------------------------------------------------------------------
// Name: CreateBlockAutoProcess::GetUrbanBlockInArea
// Created: AME 2010-08-02
// -----------------------------------------------------------------------------
gaiaGeomCollPtr CreateBlockAutoProcess::GetUrbanBlocksInAreaFromIndex( const geometry::Polygon2f& footprint )
{
    const geometry::Rectangle2f bbox = footprint.BoundingBox();
    float radius = 0.5f * sqrt( bbox.Width() * bbox.Width() + bbox.Height() * bbox.Height() );

    gaiaGeomCollPtr blocksFromIndex = 0;

    std::vector< const kernel::UrbanObject_ABC* > urbanBlocks;
    index_.GetListWithinCircle( bbox.Center(), radius, urbanBlocks ); // to add more blocks (quad tree accuracy not very good)

    for( auto it = urbanBlocks.begin(); it != urbanBlocks.end(); ++it )
        if( const kernel::UrbanPositions_ABC* attribute = ( *it )->Retrieve< kernel::UrbanPositions_ABC >() )
            geometryFactory_->AddPolygonGeometryToCollection( attribute->Polygon(), projector_, blocksFromIndex );

    return blocksFromIndex;
}

// -----------------------------------------------------------------------------
// Name: CreateBlockAutoProcess::UpdateBuildingTable
// Created: AME 2010-08-02
// -----------------------------------------------------------------------------
gaiaGeomCollPtr CreateBlockAutoProcess::ClipBlocksWithCollection( gaiaGeomCollPtr blocks, gaiaGeomCollPtr collection )
{
    if( !collection )
        return gaiaCloneGeomColl( blocks );
    if( !blocks )
        return nullptr;

    gaiaGeomCollPtr result = geometryFactory_->InitGeometryCollection();

    for( gaiaPolygonPtr block = blocks->FirstPolygon; block; block = block->Next )
    {
        // Create an empty collection
        gaiaGeomCollPtr geom = geometryFactory_->InitGeometryCollection();

        // Insert a copy of the block in that collection
        gaiaInsertPolygonInGeomColl( geom, gaiaCloneRing( block->Exterior ) );
        gaiaMbrGeometry( geom );

        if( gaiaGeomCollIntersects( geom, collection ) == 0 )
        {
            // Add a copy of this polygon to the result as it does not intersect with the collection
            gaiaInsertPolygonInGeomColl( result, gaiaCloneRing( block->Exterior ) );
        }
        gaiaFreeGeomColl( geom );
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: CreateBlockAutoProcess::FillPolygonVector
// Created: RCI 2012-12-06
// -----------------------------------------------------------------------------
void CreateBlockAutoProcess::FillPolygonVector( gaiaGeomCollPtr blocks, std::vector< geometry::Polygon2f >& vec )
{
    gaiaPolygonPtr block = blocks->FirstPolygon;
    double x, y;
    std::vector< geometry::Point2f > points;
    while( block )
    {
        points.reserve( block->Exterior->Points );
        for( int i = 0; i < block->Exterior->Points; ++i )
        {
            gaiaGetPoint( block->Exterior->Coords, i, &x, &y );
            const geometry::Point2d outPoint( projector_.Project( y, x ) );
            points.push_back( geometry::Point2f( static_cast< float >( outPoint.X() ), static_cast< float >( outPoint.Y() ) ) );
        }
        vec.push_back( geometry::Polygon2f( points ) );
        block = block->Next;
        points.clear();
    }
}
