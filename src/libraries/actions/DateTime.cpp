// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "DateTime.h"
#include "clients_kernel/GlTools_ABC.h"
#include "protocol/Protocol.h"
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
DateTime::DateTime( const OrderParameter& parameter, const Common::MsgDateTime& date )
    : Parameter< QString >( parameter )
    , time_( date.data() )
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
    std::string dateStr  = date.date().toString( Qt::ISODate  ).ascii();
    std::string timeStr  = date.time().toString( Qt::LocalDate  ).ascii();
    bpt::ptime time( bpt::time_from_string( dateStr + " " + timeStr )  );

    //bpt::ptime time( bpt::time_from_string( date.toString( Qt::ISODate ).ascii() ) );
    time_ = bpt::to_iso_string( time );
    SetValue( bpt::to_simple_string( time ).c_str() );
    //SetValue( time_.c_str() );
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
void DateTime::CommitTo( Common::MsgDateTime& message ) const
{
    message.set_data( time_ );
}

// -----------------------------------------------------------------------------
// Name: DateTime::CommitTo
// Created: SBO 2009-06-03
// -----------------------------------------------------------------------------
void DateTime::CommitTo( Common::MsgMissionParameter& message ) const
{
    message.set_null_value ( !IsSet() );
    message.mutable_value()->mutable_datetime();    // enforce initialisation of parameter to force his type
    if( IsSet() )
        CommitTo( *message.mutable_value()->mutable_datetime() );
}

// -----------------------------------------------------------------------------
// Name: DateTime::Clean
// Created: SBO 2009-06-03
// -----------------------------------------------------------------------------
void DateTime::Clean( Common::MsgMissionParameter& message ) const
{
    message.mutable_value()->clear_datetime();
}
