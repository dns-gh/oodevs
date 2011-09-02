// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MedicalCapacity.h"
#include "MedicalTreatmentAttribute.h"
#include "Entities/Agents/Units/Humans/PHY_InjuredHuman.h"
#include "Object.h"
#include <xeumeuleu/xml.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( MedicalCapacity )

// -----------------------------------------------------------------------------
// Name: MedicalCapacity::MedicalCapacity
// Created: RFT 2008-05-22
// -----------------------------------------------------------------------------
MedicalCapacity::MedicalCapacity( xml::xistream& xis )
    : emergencyBedsRate_     ( 0 )
    , emergencyDoctorsRate_  ( 0 )
    , nightDoctorsRate_      ( 0 )
{
    InitializeData( xis );
}

// -----------------------------------------------------------------------------
// Name: MedicalCapacity::MedicalCapacity
// Created: RFT 2008-05-22
// -----------------------------------------------------------------------------
MedicalCapacity::MedicalCapacity()
    : emergencyBedsRate_     ( 0 )
    , emergencyDoctorsRate_  ( 0 )
    , nightDoctorsRate_      ( 0 )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: MedicalCapacity::MedicalCapacity
// Created: RFT 2008-05-22
// -----------------------------------------------------------------------------
MedicalCapacity::MedicalCapacity( float emergencyBedsRate, float emergencyDoctorsRate, float nightDoctorsRate )
: emergencyBedsRate_     ( emergencyBedsRate )
, emergencyDoctorsRate_  ( emergencyDoctorsRate )
, nightDoctorsRate_      ( nightDoctorsRate )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: MedicalCapacity constructor
// Created: RFT 2008-05-22
// -----------------------------------------------------------------------------
MedicalCapacity::MedicalCapacity( const MedicalCapacity& /*from*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MedicalCapacity destructor
// Created: RFT 2008-05-22
// -----------------------------------------------------------------------------
MedicalCapacity::~MedicalCapacity()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: MedicalCapacity::InitializeData
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
void MedicalCapacity::InitializeData( xml::xistream& xis )
{
    xis >> xml::attribute( "night-doctors-rate", nightDoctorsRate_ )
        >> xml::start( "emergency-plan" )
            >> xml::attribute( "beds-rate", emergencyBedsRate_ )
            >> xml::attribute( "doctors-rate", emergencyDoctorsRate_ )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: MedicalCapacity::serialize
// Created: RFT 2008-05-30
// -----------------------------------------------------------------------------
template< typename Archive >
void MedicalCapacity::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: MedicalCapacity::Register
// Created: RFT 2008-07-03
// -----------------------------------------------------------------------------
void MedicalCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
}

// -----------------------------------------------------------------------------
// Name: MedicalCapacity::Instanciate
// Created: RFT 2008-06-08
// -----------------------------------------------------------------------------
void MedicalCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    object.AddCapacity( new MedicalCapacity( *this ) );
}

// -----------------------------------------------------------------------------
// Name: MedicalCapacity::Update
// Created: RFT 2008-05-22
// -----------------------------------------------------------------------------
void MedicalCapacity::Update( MIL_Object_ABC& object, unsigned int /*time*/ )
{
    MedicalTreatmentAttribute& attr = object.GetAttribute< MedicalTreatmentAttribute >();

    //See if the medical structure is able to take care of the patient in the entranceWaitingList
    MakeDiagnosis( attr );
}

// -----------------------------------------------------------------------------
// Name: MedicalCapacity::Update
// Created: SLG 2010-12-30
// -----------------------------------------------------------------------------
void MedicalCapacity::Update( xml::xistream& xis, const MIL_Object_ABC& object )
{
    const_cast< MIL_Object_ABC& >( object ).GetAttribute< MedicalTreatmentAttribute >() = MedicalTreatmentAttribute( xis );
}

// -----------------------------------------------------------------------------
// Name: MedicalCapacity::ReceivePatient
// Created: RFT 2008-05-22
// -----------------------------------------------------------------------------
void MedicalCapacity::ReceivePatient( PHY_InjuredHuman& injuredHuman )
{
    entranceWaitingList_.push( &injuredHuman );
}

// -----------------------------------------------------------------------------
// Name: MedicalCapacity::ReceivePatient
// Created: RFT 2008-05-22
// -----------------------------------------------------------------------------
void MedicalCapacity::TransferPatient()
{
    transferWaitingList_.pop();
}

// -----------------------------------------------------------------------------
// Name: MedicalCapacity::MakeDiagnosis
// Created: RFT 2008-05-22
// -----------------------------------------------------------------------------
void MedicalCapacity::MakeDiagnosis( MedicalTreatmentAttribute& attr )
{
    // First in first to be admitted in the medical structure...
    // It should also depend on the injury category : first UA then UR
    while( ! entranceWaitingList_.empty() )
    {
        PHY_InjuredHuman* patient = entranceWaitingList_.front();
        if( patient->IsAlive() )
        {
            if( attr.CanTreatPatient( patient->GetInjuryID() ) )
                Accept( attr, *patient );
            else
                RequestTransfert( *patient );
        }
        else
            deadsList_.push( patient ); // Accept to the Morgue if hospital has the capacity
        entranceWaitingList_.pop();
    }
}

// -----------------------------------------------------------------------------
// Name: MedicalCapacity::Accept
// Created: JCR 2010-06-05
// -----------------------------------------------------------------------------
void MedicalCapacity::Accept( MedicalTreatmentAttribute& attr, PHY_InjuredHuman& injuredHuman )
{
    injuredHuman.TreatInjuredHuman( attr );
}

// -----------------------------------------------------------------------------
// Name: MedicalCapacity::CanTreat
// Created: LDC 2010-07-01
// -----------------------------------------------------------------------------
bool MedicalCapacity::CanTreat( const MIL_Object_ABC& object, const PHY_InjuredHuman& injuredHuman )
{
    const MedicalTreatmentAttribute& attr = object.GetAttribute< MedicalTreatmentAttribute >();
    return attr.CanTreatPatient( injuredHuman.GetInjuryID() );
}

// -----------------------------------------------------------------------------
// Name: MedicalCapacity::RequestTransfert
// Created: JCR 2010-06-05
// -----------------------------------------------------------------------------
void MedicalCapacity::RequestTransfert( PHY_InjuredHuman& injuredHuman )
{
    transferWaitingList_.push( &injuredHuman );
}