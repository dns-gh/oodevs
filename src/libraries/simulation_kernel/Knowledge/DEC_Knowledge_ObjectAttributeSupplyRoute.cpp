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
#include "protocol/protocol.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Knowledge_ObjectAttributeSupplyRoute )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeSupplyRoute constructor
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectAttributeSupplyRoute::DEC_Knowledge_ObjectAttributeSupplyRoute()
    : attr_            ( 0 )
    , bEquipped_       ( false )
    , rWeightSupported_( 0. )
    , rWidth_          ( 0. )
    , rLength_         ( 0. )
    , rFlow_           ( 0. )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeSupplyRoute constructor
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectAttributeSupplyRoute::DEC_Knowledge_ObjectAttributeSupplyRoute( const SupplyRouteAttribute& attr )
    : attr_            ( &attr )
    , bEquipped_       ( false )
    , rWeightSupported_( 0. )
    , rWidth_          ( 0. )
    , rLength_         ( 0. )
    , rFlow_           ( 0. )
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

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeSupplyRoute::serialize
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
template< typename Archive >
void DEC_Knowledge_ObjectAttributeSupplyRoute::serialize( Archive& file, const unsigned int )
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

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeSupplyRoute::UpdateAttributes
// Created: NLD 2004-10-29
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeSupplyRoute::UpdateAttributes( bool bRecognized )
{
    if( !attr_ )
        return;
    bEquipped_ = attr_->IsEquipped();
    if( bRecognized )
    {
        rWeightSupported_ = attr_->GetMaxWeight();
        rWidth_ = attr_->GetWidth();
        rLength_ = attr_->GetLength();
        rFlow_ = attr_->GetFlow();
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeSupplyRoute::UpdateOnPerceptionLevel
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeSupplyRoute::UpdateOnPerceptionLevel( const PHY_PerceptionLevel& /*currentPerceptionLevel*/ )
{
    UpdateAttributes( false );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeSupplyRoute::UpdateOnPerception
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeSupplyRoute::UpdateOnPerception( const DEC_Knowledge_ObjectPerception& /*perception*/ )
{
//    UpdateAttributes( perception.GetObjectPerceived().IsReconBy( perception.GetAgentPerceiving().GetType() ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeSupplyRoute::UpdateOnCollision
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeSupplyRoute::UpdateOnCollision( const DEC_Knowledge_ObjectCollision& /*collision*/ )
{
//    UpdateAttributes( collision.GetObject().IsReconBy( collision.GetAgentColliding().GetType() ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeSupplyRoute::Send
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeSupplyRoute::Send( Common::MsgObjectAttributes& asn ) const
{
    asn.mutable_supply_route()->set_equipped( bEquipped_ );
    asn.mutable_supply_route()->set_max_weight( static_cast< int >( rWeightSupported_ ) );
    asn.mutable_supply_route()->set_width( static_cast< int >( rWidth_ ) );
    asn.mutable_supply_route()->set_length( static_cast< int >( rLength_ ) );
    asn.mutable_supply_route()->set_flow_rate( static_cast< int >( rFlow_ ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeSupplyRoute::IsEquipped
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
bool DEC_Knowledge_ObjectAttributeSupplyRoute::IsEquipped() const
{
    return bEquipped_;
}
