// *****************************************************************************
//
// $Created: NLD 2004-05-04 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_ObjectCampPrisonniers.cpp $
// $Author: Nld $
// $Modtime: 27/04/05 15:11 $
// $Revision: 3 $
// $Workfile: DEC_Knowledge_ObjectCampPrisonniers.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Knowledge_ObjectCampPrisonniers.h"

#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Entities/Objects/MIL_CampPrisonniers.h"

BOOST_CLASS_EXPORT_GUID( DEC_Knowledge_ObjectCampPrisonniers, "DEC_Knowledge_ObjectCampPrisonniers" )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectCampPrisonniers constructor
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectCampPrisonniers::DEC_Knowledge_ObjectCampPrisonniers( const MIL_Army& armyKnowing, MIL_RealObject_ABC& objectKnown )
    : DEC_Knowledge_Object     ( armyKnowing, objectKnown )
    , pTC2_                    ( 0 )
    , bSpecificAttributesValid_( false )
{    
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectCampPrisonniers constructor
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectCampPrisonniers::DEC_Knowledge_ObjectCampPrisonniers()
    : DEC_Knowledge_Object     ()
    , pTC2_                    ( 0 )
    , bSpecificAttributesValid_( false )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectCampPrisonniers destructor
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectCampPrisonniers::~DEC_Knowledge_ObjectCampPrisonniers()
{    
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectCampPrisonniers::serialize
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
template< typename Archive >
void DEC_Knowledge_ObjectCampPrisonniers::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< DEC_Knowledge_Object >( *this );
    file & pTC2_
         & bSpecificAttributesValid_;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectCampPrisonniers::UpdateSpecificAttributes
// Created: NLD 2004-10-29
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectCampPrisonniers::UpdateSpecificAttributes()
{
    if( !GetObjectKnown() )
        return;

    if( !bSpecificAttributesValid_ )
    {
        bSpecificAttributesValid_ = true;

        const MIL_CampPrisonniers& object = static_cast< const MIL_CampPrisonniers& >( *GetObjectKnown() );

        pTC2_ = &object.GetTC2();
        NotifyAttributeUpdated( eAttr_Specific );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectCampPrisonniers::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectCampPrisonniers::UpdateSpecificAttributes( const PHY_PerceptionLevel& /*currentPerceptionLevel*/ )
{
    UpdateSpecificAttributes();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectCampPrisonniers::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectCampPrisonniers::UpdateSpecificAttributes( const DEC_Knowledge_ObjectPerception& /*perception*/ )
{
    UpdateSpecificAttributes();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectCampPrisonniers::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectCampPrisonniers::UpdateSpecificAttributes( const DEC_Knowledge_ObjectCollision& /*collision*/ )
{
    UpdateSpecificAttributes();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectCampPrisonniers::BuildMsgSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectCampPrisonniers::BuildMsgSpecificAttributes( ASN1T_MsgObjectKnowledgeUpdate& asnMsg )
{
    if( !IsAttributeUpdated( eAttr_Specific ) || !bSpecificAttributesValid_ )
        return;

    assert( pTC2_ );

    asnAttributes_.tc2 = pTC2_->GetID();

    asnMsg.m.specific_attributesPresent        = 1;
    asnMsg.specific_attributes.t               = T_ObjectAttributesSpecific_prisoner_camp;
    asnMsg.specific_attributes.u.prisoner_camp = &asnAttributes_;
}
