// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "ObjectsModel.h"
#include "ObjectFactory_ABC.h"
#include "Object_ABC.h"

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
    Resolver< Object_ABC >::DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: ObjectsModel::CreateObject
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void ObjectsModel::CreateObject( const ASN1T_MsgObjectCreation& asnMsg )
{
    MT_LOG_INFO( "ObjectCreation - ID: " << asnMsg.oid, eReceived, 0 );
    if( ! Resolver< Object_ABC >::Find( asnMsg.oid ) )
    {
        Object_ABC* pObject = objectFactory_.Create( asnMsg );
        Resolver< Object_ABC >::Register( asnMsg.oid, *pObject );
//        application_.NotifyObjectCreated( *pObject );
    }
}
 
// -----------------------------------------------------------------------------
// Name: ObjectsModel::GetObject
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
Object_ABC& ObjectsModel::GetObject( unsigned long id )
{
    return Resolver< Object_ABC >::Get( id );
}

// -----------------------------------------------------------------------------
// Name: ObjectsModel::DeleteObject
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void ObjectsModel::DeleteObject( unsigned long id )
{
    MT_LOG_INFO( "ObjectDestruction - ID: " << id, eReceived, 0 );
//    application_.NotifyObjectDeleted( GetObject( id ) );
    delete Resolver< Object_ABC >::Find( id );
    Resolver< Object_ABC >::Remove( id );
}

