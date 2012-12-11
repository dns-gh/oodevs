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
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DictionaryUpdated.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Infrastructure_ABC.h"
#include "clients_kernel/InfrastructureType.h"
#include "clients_kernel/MedicalTreatmentType.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/UrbanObject_ABC.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
MedicalTreatmentAttribute::MedicalTreatmentAttribute( const tools::Resolver_ABC< kernel::MedicalTreatmentType, std::string >& treatmentTypes,
                                                      kernel::PropertiesDictionary& dictionary, kernel::Controllers* controllers /* = 0 */, const kernel::Entity_ABC* owner /* = 0 */ )
    : controllers_( controllers )
    , dictionary_ ( dictionary )
    , owner_      ( owner )
    , doctors_    ( 0 )
    , resolver_   ( treatmentTypes )
{
    if( !owner_ )
        UpdateDictionary();
    if( controllers_ )
        controllers_->controller_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute constructor
// Created: SBO 2006-10-20
// -----------------------------------------------------------------------------
MedicalTreatmentAttribute::MedicalTreatmentAttribute( xml::xistream& xis, const tools::Resolver_ABC< kernel::MedicalTreatmentType, std::string >& treatmentTypes,
                                                      kernel::PropertiesDictionary& dictionary, kernel::Controllers* controllers /* = 0 */, const kernel::Entity_ABC* owner /* = 0 */ )
    : controllers_( controllers )
    , dictionary_ ( dictionary )
    , owner_      ( owner )
    , doctors_    ( 0 )
    , resolver_   ( treatmentTypes )
{
    if( owner_ ) // urban
    {
        if( xis.has_child( "medical-treatment" ) )
        {
            xis >> xml::start( "medical-treatment" );
            Update( xis );
            xis >> xml::end;
        }
    }
    else
        Update( xis );
    UpdateDictionary();
    if( controllers_ )
        controllers_->controller_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
MedicalTreatmentAttribute::~MedicalTreatmentAttribute()
{
    if( controllers_ )
        controllers_->controller_.Unregister( *this );
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
    catch( const std::exception& /*e*/ )
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
    if( !IsSet() )
        return;
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
    if( !IsSet() )
        return;
    Display( displayer );
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::IsSet
// Created: ABR 2012-05-30
// -----------------------------------------------------------------------------
bool MedicalTreatmentAttribute::IsSet() const
{
    if( !owner_ )
        return true;
    if( const kernel::UrbanObject_ABC* urbanObject = dynamic_cast< const kernel::UrbanObject_ABC* >( owner_ ) )
        if( const kernel::Infrastructure_ABC* infra = owner_->Retrieve< kernel::Infrastructure_ABC >() )
            if( const kernel::InfrastructureType* type = infra->GetType() )
                return type->IsMedical();
    return false;
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::SerializeAttributes
// Created: JSR 2012-09-04
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::SerializeAttributes( xml::xostream& xos ) const
{
    SerializeObjectAttributes( xos );
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::SerializeObjectAttributes
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::SerializeObjectAttributes( xml::xostream& xos ) const
{
    if( !IsSet() )
        return;
    xos << xml::start( "medical-treatment" )
            << xml::attribute( "doctors", doctors_ );
    if( !referenceID_.empty() )
        xos << xml::attribute( "reference", referenceID_ );
    xos     << xml::start( "bed-capacities" );
    for( T_TreatmentCapacities::const_iterator it = capacities_.begin(); it != capacities_.end(); ++it )
        xos     << xml::start( "bed-capacity" )
                    << xml::attribute( "type", it->first ) 
                    << xml::attribute( "baseline", it->second )
                << xml::end;
    xos     << xml::end // bed-capacities
        << xml::end; // medical-treatment
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::NotifyUpdated
// Created: JSR 2012-09-04
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::NotifyUpdated( const kernel::UrbanObject_ABC& object )
{
    if( &object == owner_ )
        UpdateDictionary( true );
}

namespace
{
    template< typename T >
    void CreateProperties( const kernel::Entity_ABC& entity, kernel::Controller& controller, kernel::PropertiesDictionary& dictionary,
        const QString& name, T& value, bool changed )
    {
        dictionary.Register( entity, name, value );
        if( changed )
            controller.Create( kernel::DictionaryUpdated( entity, name ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::UpdateDictionary
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::UpdateDictionary( bool changed )
{
    if( owner_ && controllers_ )
    {
        if( IsSet() )
        {
            CreateProperties( *owner_, controllers_->controller_, dictionary_, tools::translate( "MedicalTreatmentAttribute", "Info/Medical Treatment attributes/Doctors" ), doctors_, changed );
            CreateProperties( *owner_, controllers_->controller_, dictionary_, tools::translate( "MedicalTreatmentAttribute", "Info/Medical Treatment attributes/Hospital ID" ), referenceID_, changed );
            for( T_TreatmentCapacities::const_iterator it = capacities_.begin(); it != capacities_.end(); ++it )
                CreateProperties( *owner_, controllers_->controller_, dictionary_, tools::translate( "MedicalTreatmentAttribute", "Info/Medical Treatment attributes/" ) + it->first.c_str(), it->second, changed );
        }
        else
        {
            dictionary_.Remove( tools::translate( "MedicalTreatmentAttribute", "Info/Medical Treatment attributes/Doctors" ) );
            dictionary_.Remove( tools::translate( "MedicalTreatmentAttribute", "Info/Medical Treatment attributes/Hospital ID" ) );
            for( T_TreatmentCapacities::const_iterator it = capacities_.begin(); it != capacities_.end(); ++it )
                dictionary_.Remove( tools::translate( "MedicalTreatmentAttribute", "Info/Medical Treatment attributes/" ) + it->first.c_str() );
            controllers_->controller_.Delete( kernel::DictionaryUpdated( *owner_, tools::translate( "MedicalTreatmentAttribute", "Info/Medical Treatment attributes/" ) ) );
        }
    }
    else
    {
        dictionary_.Register( *this, tools::translate( "MedicalTreatmentAttribute", "Info/Medical Treatment attributes/Doctors" ), doctors_ );
        dictionary_.Register( *this, tools::translate( "MedicalTreatmentAttribute", "Info/Medical Treatment attributes/Hospital ID" ), referenceID_ );
        for( T_TreatmentCapacities::const_iterator it = capacities_.begin(); it != capacities_.end(); ++it )
            dictionary_.Register( *this, tools::translate( "MedicalTreatmentAttribute", "Info/Medical Treatment attributes/" ) + it->first.c_str(), it->second );
    }
}
