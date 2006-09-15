// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "ObjectsModel.h"
#include "ObjectFactory_ABC.h"
#include "Object.h"
#include "clients_kernel/Object_ABC.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

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
    Resolver< Object_ABC >::DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: ObjectsModel::CreateObject
// Created: SBO 2006-09-11
// -----------------------------------------------------------------------------
Object_ABC* ObjectsModel::CreateObject( ObjectType& type, Team_ABC& team, const Location_ABC& location )
{
    Object_ABC* object = objectFactory_.CreateObject( type, team, location );
    Resolver< Object_ABC >::Register( object->GetId(), *object );
    return object;
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
    delete Resolver< Object_ABC >::Find( id );
    Resolver< Object_ABC >::Remove( id );
}

// -----------------------------------------------------------------------------
// Name: ObjectsModel::Serialize
// Created: SBO 2006-09-06
// -----------------------------------------------------------------------------
void ObjectsModel::Serialize( xml::xostream& xos ) const
{
    xos << start( "Objets" );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        static_cast< const Object* >( it->second )->Serialize( xos ); // $$$$ SBO 2006-09-06: 
    xos << end();
}
