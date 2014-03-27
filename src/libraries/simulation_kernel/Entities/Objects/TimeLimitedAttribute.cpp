// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "TimeLimitedAttribute.h"
#include "ObjectAttributes.h"
#include "Object.h"
#include "Tools/MIL_Tools.h"
#include "protocol/Protocol.h"

BOOST_CLASS_EXPORT_IMPLEMENT( TimeLimitedAttribute )

// -----------------------------------------------------------------------------
// Name: TimeLimitedAttributeconstructor
/* When the time limitation value is set to 0, attribute will never end
*/
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
TimeLimitedAttribute::TimeLimitedAttribute( xml::xistream& xis )
    : nLifeTime_     ( 0 )
    , nDeathTimeStep_( 0 )
{
    xis >> xml::attribute( "value", nLifeTime_ ) // s
        >> xml::optional >> xml::attribute( "initial", nDeathTimeStep_ );
    if( nLifeTime_ < 0 )
        throw MASA_EXCEPTION( xis.context() + "nMinesActivityTime_ is not greater than or equal to 0" );
}

// -----------------------------------------------------------------------------
// Name: TimeLimitedAttributeconstructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
TimeLimitedAttribute::TimeLimitedAttribute( unsigned int nActivityTime )
    : nLifeTime_     ( nActivityTime )
    , nDeathTimeStep_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TimeLimitedAttributeconstructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
TimeLimitedAttribute::TimeLimitedAttribute()
    : nLifeTime_     ( 0 )
    , nDeathTimeStep_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TimeLimitedAttribute constructor
// Created: RPD 2009-10-19
// -----------------------------------------------------------------------------
TimeLimitedAttribute::TimeLimitedAttribute( const sword::MissionParameter_Value& attributes )
    : nDeathTimeStep_( 0 )
{
    CheckCount( "time_limit", attributes, 2 );
    nLifeTime_ = GetQuantity( "time_limit", attributes, 1 );
}

// -----------------------------------------------------------------------------
// Name: TimeLimitedAttributeconstructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
TimeLimitedAttribute::TimeLimitedAttribute( const TimeLimitedAttribute& from )
    : nLifeTime_     ( from.nLifeTime_ )
    , nDeathTimeStep_( from.nDeathTimeStep_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TimeLimitedAttribute::operator=
// Created: JCR 2008-08-21
// -----------------------------------------------------------------------------
TimeLimitedAttribute& TimeLimitedAttribute::operator=( const TimeLimitedAttribute& from )
{
    nLifeTime_ = from.nLifeTime_;
    nDeathTimeStep_ = from.nDeathTimeStep_;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: TimeLimitedAttributedestructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
TimeLimitedAttribute::~TimeLimitedAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TimeLimitedAttribute::load
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
template< typename Archive >
void TimeLimitedAttribute::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectAttribute_ABC >( *this )
         & nLifeTime_
         & nDeathTimeStep_;
}

// -----------------------------------------------------------------------------
// Name: TimeLimitedAttribute::Register
// Created: JSR 2010-03-15
// -----------------------------------------------------------------------------
void TimeLimitedAttribute::Register( MIL_Object_ABC& object ) const
{
    object.SetAttribute< TimeLimitedAttribute, TimeLimitedAttribute >( *this );
}

// -----------------------------------------------------------------------------
// Name: TimeLimitedAttribute::WriteSpecificAttributes
// Created: NLD 2006-05-29
// -----------------------------------------------------------------------------
void TimeLimitedAttribute::WriteODB( xml::xostream& xos ) const
{
    xos << xml::start( "activity-time" )
        << xml::attribute( "value", nLifeTime_ );
    if( nDeathTimeStep_ > 0 )
        xos << xml::attribute( "initial", nDeathTimeStep_ );
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: TimeLimitedAttribute::Update
// Created: JCR 2008-08-19
// -----------------------------------------------------------------------------
bool TimeLimitedAttribute::IsTimeOver( unsigned int time ) const
{
    if( nLifeTime_ == 0 )
        return false;
    if( nDeathTimeStep_ == 0 )
        nDeathTimeStep_ = unsigned int( time + MIL_Tools::ConvertSecondsToSim( nLifeTime_ ) );
    return nDeathTimeStep_ <= time;
}

// -----------------------------------------------------------------------------
// Name: TimeLimitedAttribute::SendFullState
// Created: JCR 2008-08-21
// -----------------------------------------------------------------------------
void TimeLimitedAttribute::SendFullState( sword::ObjectAttributes& asn ) const
{
    asn.mutable_life_time()->set_value( nLifeTime_ );
}

// -----------------------------------------------------------------------------
// Name: TimeLimitedAttribute::SendUpdate
// Created: JCR 2008-08-21
// -----------------------------------------------------------------------------
bool TimeLimitedAttribute::SendUpdate( sword::ObjectAttributes& asn ) const
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
// Name: TimeLimitedAttribute::SetLifeTime
// Created: MMC 2012-05-21
// -----------------------------------------------------------------------------
void TimeLimitedAttribute::SetLifeTime( unsigned int lifeTime )
{
    nLifeTime_ = lifeTime;
}