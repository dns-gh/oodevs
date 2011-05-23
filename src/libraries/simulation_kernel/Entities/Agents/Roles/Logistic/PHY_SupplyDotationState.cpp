// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/PHY_SupplyDotationState.cpp $
// $Author: Jvt $
// $Modtime: 11/04/05 13:32 $
// $Revision: 3 $
// $Workfile: PHY_SupplyDotationState.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_SupplyDotationState.h"
#include "PHY_SupplyConsign_ABC.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include <boost/serialization/split_free.hpp>
#include <boost/serialization/vector.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_SupplyDotationState )

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationState::PHY_SupplyDotationState
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
PHY_SupplyDotationState::PHY_SupplyDotationState( MIL_Automate& suppliedAutomate, MIL_AutomateLOG& convoyer  )
    : PHY_SupplyState_ABC()
    , pSuppliedAutomate_ ( &suppliedAutomate )
    , pConvoyer_         ( &convoyer )
    , pConsign_          ( 0 )
    , bConsignChanged_   ( true )
    , bRequestsChanged_  ( true )
    , requests_          ()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationState constructor
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
PHY_SupplyDotationState::PHY_SupplyDotationState()
    : PHY_SupplyState_ABC()
    , pSuppliedAutomate_ ( 0 )
    , pConsign_          ( 0 )
    , bConsignChanged_   ( true )
    , bRequestsChanged_  ( true )
    , requests_          ()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationState destructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_SupplyDotationState::~PHY_SupplyDotationState()
{
    if( !requests_.empty() )
        SendMsgDestruction();
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationState::Clean
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void PHY_SupplyDotationState::Clean()
{
    bConsignChanged_  = false;
    bRequestsChanged_ = false;
    if( pConsign_ )
        pConsign_->Clean();
}

namespace boost
{
    namespace serialization
    {
        template< typename Archive >
        inline
        void serialize( Archive& file, PHY_SupplyDotationState::T_RequestMap& map, const unsigned int nVersion )
        {
            split_free( file, map, nVersion );
        }

        template< typename Archive >
        void save( Archive& file, const PHY_SupplyDotationState::T_RequestMap& map, const unsigned int )
        {
            std::size_t size = map.size();
            file << size;
            for ( PHY_SupplyDotationState::CIT_RequestMap it = map.begin(); it != map.end(); ++it )
            {
                unsigned id = it->first->GetMosID();
                file << id;
                file << it->second;
            }
        }

        template< typename Archive >
        void load( Archive& file, PHY_SupplyDotationState::T_RequestMap& map, const unsigned int )
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
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationState::serialize
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_SupplyDotationState::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< PHY_SupplyState_ABC >( *this )
         & pSuppliedAutomate_
         & pConvoyer_
         & pConsign_
         & requests_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationState::GetMerchandiseToConvoy
// Created: NLD 2005-07-13
// -----------------------------------------------------------------------------
void PHY_SupplyDotationState::GetMerchandiseToConvoy( T_MerchandiseToConvoyMap& container ) const
{
    container.clear();
    for( CIT_RequestMap it = requests_.begin(); it != requests_.end(); ++it )
        container[ it->first ] += it->second.GetTotalReservedValue();
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationState::RemoveConvoyedMerchandise
// Created: NLD 2005-02-10
// -----------------------------------------------------------------------------
void PHY_SupplyDotationState::RemoveConvoyedMerchandise( const PHY_DotationCategory& dotationCategory, double rNbrDotations )
{
    IT_RequestMap it = requests_.find( &dotationCategory );
    if( it == requests_.end() )
        return;

    it->second.RemoveConvoyedMerchandise( rNbrDotations );
    bRequestsChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationState::AddConvoyedMerchandise
// Created: NLD 2005-12-15
// -----------------------------------------------------------------------------
void PHY_SupplyDotationState::AddConvoyedMerchandise( const PHY_DotationCategory& dotationCategory, double rNbrDotations )
{
    IT_RequestMap it = requests_.find( &dotationCategory );
    if( it == requests_.end() )
        return;

    it->second.AddConvoyedMerchandise( rNbrDotations );
    bRequestsChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationState::CancelMerchandiseOverheadReservation
// Created: NLD 2005-12-15
// -----------------------------------------------------------------------------
void PHY_SupplyDotationState::CancelMerchandiseOverheadReservation()
{
    for( IT_RequestMap it = requests_.begin(); it != requests_.end(); ++it )
        it->second.CancelMerchandiseOverheadReservation();
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockState::Supply
// Created: NLD 2005-01-28
// -----------------------------------------------------------------------------
void PHY_SupplyDotationState::Supply() const
{
    assert( pSuppliedAutomate_ );
    pSuppliedAutomate_->NotifyDotationSupplied( *this );
    for( CIT_RequestMap it = requests_.begin(); it != requests_.end(); ++it )
        it->second.Supply();
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationState::SendMsgCreation
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void PHY_SupplyDotationState::SendMsgCreation() const
{
    assert( !requests_.empty() );
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

    asn().mutable_dotations()->Clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationState::SendFullState
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void PHY_SupplyDotationState::SendFullState() const
{
    assert( pSuppliedAutomate_ );
    SendMsgCreation();
    client::LogSupplyHandlingUpdate asn;
    asn().mutable_request()->set_id( nID_ );
    asn().mutable_consumer()->set_id( pSuppliedAutomate_->GetID() );
    if( pConsign_ )
        pConsign_->SendFullState( asn );
    else
        PHY_SupplyConsign_ABC::SendDefaultState( asn );
    asn.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationState::SendChangedState
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void PHY_SupplyDotationState::SendChangedState() const
{
    if( !( bConsignChanged_ || ( pConsign_ && pConsign_->HasChanged() ) || bRequestsChanged_ ) )
        return;
    assert( pSuppliedAutomate_ );
    client::LogSupplyHandlingUpdate asn;
    asn().mutable_request()->set_id(nID_ );
    asn().mutable_consumer()->set_id( pSuppliedAutomate_->GetID() );
    if( bConsignChanged_ || ( pConsign_ && pConsign_->HasChanged() ) )
    {
        if( pConsign_ )
            pConsign_->SendChangedState( asn );
        else
            PHY_SupplyConsign_ABC::SendDefaultState( asn );
    }
    if( bRequestsChanged_ )
        for( CIT_RequestMap it = requests_.begin(); it != requests_.end(); ++it )
            it->second.Serialize( *asn().mutable_dotations()->add_elem() );
    asn.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationState::SendMsgDestruction
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void PHY_SupplyDotationState::SendMsgDestruction() const
{
    assert( pSuppliedAutomate_ );
    client::LogSupplyHandlingDestruction asn;
    asn().mutable_request()->set_id( nID_ );
    asn().mutable_consumer()->set_id( pSuppliedAutomate_->GetID() );
    asn.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationState::SetConsign
// Created: NLD 2004-12-24
// -----------------------------------------------------------------------------
void PHY_SupplyDotationState::SetConsign( PHY_SupplyConsign_ABC* pConsign )
{
    if( pConsign == pConsign_ )
        return;

    pConsign_        = pConsign;
    bConsignChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationState::GetSuppliedAutomate
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
const MIL_Automate& PHY_SupplyDotationState::GetSuppliedAutomate() const
{
    assert( pSuppliedAutomate_ );
    return *pSuppliedAutomate_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationState::IsSupplying
// Created: NLD 2005-02-02
// -----------------------------------------------------------------------------
bool PHY_SupplyDotationState::IsSupplying( const PHY_DotationCategory& dotationCategory ) const
{
    return requests_.find( &dotationCategory ) != requests_.end();
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationState::AddRequest
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
void PHY_SupplyDotationState::AddRequest( const PHY_SupplyDotationRequest& request )
{
    requests_[ &request.GetDotationCategory() ] = request;
    bRequestsChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationState::GetConvoyer
// Created: AHC 2010-09-28
// -----------------------------------------------------------------------------
MIL_AutomateLOG& PHY_SupplyDotationState::GetConvoyer() const
{
    assert( pConvoyer_ );
    return *pConvoyer_;
}
