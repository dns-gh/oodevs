// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ObjectsModel.h"
#include "ObjectFactory_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "protocol/Protocol.h"

using namespace kernel;

ObjectsModel::ObjectsModel( kernel::Controllers& controllers, ObjectFactory_ABC& factory )
    : tools::TrackingResolver< kernel::Object_ABC >( controllers )
    , factory_( factory )
{
    // NOTHING
}

ObjectsModel::~ObjectsModel()
{
    Purge();
}

void ObjectsModel::Purge()
{
    DeleteAll();
}

void ObjectsModel::CreateObject( const sword::ObjectCreation& message )
{
    if( !Find( message.object().id() ) )
        Add( *factory_.Create( message ) );
}

void ObjectsModel::DeleteObject( const sword::ObjectDestruction& message )
{
    delete Find( message.object().id() );
}

void ObjectsModel::UpdateObject( const sword::ObjectUpdate& message )
{
    Object_ABC& object = Get( message.object().id() );
    factory_.RegisterAttributes( object, message.attributes() );
    object.Update( message );
}
