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
ActionParameterDateTime::ActionParameterDateTime( const kernel::OrderParameter& parameter, xml::xistream& xis )
    : ActionParameter< QString >( parameter )
{
    xis >> attribute( "value", time_ );
    bpt::ptime time( bpt::from_iso_string( time_ ) );
    SetValue( bpt::to_simple_string( time ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterDateTime constructor
// Created: SBO 2007-05-15
// -----------------------------------------------------------------------------
ActionParameterDateTime::ActionParameterDateTime( const OrderParameter& parameter, const ASN1T_DateTime& date )
    : ActionParameter< QString >( parameter )
    , time_( (const char*)date.data, 15 )
{
    bpt::ptime time( bpt::from_iso_string( time_ ) );
    SetValue( bpt::to_simple_string( time ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterDateTime constructor
// Created: AGE 2007-10-12
// -----------------------------------------------------------------------------
ActionParameterDateTime::ActionParameterDateTime( const kernel::OrderParameter& parameter, const QDateTime& date )
    : ActionParameter< QString >( parameter )
{
    bpt::ptime time( bpt::from_time_t( date.toTime_t() ) );
    time_ = bpt::to_iso_string( time );
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
    xos << attribute( "value", time_ );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterDateTime::CommitTo
// Created: SBO 2007-06-25
// -----------------------------------------------------------------------------
void ActionParameterDateTime::CommitTo( ASN1T_DateTime& asn ) const
{
    asn = time_.c_str();
}
