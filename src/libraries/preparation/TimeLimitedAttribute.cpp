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
#include "Tools.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: TimeLimitedAttribute constructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
TimeLimitedAttribute::TimeLimitedAttribute( PropertiesDictionary& dico )
    : activityTime_( 0, Units::hours )
{
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: TimeLimitedAttribute constructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
TimeLimitedAttribute::TimeLimitedAttribute( xml::xistream& xis, PropertiesDictionary& dico )
    : activityTime_( 0, Units::hours )
{
    xis >> xml::attribute( "value", activityTime_.value_ );
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
    xos << xml::start( "activity-time" )
        << xml::attribute( "value", activityTime_.value_ )
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::SetActivityTime
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void TimeLimitedAttribute::SetActivityTime( unsigned int time )
{
    activityTime_.value_ = time;
}

// -----------------------------------------------------------------------------
// Name: TimeLimitedAttribute::CreateDictionary
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void TimeLimitedAttribute::CreateDictionary( PropertiesDictionary& dico )
{
    dico.Register( *this, tools::translate( "TimeLimitedAttribute", "Info/Mine parameters/Activity time" ), activityTime_ );
}