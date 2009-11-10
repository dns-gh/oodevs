// *****************************************************************************
//
// $Created: RFT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Humans/PHY_HumanPopulation.cpp $
// $Author: RFT $
// $Modtime: 29/04/05 11:15 $
// $Revision: 13 $
// $Workfile: PHY_HumanPopulation.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL.h"
#include "MIL_Injury_Poison.h"

#include "PHY_InjuredHuman.h"

#include "MT_Tools/MT_Random.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"

#include "Entities/Objects/NBCTypeAttribute.h"
#include "Entities/Objects/MIL_MedicalTreatmentType.h"

#include "Entities/Agents/Units/Humans/PHY_HumanProtection.h"

MT_Random MIL_Injury_Poison::randomGenerator_;

//BOOST_CLASS_EXPORT_GUID( MIL_Injury_Poison, "MIL_Injury_Poison" )

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Poison::constructor
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
MIL_Injury_Poison::MIL_Injury_Poison()
    : agentConcentration_( 0 )
    , injuryID_          ( 0 )
    , NBCAgent_          ( 0 )
    , injuryCategory_    ( MIL_MedicalTreatmentType::eNone )
    , lifeExpectancy_    ( 0 )
{
    lifeExpectancy_ = SetLifeExpectancy();
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Poison::constructor
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
MIL_Injury_Poison::MIL_Injury_Poison( int agentConcentration , const std::string& NBCAgent , int injuryID )
    : agentConcentration_( agentConcentration )
    , injuryID_          ( injuryID )
    , NBCAgent_          ( NBCAgent )
    , injuryCategory_    ( MIL_MedicalTreatmentType::eNone )
    , lifeExpectancy_    ( 0 )
{
    lifeExpectancy_ = SetLifeExpectancy();
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Poison::copy constructor
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
MIL_Injury_Poison::MIL_Injury_Poison( const MIL_Injury_Poison& rhs )
    : agentConcentration_( rhs.agentConcentration_ )
    , injuryID_          ( rhs.injuryID_ )
    , NBCAgent_          ( rhs.NBCAgent_ )
    , injuryCategory_    ( rhs.injuryCategory_ )
    , lifeExpectancy_    ( rhs.lifeExpectancy_ )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Poison::destructor
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
MIL_Injury_Poison::~MIL_Injury_Poison( )

{
    //NOTHING
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_Human::load
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
void MIL_Injury_Poison::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> agentConcentration_
         >> injuryID_
         >> const_cast < std::string &>( NBCAgent_ )
         >> injuryCategory_
         >> lifeExpectancy_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Fire::save
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
void MIL_Injury_Poison::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << agentConcentration_
         << injuryID_
         << NBCAgent_
         << injuryCategory_
         << lifeExpectancy_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Poison::GetInjuryID
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
int MIL_Injury_Poison::GetInjuryID() const
{
    return injuryID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Poison::GetInjuryID
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
MIL_MedicalTreatmentType::E_InjuryCategories MIL_Injury_Poison::GetInjuryCategory() const
{
    return injuryCategory_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Poison::GetLifeExpectancy
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
float MIL_Injury_Poison::GetLifeExpectancy() const
{
    return lifeExpectancy_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Poison::GetAgentDose
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
float MIL_Injury_Poison::GetAgentDose() const
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Poison::SetExpectancyLife
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
float MIL_Injury_Poison::SetLifeExpectancy() const
{
    return MIL_MedicalTreatmentType::Find( injuryID_ )->GetLifeExpectancy( injuryCategory_ )*( 1 + 0.1*randomGenerator_.rand_ii( -1 , 1 ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Poison::SetInjuryCategory
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
void MIL_Injury_Poison::SetInjuryCategory( MIL_MedicalTreatmentType::E_InjuryCategories injuryCategory )
{
    injuryCategory_ = injuryCategory;
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Poison::CanInjuryBeDeadly
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
bool  MIL_Injury_Poison::CanInjuryBeDeadly()
{
    return MIL_MedicalTreatmentType::Find( injuryID_ )->GetLifeExpectancy( injuryCategory_ ) > 0 ;
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Poison::UpdateLifeExpectancy
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
void MIL_Injury_Poison::UpdateLifeExpectancy( float time )
{
    lifeExpectancy_ -= time;
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Poison::UpdateInjuryCategory
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
void MIL_Injury_Poison::UpdateInjuryCategory()
{
    if( injuryCategory_ == MIL_MedicalTreatmentType::eUR && lifeExpectancy_ <= MIL_MedicalTreatmentType::Find(injuryID_ )->GetLifeExpectancy( injuryCategory_ ) )
        SetInjuryCategory( MIL_MedicalTreatmentType::eUA );
    if( lifeExpectancy_ < 0 && CanInjuryBeDeadly() )
        SetInjuryCategory( MIL_MedicalTreatmentType::eDead );
}

// =============================================================================
// FUNCTOR
// =============================================================================
namespace
{
    struct PHY_PoisonProtectionFunctor
    {
        PHY_PoisonProtectionFunctor( int agentConcentration , const std::string NBCAgent , int injuryID )
            : agentConcentration_   ( agentConcentration )
            , injuryID_             ( injuryID )
            , NBCAgent_             ( NBCAgent )
            , protectionValue_      ( 0 )
        {
            //NOTHING
        }
        
        void operator() ( const PHY_ComposantePion& composantePion )
        {
            composantePion.ApplyOnHumanProtection( *this );
        }

        void operator()( const PHY_ComposantePion& composantePion, const PHY_HumanProtection& humanProtection )
        {
            protectionValue_ += humanProtection.ComputeProtectionValue( injuryID_ , agentConcentration_ , NBCAgent_ );
        }

        float GetProtectionValue()
        {
            return protectionValue_;
        }

        int                 agentConcentration_;
        int                 injuryID_;
        const std::string   NBCAgent_;
        float               protectionValue_;
    };
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Poison::SetInjury
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
bool MIL_Injury_Poison::IsInjured( const PHY_ComposantePion& pComposante )
{
    //Ne pas oublier de prendre en compte la protection avec:
    //pComposante.GetType().GetProtection().Get...
    //Ne pas oublier de prendre en compte le temps d'exposition
    PHY_PoisonProtectionFunctor protection( agentConcentration_ , NBCAgent_ , injuryID_ );
    protection( pComposante );
    unsigned int injuryThreshold = ( unsigned int )( ( 1 + 0.2*randomGenerator_.rand_ii( 0 , 1 ) ) * agentConcentration_ * ( 1 - protection.GetProtectionValue() ) );
    if( injuryThreshold > MIL_MedicalTreatmentType::Find( injuryID_ )->GetDeathThreshold() )
    {
        PHY_InjuredHuman::InitializeInjuredHuman( *this , pComposante );
        injuryCategory_ = MIL_MedicalTreatmentType::eDead;
        return true;
    }
    else if( injuryThreshold > MIL_MedicalTreatmentType::Find( injuryID_ )->GetInjuryThreshold( MIL_MedicalTreatmentType::eUA ) )
    {
        PHY_InjuredHuman::InitializeInjuredHuman( *this , pComposante );
        injuryCategory_ = MIL_MedicalTreatmentType::eUA;
        return true;
    }
    else if( injuryThreshold > MIL_MedicalTreatmentType::Find( injuryID_ )->GetInjuryThreshold( MIL_MedicalTreatmentType::eUR ) )
    {
        PHY_InjuredHuman::InitializeInjuredHuman( *this , pComposante );
        injuryCategory_ = MIL_MedicalTreatmentType::eUR;
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Poison::SetInjury
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
void MIL_Injury_Poison::SetInjury( MT_Float rNbrAliveHumans , MT_Float rDensity )
{
    //Population doesn't have any protection
    //Ne pas oublier de prendre en compte le temps d'exposition

    //First we compute the number of persons caught
    const unsigned int nNbrOfPossibleCasualties = ( unsigned int )std::min( rNbrAliveHumans, std::max( 1., rDensity * MIL_NBCType::GetLength() * MIL_NBCType::GetWidth() ) );
    
    //For, each of them, we will compute if they're going to be injured, and how
    for( unsigned int i = 0; i < nNbrOfPossibleCasualties; ++i )
    {
        unsigned int injuryThreshold = ( unsigned int )( ( 1 + 0.2*randomGenerator_.rand_ii( 0 , 1 ) )*agentConcentration_ );
        if( injuryThreshold > MIL_MedicalTreatmentType::Find( injuryID_ )->GetDeathThreshold() )
        {
            PHY_InjuredHuman::InitializeInjuredHuman( *this );
            injuryCategory_ = MIL_MedicalTreatmentType::eDead;
        }
        else if( injuryThreshold > MIL_MedicalTreatmentType::Find( injuryID_ )->GetInjuryThreshold( MIL_MedicalTreatmentType::eUA ) )
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
// Name: MIL_Injury_Poison::Injure
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
void MIL_Injury_Poison::Injure( PHY_InjuredHuman& injuredHuman )
{
    unsigned int injuryThreshold = ( unsigned int )( ( 1 + 0.2*randomGenerator_.rand_ii( 0 , 1 ) ) * agentConcentration_ );
    //If injuredHuman has a protection, we compute its protection effect
    if( injuredHuman.GetComposantePion() != 0 )
    {
        PHY_PoisonProtectionFunctor protection( agentConcentration_ , NBCAgent_ , injuryID_ );
        protection( *injuredHuman.GetComposantePion() );
        injuryThreshold *= ( 1 - protection.GetProtectionValue() );
    }
    
    //If injuredHuman isn't already injured by this kind of injury, add possibly an injury of this kind
    if( ! injuredHuman.FindInjury( injuryID_ ) )
    {
        if( injuryThreshold > MIL_MedicalTreatmentType::Find( injuryID_ )->GetDeathThreshold() )
        {
            injuredHuman.AddInjury( *this );
            injuryCategory_ = MIL_MedicalTreatmentType::eDead;
        }
        else if( injuryThreshold > MIL_MedicalTreatmentType::Find( injuryID_ )->GetInjuryThreshold( MIL_MedicalTreatmentType::eUA ) )
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
        if( injuryThreshold > MIL_MedicalTreatmentType::Find( injuryID_ )->GetDeathThreshold() )
        {
            injuredHuman.AddInjury( *this );
            injuryCategory_ = MIL_MedicalTreatmentType::eDead;
        }
        else if( injuryThreshold > MIL_MedicalTreatmentType::Find( injuryID_ )->GetInjuryThreshold( MIL_MedicalTreatmentType::eUA ) && ( injuredHuman.GetInjuryCategory( injuryID_ ) != MIL_MedicalTreatmentType::eUR ))
        {
            injuredHuman.AddInjury( *this );
            injuryCategory_ = MIL_MedicalTreatmentType::eUA;
        }
    }
}