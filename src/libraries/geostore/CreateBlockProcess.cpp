// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "geostore_pch.h"
#include "CreateBlockProcess.h"
#include "GeometryFactory.h"
#include "clients_kernel/UrbanObject_ABC.h"
#include "clients_kernel/UrbanPositions_ABC.h"
#include "SpatialIndexer.h"
using namespace geostore;

// -----------------------------------------------------------------------------
// Name: CreateBlockProcess constructor
// Created: AME 2010-08-05
// -----------------------------------------------------------------------------
CreateBlockProcess::CreateBlockProcess()
{
    geometryFactory_ =  new GeometryFactory();
}

// -----------------------------------------------------------------------------
// Name: CreateBlockProcess destructor
// Created: AME 2010-08-05
// -----------------------------------------------------------------------------
CreateBlockProcess::~CreateBlockProcess()
{
    delete geometryFactory_;
}

// -----------------------------------------------------------------------------
// Name: CreateBlockProcess::CanCreateBlock
// Created: AME 2010-08-05
// -----------------------------------------------------------------------------
bool CreateBlockProcess::CanCreateBlock( const SpatialIndexer& index, const geometry::Polygon2f& footprint, PointProjector_ABC& projector, unsigned int idToExclude /*= 0*/ )
{
    bool create = true;
    const geometry::Rectangle2f bbox = footprint.BoundingBox();
    float radius = 0.5f * sqrt( bbox.Width() * bbox.Width() + bbox.Height() * bbox.Height() );
    std::vector< const kernel::UrbanObject_ABC* > urbanBlocks;
    index.GetListWithinCircle( footprint.BoundingBoxCenter(), radius, urbanBlocks );

    if( urbanBlocks.size() > 1 || ( urbanBlocks.size() == 1 && urbanBlocks[ 0 ]->GetId() != idToExclude ) )
    {
        gaiaGeomCollPtr getBlocks = 0;
        for( auto it = urbanBlocks.begin(); it != urbanBlocks.end(); ++it )
            if( ( *it )->GetId() != idToExclude )
                if( const kernel::UrbanPositions_ABC* attribute = ( *it )->Retrieve< kernel::UrbanPositions_ABC >() )
                    geometryFactory_->AddPolygonGeometryToCollection( attribute->Polygon(), projector, getBlocks );
        if( getBlocks )
        {
            gaiaGeomCollPtr newBlock = geometryFactory_->CreatePolygonGeometry( footprint, projector );
            create = gaiaGeomCollIntersects( newBlock, getBlocks ) == 0;
            gaiaFreeGeomColl( newBlock );
            gaiaFreeGeomColl( getBlocks );
        }
    }
    return create;
}
