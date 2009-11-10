// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "MedicalTreatmentPrototype.h"
#include "clients_kernel/MedicalTreatmentType.h"
#include "tools/Iterator.h"
#include "game_asn/SimulationSenders.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentPrototype constructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
MedicalTreatmentPrototype::MedicalTreatmentPrototype( QWidget* parent, const tools::Resolver_ABC< MedicalTreatmentType >& resolver, ASN1T_MagicActionCreateObject& msg )
    : MedicalTreatmentPrototype_ABC( parent, resolver )
    , msg_( msg )     
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: MedicalTreatmentPrototype destructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
MedicalTreatmentPrototype::~MedicalTreatmentPrototype()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentPrototype::Commit
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void MedicalTreatmentPrototype::Commit()
{
    /*
    if( treatmentTypes_ != 0 )
    {
        msg_.attributes.m.medical_treatmentPresent          = 1;
        msg_.attributes.medical_treatment.beds              = beds_->value();
        msg_.attributes.medical_treatment.available_beds    = availableBeds_->value();
        msg_.attributes.medical_treatment.doctors           = doctors_->value();
        msg_.attributes.medical_treatment.available_doctors = availableDoctors_->value();

        msg_.attributes.medical_treatment.type_id.n         = 1;
        msg_.attributes.medical_treatment.type_id.elem      = new ASN1T_OID[ msg_.attributes.medical_treatment.type_id.n ];
        for( unsigned int i = 0 ; i < msg_.attributes.medical_treatment.type_id.n; i++)
            msg_.attributes.medical_treatment.type_id.elem[ i ] = treatmentTypes_->GetValue()->GetId();
    }
    */
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentPrototype::Clean
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void MedicalTreatmentPrototype::Clean()
{
    if( msg_.attributes.medical_treatment.type_id.n > 0 )
        delete [] msg_.attributes.medical_treatment.type_id.elem;
    msg_.attributes.medical_treatment.type_id.n = 0;
    msg_.attributes.m.medical_treatmentPresent = 0;
}
