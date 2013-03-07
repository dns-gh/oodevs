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
TimeLimitedAttribute::TimeLimitedAttribute( PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity )
    : activityTime_( 0, 0 )
{
    CreateDictionary( dictionary, entity );
}

// -----------------------------------------------------------------------------
// Name: TimeLimitedAttribute constructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
TimeLimitedAttribute::TimeLimitedAttribute( xml::xistream& xis, PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity )
    : activityTime_( 0, 0 )
{
    QTime activityTime;
    activityTime_ = activityTime.addSecs( xis.attribute< unsigned int >( "value" ) );
    CreateDictionary( dictionary, entity );
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
           .Display( tools::translate( "Object", "Life time:" ), activityTime_ );
}

// -----------------------------------------------------------------------------
// Name: TimeLimitedAttribute::SerializeObjectAttributes
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void TimeLimitedAttribute::SerializeObjectAttributes( xml::xostream& xos ) const
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
void TimeLimitedAttribute::CreateDictionary( PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity )
{
    dictionary.RegisterExtension( entity, this, tools::translate( "TimeLimitedAttribute", "Info/Mine parameters/Activity time" ), activityTime_ );
}