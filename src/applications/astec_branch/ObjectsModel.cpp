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
#include "Object.h"

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
    Resolver< Object >::DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: ObjectsModel::CreateObject
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void ObjectsModel::CreateObject( const ASN1T_MsgObjectCreation& asnMsg )
{
    if( ! Resolver< Object >::Find( asnMsg.oid ) )
    {
        Object* pObject = objectFactory_.Create( asnMsg );
        Resolver< Object >::Register( asnMsg.oid, *pObject );
    }
}
 
// -----------------------------------------------------------------------------
// Name: ObjectsModel::GetObject
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
Object& ObjectsModel::GetObject( unsigned long id )
{
    return Resolver< Object >::Get( id );
}

// -----------------------------------------------------------------------------
// Name: ObjectsModel::DeleteObject
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void ObjectsModel::DeleteObject( unsigned long id )
{
    delete Resolver< Object >::Find( id );
    Resolver< Object >::Remove( id );
}

