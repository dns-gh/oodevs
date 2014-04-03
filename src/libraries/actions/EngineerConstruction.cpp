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
#include "Bool.h"
#include "DateTime.h"
#include "Location.h"
#include "Numeric.h"
#include "ObstacleType.h"
#include "Quantity.h"
#include "FireClass.h"
#include "String.h"
#include "ParameterVisitor_ABC.h"
#include "clients_kernel/EntityResolver_ABC.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/Tools.h"
#include "protocol/Protocol.h"
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/bind.hpp>
#include <xeumeuleu/xml.hpp>

using namespace kernel;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: EngineerConstruction constructor
// Created: ABR 2011-11-17
// -----------------------------------------------------------------------------
EngineerConstruction::EngineerConstruction( const kernel::OrderParameter& parameter )
    : Parameter< std::string >( parameter )
    , type_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EngineerConstruction constructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
EngineerConstruction::EngineerConstruction( const OrderParameter& parameter, const ObjectType& type )
    : Parameter< std::string >( parameter, type.GetName() )
    , type_( &type )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EngineerConstruction constructor
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
EngineerConstruction::EngineerConstruction( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const tools::Resolver_ABC< ObjectType, std::string >& types,
                                            const kernel::EntityResolver_ABC& entities, const sword::PlannedWork& message, kernel::Controller& controller,
                                            const tools::StringResolver< kernel::FireClass >& resolver )
    : Parameter< std::string >( parameter )
    , type_( &types.Get( message.type() ) )
{
    SetValue( type_->GetType() );
    AddParam( *new Location(         OrderParameter( tools::translate( "ActionParameter", "Construction location" ).toStdString(), "location", false ), converter, message.position() ), "location" );
    if( message.has_type_obstacle() )
        AddParam( *new ObstacleType( OrderParameter( tools::translate( "ActionParameter", "Activation" ).toStdString(), "obstacletype", false ), static_cast< unsigned int >( message.type_obstacle() ) ), "obstacletype" );
    if( message.density() != 0.f )
        AddParam( *new Numeric(      OrderParameter( tools::translate( "ActionParameter", "Density" ).toStdString(), "float", false ), message.density() ), "density" );
    if( message.combat_train().id() != 0 )
        AddParam( *new Automat(      OrderParameter( tools::translate( "ActionParameter", "Obstacle tc2" ).toStdString(), "automat", false ), message.combat_train().id(), entities, controller ), "tc2" );
    if( message.activity_time() != 0 )
        AddParam( *new Quantity(     OrderParameter( tools::translate( "ActionParameter", "Activity time:" ).toStdString(), "quantity", true ), message.activity_time() ), "activitytime" );
    if( message.activation_time() != 0 )
        AddParam( *new Quantity(     OrderParameter( tools::translate( "ActionParameter", "Activation time:" ).toStdString(), "quantity", true ), message.activation_time() ), "activationtime" );
    if( message.has_name() && !message.name().empty() )
        AddParam( *new String(       OrderParameter( tools::translate( "ActionParameter", "Name" ).toStdString(), "string", true ), message.name() ), "name" );
    if( message.altitude_modifier() != 0 )
        AddParam( *new Quantity(     OrderParameter( tools::translate( "ActionParameter", "Altitude modifier" ).toStdString(), "quantity", false ), message.altitude_modifier() ), "altitude_modifier" );
    if( message.time_limit() > 0 )
        AddParam( *new Quantity(     OrderParameter( tools::translate( "ActionParameter", "Time limit" ).toStdString(), "quantity", false ), message.time_limit() ), "time_limit" );
    if( message.mining() )
        AddParam( *new Bool(         OrderParameter( tools::translate( "ActionParameter", "Obstacle mining" ).toStdString(), "boolean", false ), message.mining() ), "obstacle_mining" );
    if( message.lodging() > 0 )
        AddParam( *new Quantity(     OrderParameter( tools::translate( "ActionParameter", "Lodging" ).toStdString(), "quantity", false ), message.lodging() ), "lodging" );
    if( message.has_fire_class() && !message.fire_class().empty() )
        AddParam( *new FireClass(    OrderParameter( tools::translate( "ActionParameter", "Fire class:" ).toStdString(), "fireclass", false ), message.fire_class(), resolver ), "fire_class" );
    if( message.max_combustion() > 0 )
        AddParam( *new Quantity(     OrderParameter( tools::translate( "ActionParameter", "Max combustion energy:" ).toStdString(), "quantity", false ), message.max_combustion() ), "max_combustion_energy" );
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
// Name: EngineerConstruction::AddParam
// Created: ABR 2013-06-18
// -----------------------------------------------------------------------------
void EngineerConstruction::AddParam( Parameter_ABC& parameter, const std::string& keyname )
{
    parameter.SetKeyName( keyname );
    Parameter< std::string >::AddParameter( parameter );
}

// -----------------------------------------------------------------------------
// Name: Obstacle::Draw
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
void EngineerConstruction::Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const
{
    Parameter< std::string >::Draw( where, viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: EngineerConstruction::Serialize
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void EngineerConstruction::Serialize( xml::xostream& xos ) const
{
    Parameter< std::string >::Serialize( xos );
    if( IsSet() && type_ )
        xos << xml::attribute( "value", type_->GetType() );
}

// -----------------------------------------------------------------------------
// Name: EngineerConstruction::CommitTo
// Created: JCR 2008-11-03
// -----------------------------------------------------------------------------
void EngineerConstruction::CommitTo( sword::MissionParameter& message ) const
{
    message.set_null_value( !IsSet() );
    if( IsSet() )
        CommitTo( *message.add_value()->mutable_plannedwork() );
}
// -----------------------------------------------------------------------------
// Name: EngineerConstruction::CommitTo
// Created: JCR 2008-11-03
// -----------------------------------------------------------------------------
void EngineerConstruction::CommitTo( sword::MissionParameter_Value& message ) const
{
    if( IsSet() )
        CommitTo( *message.mutable_plannedwork() );
}

// -----------------------------------------------------------------------------
// Name: EngineerConstruction::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void EngineerConstruction::CommitTo( sword::PlannedWork& message ) const
{
    if( !IsSet() || !type_ )
        return;
    message.set_type( type_->GetType().c_str() );
    for( auto it = elements_.begin(); it != elements_.end(); ++it )
    {
        const std::string keyName = boost::algorithm::to_lower_copy( it->second->GetKeyName() );
        if( keyName == "location" )
            static_cast< const Location* >( it->second )->CommitTo( *message.mutable_position() );
        else if( keyName == "obstacletype" )
            static_cast< const ObstacleType* >( it->second )->CommitTo( boost::bind( &sword::PlannedWork::set_type_obstacle, boost::ref( message ), _1 ) );
        else if( keyName == "density" )
            static_cast< const Numeric* >( it->second )->CommitTo( boost::bind( &sword::PlannedWork::set_density, boost::ref( message ), _1 ) );
        else if( keyName == "tc2" )
            message.mutable_combat_train()->set_id( static_cast< const Automat* >( it->second )->GetValue()->GetId() );
        else if( keyName == "name" )
            static_cast< const String* >( it->second )->CommitTo( *message.mutable_name() );
        else if( keyName == "obstacle_mining" )
            static_cast< const Bool* >( it->second )->CommitTo( boost::bind( &sword::PlannedWork::set_mining, boost::ref( message ), _1 ) );
        else if( keyName == "fire_class" )
            message.set_fire_class( static_cast< const FireClass* >( it->second )->GetValue() );
        else
        {
            const Quantity& quantity = static_cast< const Quantity& >( *it->second );
            if( keyName == "activitytime" )
                message.set_activity_time( quantity.GetValue() );
            else if( keyName == "activationtime" )
                message.set_activation_time( quantity.GetValue() );
            else if( keyName == "altitude_modifier" )
                message.set_altitude_modifier( quantity.GetValue() );
            else if( keyName == "lodging" )
                message.set_lodging( quantity.GetValue() );
            else if( keyName == "max_combustion_energy" )
                message.set_max_combustion( quantity.GetValue() );
            else if( keyName == "time_limit" )
                message.set_time_limit( quantity.GetValue() );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: EngineerConstruction::Clean
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void EngineerConstruction::Clean( sword::PlannedWork& message ) const
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
    for( auto it = elements_.begin(); it != elements_.end(); ++it )
        if( !it->second->IsSet() && !it->second->IsOptional() )
            return false;
    return !elements_.empty();
}

// -----------------------------------------------------------------------------
// Name: EngineerConstruction::SerializeType
// Created: LDC 2011-05-13
// -----------------------------------------------------------------------------
std::string EngineerConstruction::SerializeType() const
{
    return "plannedwork";
}
