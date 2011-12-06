// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "TimeLimitedAttribute.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Tools.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: TimeLimitedAttribute constructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
TimeLimitedAttribute::TimeLimitedAttribute( PropertiesDictionary& dico )
    : activityTime_( 0, 0 )
{
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: TimeLimitedAttribute constructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
TimeLimitedAttribute::TimeLimitedAttribute( xml::xistream& xis, PropertiesDictionary& dico )
    : activityTime_( 0, 0 )
{
    QTime activityTime;
    activityTime_ = activityTime.addSecs( xis.attribute< unsigned int >( "value" ) );
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: TimeLimitedAttribute destructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
TimeLimitedAttribute::~TimeLimitedAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TimeLimitedAttribute::Display
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void TimeLimitedAttribute::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Mine parameters" ) )
           .Display( tools::translate( "Object", "Activity time:" ), activityTime_ );
}

// -----------------------------------------------------------------------------
// Name: TimeLimitedAttribute::SerializeAttributes
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void TimeLimitedAttribute::SerializeAttributes( xml::xostream& xos ) const
{
    unsigned int time = activityTime_.hour() * 3600 + activityTime_.minute() * 60 + activityTime_.second();
    xos << xml::start( "activity-time" )
        << xml::attribute( "value", time )
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::SetActivityTime
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void TimeLimitedAttribute::SetActivityTime( unsigned int time )
{
    QTime activityTime;
    activityTime_ = activityTime.addSecs( time );
}

// -----------------------------------------------------------------------------
// Name: TimeLimitedAttribute::CreateDictionary
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void TimeLimitedAttribute::CreateDictionary( PropertiesDictionary& dico )
{
    dico.Register( *this, tools::translate( "TimeLimitedAttribute", "Info/Mine parameters/Activity time" ), activityTime_ );
}