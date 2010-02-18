// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Supply/PHY_SupplyStockState.cpp $
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
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Network/NET_ASN_Messages.h"

BOOST_CLASS_EXPORT_GUID( PHY_SupplyStockState, "PHY_SupplyStockState" )

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockState::PHY_SupplyStockState
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
PHY_SupplyStockState::PHY_SupplyStockState( MIL_AutomateLOG& suppliedAutomate, bool bPushedFlow )
    : PHY_SupplyState_ABC()
    , pSuppliedAutomate_ ( &suppliedAutomate )
    , bPushedFlow_       ( bPushedFlow )
    , pConsign_          ( 0 )
    , bConsignChanged_   ( true )
    , bRequestsChanged_  ( true )
    , requests_          ()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockState constructor
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
PHY_SupplyStockState::PHY_SupplyStockState()
    : PHY_SupplyState_ABC()
    , pSuppliedAutomate_ ( 0 )
    , bPushedFlow_       ( false )
    , pConsign_          ( 0 )
    , bConsignChanged_   ( true )
    , bRequestsChanged_  ( true )
    , requests_          ()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockState destructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_SupplyStockState::~PHY_SupplyStockState()
{
    if( !requests_.empty() )
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
    namespace serialization
    {
        template< typename Archive >
        inline
        void serialize( Archive& file, PHY_SupplyStockState::T_RequestMap& map, const uint nVersion )
        {
            split_free( file, map, nVersion );
        }
        
        template< typename Archive >
        void save( Archive& file, const PHY_SupplyStockState::T_RequestMap& map, const uint )
        {
            unsigned size = map.size();
            file << size;
            for ( PHY_SupplyStockState::CIT_RequestMap it = map.begin(); it != map.end(); ++it )
            {
                unsigned id = it->first->GetMosID();
                file << id;
                file << it->second;
            }   
        }
        
        template< typename Archive >
        void load( Archive& file, PHY_SupplyStockState::T_RequestMap& map, const uint )
        {
            uint nNbr;
            file >> nNbr;
            while ( nNbr-- )
            {
                uint nCategoryID;
                
                file >> nCategoryID;
                file >> map[ PHY_DotationType::FindDotationCategory( nCategoryID ) ];
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockState::serialize
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
template< typename Archive > 
void PHY_SupplyStockState::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< PHY_SupplyState_ABC >( *this )
         & pSuppliedAutomate_
         & const_cast< bool& >( bPushedFlow_ )
         & pConsign_
         & requests_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockState::GetMerchandiseToConvoy
// Created: NLD 2005-07-13
// -----------------------------------------------------------------------------
void PHY_SupplyStockState::GetMerchandiseToConvoy( T_MerchandiseToConvoyMap& container ) const
{
    container.clear();
    for( CIT_RequestMap it = requests_.begin(); it != requests_.end(); ++it )
        container[ it->first ] += it->second.GetTotalReservedValue();
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockState::RemoveConvoyedMerchandise
// Created: NLD 2005-02-10
// -----------------------------------------------------------------------------
void PHY_SupplyStockState::RemoveConvoyedMerchandise( const PHY_DotationCategory& dotationCategory, MT_Float rNbrDotations )
{
    IT_RequestMap it = requests_.find( &dotationCategory );
    if( it == requests_.end() )
        return;
    
    it->second.RemoveConvoyedMerchandise( rNbrDotations );
    bRequestsChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockState::AddConvoyedMerchandise
// Created: NLD 2005-12-15
// -----------------------------------------------------------------------------
void PHY_SupplyStockState::AddConvoyedMerchandise( const PHY_DotationCategory& dotationCategory, MT_Float rNbrDotations )
{
    IT_RequestMap it = requests_.find( &dotationCategory );
    if( it == requests_.end() )
        return;
    
    it->second.AddConvoyedMerchandise( rNbrDotations );
    bRequestsChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockState::CancelMerchandiseOverheadReservation
// Created: NLD 2005-12-15
// -----------------------------------------------------------------------------
void PHY_SupplyStockState::CancelMerchandiseOverheadReservation()
{
    for( IT_RequestMap it = requests_.begin(); it != requests_.end(); ++it )
        it->second.CancelMerchandiseOverheadReservation();
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockState::Supply
// Created: NLD 2005-01-28
// -----------------------------------------------------------------------------
void PHY_SupplyStockState::Supply() const
{
    assert( pSuppliedAutomate_ );
    pSuppliedAutomate_->NotifyStockSupplied( *this );
    for( CIT_RequestMap it = requests_.begin(); it != requests_.end(); ++it )
    {
        it->second.Supply();
        if( !bPushedFlow_ )
            pSuppliedAutomate_->ConsumeQuota( it->second.GetDotationCategory(), it->second.GetTotalConvoyedValue() );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockState::CancelSupply
// Created: NLD 2005-02-11
// -----------------------------------------------------------------------------
void PHY_SupplyStockState::CancelSupply()
{
    assert( pSuppliedAutomate_ );
    pSuppliedAutomate_->NotifyStockSupplyCanceled( *this );
    for( IT_RequestMap it = requests_.begin(); it != requests_.end(); ++it )
        it->second.Cancel();
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockState::SendMsgCreation
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void PHY_SupplyStockState::SendMsgCreation() const
{
    assert( !requests_.empty() );
    assert( pSuppliedAutomate_ );

    NET_ASN_MsgLogSupplyHandlingCreation asn;
    asn().oid_consigne  = nID_;
    asn().oid_automate  = pSuppliedAutomate_->GetID();
    asn().tick_creation = nCreationTick_;

    ASN1T_DotationQuery* pAsnRequests = new ASN1T_DotationQuery[ requests_.size() ];
    uint i = 0;
    for( CIT_RequestMap it = requests_.begin(); it != requests_.end(); ++it, ++i )
    {
        ASN1T_DotationQuery& asnDemande = pAsnRequests[ i ];
        it->second.Serialize( asnDemande );
    }

    asn().dotations.n    = requests_.size();
    asn().dotations.elem = pAsnRequests;
    asn.Send();

    delete [] asn().dotations.elem;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockState::SendFullState
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void PHY_SupplyStockState::SendFullState() const
{
    assert( pSuppliedAutomate_ );

    SendMsgCreation();
    NET_ASN_MsgLogSupplyHandlingUpdate asn;
    asn().oid_consigne = nID_;
    asn().oid_automate = pSuppliedAutomate_->GetID();
    if( pConsign_ )
        pConsign_->SendFullState( asn );
    else
        PHY_SupplyConsign_ABC::SendDefaultState( asn );
    asn.Send();
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockState::SendChangedState
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void PHY_SupplyStockState::SendChangedState() const
{
    if( !( bConsignChanged_ || ( pConsign_ && pConsign_->HasChanged() ) || bRequestsChanged_ ) )
        return;

    assert( pSuppliedAutomate_ );

    NET_ASN_MsgLogSupplyHandlingUpdate asn;
    asn().oid_consigne = nID_;
    asn().oid_automate = pSuppliedAutomate_->GetID();

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

        asn().m.dotationsPresent = 1;

        ASN1T_DotationQuery* pAsnRequests = new ASN1T_DotationQuery[ requests_.size() ];
        uint i = 0;
        for( CIT_RequestMap it = requests_.begin(); it != requests_.end(); ++it, ++i )
        {
            ASN1T_DotationQuery& asnDemande = pAsnRequests[ i ];
            it->second.Serialize( asnDemande );
        }

        asn().dotations.n    = requests_.size();
        asn().dotations.elem = pAsnRequests;   
    }

    asn.Send();
    if( asn().m.dotationsPresent && asn().dotations.n > 0 )
        delete [] asn().dotations.elem;
}
    
// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockState::SendMsgDestruction
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void PHY_SupplyStockState::SendMsgDestruction() const
{
    assert( pSuppliedAutomate_ );

    NET_ASN_MsgLogSupplyHandlingDestruction asn;
    asn().oid_consigne = nID_;
    asn().oid_automate = pSuppliedAutomate_->GetID();
    asn.Send();
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
// Name: PHY_SupplyStockState::GetSuppliedAutomate
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
MIL_AutomateLOG& PHY_SupplyStockState::GetSuppliedAutomate() const
{
    assert( pSuppliedAutomate_ );
    return *pSuppliedAutomate_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockState::IsPushedFlow
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
bool PHY_SupplyStockState::IsPushedFlow() const
{
    return bPushedFlow_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockState::IsSupplying
// Created: NLD 2005-02-02
// -----------------------------------------------------------------------------
bool PHY_SupplyStockState::IsSupplying( const PHY_DotationCategory& dotationCategory ) const
{
    return requests_.find( &dotationCategory ) != requests_.end();    
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockState::AddRequest
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
void PHY_SupplyStockState::AddRequest( const PHY_SupplyStockRequest& request )
{
    requests_[ &request.GetDotationCategory() ] = request;
    bRequestsChanged_ = true;
}
