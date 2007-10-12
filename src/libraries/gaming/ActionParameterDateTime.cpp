// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterDateTime.h"
#include "Simulation.h"
#include "clients_kernel/GlTools_ABC.h"
#include "xeumeuleu/xml.h"
#pragma warning( push, 1 )
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning( pop )

namespace bpt = boost::posix_time;
using namespace xml;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ActionParameterDateTime constructor
// Created: SBO 2007-06-25
// -----------------------------------------------------------------------------
ActionParameterDateTime::ActionParameterDateTime( const kernel::OrderParameter& parameter, xml::xistream& xis, const Simulation& simulation )
    : ActionParameter< QString >( parameter )
    , simulation_( simulation )
{
    std::string value;
    xis >> attribute( "value", value );
    bpt::ptime time( bpt::from_iso_string( value ) );
    time_ = ( time - bpt::from_time_t( 0 ) ).total_seconds();
    SetValue( bpt::to_simple_string( time ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterDateTime constructor
// Created: SBO 2007-05-15
// -----------------------------------------------------------------------------
ActionParameterDateTime::ActionParameterDateTime( const OrderParameter& parameter, const Simulation& simulation, unsigned int ticks )
    : ActionParameter< QString >( parameter )
    , simulation_( simulation )
    , time_( ticks )
{
    bpt::ptime time( bpt::from_time_t( time_ ) );
    SetValue( bpt::to_simple_string( time ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterDateTime destructor
// Created: SBO 2007-05-15
// -----------------------------------------------------------------------------
ActionParameterDateTime::~ActionParameterDateTime()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterDateTime::Draw
// Created: SBO 2007-06-25
// -----------------------------------------------------------------------------
void ActionParameterDateTime::Draw( const geometry::Point2f&, const kernel::Viewport_ABC&, const kernel::GlTools_ABC& ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterDateTime::DisplayInToolTip
// Created: SBO 2007-05-15
// -----------------------------------------------------------------------------
void ActionParameterDateTime::DisplayInToolTip( kernel::Displayer_ABC& displayer ) const
{
    displayer.Display( "", GetValue() );
    ActionParameter< QString >::DisplayInToolTip( displayer );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterDateTime::Serialize
// Created: SBO 2007-06-25
// -----------------------------------------------------------------------------
void ActionParameterDateTime::Serialize( xml::xostream& xos ) const
{
    ActionParameter< QString >::Serialize( xos );
    xos << attribute( "value", GetValue() );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterDateTime::CommitTo
// Created: SBO 2007-06-25
// -----------------------------------------------------------------------------
void ActionParameterDateTime::CommitTo( ASN1INT& asn ) const
{
    asn = time_;
}
