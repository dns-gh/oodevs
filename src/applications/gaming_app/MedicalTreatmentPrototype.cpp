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
#include "protocol/Protocol.h"
#include "tools/Iterator.h"
#include "actions/ParameterList.h"

using namespace Common;
using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentPrototype constructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
MedicalTreatmentPrototype::MedicalTreatmentPrototype( QWidget* parent, const tools::Resolver_ABC< MedicalTreatmentType >& resolver, actions::parameters::ParameterList*& attributesList )
    : MedicalTreatmentPrototype_ABC( parent, resolver )
    , attributesList_( attributesList )
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
        msg_.mutable_attributes()->mutable_medical_treatment()->set_beds              ( beds_->value() );
        msg_.mutable_attributes()->mutable_medical_treatment()->set_available_beds    ( availableBeds_->value() );
        msg_.mutable_attributes()->mutable_medical_treatment()->set_doctors           ( doctors_->value() );
        msg_.mutable_attributes()->mutable_medical_treatment()->set_available_doctors ( availableDoctors_->value() );
        for( int i = 0 ; i < 1; i++)
            msg_.mutable_attributes()->mutable_medical_treatment()->mutable_type_id()->add_elem( type_->GetValue()->GetId() );
    }
    */
}
