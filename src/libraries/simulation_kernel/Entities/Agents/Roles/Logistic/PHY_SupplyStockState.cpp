// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/PHY_SupplyStockState.cpp $
// $Author: Jvt $
// $Modtime: 11/04/05 13:46 $
// $Revision: 3 $
// $Workfile: PHY_SupplyStockState.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_SupplyStockState.h"
#include "PHY_SupplyConsign_ABC.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include <boost/serialization/split_free.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/foreach.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_SupplyStockState )

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockState::PHY_SupplyStockState
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
PHY_SupplyStockState::PHY_SupplyStockState( MIL_AutomateLOG& convoyer, bool bConsumeQuota )
    : PHY_SupplyState_ABC()
    , pConvoyer_         ( &convoyer )
    , bConsumeQuota_     ( bConsumeQuota )
    , pConsign_          ( 0 )
    , bConsignChanged_   ( true )
    , bRequestsChanged_  ( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockState constructor
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
PHY_SupplyStockState::PHY_SupplyStockState()
    : PHY_SupplyState_ABC()
    , pConvoyer_         ( 0 )
    , bConsumeQuota_     ( false )
    , pConsign_          ( 0 )
    , bConsignChanged_   ( true )
    , bRequestsChanged_  ( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockState destructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_SupplyStockState::~PHY_SupplyStockState()
{
    if( !requestsQueued_.empty() )
        SendMsgDestruction();
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockState::Clean
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void PHY_SupplyStockState::Clean()
{
    bConsignChanged_  = false;
    bRequestsChanged_ = false;
    if( pConsign_ )
        pConsign_->Clean();
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
namespace boost
{
  /*  namespace serialization
    {
        template< typename Archive >
        inline
        void serialize( Archive& file, PHY_SupplyStockState::T_RequestMap& map, const unsigned int nVersion )
        {
            split_free( file, map, nVersion );
        }

        template< typename Archive >
        void save( Archive& file, const PHY_SupplyStockState::T_RequestMap& map, const unsigned int )
        {
            std::size_t size = map.size();
            file << size;
            for ( PHY_SupplyStockState::CIT_RequestMap it = map.begin(); it != map.end(); ++it )
            {
                unsigned id = it->first->GetMosID();
                file << id;
                file << it->second;
            }
        }

        template< typename Archive >
        void load( Archive& file, PHY_SupplyStockState::T_RequestMap& map, const unsigned int )
        {
            std::size_t nNbr;
            file >> nNbr;
            while ( nNbr-- )
            {
                unsigned int nCategoryID;

                file >> nCategoryID;
                file >> map[ PHY_DotationType::FindDotationCategory( nCategoryID ) ];
            }
        }
    }*/
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockState::serialize
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_SupplyStockState::serialize( Archive& file, const unsigned int )
{
    /*file & boost::serialization::base_object< PHY_SupplyState_ABC >( *this )
         & pSuppliedAutomate_
         & const_cast< bool& >( bConsumeQuota_ )
         & pConsign_
         & requests_;*/
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockState::GetMerchandiseToConvoy
// Created: NLD 2005-07-13
// -----------------------------------------------------------------------------
void PHY_SupplyStockState::GetMerchandiseToConvoy( T_MerchandiseToConvoyMap& container ) const
{
    container.clear();
    BOOST_FOREACH( const T_AutomatRequests::value_type& automatRequest, requestsQueued_ )
    {
        BOOST_FOREACH( const T_Requests::value_type& request, automatRequest.second )
            container[ request.first ] += request.second.GetTotalReservedValue();
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockState::RemoveConvoyedMerchandise
// Created: NLD 2005-02-10
// -----------------------------------------------------------------------------
void PHY_SupplyStockState::RemoveConvoyedMerchandise( const PHY_DotationCategory& dotationCategory, double rNbrDotations )
{
    /*IT_RequestMap it = requests_.find( &dotationCategory );
    if( it == requests_.end() )
        return;

    it->second.RemoveConvoyedMerchandise( rNbrDotations );
    bRequestsChanged_ = true;*/
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockState::AddConvoyedMerchandise
// Created: NLD 2005-12-15
// -----------------------------------------------------------------------------
void PHY_SupplyStockState::AddConvoyedMerchandise( const PHY_DotationCategory& dotationCategory, double rNbrDotations )
{
    /*IT_RequestMap it = requests_.find( &dotationCategory );
    if( it == requests_.end() )
        return;

    it->second.AddConvoyedMerchandise( rNbrDotations );
    bRequestsChanged_ = true;*/
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockState::CancelMerchandiseOverheadReservation
// Created: NLD 2005-12-15
// -----------------------------------------------------------------------------
void PHY_SupplyStockState::CancelMerchandiseOverheadReservation()
{
    /*for( IT_RequestMap it = requests_.begin(); it != requests_.end(); ++it )
        it->second.CancelMerchandiseOverheadReservation();*/
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockState::Supply
// Created: NLD 2005-01-28
// -----------------------------------------------------------------------------
void PHY_SupplyStockState::Supply( MIL_Automate& supplied, T_MerchandiseToConvoyMap& stockSupplied )
{
    T_AutomatRequests::iterator it = requestsQueued_.find( &supplied );
    if( it == requestsQueued_.end() )
        return;

    supplied.NotifyStockSupplied( *this ); //$$$$$ A REMPLACER ??
    T_Requests& requests = it->second;
    BOOST_FOREACH( T_Requests::value_type& data, requests )
    {
        PHY_SupplyStockRequest& stockRequest = data.second;
        stockRequest.Supply();
        stockSupplied[ &stockRequest.GetDotationCategory() ] += stockRequest.GetTotalConvoyedValue();
        if( bConsumeQuota_ )
            supplied.ConsumeQuota( pConsign_->GetSupplier(), stockRequest.GetDotationCategory(), stockRequest.GetTotalConvoyedValue() );
    }
    requestsDone_.insert( *it );
    requestsQueued_.erase( it );
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockState::CancelSupply
// Created: NLD 2005-02-11
// -----------------------------------------------------------------------------
void PHY_SupplyStockState::CancelSupply()
{
    /*assert( pSuppliedAutomate_ );
    pSuppliedAutomate_->NotifyStockSupplyCanceled( *this );
    for( IT_RequestMap it = requests_.begin(); it != requests_.end(); ++it )
        it->second.Cancel();*/
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockState::SetConsign
// Created: NLD 2004-12-24
// -----------------------------------------------------------------------------
void PHY_SupplyStockState::SetConsign( PHY_SupplyConsign_ABC* pConsign )
{
    if( pConsign == pConsign_ )
        return;

    pConsign_        = pConsign;
    bConsignChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockState::IsSupplying
// Created: NLD 2005-02-02
// -----------------------------------------------------------------------------
bool PHY_SupplyStockState::IsSupplying( const MIL_Automate& supplied, const PHY_DotationCategory& dotationCategory ) const
{
    T_AutomatRequests::const_iterator it = requestsQueued_.find( const_cast< MIL_Automate* >( &supplied ) );
    if( it == requestsQueued_.end() )
        return false;
    return it->second.find( &dotationCategory ) != it->second.end();
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockState::AddRequest
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
void PHY_SupplyStockState::AddRequest( MIL_Automate& supplied, const PHY_SupplyStockRequest& request )
{
    requestsQueued_[ &supplied ][ &request.GetDotationCategory() ] = request;
    bRequestsChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockState::GetConvoyer
// Created: AHC 2010-09-28
// -----------------------------------------------------------------------------
MIL_AutomateLOG& PHY_SupplyStockState::GetConvoyer() const
{
    assert( pConvoyer_ );
    return *pConvoyer_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockState::GetNextSupplied
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
MIL_Automate* PHY_SupplyStockState::GetNextSupplied() const
{
    return requestsQueued_.empty() ? 0 : requestsQueued_.begin()->first;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockState::GetWayPointsToGoToSupplied
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
const T_PointVector* PHY_SupplyStockState::GetWayPointsToGoToSupplied( const MIL_Automate& supplied ) const
{
    return 0; //$$ TMP
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockState::GetWayPointsToGoBack
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
const T_PointVector* PHY_SupplyStockState::GetWayPointsToGoBack() const
{
    return 0; //$$ TMP
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockState::SendMsgCreation
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void PHY_SupplyStockState::SendMsgCreation() const
{
/*    assert( !requests_.empty() );
    assert( pSuppliedAutomate_ );

    client::LogSupplyHandlingCreation asn;
    asn().mutable_request()->set_id( nID_ );
    asn().mutable_consumer()->set_id( pSuppliedAutomate_->GetID() );
    asn().set_tick( nCreationTick_ );

    for( CIT_RequestMap it = requests_.begin(); it != requests_.end(); ++it )
    {
        sword::DotationQuery& asnDemande = *asn().mutable_dotations()->add_elem();
        it->second.Serialize( asnDemande );
    }

    asn.Send( NET_Publisher_ABC::Publisher() );
    asn().mutable_dotations()->Clear();*/
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockState::SendFullState
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void PHY_SupplyStockState::SendFullState() const
{
    /*assert( pSuppliedAutomate_ );

    SendMsgCreation();
    client::LogSupplyHandlingUpdate asn;
    asn().mutable_request()->set_id( nID_ );
    asn().mutable_consumer()->set_id( pSuppliedAutomate_->GetID() );
    if( pConsign_ )
        pConsign_->SendFullState( asn );
    else
        PHY_SupplyConsign_ABC::SendDefaultState( asn );
    asn.Send( NET_Publisher_ABC::Publisher() );*/
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockState::SendChangedState
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void PHY_SupplyStockState::SendChangedState() const
{
    /*if( !( bConsignChanged_ || ( pConsign_ && pConsign_->HasChanged() ) || bRequestsChanged_ ) )
        return;

    assert( pSuppliedAutomate_ );

    client::LogSupplyHandlingUpdate asn;
    asn().mutable_request()->set_id( nID_ );
    asn().mutable_consumer()->set_id( pSuppliedAutomate_->GetID() );

    if( bConsignChanged_ || ( pConsign_ && pConsign_->HasChanged() ) )
    {
        if( pConsign_ )
            pConsign_->SendChangedState( asn );
        else
            PHY_SupplyConsign_ABC::SendDefaultState( asn );
    }

    if( bRequestsChanged_ )
    {
        assert( !requests_.empty() );
        for( CIT_RequestMap it = requests_.begin(); it != requests_.end(); ++it )
        {
            sword::DotationQuery& asnDemande = *asn().mutable_dotations()->add_elem();
            it->second.Serialize( asnDemande );
        }
    }
    asn.Send( NET_Publisher_ABC::Publisher() );
    if( asn().has_dotations() && asn().dotations().elem_size() > 0 )
        asn().mutable_dotations()->Clear();*/
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockState::SendMsgDestruction
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void PHY_SupplyStockState::SendMsgDestruction() const
{
    /*assert( pSuppliedAutomate_ );

    client::LogSupplyHandlingDestruction asn;
    asn().mutable_request()->set_id( nID_ );
    asn().mutable_consumer()->set_id( pSuppliedAutomate_->GetID() );
    asn.Send( NET_Publisher_ABC::Publisher() );*/
}


