// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Roles/Logistic/Supply/PHY_SupplyStockRequest.inl $
// $Author: Nld $
// $Modtime: 4/02/05 15:34 $
// $Revision: 1 $
// $Workfile: PHY_SupplyStockRequest.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockRequest::GetSupplyingAutomate
// Created: NLD 2005-02-02
// -----------------------------------------------------------------------------
inline
MIL_AutomateLOG* PHY_SupplyStockRequest::GetSupplyingAutomate() const
{
    return pSupplyingAutomate_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockRequest::GetTotalRequestedValue
// Created: NLD 2005-02-02
// -----------------------------------------------------------------------------
inline
MT_Float PHY_SupplyStockRequest::GetTotalRequestedValue() const
{
    return rTotalRequestedValue_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockRequest::GetTotalReservedValue
// Created: NLD 2005-02-04
// -----------------------------------------------------------------------------
inline
MT_Float PHY_SupplyStockRequest::GetTotalReservedValue() const
{
    return rTotalReservedValue_;
}
