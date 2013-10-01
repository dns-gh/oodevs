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
#include "Entities/Agents/MIL_Agent_ABC.h"

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
    animators_ = rhs.animators_;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void AnimatorAttribute::load
// Created: LDC 2012-03-07
// -----------------------------------------------------------------------------
template< typename Archive > void AnimatorAttribute::load( Archive& file, const unsigned int )
{
    file >> boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    file >> maxAnimators_;
    std::size_t number;
    file >> number;
    while( number-- )
    {
        MIL_Agent_ABC* agent;
        file >> agent;
        animators_.insert( agent );
    }
}

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void AnimatorAttribute::save
// Created: LDC 2012-03-07
// -----------------------------------------------------------------------------
template< typename Archive > void AnimatorAttribute::save( Archive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    file << maxAnimators_;
    std::size_t number = animators_.size();
    file << number;
    for( auto it = animators_.begin(); it != animators_.end(); ++it )
        file << *it;
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
// Name: AnimatorAttribute::GetAnimatorsRatio
// Created: JSR 2011-07-20
// -----------------------------------------------------------------------------
double AnimatorAttribute::GetAnimatorsRatio() const
{
    if( maxAnimators_ )
        return static_cast< double >( animators_.size() ) / maxAnimators_;
    else
        return 1.;
}

// -----------------------------------------------------------------------------
// Name: AnimatorAttribute::GetMaxAnimators
// Created: JSR 2012-09-06
// -----------------------------------------------------------------------------
int AnimatorAttribute::GetMaxAnimators() const
{
    return maxAnimators_;
}

// -----------------------------------------------------------------------------
// Name: AnimatorAttribute::GetAnimators
// Created: JCR 2008-08-20
// -----------------------------------------------------------------------------
const AnimatorAttribute::T_AgentSet& AnimatorAttribute::GetAnimators() const
{
    return animators_;
}
