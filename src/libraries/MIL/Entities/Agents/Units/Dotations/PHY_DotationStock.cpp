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

#include "MIL_pch.h"

#include "PHY_DotationStock.h"
#include "PHY_DotationStockContainer.h"
#include "PHY_DotationCapacity.h"
#include "PHY_DotationType.h"
#include "PHY_DotationCategory.h"

BOOST_CLASS_EXPORT_GUID( PHY_DotationStock, "PHY_DotationStock" )

// -----------------------------------------------------------------------------
// Name: PHY_DotationStock constructor
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
PHY_DotationStock::PHY_DotationStock( PHY_DotationStockContainer& stockContainer, const PHY_DotationCategory& dotationCategory, MT_Float rSupplyThresholdRatio, MT_Float rCapacity )
    : pStockContainer_ ( &stockContainer    )
    , pCategory_       ( &dotationCategory  )
    , rValue_          ( 0. )
    , rCapacity_       ( rCapacity )
    , rSupplyThreshold_( rCapacity * rSupplyThresholdRatio )
{
    SetValue( rCapacity_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStock constructor
// Created: JVT 2005-04-01
// -----------------------------------------------------------------------------
PHY_DotationStock::PHY_DotationStock()
    : pStockContainer_ ( 0 )
    , pCategory_       ( 0 )
    , rValue_          ( 0. )
    , rCapacity_       ( 0. )
    , rSupplyThreshold_( 0. )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStock destructor
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
PHY_DotationStock::~PHY_DotationStock()
{
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_DotationStock::load
// Created: JVT 2005-04-01
// -----------------------------------------------------------------------------
void PHY_DotationStock::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> pStockContainer_;
    
    uint nID;
    file >> nID;
    pCategory_ = PHY_DotationType::FindDotationCategory( nID );

    file >> rValue_
         >> rCapacity_
         >> rSupplyThreshold_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStock::save
// Created: JVT 2005-04-01
// -----------------------------------------------------------------------------
void PHY_DotationStock::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << pStockContainer_
         << pCategory_->GetMosID()
         << rValue_
         << rCapacity_
         << rSupplyThreshold_;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_DotationStock::SetValue
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
void PHY_DotationStock::SetValue( MT_Float rValue )
{
    assert( pStockContainer_ );
    
    if ( (uint)rValue_ != (uint)rValue )
        pStockContainer_->NotifyDotationChanged( *this, rValue - rValue_ );

    const bool bSupplyThresholdAlreadyReached = HasReachedSupplyThreshold();

    rValue_ = rValue;

    if ( HasReachedSupplyThreshold() )
    {
        assert( pCategory_ );
        pStockContainer_->NotifySupplyNeeded( *pCategory_, !bSupplyThresholdAlreadyReached );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStock::AddReservation
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
MT_Float PHY_DotationStock::AddReservation( MT_Float rNbr )
{
    return Consume( rNbr );    
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStock::RemoveReservation
// Created: NLD 2005-02-11
// -----------------------------------------------------------------------------
void PHY_DotationStock::RemoveReservation( MT_Float rNbr )
{
    SetValue( rValue_ + rNbr );
}
