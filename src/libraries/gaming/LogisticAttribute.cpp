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
LogisticAttribute::LogisticAttribute( Controller& controller, const tools::Resolver_ABC< Automat_ABC >& automatResolver, const tools::Resolver_ABC< Formation_ABC >& formationResolver )
    : controller_( controller )
    , automatResolver_( automatResolver )
    , formationResolver_( formationResolver )
    , logisticSuperior_( 0 )
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
        logisticSuperior_ = FindLogisticEntity( message.logistic().logistic_superior() );
        controller_.Update( *(LogisticAttribute_ABC*)this );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute::DoUpdate
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void LogisticAttribute::DoUpdate( const sword::ObjectKnowledgeUpdate& message )
{
    UpdateData( message.attributes() );
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void LogisticAttribute::DoUpdate( const sword::ObjectUpdate& message )
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
             .Display( tools::translate( "Object", "Logistic superior:" ), logisticSuperior_ );
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute::DisplayInSummary
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
void LogisticAttribute::DisplayInSummary( kernel::Displayer_ABC& displayer ) const
{
    Display( displayer );
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute::FindLogisticEntity
// Created: NLD 2011-01-19
// -----------------------------------------------------------------------------
kernel::Entity_ABC* LogisticAttribute::FindLogisticEntity( const sword::ParentEntity& message ) const
{
    if( message.has_automat() )
        return (kernel::Entity_ABC*)automatResolver_.Find( message.automat().id() );
    else if( message.has_formation() )
        return (kernel::Entity_ABC*)formationResolver_.Find( message.formation().id() );
    return 0;
}
