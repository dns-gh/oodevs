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
#include "InfrastructureAttribute.h"
#include "PhysicalAttribute.h"
#include "ResourceNetwork.h"
#include "ResourceNetworkModel.h"
#include "StaticModel.h"
#include "StructuralStateAttribute.h"
#include "UrbanColor.h"
#include "UrbanPositions.h"
#include "clients_gui/PropertiesDictionary.h"
#include "clients_gui/Tesselator.h"
#include "clients_gui/UrbanDisplayOptions.h"
#include "clients_gui/UrbanObject.h"
#include "MT_Tools/MT_Logger.h"
#include "protocol/Protocol.h"

// -----------------------------------------------------------------------------
// Name: UrbanModel constructor
// Created: SLG 2009-10-20
// -----------------------------------------------------------------------------
UrbanModel::UrbanModel( kernel::Controllers& controllers, ResourceNetworkModel& resourceNetwork, const StaticModel& staticModel )
    : controllers_        ( controllers )
    , resourceNetwork_    ( resourceNetwork )
    , static_             ( staticModel )
    , urbanDisplayOptions_( new gui::UrbanDisplayOptions( controllers, staticModel.accommodationTypes_ ) )
    , tesselator_         ( new gui::Tesselator() )
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
void UrbanModel::Create( const sword::UrbanCreation& message )
{
    unsigned long id = message.object().id();
    if( Find( id ) )
        return;
    if( message.location().coordinates().elem_size() == 0 )
    {
        MT_LOG_ERROR_MSG( "Urban block " << id << "cannot be created because of empty location." );
        return;
    }
    const kernel::ObjectType& type = static_.objectTypes_.tools::StringResolver< kernel::ObjectType >::Get( "urban block" );
    gui::UrbanObject* pTerrainObject = new gui::UrbanObject( controllers_, message.name(), id, type, static_.accommodationTypes_, *urbanDisplayOptions_ );
    gui::PropertiesDictionary& dictionary = pTerrainObject->Get< gui::PropertiesDictionary >();
    pTerrainObject->Attach< kernel::UrbanColor_ABC >( *new UrbanColor( message.attributes() ) );
    pTerrainObject->Attach< kernel::UrbanPositions_ABC >( *new UrbanPositions( message.location(), static_.coordinateConverter_, *pTerrainObject, *tesselator_ ) );
    pTerrainObject->Attach< kernel::PhysicalAttribute_ABC >( *new PhysicalAttribute( message.attributes(), dictionary, static_.accommodationTypes_, *pTerrainObject, static_.objectTypes_ ) );
    pTerrainObject->Update( message );
    pTerrainObject->Polish();
    Register( id, *pTerrainObject );
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
        kernel::UrbanObject_ABC* pTerrainObject = Find( message.object().id() );
        if( pTerrainObject )
        {
            if( message.attributes().has_structure() && pTerrainObject->Retrieve< kernel::StructuralStateAttribute_ABC >() == 0 )
                pTerrainObject->Attach< kernel::StructuralStateAttribute_ABC >( *new StructuralStateAttribute( *pTerrainObject, controllers_.controller_, pTerrainObject->Get< gui::PropertiesDictionary >() ) );
            if( pTerrainObject->Retrieve< gui::ResourceNetwork_ABC >() == 0 )
            {
                if( message.attributes().has_infrastructures() && message.attributes().infrastructures().resource_network_size() > 0 )
                    resourceNetwork_.Create( *pTerrainObject, message.attributes().infrastructures() );
                else
                    resourceNetwork_.Create( *pTerrainObject );
            }
            if( message.attributes().has_infrastructures() )
            {
                if( message.attributes().infrastructures().has_infrastructure() && pTerrainObject->Retrieve< gui::Infrastructure_ABC >() == 0 )
                {
                    pTerrainObject->Attach< gui::Infrastructure_ABC >( *new InfrastructureAttribute( controllers_, *pTerrainObject, static_.objectTypes_, pTerrainObject->Get< gui::PropertiesDictionary >() ) );
                    pTerrainObject->Get< kernel::UrbanPositions_ABC >().SetInfrastructurePresent( true );
                }
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
    GetObject( message.object().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::Purge
// Created: SLG 2009-10-20
// -----------------------------------------------------------------------------
void UrbanModel::Purge()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::GetObject
// Created: SLG 2010-03-12
// -----------------------------------------------------------------------------
kernel::UrbanObject_ABC& UrbanModel::GetObject( unsigned long id ) const
{
    return Get( id );
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::FindObject
// Created: SLG 2011-01-03
// -----------------------------------------------------------------------------
kernel::UrbanObject_ABC* UrbanModel::FindObject( unsigned long id ) const
{
    return Find( id );
}
