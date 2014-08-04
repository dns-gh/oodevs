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
#include "ObjectAttributes.h"
#include "MIL_Time_ABC.h"
#include "Object.h"
#include "CheckPoints/MIL_CheckPointInArchive.h"
#include "CheckPoints/MIL_CheckPointOutArchive.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/DEC_Knowledge_ObjectAttributeProxyPassThrough.h"
#include "protocol/Protocol.h"

BOOST_CLASS_EXPORT_IMPLEMENT( ObstacleAttribute )

BOOST_CLASS_EXPORT_KEY( DEC_Knowledge_ObjectAttributeProxyPassThrough< ObstacleAttribute > )
BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Knowledge_ObjectAttributeProxyPassThrough< ObstacleAttribute > )

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute constructor
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
ObstacleAttribute::ObstacleAttribute()
    : bActivated_( false )
    , activatedAfterConstruction_( false )
    , activationTime_( 0 )
    , activityTime_( 0 )
    , creationTime_( MIL_Time_ABC::GetTime().GetRealTime() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute constructor
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
ObstacleAttribute::ObstacleAttribute( xml::xistream& xis )
    : bActivated_    ( xis.attribute< bool >( "activated", false ) )
    , activationTime_( 0 )
    , activityTime_  ( 0 )
    , creationTime_  ( MIL_Time_ABC::GetTime().GetRealTime() )
{
    xis >> xml::optional
        >> xml::start( "activation-time" )
            >> xml::attribute( "value", activationTime_ )
        >> xml::end
        >> xml::optional
        >> xml::start( "activity-time" )
            >> xml::attribute( "value", activityTime_ )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::ObstacleAttribute
// Created: JCR 2008-07-21
// -----------------------------------------------------------------------------
ObstacleAttribute::ObstacleAttribute( const sword::MissionParameter_Value& attributes )
{
    CheckCount( "obstacle", attributes, 5 );
    bActivated_ = GetBool( "obstacle", attributes, 2 ); // first parameter is deprecated preliminary/reserved type. Kept for compatibility
    activationTime_ = GetQuantity( "obstacle", attributes, 3 );
    activityTime_ = GetQuantity( "obstacle", attributes, 4 );
    creationTime_ = MIL_Time_ABC::GetTime().GetRealTime();
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
    file & bActivated_
         & activatedAfterConstruction_
         & activationTime_
         & activityTime_
         & creationTime_;
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::SetActivityTime
// Created: JSR 2011-09-29
// -----------------------------------------------------------------------------
void ObstacleAttribute::SetActivityTime( unsigned int activityTime )
{
    activityTime_ = activityTime;
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::SetActivationTime
// Created: ABR 2012-03-23
// -----------------------------------------------------------------------------
void ObstacleAttribute::SetActivationTime( unsigned int activationTime )
{
    activationTime_ = activationTime;
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
// Name: ObstacleAttribute::GetActivityTime
// Created: MMC 2012-07-25
// -----------------------------------------------------------------------------
int ObstacleAttribute::GetActivityTime() const
{
    return activityTime_;
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::GetEndActivity
// Created: LGY 2011-08-31
// -----------------------------------------------------------------------------
int ObstacleAttribute::GetEndActivity() const
{
    return activationTime_ + activityTime_;
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::IsTimesUndefined
// Created: MMC 2012-07-25
// -----------------------------------------------------------------------------
bool ObstacleAttribute::IsTimesUndefined() const
{
    return activationTime_<= 0 && activityTime_ <= 0;
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::SetActivatedAfterConstruction
// Created: JSR 2014-08-04
// -----------------------------------------------------------------------------
void ObstacleAttribute::SetActivatedAfterConstruction( bool activatedAfterConstruction )
{
    activatedAfterConstruction_ = activatedAfterConstruction;
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::ActivatedAfterConstruction
// Created: JSR 2014-08-04
// -----------------------------------------------------------------------------
bool ObstacleAttribute::ActivatedAfterConstruction() const
{
    return activatedAfterConstruction_;
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
void ObstacleAttribute::Activate( bool activate )
{
    if( bActivated_ != activate )
    {
        bActivated_ = activate;
        NotifyAttributeUpdated( eOnUpdate );
    }
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
    asn.mutable_obstacle()->set_type( bActivated_ ? sword::ObstacleType_DemolitionTargetType_preliminary : sword::ObstacleType_DemolitionTargetType_reserved );
    asn.mutable_obstacle()->set_activated( bActivated_ );
    asn.mutable_obstacle()->set_activation_time( activationTime_ );
    asn.mutable_obstacle()->set_activity_time( activityTime_ );
    asn.mutable_obstacle()->set_creation_time( creationTime_ );
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::Send
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
bool ObstacleAttribute::SendUpdate( sword::ObjectAttributes& asn ) const
{
    if( NeedUpdate( eOnUpdate ) )
    {
        SendFullState( asn );
        Reset( eOnUpdate );
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::WriteODB
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void ObstacleAttribute::WriteODB( xml::xostream& xos ) const
{
    xos << xml::start( "obstacle" )
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
    bActivated_ = rhs.bActivated_;
    activationTime_ = rhs.activationTime_;
    activityTime_ = rhs.activityTime_;
    creationTime_ = rhs.creationTime_;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::OnUpdate
// Created: LDC 2009-03-16
// -----------------------------------------------------------------------------
void ObstacleAttribute::OnUpdate( const sword::MissionParameter_Value& attribute )
{
    if( attribute.list_size() > 1 && bActivated_ != attribute.list( 1 ).booleanvalue() )
    {
        bActivated_ = attribute.list( 1 ).booleanvalue();
        if( bActivated_ )
            activationTime_ = 0;
        else
            activityTime_ = 0;
        NotifyAttributeUpdated( eOnUpdate );
    }
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::Update
// Created: NLD 2010-10-26
// -----------------------------------------------------------------------------
bool ObstacleAttribute::Update( const ObstacleAttribute& rhs )
{
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
    if( creationTime_ != rhs.creationTime_ )
    {
        NotifyAttributeUpdated( eOnUpdate );
        creationTime_ = rhs.creationTime_;
    }
    return NeedUpdate( eOnUpdate );
}
