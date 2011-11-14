// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "StructuralStateAttribute.h"
#include "Tools.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/DictionaryUpdated.h"
#include "clients_kernel/Entity_ABC.h"
#include "protocol/Protocol.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: StructuralStateAttribute constructor
// Created: JSR 2010-09-01
// -----------------------------------------------------------------------------
StructuralStateAttribute::StructuralStateAttribute( kernel::Entity_ABC& entity, kernel::Controller& controller, PropertiesDictionary& dictionary )
    : entity_         ( entity )
    , controller_     ( controller )
    , structuralState_( 100 )
{
    CreateDictionary( dictionary );
}

// -----------------------------------------------------------------------------
// Name: StructuralStateAttribute destructor
// Created: JSR 2010-09-01
// -----------------------------------------------------------------------------
StructuralStateAttribute::~StructuralStateAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StructuralStateAttribute::GetValue
// Created: CMA 2011-09-08
// -----------------------------------------------------------------------------
unsigned int StructuralStateAttribute::GetValue() const
{
    return structuralState_;
}

// -----------------------------------------------------------------------------
// Name: StructuralStateAttribute::CreateDictionary
// Created: JSR 2010-09-01
// -----------------------------------------------------------------------------
void StructuralStateAttribute::CreateDictionary( kernel::PropertiesDictionary& dico ) const
{
    dico.Register( *this, tools::translate( "Block", "Info/StructuralState" ), structuralState_ );
}

// -----------------------------------------------------------------------------
// Name: StructuralStateAttribute::DoUpdate
// Created: JSR 2010-09-01
// -----------------------------------------------------------------------------
void StructuralStateAttribute::DoUpdate( const sword::UrbanUpdate& message )
{
    UpdateData( message.attributes() );
}

// -----------------------------------------------------------------------------
// Name: StructuralStateAttribute::DoUpdate
// Created: JSR 2010-09-01
// -----------------------------------------------------------------------------
void StructuralStateAttribute::DoUpdate( const sword::UrbanCreation& message )
{
    UpdateData( message.attributes() );
}

// -----------------------------------------------------------------------------
// Name: StructuralStateAttribute::UpdateData
// Created: JSR 2010-09-01
// -----------------------------------------------------------------------------
template< typename T >
void StructuralStateAttribute::UpdateData( const T& message )
{
    std::set< std::string > updated;

    UPDATE_SUBPROPERTY( message, structuralState_, structure, state, "Info", updated );

    if( !updated.empty() )
        controller_.Update( kernel::DictionaryUpdated( entity_, tools::translate( "Block", "Info" ) ) );
}
