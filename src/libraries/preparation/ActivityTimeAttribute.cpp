// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "ActivityTimeAttribute.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "Tools.h"

#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ActivityTimeAttribute constructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
ActivityTimeAttribute::ActivityTimeAttribute( kernel::PropertiesDictionary& dico )
    :  activityTime_( 0, Units::hours )
{
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: ActivityTimeAttribute constructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
ActivityTimeAttribute::ActivityTimeAttribute( xml::xistream& xis, kernel::PropertiesDictionary& dico )
    : activityTime_( 0, Units::hours )
{
    xis >> xml::attribute( "value", activityTime_.value_ );
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: ActivityTimeAttribute destructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
ActivityTimeAttribute::~ActivityTimeAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActivityTimeAttribute::Display
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void ActivityTimeAttribute::Display( kernel::Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Mine parameters" ) )
             .Display( tools::translate( "Object", "Activity time:" ), activityTime_ );
}

// -----------------------------------------------------------------------------
// Name: ActivityTimeAttribute::SerializeAttributes
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void ActivityTimeAttribute::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::start( "activity-time" )
            << xml::attribute( "value", activityTime_.value_ )
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::SetActivityTime
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void ActivityTimeAttribute::SetActivityTime( unsigned int time )
{
    activityTime_.value_ = time;
}

// -----------------------------------------------------------------------------
// Name: ActivityTimeAttribute::CreateDictionary
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void ActivityTimeAttribute::CreateDictionary( kernel::PropertiesDictionary& dico )
{
    dico.Register( *this, tools::translate( "ActivityTimeAttribute", "Info/Mine parameters/Activity time" ), activityTime_ );
}