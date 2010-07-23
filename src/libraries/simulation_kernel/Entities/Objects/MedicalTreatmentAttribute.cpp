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
#include "MIL_Random.h"
#include "MIL_MedicalTreatmentType.h"
#include "protocol/protocol.h"
#include <xeumeuleu/xml.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( MedicalTreatmentAttribute )

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute constructor
// Created: RFT 2008-05-30
// -----------------------------------------------------------------------------
MedicalTreatmentAttribute::MedicalTreatmentAttribute()
    : medicalTreatmentMap_()
    , beds_               ( 0 )
    , availableBeds_      ( 0 )
    , doctors_            ( 0 )
    , availableDoctors_   ( 0 )
    , initialBeds_        ( 0 )
    , initialDoctors_     ( 0 )

{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::MedicalTreatmentAttribute
// Created: RFT 2008-06-05
// -----------------------------------------------------------------------------
MedicalTreatmentAttribute::MedicalTreatmentAttribute( xml::xistream& xis )
    : medicalTreatmentMap_()
{
    std::string typeName( xis.attribute< std::string >( "medical-treatment", std::string() ) );
    const MIL_MedicalTreatmentType* pType = MIL_MedicalTreatmentType::Find( typeName );
    if( !pType )
        xis.error( "Unknown 'Medical treatment type' '" + typeName + "' for medical treatment attribute" );
    medicalTreatmentMap_.insert( std::make_pair( pType->GetID(), new T_PatientDiagnosisList() ) );
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::MedicalTreatmentAttribute
// Created: RFT 2008-06-05
// -----------------------------------------------------------------------------
MedicalTreatmentAttribute::MedicalTreatmentAttribute( const Common::MsgMissionParameter_Value& attributes )
    : beds_             ( attributes.list( 2 ).quantity() )
    , availableBeds_    ( attributes.list( 3 ).quantity() )
    , doctors_          ( attributes.list( 4 ).quantity() )
    , availableDoctors_ ( attributes.list( 5 ).quantity() )
    , initialBeds_      ( attributes.list( 2 ).quantity() )
    , initialDoctors_   ( attributes.list( 4 ).quantity() )
{
    const Common::MsgMissionParameter_Value& treatments = attributes.list( 1 );
    for( int i = 0; i < treatments.list_size(); ++i )
    {
        const MIL_MedicalTreatmentType* pType = MIL_MedicalTreatmentType::Find( treatments.list( i ).identifier() );
        if( !pType )
            throw std::runtime_error( "Unknown Medical treatment type for medical treatment attribute" );
        medicalTreatmentMap_.insert( std::make_pair( pType->GetID(), new T_PatientDiagnosisList() ) );
    }
    InitializePatientDiagnosisList( beds_ - availableBeds_ , doctors_ - availableDoctors_ );
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
// Name: MedicalTreatmentAttribute InitializePatientDiagnosisList
// Created: RFT 2008-05-30
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::InitializePatientDiagnosisList( int occupiedBeds , int occupiedDoctors )
{
    IT_MedicalTreatmentMap it           = medicalTreatmentMap_.begin();
    IT_PatientDiagnosisList iter        = it->second->begin();
    float time = float( MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() );

    for( int i = 0 ; i < occupiedDoctors ; i++ )
    {
        it->second->push_back( std::make_pair( time - ( float ) MIL_Random::rand_ii( 0 , MIL_MedicalTreatmentType::Find( it->first )->GetTreatmentTime( iter->second ) ) , ( int ) MIL_Random::rand32_ii( 0 , 1 ) ) );
    }

    for( int i = 0 ; i < occupiedBeds - occupiedDoctors ; i++ )
    {
        it->second->push_back( std::make_pair( time - ( float ) MIL_Random::rand_ii( 0 , MIL_MedicalTreatmentType::Find( it->first )->GetHospitalisationTime( iter->second ) ) , ( int ) MIL_Random::rand32_ii( 0 , 1 ) ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::operator=
// Created: RFT 2008-05-30
// -----------------------------------------------------------------------------
MedicalTreatmentAttribute& MedicalTreatmentAttribute::operator=( const MedicalTreatmentAttribute& rhs )
{
    medicalTreatmentMap_ = rhs.medicalTreatmentMap_;
    beds_                = rhs.beds_;
    availableBeds_       = rhs.availableBeds_;
    doctors_             = rhs.doctors_;
    availableDoctors_    = rhs.availableDoctors_;
    initialBeds_         = rhs.initialBeds_;
    initialDoctors_      = rhs.initialDoctors_;

    return *this;
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::load
// Created: RFT 2008-07-03
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::load( MIL_CheckPointInArchive& ar, const unsigned int )
{
    int typeID;
    int sizeOfList, injuryCategory, sizeOfMap;
    float time;

    ar >> boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    ar >> sizeOfMap;
    for ( int i = 0 ; i < sizeOfMap ; i++ )
    {
        ar >> typeID;
        ar >> sizeOfList;

        T_PatientDiagnosisList *list = new T_PatientDiagnosisList( sizeOfList );

        const MIL_MedicalTreatmentType* pType = MIL_MedicalTreatmentType::Find( typeID );
        if( !pType )
            throw std::runtime_error( "There is at least n unknown 'Medical treatment type' for medical treatment attribute" );
        for ( int j = 0 ; j < sizeOfList ; j ++ )
        {
            ar >> time;
            ar >> injuryCategory;
            list->push_back( std::make_pair( time, injuryCategory ) );
        }
        medicalTreatmentMap_.insert( std::make_pair( typeID, list ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::save
// Created: RFT 2008-07-03
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::save( MIL_CheckPointOutArchive& ar, const unsigned int ) const
{
    int sizeOfMap  = medicalTreatmentMap_.size();
    int sizeOfList = 0;
    ar << boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    ar << sizeOfMap;
    for( CIT_MedicalTreatmentMap it = medicalTreatmentMap_.begin(); it != medicalTreatmentMap_.end(); ++it )
    {
        ar << it->first;
        sizeOfList = it->second->size();
        ar << sizeOfList;
        for( CIT_PatientDiagnosisList iter = it->second->begin() ; iter != it->second->end() ; ++iter )
        {
            ar << iter->first;
            ar << iter->second;
        }
    }
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
// Name: MedicalTreatmentAttribute::Register
// Created: JSR 2010-03-15
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::Register( MIL_Object_ABC& object ) const
{
    object.SetAttribute< MedicalTreatmentAttribute, MedicalTreatmentAttribute >( *this );
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::SendFullState
// Created: RFT 2008-06-18
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::SendFullState( Common::MsgObjectAttributes& asn ) const
{
    asn.mutable_medical_treatment()->set_available_beds    ( availableBeds_ );
    asn.mutable_medical_treatment()->set_available_doctors ( availableDoctors_ );
    asn.mutable_medical_treatment()->set_beds              ( beds_ );
    asn.mutable_medical_treatment()->set_doctors           ( doctors_ );
    //Get the list of the ID of each medical treatment
    for( CIT_MedicalTreatmentMap iter = medicalTreatmentMap_.begin() ; iter != medicalTreatmentMap_.end() ; ++iter )
        asn.mutable_medical_treatment()->mutable_type_id()->add_elem(  MIL_MedicalTreatmentType::Find( iter->first )->GetID() );
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::Send
// Created: RFT 2008-06-09
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::SendUpdate( Common::MsgObjectAttributes& asn ) const
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
    xos << xml::start( "medical-traitement" );
        for( CIT_MedicalTreatmentMap it = medicalTreatmentMap_.begin(); it != medicalTreatmentMap_.end(); ++it )
        {
            xos << xml::content( "type" , it->first );
            xos << xml::attribute( "patients-data" , it->second );
        }
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::GetMap
// Created: RFT 2008-06-09
// -----------------------------------------------------------------------------
const MedicalTreatmentAttribute::T_MedicalTreatmentMap& MedicalTreatmentAttribute::GetMap() const
{
    return medicalTreatmentMap_;
}

// -----------------------------------------------------------------------------
// Name: OccupancyAttribute::FlagPatient
// Created: RFT 2008-06-09
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::FlagPatient( float time , int injuryID , int injuryCategory )
{
    IT_MedicalTreatmentMap it = medicalTreatmentMap_.find( injuryID );
    if( it != medicalTreatmentMap_.end() )
    {
        availableDoctors_ --;
        availableBeds_ --;
        it->second->push_back( std::make_pair( time, injuryCategory ));
    }
}

// -----------------------------------------------------------------------------
// Name: OccupancyAttribute::CanTreatPatient
// Created: RFT 2008-06-09
// -----------------------------------------------------------------------------
bool MedicalTreatmentAttribute::CanTreatPatient( int injuryID )
{
    IT_MedicalTreatmentMap it = medicalTreatmentMap_.find( injuryID );
    if( it != medicalTreatmentMap_.end() )
        return true;
    else
        return false;
}

// -----------------------------------------------------------------------------
// Name: OccupancyAttribute::FreeDoctors
// Created: RFT 2008-06-09
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::FreeDoctors( float time )
{
    // For each different type of injury the structure can take care of
    for( CIT_MedicalTreatmentMap it = medicalTreatmentMap_.begin(); it != medicalTreatmentMap_.end(); ++it )
    {
        for ( CIT_PatientDiagnosisList iter = it->second->begin() ; iter != it->second->end() ; ++iter )
        {
            if( time == iter->first + MIL_MedicalTreatmentType::Find( it->first )->GetTreatmentTime( iter->second ) )
                availableDoctors_ ++;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: OccupancyAttribute::FreeBeds
// Created: RFT 2008-06-09
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::FreeBeds( float time )
{
    // For each different type of injury the structure can take care of
    for( IT_MedicalTreatmentMap it = medicalTreatmentMap_.begin(); it != medicalTreatmentMap_.end(); ++it )
    {
        for ( IT_PatientDiagnosisList iter = it->second->begin() ; iter != it->second->end() ; )
        {
            if( time == iter->first + MIL_MedicalTreatmentType::Find( it->first )->GetHospitalisationTime( iter->second ) )
            {
                availableBeds_ ++;
                iter = it->second->erase( iter );
            }
            else
                ++iter;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::UpdateAvailableBeds
// Created: RFT 2008-05-22
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::UpdateAvailableBeds( bool bEmergencyPlan , float emergencyBedsRate )
{
    if( bEmergencyPlan ) //Emergency plan
    {
        availableBeds_ += ( int ) ( initialBeds_ * ( emergencyBedsRate - 1 ) );
        beds_           = ( int ) ( initialBeds_ * emergencyBedsRate );
    }

    else //Normal case
    {
        if( beds_ - initialBeds_ > availableBeds_ )//If the number of beds decreases more than the number of available beds
        {
            beds_         -= availableBeds_;
            availableBeds_ = 0;
        }
        else
        {
            availableBeds_ -= ( beds_ - initialBeds_ );//If the number of beds decreases less than the number of available beds
            beds_           = initialBeds_;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentAttribute::UpdateAvailableDoctors
// Created: RFT 2008-05-22
// -----------------------------------------------------------------------------
void MedicalTreatmentAttribute::UpdateAvailableDoctors( bool bEmergencyPlan , bool bBusinessHours , float emergencyDoctorsRate , float nightDoctorsRate )
{
    if( !bEmergencyPlan && bBusinessHours )//Normal case and Day
    {
        if( doctors_ - initialDoctors_ > availableDoctors_ )
        {
            doctors_         -= availableDoctors_;
            availableDoctors_ = 0;
        }
        else
        {
            availableDoctors_ -= ( doctors_ - initialDoctors_ );
            doctors_           = initialDoctors_;
        }
    }

    else if( !bEmergencyPlan && !bBusinessHours )//Normal case and Night
    {
        if( doctors_ - ( int ) ( initialDoctors_* nightDoctorsRate ) > availableDoctors_ )
        {
            doctors_         -= availableDoctors_;
            availableDoctors_ = 0;
        }
        else
        {
            availableDoctors_ -= ( int ) ( initialDoctors_* ( nightDoctorsRate - 1 ) );
            doctors_           = ( int ) ( initialDoctors_* nightDoctorsRate );
        }
    }

    else if( bEmergencyPlan )//Emergency plan
    {
        availableDoctors_ += ( int ) ( initialDoctors_ * ( emergencyDoctorsRate - 1 ) );
        doctors_           = ( int ) ( initialDoctors_ * emergencyDoctorsRate );
    }
}

// -----------------------------------------------------------------------------
// Name: OccupancyAttribute::GetBeds
// Created: RFT 2008-06-09
// -----------------------------------------------------------------------------
int MedicalTreatmentAttribute::GetBeds() const
{
    return beds_;
}

// -----------------------------------------------------------------------------
// Name: OccupancyAttribute::GetAvailableBeds
// Created: RFT 2008-06-09
// -----------------------------------------------------------------------------
int MedicalTreatmentAttribute::GetAvailableBeds() const
{
    return availableBeds_;
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
