// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "BurnAttribute.h"
#include "FireAttribute.h"
#include "CheckPoints/MIL_CheckPointInArchive.h"
#include "CheckPoints/MIL_CheckPointOutArchive.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "protocol/Protocol.h"

BOOST_CLASS_EXPORT_IMPLEMENT( BurnAttribute )

// -----------------------------------------------------------------------------
// Name: BurnAttribute constructor
// Created: BCI 2010-12-09
// -----------------------------------------------------------------------------
BurnAttribute::BurnAttribute()
    : currentHeat_( 0 )
    , combustionEnergy_( 0 )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: BurnAttribute constructor
// Created: BCI 2010-12-09
// -----------------------------------------------------------------------------
BurnAttribute::BurnAttribute( const BurnAttribute& other )
: currentHeat_( other.currentHeat_ )
, combustionEnergy_( other.combustionEnergy_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BurnAttribute destructor
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
BurnAttribute::~BurnAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BurnAttribute::operator=
// Created: BCI 2010-12-09
// -----------------------------------------------------------------------------
BurnAttribute& BurnAttribute::operator=( const BurnAttribute& other )
{
    currentHeat_ = other.currentHeat_;
    combustionEnergy_ = other.combustionEnergy_;

    NotifyAttributeUpdated( eOnUpdate );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: BurnAttribute::load
// Created: BCI 2010-12-09
// -----------------------------------------------------------------------------
void BurnAttribute::load( MIL_CheckPointInArchive& ar, const unsigned int )
{
    ar >> boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    ar >> currentHeat_;
    ar >> combustionEnergy_;
}

// -----------------------------------------------------------------------------
// Name: BurnAttribute::save
// Created: BCI 2010-12-09
// -----------------------------------------------------------------------------
void BurnAttribute::save( MIL_CheckPointOutArchive& ar, const unsigned int ) const
{
    ar << boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    ar << currentHeat_;
    ar << combustionEnergy_;
}

// -----------------------------------------------------------------------------
// Name: BurnAttribute::Instanciate
// Created: BCI 2010-12-09
// -----------------------------------------------------------------------------
void BurnAttribute::Instanciate( DEC_Knowledge_Object& /*object*/ ) const
{
    // $$$$ BCI 2010-12-21: todo?
    /*object.Attach< DEC_Knowledge_ObjectAttributeProxy_ABC< BurnAttribute > >( *new T_KnowledgeProxyType() );*/
}

// -----------------------------------------------------------------------------
// Name: BurnAttribute::Register
// Created: BCI 2010-12-09
// -----------------------------------------------------------------------------
void BurnAttribute::Register( MIL_Object_ABC& object ) const
{
    object.SetAttribute< BurnAttribute, BurnAttribute >( *this );
}

// -----------------------------------------------------------------------------
// Name: BurnAttribute::SendFullState
// Created: BCI 2010-12-09
// -----------------------------------------------------------------------------
void BurnAttribute::SendFullState( sword::ObjectAttributes& asn ) const
{
//    asn.set_firePresent( 1 );
    asn.mutable_burn()->set_current_heat( currentHeat_ );
    asn.mutable_burn()->set_combustion_energy( combustionEnergy_ );
}

// -----------------------------------------------------------------------------
// Name: BurnAttribute::Send
// Created: BCI 2010-12-09
// -----------------------------------------------------------------------------
bool BurnAttribute::SendUpdate( sword::ObjectAttributes& asn ) const
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
// Name: BurnAttribute::WriteODB
// Created: BCI 2010-12-09
// -----------------------------------------------------------------------------
void BurnAttribute::WriteODB( xml::xostream& xos ) const
{
    xos << xml::start( "burn" )
            << xml::attribute( "current-heat", currentHeat_ )
            << xml::attribute( "combustion-energy", combustionEnergy_ )
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: BurnAttribute::StartBurn
// Created: BCI 2010-12-10
// -----------------------------------------------------------------------------
void BurnAttribute::StartBurn( MIL_Object_ABC& object )
{
    currentHeat_ = object.GetAttribute< FireAttribute >().GetInitialHeat();
    combustionEnergy_ = 0;
}

namespace
{
    template< typename T >
    T bound( T mini, T value, T maxi )
    {
        return std::max( mini, std::min( value, maxi ) );
    }
}

// -----------------------------------------------------------------------------
// Name: BurnAttribute::Burn
// Created: BCI 2010-12-10
// -----------------------------------------------------------------------------
void BurnAttribute::Burn( MIL_Object_ABC& object )
{
    FireAttribute& fireAttribute = object.GetAttribute< FireAttribute >();
    int weatherDecreateRate = fireAttribute.GetWeatherDecreateRate( object );
    if( combustionEnergy_ < fireAttribute.GetMaxCombustionEnergy() )
    {
        currentHeat_ = bound( 0, currentHeat_ + fireAttribute.GetIncreaseRate() - weatherDecreateRate, fireAttribute.GetMaxHeat() );
        combustionEnergy_ += currentHeat_;
        NotifyAttributeUpdated( eOnUpdate );
    }
    else
    {
        int newCurrentHeat = bound( 0, currentHeat_ - fireAttribute.GetDecreaseRate() - weatherDecreateRate, fireAttribute.GetMaxHeat() );
        if( currentHeat_ != newCurrentHeat )
        {
            currentHeat_ = newCurrentHeat;
            NotifyAttributeUpdated( eOnUpdate );
        }
    }
    if( currentHeat_ == 0 )
        object.MarkForDestruction();
}

// -----------------------------------------------------------------------------
// Name: BurnAttribute::GetCurrentHeat
// Created: BCI 2011-01-25
// -----------------------------------------------------------------------------
int BurnAttribute::GetCurrentHeat() const
{
    return currentHeat_;
}

// -----------------------------------------------------------------------------
// Name: BurnAttribute::Extinguish
// Created: BCI 2011-01-25
// -----------------------------------------------------------------------------
void BurnAttribute::Extinguish( MIL_Object_ABC& object, const PHY_DotationCategory& extinguisherAgent )
{
    FireAttribute& fireAttribute = object.GetAttribute< FireAttribute >();
    int decreaseRate = fireAttribute.GetExtinguisherHeatDecreaseRate( extinguisherAgent );
    int newCurrentHeat = bound( 0, currentHeat_ - decreaseRate, fireAttribute.GetMaxHeat() );
    if( currentHeat_ != newCurrentHeat )
    {
        currentHeat_ = newCurrentHeat;
        NotifyAttributeUpdated( eOnUpdate );
    }
    if( currentHeat_ == 0 )
        object.MarkForDestruction();
}

// -----------------------------------------------------------------------------
// Name: BurnAttribute::IsExtinguished
// Created: BCI 2011-01-25
// -----------------------------------------------------------------------------
bool BurnAttribute::IsExtinguished() const
{
    return currentHeat_ == 0;
}
