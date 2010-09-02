// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "UrbanModel.h"
#include "Model.h"
#include "ObjectsModel.h"
#include "ResourceNetwork.h"
#include "ResourceNetworkModel.h"
#include "StructuralStateAttribute.h"
#include "UrbanBlockDetectionMap.h"
#include "UrbanBlockDeserializer.h"
#include "clients_gui/TerrainObjectProxy.h"
#include "clients_kernel/DetectionMap.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "protocol/Simulation.h"
#include "protocol/Protocol.h"
#include <urban/Model.h>
#include <urban/UrbanFactory.h>
#include <urban/UrbanObjectDeserializer_ABC.h>
#include <urban/TerrainObject_ABC.h>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: UrbanModel constructor
// Created: SLG 2009-10-20
// -----------------------------------------------------------------------------
UrbanModel::UrbanModel( kernel::Controller& controller, const Model& model, const kernel::DetectionMap& map )
    : controller_( controller )
    , model_( model )
    , urbanModel_( new urban::Model() )
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
    urban::TerrainObject_ABC* object = urbanModel_->GetFactory().CreateUrbanObject( id, name, &footPrint );
    UrbanBlockDeserializer urbanBlockDeserializer( message );
    object->Accept( urbanBlockDeserializer );
    gui::TerrainObjectProxy* pTerrainObject = new gui::TerrainObjectProxy( controller_, *object, message.oid(), QString( message.name().c_str() ) );
    if( message.has_attributes() && message.attributes().has_structure() )
        pTerrainObject->Attach< kernel::StructuralStateAttribute_ABC >( *new StructuralStateAttribute( controller_, pTerrainObject->Get< kernel::PropertiesDictionary >() ) );
    object->InstanciateDecoration();
    pTerrainObject->Update( message );
    pTerrainObject->Polish();
    controller_.Create( *pTerrainObject );
    if( !Find( id ) )
        Register( id, *pTerrainObject );
    urbanBlockDetectionMap_.AddUrbanBlock( *object );
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::Update
// Created: JSR 2010-06-28
// -----------------------------------------------------------------------------
void UrbanModel::Update( const MsgsSimToClient::MsgUrbanUpdate& message )
{
    // TODO � virer quand les r�seaux de ressources seront dans urban et que leur cr�ation sera envoy�e dans MsgUrbanCreation et pas MsgUrbanUpdate
    if( message.has_attributes() && message.attributes().has_infrastructures() )
    {
        if( message.attributes().infrastructures().resource_network_size() > 0 )
        {
            TerrainObjectProxy& entity = GetObject( message.oid() );
            /*if( entity.Retrieve< kernel::ResourceNetwork_ABC >() )
                model_.resourceNetwork_.UrbanUpdate( message.oid(), message.attributes().infrastructures() );
            else*/
            if( !entity.Retrieve< kernel::ResourceNetwork_ABC >() )
                model_.resourceNetwork_.Create( GetObject( message.oid() ), message.attributes().infrastructures() );
        }
        // TODO update infrastructures other than resource network
    }
    GetObject( message.oid() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::Purge
// Created: SLG 2009-10-20
// -----------------------------------------------------------------------------
void UrbanModel::Purge()
{
    DeleteAll();
    urbanModel_->Purge();
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::GetModel
// Created: SLG 2009-10-20
// -----------------------------------------------------------------------------
const urban::Model& UrbanModel::GetModel() const
{
    return *urbanModel_;
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
TerrainObjectProxy& UrbanModel::GetObject( unsigned long id ) const
{
    return Get( id );
}
