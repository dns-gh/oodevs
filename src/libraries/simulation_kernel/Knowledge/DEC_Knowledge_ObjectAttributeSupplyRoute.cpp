// *****************************************************************************
//
// $Created: NLD 2004-05-04 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_ObjectAttributeSupplyRoute.cpp $
// $Author: Nld $
// $Modtime: 27/04/05 15:11 $
// $Revision: 3 $
// $Workfile: DEC_Knowledge_ObjectAttributeSupplyRoute.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Knowledge_ObjectAttributeSupplyRoute.h"
#include "DEC_Knowledge_ObjectPerception.h"
#include "DEC_Knowledge_ObjectCollision.h"
#include "DEC_Knowledge_Object.h"
#include "Entities/Objects/SupplyRouteAttribute.h"
#include "Entities/Objects/MIL_Object_ABC.h"

#include "MIL.h"

BOOST_CLASS_EXPORT_GUID( DEC_Knowledge_ObjectAttributeSupplyRoute, "DEC_Knowledge_ObjectAttributeSupplyRoute" )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeSupplyRoute constructor
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectAttributeSupplyRoute::DEC_Knowledge_ObjectAttributeSupplyRoute()
    : attr_               ( 0 )
    , bEquipped_          ( false )
    , rWeightSupported_   ( 0. )
    , rWidth_             ( 0. )
    , rLength_            ( 0. ) 
    , rFlow_              ( 0. )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeSupplyRoute constructor
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectAttributeSupplyRoute::DEC_Knowledge_ObjectAttributeSupplyRoute( const SupplyRouteAttribute& attr )
    : attr_               ( &attr )
    , bEquipped_          ( false )
    , rWeightSupported_   ( 0. )
    , rWidth_             ( 0. )
    , rLength_            ( 0. ) 
    , rFlow_              ( 0. )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeSupplyRoute destructor
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectAttributeSupplyRoute::~DEC_Knowledge_ObjectAttributeSupplyRoute()
{    
    // NOTHING
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeSupplyRoute::serialize
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
template< typename Archive >
void DEC_Knowledge_ObjectAttributeSupplyRoute::serialize( Archive& file, const uint )
{    
    file & boost::serialization::base_object< DEC_Knowledge_ObjectAttribute_ABC >( *this );
    file & const_cast< SupplyRouteAttribute*& >( attr_ ) 
         & bEquipped_
         & rWeightSupported_
         & rWidth_
         & rLength_
         & rFlow_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeSupplyRoute::Register
// Created: JCR 2008-08-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeSupplyRoute::Register( DEC_Knowledge_Object& knObject )
{
    knObject.AttachExtension( *this );
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeSupplyRoute::UpdateSpecificAttributes
// Created: NLD 2004-10-29
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeSupplyRoute::UpdateAttributes( bool bRecognized )
{
    if ( !attr_ )
        return;

    if( bEquipped_ != attr_->IsEquipped() )
    {
        bEquipped_ = attr_->IsEquipped();
        NotifyAttributeUpdated( eOnUpdate );
    }

    if( bRecognized )
    {
        if( rWeightSupported_ != attr_->GetMaxWeight() )
        {
            rWeightSupported_ = attr_->GetMaxWeight();
            NotifyAttributeUpdated( eOnUpdate );
        }
        if( rWidth_ != attr_->GetWidth() )
        {
            rWidth_ = attr_->GetWidth();
            NotifyAttributeUpdated( eOnUpdate );
        }
        if( rLength_ != attr_->GetLength() )
        {
            rLength_ = attr_->GetLength();
            NotifyAttributeUpdated( eOnUpdate );
        }
        if( rFlow_ != attr_->GetFlow() )
        {
            rFlow_ = attr_->GetFlow();
            NotifyAttributeUpdated( eOnUpdate );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeSupplyRoute::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeSupplyRoute::UpdateOnPerceptionLevel( const PHY_PerceptionLevel& currentPerceptionLevel )
{
    UpdateAttributes( false );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeSupplyRoute::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeSupplyRoute::UpdateOnPerception( const DEC_Knowledge_ObjectPerception& perception )
{
//    UpdateAttributes( perception.GetObjectPerceived().IsReconBy( perception.GetAgentPerceiving().GetType() ) );    
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeSupplyRoute::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeSupplyRoute::UpdateOnCollision( const DEC_Knowledge_ObjectCollision& collision )
{
//    UpdateAttributes( collision.GetObject().IsReconBy( collision.GetAgentColliding().GetType() ) );    
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeSupplyRoute::BuildMsgSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeSupplyRoute::Send( ASN1T_ObjectAttributes& asn ) const
{
    if ( NeedUpdate() )
    {
        // attr_->Send( asn );
        asn.m.supply_routePresent = 1;
        asn.supply_route.equipped   = bEquipped_;
        asn.supply_route.max_weight = (int)rWeightSupported_;
        asn.supply_route.width      = (int)rWidth_;
        asn.supply_route.length     = (int)rLength_;
        asn.supply_route.flow_rate  = (int)rFlow_;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeSupplyRoute::IsEquipped
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
bool DEC_Knowledge_ObjectAttributeSupplyRoute::IsEquipped() const
{
    return bEquipped_;
}
