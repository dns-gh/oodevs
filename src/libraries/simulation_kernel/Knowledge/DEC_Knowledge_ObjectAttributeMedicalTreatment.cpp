// *****************************************************************************
//
// $Created: NLD 2004-05-04 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_ObjectAttributeMedicalTreatment.cpp $
// $Author: Nld $
// $Modtime: 27/04/05 15:11 $
// $Revision: 3 $
// $Workfile: DEC_Knowledge_ObjectAttributeMedicalTreatment.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Knowledge_ObjectAttributeMedicalTreatment.h"
#include "Entities/Objects/MIL_MedicalTreatmentType.h"
#include "Entities/Objects/MedicalTreatmentAttribute.h"
#include "DEC_Knowledge_Object.h"

#include "MIL.h"

BOOST_CLASS_EXPORT_GUID( DEC_Knowledge_ObjectAttributeMedicalTreatment, "DEC_Knowledge_ObjectAttributeMedicalTreatment" )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeMedicalTreatment constructor
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectAttributeMedicalTreatment::DEC_Knowledge_ObjectAttributeMedicalTreatment()
    : attr_                ( 0 )
    , medicalTreatmentList_()
    , availableBeds_       ( 0 )
    , availableDoctors_    ( 0 )
    , beds_                ( 0 )
    , doctors_             ( 0 ) 
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeMedicalTreatment constructor
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectAttributeMedicalTreatment::DEC_Knowledge_ObjectAttributeMedicalTreatment( const MedicalTreatmentAttribute& attr )
    : attr_                ( &attr )
    , medicalTreatmentList_()
    , availableBeds_       ( 0 )
    , availableDoctors_    ( 0 )
    , beds_                ( 0 )
    , doctors_             ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeMedicalTreatment destructor
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectAttributeMedicalTreatment::~DEC_Knowledge_ObjectAttributeMedicalTreatment()
{
    // NOTHING
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectMedicalTreatment::load
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeMedicalTreatment::load( MIL_CheckPointInArchive& file, const uint )
{
    int availableBeds, availableDoctors, beds, doctors, listSize, nID;
    file >> availableBeds
         >> availableDoctors
         >> beds
         >> doctors
         >> listSize;

    availableBeds_         = availableBeds;
    availableDoctors_      = availableDoctors;
    beds_                  = beds;
    doctors_               = doctors;

    //First we make sure the list containing all the medical treatment ids is empty
    for( IT_MedicalTreatmentTypeList it = medicalTreatmentList_.begin() ; it != medicalTreatmentList_.end() ; )
        it = medicalTreatmentList_.erase( it );

    //Then we fill it
    for( int i = 0 ; i < listSize ; i++ )
    {
        file >> nID;
        medicalTreatmentList_.push_back( nID );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectMedicalTreatment::save
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeMedicalTreatment::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    int nID;
    int listSize = medicalTreatmentList_.size();
    file << boost::serialization::base_object< DEC_Knowledge_ObjectAttribute_ABC >( *this )
         << availableBeds_
         << availableDoctors_
         << beds_
         << doctors_
         << listSize;

    for( CIT_MedicalTreatmentTypeList iter = medicalTreatmentList_.begin() ; iter != medicalTreatmentList_.end() ; ++iter )
    {
        nID = *iter;
        file << nID;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeMedicalTreatment::Register
// Created: JCR 2008-08-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeMedicalTreatment::Register( DEC_Knowledge_Object& knObject )
{
    knObject.AttachExtension( *this );
}


// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeMedicalTreatment::UpdateSpecificAttributes
// Created: NLD 2004-10-29
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeMedicalTreatment::UpdateAttributes()
{
    availableBeds_    = attr_->GetAvailableBeds();
    availableDoctors_ = attr_->GetAvailableDoctors();
    beds_             = attr_->GetBeds();
    doctors_          = attr_->GetDoctors();

    //First we empty the list containing all the medical treatment ids
    for( IT_MedicalTreatmentTypeList it = medicalTreatmentList_.begin() ; it != medicalTreatmentList_.end() ; )
        it = medicalTreatmentList_.erase( it );

    //Then we fill it by recovering the ids from the attribute
    for( MedicalTreatmentAttribute::CIT_MedicalTreatmentMap iter = attr_->GetMap().begin() ; iter != attr_->GetMap().end() ; ++iter )
        medicalTreatmentList_.push_back( MIL_MedicalTreatmentType::Find( iter->first )->GetID() );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeMedicalTreatment::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeMedicalTreatment::UpdateOnPerceptionLevel( const PHY_PerceptionLevel& /*currentPerceptionLevel*/ )
{
    UpdateAttributes();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeMedicalTreatment::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeMedicalTreatment::UpdateOnPerception( const DEC_Knowledge_ObjectPerception& /*perception*/ )
{
    UpdateAttributes();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeMedicalTreatment::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeMedicalTreatment::UpdateOnCollision( const DEC_Knowledge_ObjectCollision& /*collision*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeMedicalTreatment::BuildMsgSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeMedicalTreatment::Send( ASN1T_ObjectAttributes& asn ) const
{
    if ( attr_ )
    {
        int i = 0; //i is used to fill elem
        asn.m.medical_treatmentPresent = 1;
        asn.medical_treatment.available_beds    = availableBeds_;
        asn.medical_treatment.available_doctors = availableDoctors_;
        asn.medical_treatment.beds              = beds_;
        asn.medical_treatment.doctors           = doctors_;
        asn.medical_treatment.type_id.n         = attr_->GetMap().size();
        asn.medical_treatment.type_id.elem      = new ASN1T_OID[ asn.medical_treatment.type_id.n ];
        //Get the list of the ID of each medical treatment
        for( MedicalTreatmentAttribute::CIT_MedicalTreatmentMap iter = attr_->GetMap().begin() ; iter != attr_->GetMap().end() ; ++iter )
        {
            asn.medical_treatment.type_id.elem[ i ] = MIL_MedicalTreatmentType::Find( iter->first )->GetID();
            i++;
        }
    }
}
