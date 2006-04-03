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

#include "MIL_pch.h"

#include "PHY_SupplyDotationConsign.h"
#include "PHY_SupplyDotationState.h"

#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"

BOOST_CLASS_EXPORT_GUID( PHY_SupplyDotationConsign, "PHY_SupplyDotationConsign" )

// -----------------------------------------------------------------------------
// Name: PHY_SupplyRepairConsign::PHY_SupplyDotationConsign
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
PHY_SupplyDotationConsign::PHY_SupplyDotationConsign( MIL_AutomateLOG& supplyingAutomate, PHY_SupplyDotationState& supplyState )
    : PHY_SupplyConsign_ABC( supplyingAutomate, supplyingAutomate )
    , pSupplyState_        ( &supplyState )
    , pConvoy_             ( 0 )
{
    pConvoy_ = new PHY_DotationConvoy( *this ); 

    pSupplyState_->SetConsign( this );
    EnterStateConvoyWaitingForTransporters();
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
// Name: PHY_SupplyDotationConsign::RemoveConvoyedMerchandise
// Created: NLD 2005-12-15
// -----------------------------------------------------------------------------
void PHY_SupplyDotationConsign::RemoveConvoyedMerchandise( const PHY_DotationCategory& dotationCategory, MT_Float rNbrDotations )
{
    assert( pSupplyState_ );
    pSupplyState_->RemoveConvoyedMerchandise( dotationCategory, rNbrDotations );
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationConsign::AddConvoyedMerchandise
// Created: NLD 2005-12-15
// -----------------------------------------------------------------------------
void PHY_SupplyDotationConsign::AddConvoyedMerchandise( const PHY_DotationCategory& dotationCategory, MT_Float rNbrDotations )
{
    assert( pSupplyState_ );
    pSupplyState_->AddConvoyedMerchandise( dotationCategory, rNbrDotations );
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationConsign::CancelMerchandiseOverheadReservation
// Created: NLD 2005-12-15
// -----------------------------------------------------------------------------
void PHY_SupplyDotationConsign::CancelMerchandiseOverheadReservation()
{
    assert( pSupplyState_ );
    pSupplyState_->CancelMerchandiseOverheadReservation();
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
// Name: PHY_SupplyDotationConsign::EnterStateConvoyWaitingForTransporters
// Created: NLD 2005-12-14
// -----------------------------------------------------------------------------
void PHY_SupplyDotationConsign::EnterStateConvoyWaitingForTransporters()
{
    nTimer_ = 0;
    SetState( eConvoyWaitingForTransporters );
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationConsign::EnterStateConvoyForming
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
void PHY_SupplyDotationConsign::EnterStateConvoyForming()
{
    assert( pConvoy_ );
    nTimer_ = pConvoy_->GetFormingTime();
    SetState( eConvoyForming );
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationConsign::EnterStateConvoyGoingToLoadingPoint
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
void PHY_SupplyDotationConsign::EnterStateConvoyGoingToLoadingPoint()
{
    assert( pConvoy_ );
    nTimer_ = pConvoy_->GetTravelTimeToLoadingPoint();
    SetState( eConvoyGoingToLoadingPoint );
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
// Name: PHY_SupplyDotationConsign::EnterStateConvoyGoingToUnloadingPoint
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
void PHY_SupplyDotationConsign::EnterStateConvoyGoingToUnloadingPoint()
{
    assert( pConvoy_ );
    nTimer_ = pConvoy_->GetTravelTimeToUnloadingPoint();
    SetState( eConvoyGoingToUnloadingPoint );
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
// Name: PHY_SupplyDotationConsign::EnterStateConvoyGoingBackToFormingPoint
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
void PHY_SupplyDotationConsign::EnterStateConvoyGoingBackToFormingPoint()
{
    assert( pConvoy_ );
    nTimer_ = pConvoy_->GetTravelTimeToFormingPoint();
    SetState( eConvoyGoingBackToFormingPoint );

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
    assert( pConvoy_ );

    if ( --nTimer_ > 0 )
        return GetState() == eFinished;

    switch( GetState() )
    {
        case eConvoyWaitingForTransporters  : if( pConvoy_->ReserveTransporters() )  EnterStateConvoyForming                (); break;
        case eConvoyForming                 :                                        EnterStateConvoyGoingToLoadingPoint    (); break;
        case eConvoyGoingToLoadingPoint     :                                        EnterStateConvoyLoading                (); break;
        case eConvoyLoading                 :                                        EnterStateConvoyGoingToUnloadingPoint  (); break;
        case eConvoyGoingToUnloadingPoint   :                                        EnterStateConvoyUnloading              (); break;
        case eConvoyUnloading               :                                        EnterStateConvoyGoingBackToFormingPoint(); break;
        case eConvoyGoingBackToFormingPoint :                                        EnterStateFinished                     (); break;
        case eFinished                      :                                                                                   break;
        default:
            assert( false );
    }
    return GetState() == eFinished;
}
