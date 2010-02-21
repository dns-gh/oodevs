// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ObjectsModel.h"
#include "ObjectFactory_ABC.h"
#include "clients_kernel/Object_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ObjectsModel constructor
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
ObjectsModel::ObjectsModel( ObjectFactory_ABC& objectFactory )
    : objectFactory_( objectFactory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectsModel destructor
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
ObjectsModel::~ObjectsModel()
{
    Purge();
}

// -----------------------------------------------------------------------------
// Name: ObjectsModel::Purge
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
void ObjectsModel::Purge()
{
    tools::Resolver< Object_ABC >::DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: ObjectsModel::CreateObject
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void ObjectsModel::CreateObject( const MsgsSimToClient::MsgObjectCreation& message )
{
    if( ! tools::Resolver< Object_ABC >::Find( message.oid() ) )
    {
        Object_ABC* pObject = objectFactory_.Create( message );
        tools::Resolver< Object_ABC >::Register( message.oid(), *pObject );
    }
}
 
// -----------------------------------------------------------------------------
// Name: ObjectsModel::GetObject
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
Object_ABC& ObjectsModel::GetObject( unsigned long id )
{
    return tools::Resolver< Object_ABC >::Get( id );
}

// -----------------------------------------------------------------------------
// Name: ObjectsModel::DeleteObject
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void ObjectsModel::DeleteObject( unsigned long id )
{
    delete tools::Resolver< Object_ABC >::Find( id );
    tools::Resolver< Object_ABC >::Remove( id );
}

