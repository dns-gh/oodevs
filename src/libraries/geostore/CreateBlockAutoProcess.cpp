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

// -----------------------------------------------------------------------------
// Name: CreateBlockAutoProcess constructor
// Created: AME 2010-08-02
// -----------------------------------------------------------------------------
CreateBlockAutoProcess::CreateBlockAutoProcess( const Database& database )
    : database_         ( database )
    , blocks_           ( 0 )
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
    gaiaFreeGeomColl( blocks_ ); // It's okay to pass NULL!
    blocks_ = 0;
}

// -----------------------------------------------------------------------------
// Name: CreateBlockAutoProcess::Run
// Created: AME 2010-08-02
// -----------------------------------------------------------------------------
void CreateBlockAutoProcess::Run( const geometry::Polygon2f& footprint, UrbanModel& model, kernel::UrbanObject_ABC& parent, PointProjector_ABC& projector )
{
    gaiaGeomCollPtr poly        = geometryFactory_->CreatePolygonGeometry( footprint , projector );
    gaiaGeomCollPtr buildings   = 0;
    gaiaGeomCollPtr urbans      = 0;

    Database::CIT_Tables it;

    // Find intersecting urban areas
    it = database_.tables_.find( "urban" );
    if( it != database_.tables_.end() )
    {
        urbans = GeometryFactory::Validate( it->second->GetFeaturesIntersectsWith( poly ) );
    }

    if( ! urbans )
    {
        return;
    }

    // Find intersecting buildings
    it = database_.tables_.find( "building" );
    if( it != database_.tables_.end() )
    {
        buildings = GeometryFactory::Validate( it->second->GetFeaturesIntersectsWith( poly ) );
    }

    gaiaGeomCollPtr buffers = 0;
    gaiaGeomCollPtr areas   = 0;
    PrepareTerrainComponents( poly, areas, buffers );
    gaiaFreeGeomColl( poly );

    ClippingUrbanAreaWithTerrainComponent( buffers, areas, urbans );
    gaiaFreeGeomColl( areas );
    gaiaFreeGeomColl( urbans );

    // Update building tables
    UpdateBuildingTable( buildings );

    // Update urban blocks
    UpdateUrbanModel( model, parent, projector, footprint );
}

// -----------------------------------------------------------------------------
// Name: CreateBlockAutoProcess::PrepareTerrainComponent
// Created: AME 2010-08-02
// -----------------------------------------------------------------------------
void CreateBlockAutoProcess::PrepareTerrainComponents( gaiaGeomCollPtr selection, gaiaGeomCollPtr& areas, gaiaGeomCollPtr& buffers )
{
    // Initialize...
    areas = 0;
    buffers = 0;

    for( Database::CIT_Tables it = database_.tables_.begin(); it != database_.tables_.end(); ++it )
    {
        if( it->first == "urban" || it->first == "building" )
        {
            continue;
        }

        if( it->second->GetGeometry() == "POLYGON" ) //Aggreagation des surfaces;
        {
            gaiaGeomCollPtr inter = GeometryFactory::Validate( it->second->GetFeaturesIntersectsWith( selection ) );
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
        else //if ( it->second->GetGeometry() == "LINESTRING" ) 
        {
            gaiaGeomCollPtr lines = GeometryFactory::Validate( it->second->GetFeaturesIntersectsWith( selection ) );
            if( ! lines )
            {
                continue;
            }

            gaiaGeomCollPtr temp = gaiaGeomCollBuffer( lines, 0.00001, 3 );
            gaiaFreeGeomColl( lines );
            if( ! buffers )
            {
                // Set the initial buffers
                buffers = temp;
            }
            else
            {
                // Merge
                gaiaGeomCollPtr merged = gaiaGeometryUnion( buffers, temp );
                gaiaFreeGeomColl( buffers );
                gaiaFreeGeomColl( temp );
                buffers = merged;
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: CreateBlockAutoProcess::ClippingUrbanAreaWithTerrainComponent
// Created: AME 2010-08-02
// -----------------------------------------------------------------------------
void CreateBlockAutoProcess::ClippingUrbanAreaWithTerrainComponent( gaiaGeomCollPtr buffers, gaiaGeomCollPtr areas, gaiaGeomCollPtr urbans )
{
    // clipping urban area with buffers
    blocks_ = buffers
        ? gaiaGeometryDifference( urbans, buffers )
        : gaiaCloneGeomColl( urbans );

    // clipping blocks with others areas 
    if( gaiaDimension( areas ) == 2 )
    {
        gaiaGeomCollPtr temp = gaiaGeometryDifference( blocks_, areas );

        // geometry valid and type POLYGON
        if( gaiaDimension( temp ) == 2 )
        {
            gaiaFreeGeomColl( blocks_ );
            blocks_ = temp;
        }
        else
        {
            gaiaFreeGeomColl( temp );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: CreateBlockAutoProcess::UpdateUrbanModel
// Created: AME 2010-08-02
// -----------------------------------------------------------------------------
void CreateBlockAutoProcess::UpdateUrbanModel( UrbanModel& model, kernel::UrbanObject_ABC& parent, PointProjector_ABC& projector, const geometry::Polygon2f& footprint )
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
}

// -----------------------------------------------------------------------------
// Name: CreateBlockAutoProcess::GetUrbanBlockInArea
// Created: AME 2010-08-02
// -----------------------------------------------------------------------------
gaiaGeomCollPtr CreateBlockAutoProcess::GetUrbanBlockInArea( const UrbanModel& model, PointProjector_ABC& projector, const geometry::Polygon2f& footprint )
{
    gaiaMbrGeometry( blocks_ );
    const geometry::Rectangle2f bbox = footprint.BoundingBox();
    float radius = sqrt( ( pow( bbox.Width(), 2 ) + pow( bbox.Height(), 2 ) ) );
    geometry::Point2d centerProjected = projector.Project( ( blocks_->MaxY + blocks_->MinY ) / 2 , ( blocks_->MaxX + blocks_->MinX ) / 2  );
    geometry::Point2f center( static_cast< float >( centerProjected.X() ), static_cast< float >( centerProjected.Y() ) );
    gaiaGeomCollPtr getBlocksFromFiles = 0;
    std::vector< const kernel::UrbanObject_ABC* > urbanBlocks;
    model.GetListWithinCircle( center, radius, urbanBlocks ); // to add more blocks (quatree accuracy not very good)
    for( std::vector< const kernel::UrbanObject_ABC* >::const_iterator it = urbanBlocks.begin(); it != urbanBlocks.end(); ++it )
    {
        if( const kernel::UrbanPositions_ABC* attribute = ( *it )->Retrieve< kernel::UrbanPositions_ABC >() )
        {
            geometryFactory_->AddPolygonGeometryToCollection( attribute->Polygon(), projector, getBlocksFromFiles );
        }
    }
    return getBlocksFromFiles;
}

// -----------------------------------------------------------------------------
// Name: CreateBlockAutoProcess::UpdateBuildingTable
// Created: AME 2010-08-02
// -----------------------------------------------------------------------------
void CreateBlockAutoProcess::UpdateBuildingTable( gaiaGeomCollPtr buildings )
{
    if( ! buildings )
    {
        return;
    }

    gaiaPolygonPtr block = blocks_->FirstPolygon;
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
        if( 1 == gaiaGeomCollIntersects( geom, buildings ) )
        {
            // If it is the first polygon in the geometry set
            if( block == blocks_->FirstPolygon )
            {
                blocks_->FirstPolygon = block->Next;
            }
            // If it is the last polygon in the geometry set
            // (not "else if" because the set can contain only 1 polygon)
            if( block == blocks_->LastPolygon )
            {
                blocks_->LastPolygon = prev;
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
