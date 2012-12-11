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
#include "PHY_InjuredHuman.h"
#include "MIL_Injury_ABC.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Objects/MedicalTreatmentAttribute.h"
#include "MIL_Time_ABC.h"

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_InjuredHuman )

// -----------------------------------------------------------------------------
// Name: PHY_InjuredHuman constructor
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
PHY_InjuredHuman::PHY_InjuredHuman()
    : lifeExpectancy_( 0 )
    , injuryCategory_( MIL_MedicalTreatmentType::eNone )
    , injuryID_( 0 )
    , isTreated_( false )
    , pComposantePion_( 0 )
    , timeOfLastUpdate_( static_cast< float >( MIL_Time_ABC::GetTime().GetCurrentTimeStep() ) )
    , treatment_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_InjuredHuman copy constructor
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
PHY_InjuredHuman::PHY_InjuredHuman( const PHY_InjuredHuman& rhs )
    : lifeExpectancy_( rhs.lifeExpectancy_ )
    , injuryCategory_( rhs.injuryCategory_ )
    , injuryID_( rhs.injuryID_ )
    , isTreated_( rhs.isTreated_ )
    , injuriesList_( rhs.injuriesList_ )
    , pComposantePion_( rhs.pComposantePion_ )
    , timeOfLastUpdate_( static_cast< float >( MIL_Time_ABC::GetTime().GetCurrentTimeStep() ) )
    , treatment_( rhs.treatment_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_InjuredHuman constructor
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
PHY_InjuredHuman::PHY_InjuredHuman( MIL_Injury_ABC& injury , const PHY_ComposantePion& composantePion )
    : lifeExpectancy_( injury.SetLifeExpectancy() )
    , injuryCategory_( injury.GetInjuryCategory() )
    , injuryID_( injury.GetInjuryID() )
    , isTreated_( false )
    , pComposantePion_( &composantePion )
    , timeOfLastUpdate_( static_cast< float >( MIL_Time_ABC::GetTime().GetCurrentTimeStep() ) )
    , treatment_( 0 )
{
    AddInjury( injury );
}

// -----------------------------------------------------------------------------
// Name: PHY_InjuredHuman constructor
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
PHY_InjuredHuman::PHY_InjuredHuman( MIL_Injury_ABC& injury )
    : lifeExpectancy_( injury.SetLifeExpectancy() )
    , injuryCategory_( injury.GetInjuryCategory() )
    , injuryID_( injury.GetInjuryID() )
    , isTreated_( false )
    , pComposantePion_( 0 )
    , timeOfLastUpdate_( static_cast< float >( MIL_Time_ABC::GetTime().GetCurrentTimeStep() ) )
    , treatment_( 0 )
{
    AddInjury( injury );
}

// -----------------------------------------------------------------------------
// Name: PHY_InjuredHuman destructor
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
PHY_InjuredHuman::~PHY_InjuredHuman()
{
    /* UnregisterPatient() ? */
    if( treatment_ )
        treatment_->RegisterPatients( injuryID_, injuryCategory_, -1 );
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::load
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
void PHY_InjuredHuman::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> lifeExpectancy_
         >> injuryCategory_
         >> injuryID_
         >> treatment_;
    file >> const_cast< PHY_ComposantePion*& >( pComposantePion_ )
         >> timeOfLastUpdate_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::save
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
void PHY_InjuredHuman::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << lifeExpectancy_
         << injuryCategory_
         << injuryID_
         << treatment_
         << pComposantePion_;
    // $$$$ LDC TODO: Serialise injuries, change MIL_Injury_ABC and subclasses so they are serialised correctly.
//         << injuriesList_.size();
//    for( auto it = injuriesList_.begin() ; it != injuriesList_.end() ; ++it )
//         file << (*it);
    file << timeOfLastUpdate_;
}

// -----------------------------------------------------------------------------
// Name: PHY_InjuredHuman InitializeInjuredHuman
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
void PHY_InjuredHuman::InitializeInjuredHuman( MIL_Injury_ABC& injury, const PHY_ComposantePion& composantePion )
{
    new PHY_InjuredHuman( injury, composantePion );
}

// -----------------------------------------------------------------------------
// Name: PHY_InjuredHuman InitializeInjuredHuman
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
void PHY_InjuredHuman::InitializeInjuredHuman( MIL_Injury_ABC& injury )
{
    new PHY_InjuredHuman( injury );
}

// -----------------------------------------------------------------------------
// Name: PHY_InjuredHuman GetLifeExpectancy
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
int PHY_InjuredHuman::GetLifeExpectancy() const
{
    return static_cast< int >( lifeExpectancy_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_InjuredHuman GetInjuryCategory
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
MIL_MedicalTreatmentType::E_InjuryCategories PHY_InjuredHuman::GetInjuryCategory() const
{
    return injuryCategory_;
}

// -----------------------------------------------------------------------------
// Name: PHY_InjuredHuman GetInjuryCategory
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
MIL_MedicalTreatmentType::E_InjuryCategories PHY_InjuredHuman::GetInjuryCategory( unsigned int injuryID ) const
{
    for( auto it = injuriesList_.begin() ; it != injuriesList_.end() ; ++it )
        if( ( *it )->GetInjuryID() == injuryID )
            return ( *it )->GetInjuryCategory();
    return MIL_MedicalTreatmentType::eNone;
}

// -----------------------------------------------------------------------------
// Name: PHY_InjuredHuman GetInjuryID
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
unsigned int PHY_InjuredHuman::GetInjuryID() const
{
    return injuryID_;
}

// -----------------------------------------------------------------------------
// Name: PHY_InjuredHuman GetInjuryID
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
const PHY_ComposantePion* PHY_InjuredHuman::GetComposantePion() const
{
    return pComposantePion_;
}

// -----------------------------------------------------------------------------
// Name: PHY_InjuredHuman Update
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
void PHY_InjuredHuman::Update()
{
    //If the Injured Human isn't dead, update its parameters
    if( IsAlive() && !treatment_ )
    {
        const float currentTime = static_cast< float >( MIL_Time_ABC::GetTime().GetCurrentTimeStep() );

        //Update the characteristics of all injuries
        UpdateInjuriesInfo( currentTime );

        //Update the characteristics of the main injury (the one which will cause death first)
        UpdateInjuredHumanInfo( currentTime );

        //Record the time of the last update
        timeOfLastUpdate_ = currentTime;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_InjuredHuman UpdateInjuriesInfo
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
void PHY_InjuredHuman::UpdateInjuriesInfo( float currentTime )
{
    for( IT_InjuriesList it = injuriesList_.begin() ; it != injuriesList_.end() ; ++it )
    {
        ( *it )->UpdateLifeExpectancy( timeOfLastUpdate_ - currentTime );
        ( *it )->UpdateInjuryCategory();
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_InjuredHuman UpdateMainInjury
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
void PHY_InjuredHuman::UpdateInjuredHumanInfo( float currentTime )
{
    float lifeExpectancy = -1;
    CIT_InjuriesList iter;
    for( auto it = injuriesList_.begin() ; it != injuriesList_.end() ; ++it )
    {
        if( lifeExpectancy != -1 && ( *it )->CanInjuryBeDeadly() )
        {
            lifeExpectancy  = std::min( ( *it )->GetLifeExpectancy(), lifeExpectancy );
            iter = it;
        }
        else if( ( *it )->CanInjuryBeDeadly() )
        {
            lifeExpectancy = ( *it )->GetLifeExpectancy();
            iter = it;
        }
    }
    if( lifeExpectancy < lifeExpectancy_ - currentTime + timeOfLastUpdate_ )
    {
        lifeExpectancy_ = lifeExpectancy;
        injuryCategory_ = ( *iter )->GetInjuryCategory();
        injuryID_ = ( *iter )->GetInjuryID();
    }
    else
        lifeExpectancy_ = lifeExpectancy_ - currentTime + timeOfLastUpdate_;
}

// -----------------------------------------------------------------------------
// Name: PHY_InjuredHuman IsAlive
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
bool PHY_InjuredHuman::IsAlive() const
{
    for( auto it = injuriesList_.begin() ; it != injuriesList_.end() ; ++it )
        if( ( *it )->GetInjuryCategory() == MIL_MedicalTreatmentType::eDead )
            return false;
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_InjuredHuman IsAlive
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
void PHY_InjuredHuman::AddInjury( MIL_Injury_ABC& injury )
{
    injuriesList_.push_back( &injury );
}

// -----------------------------------------------------------------------------
// Name: PHY_InjuredHuman TreatInjuredHuman
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
void PHY_InjuredHuman::TreatInjuredHuman( MedicalTreatmentAttribute& attr )
{
    // This function is used to record an injured human is receiving his medical treatment in a medical structure
    treatment_ = &attr;
    attr.RegisterPatients( injuryID_, injuryCategory_, 1 );
}

// -----------------------------------------------------------------------------
// Name: PHY_InjuredHuman FindInjury
// Created: RFT 24/07/2008
// -----------------------------------------------------------------------------
bool PHY_InjuredHuman::FindInjury( unsigned int injuryID )
{
    for( auto it = injuriesList_.begin() ; it != injuriesList_.end() ; ++it )
        if( ( *it )->GetInjuryID() == injuryID )
            return true;
    return false;
}
