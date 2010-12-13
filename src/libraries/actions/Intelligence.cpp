// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "Intelligence.h"
#include "String.h"
#include "Bool.h"
#include "Point.h"
#include "Formation.h"
#include "Karma.h"
#include "Level.h"
#include "ParameterVisitor_ABC.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_kernel/Point.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/IntelligenceHierarchies.h"
#include "clients_kernel/Positions.h"
#include "protocol/Protocol.h"
#include <boost/bind.hpp>
#include <xeumeuleu/xml.hpp>

using namespace kernel;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: Intelligence constructor
// Created: SBO 2007-10-24
// -----------------------------------------------------------------------------
Intelligence::Intelligence( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, kernel::Controller& controller )
    : Entity< Intelligence_ABC >( parameter, 0, controller )
    , converter_( converter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Intelligence constructor
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
Intelligence::Intelligence( const CoordinateConverter_ABC& converter, xml::xistream& xis, const kernel::EntityResolver_ABC& formations, const FormationLevels& levels, kernel::Controller& controller )
    : Entity< Intelligence_ABC >( OrderParameter( xis.attribute< std::string >( "name" ).c_str(), "intelligence", false ), 0, controller )
    , converter_( converter )
{
    xis >> xml::list( "parameter", *this, &Intelligence::ReadParameter, formations, levels );
}

namespace
{
    kernel::Point MakePoint( const CoordinateConverter_ABC& converter, const sword::MsgCoordLatLong& message )
    {
        kernel::Point point;
        point.AddPoint( converter.ConvertToXY( message ) );
        return point;
    }
}

// -----------------------------------------------------------------------------
// Name: Intelligence constructor
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------

Intelligence::Intelligence( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const kernel::EntityResolver_ABC& formations, const FormationLevels& levels, const sword::Intelligence& message, kernel::Controller& controller )
    : Entity< Intelligence_ABC >( parameter, 0, controller )
    , converter_( converter )
{
    AddParameter( *new String   ( OrderParameter( tools::translate( "Parameter", "Name" ).ascii()     , "name"     , false ), message.name() ) );
    AddParameter( *new String   ( OrderParameter( tools::translate( "Parameter", "Nature" ).ascii()   , "nature"   , false ), message.nature() ) );
    AddParameter( *new Karma    ( OrderParameter( tools::translate( "Parameter", "Karma" ).ascii()    , "karma"    , false ), message.diplomacy() ) );
    AddParameter( *new Level    ( OrderParameter( tools::translate( "Parameter", "Level" ).ascii()    , "level"    , false ), message.level(), levels ) );
    AddParameter( *new Bool     ( OrderParameter( tools::translate( "Parameter", "Mounted" ).ascii()  , "bool"     , false ), message.embarked() != 0 ) );
    AddParameter( *new Formation( OrderParameter( tools::translate( "Parameter", "Formation" ).ascii(), "formation", false ), message.formation().id(), formations, controller_ ) );
    AddParameter( *new Point    ( OrderParameter( tools::translate( "Parameter", "Intelligence location" ).ascii(), "point"    , false ), converter, MakePoint( converter, message.location() ) ) );
}

// -----------------------------------------------------------------------------
// Name: Intelligence destructor
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
Intelligence::~Intelligence()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Intelligence::ReadParameter
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
void Intelligence::ReadParameter( xml::xistream& xis, const kernel::EntityResolver_ABC& formations, const FormationLevels& levels )
{
    const QString name = xis.attribute< std::string >( "name" ).c_str();
    const std::string type = xis.attribute< std::string >( "type" );

    if( type == "name" || type == "nature" )
        AddParameter( *new String( OrderParameter( name.ascii(), type, false ), xis ) );
    else if( type == "karma" )
        AddParameter( *new Karma( OrderParameter( name.ascii(), type, false ), xis ) );
    else if( type == "level" )
        AddParameter( *new Level( OrderParameter( name.ascii(), type, false ), xis, levels ) );
    else if( type == "bool" )
        AddParameter( *new Bool( OrderParameter( name.ascii(), type, false ), xis ) );
    else if( type == "formation" )
        AddParameter( *new Formation( OrderParameter( name.ascii(), type, false ), xis, formations, controller_ ) );
    else if( type == "point" )
        AddParameter( *new Point( OrderParameter( name.ascii(), type, false ), converter_, xis ) );
}

// -----------------------------------------------------------------------------
// Name: Lima::CommitTo
// Created: MGD 2010-11-12
// -----------------------------------------------------------------------------
void Intelligence::CommitTo( sword::MsgMissionParameter& message ) const
{
    message.set_null_value( !IsSet() );
    if( IsSet() )
    {
        CommitTo( *message.mutable_value()->Add()->mutable_intelligencelist()->add_elem() );
    }
}
// -----------------------------------------------------------------------------
// Name: Lima::CommitTo
// Created: MGD 2010-11-12
// -----------------------------------------------------------------------------
void Intelligence::CommitTo( sword::MsgMissionParameter_Value& message ) const
{
    if( IsSet() )
    {
        CommitTo( *message.mutable_intelligencelist()->add_elem() );
    }
}

// -----------------------------------------------------------------------------
// Name: Intelligence::CommitTo
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
void Intelligence::CommitTo( sword::Intelligence& message ) const
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        const std::string type = it->second->GetType();
        if( type == "name" )
            static_cast< const String* >( it->second )->CommitTo( *message.mutable_name() );
        else if( type == "nature" )
            static_cast< const String* >( it->second )->CommitTo( *message.mutable_nature() );
        else if( type == "level" )
            static_cast< const Level* >( it->second )->CommitTo( boost::bind( &sword::Intelligence::set_level, message, _1 ) );
        else if( type == "karma" )
            static_cast< const Karma* >( it->second )->CommitTo( boost::bind( &sword::Intelligence::set_diplomacy, message, _1 ) );
        else if( type == "bool" )
            static_cast< const Bool* >( it->second )->CommitTo( boost::bind( &sword::Intelligence::set_embarked, message, _1 ) );
        else if( type == "formation" )
            static_cast< const Formation* >( it->second )->CommitTo( *message.mutable_formation() );
        else if( type == "point" )
            static_cast< const Point* >( it->second )->CommitTo( *message.mutable_location() );
    }
}

// -----------------------------------------------------------------------------
// Name: Intelligence::Clean
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
void Intelligence::Clean( sword::Intelligence& message ) const
{
    message.Clear();
}

// -----------------------------------------------------------------------------
// Name: Intelligence::Accept
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
void Intelligence::Accept( ParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: Intelligence::CommitToChildren
// Created: SBO 2007-10-29
// -----------------------------------------------------------------------------
void Intelligence::CommitToChildren()
{
    const Intelligence_ABC& entity = *GetValue();
    const Formation_ABC* formation = static_cast< const Formation_ABC* >( entity.Get< IntelligenceHierarchies >().GetSuperior() );
    kernel::Point position;
    position.AddPoint( entity.Get< kernel::Positions >().GetPosition() );

    AddParameter( *new String   ( OrderParameter( tools::translate( "Parameter", "Name" ).ascii()     , "name"     , false ), entity.GetName().ascii() ) );
    AddParameter( *new String   ( OrderParameter( tools::translate( "Parameter", "Nature" ).ascii()   , "nature"   , false ), entity.GetSymbol() ) );
    AddParameter( *new Karma    ( OrderParameter( tools::translate( "Parameter", "Karma" ).ascii()    , "karma"    , false ), entity.GetKarma(), *formation ) );
    AddParameter( *new Level    ( OrderParameter( tools::translate( "Parameter", "Level" ).ascii()    , "level"    , false ), entity.GetLevel() ) );
    AddParameter( *new Bool     ( OrderParameter( tools::translate( "Parameter", "Mounted" ).ascii()  , "bool"     , false ), entity.IsMounted() ) );
    AddParameter( *new Formation( OrderParameter( tools::translate( "Parameter", "Formation" ).ascii(), "formation", false ), *formation, controller_ ) );
    AddParameter( *new Point    ( OrderParameter( tools::translate( "Parameter", "Intelligence location" ).ascii(), "point"    , false ), converter_, position ) );
}

// -----------------------------------------------------------------------------
// Name: Intelligence::Serialize
// Created: SBO 2007-10-31
// -----------------------------------------------------------------------------
void Intelligence::Serialize( xml::xostream& xos ) const
{
    Parameter< const Intelligence_ABC* >::Serialize( xos );
}
