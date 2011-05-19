// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "DictionaryExtensions.h"

#include "actions/ExtensionList.h"
#include "clients_kernel/OrderParameter.h"
#include "protocol/ClientSenders.h"

// -----------------------------------------------------------------------------
// Name: DictionaryExtensions constructor
// Created: ABR 2011-05-12
// -----------------------------------------------------------------------------
DictionaryExtensions::DictionaryExtensions( kernel::Controllers& controllers, const std::string& extensionType, const kernel::ExtensionTypes& resolver )
    : kernel::DictionaryExtensions( controllers, extensionType, resolver )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DictionaryExtensions destructor
// Created: ABR 2011-05-12
// -----------------------------------------------------------------------------
DictionaryExtensions::~DictionaryExtensions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DictionaryExtensions::DoUpdate
// Created: ABR 2011-05-12
// -----------------------------------------------------------------------------
void DictionaryExtensions::DoUpdate( const sword::AutomatAttributes& message )
{
    Update( message );
}

// -----------------------------------------------------------------------------
// Name: DictionaryExtensions::DoUpdate
// Created: ABR 2011-05-12
// -----------------------------------------------------------------------------
void DictionaryExtensions::DoUpdate( const sword::CrowdUpdate& message )
{
    Update( message );
}

// -----------------------------------------------------------------------------
// Name: DictionaryExtensions::DoUpdate
// Created: ABR 2011-05-12
// -----------------------------------------------------------------------------
void DictionaryExtensions::DoUpdate( const sword::FormationUpdate& message )
{
    Update( message );
}

// -----------------------------------------------------------------------------
// Name: DictionaryExtensions::DoUpdate
// Created: ABR 2011-05-12
// -----------------------------------------------------------------------------
void DictionaryExtensions::DoUpdate( const sword::PartyCreation& message )
{
    Update( message );
}

// -----------------------------------------------------------------------------
// Name: DictionaryExtensions::DoUpdate
// Created: ABR 2011-05-12
// -----------------------------------------------------------------------------
void DictionaryExtensions::DoUpdate( const sword::PopulationUpdate& message )
{
    Update( message );
}

// -----------------------------------------------------------------------------
// Name: DictionaryExtensions::DoUpdate
// Created: ABR 2011-05-12
// -----------------------------------------------------------------------------
void DictionaryExtensions::DoUpdate( const sword::UnitAttributes& message )
{
    Update( message );
}

// -----------------------------------------------------------------------------
// Name: DictionaryExtensions::Update
// Created: ABR 2011-05-12
// -----------------------------------------------------------------------------
template< typename T >
void DictionaryExtensions::Update( const T& message )
{
    if( message.has_extension() )
    {
        bool enabled = false;
        for( int i = 0; i < message.extension().entries_size(); ++i )
        {
            std::string value = message.extension().entries( i ).value();
            SetValueWithDictionnaryLink( message.extension().entries( i ).name(), value );
            enabled = enabled || !value.empty();
        }
        SetEnabled( enabled );
    }
}

// -----------------------------------------------------------------------------
// Name: DictionaryExtensions::FillParameterList
// Created: ABR 2011-05-13
// -----------------------------------------------------------------------------
void DictionaryExtensions::FillParameterList( actions::parameters::ParameterList* parameterList ) const
{
    actions::parameters::ExtensionList* extensions = new actions::parameters::ExtensionList( *new kernel::OrderParameter( "Extensions", "extensionlist", false ) );
    parameterList->AddParameter( *extensions );
    for( CIT_Extensions it = extensions_.begin(); it != extensions_.end(); ++it )
        extensions->AddExtension( it->first, ( enabled_ ) ? GetValueWithDictionnaryLink( it->first ) : "" );
}
