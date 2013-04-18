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
#include "DateTime.h"
#include "Location.h"
#include "Numeric.h"
#include "ObstacleType.h"
#include "String.h"
#include "ParameterVisitor_ABC.h"
#include "clients_kernel/EntityResolver_ABC.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/ObjectIcons.h"
#include "clients_kernel/Tools.h"
#include "protocol/Protocol.h"
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
    AddParameter( *new Location( OrderParameter( tools::translate( "Parameter", "Obstacle location" ).toAscii().constData(), "location", false ), converter, message.position() ) );
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
        const OrderParameter param( tools::translate( "ActionParameter", "Obstacle tc2" ).toAscii().constData(), "tc2", false );
        AddParameter( *new Automat( param, message.combat_train().id(), entities, controller ) );
    }
    if( message.density() != 0 )
    {
        const OrderParameter param( tools::translate( "ActionParameter", "Density" ).toAscii().constData(), "density", false );
        AddParameter( *new Numeric( param, message.density() ) );
    }
    if( message.has_name() && !message.name().empty() )
    {
        AddParameter( *new String( OrderParameter( tools::translate( "Parameter", "Name" ).toAscii().constData(), "string", true ), message.name() ) );
    }
    if( message.has_activation_time() )
    {
        const OrderParameter param( tools::translate( "gui::ObstaclePrototype_ABC", "Activation time:" ).toAscii().constData(), "integer", true );
        Numeric* numeric = new Numeric( param, static_cast< float >( message.activation_time() ) );
        numeric->SetIdentifier( "ActivationTime" );
        AddParameter( *numeric );
    }
    if( message.has_activity_time() )
    {
        const OrderParameter param( tools::translate( "gui::ObstaclePrototype_ABC", "Activity time:" ).toAscii().constData(), "integer", true );
        Numeric* numeric = new Numeric( param, static_cast< float >( message.activity_time() ) );
        numeric->SetIdentifier( "ActivityTime" );
        AddParameter( *numeric );
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
    if( type == "obstacletype" )
        AddParameter( *new ObstacleType( xis ) );
    else if( type == "location" || type == "circle" || type == "rectangle" || type == "point" || type == "polygon" || type == "line" )
        AddParameter( *new Location( OrderParameter( tools::translate( "Parameter", "Obstacle location" ).toAscii().constData(), "location", false ), converter, xis ) );
    else if( type == "tc2" )
        AddParameter( *new Automat( xis, entities, controller ) );
    else if( type == "string" )
        AddParameter( *new String( OrderParameter( tools::translate( "Parameter", "Name" ).toAscii().constData(), "string", true ), xis ) );
    else if( type == "Integer" )
    {
        std::string identifier = xis.attribute( "identifier", std::string() );
        if( identifier == "ActivityTime" )
        {
            Numeric* numeric = new Numeric( OrderParameter( tools::translate( "gui::ObstaclePrototype_ABC", "Activity time:" ).toAscii().constData(), "integer", true ), xis );
            numeric->SetIdentifier( identifier );
            AddParameter( *numeric );
        }
        else if( identifier == "ActivationTime" )
        {
            Numeric* numeric = new Numeric( OrderParameter( tools::translate( "gui::ObstaclePrototype_ABC", "Activation time:" ).toAscii().constData(), "integer", true ), xis );
            numeric->SetIdentifier( identifier );
            AddParameter( *numeric );
        }
        else if( identifier == "Density" )  
        {
            Numeric* numeric = new Numeric( OrderParameter( tools::translate( "actions::gui::ParamObstacle", "Density per 100 square meter" ).toAscii().constData(), "float", true ), xis );
            numeric->SetIdentifier( identifier );
            AddParameter( *numeric );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: Obstacle::Draw
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
void EngineerConstruction::Draw( const geometry::Point2f& where, const Viewport_ABC& viewport, const GlTools_ABC& tools ) const
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
        const std::string type = it->second->GetType();
        if( type == "location" || type == "circle" || type == "rectangle" || type == "point" || type == "polygon" || type == "line" )
            static_cast< const Location* >( it->second )->CommitTo( *message.mutable_position() );
        else if( type == "obstacletype" )
            static_cast< const ObstacleType* >( it->second )->CommitTo( boost::bind( &sword::PlannedWork::set_type_obstacle, boost::ref( message ), _1 ) );
        else if( type == "float" )
            static_cast< const Numeric* >( it->second )->CommitTo( boost::bind( &sword::PlannedWork::set_density, boost::ref( message ), _1 ) );
        else if( type == "tc2" || type == "automate" || type == "automat" )
            //static_cast< const Automat* >( it->second )->CommitTo( boost::bind( &sword::AutomatId::set_id, boost::ref( *message.mutable_combat_train() ), _1 ) );
            message.mutable_combat_train()->set_id( static_cast< const Automat* >( it->second )->GetValue()->GetId() );
        else if( type == "string" )
            static_cast< const String* >( it->second )->CommitTo( *message.mutable_name() );
        else if( type == "integer" )
        {
            const std::string name = it->second->GetIdentifier();
            if( name == "ActivityTime" )
                static_cast< const Numeric* >( it->second )->CommitTo( boost::bind( &sword::PlannedWork::set_activity_time, boost::ref( message ), _1 ) );
            else if( name == "ActivationTime" )
                static_cast< const Numeric* >( it->second )->CommitTo( boost::bind( &sword::PlannedWork::set_activation_time, boost::ref( message ), _1 ) );
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
