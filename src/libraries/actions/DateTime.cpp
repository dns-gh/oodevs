// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "DateTime.h"
#include "clients_kernel/GlTools_ABC.h"
#include <xeumeuleu/xml.h>
#pragma warning( push )
#pragma warning( disable: 4127 4512 )
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning( pop )

namespace bpt = boost::posix_time;
using namespace xml;
using namespace kernel;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: DateTime constructor
// Created: SBO 2007-06-25
// -----------------------------------------------------------------------------
DateTime::DateTime( const kernel::OrderParameter& parameter, xml::xistream& xis )
    : Parameter< QString >( parameter )
{
    xis >> attribute( "value", time_ );
    bpt::ptime time( bpt::from_iso_string( time_ ) );
    SetValue( bpt::to_simple_string( time ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: DateTime constructor
// Created: SBO 2007-05-15
// -----------------------------------------------------------------------------
DateTime::DateTime( const OrderParameter& parameter, const ASN1T_DateTime& date )
    : Parameter< QString >( parameter )
    , time_( (const char*)date.data, 15 )
{
    bpt::ptime time( bpt::from_iso_string( time_ ) );
    SetValue( bpt::to_simple_string( time ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: DateTime constructor
// Created: AGE 2007-10-12
// -----------------------------------------------------------------------------
DateTime::DateTime( const kernel::OrderParameter& parameter, const QDateTime& date )
    : Parameter< QString >( parameter )
{
    bpt::ptime time( bpt::from_time_t( date.toTime_t() ) );
    time_ = bpt::to_iso_string( time );
    SetValue( bpt::to_simple_string( time ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: DateTime destructor
// Created: SBO 2007-05-15
// -----------------------------------------------------------------------------
DateTime::~DateTime()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DateTime::Draw
// Created: SBO 2007-06-25
// -----------------------------------------------------------------------------
void DateTime::Draw( const geometry::Point2f&, const kernel::Viewport_ABC&, const kernel::GlTools_ABC& ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DateTime::DisplayInToolTip
// Created: SBO 2007-05-15
// -----------------------------------------------------------------------------
void DateTime::DisplayInToolTip( kernel::Displayer_ABC& displayer ) const
{
    displayer.Display( "", GetValue() );
    Parameter< QString >::DisplayInToolTip( displayer );
}

// -----------------------------------------------------------------------------
// Name: DateTime::Serialize
// Created: SBO 2007-06-25
// -----------------------------------------------------------------------------
void DateTime::Serialize( xml::xostream& xos ) const
{
    Parameter< QString >::Serialize( xos );
    xos << attribute( "value", time_ );
}

// -----------------------------------------------------------------------------
// Name: DateTime::CommitTo
// Created: SBO 2007-06-25
// -----------------------------------------------------------------------------
void DateTime::CommitTo( ASN1T_DateTime& asn ) const
{
    asn = time_.c_str();
}

// -----------------------------------------------------------------------------
// Name: DateTime::CommitTo
// Created: SBO 2009-06-03
// -----------------------------------------------------------------------------
void DateTime::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = !IsSet();
    asn.value.t = T_MissionParameter_value_dateTime;
    asn.value.u.dateTime = new ASN1T_DateTime();
    if( IsSet() )
        CommitTo( *asn.value.u.dateTime );
}

// -----------------------------------------------------------------------------
// Name: DateTime::Clean
// Created: SBO 2009-06-03
// -----------------------------------------------------------------------------
void DateTime::Clean( ASN1T_MissionParameter& asn ) const
{
    delete asn.value.u.dateTime;
}
