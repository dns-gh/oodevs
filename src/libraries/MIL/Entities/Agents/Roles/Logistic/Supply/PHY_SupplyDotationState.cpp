// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Supply/PHY_SupplyDotationState.cpp $
// $Author: Jvt $
// $Modtime: 11/04/05 13:32 $
// $Revision: 3 $
// $Workfile: PHY_SupplyDotationState.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "PHY_SupplyDotationState.h"

#include "PHY_RolePion_Supply.h"
#include "PHY_SupplyConsign_ABC.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Network/NET_ASN_Messages.h"

BOOST_CLASS_EXPORT_GUID( PHY_SupplyDotationState, "PHY_SupplyDotationState" )

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationState::PHY_SupplyDotationState
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
PHY_SupplyDotationState::PHY_SupplyDotationState( MIL_Automate& suppliedAutomate )
    : PHY_SupplyState_ABC()
    , pSuppliedAutomate_ ( &suppliedAutomate )
    , pConsign_          ( 0 )
    , bConsignChanged_   ( true )
    , bRequestsChanged_  ( true )
    , requests_          ()
{
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

// =============================================================================
// CHECKPOINTS
// =============================================================================
namespace boost
{
    namespace serialization
    {
        template< typename Archive >
        inline
        void serialize( Archive& file, PHY_SupplyDotationState::T_RequestMap& map, const uint nVersion )
        {
            split_free( file, map, nVersion );
        }
        
        template< typename Archive >
        void save( Archive& file, const PHY_SupplyDotationState::T_RequestMap& map, const uint )
        {
            file << map.size();
            for ( PHY_SupplyDotationState::CIT_RequestMap it = map.begin(); it != map.end(); ++it )
            {
                file << it->first->GetMosID();
                file << it->second;
            }   
        }
        
        template< typename Archive >
        void load( Archive& file, PHY_SupplyDotationState::T_RequestMap& map, const uint )
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
// Name: PHY_SupplyDotationState::serialize
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
template< typename Archive > 
void PHY_SupplyDotationState::serialize( Archive& file, const uint )
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
void PHY_SupplyDotationState::RemoveConvoyedMerchandise( const PHY_DotationCategory& dotationCategory, MT_Float rNbrDotations )
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
void PHY_SupplyDotationState::AddConvoyedMerchandise( const PHY_DotationCategory& dotationCategory, MT_Float rNbrDotations )
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

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationState::SendMsgCreation
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void PHY_SupplyDotationState::SendMsgCreation() const
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
// Name: PHY_SupplyDotationState::SendFullState
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void PHY_SupplyDotationState::SendFullState() const
{
    assert( pSuppliedAutomate_ );

    SendMsgCreation();
    NET_ASN_MsgLogRavitaillementTraitementUpdate asn;
    asn.GetAsnMsg().oid_consigne = nID_;
    asn.GetAsnMsg().oid_automate = pSuppliedAutomate_->GetID();
    if( pConsign_ )
        pConsign_->SendFullState( asn );
    else
        PHY_SupplyConsign_ABC::SendDefaultState( asn );
    asn.Send();
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

    NET_ASN_MsgLogRavitaillementTraitementUpdate asn;
    asn.GetAsnMsg().oid_consigne = nID_;
    asn.GetAsnMsg().oid_automate = pSuppliedAutomate_->GetID();

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
// Name: PHY_SupplyDotationState::SendMsgDestruction
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void PHY_SupplyDotationState::SendMsgDestruction() const
{
    assert( pSuppliedAutomate_ );

    NET_ASN_MsgLogRavitaillementTraitementDestruction asn;
    asn.GetAsnMsg().oid_consigne = nID_;
    asn.GetAsnMsg().oid_automate = pSuppliedAutomate_->GetID();
    asn.Send();
}

