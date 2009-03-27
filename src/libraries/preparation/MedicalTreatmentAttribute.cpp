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
    : typeList_             ( 0 )
    , nMedicalTreatmentType_( 0 )
    , beds_                 ( 0 )
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
MedicalTreatmentAttribute::MedicalTreatmentAttribute( xml::xistream& xis, const kernel::Resolver_ABC< kernel::MedicalTreatmentType, std::string >& MedicalTreatmentTypes, kernel::PropertiesDictionary& dico )
    : typeList_             ( 0 )
    , nMedicalTreatmentType_( 0 )
    , beds_                 ( 0 )
    , availableBeds_        ( 0 )
    , doctors_              ( 0 )
    , availableDoctors_     ( 0 )
{
    std::string type;
    xis >> content( "type", type )
        >> content( "nMedicalTreatmentType", nMedicalTreatmentType_ )
        >> content( "beds", beds_ )
        >> content( "availableBeds", availableBeds_ )
        >> content( "doctors", doctors_ )
        >> content( "availableDoctors", availableDoctors_ );

    typeList_.push_back( MedicalTreatmentTypes.Find( type ) );

    CreateDictionary( dico );
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
        .Display( tools::translate( "MedicalTreatment", "Number of available beds:" ), availableBeds_ ).Display( tools::translate( "MedicalTreatment", "" ), beds_ )
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
void MedicalTreatmentAttribute::SetData( const kernel::MedicalTreatmentType& type )
{
    typeList_.push_back( const_cast< kernel::MedicalTreatmentType* >( &type ) );
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::SerializeAttributes
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::SerializeAttributes( xml::xostream& xos ) const
{
    xos << start( "attributes" )
            << start( "medical-treatment type" );

    for( CIT_MedicalTreatmentTypeList it = typeList_.begin() ; it != typeList_.end() ; ++it )
            xos << content( "type", (*it)->GetName() );

            xos << content( "beds", beds_ )
                << content( "availableBeds", availableBeds_ )
                << content( "doctors", doctors_ )
                << content( "availableDoctors", availableDoctors_ )
            << end()
        << end();
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::CreateDictionary
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::CreateDictionary( kernel::PropertiesDictionary& dico )
{
    for( CIT_MedicalTreatmentTypeList it = typeList_.begin() ; it != typeList_.end() ; ++it )
        dico.Register( *this, tools::translate( "MedicalTreatmentAttribute", "Info/Medical Treatment attributes/Medical Treatment type" ), (*it)->GetName() );
    dico.Register( *this, tools::translate( "MedicalTreatmentAttribute", "Info/Medical Treatment attributes/Medical Treatment type" ), beds_ );
    dico.Register( *this, tools::translate( "MedicalTreatmentAttribute", "Info/Medical Treatment attributes/Medical Treatment type" ), availableBeds_ );
    dico.Register( *this, tools::translate( "MedicalTreatmentAttribute", "Info/Medical Treatment attributes/Medical Treatment type" ), doctors_ );
    dico.Register( *this, tools::translate( "MedicalTreatmentAttribute", "Info/Medical Treatment attributes/Medical Treatment type" ), availableDoctors_ );
}
