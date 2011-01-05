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
    , combustionEnergySum_( 0 )
    , combustionEnergyCount_( 0 )
    , currentCombustionEnergy_( 0 )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: BurnAttribute constructor
// Created: BCI 2010-12-09
// -----------------------------------------------------------------------------
BurnAttribute::BurnAttribute( const BurnAttribute& other )
: currentHeat_( other.currentHeat_ )
, combustionEnergySum_( other.combustionEnergySum_ )
, combustionEnergyCount_( other.combustionEnergyCount_ )
, currentCombustionEnergy_( other.currentCombustionEnergy_ )
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
    combustionEnergySum_ = other.combustionEnergySum_;
    combustionEnergyCount_ = other.combustionEnergyCount_;
    currentCombustionEnergy_ = other.currentCombustionEnergy_;

    NotifyAttributeUpdated( eOnUpdate | eOnHLAUpdate );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: BurnAttribute::load
// Created: BCI 2010-12-09
// -----------------------------------------------------------------------------
void BurnAttribute::load( MIL_CheckPointInArchive& ar, const unsigned int )
{
    std::string className;
    ar >> boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    ar >> currentHeat_
       >> combustionEnergySum_
       >> combustionEnergyCount_;
}

// -----------------------------------------------------------------------------
// Name: BurnAttribute::save
// Created: BCI 2010-12-09
// -----------------------------------------------------------------------------
void BurnAttribute::save( MIL_CheckPointOutArchive& ar, const unsigned int ) const
{
    ar << boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    ar << currentHeat_
       << combustionEnergySum_
       << combustionEnergyCount_;
}


// -----------------------------------------------------------------------------
// Name: BurnAttribute::Instanciate
// Created: BCI 2010-12-09
// -----------------------------------------------------------------------------
void BurnAttribute::Instanciate( DEC_Knowledge_Object& /*object*/ ) const
{
    // $$$$ BCI 2010-12-21: todo
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
    asn.mutable_burn()->set_combustion_energy_sum( combustionEnergySum_ );
    asn.mutable_burn()->set_combustion_energy_count( combustionEnergyCount_ );
    int todo = 0;
}

// -----------------------------------------------------------------------------
// Name: BurnAttribute::Send
// Created: BCI 2010-12-09
// -----------------------------------------------------------------------------
void BurnAttribute::SendUpdate( sword::ObjectAttributes& asn ) const
{
    if( NeedUpdate( eOnUpdate ) )
    {
        SendFullState( asn );
        Reset( eOnUpdate );
    }
}

// -----------------------------------------------------------------------------
// Name: BurnAttribute::WriteODB
// Created: BCI 2010-12-09
// -----------------------------------------------------------------------------
void BurnAttribute::WriteODB( xml::xostream& xos ) const
{
    xos << xml::start( "burn" )
            << xml::attribute( "current-heat", currentHeat_ )
            << xml::attribute( "combustion-energy-sum", combustionEnergySum_ )
            << xml::attribute( "combustion-energy-count", combustionEnergyCount_ )
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: BurnAttribute::StartBurn
// Created: BCI 2010-12-10
// -----------------------------------------------------------------------------
void BurnAttribute::StartBurn( MIL_Object_ABC& object )
{
    currentHeat_ = object.GetAttribute< FireAttribute >().GetInitialHeat();
    combustionEnergySum_ = 0;
    combustionEnergyCount_ = 0;
    currentCombustionEnergy_ = 0;
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
    if( currentCombustionEnergy_ < fireAttribute.GetMaxCombustionEnergy() )
    {
        currentHeat_ = bound( 0, currentHeat_ + fireAttribute.GetIncreaseRate() - weatherDecreateRate, fireAttribute.GetMaxHeat() );
        combustionEnergySum_ += currentHeat_;
        ++combustionEnergyCount_;
        currentCombustionEnergy_ = combustionEnergySum_ / combustionEnergyCount_;
        NotifyAttributeUpdated( eOnUpdate | eOnHLAUpdate );
    }
    else
    {
        int newCurrentHeat = bound( 0, currentHeat_ - fireAttribute.GetDecreaseRate() - weatherDecreateRate, fireAttribute.GetMaxHeat() );
        if( currentHeat_ != newCurrentHeat )
        {
            currentHeat_ = newCurrentHeat;
            NotifyAttributeUpdated( eOnUpdate | eOnHLAUpdate );
        }
    }
    if( currentHeat_ == 0 )
        object.MarkForDestruction();
}

