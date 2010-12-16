// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "FireAttribute.h"
#include "Object.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "MIL_AgentServer.h"
#include "MIL_FireClass.h"
#include "Meteo/PHY_MeteoDataManager.h"
#include "Meteo/RawVisionData/PHY_RawVisionData.h"
#include "protocol/protocol.h"
#include <xeumeuleu/xml.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( FireAttribute )

// -----------------------------------------------------------------------------
// Name: FireAttribute constructor
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
FireAttribute::FireAttribute()
    : pClass_    ( MIL_FireClass::GetDefaultFireClass() )
    , maxCombustionEnergy_( 0 )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: FireAttribute::FireAttribute
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
FireAttribute::FireAttribute( xml::xistream& xis )
    : maxCombustionEnergy_( 0 )
    , pClass_( 0 )
{

    std::string className( xml::attribute( xis, "class", std::string() ) );
    pClass_ = MIL_FireClass::Find( className );
    if( !pClass_ )
        xis.error( "Unknown 'Fire class' '" + className + "' for fire object attribute" );
    xis >> xml::attribute( "max-combustion-energy", maxCombustionEnergy_ );
}

// -----------------------------------------------------------------------------
// Name: FireAttribute constructor
// Created: JCR 2008-07-21
// -----------------------------------------------------------------------------
FireAttribute::FireAttribute( const sword::MsgMissionParameter_Value& attributes )
    : maxCombustionEnergy_( 0 )
    , pClass_   ( 0 )
{
    int todo = 0; // $$$$ BCI 2010-12-07: à revoir...
    pClass_ = MIL_FireClass::Find( attributes.list( 1 ).acharstr() );
    if( !pClass_ )
        throw std::runtime_error( "Unknown 'Fire class' for fire object attribute" );
//    asn.fire.heat
}

// -----------------------------------------------------------------------------
// Name: FireAttribute destructor
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
FireAttribute::~FireAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FireAttribute::load
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
void FireAttribute::load( MIL_CheckPointInArchive& ar, const unsigned int )
{
    std::string className;
    ar >> boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    ar >> className
       >> maxCombustionEnergy_;
    pClass_ = MIL_FireClass::Find( className );
    if( !pClass_ )
        throw std::runtime_error( "Unknown 'Fire class' '" + className + "' for fire object attribute" );
}

// -----------------------------------------------------------------------------
// Name: FireAttribute::save
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
void FireAttribute::save( MIL_CheckPointOutArchive& ar, const unsigned int ) const
{
    ar << boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    ar << pClass_->GetName()
       << maxCombustionEnergy_;
}

// -----------------------------------------------------------------------------
// Name: FireAttribute::Register
// Created: JSR 2010-03-12
// -----------------------------------------------------------------------------
void FireAttribute::Register( MIL_Object_ABC& object ) const
{
    object.SetAttribute< FireAttribute, FireAttribute >( *this );
}

// -----------------------------------------------------------------------------
// Name: FireAttribute::GetInitialHeat
// Created: BCI 2010-12-10
// -----------------------------------------------------------------------------
int FireAttribute::GetInitialHeat() const
{
    return pClass_->GetInitialHeat();
}

// -----------------------------------------------------------------------------
// Name: FireAttribute::GetDecreaseRate
// Created: BCI 2010-12-10
// -----------------------------------------------------------------------------
int FireAttribute::GetDecreaseRate() const
{
    return pClass_->GetDecreaseRate();
}
// -----------------------------------------------------------------------------
// Name: FireAttribute::GetIncreaseRate
// Created: BCI 2010-12-10
// -----------------------------------------------------------------------------
int FireAttribute::GetIncreaseRate()
{
    return pClass_->GetIncreaseRate();
}
// -----------------------------------------------------------------------------
// Name: FireAttribute::GetMaxHeat
// Created: BCI 2010-12-10
// -----------------------------------------------------------------------------
int FireAttribute::GetMaxHeat() const
{
    return pClass_->GetMaxHeat();
}
// -----------------------------------------------------------------------------
// Name: FireAttribute::GetCellSize
// Created: BCI 2010-12-10
// -----------------------------------------------------------------------------
int FireAttribute::GetCellSize() const
{
    return pClass_->GetCellSize();
}

// -----------------------------------------------------------------------------
// Name: FireAttribute::GetMaxCombustionEnergy
// Created: BCI 2010-12-10
// -----------------------------------------------------------------------------
int FireAttribute::GetMaxCombustionEnergy() const
{
    return maxCombustionEnergy_;
}

// -----------------------------------------------------------------------------
// Name: FireAttribute::SendFullState
// Created: BCI 2010-12-13
// -----------------------------------------------------------------------------
void FireAttribute::SendFullState( sword::ObjectAttributes& asn ) const
{
    asn.mutable_fire()->set_class_name( pClass_->GetName() );
    asn.mutable_fire()->set_max_combustion_energy( maxCombustionEnergy_ );
}

// -----------------------------------------------------------------------------
// Name: FireAttribute::GetWeatherDecreateRate
// Created: BCI 2010-12-13
// -----------------------------------------------------------------------------
int FireAttribute::GetWeatherDecreateRate( const MIL_Object_ABC& object ) const
{
    return pClass_->GetWeatherDecreateRate( MIL_AgentServer::GetWorkspace().GetMeteoDataManager().GetRawVisionData().GetPrecipitation( object.GetLocalisation().GetCircleCenter() ) );
}

// -----------------------------------------------------------------------------
// Name: FireAttribute::GetBurnEffect
// Created: BCI 2010-12-14
// -----------------------------------------------------------------------------
MIL_BurnEffectManipulator FireAttribute::GetBurnEffect()
{
    return MIL_BurnEffectManipulator( *pClass_ );
}
