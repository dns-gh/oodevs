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
#include "AltitudeModifierAttribute.h"
#include "FloodAttribute.h"
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
    DeleteAll();
}

namespace
{
    class GenerateODBFloods : private boost::noncopyable
    {
    public:
        explicit GenerateODBFloods() {}
        virtual ~GenerateODBFloods() {}
        void operator()( kernel::Object_ABC& object ) const
        {
            FloodAttribute* flood = static_cast< FloodAttribute* >( object.Retrieve< FloodAttribute_ABC >() );
            if( flood && flood->ReadFromODB() )
                flood->GenerateFlood( true );
        }
    };

    class ModifyODBAltitudes : private boost::noncopyable
    {
    public:
        explicit ModifyODBAltitudes() {}
        virtual ~ModifyODBAltitudes() {}
        void operator()( kernel::Object_ABC& object ) const
        {
            AltitudeModifierAttribute* altitude = static_cast< AltitudeModifierAttribute* >( object.Retrieve< AltitudeModifierAttribute_ABC >() );
            if( altitude && altitude->ReadFromODB() )
                altitude->ModifyAltitude();
        }
    };

    class PostGenerateFloods : private boost::noncopyable
    {
    public:
        explicit PostGenerateFloods() {}
        virtual ~PostGenerateFloods() {}
        void operator()( kernel::Object_ABC& object ) const
        {
            AltitudeModifierAttribute* altitude = static_cast< AltitudeModifierAttribute* >( object.Retrieve< AltitudeModifierAttribute_ABC >() );
            if( altitude && !altitude->ReadFromODB() )
                altitude->ModifyAltitude();
            FloodAttribute* flood = static_cast< FloodAttribute* >( object.Retrieve< FloodAttribute_ABC >() );
            if( flood && !flood->ReadFromODB() )
                flood->GenerateFlood( true );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: ObjectsModel::Initialize
// Created: JSR 2011-05-20
// -----------------------------------------------------------------------------
void ObjectsModel::Initialize()
{
    Apply( ModifyODBAltitudes() );
    Apply( GenerateODBFloods() );
    Apply( PostGenerateFloods() );
}

// -----------------------------------------------------------------------------
// Name: ObjectsModel::CreateObject
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void ObjectsModel::CreateObject( const sword::ObjectCreation& message )
{
    if( ! Find( message.object().id() ) )
    {
        Object_ABC* pObject = objectFactory_.Create( message );
        Register( message.object().id(), *pObject );
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectsModel::UpdateObject
// Created: NLD 2010-11-02
// -----------------------------------------------------------------------------
void ObjectsModel::UpdateObject( const sword::ObjectUpdate& message )
{
    Object_ABC& object = GetObject( message.object().id() );
    objectFactory_.RegisterAttributes( object, message.attributes() );
    object.Update( message );
}

// -----------------------------------------------------------------------------
// Name: ObjectsModel::GetObject
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
Object_ABC& ObjectsModel::GetObject( unsigned long id )
{
    return Get( id );
}

// -----------------------------------------------------------------------------
// Name: ObjectsModel::FindObject
// Created: JSR 2010-04-21
// -----------------------------------------------------------------------------
kernel::Object_ABC* ObjectsModel::FindObject( unsigned long id )
{
    return Find( id );
}

// -----------------------------------------------------------------------------
// Name: ObjectsModel::DeleteObject
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void ObjectsModel::DeleteObject( unsigned long id )
{
    delete Find( id );
    Remove( id );
}

