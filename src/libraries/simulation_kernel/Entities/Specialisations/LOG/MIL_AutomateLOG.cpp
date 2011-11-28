// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Specialisations/Log/MIL_AutomateLOG.cpp $
// $Author: Nld $
// $Modtime: 11/05/05 14:54 $
// $Revision: 18 $
// $Workfile: MIL_AutomateLOG.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_AutomateLOG.h"
#include "MIL_AutomateTypeLOG.h"
#include "ConsumeDotationNotificationHandler_ABC.h"
#include "LogisticHierarchy.h"
#include "LogisticLink_ABC.h"
#include "MIL_LogisticVisitors.h"
#include "Checkpoints/SerializationTools.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/MIL_Formation.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Maintenance.h"
#include "Entities/Agents/Roles/Logistic/PHY_MaintenanceComposanteState.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Medical.h"
#include "Entities/Agents/Roles/Logistic/PHY_MedicalCollectionAmbulance.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Supply.h"
#include "Entities/Agents/Roles/Logistic/SupplyConsign_ABC.h"
#include "Entities/Agents/Roles/Logistic/SupplyStockPushFlowRequestBuilder.h"
#include "Entities/Agents/Roles/Logistic/SupplyRequestManualDispatcher.h"
#include "Entities/Agents/Roles/Logistic/SupplyRequestContainer.h"
#include "Entities/Agents/Roles/Logistic/FuneralConsign_ABC.h"
#include "Entities/Agents/Roles/Logistic/FuneralPackagingResource.h"
#include "Entities/Agents/Units/Logistic/PHY_LogisticLevel.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Actions/PHY_ActionLogistic.h"
#include "Entities/Orders/MIL_Report.h"
#include "MT_Tools/MT_Logger.h"
#include "Network/NET_AsnException.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include <boost/foreach.hpp>
#include <xeumeuleu/xml.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_AutomateLOG )

template< typename Archive >
void save_construct_data( Archive& archive, const MIL_AutomateLOG* automat, const unsigned int /*version*/ )
{
    archive << automat->pLogLevel_->GetName();
}

template< typename Archive >
void load_construct_data( Archive& archive, MIL_AutomateLOG* automat, const unsigned int /*version*/ )
{
    std::string logLevelName;
    archive >> logLevelName;
    assert(PHY_LogisticLevel::Find(logLevelName));
    ::new(automat)MIL_AutomateLOG(*PHY_LogisticLevel::Find(logLevelName));
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG constructor
// Created: NLD 2004-12-21
// -----------------------------------------------------------------------------
MIL_AutomateLOG::MIL_AutomateLOG( MIL_Formation& formation, const PHY_LogisticLevel& logLevel )
    : pAssociatedAutomate_        ( 0 )
    , pAssociatedFormation_       ( &formation )
    , pLogisticHierarchy_         ( new logistic::LogisticHierarchy( *this, true /*use quotas*/ ) )
    , pLogLevel_                  ( &logLevel )
    , supplyConsigns_             ()
 //   , pExplicitStockSupplyState_  ( 0 )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG constructor
// Created: NLD 2007-03-29
// -----------------------------------------------------------------------------
MIL_AutomateLOG::MIL_AutomateLOG( MIL_Automate& automate, const PHY_LogisticLevel& logLevel )
    : pAssociatedAutomate_        ( &automate )
    , pAssociatedFormation_       ( 0 )
    , pLogLevel_                  ( &logLevel )
    , pLogisticHierarchy_         ( new logistic::LogisticHierarchy( *this, true /*use quotas*/ ) )
    , supplyConsigns_             ()
//    , pExplicitStockSupplyState_  ( 0 )
{
}
// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG constructor
// Created: AHC 2010-09-28
// -----------------------------------------------------------------------------
MIL_AutomateLOG::MIL_AutomateLOG( const PHY_LogisticLevel& level )
    : pAssociatedAutomate_        ( 0 )
    , pAssociatedFormation_       ( 0 )
    , pLogLevel_                  ( &level )
    , pLogisticHierarchy_         ( 0 )
    , supplyConsigns_             ()
//  , pExplicitStockSupplyState_  ( 0 )
{
}
// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG destructor
// Created: NLD 2004-12-21
// -----------------------------------------------------------------------------
MIL_AutomateLOG::~MIL_AutomateLOG()
{
    // NOTHING
}

// =============================================================================
// CHEKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_BrainLogistic::Visit
// Created: NLD 2004-12-28
// -----------------------------------------------------------------------------
template< typename T > void MIL_AutomateLOG::Visit( T& visitor ) const
{
    assert( pAssociatedFormation_ || pAssociatedAutomate_ );
    if( pAssociatedFormation_ )
        pAssociatedFormation_->Apply( visitor );
    else if( pAssociatedAutomate_ )
        pAssociatedAutomate_->Apply( visitor );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::serialize
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
template < typename Archive >
void MIL_AutomateLOG::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< logistic::LogisticHierarchyOwner_ABC >( *this )
         & pAssociatedAutomate_
         & pAssociatedFormation_
         & pLogisticHierarchy_
    ;/*     & pExplicitStockSupplyState_;
         & supplyConsigns_;*/
}

// =============================================================================
// INIT
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::ReadLogisticLink
// Created: NLD 2006-10-19
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::ReadLogisticLink( MIL_AutomateLOG& superior, xml::xistream& xis )
{
    pLogisticHierarchy_.reset( new logistic::LogisticHierarchy( *this, superior, true /*use quotas*/, xis ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::WriteLogisticLinksODB
// Created: NLD 2006-05-29
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::WriteLogisticLinksODB( xml::xostream& xos ) const
{
    pLogisticHierarchy_->WriteODB( xos );
}

// =============================================================================
// MAINTENANCE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::MaintenanceHandleComposanteForTransport
// Created: NLD 2004-12-28
// -----------------------------------------------------------------------------
PHY_MaintenanceComposanteState* MIL_AutomateLOG::MaintenanceHandleComposanteForTransport( MIL_Agent_ABC& pion, PHY_ComposantePion& composante )
{
    MaintenanceTransportVisitor visitor( composante );
    Visit( visitor );
    return visitor.pSelected_ ? visitor.pSelected_->HandleComposanteForTransport( pion, composante ) : 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::MaintenanceHandleComposanteForTransport
// Created: NLD 2004-12-28
// -----------------------------------------------------------------------------
bool MIL_AutomateLOG::MaintenanceHandleComposanteForTransport( PHY_MaintenanceComposanteState& composanteState )
{
    MaintenanceTransportVisitor visitor( composanteState.GetComposante() );
    Visit( visitor );
    return visitor.pSelected_ ? visitor.pSelected_->HandleComposanteForTransport( composanteState ) : false;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::MaintenanceHandleComposanteForRepair
// Created: NLD 2004-12-28
// -----------------------------------------------------------------------------
bool MIL_AutomateLOG::MaintenanceHandleComposanteForRepair( PHY_MaintenanceComposanteState& composanteState )
{
    MaintenanceRepairVisitor visitor( composanteState );
    Visit( visitor );
    return visitor.pSelected_ ? visitor.pSelected_->HandleComposanteForRepair( composanteState ) : false;
}

// =============================================================================
// MEDICAL
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::MedicalHandleHumanEvacuatedByThirdParty
// Created: NLD 2005-08-01
// -----------------------------------------------------------------------------
PHY_MedicalHumanState* MIL_AutomateLOG::MedicalHandleHumanEvacuatedByThirdParty( MIL_AgentPion& pion, Human_ABC& human )
{
    MedicalThirdPartyEvacuationVisitor visitor( pion, human );
    Visit( visitor );
    return visitor.pState_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::MedicalHandleHumanForEvacuation
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
PHY_MedicalHumanState* MIL_AutomateLOG::MedicalHandleHumanForEvacuation( MIL_AgentPion& pion, Human_ABC& human )
{
    MedicalEvacuationVisitor visitor( human );
    Visit( visitor );
    return visitor.pSelected_ ? visitor.pSelected_->HandleHumanForEvacuation( pion, human ) : 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::MedicalHandleHumanForCollection
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
bool MIL_AutomateLOG::MedicalHandleHumanForCollection( PHY_MedicalHumanState& humanState )
{
    MedicalCollectionVisitor visitor( humanState );
    Visit( visitor );
    return visitor.pSelected_ ? visitor.pSelected_->HandleHumanForCollection( humanState ) : false;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::MedicalReserveForSorting
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
PHY_RoleInterface_Medical* MIL_AutomateLOG::MedicalReserveForSorting( PHY_MedicalCollectionAmbulance& ambulance )
{
    MedicalSortingVisitor visitor( ambulance );
    Visit( visitor );

    if( !visitor.pSelected_ )
        return 0;

    visitor.pSelected_->ReserveForSorting( ambulance );
    return visitor.pSelected_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::MedicalHandleHumanForHealing
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
bool MIL_AutomateLOG::MedicalHandleHumanForHealing( PHY_MedicalHumanState& humanState )
{
    MedicalHealingVisitor visitor( humanState );
    Visit( visitor );
    return visitor.pSelected_ ? visitor.pSelected_->HandleHumanForHealing( humanState ) : false;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::MedicalCanCollectionAmbulanceGo
// Created: NLD 2005-01-13
// -----------------------------------------------------------------------------
bool MIL_AutomateLOG::MedicalCanCollectionAmbulanceGo( const PHY_MedicalCollectionAmbulance& ambulance ) const
{
    if( ambulance.IsAnEmergency() )
        return true;

    MedicalCollectionAmbulanceAuthorizedToGoVisitor visitor( ambulance );
    Visit( visitor );
    return visitor.bAuthorized_;
}

// =============================================================================
// QUOTAS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::NotifyQuotaThresholdReached
// Created: NLD 2011-01-11
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::NotifyQuotaThresholdReached( const PHY_DotationCategory& dotationCategory ) const
{
    if( pAssociatedAutomate_ )
        MIL_Report::PostEvent( *pAssociatedAutomate_, MIL_Report::eReport_QuotaAlmostConsumed, dotationCategory );
}

// =============================================================================
// SUPPLY
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::SupplyHandleRequest
// Created: NLD 2005-02-02
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::SupplyHandleRequest( boost::shared_ptr < logistic::SupplyConsign_ABC > consign )
{
    supplyConsigns_.push_back( consign );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::SupplyGetStock
// Created: NLD 2005-02-01
// -----------------------------------------------------------------------------
bool MIL_AutomateLOG::SupplyHasStock( const PHY_DotationCategory& dotationCategory ) const
{
    SupplyStockContainerVisitor visitor( dotationCategory );
    Visit( visitor );
    return visitor.pSelected_ != 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::SupplyGetStock
// Created: NLD 2005-02-01
// -----------------------------------------------------------------------------
double MIL_AutomateLOG::SupplyGetStock( const PHY_DotationCategory& dotationCategory, double quantity ) const
{
    SupplyStockReservationVisitor visitor( dotationCategory, quantity );
    Visit( visitor );
    return quantity - visitor.remainingQuantity_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::SupplyReturnStock
// Created: NLD 2005-03-17
// -----------------------------------------------------------------------------
bool MIL_AutomateLOG::SupplyReturnStock( const PHY_DotationCategory& dotationCategory, double quantity ) const
{
    SupplyStockReturnVisitor visitor( dotationCategory, quantity );
    Visit( visitor );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::SupplyCreateConvoyPion
// Created: NLD 2005-03-17
// -----------------------------------------------------------------------------
MIL_AgentPion* MIL_AutomateLOG::SupplyCreateConvoyPion( const MIL_AgentTypePion& type, boost::shared_ptr< logistic::SupplyConvoyReal_ABC > convoy )
{
    // Search for the 'chief' automat
    MIL_Automate* pConvoyAutomate = pAssociatedAutomate_;
    if( pAssociatedFormation_ )
    {
        SupplyConvoyCapacityVisitor visitor;
        Visit( visitor );
        if( visitor.pSelected_ )
            pConvoyAutomate = &visitor.pSelected_->GetAutomate();
    }
    if( !pConvoyAutomate )
        return 0;
    const MT_Vector2D& location = pConvoyAutomate->GetPionPC().GetRole<PHY_RoleInterface_Location>().GetPosition();
    MIL_AgentPion* convoyPion = &pConvoyAutomate->CreatePion( type, location );
    PHY_RoleInterface_Supply* itf = convoyPion->RetrieveRole< PHY_RoleInterface_Supply >();
    if( itf )
        itf->AssignConvoy( convoy );
    else
        MT_LOG_ERROR_MSG( "No role interface supply in convoy " << convoyPion->GetName() );
    return convoyPion;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::SupplyCreateConvoyPion
// Created: NLD 2005-03-17
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::SupplyDestroyConvoyPion( MIL_AgentPion& convoyPion )
{
    PHY_RoleInterface_Supply* itf = convoyPion.RetrieveRole< PHY_RoleInterface_Supply >();
    if( itf )
        itf->UnassignConvoy();
    else
        MT_LOG_ERROR_MSG( "No role interface supply in convoy " << convoyPion.GetName() );
    convoyPion.GetAutomate().DestroyPion( convoyPion );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::SupplyGetAvailableConvoyTransporter
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
bool MIL_AutomateLOG::SupplyGetAvailableConvoyTransporter( PHY_ComposantePion*& pConvoyTransporter, MIL_AgentPion*& pConvoyTransporterPion, const PHY_DotationCategory& dotationCategory  ) const
{
    SupplyConvoyAvailabilityVisitor visitor( dotationCategory );
    Visit( visitor );

    pConvoyTransporter     = visitor.pConvoySelected_;
    pConvoyTransporterPion = visitor.pSelected_;
    return pConvoyTransporter != 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::SupplyGetAvailableConvoyTransporter
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
bool MIL_AutomateLOG::SupplyGetAvailableConvoyTransporter( PHY_ComposantePion*& pConvoyTransporter, MIL_AgentPion*& pConvoyTransporterPion, const PHY_ComposanteTypePion& transporterType ) const
{
    SupplyConvoyTransporterVisitor visitor( transporterType );
    Visit( visitor );

    pConvoyTransporter     = visitor.pConvoySelected_;
    pConvoyTransporterPion = visitor.pSelected_;
    return pConvoyTransporter != 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::OnReceiveLogSupplyPushFlow
// Created: NLD 2011-07-20
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::OnReceiveLogSupplyPushFlow( const sword::PushFlowParameters& parameters, const tools::Resolver_ABC< MIL_Automate >& automateResolver )
{
    boost::shared_ptr< logistic::SupplyRequestBuilder_ABC > builder( new logistic::SupplyStockPushFlowRequestBuilder( parameters, *this, automateResolver ) );
    boost::shared_ptr< logistic::SupplyRequestContainer > requestContainer( new logistic::SupplyRequestContainer( builder ) );
    logistic::SupplyRequestManualDispatcher dispatcher( *this );
    requestContainer->Execute( dispatcher );
    supplyRequests_.push_back( requestContainer );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::OnSupplyConvoyArriving
// Created: NLD 2011-07-20
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::OnSupplyConvoyArriving( boost::shared_ptr< const logistic::SupplyConsign_ABC > supplyConsign )
{
    T_SupplyConvoysObservers tmp = supplyConvoysObserver_;
    std::for_each( tmp.begin(), tmp.end(), boost::bind( &logistic::FuneralConsign_ABC::OnSupplyConvoyArriving, _1, supplyConsign ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::OnSupplyConvoyLeaving
// Created: NLD 2011-07-20
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::OnSupplyConvoyLeaving( boost::shared_ptr< const logistic::SupplyConsign_ABC > supplyConsign )
{
    T_SupplyConvoysObservers tmp = supplyConvoysObserver_;
    std::for_each( tmp.begin(), tmp.end(), boost::bind( &logistic::FuneralConsign_ABC::OnSupplyConvoyLeaving, _1, supplyConsign ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::BelongsToLogisticBase
// Created: NLD 2011-07-20
// -----------------------------------------------------------------------------
bool MIL_AutomateLOG::BelongsToLogisticBase( const MIL_AutomateLOG& logisticBase ) const
{
    return &logisticBase == this;
}

// =============================================================================
// Funeral
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::BelongsToLogisticBase
// Created: NLD 2011-07-20
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::AddSupplyConvoysObserver( logistic::SupplyConvoysObserver_ABC& observer )
{
    supplyConvoysObserver_.insert( &observer );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::BelongsToLogisticBase
// Created: NLD 2011-07-20
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::RemoveSupplyConvoysObserver( logistic::SupplyConvoysObserver_ABC& observer )
{
    supplyConvoysObserver_.erase( &observer );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::BelongsToLogisticBase
// Created: NLD 2011-07-20
// -----------------------------------------------------------------------------
bool MIL_AutomateLOG::FuneralHandleConsign( boost::shared_ptr< logistic::FuneralConsign_ABC > consign )
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::BelongsToLogisticBase
// Created: NLD 2011-07-20
// -----------------------------------------------------------------------------
const logistic::FuneralPackagingResource* MIL_AutomateLOG::FuneralGetNextPackagingResource( const logistic::FuneralPackagingResource* currentPackaging )
{
    FuneralPackagingResourceVisitor visitor( currentPackaging );
    Visit( visitor );
    if( visitor.nextPackagingResource_ )
    {
        assert( visitor.pSelected_ );
        double quantity = 1;
        visitor.pSelected_->Apply( &dotation::ConsumeDotationNotificationHandler_ABC::NotifyConsumeDotation, visitor.nextPackagingResource_->GetDotationCategory(), quantity );
        return visitor.nextPackagingResource_;
    }
    else 
        return 0;
}

// =============================================================================
// Update
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::UpdateLogistic
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::UpdateLogistic()
{
    for( T_SupplyConsigns::iterator it = supplyConsigns_.begin(); it != supplyConsigns_.end(); )
    {
        if( (*it)->Update() )
            it = supplyConsigns_.erase( it );
        else
            ++it;
    }

    for( T_SupplyRequests::iterator it = supplyRequests_.begin(); it != supplyRequests_.end(); )
    {
        if( (*it)->Update() )
            it = supplyRequests_.erase( it );
        else
            ++it;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::UpdateState
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::UpdateState()
{
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::Clean
// Created: NLD 2005-01-31
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::Clean()
{
    BOOST_FOREACH( T_SupplyRequests::value_type& data, supplyRequests_ )
        data->Clean();
}


// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::SendFullState
// Created: NLD 2005-02-01
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::SendFullState() const
{
    pLogisticHierarchy_->SendFullState();
    BOOST_FOREACH( const T_SupplyRequests::value_type& data, supplyRequests_ )
        data->SendFullState();
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::SendChangedState
// Created: NLD 2011-01-11
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::SendChangedState() const
{
    pLogisticHierarchy_->SendChangedState();
    BOOST_FOREACH( const T_SupplyRequests::value_type& data, supplyRequests_ )
        data->SendChangedState();
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::GetID
// Created: AHC 2010-09-24
// -----------------------------------------------------------------------------
unsigned int MIL_AutomateLOG::GetID() const
{
    assert( pAssociatedFormation_ || pAssociatedAutomate_ );
    if( pAssociatedFormation_ )
        return pAssociatedFormation_->GetID();
    else // if( pAssociatedAutomate_ )
        return pAssociatedAutomate_->GetID();
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::GetArmy
// Created: AHC 2010-09-27
// -----------------------------------------------------------------------------
MIL_Army_ABC& MIL_AutomateLOG::GetArmy() const
{
    assert( pAssociatedFormation_ || pAssociatedAutomate_ );
    return pAssociatedFormation_ ?
            pAssociatedFormation_->GetArmy() :
            pAssociatedAutomate_->GetArmy();
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::GetPC
// Created: AHC 2010-09-27
// -----------------------------------------------------------------------------
const MIL_AgentPion* MIL_AutomateLOG::GetPC() const
{
    PCVisitor visitor;
    Visit( visitor );
    return visitor.pSelected_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::GetLogisticHierarchy
// Created: NLD 2011-01-12
// -----------------------------------------------------------------------------
logistic::LogisticHierarchy_ABC& MIL_AutomateLOG::GetLogisticHierarchy() const
{
    assert( pLogisticHierarchy_.get() );
    return *pLogisticHierarchy_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::GetLogisticLevel
// Created: AHC 2010-10-13
// -----------------------------------------------------------------------------
const PHY_LogisticLevel& MIL_AutomateLOG::GetLogisticLevel() const
{
    return *pLogLevel_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::GetLogisticLevel
// Created: AHC 2010-10-13
// -----------------------------------------------------------------------------
const MT_Vector2D& MIL_AutomateLOG::GetPosition() const
{
    static MT_Vector2D none;
    return GetPC() ? GetPC()->GetRole< PHY_RoleInterface_Location >().GetPosition() : none;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::Serialize
// Created: AHC 2010-10-13
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::Serialize( sword::ParentEntity& message ) const
{
    if( pAssociatedAutomate_ )
        message.mutable_automat()->set_id( pAssociatedAutomate_->GetID() );
    else if( pAssociatedFormation_ )
        message.mutable_formation()->set_id( pAssociatedFormation_->GetID() );
    else
        message.mutable_automat()->set_id( 0 );
}
