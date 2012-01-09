// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "ObjectsModel.h"
#include "Team.h"
#include "ObjectFactory_ABC.h"
#include "Model.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Tools.h"
#include <boost/bind.hpp>
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ObjectsModel constructor
// Created: JSR 2011-02-22
// -----------------------------------------------------------------------------
ObjectsModel::ObjectsModel( Controllers& controllers, ObjectFactory_ABC& factory, const tools::StringResolver< ObjectType >& resolver )
    : controllers_( controllers )
    , factory_    ( factory )
    , resolver_   ( resolver )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectsModel destructor
// Created: JSR 2011-02-22
// -----------------------------------------------------------------------------
ObjectsModel::~ObjectsModel()
{
    Purge();
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectsModel::Finalize
// Created: JSR 2011-11-04
// -----------------------------------------------------------------------------
void ObjectsModel::Finalize()
{
    Apply( boost::bind( &Object_ABC::Finalize, _1) );
}

// -----------------------------------------------------------------------------
// Name: ObjectsModel::Purge
// Created: JSR 2011-02-22
// -----------------------------------------------------------------------------
void ObjectsModel::Purge()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: ObjectsModel::CreateObject
// Created: JSR 2011-02-22
// -----------------------------------------------------------------------------
Object_ABC* ObjectsModel::CreateObject( const Team_ABC& team, const ObjectType& type, const QString& name, const Location_ABC& location )
{
    Object_ABC* object = factory_.CreateObject( type, team, name, location );
    Register( object->GetId(), *object );
    return object;
}

// -----------------------------------------------------------------------------
// Name: ObjectsModel::CreateObject
// Created: JSR 2011-02-22
// -----------------------------------------------------------------------------
void ObjectsModel::CreateObject( xml::xistream& xis, const kernel::Team_ABC& team, Model& model )
{
    try
    {
        const std::string typeName = xis.attribute< std::string >( "type" );
        const ObjectType* type = resolver_.Find( typeName );
        if( type )
        {
            Object_ABC* object = factory_.CreateObject( xis, team, *type );
            Register( object->GetId(), *object );
        }
        else
            model.AppendLoadingError( eUnknownObjectTypes, typeName );
    }
    catch( std::exception& e )
    {
        model.AppendLoadingError( eOthers, std::string( e.what() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectsModel::NotifyDeleted
// Created: JSR 2011-02-22
// -----------------------------------------------------------------------------
void ObjectsModel::NotifyDeleted( const kernel::Object_ABC& object )
{
    Remove( object.GetId() );
}
