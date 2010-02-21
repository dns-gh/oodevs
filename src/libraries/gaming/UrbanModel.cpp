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
#include "clients_gui/TerrainObjectProxy.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Entity_ABC.h"
#include "gaming/UrbanBlockDeserializer.h"
#include "protocol/Simulation.h"
#include "protocol/Protocol.h"
#include "urban/BlockModel.h"
#include "urban/Model.h"
#include "urban/StaticModel.h"
#include "urban/UrbanFactory.h"
#include "urban/UrbanObjectDeserializer_ABC.h"


// -----------------------------------------------------------------------------
// Name: UrbanModel constructor
// Created: SLG 2009-10-20
// -----------------------------------------------------------------------------
UrbanModel::UrbanModel( kernel::Controller& controller, const urban::StaticModel& staticModel )
    : controller_( controller )
    , model_( new urban::Model( staticModel ) )
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
void UrbanModel::Create( const MsgsSimToClient::MsgUrbanCreation& message )
{
    geometry::Polygon2f footPrint;
    std::string name( message.name() );
    unsigned long id = message.oid();
    for( int i = 0; i < message.location().coordinates().elem_size(); ++i )
    {
        const Common::MsgCoordLatLong& location = message.location().coordinates().elem( i );
        const geometry::Point2f point( location.latitude(), location.longitude() );
        footPrint.Add( point );
    }
    urban::TerrainObject_ABC* object = model_->GetFactory().CreateBlock( id, name, footPrint );
    urban::UrbanObjectDeserializer_ABC* urbanBlockDeserializer = new UrbanBlockDeserializer( message );
    object->Accept( *urbanBlockDeserializer );
    gui::TerrainObjectProxy* pTerrainObject = new gui::TerrainObjectProxy( message, controller_, *object ); 
    object->InstanciateDecoration();
    controller_.Create( *pTerrainObject );
    if( !Resolver< kernel::Entity_ABC >::Find( id ) )
        tools::Resolver< kernel::Entity_ABC >::Register( id, *pTerrainObject );
}
/*
// -----------------------------------------------------------------------------
// Name: DrawingsModel::Update
// Created: SLG 2009-10-20
// -----------------------------------------------------------------------------
void DrawingsModel::Update( const ASN1T_MsgUrbanUpdate& message )
{
    //TODO
}

// -----------------------------------------------------------------------------
// Name: DrawingsModel::Delete
// Created: SLG 2009-10-20
// -----------------------------------------------------------------------------
void DrawingsModel::Delete( const ASN1T_MsgUrbanDestruction& message )
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
    model_->blocks_.Purge();
}
