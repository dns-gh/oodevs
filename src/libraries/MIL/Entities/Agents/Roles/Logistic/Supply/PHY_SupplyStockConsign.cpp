// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Supply/PHY_SupplyStockConsign.cpp $
// $Author: Nld $
// $Modtime: 14/04/05 10:08 $
// $Revision: 5 $
// $Workfile: PHY_SupplyStockConsign.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "PHY_SupplyStockConsign.h"
#include "PHY_SupplyStockState.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Network/NET_ASN_Messages.h"

BOOST_CLASS_EXPORT_GUID( PHY_SupplyStockConsign, "PHY_SupplyStockConsign" )

// -----------------------------------------------------------------------------
// Name: PHY_SupplyRepairConsign::PHY_SupplyStockConsign
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
PHY_SupplyStockConsign::PHY_SupplyStockConsign( MIL_AutomateLOG& supplyAutomate, PHY_SupplyStockState& supplyState )
    : PHY_SupplyConsign_ABC( supplyAutomate )
    , pSupplyState_        ( &supplyState )
    , pConvoy_             ( 0 )
{
    pConvoy_ = new PHY_StockConvoy( *this );
    pSupplyState_->SetConsign( this );
    EnterStateConvoyForming();
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockConsign constructor
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
PHY_SupplyStockConsign::PHY_SupplyStockConsign()
    : PHY_SupplyConsign_ABC()
    , pSupplyState_        ( 0 )
    , pConvoy_             ( 0 )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockConsign destructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_SupplyStockConsign::~PHY_SupplyStockConsign()
{
    if( pSupplyState_ )
        pSupplyState_->SetConsign( 0 );
    delete pConvoy_;
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockConsign::serialize
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_SupplyStockConsign::serialize( Archive& file, const uint )
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
void PHY_SupplyStockConsign::DoSupply()
{
    assert( pSupplyState_ );
    pSupplyState_->Supply();
    delete pSupplyState_;
    pSupplyState_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockConsign::GetUVolumeRequested
// Created: NLD 2005-02-01
// -----------------------------------------------------------------------------
MT_Float PHY_SupplyStockConsign::GetUVolumeRequested() const
{
    assert( pSupplyState_ );
    return pSupplyState_->GetUVolumeRequested();
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockConsign::GetSuppliedAutomate
// Created: NLD 2005-02-01
// -----------------------------------------------------------------------------
const MIL_Automate* PHY_SupplyStockConsign::GetSuppliedAutomate() const
{
    if( !pSupplyState_ )
        return 0;
    return &pSupplyState_->GetSuppliedAutomate();
}

// =============================================================================
// STATES
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockConsign::EnterStateConvoyForming
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
void PHY_SupplyStockConsign::EnterStateConvoyForming()
{
    nTimer_ = 0;
    SetState( eConvoyForming );
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockConsign::DoConvoyForming
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
bool PHY_SupplyStockConsign::DoConvoyForming()
{
    assert( pConvoy_ );
    return pConvoy_->Form();
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockConsign::EnterStateConvoyLoading
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
void PHY_SupplyStockConsign::EnterStateConvoyLoading()
{
    assert( pConvoy_ );
    nTimer_ = pConvoy_->GetLoadingTime();
    SetState( eConvoyLoading );
}
   
// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockConsign::EnterStateConvoyGoingTo
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
void PHY_SupplyStockConsign::EnterStateConvoyGoingTo()
{
    nTimer_ = 0;
    SetState( eConvoyGoingTo );

    assert( pConvoy_ );
    pConvoy_->ActivateConvoyMission();
}
   
// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockConsign::EnterStateConvoyUnloading
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
void PHY_SupplyStockConsign::EnterStateConvoyUnloading()
{
    assert( pConvoy_ );
    nTimer_ = pConvoy_->GetUnloadingTime();
    SetState( eConvoyUnloading );
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockConsign::EnterStateConvoyGoingFrom
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
void PHY_SupplyStockConsign::EnterStateConvoyGoingFrom()
{
    nTimer_ = 0;
    SetState( eConvoyGoingFrom );
    DoSupply();
}
    
// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockConsign::EnterStateFinished
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
void PHY_SupplyStockConsign::EnterStateFinished()
{
    nTimer_ = 0;
    SetState( eFinished );

    if( pSupplyState_ )
    {
        pSupplyState_->CancelSupply();
        delete pSupplyState_;
        pSupplyState_ = 0;
    }
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockConsign::ConvoySupply
// Created: NLD 2005-02-10
// -----------------------------------------------------------------------------
bool PHY_SupplyStockConsign::ConvoySupply()
{
    if( GetState() == eConvoyGoingTo )
        EnterStateConvoyUnloading();

    return GetState() == eConvoyGoingFrom;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockConsign::ConvoyEndMission
// Created: NLD 2005-02-10
// -----------------------------------------------------------------------------
void PHY_SupplyStockConsign::ConvoyEndMission()
{
    EnterStateFinished();
    assert( pConvoy_ );
    pConvoy_->DesactivateConvoyMission();
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockConsign::RemoveConvoyedStock
// Created: NLD 2005-02-10
// -----------------------------------------------------------------------------
void PHY_SupplyStockConsign::RemoveConvoyedStock( MT_Float rUVolumeRemoved )
{
    if( pSupplyState_ )
        pSupplyState_->RemoveConvoyedStock( rUVolumeRemoved );
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockConsign::Update
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
bool PHY_SupplyStockConsign::Update()
{
    if ( --nTimer_ > 0 )
        return GetState() == eFinished;

    switch( GetState() )
    {
        case eConvoyForming   : if( DoConvoyForming() ) EnterStateConvoyLoading  (); break;
        case eConvoyLoading   :                         EnterStateConvoyGoingTo  (); break;

        case eConvoyGoingTo   :                                                      break; // Transition gérée par scripts
        case eConvoyUnloading :                         EnterStateConvoyGoingFrom(); break;
        case eConvoyGoingFrom :                                                      break; // Transition gérée par scripts
        case eFinished        :                                                      break;
        default:
            assert( false );
    }
    return GetState() == eFinished;
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockConsign::SendFullState
// Created: NLD 2005-02-10
// -----------------------------------------------------------------------------
void PHY_SupplyStockConsign::SendFullState( NET_ASN_MsgLogRavitaillementTraitementUpdate& asn ) const
{
    PHY_SupplyConsign_ABC::SendFullState( asn );

    assert( pConvoy_ );
    asn.GetAsnMsg().m.oid_pion_convoiPresent = 1;
    asn.GetAsnMsg().oid_pion_convoi = pConvoy_->GetPionConvoy() ? pConvoy_->GetPionConvoy()->GetID() : 0;
}


