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
#include "Object.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/DEC_Knowledge_ObjectAttributeMedicalTreatment.h"
#include "MIL.h"
#include "MIL_AgentServer.h"
#include "MIL_MedicalTreatmentType.h"
#include "protocol/protocol.h"
#include <xeumeuleu/xml.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>
#include <numeric>


BOOST_CLASS_EXPORT_IMPLEMENT( MedicalTreatmentAttribute )

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute constructor
// Created: RFT 2008-05-30
// -----------------------------------------------------------------------------
MedicalTreatmentAttribute::MedicalTreatmentAttribute()
    : capacities_   ( MIL_MedicalTreatmentType::RegisteredCount() )
    , status_       ( Common::ObjectAttributeMedicalTreatment::normal )
    , doctors_      ( 0 )
    , availableDoctors_ ( 0 )
    , initialDoctors_ ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::MedicalTreatmentAttribute
// Created: RFT 2008-06-05
// -----------------------------------------------------------------------------
MedicalTreatmentAttribute::MedicalTreatmentAttribute( xml::xistream& xis )
    : capacities_   ( MIL_MedicalTreatmentType::RegisteredCount() )
    , status_       ( Common::ObjectAttributeMedicalTreatment::normal )
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
    capacity.baseline_ = xml::attribute< unsigned >( xis, "baseline" );
    // capacity.occupied_ = xml::attribute< unsigned >( xis, "occupied" );
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::MedicalTreatmentAttribute
// Created: RFT 2008-06-05
// -----------------------------------------------------------------------------
MedicalTreatmentAttribute::MedicalTreatmentAttribute( const Common::MsgMissionParameter_Value& attributes )
    : doctors_          ( attributes.list( eDoctors ).quantity() ) // Doctors
    , availableDoctors_ ( 0 )
    , initialDoctors_   ( 0 )
    , referenceID_      ( attributes.list( eExternalReferenceId ).acharstr() ) // ExternalReferenceId
    , capacities_       ( MIL_MedicalTreatmentType::RegisteredCount() )
    , status_           ( Common::ObjectAttributeMedicalTreatment::normal )
{
    const Common::MsgMissionParameter_Value& capacities = attributes.list( eBedCapacities );
    for( int i = 0; i < capacities.list_size(); ++i )
    {
        const Common::MsgMissionParameter_Value& value = capacities.list( i );
        const MIL_MedicalTreatmentType* pType = MIL_MedicalTreatmentType::Find( value.list( eTypeId ).identifier() );
        if( !pType )
            throw std::runtime_error( "Unknown Medical treatment type for medical treatment attribute" );
        MedicalCapacity& capacity = capacities_[ pType->GetID() ];
        
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
    object.Attach( *new DEC_Knowledge_ObjectAttributeMedicalTreatment() );
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::Update
// Created: JCR 2010-06-05
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::Register( MIL_Object_ABC& object ) const
{
    object.SetAttribute< MedicalTreatmentAttribute, MedicalTreatmentAttribute >( *this );
}

/* -----------------------------------------------------------------------------
// MedicalTreatmentAttribute::MedicalCapacity
// ---------------------------------------------------------------------------*/

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
void MedicalTreatmentAttribute::MedicalCapacity::Update( const Common::ObjectAttributeMedicalTreatmentBedCapacity& capacity )
{
    if( capacity.has_type_id() )
        type_ = MIL_MedicalTreatmentType::Find( capacity.type_id() );
    if ( type_ )
    {
        if( capacity.has_baseline_count() )
            baseline_ = capacity.baseline_count();
        if( capacity.has_available_count() ) // UA
            occupied_[ 1 ] = static_cast< unsigned int >( std::max( 0, (int)baseline_ - (int)capacity.available_count() ) );
        if( capacity.has_emergency_count() )
            emergency_ = capacity.emergency_count();
    }
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::Update
// Created: JCR 2010-10-08
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::MedicalCapacity::Update( const Common::MsgMissionParameter_Value& capacity )
{
    if( capacity.list( eTypeId ).has_identifier() )
        type_ = MIL_MedicalTreatmentType::Find( capacity.list( eTypeId ).identifier()  );
    if ( type_ )
    {
        if( capacity.list( eBaseLineCount ).has_quantity() )
            baseline_ = capacity.list( eBaseLineCount ).quantity();
        if( capacity.list( eAvailableCount ).has_quantity() ) // UA
            occupied_[ 1 ] = static_cast< unsigned int >( std::max( 0, (int)baseline_ - (int)capacity.list( eAvailableCount ).quantity() ) );
        if( capacity.list( eEmergencyCount ).has_quantity() )
            emergency_ = capacity.list( eEmergencyCount ).quantity();
    }
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::Send
// Created: JCR 2010-06-05
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::MedicalCapacity::Send( Common::ObjectAttributeMedicalTreatmentBedCapacity& capacity ) const
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
void MedicalTreatmentAttribute::OnUpdate( const Common::MsgMissionParameter_Value& parameters )
{
    // JCR TODO : Check if value is available ?
    if ( parameters.list( eDoctors ).has_quantity() )
        doctors_ = parameters.list( eDoctors ).quantity(); // 
    
    // JCR TODO : Check if value is available ?
    if ( parameters.list( eStatus ).has_enumeration() )
        status_ = parameters.list( eStatus ).enumeration(); 

    const Common::MsgMissionParameter_Value& capacities = parameters.list( eBedCapacities );
    for( int i = 0; i < capacities.list_size(); ++i )
    {
        const Common::MsgMissionParameter_Value& value = capacities.list( i );

        if( value.list( eTypeId ).has_identifier() ) 
        {
            const unsigned int typeId = value.list( eTypeId ).identifier();
            if( capacities_.size() <= typeId )
                throw std::runtime_error( std::string( __FUNCTION__  )+ " Unknown injury id: " + boost::lexical_cast< std::string >( typeId ) );
            capacities_[ typeId ].Update( value );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::Update
// Created: JCR 2010-06-04
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::Update( const Common::ObjectAttributeMedicalTreatment& message )
{
    if ( message.has_doctors() )
        doctors_ = message.doctors();
    if ( message.has_available_doctors() )
        availableDoctors_ = message.available_doctors();
    else
        availableDoctors_ = doctors_;
    if ( message.has_external_reference_id() )
        referenceID_ = message.external_reference_id();
    if( message.has_facility_status() )
        status_ = message.facility_status();
    int size = static_cast< int >( capacities_.size() );
    if( message.bed_capacities_size() > size )
        capacities_.resize( message.bed_capacities_size() );
    for( int i = 0 ; i < message.bed_capacities_size(); ++i )
	{
        const Common::ObjectAttributeMedicalTreatmentBedCapacity& bed_capacity = message.bed_capacities( i );
        if( bed_capacity.has_type_id() )
        {
		    if( capacities_.size() <= static_cast< unsigned int >( bed_capacity.type_id() ) )
	            throw std::runtime_error( std::string( __FUNCTION__  )+ " Unknown injury id: " + boost::lexical_cast< std::string >( bed_capacity.type_id() ) );
            capacities_[ bed_capacity.type_id() ].Update( bed_capacity );
        }
    }

    NotifyAttributeUpdated( eOnUpdate );
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::SendFullState
// Created: RFT 2008-06-18
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::SendFullState( Common::ObjectAttributes& message ) const
{ 
    message.mutable_medical_treatment()->set_doctors                ( doctors_ );
    message.mutable_medical_treatment()->set_available_doctors      ( availableDoctors_ );
    message.mutable_medical_treatment()->set_external_reference_id  ( referenceID_ );
    message.mutable_medical_treatment()->set_facility_status        ( static_cast< Common::ObjectAttributeMedicalTreatment_EnumMedicalTreatmentStatus >( status_ ) );
    
    if( capacities_.size() > 0 )
    {
        for ( T_TreatmentCapacityVector::const_iterator it = capacities_.begin(); it != capacities_.end(); ++it )
            it->Send( *message.mutable_medical_treatment()->add_bed_capacities() );
        if ( message.medical_treatment().bed_capacities().size() != capacities_.size() )
            throw std::runtime_error( std::string( __FUNCTION__  )+ " Medical treatment not properly encoded" );
    }
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::Send
// Created: RFT 2008-06-09
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::SendUpdate( Common::ObjectAttributes& asn ) const
{
    if( NeedUpdate() )
    {
        SendFullState( asn );
        Reset();
    }
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::WriteODB
// Created: RFT 2008-06-09
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::WriteODB( xml::xostream& xos ) const
{
    xos << xml::start( "medical-treatement" );
        for( unsigned i = 0; i < capacities_.size(); ++i )
        {
            xos << xml::start( "bed-capacity" )
                    << xml::attribute( "type" , capacities_[ i ].type_->GetName() )
                    // << xml::attribute( "occupied" , capacities_[ i ].occupied_ )
                    << xml::attribute( "baseline" , capacities_[ i ].baseline_ )
                << xml::end;
        }
    xos << xml::end();
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::RegisterPatients
// Created: JCR 2010-06-05
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::RegisterPatients( unsigned injuryID, unsigned category, unsigned n )
{
    if( capacities_.size() <= injuryID )
        throw std::runtime_error( std::string( __FUNCTION__  )+ " Unknown injury id: " + boost::lexical_cast< std::string >( injuryID ) );
    if( capacities_[ injuryID ].occupied_.size() < category )
        throw std::runtime_error( std::string( __FUNCTION__ ) + " Unknown category id: " + boost::lexical_cast< std::string >( category ) );
    capacities_[ injuryID ].occupied_[ category ] += n;
    NotifyAttributeUpdated( 1 );
}

// -----------------------------------------------------------------------------
// Name: OccupancyAttribute::CanTreatPatient
// Created: RFT 2008-06-09
// -----------------------------------------------------------------------------
bool MedicalTreatmentAttribute::CanTreatPatient( unsigned injuryID ) const
{
    if( capacities_.size() <= injuryID )
        throw std::runtime_error( std::string( __FUNCTION__ ) + " Unknown injury id: " + boost::lexical_cast< std::string >( injuryID ) );
    return ( capacities_[ injuryID ].baseline_ - std::accumulate( capacities_[ injuryID ].occupied_.begin(), capacities_[ injuryID ].occupied_.end(), 0 ) ) > 0;
}

namespace
{
    // Uniform distribution
    class AssignDoctors
    {
    public:
        AssignDoctors( int doctors, int size ) : doctors_( doctors ), size_ ( size ) {}

        unsigned operator()( const MedicalTreatmentAttribute::MedicalCapacity& capacity )
        {
            unsigned extract = 0;
            if( capacity.baseline_ > 0 )
            {
                extract = doctors_ / size_;
                doctors_ -= extract;
            }
            --size_;
            return extract;
        }
    private:
        int doctors_;
        int size_;
    };
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::Update
// Created: JCR 2010-06-05
// -----------------------------------------------------------------------------
unsigned MedicalTreatmentAttribute::MedicalCapacity::Update( unsigned doctors, float delay )
{
    int i = 0;
    for ( InjuryCategory::iterator it = occupied_.begin(); type_ && it != occupied_.end(); ++it )
    {
        float rmfactor = delay * type_->GetHospitalisationTime( i++ ) * (float)doctors;
        *it -= static_cast< unsigned >( float( *it ) * rmfactor );
    }
    return doctors;
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::Update
// Created: JCR 2010-06-05
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::Update( float delay )
{
    std::vector< unsigned > lockedDoctors( capacities_.size(), 0 );

    std::transform( capacities_.begin(), capacities_.end(), lockedDoctors.begin(), AssignDoctors( doctors_, capacities_.size() ) );
    std::transform( capacities_.begin(), capacities_.end(), lockedDoctors.begin(), lockedDoctors.begin(),
        boost::bind( &MedicalTreatmentAttribute::MedicalCapacity::Update, _1, _2, delay ) );
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
