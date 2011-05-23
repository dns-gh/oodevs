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
#include "Entities/MIL_EntityManager.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Maintenance.h"
#include "Entities/Agents/Roles/Logistic/PHY_MaintenanceComposanteState.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Medical.h"
#include "Entities/Agents/Roles/Logistic/PHY_MedicalCollectionAmbulance.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Supply.h"
#include "Entities/Agents/Roles/Logistic/PHY_SupplyDotationState.h"
#include "Entities/Agents/Roles/Logistic/PHY_SupplyDotationRequestContainer.h"
#include "Entities/Agents/Roles/Logistic/PHY_SupplyDotationConsign.h"
#include "Entities/Agents/Roles/Logistic/PHY_SupplyStockState.h"
#include "Entities/Agents/Roles/Logistic/PHY_SupplyStockRequestContainer.h"
#include "Entities/Agents/Roles/Logistic/PHY_SupplyStockConsign.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Logistic/PHY_LogisticLevel.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Actions/PHY_ActionLogistic.h"
#include "Entities/Orders/MIL_Report.h"
#include "Network/NET_AsnException.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include <xeumeuleu/xml.hpp>
#include "Entities/Specialisations/LOG/MIL_LogisticVisitors.h"
#include "Entities/MIL_Formation.h"


BOOST_CLASS_EXPORT_IMPLEMENT( MIL_AutomateLOG )
using namespace sword;
using namespace sword;

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
    : pAssociatedAutomate_ ( 0 )
    , pAssociatedFormation_( &formation )
    , pCurrentSuperior_    ( 0 )
    , pNominalSuperior_    ( 0 )
    , pLogLevel_           ( &logLevel )
    , stockQuotasSuperior_        ()
    , stockQuotasNominalSuperior_ ()
    , supplyConsigns_             ()
    , pExplicitStockSupplyState_  ( 0 )
    , bQuotasHaveChanged_         ( false )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG constructor
// Created: NLD 2007-03-29
// -----------------------------------------------------------------------------
MIL_AutomateLOG::MIL_AutomateLOG( MIL_Automate& automate, const PHY_LogisticLevel& logLevel )
    : pAssociatedAutomate_ ( &automate )
    , pAssociatedFormation_( 0 )
    , pLogLevel_           ( &logLevel )
    , pCurrentSuperior_    ( 0 )
    , pNominalSuperior_    ( 0 )
    , stockQuotasSuperior_        ()
    , stockQuotasNominalSuperior_ ()
    , supplyConsigns_             ()
    , pExplicitStockSupplyState_  ( 0 )
    , bQuotasHaveChanged_         ( false )
{
}
// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG constructor
// Created: AHC 2010-09-28
// -----------------------------------------------------------------------------
MIL_AutomateLOG::MIL_AutomateLOG( const PHY_LogisticLevel& level )
    : pAssociatedAutomate_ ( 0 )
    , pAssociatedFormation_( 0 )
    , pLogLevel_           ( &level )
    , pCurrentSuperior_    ( 0 )
    , pNominalSuperior_    ( 0 )
    , stockQuotasSuperior_        ()
    , stockQuotasNominalSuperior_ ()
    , supplyConsigns_             ()
    , pExplicitStockSupplyState_  ( 0 )
    , bQuotasHaveChanged_         ( false )
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
namespace boost
{
    namespace serialization
    {
        template< typename Archive >
        inline void serialize( Archive& file, std::map< const PHY_DotationCategory*, MIL_AutomateLOG::sDotationQuota >& map, const unsigned int nVersion )
        {
            split_free( file, map, nVersion);
        }

        template < typename Archive >
        void save( Archive& file, const std::map< const PHY_DotationCategory*, MIL_AutomateLOG::sDotationQuota >& map, const unsigned int )
        {
            std::size_t size = map.size();
            file << size;
            for( std::map< const PHY_DotationCategory*, MIL_AutomateLOG::sDotationQuota >::const_iterator it = map.begin(); it != map.end(); ++it )
            {
                unsigned int id = it->first->GetMosID();
                file << id
                     << it->second.rQuota_
                     << it->second.rQuotaThreshold_;
            }
        }

        template < typename Archive >
        void load( Archive& file, std::map< const PHY_DotationCategory*, MIL_AutomateLOG::sDotationQuota >& map, const unsigned int )
        {
            std::size_t nNbr;
            file >> nNbr;
            while ( nNbr-- )
            {
                unsigned int nCategory;
                file >> nCategory;
                const PHY_DotationCategory* pCategory = PHY_DotationType::FindDotationCategory( nCategory );

                MIL_AutomateLOG::sDotationQuota quota;
                file >> quota.rQuota_
                     >> quota.rQuotaThreshold_;

                map.insert( std::make_pair( pCategory, quota ) );
            }
        }
    }
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
void MIL_AutomateLOG::serialize( Archive& file, const unsigned int )
{
    file & supplyConsigns_
         & pExplicitStockSupplyState_;

   file & pAssociatedAutomate_
        & pAssociatedFormation_
        & pCurrentSuperior_
        & pNominalSuperior_
        & stockQuotasSuperior_
        & stockQuotasNominalSuperior_
        & supplyConsigns_;
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
    pCurrentSuperior_ = pNominalSuperior_ = &superior;
    xis >> xml::optional >> xml::start( "quotas" )
            >> xml::list( "resource", *this, &MIL_AutomateLOG::ReadDotation )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::ReadDotation
// Created: ABL 2007-07-10
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::ReadDotation( xml::xistream& xis )
{
    std::string strType;
    xis >> xml::attribute( "name", strType );

    const PHY_DotationCategory* pDotationCategory = PHY_DotationType::FindDotationCategory( strType );
    if( !pDotationCategory )
        xis.error( "Unknown dotation type" );

    if( stockQuotasSuperior_.find( pDotationCategory ) != stockQuotasSuperior_.end() )
        xis.error( "Quota already defined" );

    unsigned int        nQuantity;
    xis >> xml::attribute( "quantity", nQuantity );
    if( nQuantity < 0 )
        xis.error( "nQuantity is not greater or equal to 0" );

    sDotationQuota quota;
    quota.rQuota_          = nQuantity;
    quota.rQuotaThreshold_ = nQuantity * 0.1; //$$ fichier de conf cpp ;)
    stockQuotasSuperior_[ pDotationCategory ] = quota;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::WriteLogisticLinksODB
// Created: NLD 2006-05-29
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::WriteLogisticLinksODB( xml::xostream& xos ) const
{
    if( !pNominalSuperior_ )
        return;

    xos << xml::start("base-logistique")
            << xml::attribute( "id", pNominalSuperior_->GetID() )
            << xml::start ( "subordinate" )
                << xml::attribute( "id", GetID() );
    if(!stockQuotasSuperior_.empty())
    {
        xos         << xml::start( "quotas" );
        for( CIT_DotationQuotaMap it = stockQuotasSuperior_.begin(); it != stockQuotasSuperior_.end(); ++it )
        {
           const PHY_DotationCategory& dotation = *it->first;
           xos << xml::start( "resource" )
                   << xml::attribute( "name", dotation.GetName() )
                   << xml::attribute( "quantity", it->second.rQuota_ )
               << xml::end; // dotation
        }
        xos         << xml::end; // quotas
    }
         xos << xml::end // subordinate
        << xml::end; // base-logistique
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
// Name: MIL_AutomateLOG::ConsumeQuota
// Created: NLD 2005-02-01
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::ConsumeQuota( const MIL_AutomateLOG& supplier, const PHY_DotationCategory& dotationCategory, double rQuotaConsumed )
{
    if(&supplier == pCurrentSuperior_)
    {
        IT_DotationQuotaMap it = stockQuotasSuperior_.find( &dotationCategory );
        assert( it != stockQuotasSuperior_.end() );

        sDotationQuota& quota = it->second;

        rQuotaConsumed = std::min( rQuotaConsumed, quota.rQuota_ );

        quota.rQuota_ -= rQuotaConsumed;
        if( quota.rQuota_ <= quota.rQuotaThreshold_ )
        {
            if( pAssociatedAutomate_ )
                MIL_Report::PostEvent( *pAssociatedAutomate_, MIL_Report::eReport_QuotaAlmostConsumed, dotationCategory );
        }
        bQuotasHaveChanged_ = true;
    }
    else if ( (pCurrentSuperior_ != pNominalSuperior_) && (&supplier == pNominalSuperior_) )
    {
        IT_DotationQuotaMap it = stockQuotasSuperior_.find( &dotationCategory );
        assert( it != stockQuotasSuperior_.end() );

        sDotationQuota& quota = it->second;

        rQuotaConsumed = std::min( rQuotaConsumed, quota.rQuota_ );

        quota.rQuota_ -= rQuotaConsumed;
        if( quota.rQuota_ <= quota.rQuotaThreshold_ )
        {
            if( pAssociatedAutomate_ )
                MIL_Report::PostEvent( *pAssociatedAutomate_, MIL_Report::eReport_QuotaAlmostConsumed, dotationCategory );
        }
        bQuotasHaveChanged_ = true;
    }
}

// =============================================================================
// SUPPLY
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::SupplyHandleRequest
// Created: NLD 2005-02-02
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::SupplyHandleRequest( PHY_SupplyDotationState& supplyDotationState, MIL_Automate& stockSupplier, bool bExternalTransfert  )
{
    supplyConsigns_.push_back( new PHY_SupplyDotationConsign( *this, supplyDotationState, stockSupplier, bExternalTransfert ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::SupplyHandleRequest
// Created: NLD 2005-02-02
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::SupplyHandleRequest( PHY_SupplyStockState& supplyStockState, MIL_Automate& stockSupplier, bool bExternalTransfert  )
{
    supplyConsigns_.push_back( new PHY_SupplyStockConsign( *this, supplyStockState, stockSupplier, bExternalTransfert ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::SupplyGetStockPion
// Created: NLD 2005-02-01
// -----------------------------------------------------------------------------
MIL_AgentPion* MIL_AutomateLOG::SupplyGetStockPion( const PHY_DotationCategory& dotationCategory, double rRequestedValue, bool bExternalTransfert ) const
{
    SupplyStockAvailabilityVisitor visitor( dotationCategory, rRequestedValue, bExternalTransfert );
    Visit( visitor );
    return visitor.pSelected_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::SupplyGetStock
// Created: NLD 2005-02-01
// -----------------------------------------------------------------------------
double MIL_AutomateLOG::SupplyGetStock( const PHY_DotationCategory& dotationCategory, double rRequestedValue, bool bExternalTransfert  ) const
{
    SupplyStockAvailabilityVisitor visitor( dotationCategory, rRequestedValue, bExternalTransfert );
    Visit( visitor );
    return visitor.pSelected_ ? visitor.pSelected_->GetRole< PHY_RoleInterface_Supply >().AddStockReservation( dotationCategory, rRequestedValue ) : 0.;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::SupplyReturnStock
// Created: NLD 2005-03-17
// -----------------------------------------------------------------------------
bool MIL_AutomateLOG::SupplyReturnStock( const PHY_DotationCategory& dotationCategory, double rReturnedValue ) const
{
    SupplyStockContainerVisitor visitor( dotationCategory );
    Visit( visitor );
    if( !visitor.pSelected_ )
        return false;
    visitor.pSelected_->RemoveStockReservation( dotationCategory, rReturnedValue );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::SupplyGetAvailableConvoyTransporter
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
bool MIL_AutomateLOG::SupplyGetAvailableConvoyTransporter( PHY_ComposantePion*& pConvoyTransporter, MIL_AgentPion*& pConvoyTransporterPion, const PHY_DotationCategory& dotationCategory , bool bExternalTransfert ) const
{
    SupplyConvoyAvailabilityVisitor visitor( dotationCategory, bExternalTransfert );
    Visit( visitor );

    pConvoyTransporter     = visitor.pConvoySelected_;
    pConvoyTransporterPion = visitor.pSelected_;
    return pConvoyTransporter != 0;
}

// =============================================================================
// UPDATE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::UpdateLogistic
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::UpdateLogistic()
{
    for( IT_SupplyConsignList it = supplyConsigns_.begin(); it != supplyConsigns_.end(); )
    {
        if( (**it).Update() )
        {
            delete *it;
            it = supplyConsigns_.erase( it );
        }
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
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::Clean
// Created: NLD 2005-01-31
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::Clean()
{
    bQuotasHaveChanged_ = false;

}


// =============================================================================
// NETWORK
// =============================================================================


// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::SendQuotas
// Created: NLD 2005-02-01
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::SendQuotas() const
{
    client::LogSupplyQuotas asn;

    if( pAssociatedAutomate_ )
        asn().mutable_supplied()->mutable_automat()->set_id( pAssociatedAutomate_->GetID() );
    else
        asn().mutable_supplied()->mutable_formation()->set_id( pAssociatedFormation_->GetID() );

    if( !stockQuotasSuperior_.empty() )
    {
        unsigned int i = 0;
        for( CIT_DotationQuotaMap it = stockQuotasSuperior_.begin(); it != stockQuotasSuperior_.end(); ++it, ++i )
        {
            sword::DotationQuota& dotQuota = *asn().mutable_quotas()->add_elem();
            dotQuota.mutable_resource()->set_id( it->first->GetMosID() );
            dotQuota.set_quantity( (unsigned int)it->second.rQuota_ );
        }
    }
    else
        asn().mutable_quotas();
    asn.Send( NET_Publisher_ABC::Publisher() );

    if( asn().quotas().elem_size() > 0 )
        asn().mutable_quotas()->Clear();
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::SendFullState
// Created: NLD 2005-02-01
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::SendFullState() const
{
    SendLogisticLinks();
    SendQuotas();
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::OnReceiveChangeLogisticLinks
// Created: NLD 2005-01-17
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::OnReceiveChangeLogisticLinks( const sword::UnitMagicAction& msg )
{
    unsigned int automatId = msg.parameters().elem( 1 ).value().Get(0).identifier();
    unsigned int formationId = msg.parameters().elem( 2 ).value().Get(0).identifier();
    if( ( automatId == ( unsigned int ) -1 ) && (formationId == ( unsigned int ) -1) )
        throw NET_AsnException< sword::HierarchyModificationAck_ErrorCode >( sword::HierarchyModificationAck::error_invalid_party );
    pCurrentSuperior_ = automatId!=0 ? GetLogisticAutomate(automatId) : GetLogisticAutomate(formationId);
    if( !pCurrentSuperior_ )
        pNominalSuperior_ = 0;
    if( !pNominalSuperior_ && pCurrentSuperior_ )
        pNominalSuperior_ = pCurrentSuperior_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::OnReceiveLogSupplyChangeQuotas
// Created: NLD 2005-02-03
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::OnReceiveLogSupplyChangeQuotas( const sword::MissionParameters& msg )
{
    unsigned int oid_donneur = msg.elem( 0 ).value().Get(0).has_automat() ?
        msg.elem( 0 ).value().Get(0).automat().id() : msg.elem( 0 ).value().Get(0).formation().id();
    if( ( !pCurrentSuperior_ && !pNominalSuperior_ ) ||
         ( GetLogisticAutomate( oid_donneur ) != pCurrentSuperior_ &&
           GetLogisticAutomate( oid_donneur ) != pNominalSuperior_)
         )
    {
        throw NET_AsnException< LogSupplyChangeQuotasAck::ErrorCode >( LogSupplyChangeQuotasAck::error_invalid_supplier );
    }

    if ( GetLogisticAutomate( oid_donneur ) == pCurrentSuperior_ )
    {
        for( int i = 0; i < msg.elem( 1 ).value_size(); ++i )
        {
            unsigned int type = msg.elem( 1 ).value().Get( i ).list( 0 ).identifier();
            int number = msg.elem( 1 ).value().Get( i ).list( 1 ).quantity();

            const PHY_DotationCategory* pDotationCategory = PHY_DotationType::FindDotationCategory( type );
            if( pDotationCategory )
            {
                sDotationQuota& dotationQuota = stockQuotasSuperior_[ pDotationCategory ];
                dotationQuota.rQuota_          = number;
                dotationQuota.rQuotaThreshold_ = number * 0.1; //$$ fichier de conf cpp ;)
            }
        }
    }
    else // nominal superior
    {
        for( int i = 0; i < msg.elem( 1 ).value_size(); ++i )
        {
            unsigned int type = msg.elem( 1 ).value().Get( i ).list( 0 ).identifier();
            int number = msg.elem( 1 ).value().Get( i ).list( 1 ).quantity();

            const PHY_DotationCategory* pDotationCategory = PHY_DotationType::FindDotationCategory( type );
            if( pDotationCategory )
            {
                sDotationQuota& dotationQuota = stockQuotasNominalSuperior_[ pDotationCategory ];
                dotationQuota.rQuota_          = number;
                dotationQuota.rQuotaThreshold_ = number * 0.1; //$$ fichier de conf cpp ;)
            }
        }
    }
    bQuotasHaveChanged_ = true;
    bQuotasHaveChanged_ = true;
}


// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::SendLogisticLinks
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::SendLogisticLinks() const
{
    client::ChangeLogisticLinks asn;

    if( GetAssociatedAutomat() )
       asn().mutable_requester()->mutable_automat()->set_id( GetID() );
   else
       asn().mutable_requester()->mutable_formation()->set_id( GetID() );

    if( pCurrentSuperior_ )
    {
        if( pCurrentSuperior_->GetAssociatedAutomat() )
            asn().mutable_logistic_base()->mutable_automat()->set_id( pCurrentSuperior_->GetID() );
        else
            asn().mutable_logistic_base()->mutable_formation()->set_id( pCurrentSuperior_->GetID() );
    }
    asn.Send( NET_Publisher_ABC::Publisher() );

}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::GetSuperior
// Created: NLD 2004-12-28
// -----------------------------------------------------------------------------
MIL_AutomateLOG* MIL_AutomateLOG::GetSuperior() const
{
    return pCurrentSuperior_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::GetMedicalSuperior
// Created: NLD 2004-12-28
// -----------------------------------------------------------------------------
MIL_AutomateLOG* MIL_AutomateLOG::GetNominalSuperior() const
{
    return pNominalSuperior_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::GetQuota
// Created: NLD 2005-02-01
// -----------------------------------------------------------------------------
double MIL_AutomateLOG::GetQuota( const MIL_AutomateLOG& supplier, const PHY_DotationCategory& dotationCategory ) const
{
    if( &supplier == pCurrentSuperior_)
    {
        CIT_DotationQuotaMap it = stockQuotasSuperior_.find( &dotationCategory );
        if( it == stockQuotasSuperior_.end() )
            return 0.;
        return it->second.rQuota_;
    }
    else if( pCurrentSuperior_!=pNominalSuperior_ && &supplier==pNominalSuperior_ )
    {
        CIT_DotationQuotaMap it = stockQuotasNominalSuperior_.find( &dotationCategory );
        if( it == stockQuotasNominalSuperior_.end() )
            return 0.;
        return it->second.rQuota_;
    }
    return 0;
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
MIL_Army_ABC& MIL_AutomateLOG::GetArmy     () const
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
// Name: MIL_AutomateLOG::SupplyCreatePionConvoy
// Created: AHC 2010-09-27
// -----------------------------------------------------------------------------
MIL_AgentPion* MIL_AutomateLOG::SupplyCreatePionConvoy( const MIL_AgentTypePion& type, bool bExternalTransfert )
{
    // Search for the 'chief' automat
    MIL_Automate* pConvoyAutomate = pAssociatedAutomate_;
    if( pAssociatedFormation_ )
    {
        SupplyConvoyCapacityVisitor visitor( bExternalTransfert );
        Visit( visitor );
        if( visitor.pSelected_ )
            pConvoyAutomate = &visitor.pSelected_->GetAutomate();
    }
    if( !pConvoyAutomate )
        return 0;
    const MT_Vector2D& location = pConvoyAutomate->GetPionPC().GetRole<PHY_RoleInterface_Location>().GetPosition();
    return &pConvoyAutomate->CreatePion( type, location );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::GetAssociatedFormation
// Created: AHC 2010-09-27
// -----------------------------------------------------------------------------
MIL_Automate*   MIL_AutomateLOG::GetAssociatedAutomat() const
{
    return pAssociatedAutomate_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::GetAssociatedFormation
// Created: AHC 2010-09-27
// -----------------------------------------------------------------------------
MIL_Formation*  MIL_AutomateLOG::GetAssociatedFormation() const
{
    return pAssociatedFormation_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::GetLogisticAutomate
// Created: NLD 2005-01-17
// -----------------------------------------------------------------------------
MIL_AutomateLOG* MIL_AutomateLOG::GetLogisticAutomate( unsigned int nID )
{
    MIL_Automate* pAutomate = MIL_AgentServer::GetWorkspace().GetEntityManager().FindAutomate( nID );
    if( pAutomate)
        return pAutomate->GetBrainLogistic();

    MIL_Formation* pFormation = MIL_AgentServer::GetWorkspace().GetEntityManager().FindFormation( nID );
    if( pFormation)
        return pFormation->GetBrainLogistic();

    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::IsExternalTransaction
// Indicate if the logistic transaction is internal to a BL or between two
// Created: AHC 2010-09-28
// -----------------------------------------------------------------------------
bool MIL_AutomateLOG::IsExternalTransaction( MIL_Automate& supplied, const MIL_AutomateLOG& supplier )
{
    bool bExternaltransfert = true;

    MIL_AutomateLOG* pTC2 = supplied.GetTC2();
    if( pTC2 && pTC2 == &supplier )
            bExternaltransfert = false;

    return bExternaltransfert;
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
// Name: MIL_AutomateLOG::FillParentEntity
// Created: AHC 2010-10-13
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::FillParentEntity(sword::ParentEntity& msg)
{
    if( pAssociatedAutomate_ )
        msg.mutable_automat()->set_id(pAssociatedAutomate_->GetID());
    else if( pAssociatedFormation_ )
        msg.mutable_formation()->set_id(pAssociatedFormation_->GetID());
    else
        msg.mutable_automat()->set_id( 0 );
}
