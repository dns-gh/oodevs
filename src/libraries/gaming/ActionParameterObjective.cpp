// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterObjective.h"
#include "ActionParameterLocation.h"
#include "ActionParameterDateTime.h"
#include "ActionParameterVisitor_ABC.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: ActionParameterObjective constructor
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
ActionParameterObjective::ActionParameterObjective( const OrderParameter& parameter )
    : ActionParameter< QString >( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObjective constructor
// Created: SBO 2007-06-25
// -----------------------------------------------------------------------------
ActionParameterObjective::ActionParameterObjective( xml::xistream& xis, const CoordinateConverter_ABC& converter )
    : ActionParameter< QString >( OrderParameter( attribute< std::string >( xis, "name" ).c_str(), "objective", false ) )
{
    xis >> list( "parameter", *this, &ActionParameterObjective::ReadParameter, converter );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObjective constructor
// Created: SBO 2007-06-25
// -----------------------------------------------------------------------------
ActionParameterObjective::ActionParameterObjective( const OrderParameter& parameter, xml::xistream& xis, const CoordinateConverter_ABC& converter )
    : ActionParameter< QString >( parameter )
{
    xis >> list( "parameter", *this, &ActionParameterObjective::ReadParameter, converter );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObjective constructor
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
ActionParameterObjective::ActionParameterObjective( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const ASN1T_MissionObjective& asn )
    : ActionParameter< QString >( parameter )
{
    AddParameter( *new ActionParameterLocation( OrderParameter( tools::translate( "ActionParameter", "Location" ).ascii(), "location", false ), converter, asn.localisation ) );
    AddParameter( *new ActionParameterDateTime( OrderParameter( tools::translate( "ActionParameter", "Schedule" ).ascii(), "datetime", false ), asn.horaire ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObjective destructor
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
ActionParameterObjective::~ActionParameterObjective()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObjective::ReadParameter
// Created: SBO 2007-06-25
// -----------------------------------------------------------------------------
void ActionParameterObjective::ReadParameter( xml::xistream& xis, const CoordinateConverter_ABC& converter )
{
    std::string type;
    xis >> attribute( "type", type );
    if( type == "location" )
        AddParameter( *new ActionParameterLocation( OrderParameter( tools::translate( "ActionParameter", "Location" ).ascii(), "location", false ), converter, xis ) );
    else if( type == "datetime" )
        AddParameter( *new ActionParameterDateTime( OrderParameter( tools::translate( "ActionParameter", "Schedule" ).ascii(), "datetime", false ), xis ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObjective::CommitTo
// Created: SBO 2007-06-25
// -----------------------------------------------------------------------------
void ActionParameterObjective::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = !IsSet();
    asn.value.t = T_MissionParameter_value_missionObjective;
    asn.value.u.missionObjective = new ASN1T_MissionObjective();
    CommitTo( *asn.value.u.missionObjective );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObjective::Clean
// Created: SBO 2007-06-25
// -----------------------------------------------------------------------------
void ActionParameterObjective::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.missionObjective )
        Clean( *asn.value.u.missionObjective );
    delete asn.value.u.missionObjective;
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObjective::CommitTo
// Created: SBO 2007-06-25
// -----------------------------------------------------------------------------
void ActionParameterObjective::CommitTo( ASN1T_MissionObjective& asn ) const
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        const std::string type = it->second->GetType();
        if( type == "location" )
            static_cast< const ActionParameterLocation* >( it->second )->CommitTo( asn.localisation );
        else if( type == "datetime" )
            static_cast< const ActionParameterDateTime* >( it->second )->CommitTo( asn.horaire );
    }
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObjective::Clean
// Created: SBO 2007-06-25
// -----------------------------------------------------------------------------
void ActionParameterObjective::Clean( ASN1T_MissionObjective& asn ) const
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        const std::string type = it->second->GetType();
        if( type == "location" )
            static_cast< const ActionParameterLocation* >( it->second )->Clean( asn.localisation );
    }
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObjective::Accept
// Created: SBO 2007-06-25
// -----------------------------------------------------------------------------
void ActionParameterObjective::Accept( ActionParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
    ActionParameter< QString >::Accept( visitor );
}
