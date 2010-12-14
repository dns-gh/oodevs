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
#include "protocol/Protocol.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: StructuralStateAttribute constructor
// Created: JSR 2010-09-01
// -----------------------------------------------------------------------------
StructuralStateAttribute::StructuralStateAttribute( kernel::Controller& controller, PropertiesDictionary& dictionary )
    : controller_     ( controller )
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
    if( message.has_structure() )
    {
        structuralState_ = message.structure().state();
        controller_.Update( *static_cast< StructuralStateAttribute_ABC* >( this ) );
    }
}
