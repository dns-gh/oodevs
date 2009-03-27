// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "FireAttribute.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: FireAttribute constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
FireAttribute::FireAttribute( Controller& controller, const Resolver_ABC< FireClass >& resolver )
    : controller_( controller )
    , resolver_  ( resolver )
    , fireClass_ ( 0 )
    , heat_      ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FireAttribute destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
FireAttribute::~FireAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FireAttribute::UpdateData
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
template< typename T >
void FireAttribute::UpdateData( const T& message )
{
    if ( message.m.firePresent )    
    {
        fireClass_  = & resolver_.Get( message.fire.class_id );
        heat_       = message.fire.heat;
        controller_.Update( *(FireAttribute_ABC*)this );        
    }
}

// -----------------------------------------------------------------------------
// Name: FireAttribute::DoUpdate
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void FireAttribute::DoUpdate( const ASN1T_MsgObjectKnowledgeUpdate& message )
{
    UpdateData( message.attributes );
}

// -----------------------------------------------------------------------------
// Name: FireAttribute::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void FireAttribute::DoUpdate( const ASN1T_MsgObjectUpdate& message )
{
    UpdateData( message.attributes );
}

// -----------------------------------------------------------------------------
// Name: FireAttribute::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void FireAttribute::DoUpdate( const ASN1T_MsgObjectCreation& message )
{
    UpdateData( message.attributes );
}

// -----------------------------------------------------------------------------
// Name: FireAttribute::Display
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void FireAttribute::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Fire", "Fire" ) )
        .Display( tools::translate( "Fire", "Fire class:" ), fireClass_ )
        .Display( tools::translate( "Fire", "Fire temperature:" ), heat_ );
}

// -----------------------------------------------------------------------------
// Name: FireAttribute::DisplayInSummary
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
void FireAttribute::DisplayInSummary( kernel::Displayer_ABC& displayer ) const
{
    Display( displayer );
}
