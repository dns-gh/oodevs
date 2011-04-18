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
#include "ResourceNetwork.h"
#include "ResourceNetworkModel.h"
#include "StaticModel.h"
#include "Usages.h"
#include "Architecture.h"
#include "StructuralStateAttribute.h"
#include "InfrastructureAttribute.h"
#include "MedicalTreatmentAttribute.h"
#include "UrbanBlockDetectionMap.h"
#include "clients_gui/TerrainObjectProxy.h"
#include "clients_gui/UrbanDisplayOptions.h"
#include "clients_gui/Usages.h"
#include "clients_gui/Architecture.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/UrbanPositions_ABC.h"
#include <urban/ColorAttribute.h>
#include <urban/Model.h>
#include <urban/UrbanFactory.h>
#include <urban/TerrainObject_ABC.h>

// -----------------------------------------------------------------------------
// Name: UrbanModel constructor
// Created: SLG 2009-10-20
// -----------------------------------------------------------------------------
UrbanModel::UrbanModel( kernel::Controllers& controllers, ResourceNetworkModel& resourceNetwork, const StaticModel& staticModel, UrbanBlockDetectionMap& map )
    : controllers_           ( controllers )
    , resourceNetwork_       ( resourceNetwork )
    , static_                ( staticModel )
    , urbanModel_            ( new urban::Model() )
    , urbanDisplayOptions_   ( new gui::UrbanDisplayOptions( controllers, staticModel.accommodationTypes_ ) )
    , urbanBlockDetectionMap_( map )
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
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::Create
// Created: SLG 2009-10-205
// -----------------------------------------------------------------------------
void UrbanModel::Create( const sword::UrbanCreation& message )
{
    unsigned long id = message.object().id();
    if( Find( id ) )
        return;
    geometry::Polygon2f footPrint;
    std::string name( message.name() );
    for( int i = 0; i < message.location().coordinates().elem_size(); ++i )
        footPrint.Add( static_.coordinateConverter_.ConvertToXY( message.location().coordinates().elem( i ) ) );
    urban::TerrainObject_ABC* object = urbanModel_->GetFactory().CreateUrbanObject( id, name, &footPrint );
    AttachExtensions( *object, message );
    gui::TerrainObjectProxy* pTerrainObject = new gui::TerrainObjectProxy( controllers_, *object, id, QString( name.c_str() ), static_.objectTypes_.tools::StringResolver< kernel::ObjectType >::Get( "urban block" ), *urbanDisplayOptions_ );
    kernel::PropertiesDictionary& dictionary = pTerrainObject->Get< kernel::PropertiesDictionary >();
    pTerrainObject->Attach< kernel::UrbanPositions_ABC >( *new UrbanPositions( *object, message.location(), static_.coordinateConverter_, name ) );
    pTerrainObject->Attach< kernel::Usages_ABC >( *new Usages( message.attributes(), std::auto_ptr< kernel::Usages_ABC >( new gui::Usages( dictionary ) ) ) );
    if( message.attributes().has_architecture() )
        pTerrainObject->Attach< kernel::Architecture_ABC >( *new Architecture( message.attributes(), std::auto_ptr< kernel::Architecture_ABC >( new gui::Architecture( dictionary ) ) ) );
    pTerrainObject->Update( message );
    pTerrainObject->Polish();
    Register( id, *pTerrainObject );
    urbanBlockDetectionMap_.AddUrbanBlock( *pTerrainObject );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkModel Update
// Created: MGD 2010-11-29
// -----------------------------------------------------------------------------
void UrbanModel::Update( const sword::UrbanUpdate& message )
{
    if( message.has_attributes() )
    {
        // TODO mettre toute cette partie dans une factory comme pour les Objets.
        gui::TerrainObjectProxy* pTerrainObject = Find( message.object().id() );
        if( pTerrainObject )
        {
            if( message.attributes().has_structure() && pTerrainObject->Retrieve< kernel::StructuralStateAttribute_ABC >() == 0 )
                pTerrainObject->Attach< kernel::StructuralStateAttribute_ABC >( *new StructuralStateAttribute( *pTerrainObject, controllers_.controller_, pTerrainObject->Get< kernel::PropertiesDictionary >() ) );

            if( pTerrainObject->Retrieve< kernel::ResourceNetwork_ABC >() == 0 )
            {
                if( message.attributes().has_infrastructures() && message.attributes().infrastructures().resource_network_size() > 0 )
                    resourceNetwork_.Create( *pTerrainObject, message.attributes().infrastructures() );
                else
                    resourceNetwork_.Create( *pTerrainObject );
            }
            if( message.attributes().has_infrastructures() )
            {
                if( message.attributes().infrastructures().has_infrastructure() && pTerrainObject->Retrieve< kernel::Infrastructure_ABC >() == 0 )
                    pTerrainObject->Attach< kernel::Infrastructure_ABC >( *new InfrastructureAttribute( controllers_, *pTerrainObject, static_.objectTypes_, pTerrainObject->Get< kernel::PropertiesDictionary >() ) );
            }
        }
    }
    GetObject( message.object().id() ).Update( message );
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
// Name: UrbanModel::GetObject
// Created: SLG 2010-03-12
// -----------------------------------------------------------------------------
gui::TerrainObjectProxy& UrbanModel::GetObject( unsigned long id ) const
{
    return Get( id );
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::FindObject
// Created: SLG 2011-01-03
// -----------------------------------------------------------------------------
gui::TerrainObjectProxy* UrbanModel::FindObject( unsigned long id ) const
{
    return Find( id );
}
