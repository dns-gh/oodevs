// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "ObstacleAttribute.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_gui/PropertiesDictionary.h"
#include "clients_kernel/Tools.h"

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute constructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
ObstacleAttribute::ObstacleAttribute( gui::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity )
    : dictionary_    ( dictionary )
    , bActivated_    ( true )
    , activationTime_( 0 )
    , activityTime_  ( 0 )
{
    CreateDictionary( entity );
}

namespace
{
    int ReadOptionalTime( xml::xistream& xis, const std::string& tag )
    {
        int result = 0;
        xis >> xml::optional
            >> xml::start( tag )
                >> xml::attribute( "value", result )
            >> xml::end;
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute constructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
ObstacleAttribute::ObstacleAttribute( xml::xistream& xis, gui::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity )
    : dictionary_    ( dictionary )
    , bActivated_    ( xis.attribute< bool >( "activated" ) )
    , activationTime_( ReadOptionalTime( xis, "activation-time" ) )
    , activityTime_  ( ReadOptionalTime( xis, "activity-time" ) )
{
    CreateDictionary( entity );
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute destructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
ObstacleAttribute::~ObstacleAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::Display
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void ObstacleAttribute::Display( kernel::Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Obstacle parameters" ) )
        .Display( tools::translate( "Object", "Obstacle activated:" ), bActivated_ )
        .Display( tools::translate( "Object", "Activation time:" ), activationTime_ )
        .Display( tools::translate( "Object", "Activity time:" ), activityTime_ );
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::DisplayInTooltip
// Created: JCR 2008-06-10
// -----------------------------------------------------------------------------
void ObstacleAttribute::DisplayInTooltip( kernel::Displayer_ABC& displayer ) const
{
    displayer.Display( tools::translate( "Object", "Obstacle activated:" ), bActivated_ )
             .Display( tools::translate( "Object", "Activation time:" ), activationTime_ )
             .Display( tools::translate( "Object", "Activity time:" ), activityTime_ );
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::SerializeObjectAttributes
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void ObstacleAttribute::SerializeObjectAttributes( xml::xostream& xos ) const
{
    xos << xml::start( "obstacle" )
            << xml::attribute( "activated", bActivated_ );
    if( activationTime_ > 0 )
        xos << xml::start( "activation-time" )
                << xml::attribute( "value", activationTime_ )
            << xml::end;
    if( activityTime_ > 0 )
        xos << xml::start( "activity-time" )
                << xml::attribute( "value", activityTime_ )
            << xml::end;
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::CreateDictionary
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void ObstacleAttribute::CreateDictionary( const kernel::Entity_ABC& entity )
{
    dictionary_.RegisterExtension( entity, this, tools::translate( "Object", "Info/Demolition target parameters/Obstacle activated" ), bActivated_ );
    dictionary_.RegisterExtension( entity, this, tools::translate( "Object", "Info/Demolition target parameters/Activation time" ), activationTime_ );
    dictionary_.RegisterExtension( entity, this, tools::translate( "Object", "Info/Demolition target parameters/Activity time" ), activityTime_ );
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::Activate
// Created: JCR 2008-06-11
// -----------------------------------------------------------------------------
void ObstacleAttribute::Activate( bool activate )
{
    bActivated_ = activate;
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::SetActivationTime
// Created: JSR 2010-07-07
// -----------------------------------------------------------------------------
void ObstacleAttribute::SetActivationTime( int time )
{
    activationTime_ = time;
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::SetActivityTime
// Created: LGY 2011-08-31
// -----------------------------------------------------------------------------
void ObstacleAttribute::SetActivityTime( int time )
{
    activityTime_ = time;
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::IsObstacleActivated
// Created: MMC 2012-07-25
// -----------------------------------------------------------------------------
bool ObstacleAttribute::IsObstacleActivated() const
{
    return bActivated_;
}
