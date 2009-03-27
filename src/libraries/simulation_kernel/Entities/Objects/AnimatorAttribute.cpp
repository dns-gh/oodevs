// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "AnimatorAttribute.h"
#include "MIL.h"
#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_GUID( AnimatorAttribute, "AnimatorAttribute" )

// -----------------------------------------------------------------------------
// Name: AnimatorAttribute constructor
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
AnimatorAttribute::AnimatorAttribute()
    : maxAnimators_ ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AnimatorAttribute constructor
// Created: JCR 2008-08-20
// -----------------------------------------------------------------------------
AnimatorAttribute::AnimatorAttribute( uint maxAnimator )
    : maxAnimators_ ( maxAnimator )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AnimatorAttribute constructor
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
AnimatorAttribute::AnimatorAttribute( xml::xistream& xis )
    : maxAnimators_ ( xml::attribute< int >( xis, "max-animator" ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AnimatorAttribute destructor
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
AnimatorAttribute::~AnimatorAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AnimatorAttribute::operator=
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
AnimatorAttribute& AnimatorAttribute::operator=( const AnimatorAttribute& rhs )
{
    maxAnimators_ = rhs.maxAnimators_;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void AnimatorAttribute::serialize
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
template< typename Archive > 
void AnimatorAttribute::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    file & maxAnimators_;
}

// -----------------------------------------------------------------------------
// Name: AnimatorAttribute::CanBeAnimatedBy
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
bool AnimatorAttribute::CanBeAnimatedBy( const MIL_Agent_ABC& agent ) const
{
    return animators_.size() < maxAnimators_ 
           || ( animators_.find( &agent ) != animators_.end() );
}

// -----------------------------------------------------------------------------
// Name: AnimatorAttribute::AddAnimator
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
bool AnimatorAttribute::AddAnimator( const MIL_Agent_ABC& agent )
{
    if ( !CanBeAnimatedBy( agent ) )
        return false;
    animators_.insert( &agent );
    return true;
}

// -----------------------------------------------------------------------------
// Name: AnimatorAttribute::ReleaseAnimator
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
void AnimatorAttribute::ReleaseAnimator( const MIL_Agent_ABC& agent )
{
    animators_.erase( &agent );
}

// -----------------------------------------------------------------------------
// Name: AnimatorAttribute::GetMaxAnimator
// Created: JCR 2008-08-20
// -----------------------------------------------------------------------------
uint AnimatorAttribute::GetMaxAnimators() const
{
    return maxAnimators_;
}
    
// -----------------------------------------------------------------------------
// Name: AnimatorAttribute::GetCurrent
// Created: JCR 2008-08-20
// -----------------------------------------------------------------------------
uint AnimatorAttribute::GetCurrent() const
{
    return animators_.size();
}


// -----------------------------------------------------------------------------
// Name: AnimatorAttribute::GetAnimators
// Created: JCR 2008-08-20
// -----------------------------------------------------------------------------
const AnimatorAttribute::T_AgentSet& AnimatorAttribute::GetAnimators() const
{
    return animators_;
}
