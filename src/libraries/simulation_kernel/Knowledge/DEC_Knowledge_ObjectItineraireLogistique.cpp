// *****************************************************************************
//
// $Created: NLD 2004-05-04 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_ObjectItineraireLogistique.cpp $
// $Author: Nld $
// $Modtime: 27/04/05 15:11 $
// $Revision: 3 $
// $Workfile: DEC_Knowledge_ObjectItineraireLogistique.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Knowledge_ObjectItineraireLogistique.h"

#include "DEC_Knowledge_ObjectPerception.h"
#include "DEC_Knowledge_ObjectCollision.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Objects/MIL_ItineraireLogistique.h"

BOOST_CLASS_EXPORT_GUID( DEC_Knowledge_ObjectItineraireLogistique, "DEC_Knowledge_ObjectItineraireLogistique" )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectItineraireLogistique constructor
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectItineraireLogistique::DEC_Knowledge_ObjectItineraireLogistique( const MIL_Army& armyKnowing, MIL_RealObject_ABC& objectKnown )
    : DEC_Knowledge_Object( armyKnowing, objectKnown )
    , bEquipped_          ( false )
    , rWeightSupported_   ( 0. )
    , rWidth_             ( 0. )
    , rLength_            ( 0. ) 
    , rFlow_              ( 0. )
{    
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectItineraireLogistique constructor
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectItineraireLogistique::DEC_Knowledge_ObjectItineraireLogistique()
    : DEC_Knowledge_Object()
    , bEquipped_          ( false )
    , rWeightSupported_   ( 0. )
    , rWidth_             ( 0. )
    , rLength_            ( 0. ) 
    , rFlow_              ( 0. )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectItineraireLogistique destructor
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectItineraireLogistique::~DEC_Knowledge_ObjectItineraireLogistique()
{    
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectItineraireLogistique::serialize
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
template< typename Archive >
void DEC_Knowledge_ObjectItineraireLogistique::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< DEC_Knowledge_Object >( *this );
    file & bEquipped_
         & rWeightSupported_
         & rWidth_
         & rLength_
         & rFlow_;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectItineraireLogistique::UpdateSpecificAttributes
// Created: NLD 2004-10-29
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectItineraireLogistique::UpdateSpecificAttributes( bool bRecognized )
{
    if( !GetObjectKnown() )
        return;

    const MIL_ItineraireLogistique& itineraireLogistique = static_cast< const MIL_ItineraireLogistique& >( *GetObjectKnown() );
    if( bEquipped_ != itineraireLogistique.IsEquipped() )
    {
        bEquipped_ = itineraireLogistique.IsEquipped();
        NotifyAttributeUpdated( eAttr_Specific );
    }

    if( bRecognized )
    {
        if( rWeightSupported_ != itineraireLogistique.GetWeightSupported() )
        {
            rWeightSupported_ = itineraireLogistique.GetWeightSupported();
            NotifyAttributeUpdated( eAttr_Specific );
        }
        if( rWidth_ != itineraireLogistique.GetWidth() )
        {
            rWidth_ = itineraireLogistique.GetWidth();
            NotifyAttributeUpdated( eAttr_Specific );
        }
        if( rLength_ != itineraireLogistique.GetLength() )
        {
            rLength_ = itineraireLogistique.GetLength();
            NotifyAttributeUpdated( eAttr_Specific );
        }
        if( rFlow_ != itineraireLogistique.GetFlow() )
        {
            rFlow_ = itineraireLogistique.GetFlow();
            NotifyAttributeUpdated( eAttr_Specific );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectItineraireLogistique::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectItineraireLogistique::UpdateSpecificAttributes( const PHY_PerceptionLevel& /*currentPerceptionLevel*/ )
{
    UpdateSpecificAttributes( false );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectItineraireLogistique::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectItineraireLogistique::UpdateSpecificAttributes( const DEC_Knowledge_ObjectPerception& perception )
{
    UpdateSpecificAttributes( IsReconBy( perception.GetAgentPerceiving().GetType() ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectItineraireLogistique::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectItineraireLogistique::UpdateSpecificAttributes( const DEC_Knowledge_ObjectCollision& collision )
{
    UpdateSpecificAttributes( IsReconBy( collision.GetAgentColliding().GetType() ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectItineraireLogistique::BuildMsgSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectItineraireLogistique::BuildMsgSpecificAttributes( ASN1T_MsgObjectKnowledgeUpdate& asnMsg )
{
    if( !IsAttributeUpdated( eAttr_Specific ) )
        return;

    asnAttributes_.equipped   = bEquipped_;
    asnAttributes_.max_weight = (int)rWeightSupported_;
    asnAttributes_.width      = (int)rWidth_;
    asnAttributes_.length     = (int)rLength_;
    asnAttributes_.flow_rate  = (int)rFlow_;

    asnMsg.m.specific_attributesPresent         = 1;
    asnMsg.specific_attributes.t                = T_ObjectAttributesSpecific_logistic_route;
    asnMsg.specific_attributes.u.logistic_route = &asnAttributes_;
}
