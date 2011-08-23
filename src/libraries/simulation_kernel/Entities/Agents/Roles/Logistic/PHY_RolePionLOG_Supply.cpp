// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/PHY_RolePionLOG_Supply.cpp $
// $Author: Nld $
// $Modtime: 13/04/05 14:16 $
// $Revision: 9 $
// $Workfile: PHY_RolePionLOG_Supply.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RolePionLOG_Supply.h"
#include "PHY_SupplyResourcesAlarms.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationStockContainer.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Units/Composantes/PHY_Composante_ABC.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Orders/MIL_Report.h"
#include "Entities/Specialisations/LOG/MIL_AgentPionLOG_ABC.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "simulation_kernel/AlgorithmsFactories.h"
#include "simulation_kernel/OnComponentFunctor_ABC.h"
#include "simulation_kernel/OnComponentFunctorComputer_ABC.h"
#include "simulation_kernel/OnComponentFunctorComputerFactory.h"
#include "simulation_kernel/OnComponentLendedFunctorComputer_ABC.h"
#include "simulation_kernel/OnComponentLendedFunctorComputerFactory.h"
#include "simulation_kernel/NetworkNotificationHandler_ABC.h"
#include <xeumeuleu/xml.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_RolePionLOG_Supply )

template< typename Archive >
void save_construct_data( Archive& archive, const PHY_RolePionLOG_Supply* role, const unsigned int /*version*/ )
{
    MIL_AgentPion* const pion = &role->pion_;
    archive << pion;
}

template< typename Archive >
void load_construct_data( Archive& archive, PHY_RolePionLOG_Supply* role, const unsigned int /*version*/ )
{
    MIL_AgentPionLOG_ABC* pion;
    archive >> pion;
    ::new( role )PHY_RolePionLOG_Supply( *pion, true );
    pion->RegisterRole( *role );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply constructor
// Created: NLD 2005-01-21
// -----------------------------------------------------------------------------
PHY_RolePionLOG_Supply::PHY_RolePionLOG_Supply( MIL_AgentPionLOG_ABC& pion, bool fromArchive /*= false*/ )
    : pion_             ( pion )
    , bSystemEnabled_    ( false )
    , bHasChanged_       ( true )
    , bExternalMustChangeState_ ( false )
    , pStocks_           ( 0 )
{
    pStocks_ = new PHY_DotationStockContainer( *this, fromArchive ? false : MIL_AgentServer::GetWorkspace().GetEntityManager().HasInfiniteDotations() );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply destructor
// Created: NLD 2005-01-21
// -----------------------------------------------------------------------------
PHY_RolePionLOG_Supply::~PHY_RolePionLOG_Supply()
{
    delete pStocks_;
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::serialize
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RolePionLOG_Supply::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< PHY_RoleInterface_Supply >( *this )
         & bSystemEnabled_
         & pStocks_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::WriteODB
// Created: NLD 2006-05-29
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Supply::WriteODB( xml::xostream& xos ) const
{
    assert( pStocks_ );
    pStocks_->WriteODB( xos );
}

// =============================================================================
// INIT
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::ReadOverloading
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Supply::ReadOverloading( xml::xistream& xis )
{
    assert( pStocks_ );
    pStocks_->ReadValues( xis );
}

// =============================================================================
//
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::FindLogisticManager
// Created: NLD 2011-01-11
// -----------------------------------------------------------------------------
MIL_AutomateLOG* PHY_RolePionLOG_Supply::FindLogisticManager() const
{
    return pion_.FindLogisticManager();
}

// =============================================================================
// Available Convoy //@TODO MGD MERGE all Logictic OnComponentFunctor in one file
// =============================================================================
class AvailableConvoyFunctor : public OnComponentFunctor_ABC
{

public:
    AvailableConvoyFunctor( const PHY_DotationCategory& dotationCategory )
        : dotationCategory_( dotationCategory )
        , pSelectedConvoy_( 0 )
    {
    }

    void operator() ( PHY_ComposantePion& composante )
    {
        if( composante.CanBePartOfConvoy() && composante.CanTransportStock( dotationCategory_ ) )
            pSelectedConvoy_ = &composante;
    }

    const PHY_DotationCategory& dotationCategory_;
    PHY_ComposantePion* pSelectedConvoy_;
};
// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::GetAvailableConvoyTransporter
// Created: NLD 2005-02-07
// -----------------------------------------------------------------------------
PHY_ComposantePion* PHY_RolePionLOG_Supply::GetAvailableConvoyTransporter( const PHY_DotationCategory& dotationCategory ) const
{
    if( !bSystemEnabled_ )
        return 0;

    AvailableConvoyFunctor functor( dotationCategory );
    std::auto_ptr< OnComponentComputer_ABC > computer( pion_.GetAlgorithms().onComponentFunctorComputerFactory_->Create( functor ) );
    pion_.Execute( *computer );
    return functor.pSelectedConvoy_;
}

class AvailableConvoyTypeFunctor : public OnComponentFunctor_ABC
{

public:
    AvailableConvoyTypeFunctor( const PHY_ComposanteTypePion& type )
        : type_           ( type )
        , pSelectedConvoy_( 0 )
    {
    }

    void operator() ( PHY_ComposantePion& composante )
    {
        if( &composante.GetType() == &type_ && composante.CanBePartOfConvoy() )
            pSelectedConvoy_ = &composante;
    }

    const PHY_ComposanteTypePion& type_;
    PHY_ComposantePion* pSelectedConvoy_;
};
// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::GetAvailableConvoyTransporter
// Created: NLD 2005-02-07
// -----------------------------------------------------------------------------
PHY_ComposantePion* PHY_RolePionLOG_Supply::GetAvailableConvoyTransporter( const PHY_ComposanteTypePion& type ) const
{
    if( !bSystemEnabled_ )
        return 0;

    AvailableConvoyTypeFunctor functor( type );
    std::auto_ptr< OnComponentComputer_ABC > computer( pion_.GetAlgorithms().onComponentFunctorComputerFactory_->Create( functor ) );
    pion_.Execute( *computer );
    return functor.pSelectedConvoy_;
}
// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::GetStockAvailablity
// Created: NLD 2005-02-01
// -----------------------------------------------------------------------------
double PHY_RolePionLOG_Supply::GetStockAvailablity( const PHY_DotationCategory& dotationCategory, double rRequestedValue ) const
{
    if( !bSystemEnabled_ && !pion_.IsDead() ) // <== Stock à terre quand pion mort = libre service
        return 0.;
    return std::min( GetStockValue( dotationCategory ), rRequestedValue );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::AddStockReservation
// Created: NLD 2005-02-01
// -----------------------------------------------------------------------------
double PHY_RolePionLOG_Supply::AddStockReservation( const PHY_DotationCategory& dotationCategory, double rRequestedValue )
{
    assert( pStocks_ );
    return pStocks_->AddReservation( dotationCategory, rRequestedValue );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::RemoveStockReservation
// Created: NLD 2005-02-11
// -----------------------------------------------------------------------------
double PHY_RolePionLOG_Supply::RemoveStockReservation( const PHY_DotationCategory& dotationCategory, double rRequestedValue )
{
    assert( pStocks_ );
    return pStocks_->RemoveReservation( dotationCategory, rRequestedValue );
}

// =============================================================================
// TOOLS
// =============================================================================

// =============================================================================
// ConvoyTransportersUse //@TODO MGD MERGE all Logictic OnComponentFunctor in one file
// =============================================================================
class ConvoyTransportersUseFunctor : public OnComponentFunctor_ABC
{

public:
    ConvoyTransportersUseFunctor( PHY_Composante_ABC::T_ComposanteUseMap& composanteUse )
        : composanteUse_( composanteUse )
    {
    }

    void operator() ( PHY_ComposantePion& composante )
    {
        if( composante.CouldBePartOfConvoy() )
        {
            PHY_Composante_ABC::T_ComposanteUse& data = composanteUse_[ &composante.GetType() ];
            ++ data.nNbrTotal_;

            if( composante.GetState().IsUsable() )
            {
                ++ data.nNbrAvailable_;
                if( !composante.CanBePartOfConvoy() )
                    ++ data.nNbrUsed_;
            }
        }
    }

    PHY_Composante_ABC::T_ComposanteUseMap& composanteUse_;
};

class ConvoyLendedTransportersUseFunctor : public OnComponentFunctor_ABC
{

public:
    ConvoyLendedTransportersUseFunctor( PHY_Composante_ABC::T_ComposanteUseMap& composanteUse )
        : composanteUse_( composanteUse )
    {
    }

    void operator() ( PHY_ComposantePion& composante )
    {
        if( composante.CouldBePartOfConvoy() )
        {
            PHY_Composante_ABC::T_ComposanteUse& data = composanteUse_[ &composante.GetType() ];
            ++ data.nNbrTotal_;
            ++ data.nNbrLent_;
        }
    }

    PHY_Composante_ABC::T_ComposanteUseMap& composanteUse_;
};
// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::GetConvoyTransportersAvailabilityRatio
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
double PHY_RolePionLOG_Supply::GetConvoyTransportersAvailabilityRatio() const
{
    unsigned int nNbrTotal                  = 0;
    unsigned int nNbrAvailableAllowedToWork = 0;
    PHY_Composante_ABC::T_ComposanteUseMap composanteUse;

    ConvoyTransportersUseFunctor functor( composanteUse );
    std::auto_ptr< OnComponentComputer_ABC > componentComputer( pion_.GetAlgorithms().onComponentFunctorComputerFactory_->Create( functor ) );
    pion_.Execute( *componentComputer );
    ConvoyLendedTransportersUseFunctor functor2( composanteUse );
    std::auto_ptr< OnComponentLendedFunctorComputer_ABC > lendedComputer( pion_.GetAlgorithms().onComponentLendedFunctorComputerFactory_->Create( functor2 ) );
    pion_.Execute( *lendedComputer );

    for( PHY_Composante_ABC::CIT_ComposanteUseMap it = composanteUse.begin(); it != composanteUse.end(); ++it )
    {
        nNbrTotal                  += it->second.nNbrTotal_;
        nNbrAvailableAllowedToWork += ( it->second.nNbrAvailable_ - it->second.nNbrUsed_ );
    }
    if( nNbrTotal == 0 )
        return 1.;
    return (double)nNbrAvailableAllowedToWork / (double)nNbrTotal;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::StartUsingForLogistic
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Supply::StartUsingForLogistic( PHY_ComposantePion& composante )
{
    double rTransporterRatio = GetConvoyTransportersAvailabilityRatio();

    bHasChanged_ = true;
    composante.StartUsingForLogistic();

    if( PHY_SupplyResourcesAlarms::IsConvoyTransporterResourcesLevelReached( rTransporterRatio, GetConvoyTransportersAvailabilityRatio() ) )
        MIL_Report::PostEvent< MIL_Agent_ABC >( pion_, MIL_Report::eReport_ConvoyTransporterResourcesLevelReached );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::StopUsingForLogistic
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Supply::StopUsingForLogistic( PHY_ComposantePion& composante )
{
    bHasChanged_ = true;
    composante.StopUsingForLogistic();
}

// =============================================================================
// STOCK SUPPLY
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::NotifySupplyNeeded
// Created: NLD 2005-01-21
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Supply::NotifySupplyNeeded( const PHY_DotationCategory& dotationCategory, bool bNewNeed ) const
{
    if( bNewNeed )
        MIL_Report::PostEvent< MIL_Agent_ABC >( pion_, MIL_Report::eReport_LogisticStockThresholdExceeded, dotationCategory );

    pion_.GetAutomate().NotifyStockSupplyNeeded( dotationCategory );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::Apply
// Created: NLD 2005-01-31
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Supply::Apply( boost::function< void( PHY_DotationStock& ) > visitor ) const
{
    assert( pStocks_ );
    pStocks_->Apply( visitor );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::ResupplyStocks
// Created: NLD 2005-02-03
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Supply::ResupplyStocks()
{
    assert( pStocks_ );
    pStocks_->Resupply();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::ResupplyStocks
// Created: SBO 2005-12-12
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Supply::ResupplyStocks( const PHY_DotationCategory& category, double rNbr )
{
    assert( pStocks_ );
    pStocks_->Resupply( category, rNbr );
}

// =============================================================================
// STOCK MANAGEMENT
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::GetStockValue
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
double PHY_RolePionLOG_Supply::GetStockValue( const PHY_DotationCategory& category ) const
{
    assert( pStocks_ );
    return pStocks_->GetValue( category );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::GetStock
// Created: NLD 2005-02-04
// -----------------------------------------------------------------------------
PHY_DotationStock* PHY_RolePionLOG_Supply::GetStock( const PHY_DotationCategory& category ) const
{
    assert( pStocks_ );
    return pStocks_->GetStock( category );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::CanContainStock
// Created: NLD 2005-03-17
// -----------------------------------------------------------------------------
bool PHY_RolePionLOG_Supply::CanContainStock( const PHY_DotationCategory& category ) const
{
    assert( pStocks_ );
    return pStocks_->GetStock( category ) != 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::AddStock
// Created: NLD 2006-03-28
// -----------------------------------------------------------------------------
PHY_DotationStock* PHY_RolePionLOG_Supply::AddStock( const PHY_DotationCategory& category ) const
{
    assert( pStocks_ );
    return pStocks_->AddStock( category );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::AddStock
// Created: BCI 2011-02-11
// -----------------------------------------------------------------------------
PHY_DotationStock* PHY_RolePionLOG_Supply::AddEmptyStock( const PHY_DotationCategory& dotationCategory, double capacity ) const
{
    assert( pStocks_ );
    return pStocks_->AddEmptyStock( dotationCategory, capacity );
}

// =============================================================================
// MAIN
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::Update
// Created: NLD 2005-01-21
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Supply::Update( bool /*bIsDead*/ )
{
    if( bHasChanged_ )
        pion_.Apply( &network::NetworkNotificationHandler_ABC::NotifyDataHasChanged );
    assert( pStocks_ );
    pStocks_->Update(); // Stock checking
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::UpdateLogistic
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Supply::UpdateLogistic( bool /*bIsDead*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::Clean
// Created: NLD 2005-01-21
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Supply::Clean()
{
    bHasChanged_ = false;
    bExternalMustChangeState_ = false;
    assert( pStocks_ );
    pStocks_->Clean();
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: SendComposanteUse
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
static
void SendComposanteUse( const PHY_Composante_ABC::T_ComposanteUseMap& data, sword::SeqOfLogSupplyEquimentAvailability& asn )
{
    if( data.empty() )
        return;
    for( PHY_Composante_ABC::CIT_ComposanteUseMap itData = data.begin(); itData != data.end(); ++itData )
    {
        sword::LogSupplyEquimentAvailability& data = *asn.add_elem();
        data.mutable_equipment()->set_id( itData->first->GetMosID().id() );
        assert( itData->second.nNbrTotal_ );

        data.set_total       ( itData->second.nNbrTotal_ );
        data.set_working  ( itData->second.nNbrUsed_ );
        data.set_available ( itData->second.nNbrAvailable_ - itData->second.nNbrUsed_ ); // nNbrAvailableAllowedToWork
        data.set_lent      ( itData->second.nNbrLent_ );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::SendFullState
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Supply::SendFullState( unsigned int context ) const
{
    client::LogSupplyState asn;
    asn().mutable_unit()->set_id( pion_.GetID() );
    asn().set_chain( bSystemEnabled_ );

    PHY_Composante_ABC::T_ComposanteUseMap composanteUse;
    ConvoyTransportersUseFunctor functor( composanteUse );
    std::auto_ptr< OnComponentComputer_ABC > componentComputer( pion_.GetAlgorithms().onComponentFunctorComputerFactory_->Create( functor ) );
    pion_.Execute( *componentComputer );
    ConvoyLendedTransportersUseFunctor functor2( composanteUse );
    std::auto_ptr< OnComponentLendedFunctorComputer_ABC > lendedComputer( pion_.GetAlgorithms().onComponentLendedFunctorComputerFactory_->Create( functor2 ) );
    pion_.Execute( *lendedComputer );

    SendComposanteUse( composanteUse, *asn().mutable_transporters()  );

    assert( pStocks_ );
    pStocks_->SendFullState( asn );

    asn.Send( NET_Publisher_ABC::Publisher(), context );

    if( asn().transporters().elem_size() > 0 )
        asn().mutable_transporters()->Clear();
    if( asn().stocks().elem_size() > 0 )
        asn().mutable_stocks()->Clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::SendChangedState
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Supply::SendChangedState() const
{
    assert( pStocks_ );
    if( !( bHasChanged_ || bExternalMustChangeState_ || pStocks_->HasChanged() ) )
        return;

    client::LogSupplyState asn;
    asn().mutable_unit()->set_id( pion_.GetID() );
    if( bHasChanged_ || bExternalMustChangeState_ )
    {
        asn().set_chain( bSystemEnabled_ );

        PHY_Composante_ABC::T_ComposanteUseMap composanteUse;
        ConvoyTransportersUseFunctor functor( composanteUse );
        std::auto_ptr< OnComponentComputer_ABC > transportedComputer( pion_.GetAlgorithms().onComponentFunctorComputerFactory_->Create( functor ) );
        pion_.Execute( *transportedComputer );
        ConvoyLendedTransportersUseFunctor functor2( composanteUse );
        std::auto_ptr< OnComponentLendedFunctorComputer_ABC > lendedComputer( pion_.GetAlgorithms().onComponentLendedFunctorComputerFactory_->Create( functor2 ) );
        pion_.Execute( *lendedComputer );

        SendComposanteUse( composanteUse, *asn().mutable_transporters() );
    }

    pStocks_->SendChangedState( asn );

    asn.Send( NET_Publisher_ABC::Publisher() );

    if( asn().has_transporters() && asn().transporters().elem_size() > 0 )
        asn().mutable_transporters()->Clear();
    if( asn().has_stocks() && asn().stocks().elem_size() > 0 )
        asn().mutable_stocks()->Clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::EnableSystem
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Supply::EnableSystem()
{
    bSystemEnabled_ = true;
    bHasChanged_    = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::DisableSystem
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Supply::DisableSystem()
{
    bSystemEnabled_ = false;
    bHasChanged_    = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::GetPion
// Created: NLD 2006-01-03
// -----------------------------------------------------------------------------
const MIL_AgentPionLOG_ABC& PHY_RolePionLOG_Supply::GetPion() const
{

    return pion_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::NotifyHasChanged
// Created: MGD 2009-09-29
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Supply::NotifyComponentHasChanged()
{
    bExternalMustChangeState_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::ComputeStockWeightAndVolume
// Created: BCI 2011-02-14
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Supply::ComputeStockWeightAndVolume( const PHY_DotationNature& nature, double& rWeight, double& rVolume ) const
{
    return pStocks_->ComputeStockWeightAndVolume( nature, rWeight, rVolume );
}
