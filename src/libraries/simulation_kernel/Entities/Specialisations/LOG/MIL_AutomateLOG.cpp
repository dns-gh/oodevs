// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_AutomateLOG.h"
#include "MIL_AutomateTypeLOG.h"
#include "ConsumeDotationNotificationHandler_ABC.h"
#include "LogisticHierarchy.h"
#include "LogisticLink_ABC.h"
#include "MIL_LogisticVisitors.h"
#include "CheckPoints/SerializationTools.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/MIL_Formation.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Maintenance.h"
#include "Entities/Agents/Roles/Logistic/PHY_MaintenanceComposanteState.h"
#include "Entities/Agents/Roles/Logistic/PHY_MedicalHumanState.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Medical.h"
#include "Entities/Agents/Roles/Logistic/PHY_MedicalCollectionAmbulance.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Supply.h"
#include "Entities/Agents/Roles/Logistic/SupplyConsign_ABC.h"
#include "Entities/Agents/Roles/Logistic/SupplyDotationManualRequestBuilder.h"
#include "Entities/Agents/Roles/Logistic/SupplyStockPushFlowRequestBuilder.h"
#include "Entities/Agents/Roles/Logistic/SupplyRequestManualDispatcher.h"
#include "Entities/Agents/Roles/Logistic/SupplyRequestContainer.h"
#include "Entities/Agents/Roles/Logistic/SupplyConvoysObserver_ABC.h"
#include "Entities/Agents/Roles/Logistic/FuneralPackagingResource.h"
#include "Entities/Agents/Units/Logistic/PHY_LogisticLevel.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Actions/PHY_ActionLogistic.h"
#include "Entities/Orders/MIL_Report.h"
#include "MIL_AgentServer.h"
#include "MT_Tools/MT_Logger.h"
#include "Tools/NET_AsnException.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "protocol/MessageParameters.h"
#include <boost/foreach.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/algorithm_ext/erase.hpp>

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
    assert( PHY_LogisticLevel::Find( logLevelName ) );
    ::new( automat )MIL_AutomateLOG( *PHY_LogisticLevel::Find( logLevelName ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG constructor
// Created: NLD 2004-12-21
// -----------------------------------------------------------------------------
MIL_AutomateLOG::MIL_AutomateLOG( MIL_Formation& formation, const PHY_LogisticLevel& logLevel )
    : pAssociatedAutomate_      ( 0 )
    , pAssociatedFormation_     ( &formation )
    , pLogisticHierarchy_       ( new logistic::LogisticHierarchy( *this, true /*use quotas*/ ) )
    , pLogLevel_                ( &logLevel )
    , maintenanceManual_        ( false )
    , maintenanceManualModified_( false )
    , supplyManual_             ( false )
    , supplyManualModified_     ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG constructor
// Created: NLD 2007-03-29
// -----------------------------------------------------------------------------
MIL_AutomateLOG::MIL_AutomateLOG( MIL_Automate& automate, const PHY_LogisticLevel& logLevel )
    : pAssociatedAutomate_      ( &automate )
    , pAssociatedFormation_     ( 0 )
    , pLogLevel_                ( &logLevel )
    , pLogisticHierarchy_       ( new logistic::LogisticHierarchy( *this, true /*use quotas*/ ) )
    , maintenanceManual_        ( false )
    , maintenanceManualModified_( false )
    , supplyManual_             ( false )
    , supplyManualModified_     ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG constructor
// Created: AHC 2010-09-28
// -----------------------------------------------------------------------------
MIL_AutomateLOG::MIL_AutomateLOG( const PHY_LogisticLevel& level )
    : pAssociatedAutomate_      ( 0 )
    , pAssociatedFormation_     ( 0 )
    , pLogLevel_                ( &level )
    , pLogisticHierarchy_       ( 0 )
    , maintenanceManual_        ( false )
    , maintenanceManualModified_( false )
    , supplyManual_             ( false )
    , supplyManualModified_     ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG destructor
// Created: NLD 2004-12-21
// -----------------------------------------------------------------------------
MIL_AutomateLOG::~MIL_AutomateLOG()
{
    // NOTHING
}

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
void MIL_AutomateLOG::serialize( Archive& ar, const unsigned int )
{
    ar & boost::serialization::base_object< logistic::LogisticHierarchyOwner_ABC >( *this )
       & boost::serialization::base_object< logistic::SupplySupplier_ABC >( *this )
       & pAssociatedAutomate_
       & pAssociatedFormation_
       & pLogisticHierarchy_
       & supplyRequests_
       & supplyConsigns_
       & maintenanceManual_
       & supplyManual_;
}

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

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::MaintenanceHandleComposanteForTransport
// Created: NLD 2004-12-28
// -----------------------------------------------------------------------------
PHY_MaintenanceComposanteState* MIL_AutomateLOG::MaintenanceHandleComposanteForTransport( MIL_Agent_ABC& pion, PHY_ComposantePion& composante )
{
    MaintenanceTransportVisitor visitor( composante );
    Visit( visitor );
    return visitor.selected_ ? visitor.selected_->HandleComposanteForTransport( pion, composante ) : 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::MaintenanceHandleComposanteForTransport
// Created: NLD 2004-12-28
// -----------------------------------------------------------------------------
bool MIL_AutomateLOG::MaintenanceHandleComposanteForTransport( PHY_MaintenanceComposanteState& composanteState )
{
    MaintenanceTransportVisitor visitor( composanteState.GetComposante() );
    Visit( visitor );
    return visitor.selected_ ? visitor.selected_->HandleComposanteForTransport( composanteState ) : false;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::MaintenanceHandleComposanteForDiagnosis
// Created: SLI 2014-02-12
// -----------------------------------------------------------------------------
bool MIL_AutomateLOG::MaintenanceHandleComposanteForDiagnosis( PHY_MaintenanceComposanteState& composanteState )
{
    MaintenanceDiagnosisVisitor visitor;
    Visit( visitor );
    return visitor.selected_ ? visitor.selected_->HandleComposanteForDiagnosis( composanteState ) : false;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::MaintenanceHandleComposanteForRepair
// Created: NLD 2004-12-28
// -----------------------------------------------------------------------------
bool MIL_AutomateLOG::MaintenanceHandleComposanteForRepair( PHY_MaintenanceComposanteState& composanteState )
{
    MaintenanceRepairVisitor visitor( composanteState );
    Visit( visitor );
    return visitor.selected_ ? visitor.selected_->HandleComposanteForRepair( composanteState ) : false;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::MaintenanceFindAlternativeTransportHandler
// Created: NLD 2012-01-03
// -----------------------------------------------------------------------------
PHY_RoleInterface_Maintenance* MIL_AutomateLOG::MaintenanceFindAlternativeTransportHandler( PHY_MaintenanceComposanteState& composanteState, const PHY_ComposanteTypePion* type /*= 0*/ )
{
    MaintenanceTransportVisitor visitor( composanteState.GetComposante(), type );
    Visit( visitor );
    return visitor.selected_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::MaintenanceFindAlternativeDiagnosisHandler
// Created: SLI 2014-02-12
// -----------------------------------------------------------------------------
PHY_RoleInterface_Maintenance* MIL_AutomateLOG::MaintenanceFindAlternativeDiagnosisHandler( const PHY_ComposanteTypePion* type /*= 0*/ )
{
    MaintenanceDiagnosisVisitor visitor( type );
    Visit( visitor );
    return visitor.selected_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::MaintenanceFindAlternativeRepairHandler
// Created: NLD 2012-01-03
// -----------------------------------------------------------------------------
PHY_RoleInterface_Maintenance* MIL_AutomateLOG::MaintenanceFindAlternativeRepairHandler( PHY_MaintenanceComposanteState& composanteState, const PHY_ComposanteTypePion* type /*= 0*/ )
{
    MaintenanceRepairVisitor visitor( composanteState, type );
    Visit( visitor );
    return visitor.selected_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::MedicalHandleHumanEvacuatedByThirdParty
// Created: NLD 2005-08-01
// -----------------------------------------------------------------------------
boost::shared_ptr< PHY_MedicalHumanState > MIL_AutomateLOG::MedicalHandleHumanEvacuatedByThirdParty( MIL_AgentPion& pion, Human_ABC& human )
{
    MedicalThirdPartyEvacuationVisitor visitor( pion, human );
    Visit( visitor );
    return visitor.pState_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::MedicalHandleHumanForEvacuation
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
boost::shared_ptr< PHY_MedicalHumanState > MIL_AutomateLOG::MedicalHandleHumanForEvacuation( MIL_AgentPion& pion, Human_ABC& human )
{
    MedicalEvacuationVisitor visitor( human );
    Visit( visitor );
    return visitor.selected_ ? visitor.selected_->HandleHumanForEvacuation( pion, human ) : boost::shared_ptr< PHY_MedicalHumanState >();
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::MedicalHandleHumanForCollection
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
bool MIL_AutomateLOG::MedicalHandleHumanForCollection( PHY_MedicalHumanState& humanState )
{
    MedicalCollectionVisitor visitor( humanState );
    Visit( visitor );
    return visitor.selected_ ? visitor.selected_->HandleHumanForCollection( humanState ) : false;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::MedicalReserveForSorting
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
PHY_RoleInterface_Medical* MIL_AutomateLOG::MedicalReserveForSorting( PHY_MedicalCollectionAmbulance& ambulance )
{
    MedicalSortingVisitor visitor;
    Visit( visitor );

    if( !visitor.selected_ )
        return 0;

    visitor.selected_->ReserveForSorting( ambulance );
    return visitor.selected_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::MedicalHandleHumanForHealing
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
bool MIL_AutomateLOG::MedicalHandleHumanForHealing( PHY_MedicalHumanState& humanState )
{
    MedicalHealingVisitor visitor( humanState );
    Visit( visitor );
    return visitor.selected_ ? visitor.selected_->HandleHumanForHealing( humanState ) : false;
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

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::MedicalFindAlternativeEvacuationHandler
// Created: NLD 2012-01-03
// -----------------------------------------------------------------------------
PHY_RoleInterface_Medical* MIL_AutomateLOG::MedicalFindAlternativeEvacuationHandler( const PHY_MedicalHumanState& humanState )
{
    MedicalEvacuationVisitor visitor( humanState.GetHuman() );
    Visit( visitor );
    return visitor.selected_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::MedicalFindAlternativeCollectionHandler
// Created: NLD 2012-01-03
// -----------------------------------------------------------------------------
PHY_RoleInterface_Medical* MIL_AutomateLOG::MedicalFindAlternativeCollectionHandler( const PHY_MedicalHumanState& humanState )
{
    MedicalCollectionVisitor visitor( humanState );
    Visit( visitor );
    return visitor.selected_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::MedicalFindAlternativeSortingHandler
// Created: NLD 2012-01-03
// -----------------------------------------------------------------------------
PHY_RoleInterface_Medical* MIL_AutomateLOG::MedicalFindAlternativeSortingHandler( const PHY_MedicalHumanState& )
{
    MedicalSortingVisitor visitor;
    Visit( visitor );
    return visitor.selected_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::MedicalFindAlternativeHealingHandler
// Created: NLD 2012-01-03
// -----------------------------------------------------------------------------
PHY_RoleInterface_Medical* MIL_AutomateLOG::MedicalFindAlternativeHealingHandler( const PHY_MedicalHumanState& humanState )
{
    MedicalHealingVisitor visitor( humanState );
    Visit( visitor );
    return visitor.selected_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::NotifyLinkAdded
// Created: MCO 2013-04-24
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::NotifyLinkAdded( const logistic::LogisticLink_ABC& link ) const
{
    if( pAssociatedAutomate_ && link.GetSuperior().GetPC() )
        BOOST_FOREACH( const auto& pion, pAssociatedAutomate_->GetPions() )
            MIL_Report::PostEvent( *link.GetSuperior().GetPC(), report::eRC_LogSuperiorAdded, *pion );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::NotifyLinkRemoved
// Created: MCO 2013-04-24
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::NotifyLinkRemoved( const logistic::LogisticLink_ABC& link ) const
{
    if( pAssociatedAutomate_ && link.GetSuperior().GetPC() )
        BOOST_FOREACH( const auto& pion, pAssociatedAutomate_->GetPions() )
            MIL_Report::PostEvent( *link.GetSuperior().GetPC(), report::eRC_LogSuperiorRemoved, *pion );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::NotifyQuotaThresholdReached
// Created: NLD 2011-01-11
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::NotifyQuotaThresholdReached( const PHY_DotationCategory& dotationCategory ) const
{
    if( pAssociatedAutomate_ )
        MIL_Report::PostEvent( *pAssociatedAutomate_, report::eRC_AllocationConsentieBientotEpuisee, dotationCategory );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::NotifyQuotaExceeded
// Created: MCO 2012-11-19
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::NotifyQuotaExceeded( const PHY_DotationCategory& dotationCategory, const T_Requesters& requesters ) const
{
    BOOST_FOREACH( T_Requesters::value_type pion, requesters )
    {
        MIL_Report::PostEvent( *pion, report::eRC_LogQuotaExceeded, dotationCategory );
        if( pAssociatedAutomate_ )
            MIL_Report::PostEvent( *pAssociatedAutomate_, report::eRC_LogQuotaExceededForAgent, dotationCategory, *pion );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::SupplyHandleRequest
// Created: NLD 2005-02-02
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::SupplyHandleRequest( const boost::shared_ptr < logistic::SupplyConsign_ABC >& consign )
{
    supplyConsigns_.push_back( consign );
}

namespace
{
    struct SupplyStockContainerVisitor : MIL_LogisticEntitiesVisitor
    {
        SupplyStockContainerVisitor( const PHY_DotationCategory& dotationCategory )
            : dotationCategory_( &dotationCategory )
            , found_( false )
        {}

        void Visit( const MIL_AgentPion& p )
        {
            if( found_ )
                return;
            const PHY_RoleInterface_Supply* candidate = p.RetrieveRole< PHY_RoleInterface_Supply >();
            found_ = candidate && candidate->CanReserveStock( *dotationCategory_ );
        }

        const PHY_DotationCategory* dotationCategory_;
        bool found_;
    };
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::SupplyHasStock
// Created: NLD 2005-02-01
// -----------------------------------------------------------------------------
bool MIL_AutomateLOG::SupplyHasStock( const PHY_DotationCategory& dotationCategory ) const
{
    SupplyStockContainerVisitor visitor( dotationCategory );
    Visit( visitor );
    return visitor.found_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::SupplyGetStock
// Created: NLD 2005-02-01
// -----------------------------------------------------------------------------
MIL_AutomateLOG::Stock MIL_AutomateLOG::SupplyGetStock( const PHY_DotationCategory& dotationCategory, double quantity ) const
{
    SupplyStockReservationVisitor visitor( dotationCategory, quantity );
    Visit( visitor );
    Stock result;
    result.quantity_ = quantity - visitor.remainingQuantity_;
    result.provider_ = visitor.provider_;
    return result;
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
MIL_AgentPion* MIL_AutomateLOG::SupplyCreateConvoyPion( const MIL_AgentTypePion& type, const boost::shared_ptr< logistic::SupplyConvoyReal_ABC >& convoy )
{
    // Search for the 'chief' automat
    MIL_Automate* pConvoyAutomate = pAssociatedAutomate_;
    if( pAssociatedFormation_ )
    {
        SupplyConvoyCapacityVisitor visitor;
        Visit( visitor );
        if( visitor.selected_ )
            pConvoyAutomate = &visitor.selected_->GetAutomate();
    }
    if( !pConvoyAutomate )
        return 0;

    const MT_Vector2D& location = pConvoyAutomate->GetPosition();

    MIL_AgentPion& convoyPion = MIL_AgentServer::GetWorkspace().GetEntityManager().CreatePion( type, *pConvoyAutomate, location, 0 );
    PHY_RoleInterface_Supply* itf = convoyPion.RetrieveRole< PHY_RoleInterface_Supply >();
    if( itf )
        itf->AssignConvoy( convoy );
    else
        MT_LOG_ERROR_MSG( "No role interface supply in convoy " << convoyPion.GetName() );
    return &convoyPion;
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
    if( convoyPion.IsImmobilized() )
        return;
    convoyPion.GetAutomate().DestroyPion( convoyPion );
}

namespace
{
    struct SupplyConvoyAvailabilityVisitor : MIL_LogisticEntitiesVisitor
    {
            SupplyConvoyAvailabilityVisitor( const PHY_DotationCategory& dotationCategory )
                : dotationCategory_( &dotationCategory )
                , pConvoySelected_( 0 )
                , selected_( 0 )
                , rTotalWeightMax_( 0 )
            {}

            void Visit( const MIL_AgentPion& tmp )
            {
                // NLD 2011-04-07 : Totally bugged ...
                // We must not use BL internal TC2 for external use
                //MIL_AutomateLOG* testBrain = tmp.GetAutomate().GetBrainLogistic();
                //if( bExternalTransfert_ && testBrain )
                    //return;
                const PHY_RoleInterface_Supply* candidate = tmp.RetrieveRole< PHY_RoleInterface_Supply >();
                PHY_ComposantePion* pTmpConvoySelected = candidate ? candidate->GetAvailableConvoyTransporter( *dotationCategory_ ) : 0;
                if( pTmpConvoySelected )
                {
                    double rTotalWeightMax = 0.;
                    double rTotalVolumeMax = 0.;
                    pTmpConvoySelected->GetStockTransporterCapacity( rTotalWeightMax, rTotalVolumeMax );
                    if( !selected_ || rTotalWeightMax_ > rTotalWeightMax )
                    {
                        rTotalWeightMax_ = rTotalWeightMax;
                        pConvoySelected_ = pTmpConvoySelected;
                        selected_       = const_cast<MIL_AgentPion*>(&tmp);
                    }
                }
            }

            const PHY_DotationCategory* dotationCategory_;
            PHY_ComposantePion* pConvoySelected_;
            MIL_AgentPion* selected_;
            double rTotalWeightMax_;
    };
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::SupplyGetAvailableConvoyTransporter
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
bool MIL_AutomateLOG::SupplyGetAvailableConvoyTransporter( PHY_ComposantePion*& pConvoyTransporter, MIL_AgentPion*& pConvoyTransporterPion, const PHY_DotationCategory& dotationCategory  ) const
{
    SupplyConvoyAvailabilityVisitor visitor( dotationCategory );
    Visit( visitor );
    pConvoyTransporter = visitor.pConvoySelected_;
    pConvoyTransporterPion = visitor.selected_;
    return pConvoyTransporter != 0;
}

namespace
{
    class SupplyConvoyTransporterVisitor : public MIL_LogisticEntitiesVisitor
                                         , private boost::noncopyable
    {
        public:
            SupplyConvoyTransporterVisitor( const PHY_ComposanteTypePion& type )
                : type_            ( type )
                , pConvoySelected_ ( 0 )
                , selected_       ( 0 )
            {
            }

            void Visit( const MIL_AgentPion& tmp )
            {
                // NLD 2011-04-07 : Totally bugged ...
                // We must not use BL internal TC2 for external use
                //MIL_AutomateLOG* testBrain = tmp.GetAutomate().GetBrainLogistic();
                //if( bExternalTransfert_ && testBrain )
                    //return;

                if( selected_ )
                    return;

                const PHY_RoleInterface_Supply* candidate = tmp.RetrieveRole< PHY_RoleInterface_Supply >();
                PHY_ComposantePion* pTmpConvoySelected = candidate!=0 ? candidate->GetAvailableConvoyTransporter( type_ ) : 0;
                if( pTmpConvoySelected )
                {
                    pConvoySelected_ = pTmpConvoySelected;
                    selected_       = const_cast<MIL_AgentPion*>(&tmp);
                }
            }

        public:
            const PHY_ComposanteTypePion& type_;
                  PHY_ComposantePion*     pConvoySelected_;
                  MIL_AgentPion*          selected_;
    };
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
    pConvoyTransporterPion = visitor.selected_;
    return pConvoyTransporter != 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::OnReceiveLogSupplyPushFlow
// Created: NLD 2011-07-20
// -----------------------------------------------------------------------------
bool MIL_AutomateLOG::OnReceiveLogSupplyPushFlow( const sword::PushFlowParameters& parameters, const AutomateFactory_ABC& automateResolver )
{
    boost::shared_ptr< logistic::SupplyRequestBuilder_ABC > builder;
    if( parameters.supply() )
        builder.reset( new logistic::SupplyDotationManualRequestBuilder( parameters, *this, automateResolver ) );
    else
        builder.reset( new logistic::SupplyStockPushFlowRequestBuilder( parameters, *this, automateResolver ) );
    boost::shared_ptr< logistic::SupplyRequestContainer > requestContainer( new logistic::SupplyRequestContainer( builder ) );
    logistic::SupplyRequestManualDispatcher dispatcher( *this );
    requestContainer->Execute( dispatcher );
    supplyRequests_.push_back( requestContainer );
    return dispatcher.AllowSupply();
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::ResetConsignsForConvoyPion
// Created: JSR 2013-02-06
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::ResetConsignsForConvoyPion( const MIL_AgentPion& pion )
{
    for( auto it = supplyConsigns_.begin(); it != supplyConsigns_.end(); ++it )
        ( *it )->ResetConsignsForConvoyPion( pion );
    for( auto it = supplyRequests_.begin(); it != supplyRequests_.end(); ++it )
        ( *it )->ResetConsignsForConvoyPion( pion );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::ResetConsignsForProvider
// Created: JSR 2013-02-11
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::ResetConsignsForProvider( const MIL_AgentPion& pion )
{
    for( auto it = supplyConsigns_.begin(); it != supplyConsigns_.end(); )
    {
        if( ( *it )->ResetConsignsForProvider( pion ) )
            it = supplyConsigns_.erase( it );
        else
            ++it;
    }
}
// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::CheckSupplyDeployed
// Created: MMC 2013-06-04
// -----------------------------------------------------------------------------
MIL_Automate* MIL_AutomateLOG::GetStockAutomat( const PHY_DotationCategory& dotationCategory, bool& deployed )
{
    deployed = false;
    SupplyDeployementVisitor visitor( dotationCategory );
    Visit( visitor );
    if( visitor.pDeployedStock_ )
    {
        deployed = true;
        return &visitor.pDeployedStock_->GetAutomate();
    }
    if( visitor.pUndeployedStock_ )
        return &visitor.pUndeployedStock_->GetAutomate();
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::OnSupplyConvoyArriving
// Created: NLD 2011-07-20
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::OnSupplyConvoyArriving( const boost::shared_ptr< logistic::SupplyConsign_ABC >& consign )
{
    const auto observers = supplyConvoysObserver_;
    boost::for_each( observers, boost::bind( &logistic::SupplyConvoysObserver_ABC::OnSupplyConvoyArriving, _1, consign ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::OnSupplyConvoyLeaving
// Created: NLD 2011-07-20
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::OnSupplyConvoyLeaving( const boost::shared_ptr< logistic::SupplyConsign_ABC >& consign )
{
    const auto observers = supplyConvoysObserver_;
    boost::for_each( observers, boost::bind( &logistic::SupplyConvoysObserver_ABC::OnSupplyConvoyLeaving, _1, consign ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::BelongsToLogisticBase
// Created: NLD 2011-07-20
// -----------------------------------------------------------------------------
bool MIL_AutomateLOG::BelongsToLogisticBase( const MIL_AutomateLOG& logisticBase ) const
{
    return &logisticBase == this;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::AddSupplyConvoysObserver
// Created: NLD 2011-07-20
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::AddSupplyConvoysObserver( logistic::SupplyConvoysObserver_ABC& observer )
{
    supplyConvoysObserver_.push_back( &observer );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::RemoveSupplyConvoysObserver
// Created: NLD 2011-07-20
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::RemoveSupplyConvoysObserver( logistic::SupplyConvoysObserver_ABC& observer )
{
    boost::remove_erase( supplyConvoysObserver_, &observer );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::FuneralGetNextPackagingResource
// Created: NLD 2011-07-20
// -----------------------------------------------------------------------------
const logistic::FuneralPackagingResource* MIL_AutomateLOG::FuneralGetNextPackagingResource( const logistic::FuneralPackagingResource* currentPackaging )
{
    FuneralPackagingResourceVisitor visitor( currentPackaging );
    Visit( visitor );
    if( visitor.nextPackagingResource_ )
    {
        assert( visitor.selected_ );
        double quantity = 1;
        visitor.selected_->Apply( &dotation::ConsumeDotationNotificationHandler_ABC::NotifyConsumeDotation, visitor.nextPackagingResource_->GetDotationCategory(), quantity );
        return visitor.nextPackagingResource_;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::UpdateLogistic
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::UpdateLogistic()
{
    boost::remove_erase_if( supplyConsigns_, std::mem_fn( &logistic::SupplyConsign_ABC::Update ) );
    boost::remove_erase_if( supplyRequests_, std::mem_fn( &logistic::SupplyRequestContainer::Update ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::Clean
// Created: NLD 2005-01-31
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::Clean()
{
    BOOST_FOREACH( T_SupplyRequests::value_type& data, supplyRequests_ )
        data->Clean();
    maintenanceManualModified_ = false;
    supplyManualModified_ = false;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::SendFullState
// Created: NLD 2005-02-01
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::SendFullState() const
{
    pLogisticHierarchy_->SendFullState();
    BOOST_FOREACH( const auto& data, supplyRequests_ )
        data->SendFullState();
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::SendChangedState
// Created: NLD 2011-01-11
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::SendChangedState() const
{
    try
    {
        pLogisticHierarchy_->SendChangedState();
        BOOST_FOREACH( const T_SupplyRequests::value_type& data, supplyRequests_ )
            data->SendChangedState();
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Error sending states of log automat " << GetLogisticId()
                << " : " << tools::GetExceptionMsg( e ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::GetLogisticId
// Created: AHC 2010-09-24
// -----------------------------------------------------------------------------
unsigned int MIL_AutomateLOG::GetLogisticId() const
{
    assert( pAssociatedFormation_ || pAssociatedAutomate_ );
    if( pAssociatedFormation_ )
        return pAssociatedFormation_->GetID();
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
    return visitor.selected_;
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
    static const MT_Vector2D none;
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

bool MIL_AutomateLOG::FinishAllHandlingsSuccessfullyWithoutDelay()
{
    boost::for_each( supplyConsigns_,
        boost::mem_fn( &logistic::SupplyConsign_ABC::FinishSuccessfullyWithoutDelay ) );
    const auto observers = supplyConvoysObserver_;
    boost::for_each( observers,
        boost::mem_fn( &logistic::SupplyConvoysObserver_ABC::FinishSuccessfullyWithoutDelay ) );
    return ! supplyConsigns_.empty() || ! observers.empty();
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::IsMaintenanceManual
// Created: SLI 2014-01-22
// -----------------------------------------------------------------------------
bool MIL_AutomateLOG::IsMaintenanceManual() const
{
    return maintenanceManual_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::IsSupplyManual
// Created: BAX 2014-02-26
// -----------------------------------------------------------------------------
bool MIL_AutomateLOG::IsSupplyManual() const
{
    return supplyManual_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::OnReceiveLogMaintenanceSetManual
// Created: SLI 2014-01-21
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::OnReceiveLogMaintenanceSetManual( const sword::MissionParameters& parameters )
{
    protocol::CheckCount( parameters, 1 );
    const bool value = protocol::GetBool( parameters, 0 );
    maintenanceManualModified_ = maintenanceManual_ != value;
    maintenanceManual_ = value;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::OnReceiveLogSupplySetManual
// Created: BAX 2014-02-26
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::OnReceiveLogSupplySetManual( const sword::MissionParameters& parameters )
{
    protocol::CheckCount( parameters, 1 );
    const bool value = protocol::GetBool( parameters, 0 );
    supplyManualModified_ = supplyManual_ != value;
    supplyManual_ = value;
}
