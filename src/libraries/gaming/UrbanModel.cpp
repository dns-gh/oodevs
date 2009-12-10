// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "UrbanModel.h"
#include "urban/Model.h"
#include "urban/StaticModel.h"
#include "urban/UrbanFactory.h"
#include "urban/BlockModel.h"
#include "clients_gui/TerrainObjectProxy.h"
#include "clients_kernel/Controller.h"
#include "gaming/UrbanBlockDeserializer.h"
#include <Urban/UrbanObjectDeserializer_ABC.h>

#include "geometry/Types.h"

// -----------------------------------------------------------------------------
// Name: UrbanModel constructor
// Created: SLG 2009-10-20
// -----------------------------------------------------------------------------
UrbanModel::UrbanModel( kernel::Controller& controller, urban::StaticModel& staticModel )
    : staticModel_( staticModel )
    , model_( *new urban::Model( staticModel_ ) )
    , controller_( controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanModel destructor
// Created: SLG 2009-10-20
// -----------------------------------------------------------------------------
UrbanModel::~UrbanModel()
{
    Purge();
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::Create
// Created: SLG 2009-10-205
// -----------------------------------------------------------------------------
void UrbanModel::Create( const ASN1T_MsgUrbanCreation& asn )
{
    unsigned i= 0;
    geometry::Polygon2f footPrint;
    std::string name( asn.name );
    unsigned long id = asn.oid;
    while ( i < asn.location.coordinates.n )
    {
        geometry::Point2f point( asn.location.coordinates.elem[i].latitude, asn.location.coordinates.elem[i].longitude );
        footPrint.Add( point );
        ++i;
    }
    urban::TerrainObject_ABC* object = model_.GetFactory().CreateBlock( id, name, footPrint );
    urban::UrbanObjectDeserializer_ABC* urbanBlockDeserializer = new UrbanBlockDeserializer( asn );
    object->Accept( *urbanBlockDeserializer );
    gui::TerrainObjectProxy* pTerrainObject = new gui::TerrainObjectProxy( asn, controller_, *object ); 
    controller_.Create( *pTerrainObject );
    if( !Resolver< kernel::Entity_ABC >::Find( id ) )
        tools::Resolver< kernel::Entity_ABC >::Register( id, *pTerrainObject );
}
/*
// -----------------------------------------------------------------------------
// Name: DrawingsModel::Update
// Created: SLG 2009-10-20
// -----------------------------------------------------------------------------
void DrawingsModel::Update( const ASN1T_MsgUrbanUpdate& asn )
{
    //TODO
}

// -----------------------------------------------------------------------------
// Name: DrawingsModel::Delete
// Created: SLG 2009-10-20
// -----------------------------------------------------------------------------
void DrawingsModel::Delete( const ASN1T_MsgUrbanDestruction& asn )
{
    //TODO
}
*/

// -----------------------------------------------------------------------------
// Name: UrbanModel::Purge
// Created: SLG 2009-10-20
// -----------------------------------------------------------------------------
void UrbanModel::Purge()
{
    tools::Resolver< kernel::Entity_ABC >::DeleteAll();
    model_.blocks_.Purge();
}
