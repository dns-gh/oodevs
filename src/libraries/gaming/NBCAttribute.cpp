// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "NBCAttribute.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Tools.h"
#include "protocol/SimulationSenders.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: NBCAttribute constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
NBCAttribute::NBCAttribute( Controller& controller, const tools::Resolver_ABC< NBCAgent >& resolver )
    : controller_( controller )
    , resolver_  ( resolver )
    , danger_    ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NBCAttribute destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
NBCAttribute::~NBCAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NBCAttribute::UpdateData
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
template< typename T >
void NBCAttribute::UpdateData( const T& message )
{
    if( message.has_nbc()  )
    {
        agents_.resize( message.nbc().nbc_agents_size() );

        for( int i = 0; i < message.nbc().nbc_agents_size(); ++i )
            agents_[ i ] = & resolver_.Get( message.nbc().nbc_agents( i ).id() );
        danger_ = message.nbc().danger_level();

        controller_.Update( *(NBCAttribute_ABC*)this );
    }
}

// -----------------------------------------------------------------------------
// Name: NBCAttribute::DoUpdate
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void NBCAttribute::DoUpdate( const sword::ObjectKnowledgeUpdate& message )
{
    UpdateData( message.attributes() );
}

// -----------------------------------------------------------------------------
// Name: NBCAttribute::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void NBCAttribute::DoUpdate( const sword::ObjectUpdate& message )
{
    UpdateData( message.attributes() );
}

// -----------------------------------------------------------------------------
// Name: NBCAttribute::Display
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void NBCAttribute::Display( Displayer_ABC& displayer ) const
{
    if( danger_ == -1 )
        displayer.Group( tools::translate( "NBC", "NBC" ) )
             .Display( tools::translate( "NBC", "Danger:" ), "-" )
             .Display( tools::translate( "NBC", "NBC agent(s):" ), "-" );
    else
        displayer.Group( tools::translate( "NBC", "NBC" ) )
             .Display( tools::translate( "NBC", "Danger:" ), danger_ )
             .Display( tools::translate( "NBC", "NBC agent(s):" ), agents_ );

}

// -----------------------------------------------------------------------------
// Name: NBCAttribute::DisplayInSummary
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
void NBCAttribute::DisplayInSummary( kernel::Displayer_ABC& displayer ) const
{
    Display( displayer );
}
