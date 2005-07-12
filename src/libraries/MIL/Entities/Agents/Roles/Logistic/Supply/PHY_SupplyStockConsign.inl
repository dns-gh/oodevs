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
// Name: PHY_SupplyStockConsign::IsSupplyDone
// Created: NLD 2005-02-10
// -----------------------------------------------------------------------------
inline
bool PHY_SupplyStockConsign::IsSupplyDone() const
{
    return GetState() == eConvoyGoingFrom;
}

