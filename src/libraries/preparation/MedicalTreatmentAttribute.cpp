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
#include "clients_kernel/Tools.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
MedicalTreatmentAttribute::MedicalTreatmentAttribute( const tools::Resolver_ABC< kernel::MedicalTreatmentType, std::string >& treatmentTypes, kernel::PropertiesDictionary& dico )
    : doctors_  ( 0 )
    , resolver_ ( treatmentTypes )
{
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute constructor
// Created: SBO 2006-10-20
// -----------------------------------------------------------------------------
MedicalTreatmentAttribute::MedicalTreatmentAttribute( xml::xistream& xis, const tools::Resolver_ABC< kernel::MedicalTreatmentType, std::string >& treatmentTypes, kernel::PropertiesDictionary& dico )
    : doctors_  ( 0 )
    , resolver_ ( treatmentTypes )
{
    xis >> xml::attribute( "doctors", doctors_ )
        >> xml::optional >> xml::attribute( "reference", referenceID_ );
    xis >> xml::start( "bed-capacities" )
        >> list( "bed-capacity", *this, &MedicalTreatmentAttribute::ReadBedCapacity );
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
// Name: MedicalTreatmentAttribute::Update
// Created: SLG 2010-12-30
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::Update( xml::xistream& xis )
{
    xis >> xml::attribute( "doctors", doctors_ )
        >> xml::optional >> xml::attribute( "reference", referenceID_ );
    xis >> xml::start( "bed-capacities" )
        >> list( "bed-capacity", *this, &MedicalTreatmentAttribute::ReadBedCapacity );
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::ReadTreatment
// Created: JCR 2009-04-15
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::ReadBedCapacity( xml::xistream& xis )
{
    const std::string type( xis.attribute< std::string >( "type" ) );
    unsigned baseline = xis.attribute< unsigned >( "baseline" );

    UpdateTreatmentCapacity ( type, baseline );
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::UpdateTreatmentCapacity
// Created: JCR 2010-06-07
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::UpdateTreatmentCapacity( const std::string& type, unsigned beds )
{
    try
    {
        const MedicalTreatmentType* treatment = resolver_.Find( type );
        if( treatment )
            capacities_[ type ] = beds;
    }
    catch ( std::exception& /*e*/ )
    {
        // LOG_ Unknown treatment type
    }
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::SetDoctors
// Created: JCR 2010-06-07
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::SetDoctors( unsigned n )
{
    doctors_ = n;
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::SetReferenceID
// Created: JCR 2010-06-07
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::SetReferenceID( const std::string& id )
{
    referenceID_ = id;
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::Display
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "MedicalTreatment", "Medical Treatment" ) )
             .Display( tools::translate( "MedicalTreatment", "Total number of doctors:" ), doctors_ );
    displayer.Group( tools::translate( "MedicalTreatment", "Medical Treatment" ) )
             .Display( tools::translate( "MedicalTreatment", "Hospital ID:" ), referenceID_ );

    displayer.Group( tools::translate( "MedicalTreatment", "Bed Capacities:" ) );
    for( T_TreatmentCapacities::const_iterator it = capacities_.begin(); it != capacities_.end(); ++it )
        displayer.Display( std::string( it->first + ":" ).c_str(), it->second );
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::DisplayInTooltip
// Created: JCR 2008-06-10
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::DisplayInTooltip( Displayer_ABC& displayer ) const
{
    Display( displayer );
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::SerializeAttributes
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::start( "medical-treatment" )
        << xml::attribute( "doctors", doctors_ );
    if( !referenceID_.empty() )
        xos << xml::attribute( "reference", referenceID_ );
    xos << xml::start( "bed-capacities" );
    for( T_TreatmentCapacities::const_iterator it = capacities_.begin(); it != capacities_.end(); ++it )
        xos << xml::start( "bed-capacity" )
            << xml::attribute( "type", it->first ) << xml::attribute( "baseline", it->second )
            << xml::end;
    xos << xml::end
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::CreateDictionary
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::CreateDictionary( kernel::PropertiesDictionary& dico )
{
    dico.Register( *this, tools::translate( "MedicalTreatmentAttribute", "Info/Medical Treatment attributes/Doctors" ), doctors_ );
    dico.Register( *this, tools::translate( "MedicalTreatmentAttribute", "Info/Medical Treatment attributes/Hospital ID" ), referenceID_ );
    for( T_TreatmentCapacities::const_iterator it = capacities_.begin(); it != capacities_.end(); ++it )
        dico.Register( *this, tools::translate( "MedicalTreatmentAttribute", std::string( "Info/Medical Treatment attributes/" + it->first ).c_str() ), it->second );
}
