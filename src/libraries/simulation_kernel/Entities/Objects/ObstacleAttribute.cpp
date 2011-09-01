// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "ObstacleAttribute.h"
#include "Object.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "CheckPoints/MIL_CheckPointInArchive.h"
#include "CheckPoints/MIL_CheckPointOutArchive.h"
#include "protocol/Protocol.h"
#include <xeumeuleu/xml.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( ObstacleAttribute )

BOOST_CLASS_EXPORT_KEY( DEC_Knowledge_ObjectAttributeProxyPassThrough< ObstacleAttribute > )
BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Knowledge_ObjectAttributeProxyPassThrough< ObstacleAttribute > )

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute constructor
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
ObstacleAttribute::ObstacleAttribute()
    : obstacle_      ( sword::ObstacleType_DemolitionTargetType_preliminary )
    , bActivated_    ( true )
    , activationTime_( 0 )
    , activityTime_  ( 0 )
    , endActivity_   ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute constructor
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
ObstacleAttribute::ObstacleAttribute( bool reserved )
    : obstacle_      ( reserved ? sword::ObstacleType_DemolitionTargetType_reserved : sword::ObstacleType_DemolitionTargetType_preliminary )
    , bActivated_    ( !reserved )
    , activationTime_( 0 )
    , activityTime_  ( 0 )
    , endActivity_   ( 0 )
{
    // NOTHING
}

namespace
{
    sword::ObstacleType_DemolitionTargetType ExtractObstacle( const std::string& obstacle )
    {
        if( obstacle == "reserved" )
            return sword::ObstacleType_DemolitionTargetType_reserved;
        return sword::ObstacleType_DemolitionTargetType_preliminary;
    }

    std::string ExtractObstacle( sword::ObstacleType_DemolitionTargetType obstacle )
    {
        switch ( obstacle )
        {
        case sword::ObstacleType_DemolitionTargetType_reserved:
            return "reserved";
        default:
            return "preliminary";
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute constructor
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
ObstacleAttribute::ObstacleAttribute( xml::xistream& xis )
    : obstacle_      ( ExtractObstacle( xis.attribute< std::string >( "type", std::string() ) ) )
    , bActivated_    ( xis.attribute< bool >( "activated", false ) )
    , activationTime_( 0 )
    , activityTime_  ( 0 )
{
    xis >> xml::optional
        >> xml::start( "activation-time" )
            >> xml::attribute( "value", activationTime_ )
        >> xml::end
        >> xml::optional
        >> xml::start( "activity-time" )
            >> xml::attribute( "value", activityTime_ )
        >> xml::end;
    if( activationTime_ != 0 && activityTime_ == 0 )  // $$$$ _RC_ LGY 2011-08-31: default 6h
        activityTime_ = 21600;
    endActivity_ = activationTime_ + activationTime_;
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::ObstacleAttribute
// Created: JCR 2008-07-21
// -----------------------------------------------------------------------------
ObstacleAttribute::ObstacleAttribute( const sword::MissionParameter_Value& attributes )
    : obstacle_      ( ( sword::ObstacleType_DemolitionTargetType ) attributes.list( 1 ).identifier() )
    , bActivated_    ( attributes.list( 2 ).booleanvalue() )
    , activationTime_( attributes.list( 3 ).quantity() )
    , activityTime_  ( attributes.list( 4 ).quantity() )
    , endActivity_   ( activationTime_ + activityTime_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute destructor
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
ObstacleAttribute::~ObstacleAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::serialize
// Created: JCR 2008-06-19
// -----------------------------------------------------------------------------
template < typename Archive > void ObstacleAttribute::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    file & obstacle_
         & bActivated_
         & activationTime_
         & activityTime_
         & endActivity_;
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::SetType
// Created: LDC 2009-03-23
// -----------------------------------------------------------------------------
void ObstacleAttribute::SetType( sword::ObstacleType_DemolitionTargetType obstacleType )
{
    obstacle_ = obstacleType;
    if( sword::ObstacleType_DemolitionTargetType_preliminary == obstacleType )
        bActivated_ = true;
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::GetActivationTime
// Created: JSR 2010-07-08
// -----------------------------------------------------------------------------
int ObstacleAttribute::GetActivationTime() const
{
    return activationTime_;
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::GetEndActivity
// Created: LGY 2011-08-31
// -----------------------------------------------------------------------------
int ObstacleAttribute::GetEndActivity() const
{
    return endActivity_;
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::IsActivable
// Created: JCR 2008-06-12
// -----------------------------------------------------------------------------
bool ObstacleAttribute::IsActivable() const
{
    return obstacle_ == sword::ObstacleType_DemolitionTargetType_reserved;
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::IsDeactivable
// Created: LGY 2011-09-01
// -----------------------------------------------------------------------------
bool ObstacleAttribute::IsDeactivable() const
{
    return bActivated_ && activityTime_ != 0 && IsActivable();
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::IsActivated
// Created: JCR 2008-06-12
// -----------------------------------------------------------------------------
bool ObstacleAttribute::IsActivated() const
{
    return bActivated_;
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::Activate
// Created: JCR 2008-06-12
// -----------------------------------------------------------------------------
void ObstacleAttribute::Activate()
{
    if( ! bActivated_ && IsActivable() )
    {
        bActivated_ = true;
        activityTime_ += activationTime_;
        activationTime_ = 0;
        NotifyAttributeUpdated( eOnUpdate );
    }
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::Deactivate
// Created: LGY 2011-08-31
// -----------------------------------------------------------------------------
void ObstacleAttribute::Deactivate()
{
    bActivated_ = false;
    activationTime_ = 0;
    activityTime_ = 0;
    endActivity_ = 0;
    NotifyAttributeUpdated( eOnUpdate );
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::Instanciate
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void ObstacleAttribute::Instanciate( DEC_Knowledge_Object& object ) const
{
    object.Attach< DEC_Knowledge_ObjectAttributeProxy_ABC< ObstacleAttribute > >( *new DEC_Knowledge_ObjectAttributeProxyPassThrough< ObstacleAttribute >() );
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::Register
// Created: JSR 2010-03-15
// -----------------------------------------------------------------------------
void ObstacleAttribute::Register( MIL_Object_ABC& object ) const
{
    object.SetAttribute< ObstacleAttribute, ObstacleAttribute >( *this );
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::Send
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void ObstacleAttribute::SendFullState( sword::ObjectAttributes& asn ) const
{
    asn.mutable_obstacle()->set_type( obstacle_ );
    asn.mutable_obstacle()->set_activated( bActivated_ );
    asn.mutable_obstacle()->set_activation_time( activationTime_ );
    asn.mutable_obstacle()->set_activity_time( activityTime_ );
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::Send
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void ObstacleAttribute::SendUpdate( sword::ObjectAttributes& asn ) const
{
    if( NeedUpdate( eOnUpdate ) )
    {
        asn.mutable_obstacle()->set_activated( bActivated_ );
        asn.mutable_obstacle()->set_type( obstacle_ );
        asn.mutable_obstacle()->set_activation_time( activationTime_ );
        asn.mutable_obstacle()->set_activity_time( activityTime_ );
        Reset( eOnUpdate );
    }
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::WriteODB
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void ObstacleAttribute::WriteODB( xml::xostream& xos ) const
{
    xos << xml::start( "obstacle" )
            << xml::attribute( "type", ExtractObstacle( obstacle_ ) )
            << xml::attribute( "activated", bActivated_ );
    if( activationTime_ != 0 )
    {
        xos << xml::start( "activation-time" )
                << xml::attribute( "value", activationTime_ )
            << xml::end;
    }
    if( activityTime_ != 0 )
    {
        xos << xml::start( "activity-time" )
                << xml::attribute( "value", activityTime_ )
            << xml::end;
    }
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::operator=
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
ObstacleAttribute& ObstacleAttribute::operator=( const ObstacleAttribute& rhs )
{
    obstacle_ = rhs.obstacle_;
    bActivated_ = rhs.bActivated_;
    activationTime_ = rhs.activationTime_;
    activityTime_ = rhs.activityTime_;
    endActivity_ = rhs.endActivity_;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::OnUpdate
// Created: LDC 2009-03-16
// -----------------------------------------------------------------------------
void ObstacleAttribute::OnUpdate( const sword::MissionParameter_Value& attribute )
{
    if( attribute.list_size() > 2 && bActivated_ != attribute.list( 2 ).booleanvalue() )
    {
        bActivated_ = attribute.list( 2 ).booleanvalue();
        NotifyAttributeUpdated( eOnUpdate );
    }
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::Update
// Created: NLD 2010-10-26
// -----------------------------------------------------------------------------
bool ObstacleAttribute::Update( const ObstacleAttribute& rhs )
{
    if( obstacle_ != rhs.obstacle_ )
    {
        NotifyAttributeUpdated( eOnUpdate );
        obstacle_ = rhs.obstacle_;
    }
    if( bActivated_ != rhs.bActivated_ )
    {
        NotifyAttributeUpdated( eOnUpdate );
        bActivated_ = rhs.bActivated_;
    }
    if( activationTime_ != rhs.activationTime_ )
    {
        NotifyAttributeUpdated( eOnUpdate );
        activationTime_ = rhs.activationTime_;
    }
    if( activityTime_ != rhs.activityTime_ )
    {
        NotifyAttributeUpdated( eOnUpdate );
        activityTime_ = rhs.activityTime_;
    }
    return NeedUpdate( eOnUpdate );
}
