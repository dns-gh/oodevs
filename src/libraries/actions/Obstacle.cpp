// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "Obstacle.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/ObjectIcons.h"
#include "clients_kernel/Tools.h"
#include "Location.h"
#include "Automat.h"
#include "Numeric.h"
#include "ObstacleType.h"
#include "ParameterVisitor_ABC.h"
#include <xeumeuleu/xml.h>

using namespace kernel;
using namespace xml;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: Obstacle constructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
Obstacle::Obstacle( const OrderParameter& parameter, const ObjectType& type )
    : Parameter< std::string >( parameter, type.GetName() )
    , type_( type )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Obstacle constructor
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
Obstacle::Obstacle( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const Resolver_ABC< ObjectType >& types, const Resolver_ABC< Automat_ABC >& automats, const ASN1T_PlannedWork& asn, kernel::Controller& controller )
    : Parameter< std::string >( parameter )
    , type_( types.Get( asn.type ) )
{
    AddParameter( *new Location( OrderParameter( tools::translate( "Parameter", "Location" ).ascii(), "location", false ), converter, asn.position ) );
    SetValue( type_.GetName() );
    SetParameters( asn, automats, controller );
}

// -----------------------------------------------------------------------------
// Name: Obstacle constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
Obstacle::Obstacle( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const Resolver_ABC< ObjectType >& types, const Resolver_ABC< Automat_ABC >& automats, xml::xistream& xis, kernel::Controller& controller )
    : Parameter< std::string >( parameter )
    , type_( types.Get( xml::attribute< unsigned int >( xis, "value" ) ) )
{
    xis >> list( "parameter", *this, &Obstacle::ReadParameter, converter, automats, controller );
    SetValue( type_.GetName() );
}

// -----------------------------------------------------------------------------
// Name: Obstacle constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
Obstacle::Obstacle( const CoordinateConverter_ABC& converter, const Resolver_ABC< ObjectType >& types, const Resolver_ABC< Automat_ABC >& automats, xml::xistream& xis, kernel::Controller& controller )
    : Parameter< std::string >( OrderParameter( xml::attribute< std::string >( xis, "name" ).c_str(), "obstacle", false ) )
    , type_( types.Get( xml::attribute< unsigned int >( xis, "value" ) ) )
{
    xis >> list( "parameter", *this, &Obstacle::ReadParameter, converter, automats, controller );
    SetValue( type_.GetName() );
}

// -----------------------------------------------------------------------------
// Name: Obstacle destructor
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
Obstacle::~Obstacle()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Obstacle::ReadParameter
// Created: SBO 2007-05-25
// -----------------------------------------------------------------------------
void Obstacle::ReadParameter( xml::xistream& xis, const CoordinateConverter_ABC& converter, const Resolver_ABC< Automat_ABC >& automats, Controller& controller )
{
    std::string type;
    xis >> attribute( "type", type );
    if( type == "obstacletype" && type_.CanBeReservedObstacle() )
        AddParameter( *new ObstacleType( xis ) );
    else if( type == "location" )
        AddParameter( *new Location( OrderParameter( tools::translate( "Parameter", "Location" ).ascii(), "location", false ), converter, xis ) );
    else if( type == "tc2" || type == "automat" )
        AddParameter( *new Automat( xis, automats, controller ) );
}

// -----------------------------------------------------------------------------
// Name: Obstacle::AddObstacleType
// Created: SBO 2007-05-25
// -----------------------------------------------------------------------------
void Obstacle::AddObstacleType( unsigned int type )
{
    const OrderParameter orderParameter( tools::translate( "Parameter", "Obstacle type" ).ascii(), "obstacletype", false );
    AddParameter( *new ObstacleType( orderParameter, type ) );
}

// -----------------------------------------------------------------------------
// Name: Obstacle::Draw
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
void Obstacle::Draw( const geometry::Point2f& where, const Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    Parameter< std::string >::Draw( where, viewport, tools );
    ObjectIcons::Draw( type_.GetId(), GetPosition(), viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: Obstacle::Serialize
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void Obstacle::Serialize( xml::xostream& xos ) const
{
    Parameter< std::string >::Serialize( xos );
    xos << attribute( "value", type_.id_ );
}

// -----------------------------------------------------------------------------
// Name: Obstacle::SetParameters
// Created: SBO 2007-04-17
// -----------------------------------------------------------------------------
void Obstacle::SetParameters( const ASN1T_PlannedWork& asn, const Resolver_ABC< Automat_ABC >& automats, kernel::Controller& controller )
{
    switch( asn.type )
    {
    case EnumObjectType::camp_prisonniers:
    case EnumObjectType::camp_refugies:
        {
            const OrderParameter param( tools::translate( "Parameter", "TC2" ).ascii(), "tc2", false );
            AddParameter( *new Automat( param, asn.tc2, automats, controller ) );
            break;
        }
    case EnumObjectType::zone_minee_lineaire:
    case EnumObjectType::zone_minee_par_dispersion:
        {
            const OrderParameter param( tools::translate( "Parameter", "Density" ).ascii(), "density", false );
            AddParameter( *new Numeric( param, asn.densite ) );
            break;
        }
    default:
        ;
    }
}

// -----------------------------------------------------------------------------
// Name: Obstacle::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void Obstacle::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = 0;
    asn.value.t = T_MissionParameter_value_plannedWork;
    asn.value.u.plannedWork = new ASN1T_PlannedWork();
    CommitTo( *asn.value.u.plannedWork );
}

// -----------------------------------------------------------------------------
// Name: Obstacle::Clean
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void Obstacle::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.plannedWork )
        Clean( *asn.value.u.plannedWork );
    delete asn.value.u.plannedWork;
}

// -----------------------------------------------------------------------------
// Name: Obstacle::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void Obstacle::CommitTo( ASN1T_PlannedWork& asn ) const
{
    asn.type = ASN1T_EnumObjectType( type_.id_ );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        const std::string type = it->second->GetType();
        if( type == "location" )
            static_cast< const Location* >( it->second )->CommitTo( asn.position );
        else if( type == "obstacletype" )
            static_cast< const ObstacleType* >( it->second )->CommitTo( asn.type_obstacle );
        else if( type == "density" )
            static_cast< const Numeric* >( it->second )->CommitTo( asn.densite );
        else if( type == "tc2" || type == "automat" )
            static_cast< const Automat* >( it->second )->CommitTo( asn.tc2 );
    }
}

// -----------------------------------------------------------------------------
// Name: Obstacle::Clean
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void Obstacle::Clean( ASN1T_PlannedWork& asn ) const
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        const std::string type = it->second->GetType();
        if( type == "location" )
            static_cast< const Location* >( it->second )->Clean( asn.position );
    }
}

// -----------------------------------------------------------------------------
// Name: Obstacle::Accept
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void Obstacle::Accept( ParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
    Parameter< std::string >::Accept( visitor );
}
