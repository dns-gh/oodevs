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
#include "PHY_DotationStock.h"
#include "PHY_DotationType.h"
#include "Entities/Agents/Units/PHY_UnitType.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Supply.h"
#include "Entities/Specialisations/LOG/MIL_AgentPionLOG_ABC.h"
#include "Entities/Orders/MIL_Report.h"
#include "protocol/ClientSenders.h"
#include "simulation_kernel/OnComponentFunctor_ABC.h"
#include "simulation_kernel/DefaultComponentFunctorComputer.h"
#include "MT_Tools/MT_Logger.h"

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_DotationStockContainer )

// -----------------------------------------------------------------------------
// Name: PHY_DotationStockContainer constructor
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
PHY_DotationStockContainer::PHY_DotationStockContainer( PHY_RoleInterface_Supply& roleSupply, bool bInfiniteDotations )
    : pRoleSupply_          ( &roleSupply )
    , bCheckStockCapacities_( false )
    , bInfiniteDotations_   ( bInfiniteDotations )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStockContainer constructor
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
PHY_DotationStockContainer::PHY_DotationStockContainer()
    : pRoleSupply_          ( 0 )
    , bCheckStockCapacities_( false )
    , bInfiniteDotations_   ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStockContainer destructor
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
PHY_DotationStockContainer::~PHY_DotationStockContainer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStockContainer::serialize
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_DotationStockContainer::serialize( Archive& file, const unsigned int )
{
    file & pRoleSupply_
         & stocks_
         & stocksChanged_
         & bCheckStockCapacities_
         & bInfiniteDotations_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStockContainer::WriteODB
// Created: NLD 2006-05-29
// -----------------------------------------------------------------------------
void PHY_DotationStockContainer::WriteODB( xml::xostream& xos ) const
{
    xos << xml::start( "stocks" );

    for( auto it = stocks_.begin(); it != stocks_.end(); ++it )
    {
        const PHY_DotationStock& dotationStock = *it->second;

        xos << xml::start( "resource" )
            << xml::attribute( "name", dotationStock.GetCategory().GetName() )
            << xml::attribute( "quantity", dotationStock.GetValue() )
        << xml::end; // dotation"
    }
    xos << xml::end; // stocks
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStockContainer::ReadValues
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
void PHY_DotationStockContainer::ReadValues( xml::xistream& xis )
{
    try
    {
        xis >> xml::optional
            >> xml::start( "stocks" )
                                 >> xml::list( "resource", *this, &PHY_DotationStockContainer::ReadStock )
                             >> xml::end;
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( tools::GetExceptionMsg( e ) );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStockContainer::ReadStock
// Created: ABL 2007-07-27
// -----------------------------------------------------------------------------
void PHY_DotationStockContainer::ReadStock( xml::xistream& xis )
{
    std::string strType;
    xis >> xml::attribute( "name", strType );

    const PHY_DotationCategory* pDotationCategory = PHY_DotationType::FindDotationCategory( strType );
    if( !pDotationCategory )
        throw MASA_EXCEPTION( xis.context() + "Unknown dotation type '" + strType + "'" );

    AddStock( *pDotationCategory, xis );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStockContainer::AddReservation
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
double PHY_DotationStockContainer::AddReservation( const PHY_DotationCategory& category, double rNbr )
{
    auto it = stocks_.find( &category );
    if( it == stocks_.end() )
        return 0;
    return it->second->AddReservation( rNbr );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStockContainer::RemoveReservation
// Created: NLD 2005-02-11
// -----------------------------------------------------------------------------
double PHY_DotationStockContainer::RemoveReservation( const PHY_DotationCategory& category, double rNbr )
{
    auto it = stocks_.find( &category );
    if( it == stocks_.end() )
        return 0;
    it->second->RemoveReservation( rNbr );
    return rNbr;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStockContainer::GetValue
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
double PHY_DotationStockContainer::GetValue( const PHY_DotationCategory& category ) const
{
    auto it = stocks_.find( &category );
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
    auto it = stocks_.find( &category );
    if( it == stocks_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStockContainer::AddStock
// Created: NLD 2006-01-03
// -----------------------------------------------------------------------------
PHY_DotationStock* PHY_DotationStockContainer::AddStock( const PHY_DotationCategory& category, xml::xistream& xis)
{
    double rValue;
    xis >> xml::attribute( "quantity", rValue );
    if( rValue < 0 )
        throw MASA_EXCEPTION( xis.context() + "stock: quantity < 0" );

    const double rThresholdRatio = pRoleSupply_->GetPion().GetType().GetUnitType().GetStockLogisticThresholdRatio( category.GetLogisticType() );

    PHY_DotationStock*& pStock = stocks_[ &category ];
    if( !pStock )
        pStock = new PHY_DotationStock( *this, category, rThresholdRatio, rValue, bInfiniteDotations_ );
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
        pStock = new PHY_DotationStock( *this, category, 0., 0., bInfiniteDotations_ );
    return pStock;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStockContainer::AddStock
// Created: BCI 2011-02-11
// -----------------------------------------------------------------------------
PHY_DotationStock* PHY_DotationStockContainer::AddEmptyStock( const PHY_DotationCategory& category, double capacity )
{
    PHY_DotationStock*& pStock = stocks_[ &category ];
    if( !pStock )
        pStock = new PHY_DotationStock( *this, category, 0., capacity, bInfiniteDotations_, true/*create empty*/ );
    return pStock;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStockContainer::GetRoleInterfaceSupply
// Created: MMC 2013-04-24
// -----------------------------------------------------------------------------
const PHY_RoleInterface_Supply* PHY_DotationStockContainer::GetRoleInterfaceSupply() const
{
    return pRoleSupply_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStockContainer::Resupply
// Created: NLD 2005-02-03
// -----------------------------------------------------------------------------
void PHY_DotationStockContainer::Resupply( bool withLog )
{
    for( auto it = stocks_.begin(); it != stocks_.end(); ++it )
        it->second->Resupply( withLog );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStockContainer::Resupply
// Created: SBO 2005-12-12
// -----------------------------------------------------------------------------
void PHY_DotationStockContainer::Resupply( const PHY_DotationCategory& category, double rNbr )
{
    PHY_DotationStock*& pStock = stocks_[ &category ];
    if( !pStock )
    {
        const double rThresholdRatio = pRoleSupply_->GetPion().GetType().GetUnitType().GetStockLogisticThresholdRatio( category.GetLogisticType() );
        pStock = new PHY_DotationStock( *this, category, rThresholdRatio, rNbr, bInfiniteDotations_ );
    }
    else
        pStock->Supply( rNbr - pStock->GetValue() ); // set to rNbr
}

namespace
{
    struct T_StockData
    {
        double rVolume_;
        double rWeight_;
    };
    typedef tools::Map< const PHY_DotationNature*, T_StockData > T_NatureStockData;

    class StockChecker : public OnComponentFunctor_ABC
    {
    public:
        void operator() ( PHY_ComposantePion& composante )
        {
            const PHY_DotationNature* pStockTransporterNature = composante.GetType().GetStockTransporterNature();
            if( /*!composante.GetType().CanBePartOfConvoy() && */pStockTransporterNature )
            {
                T_StockData& stockData = stockCapacities_[ pStockTransporterNature ];
                double rWeight = 0.;
                double rVolume = 0.;
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

    for( auto it = stocks_.begin(); it != stocks_.end(); ++it )
    {
        const PHY_DotationStock&    dotationStock    = *it->second;
        const PHY_DotationCategory& dotationCategory = dotationStock.GetCategory();
        T_StockData& stockData = stocksByNatures[ &dotationCategory.GetNature() ];
        stockData.rVolume_ += dotationStock.GetValue() * dotationCategory.GetVolume();
        stockData.rWeight_ += dotationStock.GetValue() * dotationCategory.GetWeight();
    }

    StockChecker stockChecker;
    assert( pRoleSupply_ );//@TODO MGD stock pion and not role
    MIL_AgentPion& pion = const_cast< MIL_AgentPion& >( pRoleSupply_->GetPion() );
    DefaultComponentFunctorComputer componentComputer( stockChecker );
    pion.Execute< OnComponentComputer_ABC >( componentComputer );

    for( auto it = stocksByNatures.begin(); it != stocksByNatures.end(); ++it )
    {
        const T_StockData& stock = it->second;
        const T_StockData& stockCapacity = stockChecker.stockCapacities_[ it->first ];
        if( stock.rVolume_ > stockCapacity.rVolume_ || stock.rWeight_ > stockCapacity.rWeight_ )
        {
            MIL_Report::PostEvent< MIL_Agent_ABC >( pRoleSupply_->GetPion(), report::eRC_DepassementCapaciteStockage );
            return;
        }
    }
}

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
// Name: PHY_DotationStockContainer::Apply
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
void PHY_DotationStockContainer::Apply( boost::function< void( PHY_DotationStock& ) > visitor ) const
{
    for( auto it = stocks_.begin(); it != stocks_.end(); ++it )
        visitor( *it->second );
}

//------------------------------------------------------------------------------
// Name: PHY_DotationStockContainer::SendChangedState
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
void PHY_DotationStockContainer::SendChangedState( client::LogSupplyState& asn ) const
{
    for( auto it = stocksChanged_.begin(); it != stocksChanged_.end(); ++it )
    {
        const PHY_DotationStock& dotation = **it;
        sword::DotationStock& asnRessource = *asn().mutable_stocks()->add_elem();
        asnRessource.mutable_resource()->set_id( dotation.GetCategory().GetMosID() );
        asnRessource.set_quantity( (unsigned int)dotation.GetValue() );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStockContainer::SendFullState
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
void PHY_DotationStockContainer::SendFullState( client::LogSupplyState& asn ) const
{
    for( auto it = stocks_.begin(); it != stocks_.end(); ++it )
    {
        const PHY_DotationStock& dotationStock = *it->second;
        sword::DotationStock& asnRessource = *asn().mutable_stocks()->add_elem();
        asnRessource.mutable_resource()->set_id( dotationStock.GetCategory().GetMosID() );
        asnRessource.set_quantity( (unsigned int)dotationStock.GetValue() );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStockContainer::NotifyDotationChanged
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
void PHY_DotationStockContainer::NotifyDotationChanged( const PHY_DotationStock& dotationStock, double rDelta )
{
    stocksChanged_.insert( &dotationStock );
    if( rDelta > 0 )
        bCheckStockCapacities_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStockContainer::HasChanged
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
bool PHY_DotationStockContainer::HasChanged() const
{
    return !stocksChanged_.empty();
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStockContainer::Clean
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
void PHY_DotationStockContainer::Clean()
{
    stocksChanged_.clear();
    bCheckStockCapacities_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStockContainer::Update
// Created: NLD 2006-03-28
// -----------------------------------------------------------------------------
void PHY_DotationStockContainer::Update()
{
    if( bCheckStockCapacities_ )
        CheckStockCapacities();
    UpdateSupplyNeeded();
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStockContainer::UpdateSupplyNeeded
// Created: MMC 2013-04-24
// -----------------------------------------------------------------------------
void PHY_DotationStockContainer::UpdateSupplyNeeded()
{
    for( auto it = stocks_.begin(); it != stocks_.end(); ++it )
        it->second->UpdateSupplyNeeded();
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationStockContainer::ComputeStockWeightAndVolume
// Created: BCI 2011-02-14
// -----------------------------------------------------------------------------
void PHY_DotationStockContainer::ComputeStockWeightAndVolume( const PHY_DotationNature& nature, double& rWeight, double& rVolume ) const
{
    rWeight = 0;
    rVolume = 0;

    for( auto it = stocks_.begin(); it != stocks_.end(); ++it )
    {
        const PHY_DotationStock&    dotationStock    = *it->second;
        const PHY_DotationCategory& dotationCategory = dotationStock.GetCategory();

        if( &nature == &dotationCategory.GetNature() )
        {
            rVolume += dotationStock.GetValue() * dotationCategory.GetVolume();
            rWeight += dotationStock.GetValue() * dotationCategory.GetWeight();
        }
    }
}
