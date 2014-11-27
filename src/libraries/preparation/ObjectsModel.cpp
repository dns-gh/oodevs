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
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Tools.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

ObjectsModel::ObjectsModel( Controllers& controllers, ObjectFactory_ABC& factory, const tools::StringResolver< ObjectType >& resolver )
    : tools::TrackingResolver< kernel::Object_ABC >( controllers )
    , factory_( factory )
    , resolver_( resolver )
{
    // NOTHING
}

ObjectsModel::~ObjectsModel()
{
    Purge();
}

void ObjectsModel::Finalize()
{
    Apply( std::mem_fn( &Object_ABC::Finalize ) );
}

void ObjectsModel::Purge()
{
    DeleteAll();
}

Object_ABC* ObjectsModel::CreateObject( const Team_ABC& team, const ObjectType& type, const QString& name, const Location_ABC& location )
{
    Object_ABC* object = factory_.CreateObject( type, team, name, location );
    Add( *object );
    return object;
}

void ObjectsModel::CreateObject( xml::xistream& xis, const kernel::Team_ABC& team, Model& model )
{
    try
    {
        const std::string typeName = xis.attribute< std::string >( "type" );
        if( const ObjectType* type = resolver_.Find( typeName ) )
            Add( *factory_.CreateObject( xis, team, *type ) );
        else
            model.AppendLoadingError( eUnknownObjectTypes, typeName );
    }
    catch( const std::exception& e )
    {
        model.AppendLoadingError( eImpossibleObjectCreation, tools::GetExceptionMsg( e ) );
    }
}
