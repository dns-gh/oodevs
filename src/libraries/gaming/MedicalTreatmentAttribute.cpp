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


using namespace kernel;

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
MedicalTreatmentAttribute::MedicalTreatmentAttribute( Controller& controller, const tools::Resolver_ABC< MedicalTreatmentType >& resolver )
    : controller_           ( controller )
    , resolver_             ( resolver )
    , typeList_             ( 0 )
    , nMedicalTreatmentType_( 0 )
    , beds_                 ( 0 )
    , availableBeds_        ( 0 )
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
// Name: MedicalTreatmentAttribute::UpdateData
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
template< typename T >
void MedicalTreatmentAttribute::UpdateData( const T& message )
{
    if ( message.m.medical_treatmentPresent )
    {
        beds_             = message.medical_treatment.beds;
        availableBeds_    = message.medical_treatment.available_beds;
        doctors_          = message.medical_treatment.doctors;
        availableDoctors_ = message.medical_treatment.available_doctors;

        nMedicalTreatmentType_ = message.medical_treatment.type_id.n;
        for( int i = 0 ; i < nMedicalTreatmentType_ ; i++ )
            typeList_.push_back( & resolver_.Get( message.medical_treatment.type_id.elem [ i ] ) );

        controller_.Update( *(MedicalTreatmentAttribute_ABC*)this );        
    }
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::DoUpdate
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::DoUpdate( const ASN1T_MsgObjectKnowledgeUpdate& message )
{
    UpdateData( message.attributes );
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::DoUpdate( const ASN1T_MsgObjectUpdate& message )
{
    UpdateData( message.attributes );
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::DoUpdate( const ASN1T_MsgObjectCreation& message )
{
    UpdateData( message.attributes );
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::Display
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::Display( Displayer_ABC& displayer ) const
{
    Displayer_ABC& sub = displayer.Group( tools::translate( "MedicalTreatment", "Medical Treatment services" ) )
        .Item( tools::translate( "MedicalTreatment", "Medical Treatment type:" ) )
                .Start( nMedicalTreatmentType_ );
    
    for ( CIT_MedicalTreatmentTypeList it = typeList_.begin(); it != typeList_.end(); ++it )
        sub.Add( " " ).Add( (*it)->GetName() );
    sub.End();

    displayer.Group( tools::translate( "MedicalTreatment", "Medical Treatment capacity" ) )
        .Display( tools::translate( "MedicalTreatment", "Total number of beds:" ), beds_ )
        .Display( tools::translate( "MedicalTreatment", "Number of available beds:" ), availableBeds_ )
        .Display( tools::translate( "MedicalTreatment", "Total number of doctors:" ), doctors_ )
        .Display( tools::translate( "MedicalTreatment", "Number of available doctors:" ), availableDoctors_ );
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::DisplayInSummary
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::DisplayInSummary( kernel::Displayer_ABC& displayer ) const
{
    Display( displayer );
}
