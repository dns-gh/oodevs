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

#include "MIL_Pch.h"

#include "PHY_SupplyStockState.h"

#include "PHY_RolePion_Supply.h"
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
PHY_SupplyStockState::PHY_SupplyStockState( MIL_AutomateLOG& suppliedAutomate )
    : PHY_SupplyState_ABC()
    , pSuppliedAutomate_ ( &suppliedAutomate )
    , pConsign_          ( 0 )
    , bConsignChanged_   ( true )
    , bRequestsChanged_  ( true )
    , requests_          ()
{
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockState constructor
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
PHY_SupplyStockState::PHY_SupplyStockState()
    : PHY_SupplyState_ABC()
    , pSuppliedAutomate_ ( 0 )
    , pConsign_          ( 0 )
    , bConsignChanged_   ( true )
    , bRequestsChanged_  ( true )
    , requests_          ()
{
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
            file << map.size();
            for ( PHY_SupplyStockState::CIT_RequestMap it = map.begin(); it != map.end(); ++it )
            {
                file << it->first->GetMosID();
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
         & pConsign_
         & requests_;
}

// =============================================================================
// OPERATIONS
// =============================================================================

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
// Name: PHY_SupplyStockState::Supply
// Created: NLD 2005-01-28
// -----------------------------------------------------------------------------
void PHY_SupplyStockState::Supply() const
{
    assert( pSuppliedAutomate_ );
    pSuppliedAutomate_->NotifyStockSupplied( *this );
    for( CIT_RequestMap it = requests_.begin(); it != requests_.end(); ++it )
        it->second.Supply();
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
// Name: PHY_SupplyStockState::RemoveConvoyedStock
// Created: NLD 2005-02-10
// -----------------------------------------------------------------------------
void PHY_SupplyStockState::RemoveConvoyedStock( const PHY_DotationCategory& dotationCategory, MT_Float rNbrDotations )
{
    IT_RequestMap it = requests_.find( &dotationCategory );
    if( it == requests_.end() )
        return;
    
    it->second.RemoveConvoyedStock( rNbrDotations );
        bRequestsChanged_ = true;
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockState::SendMsgCreation
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void PHY_SupplyStockState::SendMsgCreation() const
{
    assert( !requests_.empty() );
    assert( pSuppliedAutomate_ );

    NET_ASN_MsgLogRavitaillementTraitementCreation asn;
    asn.GetAsnMsg().oid_consigne    = nID_;
    asn.GetAsnMsg().oid_automate    = pSuppliedAutomate_->GetID();

    ASN1T_DemandeDotation* pAsnRequests = new ASN1T_DemandeDotation[ requests_.size() ];
    uint i = 0;
    for( CIT_RequestMap it = requests_.begin(); it != requests_.end(); ++it, ++i )
    {
        ASN1T_DemandeDotation& asnDemande = pAsnRequests[ i ];
        it->second.Serialize( asnDemande );
    }

    asn.GetAsnMsg().dotations.n    = requests_.size();
    asn.GetAsnMsg().dotations.elem = pAsnRequests;
    asn.Send();

    delete [] asn.GetAsnMsg().dotations.elem;
}


// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockState::SendFullState
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void PHY_SupplyStockState::SendFullState() const
{
    assert( pSuppliedAutomate_ );

    SendMsgCreation();
    NET_ASN_MsgLogRavitaillementTraitementUpdate asn;
    asn.GetAsnMsg().oid_consigne = nID_;
    asn.GetAsnMsg().oid_automate = pSuppliedAutomate_->GetID();
    if( pConsign_ )
        pConsign_->SendFullState( asn );
    else
    {
        asn.GetAsnMsg().m.oid_pion_log_traitantPresent = 1;
        asn.GetAsnMsg().m.oid_pion_convoiPresent       = 1;
        asn.GetAsnMsg().m.etatPresent                  = 1;
        asn.GetAsnMsg().oid_pion_convoi                = 0;
        asn.GetAsnMsg().oid_pion_log_traitant          = 0;
        asn.GetAsnMsg().etat                           = EnumLogRavitaillementTraitementEtat::termine;
    }
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

    NET_ASN_MsgLogRavitaillementTraitementUpdate asn;
    asn.GetAsnMsg().oid_consigne = nID_;
    asn.GetAsnMsg().oid_automate = pSuppliedAutomate_->GetID();

    if( bConsignChanged_ || ( pConsign_ && pConsign_->HasChanged() ) )
    {
        if( pConsign_ )
            pConsign_->SendChangedState( asn );
        else
        {
            asn.GetAsnMsg().m.oid_pion_log_traitantPresent = 1;
            asn.GetAsnMsg().m.etatPresent                  = 1;
            asn.GetAsnMsg().oid_pion_log_traitant          = 0;
            asn.GetAsnMsg().etat                           = EnumLogRavitaillementTraitementEtat::termine;
        }
    }

    if( bRequestsChanged_ )
    {
        assert( !requests_.empty() );

        asn.GetAsnMsg().m.dotationsPresent = 1;

        ASN1T_DemandeDotation* pAsnRequests = new ASN1T_DemandeDotation[ requests_.size() ];
        uint i = 0;
        for( CIT_RequestMap it = requests_.begin(); it != requests_.end(); ++it, ++i )
        {
            ASN1T_DemandeDotation& asnDemande = pAsnRequests[ i ];
            it->second.Serialize( asnDemande );
        }

        asn.GetAsnMsg().dotations.n    = requests_.size();
        asn.GetAsnMsg().dotations.elem = pAsnRequests;   
    }

    asn.Send();
    if( asn.GetAsnMsg().m.dotationsPresent && asn.GetAsnMsg().dotations.n > 0 )
        delete [] asn.GetAsnMsg().dotations.elem;
}
    
// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockState::SendMsgDestruction
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void PHY_SupplyStockState::SendMsgDestruction() const
{
    assert( pSuppliedAutomate_ );

    NET_ASN_MsgLogRavitaillementTraitementDestruction asn;
    asn.GetAsnMsg().oid_consigne = nID_;
    asn.GetAsnMsg().oid_automate = pSuppliedAutomate_->GetID();
    asn.Send();
}
