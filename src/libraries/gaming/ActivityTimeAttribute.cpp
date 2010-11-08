// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActivityTimeAttribute.h"
#include "Tools.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Units.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ActivityTimeAttribute constructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
ActivityTimeAttribute::ActivityTimeAttribute( kernel::Controller& controller )
    : controller_ ( controller )
    , activityTime_ ( 0 )
{
    // NOTHINH
}

// -----------------------------------------------------------------------------
// Name: ActivityTimeAttribute destructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
ActivityTimeAttribute::~ActivityTimeAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActivityTimeAttribute::Display
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void ActivityTimeAttribute::Display( kernel::Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Mine parameters" ) )
             .Display( tools::translate( "Object", "Activity time:" ), activityTime_ / 3600. * Units::hours );
}

// -----------------------------------------------------------------------------
// Name: ActivityTimeAttribute::DisplayInTooltip
// Created: JCR 2008-06-10
// -----------------------------------------------------------------------------
void ActivityTimeAttribute::DisplayInTooltip( Displayer_ABC& displayer ) const
{
    Display( displayer );
}

// -----------------------------------------------------------------------------
// Name: ActivityTimeAttribute::DisplayInSummary
// Created: JCR 2008-08-25
// -----------------------------------------------------------------------------
void ActivityTimeAttribute::DisplayInSummary( kernel::Displayer_ABC& displayer ) const
{
    Display( displayer );
}

// -----------------------------------------------------------------------------
// Name: template< typename T >     void ActivityTimeAttribute::UpdateData
// Created: JCR 2008-08-25
// -----------------------------------------------------------------------------
template< typename T >
void ActivityTimeAttribute::UpdateData( const T& message )
{
    if( message.has_activity_time()  )
    {
        activityTime_ = static_cast< unsigned int >( message.activity_time().value() );
        controller_.Update( *(ActivityTimeAttribute_ABC*)this );
    }
}

// -----------------------------------------------------------------------------
// Name: ActivityTimeAttribute::DoUpdate
// Created: JCR 2008-08-25
// -----------------------------------------------------------------------------
void ActivityTimeAttribute::DoUpdate( const MsgsSimToClient::MsgObjectUpdate& message )
{
    UpdateData( message.attributes() );
}

// -----------------------------------------------------------------------------
// Name: ActivityTimeAttribute::DoUpdate
// Created: JCR 2008-08-25
// -----------------------------------------------------------------------------
void ActivityTimeAttribute::DoUpdate( const MsgsSimToClient::MsgObjectKnowledgeUpdate& message )
{
    UpdateData( message.attributes() );
}

