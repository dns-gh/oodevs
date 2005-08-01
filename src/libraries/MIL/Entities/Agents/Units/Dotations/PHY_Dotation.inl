// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Dotations/PHY_Dotation.inl $
// $Author: Jvt $
// $Modtime: 31/03/05 17:35 $
// $Revision: 6 $
// $Workfile: PHY_Dotation.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_Dotation::GetCapacity
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
inline
MT_Float PHY_Dotation::GetCapacity() const
{
    return rCapacity_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Dotation::GetValue
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
inline
MT_Float PHY_Dotation::GetValue() const
{
    return rValue_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Dotation::Consume
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
inline
MT_Float PHY_Dotation::Consume( MT_Float rValue )
{
    rValue = std::min( rValue, rValue_ );
    SetValue( rValue_ - rValue );
    return rValue;
}

// -----------------------------------------------------------------------------
// Name: PHY_Dotation::GetCategory
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
inline
const PHY_DotationCategory& PHY_Dotation::GetCategory() const
{
    assert( pCategory_ );
    return *pCategory_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Dotation::Resupply
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
inline
void PHY_Dotation::Resupply( MT_Float rFactor /* = 1. */ )
{
    SetValue( rCapacity_ * rFactor );
    rConsumptionReservation_ = 0.;
    rFireReservation_        = 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_Dotation::CancelConsumptionReservation
// Created: NLD 2004-09-30
// -----------------------------------------------------------------------------
inline
void PHY_Dotation::CancelConsumptionReservation()
{
    SetValue( rValue_ + rConsumptionReservation_ );
    rConsumptionReservation_ = 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_Dotation::ConsumeConsumptionReservation
// Created: NLD 2004-09-30
// -----------------------------------------------------------------------------
inline
void PHY_Dotation::ConsumeConsumptionReservation()
{
    rConsumptionReservation_ = 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_Dotation::ConsumeFireReservation
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
inline
void PHY_Dotation::ConsumeFireReservation()
{
    rFireReservation_ = 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_Dotation::NeedSupply
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
inline
bool PHY_Dotation::NeedSupply() const
{
    if( bDotationBlocked_ )
        return false;
    return rCapacity_ - rValue_ > 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_Dotation::HasReachedSupplyThreshold
// Created: NLD 2005-02-02
// -----------------------------------------------------------------------------
inline
bool PHY_Dotation::HasReachedSupplyThreshold() const
{
    if( bDotationBlocked_ )
        return false;
    return rValue_ < rSupplyThreshold_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Dotation::Supply
// Created: NLD 2005-01-28
// -----------------------------------------------------------------------------
inline
MT_Float PHY_Dotation::Supply( MT_Float rSupply )
{
    if( bDotationBlocked_ )
        return 0.;

    const MT_Float rNewSupply = std::min( rSupply, rCapacity_ - rConsumptionReservation_ - rFireReservation_ - rValue_ );
    SetValue( rValue_ + rNewSupply );
    return rNewSupply;
}
