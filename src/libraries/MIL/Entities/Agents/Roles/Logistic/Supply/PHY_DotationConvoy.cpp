// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Supply/PHY_DotationConvoy.cpp $
// $Author: Jvt $
// $Modtime: 11/04/05 17:27 $
// $Revision: 5 $
// $Workfile: PHY_DotationConvoy.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "PHY_DotationConvoy.h"
#include "PHY_SupplyDotationConsign.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"

BOOST_CLASS_EXPORT_GUID( PHY_DotationConvoy, "PHY_DotationConvoy" )

// -----------------------------------------------------------------------------
// Name: PHY_DotationConvoy constructor
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
PHY_DotationConvoy::PHY_DotationConvoy( PHY_SupplyDotationConsign& consign )
    : PHY_Convoy_ABC( consign )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationConvoy constructor
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
PHY_DotationConvoy::PHY_DotationConvoy()
    : PHY_Convoy_ABC()
{
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationConvoy destructor
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
PHY_DotationConvoy::~PHY_DotationConvoy()
{
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: PHY_DotationConvoy::serialize
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_DotationConvoy::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< PHY_Convoy_ABC >( *this );
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_DotationConvoy::GetTravelTime
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
uint PHY_DotationConvoy::GetTravelTime() const
{
    uint nTravelTime = 0;

    assert( pConsign_ && pConsign_->GetSuppliedAutomate() );
    const MT_Vector2D vStartPos = pConsign_->GetSupplyingAutomate() .GetAlivePionsBarycenter();
    const MT_Vector2D vEndPos   = pConsign_->GetSuppliedAutomate ()->GetAlivePionsBarycenter();

    for ( CIT_ConveyorMap it = conveyors_.begin(); it != conveyors_.end(); ++it )
        nTravelTime = std::max( nTravelTime, it->second->ApproximateTravelTime( vStartPos, vEndPos ) );

    return nTravelTime;
}
