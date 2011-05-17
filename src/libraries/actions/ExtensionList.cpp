// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "actions_pch.h"
#include "ExtensionList.h"

#include "ParameterList.h"
#include "ParameterVisitor_ABC.h"
#include "String.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/EntityResolver_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Inhabitant_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "protocol/Protocol.h"

using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: ExtensionList constructor
// Created: ABR 2011-05-16
// -----------------------------------------------------------------------------
ExtensionList::ExtensionList( const kernel::OrderParameter& parameter )
    : ParameterList( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExtensionList constructor
// Created: ABR 2011-05-16
// -----------------------------------------------------------------------------
ExtensionList::ExtensionList( const kernel::OrderParameter& parameter, const sword::Extension& message )
    : ParameterList( parameter )
{
    for( int i = 0; i < message.entries_size(); ++i )
        AddExtension( message.entries( i ).name(), message.entries( i ).value() );
}

// -----------------------------------------------------------------------------
// Name: ExtensionList destructor
// Created: ABR 2011-05-13
// -----------------------------------------------------------------------------
ExtensionList::~ExtensionList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExtensionList::Accept
// Created: ABR 2011-05-13
// -----------------------------------------------------------------------------
void ExtensionList::Accept( ParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: ExtensionList::CommitTo
// Created: ABR 2011-05-13
// -----------------------------------------------------------------------------
void ExtensionList::CommitTo( sword::MissionParameter& message ) const
{
    message.set_null_value( !IsSet() );
    if( IsSet() )
        CommitTo( *message.mutable_value()->Add()->mutable_extensionlist() );
}

// -----------------------------------------------------------------------------
// Name: ExtensionList::CommitTo
// Created: ABR 2011-05-13
// -----------------------------------------------------------------------------
void ExtensionList::CommitTo( sword::MissionParameter_Value& message ) const
{
    if( IsSet() )
        CommitTo( *message.mutable_extensionlist() );
}

// -----------------------------------------------------------------------------
// Name: ExtensionList::CommitTo
// Created: ABR 2011-05-13
// -----------------------------------------------------------------------------
void ExtensionList::CommitTo( sword::Extension& message ) const
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        sword::Extension_Entry& entry = *message.add_entries();
        const ParameterList* extension = static_cast< const ParameterList* >( it->second );

        tools::Iterator< const Parameter_ABC& > paramIt = extension->CreateIterator();
        while( paramIt.HasMoreElements() )
        {
            const String& param = static_cast< const String& >( paramIt.NextElement() );
            if( param.GetName() == "key" )
                entry.set_name( param.GetValue() );
            else if( param.GetName() == "value" )
                entry.set_value( param.GetValue() );
            else
                throw std::runtime_error( __FUNCTION__ ": bad parameter for extension order parameter" );
        }
        assert( entry.has_name() );
        assert( entry.has_value() );
    }
}

// -----------------------------------------------------------------------------
// Name: ExtensionList::AddExtension
// Created: ABR 2011-05-13
// -----------------------------------------------------------------------------
void ExtensionList::AddExtension( const std::string& key, const std::string& value )
{
    ParameterList& list = AddList( "Extension" );
    list.AddString( "key", key );
    list.AddString( "value", value );
}
