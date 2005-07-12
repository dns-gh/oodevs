// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Dotations/PHY_DotationStock.inl $
// $Author: Jvt $
// $Modtime: 1/04/05 13:24 $
// $Revision: 3 $
// $Workfile: PHY_DotationStock.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_DotationStock::GetCategory
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
inline
const PHY_DotationCategory& PHY_DotationStock::GetCategory() const
{
    assert( pCategory_ );
    return *pCategory_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStock::GetValue
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
inline
MT_Float PHY_DotationStock::GetValue() const
{
    return rValue_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStock::GetCapacity
// Created: NLD 2005-01-31
// -----------------------------------------------------------------------------
inline
MT_Float PHY_DotationStock::GetCapacity() const
{
    return rCapacity_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStock::Consume
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
inline
MT_Float PHY_DotationStock::Consume( MT_Float rValue )
{
    rValue = std::min( rValue, rValue_ );
    SetValue( rValue_ - rValue );
    return rValue;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStock::NeedSupply
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
inline
bool PHY_DotationStock::NeedSupply() const
{
    return rCapacity_ - rValue_ > 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStock::HasReachedSupplyThreshold
// Created: NLD 2005-02-02
// -----------------------------------------------------------------------------
inline
bool PHY_DotationStock::HasReachedSupplyThreshold() const
{
    return rValue_ < rSupplyThreshold_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStock::Supply
// Created: NLD 2005-01-28
// -----------------------------------------------------------------------------
inline
MT_Float PHY_DotationStock::Supply( MT_Float rSupply )
{
    SetValue( rValue_ + rSupply );
    return rSupply;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStock::Resupply
// Created: NLD 2005-02-03
// -----------------------------------------------------------------------------
inline
void PHY_DotationStock::Resupply()
{
    if( rValue_ < rCapacity_ )
        SetValue( rCapacity_ );
}
