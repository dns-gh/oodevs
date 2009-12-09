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
#include "MIL_Injury_MentalDisease.h"

#include "PHY_InjuredHuman.h"

#include "MT_Tools/MT_Random.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"

#include "Entities/Objects/FireAttribute.h"
#include "Entities/Objects/MIL_MedicalTreatmentType.h"

#include "Entities/Agents/Units/Humans/PHY_HumanProtection.h"

MT_Random MIL_Injury_MentalDisease::randomGenerator_;

BOOST_CLASS_EXPORT_GUID( MIL_Injury_MentalDisease, "MIL_Injury_MentalDisease" )

// -----------------------------------------------------------------------------
// Name: MIL_Injury_MentalDisease::constructor
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
MIL_Injury_MentalDisease::MIL_Injury_MentalDisease()
    : injuryID_        ( 0 )
    , injuryCategory_  ( MIL_MedicalTreatmentType::eNone )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_MentalDisease::constructor
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
MIL_Injury_MentalDisease::MIL_Injury_MentalDisease( int injuryID )
    : injuryID_        ( injuryID )
    , injuryCategory_  ( MIL_MedicalTreatmentType::eNone )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_MentalDisease::copy constructor
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
MIL_Injury_MentalDisease::MIL_Injury_MentalDisease( const MIL_Injury_MentalDisease& rhs )
    : injuryID_        ( rhs.injuryID_ )
    , injuryCategory_  ( rhs.injuryCategory_ )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_MentalDisease::destructor
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
MIL_Injury_MentalDisease::~MIL_Injury_MentalDisease( )

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
void MIL_Injury_MentalDisease::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> injuryID_
         >> injuryCategory_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_MentalDisease::save
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
void MIL_Injury_MentalDisease::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << injuryID_
         << injuryCategory_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_MentalDisease::GetInjuryID
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
int MIL_Injury_MentalDisease::GetInjuryID() const
{
    return injuryID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_MentalDisease::GetInjuryID
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
MIL_MedicalTreatmentType::E_InjuryCategories MIL_Injury_MentalDisease::GetInjuryCategory() const
{
    return injuryCategory_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_MentalDisease::GetLifeExpectancy
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
float MIL_Injury_MentalDisease::GetLifeExpectancy() const
{
    return -1;
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_MentalDisease::GetAgentDose
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
float MIL_Injury_MentalDisease::GetAgentDose() const
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_MentalDisease::SetExpectancyLife
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
float MIL_Injury_MentalDisease::SetLifeExpectancy() const
{
    return -1;
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_MentalDisease::SetInjuryCategory
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
void MIL_Injury_MentalDisease::SetInjuryCategory( MIL_MedicalTreatmentType::E_InjuryCategories injuryCategory )
{
    injuryCategory_ = injuryCategory;
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_MentalDisease::CanInjuryBeDeadly
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
bool  MIL_Injury_MentalDisease::CanInjuryBeDeadly()
{
    return MIL_MedicalTreatmentType::Find( injuryID_ )->GetLifeExpectancy( injuryCategory_ ) > 0 ;
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_MentalDisease::CanInjuryBeDeadly
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
void MIL_Injury_MentalDisease::UpdateLifeExpectancy( float /*time*/ )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_MentalDisease::UpdateInjuryCategory
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
void MIL_Injury_MentalDisease::UpdateInjuryCategory()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_MentalDisease::SetInjury
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
bool MIL_Injury_MentalDisease::IsInjured( const PHY_ComposantePion& /*pComposante*/ )
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_MentalDisease::SetInjury
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
void MIL_Injury_MentalDisease::SetInjury( MT_Float /*rNbrAliveHumans*/ , MT_Float /*rDensity*/ )
{
    
}

// -----------------------------------------------------------------------------
// Name: MIL_Injury_MentalDisease::Injure
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
void MIL_Injury_MentalDisease::Injure( PHY_InjuredHuman& injuredHuman )
{
    if( ! injuredHuman.FindInjury( injuryID_ ) )
    {
    }
}