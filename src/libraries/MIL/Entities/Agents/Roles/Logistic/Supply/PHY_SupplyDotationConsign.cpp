// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Supply/PHY_SupplyDotationConsign.cpp $
// $Author: Nld $
// $Modtime: 13/04/05 12:15 $
// $Revision: 5 $
// $Workfile: PHY_SupplyDotationConsign.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "PHY_SupplyDotationConsign.h"
#include "PHY_SupplyDotationState.h"

#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"

BOOST_CLASS_EXPORT_GUID( PHY_SupplyDotationConsign, "PHY_SupplyDotationConsign" )

// -----------------------------------------------------------------------------
// Name: PHY_SupplyRepairConsign::PHY_SupplyDotationConsign
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
PHY_SupplyDotationConsign::PHY_SupplyDotationConsign( MIL_AutomateLOG& supplyingAutomate, PHY_SupplyDotationState& supplyState )
    : PHY_SupplyConsign_ABC( supplyingAutomate )
    , pSupplyState_        ( &supplyState )
    , pConvoy_             ( 0 )
{
    pConvoy_ = new PHY_DotationConvoy( *this ); 

    pSupplyState_->SetConsign( this );
    EnterStateConvoyForming();
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationConsign constructor
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
PHY_SupplyDotationConsign::PHY_SupplyDotationConsign()
    : PHY_SupplyConsign_ABC()
    , pSupplyState_        ( 0 )
    , pConvoy_             ( 0 )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationConsign destructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_SupplyDotationConsign::~PHY_SupplyDotationConsign()
{
    if( pSupplyState_ )
        pSupplyState_->SetConsign( 0 );
    delete pConvoy_;
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationConsign::serialize
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_SupplyDotationConsign::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< PHY_SupplyConsign_ABC >( *this )
         & pSupplyState_
         & pConvoy_;
}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_SupplyConsign_ABC::DoSupply
// Created: NLD 2005-01-28
// -----------------------------------------------------------------------------
void PHY_SupplyDotationConsign::DoSupply()
{
    assert( pSupplyState_ );
    pSupplyState_->Supply();
    delete pSupplyState_;
    pSupplyState_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationConsign::GetMerchandiseToConvoy
// Created: NLD 2005-07-13
// -----------------------------------------------------------------------------
void PHY_SupplyDotationConsign::GetMerchandiseToConvoy( T_MerchandiseToConvoyMap& container ) const
{
    assert( pSupplyState_ );
    pSupplyState_->GetMerchandiseToConvoy( container );
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationConsign::GetSuppliedAutomate
// Created: NLD 2005-02-01
// -----------------------------------------------------------------------------
const MIL_Automate* PHY_SupplyDotationConsign::GetSuppliedAutomate() const
{
    if( !pSupplyState_ )
        return 0;
    return &pSupplyState_->GetSuppliedAutomate();
}

// =============================================================================
// STATES
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationConsign::EnterStateConvoyForming
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
void PHY_SupplyDotationConsign::EnterStateConvoyForming()
{
    nTimer_ = 0;
    SetState( eConvoyForming );
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationConsign::DoConvoyForming
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
bool PHY_SupplyDotationConsign::DoConvoyForming()
{
    assert( pConvoy_ );
    return pConvoy_->Form();
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationConsign::EnterStateConvoyLoading
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
void PHY_SupplyDotationConsign::EnterStateConvoyLoading()
{
    assert( pConvoy_ );
    nTimer_ = pConvoy_->GetLoadingTime();
    SetState( eConvoyLoading );
}
   
// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationConsign::EnterStateConvoyGoingTo
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
void PHY_SupplyDotationConsign::EnterStateConvoyGoingTo()
{
    assert( pConvoy_ );
    nTimer_ = pConvoy_->GetTravelTime();
    SetState( eConvoyGoingTo );
}
   
// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationConsign::EnterStateConvoyUnloading
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
void PHY_SupplyDotationConsign::EnterStateConvoyUnloading()
{
    assert( pConvoy_ );
    nTimer_ = pConvoy_->GetUnloadingTime();
    SetState( eConvoyUnloading );
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationConsign::EnterStateConvoyGoingFrom
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
void PHY_SupplyDotationConsign::EnterStateConvoyGoingFrom()
{
    assert( pConvoy_ );
    nTimer_ = pConvoy_->GetTravelTime();
    SetState( eConvoyGoingFrom );

    DoSupply();
}
    
// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationConsign::EnterStateFinished
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
void PHY_SupplyDotationConsign::EnterStateFinished()
{
    nTimer_ = 0;
    SetState( eFinished );
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationConsign::Update
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
bool PHY_SupplyDotationConsign::Update()
{
    if ( --nTimer_ > 0 )
        return GetState() == eFinished;

    switch( GetState() )
    {
        case eConvoyForming   : if( DoConvoyForming() ) EnterStateConvoyLoading  (); break;
        case eConvoyLoading   :                         EnterStateConvoyGoingTo  (); break;
        case eConvoyGoingTo   :                         EnterStateConvoyUnloading(); break;
        case eConvoyUnloading :                         EnterStateConvoyGoingFrom(); break;
        case eConvoyGoingFrom :                         EnterStateFinished       (); break;
        case eFinished        :                                                      break;
        default:
            assert( false );
    }
    return GetState() == eFinished;
}
