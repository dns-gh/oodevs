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
#include "MIL_Injury_NBC.h"

#include "PHY_InjuredHuman.h"

#include "MT_Tools/MT_Random.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"

#include "Entities/Objects/NBCAttribute.h"
#include "Entities/Objects/MIL_MedicalTreatmentType.h"

MT_Random MIL_Injury_NBC::randomGenerator_;

//BOOST_CLASS_EXPORT_GUID( MIL_Injury_NBC, "MIL_Injury_NBC" )

// -----------------------------------------------------------------------------
// Name: MIL_Injury_NBC::constructor
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
MIL_Injury_NBC::MIL_Injury_NBC()
    : pInjuryAttribute_( 0 )
    , injuryID_        ( 0 )
    , injuryCategory_  ( MIL_MedicalTreatmentType::eNone )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_NBC::constructor
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
MIL_Injury_NBC::MIL_Injury_NBC( NBCTypeAttribute& attr , int injuryID )
    : pInjuryAttribute_( &attr )
    , injuryID_        ( injuryID )
    , injuryCategory_  ( MIL_MedicalTreatmentType::eNone )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_NBC::copy constructor
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
MIL_Injury_NBC::MIL_Injury_NBC( const MIL_Injury_NBC& rhs )
    : pInjuryAttribute_( rhs.pInjuryAttribute_ )
    , injuryID_        ( rhs.injuryID_ )
    , injuryCategory_  ( rhs.injuryCategory_ )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_NBC::destructor
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
MIL_Injury_NBC::~MIL_Injury_NBC( )

{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_NBC::GetInjuryID
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
int MIL_Injury_NBC::GetInjuryID() const
{
    return injuryID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_NBC::GetInjuryID
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
MIL_MedicalTreatmentType::E_InjuryCategories MIL_Injury_NBC::GetInjuryCategory() const
{
    return injuryCategory_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_NBC::GetLifeExpectancy
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
float MIL_Injury_NBC::GetLifeExpectancy( MIL_MedicalTreatmentType::E_InjuryCategories injuryCategory ) const
{
    return MIL_MedicalTreatmentType::Find( injuryID_ )->GetLifeExpectancy( injuryCategory_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_NBC::SetExpectancyLife
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
float MIL_Injury_NBC::SetLifeExpectancy() const
{
    return MIL_MedicalTreatmentType::Find( injuryID_ )->GetLifeExpectancy( injuryCategory_ )*( 1 + 0.1*randomGenerator_.rand_ii( -1 , 1 ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_NBC::SetInjuryCategory
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
void MIL_Injury_NBC::SetInjuryCategory( MIL_MedicalTreatmentType::E_InjuryCategories injuryCategory )
{
    injuryCategory_ = injuryCategory;
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_NBC::SetInjury
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
bool MIL_Injury_NBC::IsInjured( const PHY_ComposantePion& pComposante )
{/*
    //Ne pas oublier de prendre en compte la protection avec:
    //pComposante.GetType().GetProtection().Get...
    //Ne pas oublier de prendre en compte le temps d'exposition
    unsigned int injuryThreshold = ( unsigned int )( ( 1 + 0.2*randomGenerator_.rand_ii( 0 , 1 ) )*pInjuryAttribute_->GetHeat() );
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
    }*/
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_NBC::SetInjury
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
void MIL_Injury_NBC::SetInjury( MT_Float rNbrAliveHumans , MT_Float rDensity )
{/*
    //Population doesn't have any protection
    //Ne pas oublier de prendre en compte le temps d'exposition

    //First we compute the number of persons caught in the fire
    const unsigned int nNbrOfPossibleCasualties = ( unsigned int )std::min( rNbrAliveHumans, std::max( 1., rDensity * pInjuryAttribute_->GetLength() * pInjuryAttribute_->GetWidth() ) );
    
    //For, each of them, we will compute if they're going to be injured, and how
    for( unsigned int i = 0; i < nNbrOfPossibleCasualties; ++i )
    {
        unsigned int injuryThreshold = ( unsigned int )( ( 1 + 0.2*randomGenerator_.rand_ii( 0 , 1 ) )*pInjuryAttribute_->GetHeat() );
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
    }*/
}