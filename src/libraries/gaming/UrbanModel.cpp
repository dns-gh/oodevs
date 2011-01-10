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
#include "MedicalTreatmentAttribute.h"
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
UrbanModel::UrbanModel( kernel::Controllers& controllers, const Model& model, const StaticModel& staticModel, const kernel::DetectionMap& map )
    : controllers_           ( controllers )
    , model_                 ( model )
    , static_                ( staticModel )
    , urbanModel_            ( new urban::Model() )
    , map_                   ( map )
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
    void AttachExtensions( urban::TerrainObject_ABC& object, const sword::UrbanCreation& message )
    {
        if( !message.has_attributes() )
            return;
        if( message.attributes().has_color() )
        {
            const sword::RgbaColor& color = message.attributes().color();
            urban::ColorAttribute* colorAttribute = object.Retrieve< urban::ColorAttribute >();
            if( !colorAttribute )
            {
                colorAttribute = new urban::ColorAttribute( object );
                object.Attach( *colorAttribute );
            }
            colorAttribute->SetRed( static_cast< unsigned short >( color.red() ) );
            colorAttribute->SetGreen( static_cast< unsigned short >( color.green() ) );
            colorAttribute->SetBlue( static_cast< unsigned short >( color.blue() ) );
            colorAttribute->SetAlpha( color.alpha() );
        }
        else
        {
            urban::ColorAttribute* colorAttribute = object.Retrieve< urban::ColorAttribute >();
            if( colorAttribute )
                colorAttribute->SetAlpha( 0.7f );
        }
        if( message.attributes().has_architecture() )
        {
            const sword::UrbanAttributes::Architecture& architecture = message.attributes().architecture();
            urban::Architecture* attribute = object.Retrieve< urban::Architecture >();
            if( !attribute )
            {
                attribute = new urban::Architecture( object );
                object.Attach( *attribute );
            }
            attribute->SetHeight( architecture.height() );
            attribute->SetFloorNumber( architecture.floor_number() );
            attribute->SetRoofShape( architecture.roof_shape() );
            attribute->SetMaterial( architecture.material() );
            attribute->SetOccupation( architecture.occupation() );
            attribute->SetTrafficability( architecture.trafficability() );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::Create
// Created: SLG 2009-10-205
// -----------------------------------------------------------------------------
void UrbanModel::Create( const sword::UrbanCreation& message )
{
    geometry::Polygon2f footPrint;
    std::string name( message.name() );
    unsigned long id = message.urban_object().id();
    for( int i = 0; i < message.location().coordinates().elem_size(); ++i )
        footPrint.Add( static_.coordinateConverter_.ConvertToXY( message.location().coordinates().elem( i ) ) );
    urban::TerrainObject_ABC* object = urbanModel_->GetFactory().CreateUrbanObject( message.urban_block().id(), name, &footPrint );
    AttachExtensions( *object, message );
    gui::TerrainObjectProxy* pTerrainObject = new gui::TerrainObjectProxy( controllers_, *object, message.urban_object().id(), QString( message.name().c_str() ), static_.objectTypes_.tools::StringResolver< kernel::ObjectType >::Get( "urban block" ) );
    pTerrainObject->Attach< kernel::Positions >( *new UrbanPositions( *object, message.location(), static_.coordinateConverter_ ) );
    pTerrainObject->Update( message );
    pTerrainObject->Polish();
    if( !Find( id ) )
        Register( id, *pTerrainObject );
    urbanBlockDetectionMap_.AddUrbanBlock( *object );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkModel Update
// Created: MGD 2010-11-29
// -----------------------------------------------------------------------------
void UrbanModel::Update( const sword::UrbanUpdate& message )
{
    if( message.has_attributes() )
    {
        gui::TerrainObjectProxy* pTerrainObject = Find( message.urban_object().id() );
        if( pTerrainObject )
        {
            if( message.attributes().has_structure() && pTerrainObject->Retrieve< kernel::StructuralStateAttribute_ABC >() == 0 )
                pTerrainObject->Attach< kernel::StructuralStateAttribute_ABC >( *new StructuralStateAttribute( controllers_.controller_, pTerrainObject->Get< kernel::PropertiesDictionary >() ) );
        }
    }
    GetObject( message.urban_object().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::UpdateObject
// Created: SLG 2010-01-03
// -----------------------------------------------------------------------------
void UrbanModel::Update( const sword::ObjectUpdate& message )
{   
    if( message.has_attributes() )
    {
        gui::TerrainObjectProxy* pTerrainObject = Find( message.object().id() );
        if( pTerrainObject )
        {
            if( message.attributes().has_medical_treatment() && pTerrainObject->Retrieve< kernel::MedicalTreatmentAttribute_ABC >() == 0 )
                pTerrainObject->Attach< kernel::MedicalTreatmentAttribute_ABC >( *new MedicalTreatmentAttribute( controllers_.controller_, static_.objectTypes_ ) );
        }
    }
    GetObject( message.object().id() ).Update( message );
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

// -----------------------------------------------------------------------------
// Name: UrbanModel::FindObject
// Created: SLG 2011-01-03
// -----------------------------------------------------------------------------
TerrainObjectProxy* UrbanModel::FindObject( unsigned long id ) const
{
    return Find( id );
}

