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
#include "MIL_Injury_Wound.h"
#include "PHY_InjuredHuman.h"
#include "MIL_Random.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"

//BOOST_CLASS_EXPORT_IMPLEMENT( MIL_Injury_Wound )

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Wound::constructor
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
MIL_Injury_Wound::MIL_Injury_Wound()
    : injuryID_      ( 0 )
    , injuryCategory_( MIL_MedicalTreatmentType::eNone )
    , lifeExpectancy_( SetLifeExpectancy() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Wound::constructor
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
MIL_Injury_Wound::MIL_Injury_Wound( unsigned int injuryID )
    : injuryID_      ( injuryID )
    , injuryCategory_( MIL_MedicalTreatmentType::eNone )
    , lifeExpectancy_( SetLifeExpectancy() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Wound::copy constructor
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
MIL_Injury_Wound::MIL_Injury_Wound( const MIL_Injury_Wound& rhs )
    : injuryID_        ( rhs.injuryID_ )
    , injuryCategory_  ( rhs.injuryCategory_ )
    , lifeExpectancy_  ( rhs.lifeExpectancy_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Wound::destructor
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
MIL_Injury_Wound::~MIL_Injury_Wound()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::load
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
void MIL_Injury_Wound::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> injuryID_
         >> injuryCategory_
         >> lifeExpectancy_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Fire::save
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
void MIL_Injury_Wound::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << injuryID_
         << injuryCategory_
         << lifeExpectancy_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Wound::GetInjuryID
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
unsigned int MIL_Injury_Wound::GetInjuryID() const
{
    return injuryID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Wound::GetInjuryID
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
MIL_MedicalTreatmentType::E_InjuryCategories MIL_Injury_Wound::GetInjuryCategory() const
{
    return injuryCategory_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Wound::GetLifeExpectancy
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
float MIL_Injury_Wound::GetLifeExpectancy() const
{
    return lifeExpectancy_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Wound::GetAgentDose
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
float MIL_Injury_Wound::GetAgentDose() const
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Wound::SetExpectancyLife
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
float MIL_Injury_Wound::SetLifeExpectancy() const
{
    return MIL_MedicalTreatmentType::Find( injuryID_ )->GetLifeExpectancy( injuryCategory_ ) * ( 1.f + 0.1f * static_cast< float >( MIL_Random::rand_ii( -1 , 1 ) ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Wound::SetInjuryCategory
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
void MIL_Injury_Wound::SetInjuryCategory( MIL_MedicalTreatmentType::E_InjuryCategories injuryCategory )
{
    injuryCategory_ = injuryCategory;
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Wound::CanInjuryBeDeadly
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
bool  MIL_Injury_Wound::CanInjuryBeDeadly()
{
    return MIL_MedicalTreatmentType::Find( injuryID_ )->GetLifeExpectancy( injuryCategory_ ) > 0 ;
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Wound::UpdateLifeExpectancy
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
void MIL_Injury_Wound::UpdateLifeExpectancy( float time )
{
    lifeExpectancy_ -= time;
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Wound::UpdateInjuryCategory
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
void MIL_Injury_Wound::UpdateInjuryCategory()
{
    if( injuryCategory_ == MIL_MedicalTreatmentType::eUR && lifeExpectancy_ <= MIL_MedicalTreatmentType::Find(injuryID_ )->GetLifeExpectancy( injuryCategory_ ) )
        SetInjuryCategory( MIL_MedicalTreatmentType::eUA );
    if( lifeExpectancy_ < 0 && CanInjuryBeDeadly() )
        SetInjuryCategory( MIL_MedicalTreatmentType::eDead );
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Wound::IsInjured
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
bool MIL_Injury_Wound::IsInjured( const PHY_ComposantePion& /*pComposante*/ )
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Wound::SetInjury
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
void MIL_Injury_Wound::SetInjury( unsigned int /*nNbrAliveHumans*/ , double /*rDensity*/ )
{
    // NOTHING YET!!
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_Wound::Injure
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
void MIL_Injury_Wound::Injure( PHY_InjuredHuman& injuredHuman )
{
    if( ! injuredHuman.FindInjury( injuryID_ ) )
    {
        // NOTHING YET!!!!
    }
}
