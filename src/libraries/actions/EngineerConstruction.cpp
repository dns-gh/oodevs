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
EngineerConstruction::EngineerConstruction( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const tools::Resolver_ABC< ObjectType, std::string >& types, const kernel::EntityResolver_ABC& entities, const sword::PlannedWork& message, kernel::Controller& controller )
    : Parameter< std::string >( parameter )
    , type_( &types.Get( message.type() ) )
{
    AddParameter( *new Location( OrderParameter( tools::translate( "Parameter", "Construction location" ).toStdString(), "location", false ), converter, message.position() ) );
    SetValue( type_->GetName() );
    SetParameters( message, entities, controller );
}

// -----------------------------------------------------------------------------
// Name: EngineerConstruction::SetParameters
// Created: LDC 2009-04-01
// -----------------------------------------------------------------------------
void EngineerConstruction::SetParameters( const sword::PlannedWork& message, const kernel::EntityResolver_ABC& entities, kernel::Controller& controller )
{
    if( message.combat_train().id() != 0 )
    {
        const OrderParameter param( tools::translate( "ActionParameter", "Obstacle tc2" ).toStdString(), "tc2", false );
        AddParameter( *new Automat( param, message.combat_train().id(), entities, controller ) );
    }
    if( message.density() != 0 )
    {
        const OrderParameter param( tools::translate( "ActionParameter", "Density" ).toStdString(), "float", false );
        AddParameter( *new Numeric( param, message.density() ) );
    }
    if( message.has_name() && !message.name().empty() )
    {
        AddParameter( *new String( OrderParameter( tools::translate( "Parameter", "Name" ).toStdString(), "string", true ), message.name() ) );
    }
    if( message.has_activation_time() )
    {
        Numeric* numeric = new Numeric( OrderParameter( tools::translate( "gui::ObstaclePrototype_ABC", "Activation time:" ).toStdString(), "integer", true ), static_cast< float >( message.activation_time() ) );
        numeric->SetKeyName( "ActivationTime" );
        AddParameter( *numeric );
    }
    if( message.has_activity_time() )
    {
        Numeric* numeric = new Numeric( OrderParameter( tools::translate( "gui::ObstaclePrototype_ABC", "Activity time:" ).toStdString(), "integer", true ), static_cast< float >( message.activity_time() ) );
        numeric->SetKeyName( "ActivityTime" );
        AddParameter( *numeric );
    }
    if( message.altitude_modifier() != 0 )
    {
        const OrderParameter param( tools::translate( "ActionParameter", "Altitude modifier" ).toStdString(), "integer", false );
        AddParameter( *new Quantity( param, message.altitude_modifier() ) );
    }
    if( message.mining() != false )
    {
        const OrderParameter param( tools::translate( "ActionParameter", "Obstacle mining" ).toStdString(), "boolean", false );
        AddParameter( *new Bool( param, message.mining() ) );
    }
    if( message.time_limit() > 0 )
    {
        const OrderParameter param( tools::translate( "ActionParameter", "Time limit" ).toStdString(), "integer", false );
        AddParameter( *new Numeric( param, static_cast< float >( message.time_limit() ) ) );
    }
    if( message.lodging() > 0 )
    {
        const OrderParameter param( tools::translate( "ActionParameter", "Lodging" ).toStdString(), "integer", false );
        AddParameter( *new Quantity( param, message.lodging() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: EngineerConstruction constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
EngineerConstruction::EngineerConstruction( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const tools::Resolver_ABC< ObjectType, std::string >& types, const kernel::EntityResolver_ABC& entities, xml::xistream& xis, kernel::Controller& controller )
    : Parameter< std::string >( parameter )
    , type_( 0 )
{
    if( xis.has_attribute( "value" ) )
        type_ = &types.Get( xis.attribute< std::string >( "value" ) );
    xis >> xml::list( "parameter", *this, &EngineerConstruction::ReadParameter, converter, entities, controller );
    if( type_ )
        SetValue( type_->GetName() );
}

// -----------------------------------------------------------------------------
// Name: EngineerConstruction constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
EngineerConstruction::EngineerConstruction( const CoordinateConverter_ABC& converter, const tools::Resolver_ABC< ObjectType, std::string >& types, const kernel::EntityResolver_ABC& entities, xml::xistream& xis, kernel::Controller& controller )
    : Parameter< std::string >( OrderParameter( xis.attribute< std::string >( "name" ).c_str(), "genobject", false ) )
    , type_( 0 )
{
    if( xis.has_attribute( "value" ) )
        type_ = &types.Get( xis.attribute< std::string >( "value" ) );
    xis >> xml::list( "parameter", *this, &EngineerConstruction::ReadParameter, converter, entities, controller );
    if( type_ )
        SetValue( type_->GetName() );
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
    std::string type = xis.attribute< std::string >( "type" );
    boost::algorithm::to_lower( type );
    if( type == "obstacletype" )
        AddParameter( *new ObstacleType( xis ) );
    else if( type == "location" || type == "circle" || type == "rectangle" || type == "point" || type == "polygon" || type == "line" )
        AddParameter( *new Location( OrderParameter( tools::translate( "Parameter", "Construction location" ).toStdString(), "location", false ), converter, xis ) );
    else if( type == "tc2" )
        AddParameter( *new Automat( xis, entities, controller ) );
    else if( type == "string" )
        AddParameter( *new String( OrderParameter( tools::translate( "Parameter", "Name" ).toStdString(), "string", true ), xis ) );
    else if( type == "quantity" || type == "integer" || type == "numeric" )
    {
        std::string identifier = xis.attribute( "identifier", std::string() );
        boost::algorithm::to_lower( identifier );
        if( identifier == "altitude_modifier" )
        {
            Quantity* numeric = new Quantity( OrderParameter( tools::translate( "EngineerConstruction", "Altitude modifier" ).toStdString(), "integer", true ), xis );
            numeric->SetKeyName( identifier );
            AddParameter( *numeric );
        }
        else if( identifier == "lodging" )
        {
            Quantity* numeric = new Quantity( OrderParameter( tools::translate( "EngineerConstruction", "Lodging" ).toStdString(), "integer", true ), xis );
            numeric->SetKeyName( identifier );
            AddParameter( *numeric );
        }
        if( identifier == "activitytime" )
        {
            Numeric* numeric = new Numeric( OrderParameter( tools::translate( "gui::ObstaclePrototype_ABC", "Activity time:" ).toStdString(), "integer", true ), xis );
            numeric->SetKeyName( identifier );
            AddParameter( *numeric );
        }
        else if( identifier == "activationtime" )
        {
            Numeric* numeric = new Numeric( OrderParameter( tools::translate( "gui::ObstaclePrototype_ABC", "Activation time:" ).toStdString(), "integer", true ), xis );
            numeric->SetKeyName( identifier );
            AddParameter( *numeric );
        }
        else if( identifier == "density" )
        {
            Numeric* numeric = new Numeric( OrderParameter( tools::translate( "EngineerConstruction", "Density" ).toStdString(), "integer", true ), xis );
            numeric->SetKeyName( identifier );
            AddParameter( *numeric );
        }
    }
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
        CommitTo( *message.mutable_value()->Add()->mutable_plannedwork() );
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
        }
        else if( keyName == "obstacle_mining" && type == "boolean" )
            static_cast< const Bool* >( it->second )->CommitTo( boost::bind( &sword::PlannedWork::set_mining, boost::ref( message ), _1 ) );
        else if( keyName == "time_limit" && type == "time" )
            static_cast< const Quantity* >( it->second )->CommitTo( boost::bind( &sword::PlannedWork::set_time_limit, boost::ref( message ), _1 )  );
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
