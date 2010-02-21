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
#include "protocol/SimulationSenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute constructor
// Created: RFT 2006-09-26
// -----------------------------------------------------------------------------
MedicalTreatmentAttribute::MedicalTreatmentAttribute( const Model& model, const Common::MsgObjectAttributes& message )
    : ObjectAttribute_ABC( model, message )
    , beds_              ( 0 )
    , availableBeds_     ( 0 )
    , doctors_           ( 0 )
    , availableDoctors_  ( 0 )
{
    Update( message );
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute destructor
// Created: RFT 2006-09-26
// -----------------------------------------------------------------------------
MedicalTreatmentAttribute::~MedicalTreatmentAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::Update
// Created: RFT 2006-09-26
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::Update( const Common::MsgObjectAttributes& message )
{
    if ( message.has_medical_treatment() )
    {
        beds_               = message.medical_treatment().beds();
        availableBeds_      = message.medical_treatment().available_beds();
        doctors_            = message.medical_treatment().doctors();
        availableDoctors_   = message.medical_treatment().available_doctors();
        for( unsigned int i = 0 ; i < message.medical_treatment().type_id().elem_size() ; i++ )
            medicalTreatmentList_.push_back( message.medical_treatment().type_id().elem( i ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::Send
// Created: RFT 2006-09-27
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::Send( Common::MsgObjectAttributes& message ) const
{
    message.mutable_medical_treatment()->set_available_beds    ( availableBeds_ );
    message.mutable_medical_treatment()->set_available_doctors ( availableDoctors_ );
    message.mutable_medical_treatment()->set_beds              ( beds_ );
    message.mutable_medical_treatment()->set_doctors           ( doctors_ );
    //Get the list of the ID of each medical treatment
    for( CIT_MedicalTreatmentTypeList iter = medicalTreatmentList_.begin() ; iter != medicalTreatmentList_.end() ; ++iter )
        message.mutable_medical_treatment()->mutable_type_id()->add_elem( *iter );

}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::Delete
// Created: RFT 2006-09-28
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::Delete( Common::MsgObjectAttributes& /*message*/ ) const
{
    // NOTHING
}
