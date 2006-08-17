// *****************************************************************************
//
// $Created: NLD 2004-05-04 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_ObjectCampRefugies.cpp $
// $Author: Nld $
// $Modtime: 27/04/05 15:11 $
// $Revision: 3 $
// $Workfile: DEC_Knowledge_ObjectCampRefugies.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Knowledge_ObjectCampRefugies.h"

#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Entities/Objects/MIL_CampRefugies.h"

BOOST_CLASS_EXPORT_GUID( DEC_Knowledge_ObjectCampRefugies, "DEC_Knowledge_ObjectCampRefugies" )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectCampRefugies constructor
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectCampRefugies::DEC_Knowledge_ObjectCampRefugies( const MIL_Army& armyKnowing, MIL_RealObject_ABC& objectKnown )
    : DEC_Knowledge_Object     ( armyKnowing, objectKnown )
    , pTC2_                    ( 0 )
    , bSpecificAttributesValid_( false )
{    
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectCampRefugies constructor
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectCampRefugies::DEC_Knowledge_ObjectCampRefugies()
    : DEC_Knowledge_Object     ()
    , pTC2_                    ( 0 )
    , bSpecificAttributesValid_( false )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectCampRefugies destructor
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectCampRefugies::~DEC_Knowledge_ObjectCampRefugies()
{    
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectCampRefugies::serialize
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
template< typename Archive >
void DEC_Knowledge_ObjectCampRefugies::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< DEC_Knowledge_Object >( *this );
    file & pTC2_
         & bSpecificAttributesValid_;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectCampRefugies::UpdateSpecificAttributes
// Created: NLD 2004-10-29
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectCampRefugies::UpdateSpecificAttributes()
{
    if( !GetObjectKnown() )
        return;

    if( !bSpecificAttributesValid_ )
    {
        bSpecificAttributesValid_ = true;

        const MIL_CampRefugies& object = static_cast< const MIL_CampRefugies& >( *GetObjectKnown() );

        pTC2_ = &object.GetTC2();
        NotifyAttributeUpdated( eAttr_Specific );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectCampRefugies::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectCampRefugies::UpdateSpecificAttributes( const PHY_PerceptionLevel& /*currentPerceptionLevel*/ )
{
    UpdateSpecificAttributes();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectCampRefugies::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectCampRefugies::UpdateSpecificAttributes( const DEC_Knowledge_ObjectPerception& /*perception*/ )
{
    UpdateSpecificAttributes();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectCampRefugies::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectCampRefugies::UpdateSpecificAttributes( const DEC_Knowledge_ObjectCollision& /*collision*/ )
{
    UpdateSpecificAttributes();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectCampRefugies::BuildMsgSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectCampRefugies::BuildMsgSpecificAttributes( ASN1T_MsgObjectKnowledgeUpdate& asnMsg )
{
    if( !IsAttributeUpdated( eAttr_Specific ) || !bSpecificAttributesValid_ )
        return;

    assert( pTC2_ );

    asnAttributes_.tc2 = pTC2_->GetID();

    asnMsg.m.attributs_specifiquesPresent        = 1;
    asnMsg.attributs_specifiques.t               = T_AttrObjectSpecific_camp_refugies;
    asnMsg.attributs_specifiques.u.camp_refugies = &asnAttributes_;
}
