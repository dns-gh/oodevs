// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "MedicalTreatmentAttribute.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/MedicalTreatmentType.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "Tools.h"
#include <xeumeuleu/xml.h>

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
MedicalTreatmentAttribute::MedicalTreatmentAttribute( kernel::PropertiesDictionary& dico )
    : beds_                 ( 0 )
    , availableBeds_        ( 0 )
    , doctors_              ( 0 )
    , availableDoctors_     ( 0 )
{
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute constructor
// Created: SBO 2006-10-20
// -----------------------------------------------------------------------------
MedicalTreatmentAttribute::MedicalTreatmentAttribute( xml::xistream& xis, const tools::Resolver_ABC< kernel::MedicalTreatmentType, std::string >& treatmentTypes, kernel::PropertiesDictionary& dictionary )
    : beds_                 ( 0 )
    , availableBeds_        ( 0 )
    , doctors_              ( 0 )
    , availableDoctors_     ( 0 )
{    
    xis >> attribute( "beds", beds_ ) >> attribute( "availableBeds", availableBeds_ )
        >> attribute( "doctors", doctors_ ) >> attribute( "availableDoctors", availableDoctors_ );
    xis >> list( "type", *this, &MedicalTreatmentAttribute::ReadTreatment, treatmentTypes );
    CreateDictionary( dictionary );
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
// Name: MedicalTreatmentAttribute::ReadTreatment
// Created: JCR 2009-04-15
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::ReadTreatment( xml::xistream& xis, const tools::Resolver_ABC< kernel::MedicalTreatmentType, std::string >& treatmentResolver )
{
    std::string type = content( xis, "type", std::string() );
    const MedicalTreatmentType& treatment = treatmentResolver.Get( type );
    AddMedicalTreatment( treatment );
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::Display
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "MedicalTreatment", "Medical Treatment" ) )
        .Display( tools::translate( "MedicalTreatment", "Medical Treatment types:" ), treatmentTypes_ )
        .Display( tools::translate( "MedicalTreatment", "Total number of beds:" ), beds_ )
        .Display( tools::translate( "MedicalTreatment", "Number of available beds:" ), availableBeds_ )
        .Display( tools::translate( "MedicalTreatment", "Total number of doctors:" ), doctors_ )
        .Display( tools::translate( "MedicalTreatment", "Number of available doctors:" ), availableDoctors_ );
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::DisplayInTooltip
// Created: JCR 2008-06-10
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::DisplayInTooltip( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "MedicalTreatment", "Medical Treatment" ) )
        .Display( tools::translate( "MedicalTreatment", "Total number of beds:" ), beds_ )
        .Display( tools::translate( "MedicalTreatment", "Number of available beds:" ), availableBeds_ )
        .Display( tools::translate( "MedicalTreatment", "Total number of doctors:" ), doctors_ )
        .Display( tools::translate( "MedicalTreatment", "Number of available doctors:" ), availableDoctors_ );
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::SetAgent
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::AddMedicalTreatment( const kernel::MedicalTreatmentType& type )
{
    if ( std::find( treatmentTypes_.begin(), treatmentTypes_.end(), &type ) == treatmentTypes_.end() )
        treatmentTypes_.push_back( &type );
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::SerializeAttributes
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::SerializeAttributes( xml::xostream& xos ) const
{
    xos << start( "medical-treatment" );
    xos << attribute( "beds", beds_ )
        << attribute( "availableBeds", availableBeds_ )
        << attribute( "doctors", doctors_ )
        << attribute( "availableDoctors", availableDoctors_ );
    for( T_MedicalTreatments::const_iterator it = treatmentTypes_.begin(); it != treatmentTypes_.end(); ++it )    
        xos << content( "type", (*it)->GetName() );
    xos << end();
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::CreateDictionary
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::CreateDictionary( kernel::PropertiesDictionary& dico )
{
    dico.Register( *this, tools::translate( "MedicalTreatmentAttribute", "Info/Medical Treatment attributes/Medical Treatment type" ), beds_ );
    dico.Register( *this, tools::translate( "MedicalTreatmentAttribute", "Info/Medical Treatment attributes/Medical Treatment type" ), availableBeds_ );
    dico.Register( *this, tools::translate( "MedicalTreatmentAttribute", "Info/Medical Treatment attributes/Medical Treatment type" ), doctors_ );
    dico.Register( *this, tools::translate( "MedicalTreatmentAttribute", "Info/Medical Treatment attributes/Medical Treatment type" ), availableDoctors_ );
}
