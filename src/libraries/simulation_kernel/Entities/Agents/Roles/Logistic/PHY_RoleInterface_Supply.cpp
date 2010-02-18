// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Supply/PHY_RoleInterface_Supply.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 11:48 $
// $Revision: 2 $
// $Workfile: PHY_RoleInterface_Supply.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RoleInterface_Supply.h"

BOOST_CLASS_EXPORT_GUID( PHY_RoleInterface_Supply, "PHY_RoleInterface_Supply" )

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Supply constructor
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
PHY_RoleInterface_Supply::PHY_RoleInterface_Supply()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Supply destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RoleInterface_Supply::~PHY_RoleInterface_Supply()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Supply::WriteODB
// Created: NLD 2006-05-29
// -----------------------------------------------------------------------------
void PHY_RoleInterface_Supply::WriteODB( xml::xostream& /*xos*/ ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Supply::UpdateLogistic
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
void PHY_RoleInterface_Supply::UpdateLogistic( bool /*bIsDead*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Supply::ReadOverloading
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
void PHY_RoleInterface_Supply::ReadOverloading( xml::xistream& /*xis*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Supply::EnableSystem
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void PHY_RoleInterface_Supply::EnableSystem()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Supply::DisableSystem
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void PHY_RoleInterface_Supply::DisableSystem()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Supply::CanContainStock
// Created: NLD 2005-03-17
// -----------------------------------------------------------------------------
bool PHY_RoleInterface_Supply::CanContainStock( const PHY_DotationCategory& /*dotationCategory*/ ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Supply::CreateStock
// Created: NLD 2006-03-28
// -----------------------------------------------------------------------------
PHY_DotationStock* PHY_RoleInterface_Supply::AddStock( const PHY_DotationCategory& /*dotationCategory*/ ) const
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Supply::GetStock
// Created: NLD 2005-02-04
// -----------------------------------------------------------------------------
PHY_DotationStock* PHY_RoleInterface_Supply::GetStock( const PHY_DotationCategory& /*dotationCategory*/ ) const
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Supply::GetStockAvailablity
// Created: NLD 2005-02-01
// -----------------------------------------------------------------------------
MT_Float PHY_RoleInterface_Supply::GetStockAvailablity( const PHY_DotationCategory& /*dotationCategory*/, MT_Float /*rRequestedValue*/ ) const
{
    return 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Supply::AddStockReservation
// Created: NLD 2005-02-01
// -----------------------------------------------------------------------------
MT_Float PHY_RoleInterface_Supply::AddStockReservation( const PHY_DotationCategory& /*dotationCategory*/, MT_Float /*rRequestedValue*/ )
{
    return 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Supply::RemoveStockReservation
// Created: NLD 2005-02-11
// -----------------------------------------------------------------------------
void PHY_RoleInterface_Supply::RemoveStockReservation( const PHY_DotationCategory& /*dotationCategory*/, MT_Float /*rRequestedValue*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Supply::GetAvailableConvoyTransporter
// Created: NLD 2005-02-07
// -----------------------------------------------------------------------------
PHY_ComposantePion* PHY_RoleInterface_Supply::GetAvailableConvoyTransporter( const PHY_DotationCategory& /*dotationCategory*/ ) const
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Supply::StartUsingForLogistic
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
void PHY_RoleInterface_Supply::StartUsingForLogistic( PHY_ComposantePion& /*composante*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Supply::StopUsingForLogistic
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
void PHY_RoleInterface_Supply::StopUsingForLogistic( PHY_ComposantePion& /*composante*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Supply::FillSupplyRequest
// Created: NLD 2005-01-31
// -----------------------------------------------------------------------------
void PHY_RoleInterface_Supply::FillSupplyRequest( PHY_SupplyStockRequestContainer& /*supplyRequest*/ ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Supply::ResupplyStocks
// Created: NLD 2005-02-03
// -----------------------------------------------------------------------------
void PHY_RoleInterface_Supply::ResupplyStocks()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Supply::ResupplyStocks
// Created: SBO 2005-12-12
// -----------------------------------------------------------------------------
void PHY_RoleInterface_Supply::ResupplyStocks( const PHY_DotationCategory& /*category*/, MT_Float /*rNbr*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Supply::AssignConvoy
// Created: NLD 2005-02-09
// -----------------------------------------------------------------------------
void PHY_RoleInterface_Supply::AssignConvoy( PHY_StockConvoy& /*convoy*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Supply::UnassignConvoy
// Created: NLD 2005-02-09
// -----------------------------------------------------------------------------
void PHY_RoleInterface_Supply::UnassignConvoy( PHY_StockConvoy& /*convoy*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Supply::ConvoyLoad
// Created: NLD 2005-02-10
// -----------------------------------------------------------------------------
bool PHY_RoleInterface_Supply::ConvoyLoad() const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Supply::ConvoyUnload
// Created: NLD 2005-02-10
// -----------------------------------------------------------------------------
bool PHY_RoleInterface_Supply::ConvoyUnload() const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Supply::ConvoyIsLoadingDone
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
bool PHY_RoleInterface_Supply::ConvoyIsLoadingDone() const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Supply::ConvoyIsUnloadingDone
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
bool PHY_RoleInterface_Supply::ConvoyIsUnloadingDone() const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Supply::ConvoyGetSupplyingAutomate
// Created: NLD 2006-07-31
// -----------------------------------------------------------------------------
const MIL_AutomateLOG* PHY_RoleInterface_Supply::ConvoyGetSupplyingAutomate() const
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Supply::ConvoyGetConvoyingAutomate
// Created: NLD 2006-07-31
// -----------------------------------------------------------------------------
const MIL_AutomateLOG* PHY_RoleInterface_Supply::ConvoyGetConvoyingAutomate() const
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Supply::ConvoyGetSuppliedAutomate
// Created: NLD 2006-07-31
// -----------------------------------------------------------------------------
const MIL_Automate* PHY_RoleInterface_Supply::ConvoyGetSuppliedAutomate() const
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Supply::ConvoyEndMission
// Created: NLD 2005-02-10
// -----------------------------------------------------------------------------
void PHY_RoleInterface_Supply::ConvoyEndMission()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Supply::Clean
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
void PHY_RoleInterface_Supply::Clean()
{
    // NOTHING
}
