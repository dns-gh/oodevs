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
#include "clients_kernel/Tools.h"
#include "protocol/SimulationSenders.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: FireAttribute constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
FireAttribute::FireAttribute( Controller& controller, const tools::Resolver_ABC< FireClass, std::string >& resolver )
    : controller_( controller )
    , resolver_  ( resolver )
    , fireClass_ ( 0 )
    , maxCombustionEnery_( 0 )
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
    if( message.has_fire()  )
    {
        fireClass_ = & resolver_.Get( message.fire().class_name() );
        maxCombustionEnery_ = message.fire().max_combustion_energy();
        controller_.Update( *(FireAttribute_ABC*)this );
    }
}

// -----------------------------------------------------------------------------
// Name: FireAttribute::DoUpdate
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void FireAttribute::DoUpdate( const sword::ObjectKnowledgeUpdate& message )
{
    UpdateData( message.attributes() );
}

// -----------------------------------------------------------------------------
// Name: FireAttribute::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void FireAttribute::DoUpdate( const sword::ObjectUpdate& message )
{
    UpdateData( message.attributes() );
}

// -----------------------------------------------------------------------------
// Name: FireAttribute::Display
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void FireAttribute::Display( kernel::Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Fire" ) )
        .Display( tools::translate( "Object", "Fire class:" ), fireClass_ )
        .Display( tools::translate( "Object", "Max combustion energy:" ), maxCombustionEnery_ );
}

// -----------------------------------------------------------------------------
// Name: FireAttribute::DisplayInSummary
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
void FireAttribute::DisplayInSummary( kernel::Displayer_ABC& displayer ) const
{
    Display( displayer );
}
