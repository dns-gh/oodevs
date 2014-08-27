// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/PHY_RoleInterface_Supply.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 11:48 $
// $Revision: 2 $
// $Workfile: PHY_RoleInterface_Supply.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RoleInterface_Supply.h"

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

bool PHY_RoleInterface_Supply::IsSystemEnabled() const
{
    return false;
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
// Name: PHY_RoleInterface_Supply::AddStock
// Created: BCI 2011-02-11
// -----------------------------------------------------------------------------
PHY_DotationStock* PHY_RoleInterface_Supply::AddEmptyStock( const PHY_DotationCategory& /*dotationCategory*/, double /*capacity*/ ) const
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Supply::ComputeStockWeightAndVolume
// Created: BCI 2011-02-14
// -----------------------------------------------------------------------------
void PHY_RoleInterface_Supply::ComputeStockWeightAndVolume( const PHY_DotationNature&, double& rWeight, double& rVolume ) const
{
    rWeight = 0;
    rVolume = 0;
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
// Name: PHY_RoleInterface_Supply::AddStockReservation
// Created: NLD 2005-02-01
// -----------------------------------------------------------------------------
double PHY_RoleInterface_Supply::AddStockReservation( const PHY_DotationCategory& /*dotationCategory*/, double /*rRequestedValue*/ )
{
    return 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Supply::RemoveStockReservation
// Created: NLD 2005-02-11
// -----------------------------------------------------------------------------
double PHY_RoleInterface_Supply::RemoveStockReservation( const PHY_DotationCategory& /*dotationCategory*/, double /*rRequestedValue*/ )
{
    return 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Supply::CanReserveStock
// Created: NLD 2005-02-01
// -----------------------------------------------------------------------------
bool PHY_RoleInterface_Supply::CanReserveStock( const PHY_DotationCategory& /*dotationCategory*/ ) const
{
    return false;
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
// Name: PHY_RoleInterface_Supply::GetAvailableConvoyTransporter
// Created: NLD 2005-02-07
// -----------------------------------------------------------------------------
PHY_ComposantePion* PHY_RoleInterface_Supply::GetAvailableConvoyTransporter( const PHY_ComposanteTypePion& /*type*/ ) const
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
// Name: PHY_RoleInterface_Supply::ResupplyStocks
// Created: NLD 2005-02-03
// -----------------------------------------------------------------------------
void PHY_RoleInterface_Supply::ResupplyStocks( bool /*withLog*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Supply::ResupplyStocks
// Created: SBO 2005-12-12
// -----------------------------------------------------------------------------
void PHY_RoleInterface_Supply::ResupplyStocks( const PHY_DotationCategory& /*category*/, double /*rNbr*/ )
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

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Supply::Apply
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
void PHY_RoleInterface_Supply::Apply( std::function< void( PHY_DotationStock& ) > visitor ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Supply::ConnectToResourceNode
// Created: JSR 2012-04-23
// -----------------------------------------------------------------------------
void PHY_RoleInterface_Supply::ConnectToResourceNode( unsigned int /*objectId*/, const std::string& /*resource*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Supply::DisconnectFromResourceNode
// Created: JSR 2012-04-23
// -----------------------------------------------------------------------------
void PHY_RoleInterface_Supply::DisconnectFromResourceNode()
{
    // NOTHING
}
