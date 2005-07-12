// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Dotations/PHY_DotationStockContainer.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 11:48 $
// $Revision: 3 $
// $Workfile: PHY_DotationStockContainer.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "PHY_DotationStockContainer.h"

#include "PHY_DotationCategory.h"
#include "PHY_DotationCapacity.h"
#include "PHY_DotationStock.h"
#include "PHY_DotationType.h"
#include "Entities/Agents/Roles/Logistic/Supply/PHY_SupplyStockRequestContainer.h"
#include "Entities/Agents/Roles/Logistic/Supply/PHY_RolePionLOG_Supply.h"
#include "Network/NET_ASN_Messages.h"

BOOST_CLASS_EXPORT_GUID( PHY_DotationStockContainer, "PHY_DotationStockContainer" )

// -----------------------------------------------------------------------------
// Name: PHY_DotationStockContainer constructor
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
PHY_DotationStockContainer::PHY_DotationStockContainer( PHY_RolePionLOG_Supply& roleSupply )
    : pRoleSupply_   ( &roleSupply )
    , stocks_       ()   
    , stocksChanged_()
{
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStockContainer constructor
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
PHY_DotationStockContainer::PHY_DotationStockContainer()
    : pRoleSupply_  ( 0 )
    , stocks_       ()
    , stocksChanged_()
{
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStockContainer destructor
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
PHY_DotationStockContainer::~PHY_DotationStockContainer()
{
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
namespace boost
{
    namespace serialization
    {
        template< typename Archive >
        void serialize( Archive& file, PHY_DotationStockContainer::T_StockMap& map, const uint nVersion )
        {
            split_free( file, map, nVersion );
        }
        
        template< typename Archive >
        void save( Archive& file, const PHY_DotationStockContainer::T_StockMap& map, const uint )
        {
            file << map.size();
            for ( PHY_DotationStockContainer::CIT_StockMap it = map.begin(); it != map.end(); ++it )
            {
                file << it->first->GetMosID();
                file << it->second;
            }
        }
        
        template< typename Archive >
        void load( Archive& file, PHY_DotationStockContainer::T_StockMap& map, const uint )
        {
            uint nNbr;
            file >> nNbr;
            while ( nNbr-- )
            {
                uint nID;
                
                file >> nID;
                const PHY_DotationCategory* pDotationCategory = PHY_DotationType::FindDotationCategory( nID );
                
                PHY_DotationStock* pDotationStock;
                file >> pDotationStock;
                
                map[ pDotationCategory ] = pDotationStock;
            }
        }
        
        template< typename Archive >
        void serialize( Archive& file, PHY_DotationStockContainer::T_StockSet& set, const uint nVersion )
        {
            split_free( file, set, nVersion );
        }
        
        template< typename Archive >
        void save( Archive& file, const PHY_DotationStockContainer::T_StockSet& set, const uint )
        {
            file << set.size();
            for ( PHY_DotationStockContainer::CIT_StockSet it = set.begin(); it != set.end(); ++it )
                file << *it;
        }
        
        template< typename Archive >
        void load( Archive& file, PHY_DotationStockContainer::T_StockSet& set, const uint )
        {
            uint nNbr;
            file >> nNbr;
            while ( nNbr-- )
            {
                PHY_DotationStock* pStock;
                file >> pStock;
                set.insert( pStock );
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStockContainer::serialize
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_DotationStockContainer::serialize( Archive& file, const uint )
{
    file & pRoleSupply_
         & stocks_
         & stocksChanged_;
}

// =============================================================================
// INIT
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_DotationStockContainer::ReadValues
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
void PHY_DotationStockContainer::ReadValues( MIL_InputArchive& archive )
{
    if ( !archive.BeginList( "Stocks", MIL_InputArchive::eNothing ) )
        return;

    while( archive.NextListElement() )
    {
        archive.Section( "Dotation" );

        std::string strDotationType;
        archive.ReadAttribute( "nom", strDotationType  );

        const PHY_DotationType* pDotationType = PHY_DotationType::FindDotationType( strDotationType );
        if( !pDotationType )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown dotation type", archive.GetContext() );

        archive.BeginList( "Categories" );
        while ( archive.NextListElement() )
        {
            archive.Section( "Categorie" );

            std::string strCategoryName;
            archive.ReadAttribute( "nom", strCategoryName );

            const PHY_DotationCategory* pDotationCategory = pDotationType->FindDotationCategory( strCategoryName );
            if( !pDotationCategory )
                throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown dotation category", archive.GetContext() );

            CIT_StockMap itDotationStock = stocks_.find( pDotationCategory );
            if( itDotationStock == stocks_.end() )
                throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Dotation not in basic unit type", archive.GetContext() );

            itDotationStock->second->ReadValue( archive );

            archive.EndSection(); // Categorie
        }
        archive.EndList(); // Categories    
        archive.EndSection(); // Dotation
    }
    archive.EndList(); // Stocks
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_DotationStockContainer::AddCapacity
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
void PHY_DotationStockContainer::AddCapacity( const PHY_DotationCapacity& capacity )
{
    PHY_DotationStock*& pStock = stocks_[ &capacity.GetCategory() ];
    assert( !pStock );
    pStock = new PHY_DotationStock( *this, capacity );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStockContainer::AddReservation
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
MT_Float PHY_DotationStockContainer::AddReservation( const PHY_DotationCategory& category, MT_Float rNbr )
{
    CIT_StockMap it = stocks_.find( &category );
    assert( it != stocks_.end() );
    return it->second->AddReservation( rNbr );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStockContainer::RemoveReservation
// Created: NLD 2005-02-11
// -----------------------------------------------------------------------------
void PHY_DotationStockContainer::RemoveReservation( const PHY_DotationCategory& category, MT_Float rNbr )
{
    CIT_StockMap it = stocks_.find( &category );
    assert( it != stocks_.end() );
    it->second->RemoveReservation( rNbr );
}


// -----------------------------------------------------------------------------
// Name: PHY_DotationStockContainer::GetValue
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
MT_Float PHY_DotationStockContainer::GetValue( const PHY_DotationCategory& category ) const
{
    CIT_StockMap it = stocks_.find( &category );
    if( it == stocks_.end() )
        return 0.;
    return it->second->GetValue();
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStockContainer::GetStock
// Created: NLD 2005-02-04
// -----------------------------------------------------------------------------
PHY_DotationStock* PHY_DotationStockContainer::GetStock( const PHY_DotationCategory& category ) const
{
    CIT_StockMap it = stocks_.find( &category );
    if( it == stocks_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStockContainer::Resupply
// Created: NLD 2005-02-03
// -----------------------------------------------------------------------------
void PHY_DotationStockContainer::Resupply()
{
    for( CIT_StockMap it = stocks_.begin(); it != stocks_.end(); ++it )
        it->second->Resupply();
}

// =============================================================================
// LOG
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_DotationStockContainer::NotifySupplyNeeded
// Created: NLD 2005-01-21
// -----------------------------------------------------------------------------
void PHY_DotationStockContainer::NotifySupplyNeeded( const PHY_DotationCategory& dotationCategory ) const
{   
    assert( pRoleSupply_ );
    pRoleSupply_->NotifySupplyNeeded( dotationCategory );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStockContainer::FillSupplyRequest
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
void PHY_DotationStockContainer::FillSupplyRequest( PHY_SupplyStockRequestContainer& supplyRequest ) const
{
    for( CIT_StockMap it = stocks_.begin(); it != stocks_.end(); ++it )
        supplyRequest.AddStock( *it->second );
}

// =============================================================================
// NETWORK
// =============================================================================

//------------------------------------------------------------------------------
// Name: PHY_DotationStockContainer::SendChangedState
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
void PHY_DotationStockContainer::SendChangedState( NET_ASN_MsgLogRavitaillementEtat& asn ) const
{
    asn.GetAsnMsg().stocks.n = stocksChanged_.size();
    if( stocksChanged_.empty() )
        return;

    ASN1T_DotationStock* pResources = new ASN1T_DotationStock[ stocksChanged_.size() ];
    uint i = 0;
    for( CIT_StockSet itStock = stocksChanged_.begin(); itStock != stocksChanged_.end(); ++itStock, ++i )
    {
        const PHY_DotationStock& dotation = **itStock;
        ASN1T_DotationStock& asnRessource = pResources[i];
        asnRessource.ressource_id         = dotation.GetCategory().GetMosID();
        asnRessource.quantite_disponible  = (uint)dotation.GetValue();
    }
    
    asn.GetAsnMsg().stocks.elem     = pResources;
    asn.GetAsnMsg().m.stocksPresent = 1;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStockContainer::SendFullState
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
void PHY_DotationStockContainer::SendFullState( NET_ASN_MsgLogRavitaillementEtat& asn ) const
{
    asn.GetAsnMsg().stocks.n = stocks_.size();

    if( stocks_.empty() )
        return;

    ASN1T_DotationStock* pResources = new ASN1T_DotationStock[ stocks_.size() ];
    uint i = 0;

    for( CIT_StockMap itStock = stocks_.begin(); itStock != stocks_.end(); ++itStock )
    {
        const PHY_DotationStock& dotationStock = *itStock->second;
        ASN1T_DotationStock& asnRessource      = pResources[ i++ ];
        asnRessource.ressource_id              = dotationStock.GetCategory().GetMosID();
        asnRessource.quantite_disponible       = (uint)dotationStock.GetValue();
    }

    asn.GetAsnMsg().stocks.elem     = pResources;
    asn.GetAsnMsg().m.stocksPresent = 1;
}
