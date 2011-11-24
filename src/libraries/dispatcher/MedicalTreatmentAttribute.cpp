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
#include <boost/lexical_cast.hpp>

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::Update
// Created: JCR 2010-06-02
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::MedicalCapacity::Update( const sword::MedicalTreatmentBedCapacity& capacity )
{
    typeId_ = capacity.type_id();
    if( capacity.has_baseline_count() )
        baseline_ = capacity.baseline_count();
    if( capacity.has_available_count() )
        available_ = capacity.available_count();
    if( capacity.has_emergency_count() )
        emergency_ = capacity.emergency_count();
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::Send
// Created: JCR 2010-06-02
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::MedicalCapacity::Send( sword::MedicalTreatmentBedCapacity& capacity ) const
{
    if( typeId_ >= 0 )
    {
        capacity.set_type_id( typeId_ );
        capacity.set_baseline_count( baseline_ );
        capacity.set_available_count( available_ );
        capacity.set_emergency_count( emergency_ );
    }
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute constructor
// Created: RFT 2006-09-26
// -----------------------------------------------------------------------------
MedicalTreatmentAttribute::MedicalTreatmentAttribute( const sword::ObjectAttributes& message )
    : doctors_             ( 0 )
    , availableDoctors_    ( 0 )
    , status_              ( sword::ObjectAttributeMedicalTreatment::normal )
{
    if( message.has_medical_treatment() )
        Update( message.medical_treatment() );
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
void MedicalTreatmentAttribute::Update( const sword::ObjectAttributes& message )
{
    if( message.has_medical_treatment() )
        Update( message.medical_treatment() );
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::Update
// Created: JCR 2010-10-07
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::Update( const sword::ObjectAttributeMedicalTreatment& message )
{
    if( message.has_doctors() )
        doctors_ = message.doctors();
    if( message.has_available_doctors() )
        availableDoctors_ = message.available_doctors();
    if( message.has_external_reference_id() )
        referenceID_ = message.external_reference_id();
    if( message.has_facility_status() )
        status_ = message.facility_status();
    for( int i = 0 ; i < message.bed_capacities_size(); ++i )
    {
        const sword::MedicalTreatmentBedCapacity& bed_capacity = message.bed_capacities( i );
        if( bed_capacity.has_type_id() )
            capacities_[ bed_capacity.type_id() ].Update( bed_capacity );
    }
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::Send
// Created: RFT 2006-09-27
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::Send( sword::ObjectAttributes& message ) const
{
    message.mutable_medical_treatment()->set_doctors                ( doctors_ );
    message.mutable_medical_treatment()->set_available_doctors      ( availableDoctors_ );
    message.mutable_medical_treatment()->set_external_reference_id  ( referenceID_ );
    message.mutable_medical_treatment()->set_facility_status        ( status_ );
    for( T_TreatmentCapacityVector::const_iterator it = capacities_.begin(); it != capacities_.end(); ++it )
        it->second.Send( *message.mutable_medical_treatment()->add_bed_capacities() );
}
