// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_Injury_Contamination.h"
#include "MIL_Random.h"
#include "PHY_InjuredHuman.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Objects/MIL_NBCType.h"
#include "Entities/Agents/Units/Humans/PHY_HumanProtection.h"

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_Injury_Contamination )

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Contamination::constructor
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
MIL_Injury_Contamination::MIL_Injury_Contamination()
    : agentConcentration_( 0 )
    , NBCAgent_          ( 0 )
    , injuryID_          ( 0 )
    , injuryCategory_    ( MIL_MedicalTreatmentType::eNone )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Contamination::constructor
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
MIL_Injury_Contamination::MIL_Injury_Contamination( int agentConcentration, const std::string& NBCAgent, unsigned int injuryID )
    : agentConcentration_( agentConcentration )
    , NBCAgent_          ( NBCAgent )
    , injuryID_          ( injuryID )
    , injuryCategory_    ( MIL_MedicalTreatmentType::eNone )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Contamination::copy constructor
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
MIL_Injury_Contamination::MIL_Injury_Contamination( const MIL_Injury_Contamination& rhs )
    : agentConcentration_( rhs.agentConcentration_ )
    , NBCAgent_          ( rhs.NBCAgent_ )
    , injuryID_          ( rhs.injuryID_ )
    , injuryCategory_    ( rhs.injuryCategory_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Contamination::destructor
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
MIL_Injury_Contamination::~MIL_Injury_Contamination( )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::load
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
void MIL_Injury_Contamination::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> agentConcentration_
         >> const_cast< std::string& >( NBCAgent_ )
         >> injuryID_
         >> injuryCategory_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Fire::save
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
void MIL_Injury_Contamination::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << agentConcentration_
         << NBCAgent_
         << injuryID_
         << injuryCategory_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Contamination::GetInjuryID
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
unsigned int MIL_Injury_Contamination::GetInjuryID() const
{
    return injuryID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Contamination::GetInjuryID
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
MIL_MedicalTreatmentType::E_InjuryCategories MIL_Injury_Contamination::GetInjuryCategory() const
{
    return injuryCategory_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Contamination::GetLifeExpectancy
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
float MIL_Injury_Contamination::GetLifeExpectancy() const
{
    return -1;
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Contamination::GetAgentDose
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
float MIL_Injury_Contamination::GetAgentDose() const
{
    return agentDose_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Contamination::SetExpectancyLife
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
float MIL_Injury_Contamination::SetLifeExpectancy() const
{
    return -1;
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Contamination::SetInjuryCategory
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
void MIL_Injury_Contamination::SetInjuryCategory( MIL_MedicalTreatmentType::E_InjuryCategories injuryCategory )
{
    injuryCategory_ = injuryCategory;
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Contamination::CanInjuryBeDeadly
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
bool  MIL_Injury_Contamination::CanInjuryBeDeadly()
{
    return MIL_MedicalTreatmentType::Find( injuryID_ )->GetLifeExpectancy( injuryCategory_ ) > 0 ;
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Contamination::CanInjuryBeDeadly
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
void MIL_Injury_Contamination::UpdateLifeExpectancy( float /*time*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Contamination::UpdateInjuryCategory
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
void MIL_Injury_Contamination::UpdateInjuryCategory()
{
    // NOTHING
}

namespace
{
    //Be careful: do not confuse contamination and poisoning!
    struct PHY_ContaminationProtectionFunctor
    {
        PHY_ContaminationProtectionFunctor( int agentConcentration, const std::string& NBCAgent, unsigned int injuryID )
            : agentConcentration_( agentConcentration )
            , injuryID_          ( injuryID )
            , NBCAgent_          ( NBCAgent )
            , protectionValue_   ( 0 )
        {}
        void operator()( const PHY_ComposantePion& composantePion )
        {
            composantePion.ApplyOnHumanProtection( *this );
        }
        void operator()( const PHY_ComposantePion& /*composantePion*/, const PHY_HumanProtection& humanProtection )
        {
            protectionValue_ += humanProtection.ComputeProtectionValue( injuryID_, agentConcentration_, NBCAgent_ );
        }
        float GetProtectionValue()
        {
            return protectionValue_;
        }
        int agentConcentration_;
        unsigned int injuryID_;
        std::string NBCAgent_;
        float protectionValue_;
    };
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Contamination::IsInjured
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
bool MIL_Injury_Contamination::IsInjured( const PHY_ComposantePion& pComposante )
{
    //Ne pas oublier de prendre en compte la protection avec:
    //pComposante.GetType().GetProtection().Get...
    PHY_ContaminationProtectionFunctor protection( agentConcentration_, NBCAgent_, injuryID_ );
    protection( pComposante );
    double injuryThreshold = ( 1 + 0.2 * MIL_Random::rand_ii( 0, 1, MIL_Random::eWounds ) ) * agentConcentration_ * ( 1 - protection.GetProtectionValue() );
    if( injuryThreshold > MIL_MedicalTreatmentType::Find( injuryID_ )->GetInjuryThreshold( MIL_MedicalTreatmentType::eUA ) )
    {
        PHY_InjuredHuman::InitializeInjuredHuman( *this , pComposante );
        injuryCategory_ = MIL_MedicalTreatmentType::eUA;
        return true;
    }
    else if( injuryThreshold > MIL_MedicalTreatmentType::Find( injuryID_ )->GetInjuryThreshold( MIL_MedicalTreatmentType::eUR ) )
    {
        PHY_InjuredHuman::InitializeInjuredHuman( *this, pComposante );
        injuryCategory_ = MIL_MedicalTreatmentType::eUR;
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Contamination::SetInjury
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
void MIL_Injury_Contamination::SetInjury( unsigned int nNbrAliveHumans , double rDensity )
{
    //Population doesn't have any protection

    //First we compute the number of persons caught in the fire
    const unsigned int nNbrOfPossibleCasualties = std::min( nNbrAliveHumans, std::max( static_cast< unsigned int >( 1 ), static_cast< unsigned int >( rDensity * MIL_NBCType::GetLength() * MIL_NBCType::GetWidth() ) ) );

    //For, each of them, we will compute if they're going to be injured, and how
    for( unsigned int i = 0; i < nNbrOfPossibleCasualties; ++i )
    {
        double injuryThreshold = ( 1 + 0.2 * MIL_Random::rand_ii( 0, 1, MIL_Random::eWounds ) ) * agentConcentration_;
        if( injuryThreshold > MIL_MedicalTreatmentType::Find( injuryID_ )->GetInjuryThreshold( MIL_MedicalTreatmentType::eUA ) )
        {
            PHY_InjuredHuman::InitializeInjuredHuman( *this );
            injuryCategory_ = MIL_MedicalTreatmentType::eUA;
        }
        else if( injuryThreshold > MIL_MedicalTreatmentType::Find( injuryID_ )->GetInjuryThreshold( MIL_MedicalTreatmentType::eUR ) )
        {
            PHY_InjuredHuman::InitializeInjuredHuman( *this );
            injuryCategory_ = MIL_MedicalTreatmentType::eUR;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Contamination::Injure
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
void MIL_Injury_Contamination::Injure( PHY_InjuredHuman& injuredHuman )
{
    double injuryThreshold = ( 1 + 0.2 * MIL_Random::rand_ii( 0, 1, MIL_Random::eWounds ) ) * agentConcentration_;
    //If injuredHuman has a protection, we compute its protection effect
    if( injuredHuman.GetComposantePion() != 0 )
    {
        PHY_ContaminationProtectionFunctor protection( agentConcentration_, NBCAgent_, injuryID_ );
        protection( *injuredHuman.GetComposantePion() );
        injuryThreshold *= ( 1 - protection.GetProtectionValue() );
    }
    //If injuredHuman isn't already injured by this kind of injury, add possibly an injury of this kind
    if( !injuredHuman.FindInjury( injuryID_ ) )
    {
        if( injuryThreshold > MIL_MedicalTreatmentType::Find( injuryID_ )->GetInjuryThreshold( MIL_MedicalTreatmentType::eUA ) )
        {
            injuredHuman.AddInjury( *this );
            injuryCategory_ = MIL_MedicalTreatmentType::eUA;
        }
        else if( injuryThreshold > MIL_MedicalTreatmentType::Find( injuryID_ )->GetInjuryThreshold( MIL_MedicalTreatmentType::eUR ) )
        {
            injuredHuman.AddInjury( *this );
            injuryCategory_ = MIL_MedicalTreatmentType::eUR;
        }
    }
    //Check if the injury isn't worse than the one which already exists. If so, modified it! (c est d'un cynisme!)
    else
    {
        if( injuryThreshold > MIL_MedicalTreatmentType::Find( injuryID_ )->GetInjuryThreshold( MIL_MedicalTreatmentType::eUA ) && ( injuredHuman.GetInjuryCategory( injuryID_ ) != MIL_MedicalTreatmentType::eUR ))
        {
            injuredHuman.AddInjury( *this );
            injuryCategory_ = MIL_MedicalTreatmentType::eUA;
        }
    }
}
