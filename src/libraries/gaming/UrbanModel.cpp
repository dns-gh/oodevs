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
#include "clients_kernel/DetectionMap.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Entity_ABC.h"
#include "gaming/UrbanBlockDeserializer.h"
#include "protocol/Simulation.h"
#include "protocol/Protocol.h"
#include "UrbanBlockDetectionMap.h"
#include <urban/Model.h>
#include <urban/UrbanFactory.h>
#include <urban/UrbanObjectDeserializer_ABC.h>
#include <urban/TerrainObject_ABC.h>


// -----------------------------------------------------------------------------
// Name: UrbanModel constructor
// Created: SLG 2009-10-20
// -----------------------------------------------------------------------------
UrbanModel::UrbanModel( kernel::Controller& controller, const kernel::DetectionMap& map )
    : controller_( controller )
    , model_( new urban::Model() )
    , map_( map )
    , urbanBlockDetectionMap_( *new UrbanBlockDetectionMap( map ) )
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
        const geometry::Point2f point( float( location.latitude() ), float( location.longitude() ) );
        footPrint.Add( point );
    }
    urban::TerrainObject_ABC* object = model_->GetFactory().CreateUrbanObject( id, name, &footPrint );
    UrbanBlockDeserializer urbanBlockDeserializer( message );
    object->Accept( urbanBlockDeserializer );
    gui::InfrastructureParameters infrastructure;
    infrastructure.structuralState_ = unsigned int( 100. * message.attributes().capacity().structuralstate() );
    gui::TerrainObjectProxy* pTerrainObject = new gui::TerrainObjectProxy( controller_, *object, message.oid(), QString( message.name().c_str() ) , infrastructure );
    object->InstanciateDecoration();
    controller_.Create( *pTerrainObject );
    if( !Resolver< kernel::Entity_ABC >::Find( id ) )
        tools::Resolver< kernel::Entity_ABC >::Register( id, *pTerrainObject );
    urbanBlockDetectionMap_.AddUrbanBlock( *object );
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::Update
// Created: JSR 2010-06-28
// -----------------------------------------------------------------------------
void UrbanModel::Update( const MsgsSimToClient::MsgUrbanUpdate& message )
{
    gui::InfrastructureParameters infrastructure;
    infrastructure.structuralState_ = unsigned int( 100. * message.attributes().capacity().structuralstate() );
    GetObject( message.oid() ).Update( infrastructure );
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::Purge
// Created: SLG 2009-10-20
// -----------------------------------------------------------------------------
void UrbanModel::Purge()
{
    tools::Resolver< kernel::Entity_ABC >::DeleteAll();
    model_->Purge();
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::GetModel
// Created: SLG 2009-10-20
// -----------------------------------------------------------------------------
const urban::Model& UrbanModel::GetModel() const
{
    return *model_;
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::GetUrbanBlockMap
// Created: SLG 2010-03-12
// -----------------------------------------------------------------------------
const UrbanBlockDetectionMap& UrbanModel::GetUrbanBlockMap() const
{
    return urbanBlockDetectionMap_;
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::GetUrbanBlockMap
// Created: SLG 2010-03-12
// -----------------------------------------------------------------------------
kernel::Entity_ABC& UrbanModel::GetObject( unsigned long id ) const
{
    return tools::Resolver< kernel::Entity_ABC >::Get( id );
}
