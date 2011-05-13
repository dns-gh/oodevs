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
#include <xeumeuleu/xml.hpp>

using namespace kernel;
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
    : Parameter< QString >( OrderParameter( xis.attribute< std::string >( "name" ).c_str(), "objective", false ) )
{
    xis >> xml::list( "parameter", *this, &Objective::ReadParameter, converter );
}

// -----------------------------------------------------------------------------
// Name: Objective constructor
// Created: SBO 2007-06-25
// -----------------------------------------------------------------------------
Objective::Objective( const OrderParameter& parameter, xml::xistream& xis, const CoordinateConverter_ABC& converter )
    : Parameter< QString >( parameter )
{
    xis >> xml::list( "parameter", *this, &Objective::ReadParameter, converter );
}

// -----------------------------------------------------------------------------
// Name: Objective constructor
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
Objective::Objective( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const sword::MissionObjective& message )
    : Parameter< QString >( parameter )
{
    AddParameter( *new Location( OrderParameter( tools::translate( "Parameter", "Objective location" ).ascii(), "location", false ), converter, message.location() ) );
    AddParameter( *new DateTime( OrderParameter( tools::translate( "Parameter", "Schedule" ).ascii(), "datetime", false ), message.time() ) );
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
    xis >> xml::attribute( "type", type );
    if( type == "location" )
        AddParameter( *new Location( OrderParameter( tools::translate( "Parameter", "Objective location" ).ascii(), "location", false ), converter, xis ) );
    else if( type == "datetime" )
        AddParameter( *new DateTime( OrderParameter( tools::translate( "Parameter", "Schedule" ).ascii(), "datetime", false ), xis ) );
}

// -----------------------------------------------------------------------------
// Name: Objective::CommitTo
// Created: SBO 2007-06-25
// -----------------------------------------------------------------------------
void Objective::CommitTo( sword::MissionParameter& message ) const
{
    message.set_null_value( !IsSet() );
    if( IsSet() )
        CommitTo( *message.mutable_value()->Add()->mutable_missionobjective() );
}
// -----------------------------------------------------------------------------
// Name: Objective::CommitTo
// Created: SBO 2007-06-25
// -----------------------------------------------------------------------------
void Objective::CommitTo( sword::MissionParameter_Value& message ) const
{
    if( IsSet() )
        CommitTo( *message.mutable_missionobjective() );
}

// -----------------------------------------------------------------------------
// Name: Objective::CommitTo
// Created: SBO 2007-06-25
// -----------------------------------------------------------------------------
void Objective::CommitTo( sword::MissionObjective& message ) const
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        const std::string type = it->second->GetType();
        if( type == "location" )
            static_cast< const Location* >( it->second )->CommitTo( *message.mutable_location() );
        else if( type == "datetime" )
            static_cast< const DateTime* >( it->second )->CommitTo( *message.mutable_time() );
    }
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

// -----------------------------------------------------------------------------
// Name: Objective::SerializeType
// Created: LDC 2011-05-13
// -----------------------------------------------------------------------------
std::string Objective::SerializeType() const
{
    return "objective";
}
