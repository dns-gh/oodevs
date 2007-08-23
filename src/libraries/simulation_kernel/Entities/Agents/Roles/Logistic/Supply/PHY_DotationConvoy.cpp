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

#include "simulation_kernel_pch.h"

#include "PHY_DotationConvoy.h"
#include "PHY_SupplyDotationConsign.h"
#include "PHY_Conveyor.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "xeumeuleu/xml.h"

using namespace xml;

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
uint PHY_DotationConvoy::GetTravelTime( const MT_Vector2D& startPos, const MT_Vector2D& endPos ) const
{
    uint nTravelTime = 0;

    for ( CIT_ConveyorMap it = conveyors_.begin(); it != conveyors_.end(); ++it )
        nTravelTime = std::max( nTravelTime, it->second->ApproximateTravelTime( startPos, endPos ) );

    return nTravelTime;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationConvoy::GetTravelTimeToLoadingPoint
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
uint PHY_DotationConvoy::GetTravelTimeToLoadingPoint() const
{
    assert( pConsign_ );
    const MT_Vector2D& formingPoint = pConsign_->GetConvoyingAutomate().GetPionPC().GetRole< PHY_RolePion_Location >().GetPosition();
    const MT_Vector2D& loadingPoint = pConsign_->GetSupplyingAutomate().GetPionPC().GetRole< PHY_RolePion_Location >().GetPosition();

    return GetTravelTime( formingPoint, loadingPoint );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationConvoy::GetTravelTimeToUnloadingPoint
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
uint PHY_DotationConvoy::GetTravelTimeToUnloadingPoint() const
{
    assert( pConsign_ );
    const MT_Vector2D& loadingPoint   = pConsign_->GetSupplyingAutomate().GetPionPC().GetRole< PHY_RolePion_Location >().GetPosition();
    const MT_Vector2D& unloadingPoint = pConsign_->GetSuppliedAutomate ().GetPionPC().GetRole< PHY_RolePion_Location >().GetPosition();

    return GetTravelTime( loadingPoint, unloadingPoint );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationConvoy::GetTravelTimeToFormingPoint
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
uint PHY_DotationConvoy::GetTravelTimeToFormingPoint() const
{
    assert( pConsign_ );
    const MT_Vector2D& formingPoint   = pConsign_->GetConvoyingAutomate().GetPionPC().GetRole< PHY_RolePion_Location >().GetPosition();
    const MT_Vector2D& unloadingPoint = pConsign_->GetSuppliedAutomate ().GetPionPC().GetRole< PHY_RolePion_Location >().GetPosition();

    return GetTravelTime( unloadingPoint, formingPoint );
}

