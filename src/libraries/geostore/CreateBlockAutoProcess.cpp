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
CreateBlockAutoProcess::CreateBlockAutoProcess( const Database& database, SpatialRequestStatus& status )
    : database_( database )
    , blocks_  ( 0 )
    , status_  ( status )
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

namespace
{
    void AddBuildingsToUrban( gaiaGeomCollPtr urbans, gaiaGeomCollPtr buildings )
    {
        if( !buildings )
            return;
        gaiaPolygonPtr block = buildings->FirstPolygon;
        while( block )
        {
            gaiaInsertPolygonInGeomColl( urbans, gaiaCloneRing( block->Exterior ) );
            block = block->Next;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: CreateBlockAutoProcess::Run
// Created: AME 2010-08-02
// -----------------------------------------------------------------------------
void CreateBlockAutoProcess::Run( const geometry::Polygon2f& footprint, UrbanModel& model, kernel::UrbanObject_ABC& parent, PointProjector_ABC& projector )
{
    blocks_ = geometryFactory_->InitGeometryCollection();
    gaiaGeomCollPtr poly( geometryFactory_->CreatePolygonGeometry( footprint , projector ) );
    gaiaGeomCollPtr urbans( 0 );
    gaiaGeomCollPtr buffers( 0 );
    gaiaGeomCollPtr buildings( 0 );
    gaiaGeomCollPtr areas( 0 );
    Database::CIT_Tables it = database_.tables_.find( "urban" );
    if( it != database_.tables_.end() )
        urbans = it->second->GetFeaturesIntersectsWith( poly );
    if( !geometryFactory_->CheckValidity( urbans ) )
    {   
        status_.UpdateSpatialStatus( 0, true );
        return;
    }
    it = database_.tables_.find( "building" );
    if( it != database_.tables_.end() )
        buildings = it->second->GetFeaturesIntersectsWith( poly );
    PrepareTerrainComponents( poly, areas, buffers );
    gaiaFreeGeomColl( poly );
    AddBuildingsToUrban( urbans, buildings );
    ClippingUrbanAreaWithTerrainComponent( buffers, areas, urbans );
    UpdateUrbanModel( model, parent, projector, footprint ); //Update urban blocks
}

// -----------------------------------------------------------------------------
// Name: CreateBlockAutoProcess::PrepareTerrainComponent
// Created: AME 2010-08-02
// -----------------------------------------------------------------------------
void CreateBlockAutoProcess::PrepareTerrainComponents( gaiaGeomCollPtr selection, gaiaGeomCollPtr& areas, gaiaGeomCollPtr& buffers )
{
    gaiaGeomCollPtr temp( 0 );
    gaiaGeomCollPtr temp2( 0 );
    gaiaGeomCollPtr lines( 0 );
    for( Database::CIT_Tables it = database_.tables_.begin(); it != database_.tables_.end(); ++it )
    {
        if( it->first == "urban" || it->first == "building" )
            continue;
        
        if( it->second->GetGeometry() == "POLYGON" ) //Aggreagation des surfaces;
        {
            temp = it->second->GetFeaturesIntersectsWith( selection );
            if( !geometryFactory_->CheckValidity( temp ) )
                continue;

            if( geometryFactory_->CheckValidity( areas ) ) 
            {
                temp2 = gaiaGeometryUnion( temp, areas );
                if( geometryFactory_->CheckValidity( temp2 ) )
                {
                    gaiaFreeGeomColl( areas );
                    areas = gaiaCloneGeomColl( temp2 );
                }
                gaiaFreeGeomColl( temp2 );
            }
            else
            {
                areas = gaiaCloneGeomColl( temp );
                gaiaFreeGeomColl( temp );
            }
        }
        else //if ( it->second->GetGeometry() == "LINESTRING" ) 
        {
            lines = it->second->GetFeaturesIntersectsWith( selection );
            if( geometryFactory_->CheckValidity( lines ) )
            {
                temp = gaiaGeomCollBuffer( lines, 0.0001, 3 );
                gaiaFreeGeomColl( lines ); 
                if( !geometryFactory_->CheckValidity(  buffers ) )
                    buffers = gaiaCloneGeomColl( temp );
                else
                {
                    temp2 = gaiaGeometryUnion( buffers, temp );
                    gaiaFreeGeomColl( buffers );
                    buffers = gaiaCloneGeomColl( temp2 );
                    gaiaFreeGeomColl( temp2 );
                }
                gaiaFreeGeomColl( temp );
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
    gaiaGeomCollPtr temp( 0 );
    if( geometryFactory_->CheckValidity( buffers ) ) //clipping urban area with buffers
    {   
        blocks_ = gaiaGeometryDifference( urbans, buffers );
        gaiaFreeGeomColl( buffers ); 
    }
    else
        blocks_ = gaiaCloneGeomColl( urbans ); 
    gaiaFreeGeomColl( urbans );
    if( gaiaDimension( areas ) == 2 ) //clipping blocks with others areas 
        temp = gaiaGeometryDifference( blocks_, areas );
    if( gaiaDimension( temp ) == 2 ) //geometry valid and type POLYGON
        blocks_ = gaiaCloneGeomColl( temp ); 
    gaiaFreeGeomColl( areas );
    gaiaFreeGeomColl( temp );
}

// -----------------------------------------------------------------------------
// Name: CreateBlockAutoProcess::UpdateUrbanModel
// Created: AME 2010-08-02
// -----------------------------------------------------------------------------
void CreateBlockAutoProcess::UpdateUrbanModel( UrbanModel& model, kernel::UrbanObject_ABC& parent, PointProjector_ABC& projector, const geometry::Polygon2f& footprint )
{
    //Cas particuliers des polygones à trous. Pas encore traité; créer une médiane pour couper le batiment...
    if( !geometryFactory_->CheckValidity( blocks_ ) )
        return;
    gaiaGeomCollPtr getBlocksFromFiles = GetUrbanBlockInArea( model, projector, footprint );
    gaiaPolygonPtr block = blocks_->FirstPolygon;
    gaiaGeomCollPtr blockIntersect; 
    bool existingBlocks = geometryFactory_->CheckValidity( getBlocksFromFiles );
    bool addNewBlock;
    int count = 0;
    while( block )
    {
        addNewBlock = true;
        if( existingBlocks )
        {
            blockIntersect = geometryFactory_->InitGeometryCollection();
            gaiaPolygonPtr cloneBlock = gaiaClonePolygon( block );
            gaiaInsertPolygonInGeomColl( blockIntersect, cloneBlock->Exterior );
            gaiaMbrGeometry( blockIntersect );
            if( gaiaGeomCollIntersects( blockIntersect, getBlocksFromFiles ) == 1 )
                addNewBlock = false;
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
            kernel::UrbanObject_ABC* newBlock = model.Create( polygon, &parent );
            if( newBlock )
                ++count;
        }
        block = block->Next;
    }
    if( existingBlocks )
        gaiaFreeGeomColl( getBlocksFromFiles );
    gaiaFreeGeomColl( blocks_ );
    status_.UpdateSpatialStatus( count, true );
}

// -----------------------------------------------------------------------------
// Name: CreateBlockAutoProcess::GetUrbanBlockInArea
// Created: AME 2010-08-02
// -----------------------------------------------------------------------------
gaiaGeomCollPtr CreateBlockAutoProcess::GetUrbanBlockInArea( const UrbanModel& model, PointProjector_ABC& projector, const geometry::Polygon2f& footprint )
{
    gaiaMbrGeometry( blocks_ );
    const geometry::Rectangle2f bbox = footprint.BoundingBox();
    float radius = 0.5f * sqrt( bbox.Width() * bbox.Width() + bbox.Height() * bbox.Height() );
    geometry::Point2d centerProjected = projector.Project( ( blocks_->MaxY + blocks_->MinY ) / 2 , ( blocks_->MaxX + blocks_->MinX ) / 2  );
    geometry::Point2f center( static_cast< float >( centerProjected.X() ), static_cast< float >( centerProjected.Y() ) );
    gaiaGeomCollPtr getBlocksFromFiles = 0;
    std::vector< const kernel::UrbanObject_ABC* > urbanBlocks;
    model.GetListWithinCircle( center, radius, urbanBlocks ); // to add more blocks (quatree accuracy not very good)
    for( std::vector< const kernel::UrbanObject_ABC* >::const_iterator it = urbanBlocks.begin(); it != urbanBlocks.end(); ++it )
        if( const kernel::UrbanPositions_ABC* attribute = ( *it )->Retrieve< kernel::UrbanPositions_ABC >() )
            geometryFactory_->AddPolygonGeometryToCollection( attribute->Polygon(), projector, getBlocksFromFiles );

    return getBlocksFromFiles;
}
