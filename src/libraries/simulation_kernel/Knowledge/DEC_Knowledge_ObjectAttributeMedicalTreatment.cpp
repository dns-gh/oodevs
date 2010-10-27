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
#include "protocol/protocol.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Knowledge_ObjectAttributeMedicalTreatment )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeMedicalTreatment constructor
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectAttributeMedicalTreatment::DEC_Knowledge_ObjectAttributeMedicalTreatment()
    : attr_            ( 0 )
    , availableDoctors_( 0 )
    , doctors_         ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeMedicalTreatment constructor
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectAttributeMedicalTreatment::DEC_Knowledge_ObjectAttributeMedicalTreatment( const MedicalTreatmentAttribute& attr )
    : attr_            ( &attr )
    , availableDoctors_( 0 )
    , doctors_         ( 0 )
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
void DEC_Knowledge_ObjectAttributeMedicalTreatment::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< DEC_Knowledge_ObjectAttribute_ABC >( *this );
    file >> const_cast< MedicalTreatmentAttribute*& >( attr_ );
    /* file >> availableDoctors_
         >> doctors_
    	 >> referenceID_
         >> status_; */
//    file >> capacities_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectMedicalTreatment::save
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeMedicalTreatment::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< DEC_Knowledge_ObjectAttribute_ABC >( *this )
         << const_cast< MedicalTreatmentAttribute*& >( attr_ );
         /* << availableDoctors_
         << doctors_
         << referenceID_
		 << status_;
*/
//    file << capacities_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeMedicalTreatment::Register
// Created: JCR 2008-08-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeMedicalTreatment::Register( DEC_Knowledge_Object& knObject )
{
    knObject.AttachExtension( *this );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeMedicalTreatment::UpdateSpecificAttributes
// Created: NLD 2004-10-29
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeMedicalTreatment::UpdateAttributes()
{
    // JCR 2010-10-07: Automatically updated through the real object attributes.

/*
    if( !attr_ )
        return;
    availableDoctors_ = attr_->GetAvailableDoctors();
    doctors_ = attr_->GetDoctors();
 */  
    //Then we fill it by recovering the ids from the attribute
    // for( MedicalTreatmentAttribute::CIT_MedicalTreatmentMap iter = attr_->GetMap().begin() ; iter != attr_->GetMap().end() ; ++iter )
    //    medicalTreatmentList_.push_back( MIL_MedicalTreatmentType::Find( iter->first )->GetID() );
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
    UpdateAttributes();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeMedicalTreatment::BuildMsgSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeMedicalTreatment::Send( Common::ObjectAttributes& message ) const
{
 	if( attr_ )
        attr_->SendFullState( message );
}
