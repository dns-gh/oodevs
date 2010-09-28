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
#include "UrbanPositions.h"
#include "Model.h"
#include "ObjectsModel.h"
#include "ResourceNetwork.h"
#include "ResourceNetworkModel.h"
#include "StaticModel.h"
#include "StructuralStateAttribute.h"
#include "UrbanBlockDetectionMap.h"
#include "clients_gui/TerrainObjectProxy.h"
#include "clients_kernel/DetectionMap.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "protocol/Simulation.h"
#include "protocol/Protocol.h"
#include <urban/Architecture.h>
#include <urban/ColorAttribute.h>
#include <urban/Model.h>
#include <urban/UrbanFactory.h>
#include <urban/TerrainObject_ABC.h>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: UrbanModel constructor
// Created: SLG 2009-10-20
// -----------------------------------------------------------------------------
UrbanModel::UrbanModel( kernel::Controller& controller, const Model& model, const StaticModel& staticModel, const kernel::DetectionMap& map )
    : controller_( controller )
    , model_( model )
    , static_( staticModel )
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

namespace
{
    void AttachExtensions( urban::TerrainObject_ABC& object, const MsgsSimToClient::MsgUrbanCreation& message )
    {
        if( !message.has_attributes() )
            return;
        if( message.attributes().has_color() )
        {
            const MsgsSimToClient::UrbanAttributes::RgbaColor& color = message.attributes().color();
            urban::ColorAttribute* colorAttribute = new urban::ColorAttribute( object );
            colorAttribute->SetRed( static_cast< unsigned short >( color.red() ) );
            colorAttribute->SetGreen( static_cast< unsigned short >( color.green() ) );
            colorAttribute->SetBlue( static_cast< unsigned short >( color.blue() ) );
            colorAttribute->SetAlpha( color.alpha() );
            object.Attach( *colorAttribute );
        }
        if( message.attributes().has_architecture() )
        {
            const MsgsSimToClient::UrbanAttributes::Architecture& architecture = message.attributes().architecture();
            urban::Architecture* attribute = new urban::Architecture( object );
            attribute->SetHeight( architecture.height() );
            attribute->SetFloorNumber( architecture.floor_number() );
            attribute->SetRoofShape( architecture.roof_shape() );
            attribute->SetMaterial( architecture.material() );
            attribute->SetOccupation( architecture.occupation() );
            attribute->SetTrafficability( architecture.trafficability() );
            object.Attach( *attribute );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::Create
// Created: SLG 2009-10-205
// -----------------------------------------------------------------------------
void UrbanModel::Create( const MsgsSimToClient::MsgUrbanCreation& message )
{
    geometry::Polygon2f footPrint;
    std::string name( message.name() );
    unsigned long id = message.urban_object().id();
    for( int i = 0; i < message.location().coordinates().elem_size(); ++i )
        footPrint.Add( static_.coordinateConverter_.ConvertToXY( message.location().coordinates().elem( i ) ) );
    urban::TerrainObject_ABC* object = urbanModel_->GetFactory().CreateUrbanObject( id, name, &footPrint );
    AttachExtensions( *object, message );
    gui::TerrainObjectProxy* pTerrainObject = new gui::TerrainObjectProxy( controller_, *object, message.urban_object().id(), QString( message.name().c_str() ) );
    if( message.has_attributes() )
    {
        if( message.attributes().has_structure() )
            pTerrainObject->Attach< kernel::StructuralStateAttribute_ABC >( *new StructuralStateAttribute( controller_, pTerrainObject->Get< kernel::PropertiesDictionary >() ) );
        if( message.attributes().has_infrastructures() )
        {
            if( message.attributes().infrastructures().resource_network_size() > 0 )
                model_.resourceNetwork_.Create( *pTerrainObject, message.attributes().infrastructures() );
            // TODO update infrastructures other than resource network
        }
    }
    pTerrainObject->Attach< kernel::Positions >( *new UrbanPositions( *object, message.location(), static_.coordinateConverter_ ) );
    object->InstanciateDecoration();
    pTerrainObject->Update( message );
    pTerrainObject->Polish();
    if( !Find( id ) )
        Register( id, *pTerrainObject );
    urbanBlockDetectionMap_.AddUrbanBlock( *object );
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
