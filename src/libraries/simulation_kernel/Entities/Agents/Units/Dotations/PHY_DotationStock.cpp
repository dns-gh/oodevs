// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Dotations/PHY_DotationStock.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 11:48 $
// $Revision: 3 $
// $Workfile: PHY_DotationStock.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_DotationStock.h"
#include "PHY_DotationStockContainer.h"
#include "PHY_DotationCapacity.h"
#include "PHY_DotationType.h"
#include "PHY_DotationCategory.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Supply.h"
#include "Entities/MIL_EntityManager.h"

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_DotationStock )

const double PHY_DotationStock::maxCapacity_ = 10000000;

// -----------------------------------------------------------------------------
// Name: PHY_DotationStock constructor
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
PHY_DotationStock::PHY_DotationStock( PHY_DotationStockContainer& stockContainer, const PHY_DotationCategory& dotationCategory, double rSupplyThresholdRatio, double rCapacity, bool bInfiniteDotations, bool bCreateEmpty )
    : pStockContainer_   ( &stockContainer    )
    , pCategory_         ( &dotationCategory  )
    , rValue_            ( 0. )
    , rRequestedValue_   ( 0. )
    , rCapacity_         ( bInfiniteDotations ? maxCapacity_ : rCapacity )
    , rSupplyThreshold_  ( rCapacity * rSupplyThresholdRatio )
    , bInfiniteDotations_( bInfiniteDotations )
{
    if( !bCreateEmpty )
        SetValue( rCapacity_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStock constructor
// Created: JVT 2005-04-01
// -----------------------------------------------------------------------------
PHY_DotationStock::PHY_DotationStock()
    : pStockContainer_  ( 0 )
    , pCategory_        ( 0 )
    , rValue_           ( 0. )
    , rRequestedValue_  ( 0. )
    , rCapacity_        ( 0. )
    , rSupplyThreshold_ ( 0. )
    , bInfiniteDotations_( false)
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStock destructor
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
PHY_DotationStock::~PHY_DotationStock()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStock::load
// Created: JVT 2005-04-01
// -----------------------------------------------------------------------------
void PHY_DotationStock::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> pStockContainer_
         >> pCategory_
         >> rValue_
         >> rCapacity_
         >> rSupplyThreshold_
         >> bInfiniteDotations_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStock::save
// Created: JVT 2005-04-01
// -----------------------------------------------------------------------------
void PHY_DotationStock::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << pStockContainer_
         << pCategory_
         << rValue_
         << rCapacity_
         << rSupplyThreshold_
         << bInfiniteDotations_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStock::SetValue
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
void PHY_DotationStock::SetValue( double rValue )
{
    assert( pStockContainer_ );

    if( bInfiniteDotations_ && rValue < rSupplyThreshold_ )
        rValue = rCapacity_;

    rValue = std::min( rValue, maxCapacity_ );

    if( (unsigned int)rValue_ != (unsigned int)rValue )
        pStockContainer_->NotifyDotationChanged( *this, rValue - rValue_ );

    const bool bSupplyThresholdAlreadyReached = HasReachedSupplyThreshold();

    rValue_ = rValue;

    if( HasReachedSupplyThreshold() )
    {
        if( rRequestedValue_ == 0 )
            rRequestedValue_ = rCapacity_ - rValue_;
        assert( pCategory_ );
        pStockContainer_->NotifySupplyNeeded( *pCategory_, !bSupplyThresholdAlreadyReached );
    }
    else
        rRequestedValue_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStock::AddReservation
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
double PHY_DotationStock::AddReservation( double rNbr )
{
    return Consume( rNbr );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStock::RemoveReservation
// Created: NLD 2005-02-11
// -----------------------------------------------------------------------------
void PHY_DotationStock::RemoveReservation( double rNbr )
{
    SetValue( rValue_ + rNbr );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStock::GetCategory
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
const PHY_DotationCategory& PHY_DotationStock::GetCategory() const
{
    assert( pCategory_ );
    return *pCategory_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStock::GetValue
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
double PHY_DotationStock::GetValue() const
{
    return rValue_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStock::GetCapacity
// Created: NLD 2005-01-31
// -----------------------------------------------------------------------------
double PHY_DotationStock::GetCapacity() const
{
    return rCapacity_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStock::IsEmpty
// Created: BCI 2011-02-14
// -----------------------------------------------------------------------------
bool PHY_DotationStock::IsEmpty() const
{
    return rValue_ <= 0.0;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStock::Consume
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
double PHY_DotationStock::Consume( double rValue )
{
    rValue = std::min( rValue, rValue_ );
    SetValue( rValue_ - rValue );
    return rValue;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStock::NeedSupply
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
bool PHY_DotationStock::NeedSupply() const
{
    return rCapacity_ - rValue_ > 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStock::HasReachedSupplyThreshold
// Created: NLD 2005-02-02
// -----------------------------------------------------------------------------
bool PHY_DotationStock::HasReachedSupplyThreshold() const
{
    if( bInfiniteDotations_ )
        return false;
    if( IsJammed() )
        return false;
    return rValue_ < rSupplyThreshold_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStock::Supply
// Created: NLD 2005-01-28
// -----------------------------------------------------------------------------
double PHY_DotationStock::Supply( double rSupply )
{
    SetValue( rValue_ + rSupply );
    return rSupply;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStock::SupplyUntilFull
// Created: BCI 2011-02-14
// -----------------------------------------------------------------------------
double PHY_DotationStock::SupplyUntilFull( double rSupply )
{
    double oldValue = rValue_;
    SetValue( std::min( oldValue + rSupply, rCapacity_ ) );
    return rValue_ - oldValue;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStock::Resupply
// Created: NLD 2005-02-03
// -----------------------------------------------------------------------------
void PHY_DotationStock::Resupply( bool withLog )
{
    if( withLog )
        SetValue( std::max( rCapacity_ - rRequestedValue_, rValue_ ) );
    else
        if( rValue_ < rCapacity_ )
            SetValue( rCapacity_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStock::UpdateSupplyNeeded
// Created: MMC 2013-04-24
// -----------------------------------------------------------------------------
void PHY_DotationStock::UpdateSupplyNeeded()
{
    assert( pCategory_ );
    assert( pStockContainer_ );
    if( HasReachedSupplyThreshold() && !pStockContainer_->HasSupplyNeededNotified( *pCategory_ ) )
        pStockContainer_->NotifySupplyNeeded( *pCategory_, true );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStock::IsJammed
// Created: MMC 2013-04-24
// -----------------------------------------------------------------------------
bool PHY_DotationStock::IsJammed() const
{
    assert( pStockContainer_ );
    const PHY_RoleInterface_Supply* pSupply = pStockContainer_->GetRoleInterfaceSupply();
    if( !pSupply )
        return false;
    const MIL_AgentPion& pPion = pSupply->GetPion();
    return ( pPion.IsJammed() || pPion.IsLogisticJammed() );
}
