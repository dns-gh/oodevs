// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ObjectFactory.h"
#include "Object.h"
#include "Explosions.h"
#include "Model.h"
#include "AgentsModel.h"
#include "ObjectsModel.h"
#include "TeamsModel.h"
#include "UrbanModel.h"
#include "ObjectPositions.h"
#include "StaticModel.h"
#include "Color.h"

#include "actions/ActionsModel.h"
#include "clients_gui/PropertiesDictionary.h"
#include "clients_kernel/Color_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/ObjectHierarchies.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "protocol/Simulation.h"

// -----------------------------------------------------------------------------
// Name: ObjectFactory constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
ObjectFactory::ObjectFactory( kernel::Controllers& controllers,
                              Model& model,
                              const StaticModel& staticModel,
                              const kernel::Time_ABC& simulation,
                              const kernel::Profile_ABC& profile,
                              actions::ActionsModel& actionsModel )
    : attributesFactory_( controllers, model, staticModel, simulation )
    , controllers_( controllers )
    , model_( model )
    , static_( staticModel )
    , profile_( profile )
    , actionsModel_( actionsModel )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectFactory destructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
ObjectFactory::~ObjectFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectFactory::Create
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
kernel::Object_ABC* ObjectFactory::Create( const sword::ObjectCreation& message )
{
    Object* result = new Object( message, controllers_.controller_, static_.coordinateConverter_, static_.objectTypes_,
                                 [=]( const kernel::Object_ABC& object ) { return profile_.CanDoMagic( object ); } );
    result->SetRenameObserver( [=]( const QString& name ){ actionsModel_.PublishRename( *result, name ); } );
    result->Attach( *new Explosions( controllers_.controller_, model_.fireResultsFactory_ ) );
    result->Attach< kernel::Positions >( *new ObjectPositions( result->GetType(), static_.coordinateConverter_ ) );
    result->Attach< kernel::TacticalHierarchies >( *new kernel::ObjectHierarchies( *result, &model_.teams_.GetTeam( message.party().id() ) ) );
    if( message.has_color() )
        result->Attach< kernel::Color_ABC >( *new Color( message.color() ) );
    result->Update( message );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: ObjectFactory::RegisterAttributes
// Created: NLD 2010-11-02
// -----------------------------------------------------------------------------
void ObjectFactory::RegisterAttributes( kernel::Object_ABC& result, const sword::ObjectAttributes& attributes ) const
{
    attributesFactory_.Register( result, attributes );
}
