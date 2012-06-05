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
#include "preparation/UrbanModel.h"
#include "urban/GeometryAttribute.h"
#include "urban/Model_ABC.h"
#include "urban/TerrainObject_ABC.h"

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
bool CreateBlockProcess::CanCreateBlock( const UrbanModel& model, const geometry::Polygon2f& footprint, PointProjector_ABC& projector )
{
    bool create( true );
    const geometry::Rectangle2f bbox = footprint.BoundingBox();
    float radius = sqrt( pow( bbox.Width(), 2 ) + pow( bbox.Height(), 2 ) );
    std::vector< const kernel::UrbanObject_ABC* > urbanBlocks;
    std::vector< const kernel::UrbanObject_ABC* >::const_iterator it;
    model.GetListWithinCircle( footprint.BoundingBoxCenter(), radius , urbanBlocks );

    if( urbanBlocks.size() != 0 )
    {
        gaiaGeomCollPtr getBlocks = 0;
        gaiaGeomCollPtr newBlock = geometryFactory_->CreatePolygonGeometry( footprint, projector );
        for( it = urbanBlocks.begin(); it != urbanBlocks.end(); ++it )
        {
            if( const kernel::UrbanPositions_ABC* attribute = ( *it )->Retrieve< kernel::UrbanPositions_ABC >() )
                geometryFactory_->AddPolygonGeometryToCollection( attribute->Polygon(), projector, getBlocks );
        }
        if( gaiaGeomCollIntersects( newBlock, getBlocks ) == 1 ) //don't keep blocks
            create = false;
    }
    return create;
}
