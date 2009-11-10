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
#include "MIL_Injury_Fire.h"

#include "PHY_InjuredHuman.h"

#include "MT_Tools/MT_Random.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"

#include "Entities/Objects/MIL_FireClass.h"
#include "Entities/Objects/MIL_MedicalTreatmentType.h"

#include "Entities/Agents/Units/Humans/PHY_HumanProtection.h"

MT_Random MIL_Injury_Fire::randomGenerator_;

BOOST_CLASS_EXPORT_GUID( MIL_Injury_Fire, "MIL_Injury_Fire" )

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Fire::constructor
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
MIL_Injury_Fire::MIL_Injury_Fire()
    : MIL_Injury_ABC   ()
    , heat_            ( 0 )
    , fireClass_       ( 0 )
    , injuryID_        ( 0 )
    , injuryCategory_  ( MIL_MedicalTreatmentType::eNone )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Fire::constructor
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
MIL_Injury_Fire::MIL_Injury_Fire( int heat , const std::string fireClass_ , int injuryID )
    : MIL_Injury_ABC   ()
    , heat_            ( heat )
    , fireClass_       ( fireClass_ )
    , injuryID_        ( injuryID )
    , injuryCategory_  ( MIL_MedicalTreatmentType::eNone )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Fire::copy constructor
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
MIL_Injury_Fire::MIL_Injury_Fire( const MIL_Injury_Fire& rhs )
    : MIL_Injury_ABC   ()
    , heat_            ( rhs.heat_ )
    , fireClass_       ( rhs.fireClass_ )
    , injuryID_        ( rhs.injuryID_ )
    , injuryCategory_  ( rhs.injuryCategory_ )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Fire::destructor
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
MIL_Injury_Fire::~MIL_Injury_Fire()
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
void MIL_Injury_Fire::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> heat_
         >> const_cast < std::string &>( fireClass_ )
         >> injuryID_
         >> injuryCategory_
         >> lifeExpectancy_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Fire::save
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
void MIL_Injury_Fire::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << heat_
         << fireClass_
         << injuryID_
         << injuryCategory_
         << lifeExpectancy_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Fire::GetInjuryID
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
int MIL_Injury_Fire::GetInjuryID() const
{
    return injuryID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Fire::GetInjuryID
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
MIL_MedicalTreatmentType::E_InjuryCategories MIL_Injury_Fire::GetInjuryCategory() const
{
    return injuryCategory_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Fire::GetLifeExpectancy
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
float MIL_Injury_Fire::GetLifeExpectancy() const
{
    return lifeExpectancy_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Fire::GetAgentDose
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
float MIL_Injury_Fire::GetAgentDose() const
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Fire::CanInjuryBeDeadly
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
bool  MIL_Injury_Fire::CanInjuryBeDeadly()
{
    return MIL_MedicalTreatmentType::Find( injuryID_ )->GetLifeExpectancy( injuryCategory_ ) > 0 ;
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Fire::UpdateLifeExpectancy
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
void MIL_Injury_Fire::UpdateLifeExpectancy( float time )
{
    lifeExpectancy_ -= time;
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Fire::UpdateInjuryCategory
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
void MIL_Injury_Fire::UpdateInjuryCategory()
{
    if( injuryCategory_ == MIL_MedicalTreatmentType::eUR && lifeExpectancy_ <= MIL_MedicalTreatmentType::Find(injuryID_ )->GetLifeExpectancy( injuryCategory_ ) )
        SetInjuryCategory( MIL_MedicalTreatmentType::eUA );
    if( lifeExpectancy_ < 0 && CanInjuryBeDeadly() )
        SetInjuryCategory( MIL_MedicalTreatmentType::eDead );
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Fire::SetExpectancyLife
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
float MIL_Injury_Fire::SetLifeExpectancy() const
{
    return MIL_MedicalTreatmentType::Find( injuryID_ )->GetLifeExpectancy( injuryCategory_ )*( 1 + 0.1*randomGenerator_.rand_ii( -1 , 1 ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Fire::SetInjuryCategory
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
void MIL_Injury_Fire::SetInjuryCategory( MIL_MedicalTreatmentType::E_InjuryCategories injuryCategory )
{
    injuryCategory_ = injuryCategory;
}

// =============================================================================
// FUNCTOR
// =============================================================================
namespace
{
    struct PHY_FireProtectionFunctor
    {
        PHY_FireProtectionFunctor( int heat , const std::string fireClass , int injuryID )
            : heat_           ( heat )
            , injuryID_       ( injuryID )
            , fireClass_      ( fireClass )
            , protectionValue_( 0 )
        {
            //NOTHING
        }
        
        void operator() ( const PHY_ComposantePion& composantePion )
        {
            composantePion.ApplyOnHumanProtection( *this );
        }

        void operator()( const PHY_ComposantePion& composantePion, const PHY_HumanProtection& humanProtection )
        {
            //Protection value is the sum of the effects of all the protections which can protect against a fire of class fireClass_
            protectionValue_ += humanProtection.ComputeProtectionValue( injuryID_ , heat_ , fireClass_ );
        }

        float GetProtectionValue()
        {
            return protectionValue_;
        }

        int                 heat_;
        int                 injuryID_;
        const std::string   fireClass_;
        float               protectionValue_;
    };
}

// =============================================================================
// INJURE ComposantePion, Population and PHY_InjuredHuman
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Fire::SetInjury
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
bool MIL_Injury_Fire::IsInjured( const PHY_ComposantePion& pComposante )
{   
    PHY_FireProtectionFunctor protection( heat_ , fireClass_ , injuryID_ );
    
    // pComposante.ApplyOnHumanProtection( protection );    
    // PHY_InjuredHuman* injured = new PHY_InjuredHuman( *this, pComposante );
    // Injure( injured );

    protection( pComposante );
    //The injury which will be caused is computed by the multiplication of the heat by the protection value and a random factor
    unsigned int injuryThreshold = ( unsigned int )( ( 1 + 0.2 * randomGenerator_.rand_ii( 0 , 1 ) ) * heat_ * ( 1 - protection.GetProtectionValue() ) );
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
// Name: MIL_Injury_Fire::SetInjury
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
void MIL_Injury_Fire::SetInjury( MT_Float rNbrAliveHumans , MT_Float rDensity )
{
    //Population doesn't have any protection
    //First we compute the number of persons caught in the fire
    const unsigned int nNbrOfPossibleCasualties = ( unsigned int )std::min( rNbrAliveHumans, std::max( 1., rDensity * MIL_FireClass::GetLength() * MIL_FireClass::GetWidth() ) );
    
    //For, each of them, we will compute if they're going to be injured, and how
    for( unsigned int i = 0; i < nNbrOfPossibleCasualties; ++i )
    {
        unsigned int injuryThreshold = ( unsigned int )( ( 1 + 0.2 * randomGenerator_.rand_ii( 0 , 1 ) ) * heat_ );
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
// Name: MIL_Injury_Fire::SetInjury
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
void MIL_Injury_Fire::Injure( PHY_InjuredHuman& injuredHuman )
{
    unsigned int injuryThreshold = ( unsigned int )( ( 1 + 0.2 * randomGenerator_.rand_ii( 0 , 1 ) ) * heat_ );  
    //If injuredHuman has a protection, we compute its protection effect
    if( injuredHuman.GetComposantePion() != 0 )
    {
        PHY_FireProtectionFunctor protection( heat_ , fireClass_ , injuryID_ );
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