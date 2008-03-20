// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterObstacle.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/ObjectIcons.h"
#include "ActionParameterLocation.h"
#include "ActionParameterAutomat.h"
#include "ActionParameterNumeric.h"
#include "ActionParameterObstacleType.h"
#include "ActionParameterVisitor_ABC.h"
#include "Tools.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: ActionParameterObstacle constructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
ActionParameterObstacle::ActionParameterObstacle( const OrderParameter& parameter, const ObjectType& type )
    : ActionParameter< std::string >( parameter, type.GetName() )
    , type_( type )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObstacle constructor
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
ActionParameterObstacle::ActionParameterObstacle( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const Resolver_ABC< ObjectType >& types, const Resolver_ABC< Automat_ABC >& automats, const ASN1T_PlannedWork& asn )
    : ActionParameter< std::string >( parameter )
    , type_( types.Get( asn.type ) )
{
    AddParameter( *new ActionParameterLocation( OrderParameter( tools::translate( "ActionParameter", "Location" ).ascii(), "location", false ), converter, asn.position ) );
    SetValue( type_.GetName() );
    SetParameters( asn, automats );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObstacle constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
ActionParameterObstacle::ActionParameterObstacle( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const Resolver_ABC< ObjectType >& types, const Resolver_ABC< Automat_ABC >& automats, xml::xistream& xis )
    : ActionParameter< std::string >( parameter )
    , type_( types.Get( xml::attribute< unsigned int >( xis, "value" ) ) )
{
    xis >> list( "parameter", *this, &ActionParameterObstacle::ReadParameter, converter, automats );
    SetValue( type_.GetName() );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObstacle constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
ActionParameterObstacle::ActionParameterObstacle( const CoordinateConverter_ABC& converter, const Resolver_ABC< ObjectType >& types, const Resolver_ABC< Automat_ABC >& automats, xml::xistream& xis )
    : ActionParameter< std::string >( OrderParameter( xml::attribute< std::string >( xis, "name" ).c_str(), "obstacle", false ) )
    , type_( types.Get( xml::attribute< unsigned int >( xis, "value" ) ) )
{
    xis >> list( "parameter", *this, &ActionParameterObstacle::ReadParameter, converter, automats );
    SetValue( type_.GetName() );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObstacle destructor
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
ActionParameterObstacle::~ActionParameterObstacle()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObstacle::ReadParameter
// Created: SBO 2007-05-25
// -----------------------------------------------------------------------------
void ActionParameterObstacle::ReadParameter( xml::xistream& xis, const CoordinateConverter_ABC& converter, const Resolver_ABC< Automat_ABC >& automats )
{
    std::string type;
    xis >> attribute( "type", type );
    if( type == "obstacletype" && type_.CanBeReservedObstacle() )
        AddParameter( *new ActionParameterObstacleType( xis ) );
    else if( type == "location" )
        AddParameter( *new ActionParameterLocation( OrderParameter( tools::translate( "ActionParameter", "Location" ).ascii(), "location", false ), converter, xis ) );
    else if( type == "tc2" )
        AddParameter( *new ActionParameterAutomat( xis, automats ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObstacle::AddObstacleType
// Created: SBO 2007-05-25
// -----------------------------------------------------------------------------
void ActionParameterObstacle::AddObstacleType( unsigned int type )
{
    const OrderParameter orderParameter( tools::translate( "ActionParameter", "Obstacle type" ).ascii(), "obstacletype", false );
    AddParameter( *new ActionParameterObstacleType( orderParameter, type ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObstacle::Draw
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
void ActionParameterObstacle::Draw( const geometry::Point2f& where, const Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    ActionParameter< std::string >::Draw( where, viewport, tools );
    ObjectIcons::Draw( type_.GetId(), GetPosition(), viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObstacle::Serialize
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void ActionParameterObstacle::Serialize( xml::xostream& xos ) const
{
    ActionParameter< std::string >::Serialize( xos );
    xos << attribute( "value", type_.id_ );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObstacle::SetParameters
// Created: SBO 2007-04-17
// -----------------------------------------------------------------------------
void ActionParameterObstacle::SetParameters( const ASN1T_PlannedWork& asn, const Resolver_ABC< Automat_ABC >& automats )
{
    switch( asn.type )
    {
    case EnumObjectType::camp_prisonniers:
    case EnumObjectType::camp_refugies:
        {
            const OrderParameter param( tools::translate( "ActionParameter", "TC2" ).ascii(), "tc2", false );
            AddParameter( *new ActionParameterAutomat( param, asn.tc2, automats ) );
            break;
        }
    case EnumObjectType::zone_minee_lineaire:
    case EnumObjectType::zone_minee_par_dispersion:
        {
            const OrderParameter param( tools::translate( "ActionParameter", "Density" ).ascii(), "density", false );
            AddParameter( *new ActionParameterNumeric( param, asn.densite ) );
            break;
        }
    default:
        ;
    }
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObstacle::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ActionParameterObstacle::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = 0;
    asn.value.t = T_MissionParameter_value_plannedWork;
    asn.value.u.plannedWork = new ASN1T_PlannedWork();
    CommitTo( *asn.value.u.plannedWork );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObstacle::Clean
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ActionParameterObstacle::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.plannedWork )
        Clean( *asn.value.u.plannedWork );
    delete asn.value.u.plannedWork;
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObstacle::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ActionParameterObstacle::CommitTo( ASN1T_PlannedWork& asn ) const
{
    asn.type = ASN1T_EnumObjectType( type_.id_ );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        const std::string type = it->second->GetType();
        if( type == "location" )
            static_cast< const ActionParameterLocation* >( it->second )->CommitTo( asn.position );
        else if( type == "obstacletype" )
            static_cast< const ActionParameterObstacleType* >( it->second )->CommitTo( asn.type_obstacle );
        else if( type == "density" )
            static_cast< const ActionParameterNumeric* >( it->second )->CommitTo( asn.densite );
        else if( type == "tc2" )
            static_cast< const ActionParameterAutomat* >( it->second )->CommitTo( asn.tc2 );
    }
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObstacle::Clean
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ActionParameterObstacle::Clean( ASN1T_PlannedWork& asn ) const
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        const std::string type = it->second->GetType();
        if( type == "location" )
            static_cast< const ActionParameterLocation* >( it->second )->Clean( asn.position );
    }
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObstacle::Accept
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ActionParameterObstacle::Accept( ActionParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
    ActionParameter< std::string >::Accept( visitor );
}
