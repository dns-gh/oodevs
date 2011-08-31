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
#include "Tools.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute constructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
ObstacleAttribute::ObstacleAttribute( kernel::PropertiesDictionary& dictionary )
    : dictionary_    ( dictionary )
    , type_          ( eDemolitionTargetType_Preliminary )
    , bActivated_    ( true )
    , activationTime_( 0 )
    , activityTime_  ( 0 )
{
    CreateDictionary();
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute constructor
// Created: JCR 2008-06-11
// -----------------------------------------------------------------------------
ObstacleAttribute::ObstacleAttribute( kernel::PropertiesDictionary& dictionary, Enum_DemolitionTargetType type )
    : dictionary_    ( dictionary )
    , type_          ( type )
    , bActivated_    ( type_.GetValue() == eDemolitionTargetType_Preliminary )
    , activationTime_( 0 )
    , activityTime_  ( 0 )
{
    CreateDictionary();
}

namespace
{
    int GetActivationTime( xml::xistream& xis )
    {
        int result = 0;
        xis >> xml::optional()
            >> xml::start( "activation-time" )
                >> xml::attribute( "value", result )
            >> xml::end;
        return result;
    }
    int GetActivityTime( xml::xistream& xis )
    {
        int result = 0;
        xis >> xml::optional()
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
ObstacleAttribute::ObstacleAttribute( xml::xistream& xis, kernel::PropertiesDictionary& dictionary )
    : dictionary_    ( dictionary )
    , type_          ( xis.attribute< std::string >( "type", std::string() ) )
    , bActivated_    ( xis.attribute< bool >( "activated" ) )
    , activationTime_( GetActivationTime( xis ) )
    , activityTime_  ( GetActivityTime( xis ) )
{
    CreateDictionary();
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
             .Display( tools::translate( "Object", "Reserved obstacle activated:" ), bActivated_ );
    if( !bActivated_ )
    {
        displayer.Display( tools::translate( "Object", "Activation time:" ), activationTime_ / 3600. * Units::hours );
        displayer.Display( tools::translate( "Object", "Activity time:" ), activityTime_ / 3600. * Units::hours );
    }
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::DisplayInTooltip
// Created: JCR 2008-06-10
// -----------------------------------------------------------------------------
void ObstacleAttribute::DisplayInTooltip( Displayer_ABC& displayer ) const
{
    displayer.Display( tools::translate( "Object", "Obstacle type:" ), type_.GetValue() );
    if( type_.GetValue() == eDemolitionTargetType_Reserved )
        displayer.Display( tools::translate( "Object", "Reserved obstacle activated:" ), bActivated_ );
    if( !bActivated_ )
    {
        displayer.Display( tools::translate( "Object", "Activation time:" ), activationTime_ / 3600. * Units::hours );
        displayer.Display( tools::translate( "Object", "Activity time:" ), activityTime_ / 3600. * Units::hours );
    }
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::SerializeAttributes
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void ObstacleAttribute::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::start( "obstacle" )
            << xml::attribute( "type", type_.ToXml() )
            << xml::attribute( "activated", bActivated_ );
    if( !bActivated_ )
    {
        xos << xml::start( "activation-time" )
                << xml::attribute( "value", activationTime_ )
            << xml::end
            << xml::start( "activity-time" )
                << xml::attribute( "value", activityTime_ )
            << xml::end;
    }
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::CreateDictionary
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void ObstacleAttribute::CreateDictionary()
{
    dictionary_.Register( *this, tools::translate( "Object", "Info/Demolition target parameters/Obstacle type" ), type_ );
    dictionary_.Register( *this, tools::translate( "Object", "Info/Demolition target parameters/Obstacle activated" ), bActivated_ );
    if( !bActivated_ )
    {
        dictionary_.Register( *this, tools::translate( "Object", "Info/Demolition target parameters/Activation time" ), activationTime_ );
        dictionary_.Register( *this, tools::translate( "Object", "Info/Demolition target parameters/Activity time" ), activityTime_ );
    }
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::Activate
// Created: JCR 2008-06-11
// -----------------------------------------------------------------------------
void ObstacleAttribute::Activate( bool activate )
{
    if( type_.GetValue() == eDemolitionTargetType_Reserved )
    {
        bActivated_ = activate;
        if( bActivated_ )
        {
            dictionary_.Remove( tools::translate( "Object", "Info/Demolition target parameters/Activation time" ) );
            dictionary_.Remove( tools::translate( "Object", "Info/Demolition target parameters/Activity time" ) );
        }
    }
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
// Name: ObstacleAttribute::IsReservedObstacle
// Created: LDC 2010-12-27
// -----------------------------------------------------------------------------
bool ObstacleAttribute::IsReservedObstacle() const
{
    return type_.GetValue() == eDemolitionTargetType_Preliminary;
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::IsReservedObstacleActivated
// Created: LDC 2010-12-27
// -----------------------------------------------------------------------------
bool ObstacleAttribute::IsReservedObstacleActivated() const
{
    return bActivated_;
}
