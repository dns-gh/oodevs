// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "MedicalTreatmentAttribute.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/MedicalTreatmentType.h"
#include "Tools.h"
#include <boost/lexical_cast.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
MedicalTreatmentAttribute::MedicalTreatmentAttribute( Controller& controller, const tools::Resolver_ABC< MedicalTreatmentType >& resolver )
    : controller_           ( controller )
    , resolver_             ( resolver )
    , doctors_              ( 0 )
    , availableDoctors_     ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
MedicalTreatmentAttribute::~MedicalTreatmentAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::FillData
// Created: JCR 2010-06-28
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::FillCapacities( T_TreatmentCapacities& data ) const
{
    data = capacities_;
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::GetReferenceId
// Created: JCR 2010-06-28
// -----------------------------------------------------------------------------
const std::string& MedicalTreatmentAttribute::GetReferenceId() const
{
    return referenceID_;
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::GetDoctors
// Created: JCR 2010-07-09
// -----------------------------------------------------------------------------
int MedicalTreatmentAttribute::GetDoctors() const
{
    return doctors_;
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::GetAvailableDoctors
// Created: JCR 2010-07-09
// -----------------------------------------------------------------------------
int MedicalTreatmentAttribute::GetAvailableDoctors() const
{
    return availableDoctors_;
}

namespace
{
    void UpdateCapacity( const kernel::MedicalTreatmentType& type, const sword::MedicalTreatmentBedCapacity& message, MedicalTreatmentCapacity& capacity )
    {
        capacity.type_ = &type;
        capacity.name_ = type.GetName();
        if( message.has_available_count() )
            capacity.available_ = message.available_count();
        if( message.has_baseline_count() )
        {
            capacity.baseline_ = message.baseline_count();
            capacity.available_ = std::min( capacity.available_, capacity.baseline_ );
        }
        if( message.has_emergency_count() )
            capacity.emergency_ = message.emergency_count();
    }
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::MedicalTreatmentAttribute::UpdateData
// Created: JCR 2010-10-07
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::UpdateData( const sword::ObjectAttributeMedicalTreatment& message )
{
    if( message.has_doctors() )
        doctors_ = message.doctors();
    if( message.has_available_doctors() )
        availableDoctors_ = message.available_doctors();
    if( message.has_external_reference_id() )
        referenceID_ = message.external_reference_id();
    if( message.has_facility_status() )
        status_ = message.facility_status();
    for( int i = 0 ; i < message.bed_capacities_size(); i++ )
    {
        const sword::MedicalTreatmentBedCapacity& capacity = message.bed_capacities( i );
        if( capacity.has_type_id() )
        {
            const kernel::MedicalTreatmentType* type = resolver_.Find( capacity.type_id() );
            if( !type )
                throw std::runtime_error( std::string( __FUNCTION__  )+ " Unknown injury id: " + boost::lexical_cast< std::string >( capacity.type_id() ) );
            ::UpdateCapacity( *type, capacity, capacities_[ capacity.type_id() ] );
        }
    }
    controller_.Update( *(MedicalTreatmentAttribute_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::DoUpdate
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::DoUpdate( const sword::ObjectKnowledgeUpdate& message )
{
    if( message.attributes().has_medical_treatment() )
        UpdateData( message.attributes().medical_treatment() );
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::DoUpdate( const sword::ObjectUpdate& message )
{
    if( message.attributes().has_medical_treatment() )
        UpdateData( message.attributes().medical_treatment() );
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::Display
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "MedicalTreatment", "Medical Treatment" ) )
         .Display( tools::translate( "MedicalTreatment", "Total number of doctors:" ), doctors_ )
         .Display( tools::translate( "MedicalTreatment", "Number of available doctors:" ), availableDoctors_ );

    displayer.Group( tools::translate( "MedicalTreatment", "Medical Treatment" ) )
         .Display( tools::translate( "MedicalTreatment", "Hospital ID:" ), referenceID_ );

    // $$$$ LDC 2011-04-04 FIXME: These are displayed in ObjectPanel but not here???
    tools::translate( "MedicalTreatment", "Total number of beds:" );
    tools::translate( "MedicalTreatment", "Number of available beds:" );

    displayer.Group( tools::translate( "MedicalTreatment", "Medical Treatment services (Available(Baseline)):" ) );
    for( T_TreatmentCapacities::const_iterator it = capacities_.begin(); it != capacities_.end(); ++it )
    {
        if( it->second.type_ )
            displayer.Display( std::string( it->second.name_ + ": " ).c_str(),
                           std::string( boost::lexical_cast<std::string>( it->second.available_ ) + " (" + boost::lexical_cast<std::string>( it->second.baseline_ ) + ")" ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::DisplayInSummary
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::DisplayInSummary( kernel::Displayer_ABC& displayer ) const
{
    Display( displayer );
}
