// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/HumanFactors/PHY_RolePion_HumanFactors.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 16:42 $
// $Revision: 4 $
// $Workfile: PHY_RolePion_HumanFactors.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RolePion_HumanFactors.h"
#include "Network/NET_ASN_Messages.h"
#include "Entities/Agents/Units/HumanFactors/PHY_Morale.h"
#include "Entities/Agents/Units/HumanFactors/PHY_Experience.h"
#include "Entities/Agents/Units/HumanFactors/PHY_Tiredness.h"

#include "simulation_kernel/Entities/MIL_Entity_ABC.h"
#include "simulation_kernel/PostureComputer_ABC.h"
#include "simulation_kernel/NetworkNotificationHandler_ABC.h"
#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_GUID( PHY_RolePion_HumanFactors, "PHY_RolePion_HumanFactors" )

template< typename Archive >
void save_construct_data( Archive& archive, const PHY_RolePion_HumanFactors* role, const unsigned int /*version*/ )
{
    MIL_Entity_ABC* const entity = &role->entity_;
    archive << entity;
}

template< typename Archive >
void load_construct_data( Archive& archive, PHY_RolePion_HumanFactors* role, const unsigned int /*version*/ )
{
    MIL_Entity_ABC* entity;
    archive >> entity;
    ::new( role )PHY_RolePion_HumanFactors( *entity );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_HumanFactors::PHY_RolePion_HumanFactors( MIL_Entity_ABC& entity )
    : bHasChanged_                  ( true )
    , pMorale_                      ( &PHY_Morale    ::bon_     )
    , pExperience_                  ( &PHY_Experience::veteran_ )
    , pTiredness_                   ( &PHY_Tiredness ::normal_  )
    , entity_                       ( entity )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_HumanFactors::~PHY_RolePion_HumanFactors()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::load
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
void PHY_RolePion_HumanFactors::load( MIL_CheckPointInArchive& file, const uint )
{
    assert( !pMorale_ );
    assert( !pExperience_ );
    assert( !pTiredness_ );
    file >> boost::serialization::base_object< PHY_RoleInterface_HumanFactors >( *this );
    uint nID;
    file >> nID;
    pMorale_ = PHY_Morale::Find( nID );
    assert( pMorale_ );
    
    file >> nID;
    pExperience_ = PHY_Experience::Find( nID );
    assert( pExperience_ );
    
    file >> nID;
    pTiredness_ = PHY_Tiredness::Find( nID );
    assert( pTiredness_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::save
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
void PHY_RolePion_HumanFactors::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    assert( pMorale_ );
    assert( pExperience_ );
    assert( pTiredness_ );
    file << boost::serialization::base_object< PHY_RoleInterface_HumanFactors >( *this );

    unsigned morale     = pMorale_->GetID(),
             experience = pExperience_->GetID(),
             tiredness  = pTiredness_->GetID();
    file << morale
         << experience
         << tiredness;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::ReadOverloading
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
void PHY_RolePion_HumanFactors::ReadOverloading( xml::xistream& xis )
{
    xis >> xml::list( "FacteursHumains", *this, &PHY_RolePion_HumanFactors::ReadFacteursHumains );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::ReadFacteursHumains
// Created: ABL 2007-07-10
// -----------------------------------------------------------------------------
void PHY_RolePion_HumanFactors::ReadFacteursHumains( xml::xistream& xis )
{

    xis >> xml::list( "Fatigue", *this, &PHY_RolePion_HumanFactors::ReadFatigue )
        >> xml::list( "Moral", *this, &PHY_RolePion_HumanFactors::ReadMoral )
        >> xml::list( "Experience", *this, &PHY_RolePion_HumanFactors::ReadExperience );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::ReadFatigue
// Created: ABL 2007-07-10
// -----------------------------------------------------------------------------
void PHY_RolePion_HumanFactors::ReadFatigue( xml::xistream& xis )
{
    std::string strTmp;
    xis >> strTmp;
    pTiredness_ = PHY_Tiredness::Find( strTmp );
    if( !pTiredness_ )
        xis.error( "Unknown tiredness type" );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::ReadMoral
// Created: ABL 2007-07-10
// -----------------------------------------------------------------------------
void PHY_RolePion_HumanFactors::ReadMoral( xml::xistream& xis )
{
    std::string strTmp;
    xis >> strTmp;
    pMorale_ = PHY_Morale::Find( strTmp );
    if( !pMorale_)
        xis.error( "Unknown morale type" );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::ReadExperience
// Created: ABL 2007-07-10
// -----------------------------------------------------------------------------
void PHY_RolePion_HumanFactors::ReadExperience( xml::xistream& xis )
{
    std::string strTmp;
    xis >> strTmp;
    pExperience_ = PHY_Experience::Find( strTmp );
    if( !pExperience_ )
        xis.error( "Unknown experience type" );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::SendFullState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_HumanFactors::SendFullState( NET_ASN_MsgUnitAttributes& msg ) const
{
    msg().m.fatiguePresent    = 1;
    msg().m.moralPresent      = 1;
    msg().m.experiencePresent = 1;

    msg().fatigue    = pTiredness_ ->GetAsnID();
    msg().moral      = pMorale_    ->GetAsnID();
    msg().experience = pExperience_->GetAsnID();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::SendChangedState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_HumanFactors::SendChangedState( NET_ASN_MsgUnitAttributes& msg ) const
{
    if( bHasChanged_ )
        SendFullState( msg );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::Update
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_HumanFactors::Update( bool /*bIsDead*/ )
{
    if( HasChanged() )
    {
        entity_.Apply( &network::NetworkNotificationHandler_ABC::NotifyDataHasChanged );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::Clean
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
void PHY_RolePion_HumanFactors::Clean()
{
    bHasChanged_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::HasChanged
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
bool PHY_RolePion_HumanFactors::HasChanged() const
{
    return bHasChanged_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::SetMorale
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
void PHY_RolePion_HumanFactors::SetMorale( const PHY_Morale& morale )
{
    if( morale == *pMorale_ )
        return;

    pMorale_     = &morale;
    bHasChanged_ = true;
}
    
// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::SetExperience
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
void PHY_RolePion_HumanFactors::SetExperience( const PHY_Experience& experience )
{
    if( experience == *pExperience_ )
        return;

    pExperience_ = &experience;
    bHasChanged_ = true;
}
 
// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::SetTiredness
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
void PHY_RolePion_HumanFactors::SetTiredness( const PHY_Tiredness& tiredness )
{
    if( tiredness == *pTiredness_ )
        return;

    pTiredness_  = &tiredness;
    bHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::ModifyMaxSpeed
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
MT_Float PHY_RolePion_HumanFactors::ModifyMaxSpeed( MT_Float rSpeed ) const
{
    return rSpeed * pExperience_->GetCoefMaxSpeedModificator() * pTiredness_->GetCoefMaxSpeedModificator();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::ModifyReloadingDuration
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
MT_Float PHY_RolePion_HumanFactors::ModifyReloadingDuration( MT_Float rDuration ) const
{
    return rDuration * pExperience_->GetCoefReloadingTimeModificator() * pTiredness_->GetCoefReloadingTimeModificator();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::ModifyPH
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
MT_Float PHY_RolePion_HumanFactors::ModifyPH( MT_Float rPH ) const
{
    return rPH * pExperience_->GetCoefPhModificator() * pTiredness_->GetCoefPhModificator();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::GetSensorDistanceModificator
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
MT_Float PHY_RolePion_HumanFactors::GetSensorDistanceModificator() const
{
    return pExperience_->GetCoefSensorDistanceModificator() * pTiredness_->GetCoefSensorDistanceModificator();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::GetMorale
// Created: JVT 2004-11-30
// -----------------------------------------------------------------------------
const PHY_Morale& PHY_RolePion_HumanFactors::GetMorale() const
{
    assert( pMorale_ );
    return *pMorale_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::GetTiredness
// Created: JVT 2004-11-30
// -----------------------------------------------------------------------------
const PHY_Tiredness& PHY_RolePion_HumanFactors::GetTiredness() const
{
    assert( pTiredness_ );
    return *pTiredness_;
}

// -----------------------------------------------------------------------------
// Name: DefaultPostureComputer::Execute
// Created: MGD 2009-09-22
// -----------------------------------------------------------------------------
void PHY_RolePion_HumanFactors::Execute( posture::PostureComputer_ABC& algorithm ) const
{
    algorithm.AddCoefficientModifier( pExperience_->GetCoefPostureTimeModificator() );
    algorithm.AddCoefficientModifier( pTiredness_->GetCoefPostureTimeModificator() );
}
