// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/PHY_DotationConvoy.cpp $
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
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include <xeumeuleu/xml.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_DotationConvoy )

// -----------------------------------------------------------------------------
// Name: PHY_DotationConvoy constructor
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
PHY_DotationConvoy::PHY_DotationConvoy( PHY_SupplyDotationConsign& consign )
    : PHY_Convoy_ABC( consign )
    , pConsign_     ( &consign )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationConvoy constructor
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
PHY_DotationConvoy::PHY_DotationConvoy()
    : PHY_Convoy_ABC()
    , pConsign_     ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationConvoy destructor
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
PHY_DotationConvoy::~PHY_DotationConvoy()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationConvoy::GetTravelTime
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
unsigned int PHY_DotationConvoy::GetTravelTime( const MT_Vector2D& startPos, const MT_Vector2D& endPos ) const
{
    unsigned int nTravelTime = 0;

    for ( CIT_ConveyorMap it = conveyors_.begin(); it != conveyors_.end(); ++it )
        nTravelTime = std::max( nTravelTime, it->second->ApproximateTravelTime( startPos, endPos ) );

    return nTravelTime;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationConvoy::GetTravelTimeToLoadingPoint
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
unsigned int PHY_DotationConvoy::GetTravelTimeToLoadingPoint() const
{
    assert( pConsign_ );
    assert( pConsign_->GetSupplier().GetPC() );
    assert( pConsign_->GetConvoyer().GetPC() );
    const MT_Vector2D& formingPoint = pConsign_->GetConvoyer().GetPC()->GetRole< PHY_RoleInterface_Location >().GetPosition();
    const MT_Vector2D& loadingPoint = pConsign_->GetSupplier().GetPC()->GetRole< PHY_RoleInterface_Location >().GetPosition();

    return GetTravelTime( formingPoint, loadingPoint );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationConvoy::GetTravelTimeToUnloadingPoint
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
unsigned int PHY_DotationConvoy::GetTravelTimeToUnloadingPoint() const
{
    assert( pConsign_ );
    assert( pConsign_->GetSupplier().GetPC() );
    if( !pConsign_->GetNextSupplied() )
        return 0;
    const MT_Vector2D& loadingPoint   = pConsign_->GetSupplier().GetPC()->GetRole< PHY_RoleInterface_Location >().GetPosition();
    const MT_Vector2D& unloadingPoint = pConsign_->GetNextSupplied()->GetPionPC().GetRole< PHY_RoleInterface_Location >().GetPosition();
    return GetTravelTime( loadingPoint, unloadingPoint );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationConvoy::GetTravelTimeToFormingPoint
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
unsigned int PHY_DotationConvoy::GetTravelTimeToFormingPoint() const
{
    assert( pConsign_ );
    assert( pConsign_->GetConvoyer().GetPC() );
    if( !pConsign_->GetNextSupplied() )
        return 0;
    const MT_Vector2D& formingPoint   = pConsign_->GetConvoyer().GetPC()->GetRole< PHY_RoleInterface_Location >().GetPosition();
    const MT_Vector2D& unloadingPoint = pConsign_->GetNextSupplied()->GetPionPC().GetRole< PHY_RoleInterface_Location >().GetPosition();
    return GetTravelTime( unloadingPoint, formingPoint );
}


// -----------------------------------------------------------------------------
// Name: PHY_DotationConvoy::load
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void PHY_DotationConvoy::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< PHY_Convoy_ABC >( *this )
         >> pConsign_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationConvoy::save
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void PHY_DotationConvoy::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< PHY_Convoy_ABC >( *this )
         << pConsign_;
}