// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "actions_pch.h"
#include "EngineerConstruction.h"

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
// Name: EngineerConstruction constructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
EngineerConstruction::EngineerConstruction( const OrderParameter& parameter, const ObjectType& type )
    : Parameter< std::string >( parameter, type.GetName() )
    , type_( type )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EngineerConstruction constructor
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
EngineerConstruction::EngineerConstruction( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const Resolver_ABC< ObjectType, std::string >& types, const Resolver_ABC< Automat_ABC >& automats, const ASN1T_PlannedWork& asn, kernel::Controller& controller )
    : Parameter< std::string >( parameter )
    , type_( types.Get( asn.type ) )
{
    AddParameter( *new Location( OrderParameter( tools::translate( "Parameter", "Location" ).ascii(), "location", false ), converter, asn.position ) );
    SetValue( type_.GetName() );
// TODO    SetParameters( asn, automats, controller );
}

// -----------------------------------------------------------------------------
// Name: EngineerConstruction constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
EngineerConstruction::EngineerConstruction( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const Resolver_ABC< ObjectType, std::string >& types, const Resolver_ABC< Automat_ABC >& automats, xml::xistream& xis, kernel::Controller& controller )
    : Parameter< std::string >( parameter )
    , type_( types.Get( xml::attribute< std::string >( xis, "value" ) ) )
{
    xis >> list( "parameter", *this, &EngineerConstruction::ReadParameter, converter, automats, controller );
    SetValue( type_.GetName() );
}

// -----------------------------------------------------------------------------
// Name: EngineerConstruction constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
EngineerConstruction::EngineerConstruction( const CoordinateConverter_ABC& converter, const Resolver_ABC< ObjectType, std::string >& types, const Resolver_ABC< Automat_ABC >& automats, xml::xistream& xis, kernel::Controller& controller )
    : Parameter< std::string >( OrderParameter( xml::attribute< std::string >( xis, "name" ).c_str(), "obstacle", false ) )
    , type_ ( types.Get( xml::attribute< std::string >( xis, "value" ) ) )
{
    xis >> list( "parameter", *this, &EngineerConstruction::ReadParameter, converter, automats, controller );
    SetValue( type_.GetName() );
}

// -----------------------------------------------------------------------------
// Name: EngineerConstruction destructor
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
EngineerConstruction::~EngineerConstruction()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EngineerConstruction::ReadParameter
// Created: SBO 2007-05-25
// -----------------------------------------------------------------------------
void EngineerConstruction::ReadParameter( xml::xistream& xis, const CoordinateConverter_ABC& converter, const Resolver_ABC< Automat_ABC >& automats, Controller& controller )
{
    std::string type;
    xis >> attribute( "type", type );
    if( type == "obstacletype" )
        AddParameter( *new ObstacleType( xis ) );
    else if( type == "location" )
        AddParameter( *new Location( OrderParameter( tools::translate( "Parameter", "Location" ).ascii(), "location", false ), converter, xis ) );
    else if( type == "tc2" )
        AddParameter( *new Automat( xis, automats, controller ) );
}

// -----------------------------------------------------------------------------
// Name: EngineerConstruction::AddEngineerConstructionType
// Created: SBO 2007-05-25
// -----------------------------------------------------------------------------
// TODO
//void EngineerConstruction::AddObstacleType( const std::string& type )
//{
//    const OrderParameter orderParameter( tools::translate( "Parameter", "Obstacle type" ).ascii(), "obstacletype", false );
//    AddParameter( *new EngineerConstructionType( orderParameter, type ) );
//}

// -----------------------------------------------------------------------------
// Name: Obstacle::Draw
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
void EngineerConstruction::Draw( const geometry::Point2f& where, const Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    Parameter< std::string >::Draw( where, viewport, tools );
    ObjectIcons::Draw( type_, GetPosition(), viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: EngineerConstruction::Serialize
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void EngineerConstruction::Serialize( xml::xostream& xos ) const
{
    Parameter< std::string >::Serialize( xos );
    xos << attribute( "value", type_.GetName() );
}

// -----------------------------------------------------------------------------
// Name: EngineerConstruction::CommitTo
// Created: JCR 2008-11-03
// -----------------------------------------------------------------------------
void EngineerConstruction::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = 0;    
    asn.value.t = T_MissionParameter_value_plannedWork;
    asn.value.u.plannedWork = new ASN1T_PlannedWork();
    CommitTo( *asn.value.u.plannedWork );
}
    
// -----------------------------------------------------------------------------
// Name: EngineerConstruction::Clean
// Created: JCR 2008-11-03
// -----------------------------------------------------------------------------
void EngineerConstruction::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.plannedWork )
        Clean( *asn.value.u.plannedWork );
    delete asn.value.u.plannedWork;
}

// -----------------------------------------------------------------------------
// Name: EngineerConstruction::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void EngineerConstruction::CommitTo( ASN1T_PlannedWork& asn ) const
{    
    asn.type = type_.GetName().c_str();
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
// Name: EngineerConstruction::Clean
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void EngineerConstruction::Clean( ASN1T_PlannedWork& asn ) const
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        const std::string type = it->second->GetType();
        if( type == "location" )
            static_cast< const Location* >( it->second )->Clean( asn.position );
    }
}

// -----------------------------------------------------------------------------
// Name: EngineerConstruction::Accept
// Created: JCR 2008-11-03
// -----------------------------------------------------------------------------
void EngineerConstruction::Accept( ParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
    Parameter< std::string >::Accept( visitor );
}
