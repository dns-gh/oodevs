// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/PHY_SupplyStockConsign.cpp $
// $Author: Nld $
// $Modtime: 14/04/05 10:08 $
// $Revision: 5 $
// $Workfile: PHY_SupplyStockConsign.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "PHY_SupplyStockConsign.h"
#include "PHY_SupplyStockState.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "protocol/ClientSenders.h"

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_SupplyStockConsign )

// -----------------------------------------------------------------------------
// Name: PHY_SupplyRepairConsign::PHY_SupplyStockConsign
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
PHY_SupplyStockConsign::PHY_SupplyStockConsign( MIL_AutomateLOG& supplyingAutomate, PHY_SupplyStockState& supplyState, MIL_Automate& stockSupplier )
    : PHY_SupplyConsign_ABC( supplyingAutomate, supplyState.GetSuppliedAutomate(), supplyState.GetConvoyer(), stockSupplier )
    , pSupplyState_        ( &supplyState )
    , pConvoy_             ( 0 )
{
    pConvoy_ = new PHY_StockConvoy( *this );
    pSupplyState_->SetConsign( this );
    EnterStateConvoyWaitingForTransporters();
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
    // NOTHING
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

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockConsign::serialize
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_SupplyStockConsign::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< PHY_SupplyConsign_ABC >( *this )
         & pSupplyState_
         & pConvoy_;
}

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
    pConvoy_->EmptyOut();
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockConsign::GetMerchandiseToConvoy
// Created: NLD 2005-07-13
// -----------------------------------------------------------------------------
void PHY_SupplyStockConsign::GetMerchandiseToConvoy( T_MerchandiseToConvoyMap& container ) const
{
    assert( pSupplyState_ );
    pSupplyState_->GetMerchandiseToConvoy( container );
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockConsign::RemoveConvoyedMerchandise
// Created: NLD 2005-12-15
// -----------------------------------------------------------------------------
void PHY_SupplyStockConsign::RemoveConvoyedMerchandise( const PHY_DotationCategory& dotationCategory, double rNbrDotations )
{
    assert( pSupplyState_ );
    pSupplyState_->RemoveConvoyedMerchandise( dotationCategory, rNbrDotations );
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockConsign::AddConvoyedMerchandise
// Created: NLD 2005-12-15
// -----------------------------------------------------------------------------
void PHY_SupplyStockConsign::AddConvoyedMerchandise( const PHY_DotationCategory& dotationCategory, double rNbrDotations )
{
    assert( pSupplyState_ );
    pSupplyState_->AddConvoyedMerchandise( dotationCategory, rNbrDotations );
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockConsign::CancelMerchandiseOverheadReservation
// Created: NLD 2005-12-15
// -----------------------------------------------------------------------------
void PHY_SupplyStockConsign::CancelMerchandiseOverheadReservation()
{
    assert( pSupplyState_ );
    pSupplyState_->CancelMerchandiseOverheadReservation();
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockConsign::EnterStateConvoyWaitingForTransporters
// Created: NLD 2005-12-14
// -----------------------------------------------------------------------------
void PHY_SupplyStockConsign::EnterStateConvoyWaitingForTransporters()
{
    nTimer_ = 0;
    SetState( eConvoyWaitingForTransporters );
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockConsign::EnterStateConvoyForming
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
void PHY_SupplyStockConsign::EnterStateConvoyForming()
{
    assert( pConvoy_ );
    nTimer_ = pConvoy_->GetFormingTime();
    SetState( eConvoyForming );
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockConsign::EnterStateConvoyGoingToLoadingPoint
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
void PHY_SupplyStockConsign::EnterStateConvoyGoingToLoadingPoint()
{
    assert( pConvoy_ );
    pConvoy_->Form(); // Création du pion
    pConvoy_->ActivateConvoyMission();
    nTimer_ = 0;
    SetState( eConvoyGoingToLoadingPoint );
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockConsign::ConvoyLoad
// Created: NLD 2005-02-10
// -----------------------------------------------------------------------------
bool PHY_SupplyStockConsign::ConvoyLoad()
{
    // Appelé depuis scripts
    if( GetState() == eConvoyGoingToLoadingPoint )
        EnterStateConvoyLoading();

    return IsLoadingDone(); // Chargement effectué quand state == "going to unloading point"
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
// Name: PHY_SupplyStockConsign::EnterStateConvoyGoingToUnloadingPoint
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
void PHY_SupplyStockConsign::EnterStateConvoyGoingToUnloadingPoint()
{
    assert( pConvoy_ );
    nTimer_ = 0;
    SetState( eConvoyGoingToUnloadingPoint );
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockConsign::ConvoyUnload
// Created: NLD 2005-02-10
// -----------------------------------------------------------------------------
bool PHY_SupplyStockConsign::ConvoyUnload()
{
    // Appelé depuis scripts
    if( GetState() == eConvoyGoingToUnloadingPoint )
        EnterStateConvoyUnloading();

    return IsUnloadingDone(); // Déchargement / ravitaillement effectué quand state == "going back to forming point"
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
// Name: PHY_SupplyStockConsign::EnterStateConvoyGoingBackToFormingPoint
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
void PHY_SupplyStockConsign::EnterStateConvoyGoingBackToFormingPoint()
{
    nTimer_ = 0;
    SetState( eConvoyGoingBackToFormingPoint );
    DoSupply();
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockConsign::ConvoyEndMission
// Created: NLD 2005-02-10
// -----------------------------------------------------------------------------
void PHY_SupplyStockConsign::ConvoyEndMission()
{
    // Appelé depuis scripts
    EnterStateFinished();
    assert( pConvoy_ );
    pConvoy_->DesactivateConvoyMission();
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

    pConvoy_->EmptyOut();
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockConsign::Update
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
bool PHY_SupplyStockConsign::Update()
{
    assert( pConvoy_ );

    if( --nTimer_ > 0 )
        return GetState() == eFinished;

    switch( GetState() )
    {
        case eConvoyWaitingForTransporters  : if( pConvoy_->ReserveTransporters() )  EnterStateConvoyForming                (); break;
        case eConvoyForming                 :                                        EnterStateConvoyGoingToLoadingPoint    (); break;
        case eConvoyGoingToLoadingPoint     :                                                                                   break; // Transition gérée par scripts
        case eConvoyLoading                 :                                        EnterStateConvoyGoingToUnloadingPoint  (); break;
        case eConvoyGoingToUnloadingPoint   :                                                                                   break; // Transition gérée par scripts
        case eConvoyUnloading               :                                        EnterStateConvoyGoingBackToFormingPoint(); break;
        case eConvoyGoingBackToFormingPoint :                                                                                   break; // Transition gérée par scripts
        case eFinished                      :                                                                                   break;
        default:
            assert( false );
    }
    return GetState() == eFinished;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockConsign::SendFullState
// Created: NLD 2005-02-10
// -----------------------------------------------------------------------------
void PHY_SupplyStockConsign::SendFullState( client::LogSupplyHandlingUpdate& asn ) const
{
    PHY_SupplyConsign_ABC::SendFullState( asn );

    assert( pConvoy_ );
//    asn().set_oid_pion_convoyantPresent( 1 );
    asn().mutable_convoying_unit()->set_id( pConvoy_->GetPionConvoy() ? pConvoy_->GetPionConvoy()->GetID() : 0 );
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockConsign::IsLoadingDone
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
bool PHY_SupplyStockConsign::IsLoadingDone() const
{
    return GetState() > eConvoyLoading;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockConsign::IsUnloadingDone
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
bool PHY_SupplyStockConsign::IsUnloadingDone() const
{
    return GetState() > eConvoyUnloading;
}

