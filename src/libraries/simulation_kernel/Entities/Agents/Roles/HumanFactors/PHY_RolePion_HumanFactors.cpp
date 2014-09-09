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
#include "Entities/Agents/Roles/Dotations/PHY_RolePion_Dotations.h"
#include "Entities/Agents/Units/Dotations/PHY_ConsumptionType.h"
#include "Entities/Agents/Units/HumanFactors/PHY_Morale.h"
#include "Entities/Agents/Units/HumanFactors/PHY_Experience.h"
#include "Entities/Agents/Units/HumanFactors/PHY_Tiredness.h"
#include "Entities/Agents/Units/HumanFactors/PHY_Stress.h"
#include "simulation_kernel/Entities/MIL_Entity_ABC.h"
#include "simulation_kernel/PostureComputer_ABC.h"
#include "simulation_kernel/NetworkNotificationHandler_ABC.h"
#include "simulation_kernel/WeaponReloadingComputer_ABC.h"
#include "simulation_kernel/SpeedComputer_ABC.h"
#include "simulation_kernel/PerceptionDistanceComputer.h"
#include "MIL_AgentServer.h"
#include "tools/ExerciseSettings.h"
#include "protocol/ClientSenders.h"

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_RolePion_HumanFactors )

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors constructor
// Created: JSR 2013-01-09
// -----------------------------------------------------------------------------
PHY_RolePion_HumanFactors::PHY_RolePion_HumanFactors()
    : owner_         ( 0 )
    , bHasChanged_   ( true )
    , pMorale_       ( &PHY_Morale::bon_ )
    , pExperience_   ( &PHY_Experience::veteran_ )
    , pTiredness_    ( &PHY_Tiredness::normal_ )
    , pStress_       ( &PHY_Stress::calm_ )
    , stressValue_   ( 0.f )
    , tirednessValue_( 0.f )
{
    if( MIL_AgentServer::GetWorkspace().GetSettings().GetValue< bool >( "human-evolution" ) )
        evolutionFunction_ = &PHY_RolePion_HumanFactors::Evolution;
    else
        evolutionFunction_ = &PHY_RolePion_HumanFactors::NoEvolution;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_HumanFactors::PHY_RolePion_HumanFactors( MIL_Entity_ABC& entity )
    : owner_         ( &entity )
    , bHasChanged_   ( true )
    , pMorale_       ( &PHY_Morale::bon_ )
    , pExperience_   ( &PHY_Experience::veteran_ )
    , pTiredness_    ( &PHY_Tiredness::normal_ )
    , pStress_       ( &PHY_Stress::calm_ )
    , stressValue_   ( 0.f )
    , tirednessValue_( 0.f )
{
    if( MIL_AgentServer::GetWorkspace().GetSettings().GetValue< bool >( "human-evolution" ) )
        evolutionFunction_ = &PHY_RolePion_HumanFactors::Evolution;
    else
        evolutionFunction_ = &PHY_RolePion_HumanFactors::NoEvolution;
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
void PHY_RolePion_HumanFactors::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< PHY_RoleInterface_HumanFactors >( *this );
    file >> owner_;
    unsigned int nID;
    file >> nID;
    pMorale_ = PHY_Morale::Find( nID );
    assert( pMorale_ );

    file >> nID;
    pExperience_ = PHY_Experience::Find( nID );
    assert( pExperience_ );

    file >> nID;
    pTiredness_ = PHY_Tiredness::Find( nID );
    assert( pTiredness_ );

    file >> nID;
    pStress_ = PHY_Stress::Find( nID );
    assert( pStress_ );
    file >> tirednessValue_;
    file >> stressValue_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::save
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
void PHY_RolePion_HumanFactors::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    assert( pMorale_ );
    assert( pExperience_ );
    assert( pTiredness_ );
    assert( pStress_ );
    file << boost::serialization::base_object< PHY_RoleInterface_HumanFactors >( *this );

    unsigned int morale = pMorale_->GetID();
    unsigned int experience = pExperience_->GetID();
    unsigned int tiredness = pTiredness_->GetID();
    unsigned int stress = pStress_->GetID();
    file << owner_;
    file << morale;
    file << experience;
    file << tiredness;
    file << stress;
    file << tirednessValue_;
    file << stressValue_;
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
        >> xml::list( "Experience", *this, &PHY_RolePion_HumanFactors::ReadExperience )
        >> xml::list( "Stress", *this, &PHY_RolePion_HumanFactors::ReadStress );
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
        throw MASA_EXCEPTION( xis.context() + "Unknown tiredness type" );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::ReadStress
// Created: LDC 2011-06-17
// -----------------------------------------------------------------------------
void PHY_RolePion_HumanFactors::ReadStress( xml::xistream& xis )
{
    std::string strTmp;
    xis >> strTmp;
    pStress_ = PHY_Stress::Find( strTmp );
    if( !pStress_ )
        throw MASA_EXCEPTION( xis.context() + "Unknown stress type" );
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
        throw MASA_EXCEPTION( xis.context() + "Unknown morale type" );
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
        throw MASA_EXCEPTION( xis.context() + "Unknown experience type" );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::SendFullState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_HumanFactors::SendFullState( client::UnitAttributes& msg ) const
{
    msg().set_tiredness( pTiredness_->GetAsnID() );
    msg().set_morale( pMorale_->GetAsnID() );
    msg().set_experience( pExperience_->GetAsnID() );
    msg().set_stress( pStress_->GetAsnID() );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::SendChangedState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_HumanFactors::SendChangedState( client::UnitAttributes& msg ) const
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
    ( this->*evolutionFunction_ )();
    if( HasChanged() )
        owner_->Apply( &network::NetworkNotificationHandler_ABC::NotifyDataHasChanged );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::NoEvolution
// Created: ABR 2011-12-14
// -----------------------------------------------------------------------------
void PHY_RolePion_HumanFactors::NoEvolution()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::Evolution
// Created: ABR 2011-12-14
// -----------------------------------------------------------------------------
void PHY_RolePion_HumanFactors::Evolution()
{
    UpdateStressValue();
    UpdateStress();
    UpdateTirednessValue();
    UpdateTiredness();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::UpdateTirednessValue
// Created: ABR 2011-12-13
// -----------------------------------------------------------------------------
void PHY_RolePion_HumanFactors::UpdateTirednessValue()
{
    const PHY_ConsumptionType& consumptionType = owner_->GetRole< dotation::PHY_RolePion_Dotations >().GetConsumptionMode();
    if( consumptionType.GetID() == PHY_ConsumptionType::engineStopped_.GetID() )
        tirednessValue_ += PHY_Tiredness::evolution_.engineStopped_;
    else if( consumptionType.GetID() == PHY_ConsumptionType::engineStarted_.GetID() )
        tirednessValue_ += PHY_Tiredness::evolution_.engineRunning_;
    else if( consumptionType.GetID() == PHY_ConsumptionType::moving_.GetID() )
        tirednessValue_ += PHY_Tiredness::evolution_.moving_;
    else if( consumptionType.GetID() == PHY_ConsumptionType::working_.GetID() )
        tirednessValue_ += PHY_Tiredness::evolution_.working_;
    if( tirednessValue_ < 0.f )
        tirednessValue_ = 0.f;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::UpdateStressValue
// Created: ABR 2011-12-13
// -----------------------------------------------------------------------------
void PHY_RolePion_HumanFactors::UpdateStressValue()
{
    stressValue_ += PHY_Stress::evolution_.decPerHour_;
    if( stressValue_ < 0.f )
        stressValue_ = 0.f;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::NotifyAttacked
// Created: ABR 2011-12-13
// -----------------------------------------------------------------------------
void PHY_RolePion_HumanFactors::NotifyAttacked()
{
    if( evolutionFunction_ == &PHY_RolePion_HumanFactors::Evolution )
    {
        stressValue_ += PHY_Stress::evolution_.incPerShot_;
        UpdateStress();
        if( HasChanged() )
            owner_->Apply( &network::NetworkNotificationHandler_ABC::NotifyDataHasChanged );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::UpdateTiredness
// Created: ABR 2011-12-13
// -----------------------------------------------------------------------------
void PHY_RolePion_HumanFactors::UpdateTiredness()
{
    if( *pTiredness_ == PHY_Tiredness::normal_ )
    {
        if( tirednessValue_ >= PHY_Tiredness::evolution_.firstThreshold_ )
            SetTiredness( PHY_Tiredness::fatigue_ );
    }
    else if( *pTiredness_ == PHY_Tiredness::fatigue_ )
    {
        if( tirednessValue_ == 0.f )
            SetTiredness( PHY_Tiredness::normal_ );
        else if( tirednessValue_ >= PHY_Tiredness::evolution_.secondThreshold_ )
            SetTiredness( PHY_Tiredness::epuise_ );
    }
    else if( *pTiredness_ == PHY_Tiredness::epuise_ )
    {
        if( tirednessValue_ < PHY_Tiredness::evolution_.firstThreshold_ )
            SetTiredness( PHY_Tiredness::fatigue_ );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::UpdateStress
// Created: ABR 2011-12-13
// -----------------------------------------------------------------------------
void PHY_RolePion_HumanFactors::UpdateStress()
{
    if( *pStress_ == PHY_Stress::calm_ )
    {
        if( stressValue_ >= PHY_Stress::evolution_.firstThreshold_ )
            SetStress( PHY_Stress::worried_ );
    }
    else if( *pStress_ == PHY_Stress::worried_ )
    {
        if( stressValue_ == 0.f )
            SetStress( PHY_Stress::calm_ );
        else if( stressValue_ >= PHY_Stress::evolution_.secondThreshold_ )
            SetStress( PHY_Stress::stressed_ );
    }
    else if( *pStress_ == PHY_Stress::stressed_ )
    {
        if( stressValue_ < PHY_Stress::evolution_.firstThreshold_ )
            SetStress( PHY_Stress::worried_ );
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
void PHY_RolePion_HumanFactors::SetTiredness( const PHY_Tiredness& tiredness, bool updateValue /*=false*/ )
{
    if( tiredness == *pTiredness_ )
        return;
    pTiredness_  = &tiredness;
    bHasChanged_ = true;
    if( updateValue )
    {
        if( *pTiredness_ == PHY_Tiredness::normal_ )
            tirednessValue_ = 0;
        else if( *pTiredness_ == PHY_Tiredness::fatigue_ )
            tirednessValue_ = PHY_Tiredness::evolution_.firstThreshold_;
        else if( *pTiredness_ == PHY_Tiredness::epuise_ )
            tirednessValue_ = PHY_Tiredness::evolution_.secondThreshold_;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::SetStress
// Created: LDC 2011-06-17
// -----------------------------------------------------------------------------
void PHY_RolePion_HumanFactors::SetStress( const PHY_Stress& stress, bool updateValue /*=false*/ )
{
    if( stress == *pStress_ )
        return;
    pStress_  = &stress;
    bHasChanged_ = true;
    if( updateValue )
    {
        if( *pStress_ == PHY_Stress::calm_ )
            stressValue_ = 0;
        else if( *pStress_ == PHY_Stress::worried_ )
            stressValue_ = PHY_Stress::evolution_.firstThreshold_;
        else if( *pStress_ == PHY_Stress::stressed_ )
            stressValue_ = PHY_Stress::evolution_.secondThreshold_;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::Execute
// Created: AHC 2009-10-02
// -----------------------------------------------------------------------------
void PHY_RolePion_HumanFactors::Execute( moving::SpeedComputer_ABC& algorithm ) const
{
    algorithm.AddModifier(pExperience_->GetCoefMaxSpeedModificator() * pTiredness_->GetCoefMaxSpeedModificator() * pStress_->GetCoefMaxSpeedModificator(),true);
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::Execute
// Created: NLD 2009-10-05
// -----------------------------------------------------------------------------
void PHY_RolePion_HumanFactors::Execute( firing::WeaponReloadingComputer_ABC& algorithm ) const
{
    algorithm.AddModifier( pExperience_->GetCoefReloadingTimeModificator() );
    algorithm.AddModifier( pTiredness_->GetCoefReloadingTimeModificator() );
    algorithm.AddModifier( pStress_->GetCoefReloadingTimeModificator() );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::Execute
// Created: MGD 2009-09-22
// -----------------------------------------------------------------------------
void PHY_RolePion_HumanFactors::Execute( posture::PostureComputer_ABC& algorithm ) const
{
    algorithm.AddCoefficientModifier( pExperience_->GetCoefPostureTimeModificator() );
    algorithm.AddCoefficientModifier( pTiredness_->GetCoefPostureTimeModificator() );
    algorithm.AddCoefficientModifier( pStress_->GetCoefPostureTimeModificator() );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::Execute
// Created: MGD 2009-10-06
// -----------------------------------------------------------------------------
void PHY_RolePion_HumanFactors::Execute( detection::PerceptionDistanceComputer& algorithm ) const
{
    algorithm.AddModifier( pExperience_->GetCoefSensorDistanceModificator() );
    algorithm.AddModifier( pTiredness_->GetCoefSensorDistanceModificator() );
    algorithm.AddModifier( pStress_->GetCoefSensorDistanceModificator() );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_HumanFactors::ModifyPH
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
double PHY_RolePion_HumanFactors::ModifyPH( double rPH ) const
{
    return rPH * pExperience_->GetCoefPhModificator() * pTiredness_->GetCoefPhModificator() * pStress_->GetCoefPhModificator();
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
