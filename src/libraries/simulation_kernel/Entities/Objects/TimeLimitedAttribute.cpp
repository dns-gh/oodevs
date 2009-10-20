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
#include "Tools/MIL_Tools.h"

#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_GUID( TimeLimitedAttribute, "TimeLimitedAttribute" )

// -----------------------------------------------------------------------------
// Name: TimeLimitedAttributeconstructor
/* When the time limitation value is set to 0, attribute will never end
*/
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
TimeLimitedAttribute::TimeLimitedAttribute( xml::xistream& xis )
    : nActivityTime_ ( 0 )
    , nDeathTimeStep_ ( 0 )
{
    xis >> xml::attribute( "value", nActivityTime_ ) // s
        >> xml::optional() >> xml::attribute( "initial", nDeathTimeStep_ );
    if( nActivityTime_ < 0 )
        xis.error( "nMinesActivityTime_ is not greater than or equal to 0" );
}

// -----------------------------------------------------------------------------
// Name: TimeLimitedAttributeconstructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
TimeLimitedAttribute::TimeLimitedAttribute( uint nActivityTime )
    : nActivityTime_ ( nActivityTime )
    , nDeathTimeStep_ ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TimeLimitedAttributeconstructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
TimeLimitedAttribute::TimeLimitedAttribute()
    : nActivityTime_ ( 0 )
    , nDeathTimeStep_ ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TimeLimitedAttribute constructor
// Created: RPD 2009-10-19
// -----------------------------------------------------------------------------
TimeLimitedAttribute::TimeLimitedAttribute( const ASN1T_ObjectAttributes& asn )
    : nActivityTime_ ( asn.activity_time.value )
    , nDeathTimeStep_ ( 0 )
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: TimeLimitedAttributeconstructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
TimeLimitedAttribute::TimeLimitedAttribute( const TimeLimitedAttribute& from )
    : nActivityTime_ ( from.nActivityTime_ )
    , nDeathTimeStep_ ( from.nDeathTimeStep_ )
{    
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TimeLimitedAttribute::operator=
// Created: JCR 2008-08-21
// -----------------------------------------------------------------------------
TimeLimitedAttribute& TimeLimitedAttribute::operator=( const TimeLimitedAttribute& from )  
{
    nActivityTime_ = from.nActivityTime_;
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
void TimeLimitedAttribute::serialize( Archive& file, const uint )
{    
    file & boost::serialization::base_object< ObjectAttribute_ABC >( *this )         
         & nActivityTime_ 
         & nDeathTimeStep_;
}

// -----------------------------------------------------------------------------
// Name: TimeLimitedAttribute::WriteSpecificAttributes
// Created: NLD 2006-05-29
// -----------------------------------------------------------------------------
void TimeLimitedAttribute::WriteODB( xml::xostream& xos ) const
{
    xos << xml::start( "activity-time" )
        << xml::attribute( "value", nActivityTime_ );
    if ( nDeathTimeStep_ > 0 )
        xos << xml::attribute( "initial", nDeathTimeStep_ );
    xos << xml::end();
}

// -----------------------------------------------------------------------------
// Name: TimeLimitedAttribute::Update
// Created: JCR 2008-08-19
// -----------------------------------------------------------------------------
bool TimeLimitedAttribute::IsTimeOver( uint time ) const
{
    if ( nActivityTime_ == 0 )
        return false;
    if( nDeathTimeStep_ == 0 )
        nDeathTimeStep_ = uint( time + MIL_Tools::ConvertSecondsToSim( nActivityTime_ ) );
    return nDeathTimeStep_ <= time;
}

// -----------------------------------------------------------------------------
// Name: TimeLimitedAttribute::SendFullState
// Created: JCR 2008-08-21
// -----------------------------------------------------------------------------
void TimeLimitedAttribute::SendFullState( ASN1T_ObjectAttributes& asn ) const
{
    asn.m.activity_timePresent = 1;
    asn.activity_time.value = nActivityTime_;
}
    
// -----------------------------------------------------------------------------
// Name: TimeLimitedAttribute::SendUpdate
// Created: JCR 2008-08-21
// -----------------------------------------------------------------------------
void TimeLimitedAttribute::SendUpdate( ASN1T_ObjectAttributes& asn ) const
{
    SendFullState( asn );
}
