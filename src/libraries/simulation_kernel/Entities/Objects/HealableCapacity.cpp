// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "HealableCapacity.h"
#include "MedicalTreatmentAttribute.h"
#include "Entities/Agents/Units/Humans/PHY_Human.h"
#include "Object.h"

#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_GUID( HealableCapacity, "HealableCapacity" )

// -----------------------------------------------------------------------------
// Name: HealableCapacity::HealableCapacity
// Created: RFT 2008-05-22
// -----------------------------------------------------------------------------
HealableCapacity::HealableCapacity( xml::xistream& xis )
    : bEmergencyPlan_        ( false )
    , bBusinessHours_        ( true )//A determiner en fonction de l'heure de la sim
    , emergencyBedsRate_     ( 0 )
    , emergencyDoctorsRate_  ( 0 )
    , nightDoctorsRate_      ( 0 )
    , careWaitingList_       ()
    , transferWaitingList_   ()
    , entranceWaitingList_   ()
{
    InitializeData( xis );
}

// -----------------------------------------------------------------------------
// Name: HealableCapacity::HealableCapacity
// Created: RFT 2008-05-22
// -----------------------------------------------------------------------------
HealableCapacity::HealableCapacity()
    : bEmergencyPlan_        ( false )
    , bBusinessHours_        ( true )
    , emergencyBedsRate_     ( 0 )
    , emergencyDoctorsRate_  ( 0 )
    , nightDoctorsRate_      ( 0 )
    , careWaitingList_       ()
    , transferWaitingList_   ()
    , entranceWaitingList_   ()
{
    //NOTHING
}


// -----------------------------------------------------------------------------
// Name: HealableCapacity constructor
// Created: RFT 2008-05-22
// -----------------------------------------------------------------------------
HealableCapacity::HealableCapacity( const HealableCapacity& from )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HealableCapacity destructor
// Created: RFT 2008-05-22
// -----------------------------------------------------------------------------
HealableCapacity::~HealableCapacity()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: MobilityCapacity::InitializeSpeed
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
void HealableCapacity::InitializeData( xml::xistream& xis )
{
    xis >> xml::attribute( "emergency-plan-beds", emergencyBedsRate_ );
    xis >> xml::attribute( "emergency-plan-doctors", emergencyDoctorsRate_ );
    xis >> xml::attribute( "night-doctors", nightDoctorsRate_ );
}

// -----------------------------------------------------------------------------
// Name: HealableCapacity::serialize
// Created: RFT 2008-05-30
// -----------------------------------------------------------------------------
template< typename Archive >
void HealableCapacity::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this );        
}

// -----------------------------------------------------------------------------
// Name: HealableCapacity::Register
// Created: RFT 2008-07-03
// -----------------------------------------------------------------------------
void HealableCapacity::Register( Object& object )
{
    object.AddCapacity( this );
}

// -----------------------------------------------------------------------------
// Name: HealableCapacity::Instanciate
// Created: RFT 2008-06-08
// -----------------------------------------------------------------------------
void HealableCapacity::Instanciate( Object& object ) const
{
    object.AddCapacity( new HealableCapacity( *this ) );
}

// -----------------------------------------------------------------------------
// Name: HealableCapacity::Update
// Created: RFT 2008-05-22
// -----------------------------------------------------------------------------
void HealableCapacity::Update( Object& object, float time )
{
    MedicalTreatmentAttribute attr = object.GetAttribute< MedicalTreatmentAttribute >();
    
    //Initialize capacity state
    bool bCanReceiveNewPatient_ = UpdateInitialState( attr, time );

    //See if the medical structure is able to take care of the patient in the entranceWaitingList
    MakeDiagnosis( attr );

    //Take care of the patients in the careWaitingList
    while( bCanReceiveNewPatient_ && careWaitingList_.size() > 0 )
    {
        attr.FlagPatient( time, GetFirstWaitingPatientInjuryName() , GetFirstWaitingPatientInjuryCategory() );
        bCanReceiveNewPatient_ = UpdateState( attr );
        careWaitingList_.pop_front();
    }
}

// -----------------------------------------------------------------------------
// Name: HealableCapacity::Update
// Created: RFT 2008-05-22
// -----------------------------------------------------------------------------
bool HealableCapacity::UpdateInitialState( MedicalTreatmentAttribute& attr, float time )
{
    //Update data if any doctor or any bed is now available ( a patient is healed or leaves the medical structure )
    attr.FreeDoctors( time );
    attr.FreeBeds( time );

    //Update data
    attr.UpdateAvailableBeds( bEmergencyPlan_ , emergencyBedsRate_ );
    attr.UpdateAvailableDoctors( bEmergencyPlan_ , bBusinessHours_ , emergencyDoctorsRate_, nightDoctorsRate_ );
    
    //Update information about the possibility to admit a new patient
    return ( attr.GetAvailableBeds() > 0 && attr.GetAvailableDoctors() > 0 );
}

// -----------------------------------------------------------------------------
// Name: HealableCapacity::Update
// Created: RFT 2008-05-22
// -----------------------------------------------------------------------------
bool HealableCapacity::UpdateState( MedicalTreatmentAttribute& attr )
{
    //Update data
    attr.UpdateAvailableBeds( bEmergencyPlan_ , emergencyBedsRate_ );
    attr.UpdateAvailableDoctors( bEmergencyPlan_ , bBusinessHours_ , emergencyDoctorsRate_, nightDoctorsRate_ );

    //Update information about the possibility to admit a new patient
    return ( attr.GetAvailableBeds() > 0 && attr.GetAvailableDoctors() > 0 );
}



// -----------------------------------------------------------------------------
// Name: HealableCapacity::ActivateEmergencyPlan
// Created: RFT 2008-05-22
// -----------------------------------------------------------------------------
void HealableCapacity::ActivateEmergencyPlan()
{
    bEmergencyPlan_ = true ;
}

// -----------------------------------------------------------------------------
// Name: HealableCapacity::DeactivateEmergencyPlan
// Created: RFT 2008-05-22
// -----------------------------------------------------------------------------
void HealableCapacity::DeactivateEmergencyPlan()
{
    bEmergencyPlan_ = false ;
}

// -----------------------------------------------------------------------------
// Name: HealableCapacity::ReceivePatient
// Created: RFT 2008-05-22
// -----------------------------------------------------------------------------
void HealableCapacity::ReceivePatient( const PHY_Human& injuredHuman )
{
    entranceWaitingList_.push_back( injuredHuman );
}

// -----------------------------------------------------------------------------
// Name: HealableCapacity::ReceivePatient
// Created: RFT 2008-05-22
// -----------------------------------------------------------------------------
void HealableCapacity::TransferPatient()
{
    transferWaitingList_.pop_front();
}

// -----------------------------------------------------------------------------
// Name: HealableCapacity::MakeDiagnosis
// Created: RFT 2008-05-22
// -----------------------------------------------------------------------------
void HealableCapacity::MakeDiagnosis( MedicalTreatmentAttribute& attr )
{
    //First in first to be admitted in the medical structure...
    //It should also depend on the injury category : first UA then UR
    while( ! entranceWaitingList_.empty() )
    {
        if( attr.CanTreatPatient( entranceWaitingList_.front().GetWound().GetName() ) )
            careWaitingList_.push_back( entranceWaitingList_.front() );
        else
            transferWaitingList_.push_back( entranceWaitingList_.front() );

        entranceWaitingList_.pop_front();
    }
}

// -----------------------------------------------------------------------------
// Name: HealableCapacity::Update
// Created: RFT 2008-05-22
// -----------------------------------------------------------------------------
const std::string HealableCapacity::GetFirstWaitingPatientInjuryName() const
{
    return "Surgery";
}

int HealableCapacity::GetFirstWaitingPatientInjuryCategory() const
{
    return 1;//UA
}
