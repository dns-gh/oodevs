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
#include "Object.h"
#include <xeumeuleu/xml.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( AnimatorAttribute )

// -----------------------------------------------------------------------------
// Name: AnimatorAttribute constructor
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
AnimatorAttribute::AnimatorAttribute()
    : maxAnimators_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AnimatorAttribute constructor
// Created: JCR 2008-08-20
// -----------------------------------------------------------------------------
AnimatorAttribute::AnimatorAttribute( unsigned int maxAnimator )
    : maxAnimators_( maxAnimator )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AnimatorAttribute constructor
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
AnimatorAttribute::AnimatorAttribute( xml::xistream& xis )
    : maxAnimators_( xis.attribute< int >( "max-animator" ) )
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
void AnimatorAttribute::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    file & maxAnimators_;
}

// -----------------------------------------------------------------------------
// Name: AnimatorAttribute::Register
// Created: JSR 2010-03-15
// -----------------------------------------------------------------------------
void AnimatorAttribute::Register( MIL_Object_ABC& object ) const
{
    object.SetAttribute< AnimatorAttribute, AnimatorAttribute >( *this );
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
    if( !CanBeAnimatedBy( agent ) )
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
unsigned int AnimatorAttribute::GetMaxAnimators() const
{
    return maxAnimators_;
}

// -----------------------------------------------------------------------------
// Name: AnimatorAttribute::GetCurrent
// Created: JCR 2008-08-20
// -----------------------------------------------------------------------------
unsigned int AnimatorAttribute::GetCurrent() const
{
    return static_cast< unsigned >( animators_.size() );
}

// -----------------------------------------------------------------------------
// Name: AnimatorAttribute::GetAnimators
// Created: JCR 2008-08-20
// -----------------------------------------------------------------------------
const AnimatorAttribute::T_AgentSet& AnimatorAttribute::GetAnimators() const
{
    return animators_;
}
