// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Object.h"
#include "Model.h"
#include "Side.h"
#include "protocol/ClientPublisher_ABC.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "clients_kernel/ObjectType.h"
#include "protocol/ClientSenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Object constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
Object::Object( Model_ABC& model, const sword::ObjectCreation& msg, const tools::Resolver_ABC< kernel::ObjectType, std::string >& types )
    : Object_ABC   ( msg.object().id(), QString( msg.name().c_str() ) )
    , type_        ( types.Get( msg.type().id() ) )
    , localisation_( msg.location() )
    , side_        ( model.Sides().Get( msg.party().id() ) )
    , attributes_  ( model )
{
    side_.Register( *this );
    AddExtension( *this );
    optionals_.localisationPresent = 0;
    if( msg.has_color() )
        color_.reset( new sword::RgbColor( msg.color() ) );
    if( msg.has_extension() )
        for( int i = 0; i < msg.extension().entries_size(); ++i )
            extensions_[ msg.extension().entries( i ).name() ] = msg.extension().entries( i ).value();
}

// -----------------------------------------------------------------------------
// Name: Object destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
Object::~Object()
{
    side_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: Object::DoUpdate
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void Object::DoUpdate( const sword::ObjectUpdate& msg )
{
    if( msg.has_location() )
    {
        localisation_.Update( msg.location() );
        optionals_.localisationPresent = 1;
    }
    attributes_.Update( msg.attributes() );
    Observable< sword::ObjectUpdate >::Notify( msg );
}

// -----------------------------------------------------------------------------
// Name: Object::SendCreation
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void Object::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::ObjectCreation asn;
    asn().mutable_object()->set_id( GetId() );
    asn().mutable_type()->set_id( type_.GetType().c_str() );
    asn().set_name( GetName().toStdString() );
    asn().mutable_party()->set_id( side_.GetId() );
    localisation_.Send( *asn().mutable_location() );
    if( color_ )
        *asn().mutable_color() = *color_;
    asn().mutable_attributes(); //$$$$ NLD 2010-10-26 - A VIRER quand viré dans le protocole ... le message de creation ne doit PAS envoyer les attributs
    for( std::map< std::string, std::string >::const_iterator it = extensions_.begin(); it !=  extensions_.end(); ++it )
    {
        sword::Extension_Entry* entry = asn().mutable_extension()->add_entries();
        entry->set_name( it->first );
        entry->set_value( it->second );
    }
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Object::SendFullUpdate
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void Object::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    client::ObjectUpdate asn;
    asn().mutable_object()->set_id( GetId() );
    if( optionals_.localisationPresent )
        localisation_.Send( *asn().mutable_location() );
    attributes_.Send( *asn().mutable_attributes() );
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Object::SendDestruction
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void Object::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    client::ObjectDestruction destruction;
    destruction().mutable_object()->set_id( GetId() );
    destruction.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Object::Accept
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
void Object::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: Object::Display
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
void Object::Display( kernel::Displayer_ABC& ) const
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: Object::GetType
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
const kernel::ObjectType& Object::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: Object::GetTeam
// Created: SBO 2010-06-07
// -----------------------------------------------------------------------------
const Team_ABC& Object::GetTeam() const
{
    return side_;
}

// -----------------------------------------------------------------------------
// Name: Object::GetSymbol
// Created: JSR 2012-02-17
// -----------------------------------------------------------------------------
std::string Object::GetSymbol() const
{
    return type_.GetSymbol( localisation_.GetTypeName() );
}

// -----------------------------------------------------------------------------
// Name: Object::GetLocalisation
// Created: AHC 2012-08-08
// -----------------------------------------------------------------------------
const Localisation& Object::GetLocalisation() const
{
    return localisation_;
}

// -----------------------------------------------------------------------------
// Name: Object::GetExtension
// Created: AHC 2012-10-04
// -----------------------------------------------------------------------------
bool Object::GetExtension( const std::string& key, std::string& result ) const
{
    std::map< std::string, std::string >::const_iterator it = extensions_.find( key );
    if( it == extensions_.end() )
        return false;
    result = it->second;
    return true;
}

// -----------------------------------------------------------------------------
// Name: Object::GetAttributes
// Created: AHC 2013-07-08
// -----------------------------------------------------------------------------
const ObjectAttributeContainer& Object::GetAttributes() const
{
    return attributes_;
}
