// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "LogisticRouteAttributes.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Units.h"
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: LogisticRouteAttributes constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
LogisticRouteAttributes::LogisticRouteAttributes( Controller& controller )
    : controller_( controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticRouteAttributes destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
LogisticRouteAttributes::~LogisticRouteAttributes()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticRouteAttributes::UpdateData
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
template< typename T >
void LogisticRouteAttributes::UpdateData( const T& message )
{
    if( message.m.specific_attributesPresent
     && message.specific_attributes.t == T_ObjectAttributesSpecific_logistic_route )
    {
        nLogRouteFlow_      = message.specific_attributes.u.logistic_route->flow_rate;
        nLogRouteLength_    = message.specific_attributes.u.logistic_route->length;
        nLogRouteWidth_     = message.specific_attributes.u.logistic_route->width;
        nLogRouteMaxWeight_ = message.specific_attributes.u.logistic_route->max_weight;
        bLogRouteEquipped_  = message.specific_attributes.u.logistic_route->equipped;
        controller_.Update( *(LogisticRouteAttributes_ABC*)this );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticRouteAttributes::DoUpdate
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void LogisticRouteAttributes::DoUpdate( const ASN1T_MsgObjectKnowledgeUpdate& message )
{
    UpdateData( message );
}

// -----------------------------------------------------------------------------
// Name: LogisticRouteAttributes::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void LogisticRouteAttributes::DoUpdate( const ASN1T_MsgObjectUpdate& message )
{
    UpdateData( message );
}

// -----------------------------------------------------------------------------
// Name: LogisticRouteAttributes::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void LogisticRouteAttributes::DoUpdate( const ASN1T_MsgObjectCreation& message )
{
    UpdateData( message );
}

// -----------------------------------------------------------------------------
// Name: LogisticRouteAttributes::Display
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void LogisticRouteAttributes::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Logistic route" ) )
                .Display( tools::translate( "Object", "Equipped:" ), bLogRouteEquipped_ )
                .Display( tools::translate( "Object", "Flow:" ), nLogRouteFlow_ * Units::vehiclesPerHour )
                .Display( tools::translate( "Object", "Width:" ), nLogRouteWidth_ * Units::meters )
                .Display( tools::translate( "Object", "Length:" ), nLogRouteLength_ * Units::meters )
                .Display( tools::translate( "Object", "Maximum weight:" ), nLogRouteMaxWeight_ * Units::tons );
}

// -----------------------------------------------------------------------------
// Name: LogisticRouteAttributes::DisplayInSummary
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
void LogisticRouteAttributes::DisplayInSummary( kernel::Displayer_ABC& displayer ) const
{
    Display( displayer );
}
