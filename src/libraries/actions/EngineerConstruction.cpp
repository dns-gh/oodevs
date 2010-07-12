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
#include "Automat.h"
#include "Location.h"
#include "Numeric.h"
#include "ObstacleType.h"
#include "ParameterVisitor_ABC.h"
#include "clients_kernel/EntityResolver_ABC.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/ObjectIcons.h"
#include "clients_kernel/Tools.h"
#include "protocol/Protocol.h"
#include <boost/bind.hpp>
#include <xeumeuleu/xml.hpp>

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
EngineerConstruction::EngineerConstruction( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const tools::Resolver_ABC< ObjectType, std::string >& types, const kernel::EntityResolver_ABC& entities, const Common::MsgPlannedWork& message, kernel::Controller& controller )
    : Parameter< std::string >( parameter )
    , type_( types.Get( message.type() ) )
{
    AddParameter( *new Location( OrderParameter( tools::translate( "Parameter", "Location" ).ascii(), "location", false ), converter, message.position() ) );
    SetValue( type_.GetName() );
    SetParameters( message, entities, controller );
}

// -----------------------------------------------------------------------------
// Name: EngineerConstruction::SetParameters
// Created: LDC 2009-04-01
// -----------------------------------------------------------------------------
void EngineerConstruction::SetParameters( const Common::MsgPlannedWork& message, const kernel::EntityResolver_ABC& entities, kernel::Controller& controller )
{
    if( message.tc2() != 0 )
    {
        const OrderParameter param( tools::translate( "ActionParameter", "TC2" ).ascii(), "tc2", false );
        AddParameter( *new Automat( param, message.tc2(), entities, controller ) );
    }
    if( message.densite() != 0 )
    {
        const OrderParameter param( tools::translate( "ActionParameter", "Density" ).ascii(), "density", false );
        AddParameter( *new Numeric( param, message.densite() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: EngineerConstruction constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
EngineerConstruction::EngineerConstruction( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const tools::Resolver_ABC< ObjectType, std::string >& types, const kernel::EntityResolver_ABC& entities, xml::xistream& xis, kernel::Controller& controller )
    : Parameter< std::string >( parameter )
    , type_( types.Get( xml::attribute< std::string >( xis, "value" ) ) )
{
    xis >> list( "parameter", *this, &EngineerConstruction::ReadParameter, converter, entities, controller );
    SetValue( type_.GetName() );
}

// -----------------------------------------------------------------------------
// Name: EngineerConstruction constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
EngineerConstruction::EngineerConstruction( const CoordinateConverter_ABC& converter, const tools::Resolver_ABC< ObjectType, std::string >& types, const kernel::EntityResolver_ABC& entities, xml::xistream& xis, kernel::Controller& controller )
    : Parameter< std::string >( OrderParameter( xml::attribute< std::string >( xis, "name" ).c_str(), "obstacle", false ) )
    , type_ ( types.Get( xml::attribute< std::string >( xis, "value" ) ) )
{
    xis >> list( "parameter", *this, &EngineerConstruction::ReadParameter, converter, entities, controller );
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
void EngineerConstruction::ReadParameter( xml::xistream& xis, const CoordinateConverter_ABC& converter, const kernel::EntityResolver_ABC& entities, Controller& controller )
{
    std::string type;
    xis >> attribute( "type", type );
    if( type == "obstacletype" )
        AddParameter( *new ObstacleType( xis ) );
    else if( type == "location" )
        AddParameter( *new Location( OrderParameter( tools::translate( "Parameter", "Location" ).ascii(), "location", false ), converter, xis ) );
    else if( type == "tc2" )
        AddParameter( *new Automat( xis, entities, controller ) );
}

// -----------------------------------------------------------------------------
// Name: Obstacle::Draw
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
void EngineerConstruction::Draw( const geometry::Point2f& where, const Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    Parameter< std::string >::Draw( where, viewport, tools );
//    if( const kernel::Location_ABC* location = GetLocation() ) // $$$$ SBO 2009-06-05: TODO
//        tools.DrawTacticalGraphics( type_.GetSymbol(), *location, tools.ShouldDisplay() );
}

// -----------------------------------------------------------------------------
// Name: EngineerConstruction::Serialize
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void EngineerConstruction::Serialize( xml::xostream& xos ) const
{
    Parameter< std::string >::Serialize( xos );
    xos << attribute( "value", type_.GetType() );
}

// -----------------------------------------------------------------------------
// Name: EngineerConstruction::CommitTo
// Created: JCR 2008-11-03
// -----------------------------------------------------------------------------
void EngineerConstruction::CommitTo( Common::MsgMissionParameter& message ) const
{
    message.set_null_value( 0 );
    message.mutable_value()->mutable_plannedwork();    // enforce initialisation of parameter to force his type
    CommitTo( *message.mutable_value()->mutable_plannedwork() );
}

// -----------------------------------------------------------------------------
// Name: EngineerConstruction::Clean
// Created: JCR 2008-11-03
// -----------------------------------------------------------------------------
void EngineerConstruction::Clean( Common::MsgMissionParameter& message ) const
{
    message.mutable_value()->clear_plannedwork();
}

// -----------------------------------------------------------------------------
// Name: EngineerConstruction::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void EngineerConstruction::CommitTo( Common::MsgPlannedWork& message ) const
{
    message.set_type( type_.GetType().c_str() );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        const std::string type = it->second->GetType();
        if( type == "location" )
            static_cast< const Location* >( it->second )->CommitTo( *message.mutable_position() );
        else if( type == "obstacletype" )
            static_cast< const ObstacleType* >( it->second )->CommitTo( boost::bind( &Common::MsgPlannedWork::set_type_obstacle, boost::ref(message), _1 ) );
        else if( type == "density" )
            static_cast< const Numeric* >( it->second )->CommitTo( boost::bind( &Common::MsgPlannedWork::set_densite, boost::ref(message), _1 ) );
        else if( type == "tc2" || type == "automat" )
            static_cast< const Automat* >( it->second )->CommitTo( boost::bind( &Common::MsgPlannedWork::set_tc2, boost::ref(message), _1 ) );
    }
}

// -----------------------------------------------------------------------------
// Name: EngineerConstruction::Clean
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void EngineerConstruction::Clean( Common::MsgPlannedWork& message ) const
{
    message.Clear();
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

// -----------------------------------------------------------------------------
// Name: EngineerConstruction::IsSet
// Created: SBO 2009-06-16
// -----------------------------------------------------------------------------
bool EngineerConstruction::IsSet() const
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        if( !it->second->IsSet() )
            return false;
    return !elements_.empty();
}
