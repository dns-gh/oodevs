// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Supply/PHY_RolePion_Supply.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 11:48 $
// $Revision: 4 $
// $Workfile: PHY_RolePion_Supply.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "PHY_RolePion_Supply.h"

BOOST_CLASS_EXPORT_GUID( PHY_RolePion_Supply, "PHY_RolePion_Supply" )

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Supply constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_Supply::PHY_RolePion_Supply( MT_RoleContainer& role )
    : PHY_RoleInterface_Supply( role )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Supply constructor
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
PHY_RolePion_Supply::PHY_RolePion_Supply()
    : PHY_RoleInterface_Supply()
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Supply destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_Supply::~PHY_RolePion_Supply()
{

}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Supply::serialize
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RolePion_Supply::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< PHY_RoleInterface_Supply >( *this );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Supply::Update
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
void PHY_RolePion_Supply::Update( bool /*bIsDead*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Supply::Clean
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
void PHY_RolePion_Supply::Clean()
{
    // NOTHING
}



// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Supply::EnableSystem
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void PHY_RolePion_Supply::EnableSystem()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Supply::DisableSystem
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void PHY_RolePion_Supply::DisableSystem()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Supply::GetStockAvailablity
// Created: NLD 2005-02-01
// -----------------------------------------------------------------------------
MT_Float PHY_RolePion_Supply::GetStockAvailablity( const PHY_DotationCategory& /*dotationCategory*/, MT_Float /*rRequestedValue*/ ) const
{
    return 0.;
}
    
// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Supply::AddStockReservation
// Created: NLD 2005-02-01
// -----------------------------------------------------------------------------
MT_Float PHY_RolePion_Supply::AddStockReservation( const PHY_DotationCategory& /*dotationCategory*/, MT_Float /*rRequestedValue*/ )
{
    return 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Supply::RemoveStockReservation
// Created: NLD 2005-02-11
// -----------------------------------------------------------------------------
void PHY_RolePion_Supply::RemoveStockReservation( const PHY_DotationCategory& /*dotationCategory*/, MT_Float /*rRequestedValue*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Supply::ReadOverloading
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
void PHY_RolePion_Supply::ReadOverloading( MIL_InputArchive& /*archive*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Supply::StartUsingForLogistic
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
void PHY_RolePion_Supply::StartUsingForLogistic( PHY_ComposantePion& /*composante*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Supply::StopUsingForLogistic
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
void PHY_RolePion_Supply::StopUsingForLogistic( PHY_ComposantePion& /*composante*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Supply::FillSupplyRequest
// Created: NLD 2005-01-31
// -----------------------------------------------------------------------------
void PHY_RolePion_Supply::FillSupplyRequest( PHY_SupplyStockRequestContainer& /*supplyRequest*/ ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Supply::ResupplyStocks
// Created: NLD 2005-02-03
// -----------------------------------------------------------------------------
void PHY_RolePion_Supply::ResupplyStocks()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Supply::ResupplyStocks
// Created: SBO 2005-12-12
// -----------------------------------------------------------------------------
void PHY_RolePion_Supply::ResupplyStocks( const PHY_DotationCategory& /*category*/, MT_Float /*rNbr*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: phy_rolepion_supply::CanContainStock
// Created: NLD 2005-03-17
// -----------------------------------------------------------------------------
bool PHY_RolePion_Supply::CanContainStock( const PHY_DotationCategory& /*dotationCategory*/ ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Supply::GetStock
// Created: NLD 2005-02-04
// -----------------------------------------------------------------------------
PHY_DotationStock* PHY_RolePion_Supply::GetStock( const PHY_DotationCategory& /*dotationCategory*/ ) const
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Supply::GetAvailableConvoyTransporter
// Created: NLD 2005-02-07
// -----------------------------------------------------------------------------
PHY_ComposantePion* PHY_RolePion_Supply::GetAvailableConvoyTransporter() const
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Supply::GetAvailableConvoyCommander
// Created: NLD 2005-02-07
// -----------------------------------------------------------------------------
PHY_ComposantePion* PHY_RolePion_Supply::GetAvailableConvoyCommander() const
{
    return 0;
}

// =============================================================================
// CONVOY
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Supply::AssignConvoy
// Created: NLD 2005-02-09
// -----------------------------------------------------------------------------
void PHY_RolePion_Supply::AssignConvoy( PHY_StockConvoy& /*convoy*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Supply::UnassignConvoy
// Created: NLD 2005-02-09
// -----------------------------------------------------------------------------
void PHY_RolePion_Supply::UnassignConvoy( PHY_StockConvoy& /*convoy*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Supply::ConvoyIsSupplyDone
// Created: NLD 2005-02-10
// -----------------------------------------------------------------------------
bool PHY_RolePion_Supply::ConvoyIsSupplyDone() const
{
    return true;
}
    
// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Supply::ConvoyGetSuppliedAutomate
// Created: NLD 2005-02-10
// -----------------------------------------------------------------------------
const MIL_Automate* PHY_RolePion_Supply::ConvoyGetSuppliedAutomate() const
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Supply::ConvoyEndMission
// Created: NLD 2005-02-10
// -----------------------------------------------------------------------------
void PHY_RolePion_Supply::ConvoyEndMission()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Supply::ConvoySupply
// Created: NLD 2005-02-10
// -----------------------------------------------------------------------------
bool PHY_RolePion_Supply::ConvoySupply() const
{
    return true;
}


// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Supply::NotifyComposanteChanged
// Created: NLD 2005-02-10
// -----------------------------------------------------------------------------
void PHY_RolePion_Supply::NotifyComposanteChanged( PHY_ComposantePion& /*composante*/ )
{
    // NOTHING
}


// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Supply::SendChangedState
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void PHY_RolePion_Supply::SendChangedState() const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Supply::SendFullState
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void PHY_RolePion_Supply::SendFullState() const
{
    // NOTHING
}
