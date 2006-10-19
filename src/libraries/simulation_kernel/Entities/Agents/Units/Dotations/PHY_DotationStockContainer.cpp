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

#include "simulation_kernel_pch.h"

#include "PHY_DotationStockContainer.h"

#include "PHY_DotationCategory.h"
#include "PHY_DotationCapacity.h"
#include "PHY_DotationStock.h"
#include "PHY_DotationType.h"
#include "Entities/Agents/Units/PHY_UnitType.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Logistic/Supply/PHY_SupplyStockRequestContainer.h"
#include "Entities/Agents/Roles/Logistic/Supply/PHY_RolePionLOG_Supply.h"
#include "Entities/Specialisations/LOG/MIL_AgentPionLOG_ABC.h"
#include "Entities/RC/MIL_RC.h"
#include "Network/NET_ASN_Messages.h"

BOOST_CLASS_EXPORT_GUID( PHY_DotationStockContainer, "PHY_DotationStockContainer" )

// -----------------------------------------------------------------------------
// Name: PHY_DotationStockContainer constructor
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
PHY_DotationStockContainer::PHY_DotationStockContainer( PHY_RolePionLOG_Supply& roleSupply )
    : pRoleSupply_          ( &roleSupply )
    , stocks_               ()   
    , stocksChanged_        ()
    , bCheckStockCapacities_( false )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStockContainer constructor
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
PHY_DotationStockContainer::PHY_DotationStockContainer()
    : pRoleSupply_          ( 0 )
    , stocks_               ()
    , stocksChanged_        ()
    , bCheckStockCapacities_( false )
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

// -----------------------------------------------------------------------------
// Name: PHY_DotationStockContainer::WriteODB
// Created: NLD 2006-05-29
// -----------------------------------------------------------------------------
void PHY_DotationStockContainer::WriteODB( MT_XXmlOutputArchive& archive ) const
{
    archive.Section( "stocks") ;

    for( CIT_StockMap it = stocks_.begin(); it != stocks_.end(); ++it )
    {
        const PHY_DotationStock& dotationStock = *it->second;

        archive.Section( "Dotation" );
        archive.WriteAttribute( "nom", dotationStock.GetCategory().GetType().GetName() );
        archive.Section( "Categories" );
        archive.Section( "Categorie" );
        archive.WriteAttribute( "nom", dotationStock.GetCategory().GetName() );
        archive.Write( dotationStock.GetValue() );
        archive.EndSection(); // Categories
        archive.EndSection(); // Categorie
        archive.EndSection(); // Dotation
    }
    archive.EndSection(); // Stocks
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
    if ( !archive.BeginList( "stocks", MIL_InputArchive::eNothing ) )
        return;

    while( archive.NextListElement() )
    {
        archive.Section( "dotation" );

        std::string strType;
        archive.ReadAttribute( "name", strType );

        const PHY_DotationCategory* pDotationCategory = PHY_DotationType::FindDotationCategory( strType );
        if( !pDotationCategory )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown dotation type", archive.GetContext() );

        AddStock( *pDotationCategory, archive );

        archive.EndSection(); // dotation
    }
    archive.EndList(); // stocks
}

// =============================================================================
// OPERATIONS
// =============================================================================

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
// Name: PHY_DotationStockContainer::AddStock
// Created: NLD 2006-01-03
// -----------------------------------------------------------------------------
PHY_DotationStock* PHY_DotationStockContainer::AddStock( const PHY_DotationCategory& category, MIL_InputArchive& archive )
{
    MT_Float rValue;
    archive.ReadAttribute( "quantity", rValue, CheckValueGreaterOrEqual( 0. ) );

    const MT_Float rThresholdRatio = pRoleSupply_->GetPion().GetType().GetUnitType().GetStockLogisticThresholdRatio( category.GetLogisticType() );

    PHY_DotationStock*& pStock = stocks_[ &category ];
    if( !pStock )
        pStock = new PHY_DotationStock( *this, category, rThresholdRatio, rValue );
    return pStock;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStockContainer::AddStock
// Created: NLD 2006-03-28
// -----------------------------------------------------------------------------
PHY_DotationStock* PHY_DotationStockContainer::AddStock( const PHY_DotationCategory& category )
{
    PHY_DotationStock*& pStock = stocks_[ &category ];
    if( !pStock )
        pStock = new PHY_DotationStock( *this, category, 0., 0. );
    return pStock;
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

// -----------------------------------------------------------------------------
// Name: PHY_DotationStockContainer::Resupply
// Created: SBO 2005-12-12
// -----------------------------------------------------------------------------
void PHY_DotationStockContainer::Resupply( const PHY_DotationCategory& category, MT_Float rNbr )
{
    PHY_DotationStock*& pStock = stocks_[ &category ];
    if( !pStock )
    {
        const MT_Float rThresholdRatio = pRoleSupply_->GetPion().GetType().GetUnitType().GetStockLogisticThresholdRatio( category.GetLogisticType() );
        pStock = new PHY_DotationStock( *this, category, rThresholdRatio, rNbr );
    }
    else
        pStock->Supply( rNbr - pStock->GetValue() ); // set to rNbr
}

// =============================================================================
// STOCK CAPACITIES CHECKER
// =============================================================================

namespace
{
    struct T_StockData
    {
        MT_Float rVolume_;
        MT_Float rWeight_;
    };
    typedef std::map < const PHY_DotationNature*, T_StockData > T_NatureStockData;
    typedef T_NatureStockData::const_iterator                   CIT_NatureStockData;

    struct sStockChecker
    {
        void operator() ( const PHY_ComposantePion& composante )
        {
            const PHY_DotationNature* pStockTransporterNature = composante.GetType().GetStockTransporterNature();

            if( /*!composante.GetType().CanBePartOfConvoy() && */pStockTransporterNature )
            {
                T_StockData& stockData = stockCapacities_[ pStockTransporterNature ];

                MT_Float rWeight = 0.;
                MT_Float rVolume = 0.;
                composante.GetType().GetStockTransporterCapacity( rWeight, rVolume );

                stockData.rVolume_ += rVolume;
                stockData.rWeight_ += rWeight;
            }
        }

        T_NatureStockData stockCapacities_;
    };
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStockContainer::CheckStockCapacities
// Created: NLD 2006-03-24
// -----------------------------------------------------------------------------
void PHY_DotationStockContainer::CheckStockCapacities()
{
    T_NatureStockData stocksByNatures;

    for( CIT_StockMap it = stocks_.begin(); it != stocks_.end(); ++it )
    {
        const PHY_DotationStock&    dotationStock    = *it->second;
        const PHY_DotationCategory& dotationCategory = dotationStock.GetCategory();

        T_StockData& stockData = stocksByNatures[ &dotationCategory.GetNature() ];
        stockData.rVolume_ += dotationStock.GetValue() * dotationCategory.GetVolume();
        stockData.rWeight_ += dotationStock.GetValue() * dotationCategory.GetWeight();
    }

    sStockChecker stockChecker;

    assert( pRoleSupply_ );
    pRoleSupply_->GetPion().GetRole< PHY_RolePion_Composantes >().Apply( stockChecker );

    for( CIT_NatureStockData it = stocksByNatures.begin(); it != stocksByNatures.end(); ++it )
    {
        const T_StockData& stock = it->second;

        const T_StockData& stockCapacity = stockChecker.stockCapacities_[ it->first ];

        if( stock.rVolume_ > stockCapacity.rVolume_ || stock.rWeight_ > stockCapacity.rWeight_ )
        {
            MIL_RC::pRcDepassementCapaciteStockage_->Send( pRoleSupply_->GetPion(), MIL_RC::eRcTypeEvent );
            return;
        }
    }
}

// =============================================================================
// LOG
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_DotationStockContainer::NotifySupplyNeeded
// Created: NLD 2005-01-21
// -----------------------------------------------------------------------------
void PHY_DotationStockContainer::NotifySupplyNeeded( const PHY_DotationCategory& dotationCategory, bool bNewNeed ) const
{   
    assert( pRoleSupply_ );
    pRoleSupply_->NotifySupplyNeeded( dotationCategory, bNewNeed );
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
