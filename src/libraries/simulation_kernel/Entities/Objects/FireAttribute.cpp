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
#include "protocol/Protocol.h"
#include "MT_Tools/MT_Logger.h"

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
        throw MASA_EXCEPTION( xis.context() + "Unknown 'Fire class' '" + className + "' for fire object attribute" );
    xis >> xml::attribute( "max-combustion-energy", maxCombustionEnergy_ );
}

// -----------------------------------------------------------------------------
// Name: FireAttribute constructor
// Created: JCR 2008-07-21
// -----------------------------------------------------------------------------
FireAttribute::FireAttribute( const sword::MissionParameter_Value& attributes )
    : maxCombustionEnergy_( 0 )
    , pClass_   ( 0 )
{
    pClass_ = MIL_FireClass::Find( attributes.list( 1 ).acharstr() );
    if( !pClass_ )
    {
        MT_LOG_ERROR_MSG( "Unknown 'Fire class' '" << attributes.list( 1 ).acharstr() << "' for fire object attribute" );
        throw MASA_EXCEPTION( "Unknown 'Fire class' for fire object attribute" );
    }
    maxCombustionEnergy_ = attributes.list( 2 ).quantity();
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
        throw MASA_EXCEPTION( "Unknown 'Fire class' '" + className + "' for fire object attribute" );
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
// Name: FireAttribute::WriteODB
// Created: LDC 2013-01-03
// -----------------------------------------------------------------------------
void FireAttribute::WriteODB( xml::xostream& xos ) const
{
    if( !pClass_ )
    {
        MT_LOG_ERROR_MSG( "No 'Fire class' to save for fire object attribute" );
    }
    xos << xml::start( "fire" )
            << xml::attribute( "class", pClass_ ? pClass_->GetName()  : "" )
            << xml::attribute( "max-combustion-energy", maxCombustionEnergy_ )
        << xml::end;
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
// Name: FireAttribute::SendUpdate
// Created: LGY 2012-01-17
// -----------------------------------------------------------------------------
bool FireAttribute::SendUpdate( sword::ObjectAttributes& asn ) const
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

// -----------------------------------------------------------------------------
// Name: FireAttribute::GetSurfaceFirePotentials
// Created: BCI 2010-12-20
// -----------------------------------------------------------------------------
void FireAttribute::GetSurfaceFirePotentials( const TerrainData& terrainData, int& ignitionThreshold, int& maxCombustionEnergy ) const
{
    pClass_->GetSurfaceFirePotentials( terrainData, ignitionThreshold, maxCombustionEnergy );
    if( maxCombustionEnergy > maxCombustionEnergy_ )
        maxCombustionEnergy = maxCombustionEnergy_;
}

// -----------------------------------------------------------------------------
// Name: FireAttribute constructor
// Created: BCI 2011-01-07
// -----------------------------------------------------------------------------
FireAttribute::FireAttribute( const FireAttribute& other )
: pClass_( other.pClass_ )
, maxCombustionEnergy_( other.maxCombustionEnergy_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FireAttribute::operator=
// Created: BCI 2011-01-07
// -----------------------------------------------------------------------------
FireAttribute& FireAttribute::operator=( const FireAttribute& other )
{
    if( this != &other )
    {
        pClass_ = other.pClass_;
        maxCombustionEnergy_ = other.maxCombustionEnergy_;
        NotifyAttributeUpdated( eOnUpdate );
    }
    return *this;
}

// -----------------------------------------------------------------------------
// Name: FireAttribute::GetClass
// Created: BCI 2011-01-11
// -----------------------------------------------------------------------------
const MIL_FireClass& FireAttribute::GetClass() const
{
    return *pClass_;
}

// -----------------------------------------------------------------------------
// Name: FireAttribute::FindBestExtinguisherAgent
// Created: BCI 2011-01-25
// -----------------------------------------------------------------------------
const PHY_DotationCategory* FireAttribute::FindBestExtinguisherAgent( std::function< bool( const PHY_DotationCategory& ) > isExtinguisherAgentOkFun ) const
{
    return pClass_->FindBestExtinguisherAgent( isExtinguisherAgentOkFun );
}

// -----------------------------------------------------------------------------
// Name: FireAttribute::GetExtinguisherHeatDecreaseRate
// Created: BCI 2011-01-25
// -----------------------------------------------------------------------------
int FireAttribute::GetExtinguisherHeatDecreaseRate( const PHY_DotationCategory& extinguisherAgent ) const
{
    return pClass_->GetExtinguisherHeatDecreaseRate( extinguisherAgent );
}

// -----------------------------------------------------------------------------
// Name: FireAttribute::SetFireClass
// Created: LGY 2011-01-25
// -----------------------------------------------------------------------------
void FireAttribute::SetFireClass( const std::string& className, unsigned int maxCombustion )
{
    pClass_ = MIL_FireClass::Find( className );
    if( !pClass_ )
        throw MASA_EXCEPTION( "Unknown 'Fire class' for fire object attribute" );
    maxCombustionEnergy_ = maxCombustion;
}
