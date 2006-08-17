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

#include "MIL_pch.h"

#include "MIL_AutomateLOG.h"
#include "MIL_AutomateTypeLOG.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Agents/Roles/Logistic/Maintenance/PHY_RolePion_Maintenance.h"
#include "Entities/Agents/Roles/Logistic/Maintenance/PHY_MaintenanceComposanteState.h"
#include "Entities/Agents/Roles/Logistic/Medical/PHY_RolePion_Medical.h"
#include "Entities/Agents/Roles/Logistic/Medical/PHY_MedicalCollectionAmbulance.h"
#include "Entities/Agents/Roles/Logistic/Supply/PHY_RolePion_Supply.h"
#include "Entities/Agents/Roles/Logistic/Supply/PHY_SupplyDotationState.h"
#include "Entities/Agents/Roles/Logistic/Supply/PHY_SupplyDotationRequestContainer.h"
#include "Entities/Agents/Roles/Logistic/Supply/PHY_SupplyDotationConsign.h"
#include "Entities/Agents/Roles/Logistic/Supply/PHY_SupplyStockState.h"
#include "Entities/Agents/Roles/Logistic/Supply/PHY_SupplyStockRequestContainer.h"
#include "Entities/Agents/Roles/Logistic/Supply/PHY_SupplyStockConsign.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Actions/PHY_ActionLogistic.h"
#include "Entities/RC/MIL_RC_AllocationConsentieBientotEpuisee.h"
#include "Network/NET_ASN_Messages.h"

BOOST_CLASS_EXPORT_GUID( MIL_AutomateLOG, "MIL_AutomateLOG" )

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG constructor
// Created: NLD 2004-12-21
// -----------------------------------------------------------------------------
MIL_AutomateLOG::MIL_AutomateLOG( const MIL_AutomateTypeLOG& type, uint nID, MIL_InputArchive& archive )
    : MIL_Automate                ( type, nID, archive )
    , pMaintenanceSuperior_       ( 0 )
    , pMedicalSuperior_           ( 0 )
    , pSupplySuperior_            ( 0 )
    , stockQuotas_                ()
    , supplyConsigns_             ()
    , bStockSupplyNeeded_         ( false )
    , pExplicitStockSupplyState_  ( 0 )
    , pushedFlowsSupplyStates_    ()
    , bQuotasHaveChanged_         ( false )
    , nTickRcStockSupplyQuerySent_( 0 )
    , pLogisticAction_            ( new PHY_ActionLogistic< MIL_AutomateLOG >( *this ) )
{   
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG constructor
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
MIL_AutomateLOG::MIL_AutomateLOG()
    : MIL_Automate                ()
    , pMaintenanceSuperior_       ( 0 )
    , pMedicalSuperior_           ( 0 )
    , pSupplySuperior_            ( 0 )
    , stockQuotas_                ()
    , supplyConsigns_             ()
    , bStockSupplyNeeded_         ( false )
    , pExplicitStockSupplyState_  ( 0 )
    , pushedFlowsSupplyStates_    ()
    , bQuotasHaveChanged_         ( false )
    , nTickRcStockSupplyQuerySent_( 0 )
    , pLogisticAction_            ( new PHY_ActionLogistic< MIL_AutomateLOG >( *this ) )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG destructor
// Created: NLD 2004-12-21
// -----------------------------------------------------------------------------
MIL_AutomateLOG::~MIL_AutomateLOG()
{
    delete pLogisticAction_;
}

// =============================================================================
// CHEKPOINTS
// =============================================================================
namespace boost
{
    namespace serialization
    {
        template< typename Archive >
        inline void serialize( Archive& file, std::map< const PHY_DotationCategory*, MIL_AutomateLOG::sDotationQuota >& map, const uint nVersion )
        {
            split_free( file, map, nVersion); 
        }

        template < typename Archive >
        void save( Archive& file, const std::map< const PHY_DotationCategory*, MIL_AutomateLOG::sDotationQuota >& map, const uint )
        {
            file << map.size();
            for ( std::map< const PHY_DotationCategory*, MIL_AutomateLOG::sDotationQuota >::const_iterator it = map.begin(); it != map.end(); ++it )
            {
                file << it->first->GetMosID()
                     << it->second.rQuota_
                     << it->second.rQuotaThreshold_;
            }
        }
        
        template < typename Archive >
        void load( Archive& file, std::map< const PHY_DotationCategory*, MIL_AutomateLOG::sDotationQuota >& map, const uint )
        {
            uint nNbr;
            file >> nNbr;
            while ( nNbr-- )
            {
                uint nCategory;
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
// Name: MIL_AutomateLOG::serialize
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
template < typename Archive >
void MIL_AutomateLOG::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MIL_Automate >( *this );
    file & pMaintenanceSuperior_
         & pMedicalSuperior_
         & pSupplySuperior_
         & stockQuotas_
         & supplyConsigns_
         & bStockSupplyNeeded_
         & pExplicitStockSupplyState_
         & pushedFlowsSupplyStates_
         & nTickRcStockSupplyQuerySent_;
}
    
// =============================================================================
// INIT
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::ReadMaintenanceData
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::ReadMaintenanceData( MIL_InputArchive& archive )
{
    pMaintenanceSuperior_ = 0;
    if( !archive.Section( "Maintenance", MIL_InputArchive::eNothing ) )
        return;

    uint nAutomateID;
    archive.ReadAttribute( "automate", nAutomateID );

    MIL_Automate* pAutomate = MIL_AgentServer::GetWorkspace().GetEntityManager().FindAutomate( nAutomateID );
    if( !pAutomate )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown Maintenance automata", archive.GetContext() );
    if( !pAutomate->GetType().IsLogistic() )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Maintenance automata isn't a logistic automata", archive.GetContext() );
    pMaintenanceSuperior_ = static_cast< MIL_AutomateLOG* >( pAutomate );

    archive.EndSection(); // Maintenance
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::ReadMedicalData
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::ReadMedicalData( MIL_InputArchive& archive )
{
    pMedicalSuperior_ = 0;
    if( !archive.Section( "Sante", MIL_InputArchive::eNothing ) )
        return;

    uint nAutomateID;
    archive.ReadAttribute( "automate", nAutomateID );

    MIL_Automate* pAutomate = MIL_AgentServer::GetWorkspace().GetEntityManager().FindAutomate( nAutomateID );
    if( !pAutomate )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown Medical automata", archive.GetContext() );
    if( !pAutomate->GetType().IsLogistic() )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Medical automata isn't a logistic automata", archive.GetContext() );
    pMedicalSuperior_ = static_cast< MIL_AutomateLOG* >( pAutomate );

    archive.EndSection(); // Sante
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::ReadDotationQuotaCategories
// Created: NLD 2004-09-30
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::ReadDotationQuotaCategories( MIL_InputArchive& archive, const PHY_DotationType& dotationType )
{
    archive.BeginList( "Categories" );

    while ( archive.NextListElement() )
    {
        archive.Section( "Categorie" );
        
        std::string strCategoryName;
        archive.ReadAttribute( "nom", strCategoryName );

        const PHY_DotationCategory* pDotationCategory = dotationType.FindDotationCategory( strCategoryName );
        if ( !pDotationCategory )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown dotation category", archive.GetContext() );

        if( stockQuotas_.find( pDotationCategory ) != stockQuotas_.end() )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Quota already defined", archive.GetContext() );

        MT_Float rQuota;
        archive.ReadAttribute( "quota", rQuota );

        sDotationQuota quota;
        quota.rQuota_           = rQuota;
        quota.rQuotaThreshold_  = rQuota * 0.1; //$$ fichier de conf cpp ;)
        stockQuotas_[ pDotationCategory ] = quota;
    
        archive.EndSection(); // Categorie
    }
    archive.EndList(); // Categories    
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::ReadSupplyData
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::ReadSupplyData( MIL_InputArchive& archive )
{
    pSupplySuperior_ = 0;
    if( !archive.Section( "Ravitaillement", MIL_InputArchive::eNothing ) )
        return;

    uint nAutomateID;
    archive.ReadAttribute( "automate", nAutomateID );

    MIL_Automate* pAutomate = MIL_AgentServer::GetWorkspace().GetEntityManager().FindAutomate( nAutomateID );
    if( !pAutomate )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown Supply automata", archive.GetContext() );
    if( !pAutomate->GetType().IsLogistic() )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Supply automata isn't a logistic automata", archive.GetContext() );
    pSupplySuperior_ = static_cast< MIL_AutomateLOG* >( pAutomate );

    archive.BeginList( "Quotas" );
    while( archive.NextListElement() )
    {
        archive.Section( "Dotation" );

        std::string strDotationType;
        archive.ReadAttribute( "nom", strDotationType );

        const PHY_DotationType* pDotationType = PHY_DotationType::FindDotationType( strDotationType );
        if ( !pDotationType )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown dotation type", archive.GetContext() );
       
        ReadDotationQuotaCategories( archive, *pDotationType );
        archive.EndSection(); // Dotation
    }
    archive.EndList(); // Quotas
    archive.EndSection(); // Ravitaillement
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::ReadLogisticHierarchy
// Created: NLD 2004-12-21
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::ReadLogisticHierarchy( MIL_InputArchive& archive )
{
    MIL_Automate::ReadLogisticHierarchy( archive );

    pMaintenanceSuperior_ = 0;
    pMedicalSuperior_     = 0;
    pSupplySuperior_      = 0;

    archive.Section( "LiensHierarchiques" );
    if( !archive.Section( "Logistique", MIL_InputArchive::eNothing ) )
    {
        archive.EndSection(); // LiensHierarchiques
        return;
    }

    ReadMaintenanceData( archive );
    ReadMedicalData    ( archive );
    ReadSupplyData     ( archive );
 
    archive.EndSection(); // Logistique
    archive.EndSection(); // LiensHierarchiques
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::WriteLogisticHierarchy
// Created: NLD 2006-05-29
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::WriteLogisticHierarchy( MT_XXmlOutputArchive& archive ) const
{
    MIL_Automate::WriteLogisticHierarchy( archive );

    if( !pMaintenanceSuperior_ && !pMedicalSuperior_ && !pSupplySuperior_ )
        return;
    
    archive.Section( "Logistique" );
    if( pMaintenanceSuperior_ )
    {
        archive.Section( "Maintenance" );
        archive.WriteAttribute( "automate", pMaintenanceSuperior_->GetID() );
        archive.EndSection(); // Maintenance
    }
    
    if( pMedicalSuperior_ )
    {
        archive.Section( "Sante" );
        archive.WriteAttribute( "automate", pMedicalSuperior_->GetID() );
        archive.EndSection(); // Sante
    }

    if( pSupplySuperior_ )
    {
        archive.Section( "Ravitaillement" );
        archive.WriteAttribute( "automate", pSupplySuperior_->GetID() );

        archive.Section( "Quotas" );
        for( CIT_DotationQuotaMap it = stockQuotas_.begin(); it != stockQuotas_.end(); ++it )
        {
            const PHY_DotationCategory& dotationCategory = *it->first;

            archive.Section( "Dotation" );
            archive.WriteAttribute( "nom", dotationCategory.GetType().GetName() );
            archive.Section( "Categories" );
            archive.Section( "Categorie" );
            archive.WriteAttribute( "nom"  , dotationCategory.GetName() );
            archive.WriteAttribute( "quota", it->second.rQuota_ );
            archive.EndSection(); // Categorie
            archive.EndSection(); // Categories
            archive.EndSection(); // Dotation
        }           
        archive.EndSection(); // Quotas

        archive.EndSection(); // Ravitaillement
    }
    archive.EndSection(); // Logistique
}

// =============================================================================
// MAINTENANCE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::MaintenanceHandleComposanteForTransport
// Created: NLD 2004-12-28
// -----------------------------------------------------------------------------
PHY_MaintenanceComposanteState* MIL_AutomateLOG::MaintenanceHandleComposanteForTransport( MIL_AgentPion& pion, PHY_ComposantePion& composante )
{
    int nScore = std::numeric_limits< int >::min();
    PHY_RolePion_Maintenance* pSelectedRoleMaintenance = 0;
    const T_PionVector& pions = GetPions();
    for( CIT_PionVector itPion = pions.begin(); itPion != pions.end(); ++itPion )
    {
        PHY_RolePion_Maintenance& roleMaintenance = (**itPion).GetRole< PHY_RolePion_Maintenance >();
        const int nNewScore = roleMaintenance.GetAvailabilityScoreForTransport( composante );
        if( nNewScore > nScore )
        {
            nScore                   = nNewScore;
            pSelectedRoleMaintenance = &roleMaintenance;
        }
    }
    return pSelectedRoleMaintenance ? pSelectedRoleMaintenance->HandleComposanteForTransport( pion, composante ) : 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::MaintenanceHandleComposanteForTransport
// Created: NLD 2004-12-28
// -----------------------------------------------------------------------------
bool MIL_AutomateLOG::MaintenanceHandleComposanteForTransport( PHY_MaintenanceComposanteState& composanteState )
{
    int nScore = std::numeric_limits< int >::min();
    PHY_RolePion_Maintenance* pSelectedRoleMaintenance = 0;
    const T_PionVector& pions = GetPions();
    for( CIT_PionVector itPion = pions.begin(); itPion != pions.end(); ++itPion )
    {
        PHY_RolePion_Maintenance& roleMaintenance = (**itPion).GetRole< PHY_RolePion_Maintenance >();
        const int nNewScore = roleMaintenance.GetAvailabilityScoreForTransport( composanteState.GetComposante() );
        if( nNewScore > nScore )
        {
            nScore                   = nNewScore;
            pSelectedRoleMaintenance = &roleMaintenance;
        }
    }
    return pSelectedRoleMaintenance ? pSelectedRoleMaintenance->HandleComposanteForTransport( composanteState ) : false;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::MaintenanceHandleComposanteForRepair
// Created: NLD 2004-12-28
// -----------------------------------------------------------------------------
bool MIL_AutomateLOG::MaintenanceHandleComposanteForRepair( PHY_MaintenanceComposanteState& composanteState )
{
    int nScore = std::numeric_limits< int >::min();
    PHY_RolePion_Maintenance* pSelectedRoleMaintenance = 0;
    const T_PionVector& pions = GetPions();
    for( CIT_PionVector itPion = pions.begin(); itPion != pions.end(); ++itPion )
    {
        PHY_RolePion_Maintenance& roleMaintenance = (**itPion).GetRole< PHY_RolePion_Maintenance >();
        const int nNewScore = roleMaintenance.GetAvailabilityScoreForRepair( composanteState );
        if( nNewScore > nScore )
        {
            nScore                   = nNewScore;
            pSelectedRoleMaintenance = &roleMaintenance;
        }
    }
    return pSelectedRoleMaintenance ? pSelectedRoleMaintenance->HandleComposanteForRepair( composanteState ) : false;
}

// =============================================================================
// MEDICAL
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::MedicalHandleHumanEvacuatedByThirdParty
// Created: NLD 2005-08-01
// -----------------------------------------------------------------------------
PHY_MedicalHumanState* MIL_AutomateLOG::MedicalHandleHumanEvacuatedByThirdParty( MIL_AgentPion& pion, PHY_Human& human )
{
    const T_PionVector& pions  = GetPions();
    for( CIT_PionVector itPion = pions.begin(); itPion != pions.end(); ++itPion )
    {
        PHY_MedicalHumanState* pState = (**itPion).GetRole< PHY_RolePion_Medical >().HandleHumanEvacuatedByThirdParty( pion, human );
        if( pState )
            return pState;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::MedicalHandleHumanForEvacuation
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
PHY_MedicalHumanState* MIL_AutomateLOG::MedicalHandleHumanForEvacuation( MIL_AgentPion& pion, PHY_Human& human )
{
    int nScore = std::numeric_limits< int >::min();
    PHY_RolePion_Medical* pSelectedRoleMedical = 0;
    const T_PionVector& pions = GetPions();
    for( CIT_PionVector itPion = pions.begin(); itPion != pions.end(); ++itPion )
    {
        PHY_RolePion_Medical& roleMedical = (**itPion).GetRole< PHY_RolePion_Medical >();
        const int nNewScore = roleMedical.GetAvailabilityScoreForEvacuation();
        if( nNewScore > nScore )
        {
            nScore               = nNewScore;
            pSelectedRoleMedical = &roleMedical;
        }
    }
    return pSelectedRoleMedical ? pSelectedRoleMedical->HandleHumanForEvacuation( pion, human ) : 0;    
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::MedicalHandleHumanForCollection
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
bool MIL_AutomateLOG::MedicalHandleHumanForCollection( PHY_MedicalHumanState& humanState )
{
    int nScore = std::numeric_limits< int >::min();
    PHY_RolePion_Medical* pSelectedRoleMedical = 0;
    const T_PionVector& pions = GetPions();
    for( CIT_PionVector itPion = pions.begin(); itPion != pions.end(); ++itPion )
    {
        PHY_RolePion_Medical& roleMedical = (**itPion).GetRole< PHY_RolePion_Medical >();
        const int nNewScore = roleMedical.GetAvailabilityScoreForCollection();
        if( nNewScore > nScore )
        {
            nScore               = nNewScore;
            pSelectedRoleMedical = &roleMedical;
        }
    }
    return pSelectedRoleMedical ? pSelectedRoleMedical->HandleHumanForCollection( humanState ) : false;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::MedicalReserveForSorting
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
PHY_RolePion_Medical* MIL_AutomateLOG::MedicalReserveForSorting( PHY_MedicalCollectionAmbulance& ambulance )
{
    int nScore = std::numeric_limits< int >::min();
    PHY_RolePion_Medical* pSelectedRoleMedical = 0;
    const T_PionVector& pions = GetPions();
    for( CIT_PionVector itPion = pions.begin(); itPion != pions.end(); ++itPion )
    {
        PHY_RolePion_Medical& roleMedical = (**itPion).GetRole< PHY_RolePion_Medical >();
        const int nNewScore = roleMedical.GetAvailabilityScoreForSorting( ambulance );
        if( nNewScore > nScore )
        {
            nScore               = nNewScore;
            pSelectedRoleMedical = &roleMedical;
        }
    }
    if( !pSelectedRoleMedical )
        return 0;

    pSelectedRoleMedical->ReserveForSorting( ambulance );
    return pSelectedRoleMedical;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::MedicalHandleHumanForHealing
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
bool MIL_AutomateLOG::MedicalHandleHumanForHealing( PHY_MedicalHumanState& humanState )
{
    int nScore = std::numeric_limits< int >::min();
    PHY_RolePion_Medical* pSelectedRoleMedical = 0;
    const T_PionVector& pions = GetPions();
    for( CIT_PionVector itPion = pions.begin(); itPion != pions.end(); ++itPion )
    {
        PHY_RolePion_Medical& roleMedical = (**itPion).GetRole< PHY_RolePion_Medical >();
        const int nNewScore = roleMedical.GetAvailabilityScoreForHealing( humanState );
        if( nNewScore > nScore )
        {
            nScore               = nNewScore;
            pSelectedRoleMedical = &roleMedical;
        }
    }
    return pSelectedRoleMedical ? pSelectedRoleMedical->HandleHumanForHealing( humanState ) : false;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::MedicalCanCollectionAmbulanceGo
// Created: NLD 2005-01-13
// -----------------------------------------------------------------------------
bool MIL_AutomateLOG::MedicalCanCollectionAmbulanceGo( const PHY_MedicalCollectionAmbulance& ambulance ) const
{
    if( ambulance.IsAnEmergency() )
        return true;

    const T_PionVector& pions  = GetPions();
    for( CIT_PionVector itPion = pions.begin(); itPion != pions.end(); ++itPion )
    {
        if( !(**itPion).GetRole< PHY_RolePion_Medical >().CanCollectionAmbulanceGo( ambulance ) )
            return false;
    }
    return true;
}

// =============================================================================
// QUOTAS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::ConsumeQuota
// Created: NLD 2005-02-01
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::ConsumeQuota( const PHY_DotationCategory& dotationCategory, MT_Float rQuotaConsumed )
{
    IT_DotationQuotaMap it = stockQuotas_.find( &dotationCategory );
    assert( it != stockQuotas_.end() );
    
    sDotationQuota& quota = it->second;
    
    rQuotaConsumed = std::min( rQuotaConsumed, quota.rQuota_ );

    quota.rQuota_ -= rQuotaConsumed;
    if( quota.rQuota_ <= quota.rQuotaThreshold_ )
        MIL_RC::pRcAllocationConsentieBientotEpuisee_->Send( *this, MIL_RC::eRcTypeOperational, dotationCategory );
    bQuotasHaveChanged_ = true;  
}

// =============================================================================
// SUPPLY
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::SupplyHandleRequest
// Created: NLD 2005-02-02
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::SupplyHandleRequest( PHY_SupplyDotationState& supplyDotationState )
{
    supplyConsigns_.push_back( new PHY_SupplyDotationConsign( *this, supplyDotationState ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::SupplyHandleRequest
// Created: NLD 2005-02-02
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::SupplyHandleRequest( PHY_SupplyStockState& supplyStockState )
{
    supplyConsigns_.push_back( new PHY_SupplyStockConsign( *this, supplyStockState ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::SupplyGetStockPion
// Created: NLD 2005-02-01
// -----------------------------------------------------------------------------
PHY_RolePion_Supply* MIL_AutomateLOG::SupplyGetStockPion( const PHY_DotationCategory& dotationCategory, MT_Float rRequestedValue ) const
{
    const T_PionVector& pions  = GetPions();
    PHY_RolePion_Supply* pSelectedStockPion = 0;
    MT_Float             rScore             = 0;
    for( CIT_PionVector itPion = pions.begin(); itPion != pions.end(); ++itPion )
    {
        PHY_RolePion_Supply& stockPion = (**itPion).GetRole< PHY_RolePion_Supply >();

        const MT_Float rNewScore = stockPion.GetStockAvailablity( dotationCategory, rRequestedValue );
        if( rNewScore > rScore )
        {
            rScore             = rNewScore;
            pSelectedStockPion = &stockPion;
        }
    }
    return pSelectedStockPion;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::SupplyGetStock
// Created: NLD 2005-02-01
// -----------------------------------------------------------------------------
MT_Float MIL_AutomateLOG::SupplyGetStock( const PHY_DotationCategory& dotationCategory, MT_Float rRequestedValue ) const
{
    const T_PionVector& pions  = GetPions();
    PHY_RolePion_Supply* pSelectedStockPion = 0;
    MT_Float             rScore             = 0;
    for( CIT_PionVector itPion = pions.begin(); itPion != pions.end(); ++itPion )
    {
        PHY_RolePion_Supply& stockPion = (**itPion).GetRole< PHY_RolePion_Supply >();

        const MT_Float rNewScore = stockPion.GetStockAvailablity( dotationCategory, rRequestedValue );
        if( rNewScore > rScore )
        {
            rScore             = rNewScore;
            pSelectedStockPion = &stockPion;
        }
    }
    if( !pSelectedStockPion )
        return 0.;
    return pSelectedStockPion->AddStockReservation( dotationCategory, rRequestedValue );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::SupplyReturnStock
// Created: NLD 2005-03-17
// -----------------------------------------------------------------------------
bool MIL_AutomateLOG::SupplyReturnStock( const PHY_DotationCategory& dotationCategory, MT_Float rReturnedValue  ) const
{
    const T_PionVector& pions  = GetPions();
    for( CIT_PionVector itPion = pions.begin(); itPion != pions.end(); ++itPion )
    {
        PHY_RolePion_Supply& stockPion = (**itPion).GetRole< PHY_RolePion_Supply >();
        if( stockPion.CanContainStock( dotationCategory ) )
        {
            stockPion.RemoveStockReservation( dotationCategory, rReturnedValue );
            return true;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::SupplyGetAvailableConvoyTransporter
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
bool MIL_AutomateLOG::SupplyGetAvailableConvoyTransporter( PHY_ComposantePion*& pConvoyTransporter, MIL_AgentPion*& pConvoyTransporterPion, const PHY_DotationCategory& dotationCategory ) const
{
    pConvoyTransporter     = 0;
    pConvoyTransporterPion = 0;

    const T_PionVector& pions  = GetPions();
    for( CIT_PionVector itPion = pions.begin(); itPion != pions.end(); ++itPion )
    {
        pConvoyTransporter = (**itPion).GetRole< PHY_RolePion_Supply >().GetAvailableConvoyTransporter( dotationCategory );
        if( pConvoyTransporter )
        {
            pConvoyTransporterPion = *itPion;
            return true;
        }
    }
    return false;
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
    MIL_Automate::UpdateState();
    
    // Supply system (TC2->BLD->BLT)
    if( !bStockSupplyNeeded_ || pExplicitStockSupplyState_ || !pSupplySuperior_ )
        return;

    PHY_SupplyStockRequestContainer supplyRequests( *this );   
    bStockSupplyNeeded_ = !supplyRequests.Execute( *pSupplySuperior_, pExplicitStockSupplyState_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::Clean
// Created: NLD 2005-01-31
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::Clean()
{
    MIL_Automate::Clean();
    bQuotasHaveChanged_ = false;
    if( pExplicitStockSupplyState_ )
        pExplicitStockSupplyState_->Clean();
    for( CIT_SupplyStockStateSet it = pushedFlowsSupplyStates_.begin(); it != pushedFlowsSupplyStates_.end(); ++it )
        (**it).Clean();
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::IsSupplyInProgress
// Created: NLD 2005-12-14
// -----------------------------------------------------------------------------
bool MIL_AutomateLOG::IsSupplyInProgress( const PHY_DotationCategory& dotationCategory ) const
{
    if( pExplicitStockSupplyState_ && pExplicitStockSupplyState_->IsSupplying( dotationCategory ) )
        return true;
    
    for( CIT_SupplyStockStateSet it = pushedFlowsSupplyStates_.begin(); it != pushedFlowsSupplyStates_.end(); ++it )
    {
        if( (**it).IsSupplying( dotationCategory ) )
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::NotifyStockSupplyNeeded
// Created: NLD 2005-01-31
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::NotifyStockSupplyNeeded( const PHY_DotationCategory& dotationCategory )
{
    if( bStockSupplyNeeded_ )
        return;

    if( IsSupplyInProgress( dotationCategory ) )
        return;

    bStockSupplyNeeded_ = true;

    // Pas de RC si log non branchée ou si RC envoyé au tick précédent
    const uint nCurrentTick = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep();
    if( GetTC2() && ( nCurrentTick > ( nTickRcStockSupplyQuerySent_ + 1 ) || nTickRcStockSupplyQuerySent_ == 0 ) )
        MIL_RC::pRcDemandeRavitaillementStocks_->Send( *this, MIL_RC::eRcTypeOperational ); // Rcs uniquement quand la log est branchée
    nTickRcStockSupplyQuerySent_ = nCurrentTick;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::RemoveSupplyStockState
// Created: NLD 2005-12-14
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::RemoveSupplyStockState( const PHY_SupplyStockState& supplyState )
{
    if( &supplyState == pExplicitStockSupplyState_ )
    {
        pExplicitStockSupplyState_ = 0;
        return;
    }

    IT_SupplyStockStateSet it = pushedFlowsSupplyStates_.find( const_cast< PHY_SupplyStockState* >( &supplyState ) );
    assert( it != pushedFlowsSupplyStates_.end() );
    pushedFlowsSupplyStates_.erase( it );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::NotifyStockSupplied
// Created: NLD 2005-01-28
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::NotifyStockSupplied( const PHY_SupplyStockState& supplyState )
{
    MIL_RC::pRcRavitaillementStockEffectue_->Send( *this, MIL_RC::eRcTypeOperational );
    RemoveSupplyStockState( supplyState );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::NotifyStockSupplyCanceled
// Created: NLD 2005-02-11
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::NotifyStockSupplyCanceled( const PHY_SupplyStockState& supplyState )
{
    MIL_RC::pRcRavitaillementStockAnnule_->Send( *this, MIL_RC::eRcTypeOperational );
    RemoveSupplyStockState( supplyState );
    bStockSupplyNeeded_ = true;
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::GetLogisticAutomate
// Created: NLD 2005-01-17
// -----------------------------------------------------------------------------
MIL_AutomateLOG* MIL_AutomateLOG::GetLogisticAutomate( uint nID )
{
    MIL_Automate* pAutomate = MIL_AgentServer::GetWorkspace().GetEntityManager().FindAutomate( nID );
    if( !pAutomate || !pAutomate->GetType().IsLogistic() )
        return 0;
    return static_cast< MIL_AutomateLOG* >( pAutomate );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::UpdateNetwork
// Created: MIL_AutomateLOG 2005-01-25
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::UpdateNetwork() const
{
    MIL_Automate::UpdateNetwork();
    if( pExplicitStockSupplyState_ )
        pExplicitStockSupplyState_->SendChangedState();
    for( CIT_SupplyStockStateSet it = pushedFlowsSupplyStates_.begin(); it != pushedFlowsSupplyStates_.end(); ++it )
        (**it).SendChangedState();

    if( bQuotasHaveChanged_ )
        SendQuotas();
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::SendQuotas
// Created: NLD 2005-02-01
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::SendQuotas() const
{
    NET_ASN_MsgLogRavitaillementQuotas asn;
    asn.GetAsnMsg().oid_automate = GetID();
    asn.GetAsnMsg().quotas.n     = stockQuotas_.size();
    if( !stockQuotas_.empty() )
    {
        ASN1T_DotationQuota* pDotationQuota = new ASN1T_DotationQuota[ stockQuotas_.size() ];
        uint i = 0;
        for( CIT_DotationQuotaMap it = stockQuotas_.begin(); it != stockQuotas_.end(); ++it, ++i )
        {
            ASN1T_DotationQuota& dotQuota = pDotationQuota[ i ];
            dotQuota.ressource_id     = it->first->GetMosID();
            dotQuota.quota_disponible = (uint)it->second.rQuota_;
        }
        asn.GetAsnMsg().quotas.elem = pDotationQuota;
    }
    asn.Send();

    if( asn.GetAsnMsg().quotas.n > 0 )
        delete [] asn.GetAsnMsg().quotas.elem;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::SendFullState
// Created: NLD 2005-02-01
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::SendFullState() const
{
    MIL_Automate::SendFullState();
    if( pExplicitStockSupplyState_ )
        pExplicitStockSupplyState_->SendFullState();
    for( CIT_SupplyStockStateSet it = pushedFlowsSupplyStates_.begin(); it != pushedFlowsSupplyStates_.end(); ++it )
        (**it).SendFullState();
    SendQuotas();
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::OnReceiveMsgChangeLogisticLinks
// Created: NLD 2005-01-17
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::OnReceiveMsgChangeLogisticLinks( ASN1T_MsgChangeLiensLogistiques& msg, MIL_MOSContextID nCtx )
{
    NET_ASN_MsgChangeLiensLogistiquesAck asnReplyMsg;
    asnReplyMsg.GetAsnMsg().oid_automate = msg.oid_automate;

    bool bNewTC2                 = false;
    bool bNewMaintenanceSuperior = false;
    bool bNewMedicalSuperior     = false;
    bool bNewSupplySuperior      = false;

    MIL_AutomateLOG* pNewTC2                 = 0;
    MIL_AutomateLOG* pNewMaintenanceSuperior = 0;
    MIL_AutomateLOG* pNewMedicalSuperior     = 0;
    MIL_AutomateLOG* pNewSupplySuperior      = 0;

    if( msg.m.oid_tc2Present )
    {
        bNewTC2 = true;
        asnReplyMsg.GetAsnMsg().m.oid_tc2Present = 1;
        asnReplyMsg.GetAsnMsg().oid_tc2 = msg.oid_tc2;
        if( msg.oid_tc2 != 0 )
        {
            pNewTC2 = GetLogisticAutomate( msg.oid_tc2 );
            if( !pNewTC2 )
            {
                asnReplyMsg.GetAsnMsg().error_code = EnumChangeLiensLogistiquesErrorCode::error_invalid_automate_tc2;
                asnReplyMsg.Send( nCtx );
                return;
            }
        }
    }    
    if( msg.m.oid_maintenancePresent )
    {
        bNewMaintenanceSuperior = true;
        asnReplyMsg.GetAsnMsg().m.oid_maintenancePresent = 1;
        asnReplyMsg.GetAsnMsg().oid_maintenance = msg.oid_maintenance;

        if( msg.oid_maintenance != 0 )
        {
            pNewMaintenanceSuperior = GetLogisticAutomate( msg.oid_maintenance );
            if( !pNewMaintenanceSuperior )
            {
                asnReplyMsg.GetAsnMsg().error_code = EnumChangeLiensLogistiquesErrorCode::error_invalid_automate_maintenance;
                asnReplyMsg.Send( nCtx );
                return;
            }
        }
    }
    if( msg.m.oid_santePresent )
    {
        bNewMedicalSuperior = true;
        asnReplyMsg.GetAsnMsg().m.oid_santePresent = 1;
        asnReplyMsg.GetAsnMsg().oid_sante = msg.oid_sante;

        if( msg.oid_sante != 0 )
        {
            pNewMedicalSuperior = GetLogisticAutomate( msg.oid_sante );
            if( !pNewMedicalSuperior )
            {
                asnReplyMsg.GetAsnMsg().error_code = EnumChangeLiensLogistiquesErrorCode::error_invalid_automate_sante;
                asnReplyMsg.Send( nCtx );
                return;
            }
        }
    }
    if( msg.m.oid_ravitaillementPresent )
    {
        bNewSupplySuperior = true;
        asnReplyMsg.GetAsnMsg().m.oid_ravitaillementPresent = 1;
        asnReplyMsg.GetAsnMsg().oid_ravitaillement = msg.oid_ravitaillement;

        if( msg.oid_ravitaillement != 0 )
        {
            pNewSupplySuperior = GetLogisticAutomate( msg.oid_ravitaillement );
            if( !pNewSupplySuperior )
            {
                asnReplyMsg.GetAsnMsg().error_code = EnumChangeLiensLogistiquesErrorCode::error_invalid_automate_ravitaillement;
                asnReplyMsg.Send( nCtx );
                return;
            }
        }
    }

    if( bNewTC2 ) 
        pTC2_ = pNewTC2;
    if( bNewMaintenanceSuperior )
        pMaintenanceSuperior_ = pNewMaintenanceSuperior;
    if( bNewMedicalSuperior )
        pMedicalSuperior_ = pNewMedicalSuperior;
    if( bNewSupplySuperior )
        pSupplySuperior_ = pNewSupplySuperior;

    asnReplyMsg.GetAsnMsg().error_code = EnumChangeLiensLogistiquesErrorCode::no_error;
    asnReplyMsg.Send( nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::OnReceiveMsgLogSupplyChangeQuotas
// Created: NLD 2005-02-03
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::OnReceiveMsgLogSupplyChangeQuotas( ASN1T_MsgLogRavitaillementChangeQuotas& asnMsg, MIL_MOSContextID nCtx )
{
    NET_ASN_MsgLogRavitaillementChangeQuotasAck asnReplyMsg;

    if( !pSupplySuperior_ || GetLogisticAutomate( asnMsg.oid_donneur ) != pSupplySuperior_ )
    {
        asnReplyMsg.GetAsnMsg() = MsgLogRavitaillementChangeQuotasAck::error_invalid_donneur;
        asnReplyMsg.Send( nCtx );
        return;
    }

    for( uint i = 0; i < asnMsg.quotas.n; ++i )
    {
        ASN1T_DotationQuota& asnQuota = asnMsg.quotas.elem[ i ];

        const PHY_DotationCategory* pDotationCategory = PHY_DotationType::FindDotationCategory( asnQuota.ressource_id );
        if( pDotationCategory )
        {
            sDotationQuota& dotationQuota = stockQuotas_[ pDotationCategory ];
            dotationQuota.rQuota_          = asnQuota.quota_disponible;
            dotationQuota.rQuotaThreshold_ = asnQuota.quota_disponible * 0.1; //$$ fichier de conf cpp ;)
        }
    }
    asnReplyMsg.GetAsnMsg() = MsgLogRavitaillementChangeQuotasAck::no_error;
    asnReplyMsg.Send( nCtx );
    bQuotasHaveChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::OnReceiveMsgLogSupplyPushFlow
// Created: NLD 2005-02-04
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::OnReceiveMsgLogSupplyPushFlow( ASN1T_MsgLogRavitaillementPousserFlux& asnMsg, MIL_MOSContextID nCtx )
{
    NET_ASN_MsgLogRavitaillementPousserFluxAck asnReplyMsg;

    MIL_AutomateLOG* pSupplier = GetLogisticAutomate( asnMsg.oid_donneur );

    if( !pSupplier )
    {
        asnReplyMsg.GetAsnMsg() = MsgLogRavitaillementPousserFluxAck::error_invalid_donneur;
        asnReplyMsg.Send( nCtx );
        return;
    }

    PHY_SupplyStockRequestContainer supplyRequests( *this, asnMsg.stocks );

    PHY_SupplyStockState* pSupplyState = 0;
    supplyRequests.Execute( *pSupplier, pSupplyState );
    if( pSupplyState )
        pushedFlowsSupplyStates_.insert( pSupplyState );

    asnReplyMsg.GetAsnMsg() = MsgLogRavitaillementPousserFluxAck::no_error;
    asnReplyMsg.Send( nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::WriteCreationMsg
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
void MIL_AutomateLOG::WriteCreationMsg( NET_ASN_MsgAutomateCreation& asnMsg ) const
{
    MIL_Automate::WriteCreationMsg( asnMsg );    

    if( pMaintenanceSuperior_ )
    {
        asnMsg.GetAsnMsg().m.oid_maintenancePresent = 1;
        asnMsg.GetAsnMsg().oid_maintenance          = pMaintenanceSuperior_->GetID();
    }
    if( pMedicalSuperior_ )
    {
        asnMsg.GetAsnMsg().m.oid_santePresent = 1;
        asnMsg.GetAsnMsg().oid_sante          = pMedicalSuperior_->GetID();
    }
    if( pSupplySuperior_ )
    {
        asnMsg.GetAsnMsg().m.oid_ravitaillementPresent = 1;
        asnMsg.GetAsnMsg().oid_ravitaillement          = pSupplySuperior_->GetID();
    }
}
