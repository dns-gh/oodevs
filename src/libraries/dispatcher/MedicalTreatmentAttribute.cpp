// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "MedicalTreatmentAttribute.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute constructor
// Created: RFT 2006-09-26
// -----------------------------------------------------------------------------
MedicalTreatmentAttribute::MedicalTreatmentAttribute( const Model& model, const ASN1T_ObjectAttributes& asnMsg )
    : ObjectAttribute_ABC  ( model, asnMsg )
    , beds_                ( 0 )
    , availableBeds_       ( 0 )
    , doctors_             ( 0 )
    , availableDoctors_    ( 0 )
    , medicalTreatmentList_()
{
    if( asnMsg.m.medical_treatmentPresent )
    {
        beds_   = asnMsg.medical_treatment.beds;
        availableBeds_ = asnMsg.medical_treatment.available_beds;
        doctors_ = asnMsg.medical_treatment.doctors;
        availableDoctors_ = asnMsg.medical_treatment.available_doctors;
        for( int i = 0 ; i < asnMsg.medical_treatment.type_id.n ; i++ )
            medicalTreatmentList_.push_back( asnMsg.medical_treatment.type_id.elem[ i ] );
 
    }
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute destructor
// Created: RFT 2006-09-26
// -----------------------------------------------------------------------------
MedicalTreatmentAttribute::~MedicalTreatmentAttribute()
{

}  

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::Update
// Created: RFT 2006-09-26
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::Update( const ASN1T_ObjectAttributes& asnMsg )
{
    if ( asnMsg.m.medical_treatmentPresent )
    {
        beds_   = asnMsg.medical_treatment.beds;
        availableBeds_ = asnMsg.medical_treatment.available_beds;
        doctors_ = asnMsg.medical_treatment.doctors;
        availableDoctors_ = asnMsg.medical_treatment.available_doctors;
        for( int i = 0 ; i < asnMsg.medical_treatment.type_id.n ; i++ )
            medicalTreatmentList_.push_back( asnMsg.medical_treatment.type_id.elem[ i ] );
    }  
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::Send
// Created: RFT 2006-09-27
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::Send( ASN1T_ObjectAttributes& asnMsg ) const
{
    int i = 0; //i is used to fill elem
    asnMsg.m.medical_treatmentPresent = 1;
    asnMsg.medical_treatment.available_beds    = availableBeds_;
    asnMsg.medical_treatment.available_doctors = availableDoctors_;
    asnMsg.medical_treatment.beds              = beds_;
    asnMsg.medical_treatment.doctors           = doctors_;
    asnMsg.medical_treatment.type_id.n         = medicalTreatmentList_.size();
    asnMsg.medical_treatment.type_id.elem      = new ASN1T_OID[ asnMsg.medical_treatment.type_id.n ];
    //Get the list of the ID of each medical treatment
    for( CIT_MedicalTreatmentTypeList iter = medicalTreatmentList_.begin() ; iter != medicalTreatmentList_.end() ; ++iter )
    {
        asnMsg.medical_treatment.type_id.elem[ i ] = *iter;
        i++;
    }
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::AsnDelete
// Created: RFT 2006-09-28
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::AsnDelete( ASN1T_ObjectAttributes& /*asnMsg*/ ) const
{
    // NOTHING
}
