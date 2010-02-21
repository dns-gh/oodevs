// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "Objective.h"
#include "Location.h"
#include "DateTime.h"
#include "ParameterVisitor_ABC.h"
#include "protocol/Protocol.h"
#include <xeumeuleu/xml.h>

using namespace kernel;
using namespace xml;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: Objective constructor
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
Objective::Objective( const OrderParameter& parameter )
    : Parameter< QString >( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Objective constructor
// Created: SBO 2007-06-25
// -----------------------------------------------------------------------------
Objective::Objective( xml::xistream& xis, const CoordinateConverter_ABC& converter )
    : Parameter< QString >( OrderParameter( attribute< std::string >( xis, "name" ).c_str(), "objective", false ) )
{
    xis >> list( "parameter", *this, &Objective::ReadParameter, converter );
}

// -----------------------------------------------------------------------------
// Name: Objective constructor
// Created: SBO 2007-06-25
// -----------------------------------------------------------------------------
Objective::Objective( const OrderParameter& parameter, xml::xistream& xis, const CoordinateConverter_ABC& converter )
    : Parameter< QString >( parameter )
{
    xis >> list( "parameter", *this, &Objective::ReadParameter, converter );
}

// -----------------------------------------------------------------------------
// Name: Objective constructor
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
Objective::Objective( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const Common::MsgMissionObjective& message )
    : Parameter< QString >( parameter )
{
    AddParameter( *new Location( OrderParameter( tools::translate( "Parameter", "Location" ).ascii(), "location", false ), converter, message.localisation() ) );
    AddParameter( *new DateTime( OrderParameter( tools::translate( "Parameter", "Schedule" ).ascii(), "datetime", false ), message.horaire() ) );
}

// -----------------------------------------------------------------------------
// Name: Objective destructor
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
Objective::~Objective()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Objective::ReadParameter
// Created: SBO 2007-06-25
// -----------------------------------------------------------------------------
void Objective::ReadParameter( xml::xistream& xis, const CoordinateConverter_ABC& converter )
{
    std::string type;
    xis >> attribute( "type", type );
    if( type == "location" )
        AddParameter( *new Location( OrderParameter( tools::translate( "Parameter", "Location" ).ascii(), "location", false ), converter, xis ) );
    else if( type == "datetime" )
        AddParameter( *new DateTime( OrderParameter( tools::translate( "Parameter", "Schedule" ).ascii(), "datetime", false ), xis ) );
}

// -----------------------------------------------------------------------------
// Name: Objective::CommitTo
// Created: SBO 2007-06-25
// -----------------------------------------------------------------------------
void Objective::CommitTo( Common::MsgMissionParameter& message ) const
{
    message.set_null_value( !IsSet() );
    message.mutable_value()->mutable_missionobjective();    // enforce initialisation of parameter to force his type
    if( IsSet() )
        CommitTo( *message.mutable_value()->mutable_missionobjective() );
}

// -----------------------------------------------------------------------------
// Name: Objective::Clean
// Created: SBO 2007-06-25
// -----------------------------------------------------------------------------
void Objective::Clean( Common::MsgMissionParameter& message ) const
{
    message.mutable_value()->clear_missionobjective();
}

// -----------------------------------------------------------------------------
// Name: Objective::CommitTo
// Created: SBO 2007-06-25
// -----------------------------------------------------------------------------
void Objective::CommitTo( Common::MsgMissionObjective& message ) const
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        const std::string type = it->second->GetType();
        if( type == "location" )
            static_cast< const Location* >( it->second )->CommitTo( *message.mutable_localisation() );
        else if( type == "datetime" )
            static_cast< const DateTime* >( it->second )->CommitTo( *message.mutable_horaire() );
    }
}

// -----------------------------------------------------------------------------
// Name: Objective::Clean
// Created: SBO 2007-06-25
// -----------------------------------------------------------------------------
void Objective::Clean( Common::MsgMissionObjective& message ) const
{
    message.clear_localisation();
}

// -----------------------------------------------------------------------------
// Name: Objective::Accept
// Created: SBO 2007-06-25
// -----------------------------------------------------------------------------
void Objective::Accept( ParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
    Parameter< QString >::Accept( visitor );
}
