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
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Tools.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute constructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
ObstacleAttribute::ObstacleAttribute( kernel::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity )
    : dictionary_    ( dictionary )
    , type_          ( eDemolitionTargetType_Preliminary )
    , bActivated_    ( true )
    , activationTime_( 0, 0 )
    , activityTime_  ( 0, 0 )
{
    CreateDictionary( entity );
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute constructor
// Created: JCR 2008-06-11
// -----------------------------------------------------------------------------
ObstacleAttribute::ObstacleAttribute( kernel::PropertiesDictionary& dictionary, Enum_DemolitionTargetType type, const kernel::Entity_ABC& entity )
    : dictionary_    ( dictionary )
    , type_          ( type )
    , bActivated_    ( type_.GetValue() == eDemolitionTargetType_Preliminary )
    , activationTime_( 0, 0 )
    , activityTime_  ( 0, 0 )
{
    CreateDictionary( entity );
}

namespace
{
    int GetActivationTime( xml::xistream& xis )
    {
        int result = 0;
        xis >> xml::optional
            >> xml::start( "activation-time" )
                >> xml::attribute( "value", result )
            >> xml::end;
        return result;
    }
    int GetActivityTime( xml::xistream& xis )
    {
        int result = 0;
        xis >> xml::optional
            >> xml::start( "activity-time" )
                >> xml::attribute( "value", result )
            >> xml::end;
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute constructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
ObstacleAttribute::ObstacleAttribute( xml::xistream& xis, kernel::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity )
    : dictionary_    ( dictionary )
    , type_          ( xis.attribute< std::string >( "type", std::string() ) )
    , bActivated_    ( xis.attribute< bool >( "activated" ) )
    , activationTime_( 0, 0 )
    , activityTime_  ( 0, 0 )
{
    QTime activationTime;
    activationTime_ = activationTime.addSecs( GetActivationTime( xis ) );
    QTime activityTime;
    activityTime_ = activityTime.addSecs( GetActivityTime( xis ) );
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
    displayer.Group( tools::translate( "Object", "Mine parameters" ) )
        .Display( tools::translate( "Object", "Obstacle type:" ), type_.GetValue() )
        .Display( tools::translate( "Object", "Obstacle activated:" ), bActivated_ );
    if( type_.GetValue() == eDemolitionTargetType_Preliminary )
    {
        displayer.Display( tools::translate( "Object", "Activation time:" ), activationTime_ );
        displayer.Display( tools::translate( "Object", "Activity time:" ), activityTime_ );
    }
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::DisplayInTooltip
// Created: JCR 2008-06-10
// -----------------------------------------------------------------------------
void ObstacleAttribute::DisplayInTooltip( Displayer_ABC& displayer ) const
{
    displayer.Display( tools::translate( "Object", "Obstacle type:" ), type_.GetValue() )
        .Display( tools::translate( "Object", "Obstacle activated:" ), bActivated_ );

    if( type_.GetValue() == eDemolitionTargetType_Preliminary )
    {
        displayer.Display( tools::translate( "Object", "Activation time:" ), activationTime_ );
        displayer.Display( tools::translate( "Object", "Activity time:" ), activityTime_ );
    }
}

namespace
{
    unsigned int Convert( const QTime& time )
    {
        return  time.hour() * 3600 + time.minute() * 60 + time.second();;
    }
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::SerializeObjectAttributes
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void ObstacleAttribute::SerializeObjectAttributes( xml::xostream& xos ) const
{
    xos << xml::start( "obstacle" )
            << xml::attribute( "type", type_.ToXml() )
            << xml::attribute( "activated", bActivated_ );
    if( type_.GetValue() == eDemolitionTargetType_Preliminary )
    {
        unsigned int time = Convert( activationTime_ );
        if( time > 0 )
            xos << xml::start( "activation-time" )
                    << xml::attribute( "value", time )
                << xml::end;
        time = Convert( activityTime_);
        if( time > 0 )
            xos << xml::start( "activity-time" )
                    << xml::attribute( "value", Convert( activityTime_ ) )
                << xml::end;
    }
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::CreateDictionary
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void ObstacleAttribute::CreateDictionary( const kernel::Entity_ABC& entity )
{
    dictionary_.RegisterExtension( entity, this, tools::translate( "Object", "Info/Demolition target parameters/Obstacle type" ), type_ );
    dictionary_.RegisterExtension( entity, this, tools::translate( "Object", "Info/Demolition target parameters/Obstacle activated" ), bActivated_ );
    if( type_.GetValue() == eDemolitionTargetType_Preliminary )
    {
        dictionary_.RegisterExtension( entity, this, tools::translate( "Object", "Info/Demolition target parameters/Activation time" ), activationTime_ );
        dictionary_.RegisterExtension( entity, this, tools::translate( "Object", "Info/Demolition target parameters/Activity time" ), activityTime_ );
    }
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
    QTime activationTime;
    activationTime_ = activationTime.addSecs( time );
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::SetActivityTime
// Created: LGY 2011-08-31
// -----------------------------------------------------------------------------
void ObstacleAttribute::SetActivityTime( int time )
{
    QTime activityTime;
    activityTime_ = activityTime.addSecs( time );
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::IsReservedObstacle
// Created: LDC 2010-12-27
// -----------------------------------------------------------------------------
bool ObstacleAttribute::IsReservedObstacle() const
{
    return type_.GetValue() == eDemolitionTargetType_Preliminary;
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::IsObstacleActivated
// Created: MMC 2012-07-25
// -----------------------------------------------------------------------------
bool ObstacleAttribute::IsObstacleActivated() const
{
    return bActivated_;
}
