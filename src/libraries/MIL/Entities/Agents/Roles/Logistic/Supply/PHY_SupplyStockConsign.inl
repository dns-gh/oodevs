// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Roles/Logistic/Supply/PHY_SupplyStockConsign.inl $
// $Author: Nld $
// $Modtime: 10/02/05 16:58 $
// $Revision: 2 $
// $Workfile: PHY_SupplyStockConsign.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockConsign::IsLoadingDone
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
inline
bool PHY_SupplyStockConsign::IsLoadingDone() const
{
    return GetState() > eConvoyLoading;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockConsign::IsUnloadingDone
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
inline
bool PHY_SupplyStockConsign::IsUnloadingDone() const
{
    return GetState() > eConvoyUnloading;
}

