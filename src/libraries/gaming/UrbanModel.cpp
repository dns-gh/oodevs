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
#include "Architecture.h"
#include "InfrastructureAttribute.h"
#include "MedicalTreatmentAttribute.h"
#include "ResourceNetwork.h"
#include "ResourceNetworkModel.h"
#include "StaticModel.h"
#include "StructuralStateAttribute.h"
#include "UrbanColor.h"
#include "UrbanPositions.h"
#include "Usages.h"
#include "clients_gui/TerrainObjectProxy.h"
#include "clients_gui/UrbanDisplayOptions.h"
#include "clients_gui/Usages.h"
#include "clients_gui/Architecture.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "MT_Tools/MT_Logger.h"

// -----------------------------------------------------------------------------
// Name: UrbanModel constructor
// Created: SLG 2009-10-20
// -----------------------------------------------------------------------------
UrbanModel::UrbanModel( kernel::Controllers& controllers, ResourceNetworkModel& resourceNetwork, const StaticModel& staticModel )
    : controllers_        ( controllers )
    , resourceNetwork_    ( resourceNetwork )
    , static_             ( staticModel )
    , urbanDisplayOptions_( new gui::UrbanDisplayOptions( controllers, staticModel.accommodationTypes_ ) )
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
    gui::TerrainObjectProxy* pTerrainObject = new gui::TerrainObjectProxy( controllers_, message.name(), id, type, *urbanDisplayOptions_, static_.accommodationTypes_ );
    kernel::PropertiesDictionary& dictionary = pTerrainObject->Get< kernel::PropertiesDictionary >();
    pTerrainObject->Attach< kernel::UrbanColor_ABC >( *new UrbanColor( message.attributes() ) );
    const kernel::UrbanColor_ABC& color = pTerrainObject->Get< kernel::UrbanColor_ABC >();
    pTerrainObject->Attach< kernel::UrbanPositions_ABC >( *new UrbanPositions( message.location(), message.attributes(), static_.coordinateConverter_, pTerrainObject->GetName().toStdString(), color ) );
    if( message.attributes().has_architecture() )
        pTerrainObject->Attach< kernel::Architecture_ABC >( *new Architecture( message.attributes(), std::auto_ptr< kernel::Architecture_ABC >( new gui::Architecture( dictionary ) ) ) );
    pTerrainObject->Attach< kernel::Usages_ABC >( *new Usages( message.attributes(), std::auto_ptr< kernel::Usages_ABC >( new gui::Usages( dictionary, static_.accommodationTypes_, pTerrainObject->GetLivingSpace() ) ) ) );
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
                {
                    pTerrainObject->Attach< kernel::Infrastructure_ABC >( *new InfrastructureAttribute( controllers_, *pTerrainObject, static_.objectTypes_, pTerrainObject->Get< kernel::PropertiesDictionary >() ) );
                    pTerrainObject->Get< kernel::UrbanPositions_ABC >().SetInfrastructurePresent();
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
    if( message.has_attributes() )
    {
        gui::TerrainObjectProxy* pTerrainObject = Find( message.object().id() );
        if( pTerrainObject && message.attributes().has_medical_treatment() && pTerrainObject->Retrieve< kernel::MedicalTreatmentAttribute_ABC >() == 0 )
            pTerrainObject->Attach< kernel::MedicalTreatmentAttribute_ABC >( *new MedicalTreatmentAttribute( controllers_.controller_, static_.objectTypes_ ) );
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
