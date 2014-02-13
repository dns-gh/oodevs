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
    if( message.combat_train().id() != 0 )
        AddParam( *new Automat(      OrderParameter( tools::translate( "ActionParameter", "Obstacle tc2" ).toStdString(), "tc2", false ), message.combat_train().id(), entities, controller ), "tc2" );
    if( message.has_type_obstacle() )
        AddParam( *new ObstacleType( OrderParameter( tools::translate( "ActionParameter", "Activation" ).toStdString(), "obstacletype", false ), static_cast< unsigned int >( message.type_obstacle() ) ), "obstacletype" );
    if( message.has_name() && !message.name().empty() )
        AddParam( *new String(       OrderParameter( tools::translate( "ActionParameter", "Name" ).toStdString(), "string", true ), message.name() ), "name" );
    if( message.mining() != false )
        AddParam( *new Bool(         OrderParameter( tools::translate( "ActionParameter", "Obstacle mining" ).toStdString(), "boolean", false ), message.mining() ), "obstacle_mining" );
    if( message.density() != 0 )
        AddParam( *new Numeric(      OrderParameter( tools::translate( "ActionParameter", "Density" ).toStdString(), "float", false ), message.density() ), "density" );
    if( message.has_activation_time() )
        AddParam( *new Numeric(      OrderParameter( tools::translate( "ActionParameter", "Activation time:" ).toStdString(), "integer", true ), static_cast< float >( message.activation_time() ) ), "activationtime" );
    if( message.has_activity_time() )
        AddParam( *new Numeric(      OrderParameter( tools::translate( "ActionParameter", "Activity time:" ).toStdString(), "integer", true ), static_cast< float >( message.activity_time() ) ), "activitytime" );
    if( message.time_limit() > 0 )
        AddParam( *new Numeric(      OrderParameter( tools::translate( "ActionParameter", "Time limit" ).toStdString(), "integer", false ), static_cast< float >( message.time_limit() ) ), "time_limit" );
    if( message.altitude_modifier() != 0 )
        AddParam( *new Quantity(     OrderParameter( tools::translate( "ActionParameter", "Altitude modifier" ).toStdString(), "integer", false ), message.altitude_modifier() ), "altitude_modifier" );
    if( message.lodging() > 0 )
        AddParam( *new Quantity(     OrderParameter( tools::translate( "ActionParameter", "Lodging" ).toStdString(), "integer", false ), message.lodging() ), "lodging" );
    if( message.has_fire_class() && !message.fire_class().empty() )
        AddParam( *new FireClass( OrderParameter( tools::translate( "ActionParameter", "Fire class:" ).toStdString(), "fireclass", false ), message.fire_class(), resolver ), "fire_class" );
    if( message.max_combustion() > 0 )
        AddParam( *new Quantity(     OrderParameter( tools::translate( "ActionParameter", "Max combustion energy:" ).toStdString(), "integer", false ), message.max_combustion() ), "max_combustion_energy" );
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
//    if( const kernel::Location_ABC* location = GetLocation() && type_ ) // $$$$ SBO 2009-06-05: TODO
//        tools.DrawTacticalGraphics( type_->GetSymbol(), *location, tools.ShouldDisplay() );
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
        const std::string type = boost::algorithm::to_lower_copy( it->second->GetType() );

        if( type == "location" || type == "circle" || type == "rectangle" || type == "point" || type == "polygon" || type == "line" )
            static_cast< const Location* >( it->second )->CommitTo( *message.mutable_position() );
        else if( type == "obstacletype" )
            static_cast< const ObstacleType* >( it->second )->CommitTo( boost::bind( &sword::PlannedWork::set_type_obstacle, boost::ref( message ), _1 ) );
        else if( keyName == "density" && type == "float" )
            static_cast< const Numeric* >( it->second )->CommitTo( boost::bind( &sword::PlannedWork::set_density, boost::ref( message ), _1 ) );
        else if( type == "tc2" || type == "automat" || type == "automate" )
            message.mutable_combat_train()->set_id( static_cast< const Automat* >( it->second )->GetValue()->GetId() );
        else if( type == "string" )
            static_cast< const String* >( it->second )->CommitTo( *message.mutable_name() );
        else if( type == "integer" || type == "quantity" )
        {
            if( keyName == "activitytime" )
                message.set_activity_time( static_cast< int >( static_cast< const Numeric* >( it->second )->GetValue() ) );
            else if( keyName == "activationtime" )
                message.set_activation_time( static_cast< int >( static_cast< const Numeric* >( it->second )->GetValue() ) );
            else if( keyName == "altitude_modifier" )
                static_cast< const Quantity* >( it->second )->CommitTo( boost::bind( &sword::PlannedWork::set_altitude_modifier, boost::ref( message ), _1 ) );
            else if( keyName == "lodging" )
                static_cast< const Quantity* >( it->second )->CommitTo( boost::bind( &sword::PlannedWork::set_lodging, boost::ref( message ), _1 ) );
            else if( keyName == "max_combustion_energy" )
                static_cast< const Quantity* >( it->second )->CommitTo( boost::bind( &sword::PlannedWork::set_max_combustion, boost::ref( message ), _1 ) );
        }
        else if( keyName == "obstacle_mining" && type == "boolean" )
            static_cast< const Bool* >( it->second )->CommitTo( boost::bind( &sword::PlannedWork::set_mining, boost::ref( message ), _1 ) );
        else if( keyName == "time_limit" && type == "time" )
            static_cast< const Quantity* >( it->second )->CommitTo( boost::bind( &sword::PlannedWork::set_time_limit, boost::ref( message ), _1 )  );
        else if( type == "fireclass" )
            message.set_fire_class( static_cast< const FireClass* >( it->second )->GetValue() );
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
