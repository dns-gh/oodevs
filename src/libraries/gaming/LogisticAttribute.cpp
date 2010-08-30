// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "LogisticAttribute.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: LogisticAttribute constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
LogisticAttribute::LogisticAttribute( Controller& controller, const tools::Resolver_ABC< Automat_ABC >& resolver )
    : controller_( controller )
    , resolver_( resolver )
    , tc2_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
LogisticAttribute::~LogisticAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute::UpdateData
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
template< typename T >
void LogisticAttribute::UpdateData( const T& message )
{
    if( message.has_logistic()  )
    {
        tc2_ = resolver_.Find( message.logistic().tc2().id() );
        controller_.Update( *(LogisticAttribute_ABC*)this );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute::DoUpdate
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void LogisticAttribute::DoUpdate( const MsgsSimToClient::MsgObjectKnowledgeUpdate& message )
{
    UpdateData( message.attributes() );
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void LogisticAttribute::DoUpdate( const MsgsSimToClient::MsgObjectUpdate& message )
{
    UpdateData( message.attributes() );
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void LogisticAttribute::DoUpdate( const MsgsSimToClient::MsgObjectCreation& message )
{
    UpdateData( message.attributes() );
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute::Display
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void LogisticAttribute::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Camp" ) )
             .Display( tools::translate( "Object", "TC2:" ), tc2_ );
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute::DisplayInSummary
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
void LogisticAttribute::DisplayInSummary( kernel::Displayer_ABC& displayer ) const
{
    Display( displayer );
}
