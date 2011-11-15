// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MedicalTreatmentAttribute.h"
#include "MIL_MedicalTreatmentType.h"
#include "protocol/Protocol.h"
#include <xeumeuleu/xml.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>
#include <numeric>

BOOST_CLASS_EXPORT_IMPLEMENT( MedicalTreatmentAttribute )
BOOST_CLASS_EXPORT_KEY( DEC_Knowledge_ObjectAttributeProxyPassThrough< MedicalTreatmentAttribute > )
BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Knowledge_ObjectAttributeProxyPassThrough< MedicalTreatmentAttribute > )

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute constructor
// Created: RFT 2008-05-30
// -----------------------------------------------------------------------------
MedicalTreatmentAttribute::MedicalTreatmentAttribute()
    : status_          ( sword::ObjectAttributeMedicalTreatment::normal )
    , doctors_         ( 0 )
    , availableDoctors_( 0 )
    , initialDoctors_  ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::MedicalTreatmentAttribute
// Created: RFT 2008-06-05
// -----------------------------------------------------------------------------
MedicalTreatmentAttribute::MedicalTreatmentAttribute( xml::xistream& xis )
    : status_    ( sword::ObjectAttributeMedicalTreatment::normal )
{
    xis >> xml::attribute( "doctors", doctors_ )
        >> xml::optional >> xml::attribute( "reference", referenceID_ )
        >> xml::start( "bed-capacities" )
            >> xml::list( "bed-capacity", *this, &MedicalTreatmentAttribute::InitializeBedCapacity )
        >> xml::end;
    initialDoctors_ = doctors_;
    availableDoctors_ = doctors_;
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::InitializeBedCapacity
// Created: JCR 2010-06-05
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::InitializeBedCapacity( xml::xistream& xis )
{
    const std::string typeName( xml::attribute< std::string >( xis, "type" ) );
    const MIL_MedicalTreatmentType* pType = MIL_MedicalTreatmentType::Find( typeName );
    if( !pType )
        xis.error( "Unknown 'Medical treatment type' '" + typeName + "' for medical treatment attribute" );
    MedicalCapacity& capacity = capacities_[ pType->GetID() ];
    capacity.type_ = pType;
    capacity.baseline_ = xml::attribute< unsigned int >( xis, "baseline" );
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::MedicalTreatmentAttribute
// Created: RFT 2008-06-05
// -----------------------------------------------------------------------------
MedicalTreatmentAttribute::MedicalTreatmentAttribute( const sword::MissionParameter_Value& attributes )
    : doctors_         ( attributes.list( eDoctors ).quantity() ) // Doctors
    , availableDoctors_( 0 )
    , initialDoctors_  ( 0 )
    , referenceID_     ( attributes.list( eExternalReferenceId ).acharstr() ) // ExternalReferenceId
    , status_          ( sword::ObjectAttributeMedicalTreatment::normal )
{
    const sword::MissionParameter_Value& capacities = attributes.list( eBedCapacities );
    for( int i = 0; i < capacities.list_size(); ++i )
    {
        const sword::MissionParameter_Value& value = capacities.list( i );
        const MIL_MedicalTreatmentType* pType = MIL_MedicalTreatmentType::Find( value.list( eTypeId ).identifier() );
        if( !pType )
            throw std::runtime_error( "Unknown Medical treatment type for medical treatment attribute" );
        MedicalCapacity& capacity = capacities_[ pType->GetID() ];
        capacity.type_ = pType;
        capacity.baseline_ = value.list( eBaseLineCount ).quantity();
        // TODO $$$$ : capacity.occupied_ = capacity.baseline_ - value.list( 2 ).quantity();
        // capacity.emergency_ = value.list( 3 ).quantity();
    }
    // InitializePatientDiagnosisList( beds_ - availableBeds_ , doctors_ - availableDoctors_ );
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute destructor
// Created: RFT 2008-05-30
// -----------------------------------------------------------------------------
MedicalTreatmentAttribute::~MedicalTreatmentAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::operator=
// Created: RFT 2008-05-30
// -----------------------------------------------------------------------------
MedicalTreatmentAttribute& MedicalTreatmentAttribute::operator=( const MedicalTreatmentAttribute& rhs )
{
    // medicalTreatmentMap_;
    doctors_ = rhs.doctors_;
    availableDoctors_ = rhs.availableDoctors_;
    initialDoctors_ = rhs.initialDoctors_;
    capacities_ = rhs.capacities_;
    referenceID_ = rhs.referenceID_;
    status_ = rhs.status_;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::load
// Created: RFT 2008-07-03
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::load( MIL_CheckPointInArchive& ar, const unsigned int )
{
    ar >> boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    ar >> doctors_
       >> availableDoctors_
       >> initialDoctors_
       >> referenceID_
       >> status_;
    ar >> capacities_;
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::save
// Created: RFT 2008-07-03
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::save( MIL_CheckPointOutArchive& ar, const unsigned int ) const
{
    ar << boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    ar << doctors_
       << availableDoctors_
       << initialDoctors_
       << referenceID_
       << status_;
    ar << capacities_;
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::Instanciate
// Created: RFT 2008-06-09
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::Instanciate( DEC_Knowledge_Object& object ) const
{
    object.Attach< DEC_Knowledge_ObjectAttributeProxy_ABC< MedicalTreatmentAttribute > >( *new T_KnowledgeProxyType() );
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::Update
// Created: JCR 2010-06-05
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::Register( MIL_Object_ABC& object ) const
{
    object.SetAttribute< MedicalTreatmentAttribute, MedicalTreatmentAttribute >( *this );
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::load
// Created: JCR 2010-06-05
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::MedicalCapacity::load( MIL_CheckPointInArchive& ar, const unsigned int )
{
    int type;
    ar >> baseline_
       >> occupied_
       >> emergency_
       >> time_;
    ar >> type;
    if( type >= 0 )
        type_ = MIL_MedicalTreatmentType::Find( type );
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::save
// Created: JCR 2010-06-05
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::MedicalCapacity::save( MIL_CheckPointOutArchive& ar, const unsigned int ) const
{
    ar << baseline_
       << occupied_
       << emergency_
       << time_;
    int type = -1;
    if( type_ )
        type = type_->GetID();
    ar << type;
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::Update
// Created: JCR 2010-06-05
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::MedicalCapacity::Update( const sword::MedicalTreatmentBedCapacity& capacity )
{
    if( capacity.has_type_id() )
        type_ = MIL_MedicalTreatmentType::Find( capacity.type_id() );
    if( type_ )
    {
        if( capacity.has_baseline_count() )
            baseline_ = capacity.baseline_count();
        if( capacity.has_available_count() ) // UA
            occupied_[ 1 ] = baseline_ > capacity.available_count() ? baseline_ - capacity.available_count() : 0;
        if( capacity.has_emergency_count() )
            emergency_ = capacity.emergency_count();
    }
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::Update
// Created: JCR 2010-10-08
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::MedicalCapacity::Update( const sword::MissionParameter_Value& capacity )
{
    if( capacity.list( eTypeId ).has_identifier() )
        type_ = MIL_MedicalTreatmentType::Find( capacity.list( eTypeId ).identifier()  );
    if( type_ )
    {
        if( capacity.list( eBaseLineCount ).has_quantity() )
            baseline_ = capacity.list( eBaseLineCount ).quantity();
        if( capacity.list( eAvailableCount ).has_quantity() ) // UA
            occupied_[ 1 ] = std::max( static_cast< unsigned int >( 0 ), baseline_ - capacity.list( eAvailableCount ).quantity() );
        if( capacity.list( eEmergencyCount ).has_quantity() )
            emergency_ = capacity.list( eEmergencyCount ).quantity();
    }
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::Send
// Created: JCR 2010-06-05
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::MedicalCapacity::Send( sword::MedicalTreatmentBedCapacity& capacity ) const
{
    if( type_ )
    {
        capacity.set_type_id( type_->GetID() );
        capacity.set_baseline_count( baseline_ );
        capacity.set_available_count( baseline_ - std::accumulate( occupied_.begin(), occupied_.end(), 0 ) );
        capacity.set_emergency_count( emergency_ );
    }
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::OnUpdate
// Created: JCR 2010-06-11
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::OnUpdate( const sword::MissionParameter_Value& parameters )
{
    // JCR TODO : Check if value is available ?
    if( parameters.list( eDoctors ).has_quantity() )
        doctors_ = parameters.list( eDoctors ).quantity(); //

    // JCR TODO : Check if value is available ?
    if( parameters.list( eStatus ).has_enumeration() )
        status_ = parameters.list( eStatus ).enumeration();

    const sword::MissionParameter_Value& capacities = parameters.list( eBedCapacities );
    for( int i = 0; i < capacities.list_size(); ++i )
    {
        const sword::MissionParameter_Value& value = capacities.list( i );
        if( value.list( eTypeId ).has_identifier() )
        {
            const unsigned int typeId = value.list( eTypeId ).identifier();
            capacities_[ typeId ].Update( value );
        }
    }
    NotifyAttributeUpdated( eOnUpdate );
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::Update
// Created: JCR 2010-06-04
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::Update( const sword::ObjectAttributeMedicalTreatment& message )
{
    if( message.has_doctors() )
        doctors_ = message.doctors();
    if( message.has_available_doctors() )
        availableDoctors_ = message.available_doctors();
    else
        availableDoctors_ = doctors_;
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
    NotifyAttributeUpdated( eOnUpdate );
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::SendFullState
// Created: RFT 2008-06-18
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::SendFullState( sword::ObjectAttributes& message ) const
{
    message.mutable_medical_treatment()->set_doctors( doctors_ );
    message.mutable_medical_treatment()->set_available_doctors( availableDoctors_ );
    message.mutable_medical_treatment()->set_external_reference_id( referenceID_ );
    message.mutable_medical_treatment()->set_facility_status( static_cast< sword::ObjectAttributeMedicalTreatment_EnumMedicalTreatmentStatus >( status_ ) );
    for( T_TreatmentCapacityVector::const_iterator it = capacities_.begin(); it != capacities_.end(); ++it )
        it->second.Send( *message.mutable_medical_treatment()->add_bed_capacities() );
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::Send
// Created: RFT 2008-06-09
// -----------------------------------------------------------------------------
bool MedicalTreatmentAttribute::SendUpdate( sword::ObjectAttributes& asn ) const
{
    if( NeedUpdate( eOnUpdate ) )
    {
        SendFullState( asn );
        Reset( eOnUpdate );
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::WriteODB
// Created: RFT 2008-06-09
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::WriteODB( xml::xostream& xos ) const
{
    xos << xml::start( "medical-treatement" );

    for( T_TreatmentCapacityVector::const_iterator it = capacities_.begin(); it != capacities_.end(); ++it )
    {
        xos << xml::start( "bed-capacity" )
                << xml::attribute( "type" , it->second.type_->GetName() )
                << xml::attribute( "baseline" , it->second.baseline_ )
            << xml::end;
    }
    xos << xml::end();
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::RegisterPatients
// Created: JCR 2010-06-05
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::RegisterPatients( unsigned int injuryID, unsigned int category, int n )
{
    if( capacities_.find( injuryID ) == capacities_.end() )
        throw std::runtime_error( std::string( __FUNCTION__  )+ " Unknown injury id: " + boost::lexical_cast< std::string >( injuryID ) );
    if( capacities_[ injuryID ].occupied_.size() < category )
        throw std::runtime_error( std::string( __FUNCTION__ ) + " Unknown category id: " + boost::lexical_cast< std::string >( category ) );
    capacities_[ injuryID ].occupied_[ category ] += n;
    NotifyAttributeUpdated( eOnUpdate );
}

// -----------------------------------------------------------------------------
// Name: OccupancyAttribute::CanTreatPatient
// Created: RFT 2008-06-09
// -----------------------------------------------------------------------------
bool MedicalTreatmentAttribute::CanTreatPatient( unsigned int injuryID ) const
{
    T_TreatmentCapacityVector::const_iterator it = capacities_.find( injuryID );
    if( it == capacities_.end() )
        return false;
    const MedicalTreatmentAttribute::MedicalCapacity& capacity = it->second;
    return ( capacity.baseline_ - std::accumulate( capacity.occupied_.begin(), capacity.occupied_.end(), 0 ) ) > 0;
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::Update
// Created: JCR 2010-06-05
// -----------------------------------------------------------------------------
unsigned int MedicalTreatmentAttribute::MedicalCapacity::Update( unsigned int doctors, float delay )
{
    int i = 0;
    for( InjuryCategory::iterator it = occupied_.begin(); type_ && it != occupied_.end(); ++it )
    {
        float rmfactor = delay * type_->GetHospitalisationTime( i++ ) * doctors;
        *it -= static_cast< unsigned int >( ( *it ) * rmfactor );
    }
    return doctors;
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::Update
// Created: JCR 2010-06-05
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::Update( float delay )
{
    std::map< int, unsigned int > lockedDoctors;
    unsigned int size = static_cast< unsigned >( capacities_.size() );
    unsigned int doctorPerCapacity = doctors_ / size;
    int remain = doctors_ % size;
    for( T_TreatmentCapacityVector::const_iterator it = capacities_.begin(); it != capacities_.end(); ++it )
        lockedDoctors[it->first] = ( --remain >= 0 ) ? doctorPerCapacity : doctorPerCapacity + 1;
    for( T_TreatmentCapacityVector::iterator it = capacities_.begin(); it != capacities_.end(); ++it )
        it->second.Update( lockedDoctors[it->first], delay );
    NotifyAttributeUpdated( eOnUpdate );
}

// -----------------------------------------------------------------------------
// Name: OccupancyAttribute::GetDoctors
// Created: RFT 2008-06-09
// -----------------------------------------------------------------------------
int MedicalTreatmentAttribute::GetDoctors() const
{
    return doctors_;
}

// -----------------------------------------------------------------------------
// Name: OccupancyAttribute::GetAvailableDoctors
// Created: RFT 2008-06-09
// -----------------------------------------------------------------------------
int MedicalTreatmentAttribute::GetAvailableDoctors() const
{
    return availableDoctors_;
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::Update
// Created: NLD 2010-10-26
// -----------------------------------------------------------------------------
bool MedicalTreatmentAttribute::Update( const MedicalTreatmentAttribute& rhs )
{
    if( medicalTreatmentMap_ != rhs.medicalTreatmentMap_ )
    {
        NotifyAttributeUpdated( eOnUpdate );
        medicalTreatmentMap_ = rhs.medicalTreatmentMap_;
    }
    if( doctors_ != rhs.doctors_ )
    {
        NotifyAttributeUpdated( eOnUpdate );
        doctors_ = rhs.doctors_;
    }
    if( availableDoctors_ != rhs.availableDoctors_ )
    {
        NotifyAttributeUpdated( eOnUpdate );
        availableDoctors_ = rhs.availableDoctors_;
    }
    if( initialDoctors_ != rhs.initialDoctors_ )
    {
        NotifyAttributeUpdated( eOnUpdate );
        initialDoctors_ = rhs.initialDoctors_;
    }
    if( capacities_ != rhs.capacities_ )
    {
        NotifyAttributeUpdated( eOnUpdate );
        capacities_ = rhs.capacities_;
    }
    if( referenceID_ != rhs.referenceID_ )
    {
        NotifyAttributeUpdated( eOnUpdate );
        referenceID_ = rhs.referenceID_;
    }
    if( status_ != rhs.status_ )
    {
        NotifyAttributeUpdated( eOnUpdate );
        status_ = rhs.status_;
    }
    return NeedUpdate( eOnUpdate );
}
