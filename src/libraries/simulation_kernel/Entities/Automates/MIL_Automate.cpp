// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Automates/MIL_Automate.cpp $
// $Author: Nld $
// $Modtime: 12/05/05 17:39 $
// $Revision: 32 $
// $Workfile: MIL_Automate.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_Automate.h"

#include "MIL_AutomateType.h"
#include "DEC_AutomateDecision.h"

#include "Entities/MIL_Formation.h"
#include "Entities/Agents/MIL_AgentTypePion.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Refugee/PHY_RolePion_Refugee.h"
#include "Entities/Agents/Roles/Surrender/PHY_RolePion_Surrender.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Agents/Roles/Perception/PHY_RolePion_Perceiver.h"
#include "Entities/Agents/Roles/Logistic/Supply/PHY_SupplyDotationState.h"
#include "Entities/Agents/Roles/Logistic/Supply/PHY_SupplyDotationRequestContainer.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Objects/MIL_RealObject_ABC.h"
#include "Entities/Objects/MIL_RealObjectType.h"
#include "Entities/Objects/MIL_CampPrisonniers.h"
#include "Entities/Objects/MIL_CampRefugies.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Entities/Specialisations/LOG/MIL_AutomateTypeLOG.h"
#include "Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "Entities/Orders/Automate/MIL_AutomateMission_ABC.h"
#include "Entities/RC/MIL_RC.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/MIL_Army.h"
#include "Network/NET_AS_MOSServerMsgMgr.h"
#include "Network/NET_AgentServer.h"
#include "Network/NET_ASN_Messages.h"
#include "Decision/DEC_ModelAutomate.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Automate.h"
#include "Tools/MIL_Tools.h"
#include "Tools/MIL_IDManager.h"

BOOST_CLASS_EXPORT_GUID( MIL_Automate, "MIL_Automate" )

// -----------------------------------------------------------------------------
// Name: MIL_Automate constructor
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_Automate::MIL_Automate( const MIL_AutomateType& type, uint nID, MIL_Formation& formation, MIL_InputArchive& archive )
    : pType_                             ( &type )
    , nID_                               ( nID )
    , pFormation_                        ( &formation )
    , strName_                           ( type.GetName() )
    , bEmbraye_                          ( true )
    , pKnowledgeGroup_                   ( 0 )
    , pDecision_                         ( 0 )
    , orderManager_                      ( *this )
    , pPionPC_                           ( 0 )
    , pions_                             ()
    , recycledPions_                     ()
    , bAutomateModeChanged_              ( true )
    , pTC2_                              ( 0 )
    , pNominalTC2_                       ( 0 )
    , bDotationSupplyNeeded_             ( false )
    , bDotationSupplyExplicitlyRequested_( false )
    , dotationSupplyStates_              ( )
    , nTickRcDotationSupplyQuerySent_    ( 0 )
    , pKnowledgeBlackBoard_              ( new DEC_KnowledgeBlackBoard_Automate( *this ) )
    , bSurrendered_                      ( false )
    , bPrisoner_                         ( false )
    , pPrisonerCamp_                     ( 0 )
    , pRefugeeCamp_                      ( false )
{
    archive.ReadAttribute( "engaged", bEmbraye_, MIL_InputArchive::eNothing );
    archive.ReadAttribute( "name"   , strName_ , MIL_InputArchive::eNothing );

    uint nKnowledgeGroup;
    archive.ReadAttribute( "knowledge-group", nKnowledgeGroup );
    pKnowledgeGroup_ = GetArmy().FindKnowledgeGroup( nKnowledgeGroup );
    if( !pKnowledgeGroup_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown knowledge group", archive.GetContext() );
    pKnowledgeGroup_->RegisterAutomate( *this );
    pFormation_     ->RegisterAutomate( *this );

    pDecision_ = new DEC_AutomateDecision( *this ); //$$$ BULLSHIT : strName_ must be initialized ...

    InitializeSubordinates( archive );    
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate constructor
// Created: JVT 2005-03-15
// -----------------------------------------------------------------------------
MIL_Automate::MIL_Automate()
    : pType_                             ( 0 )
    , nID_                               ( 0 )
    , pFormation_                        ( 0 )
    , strName_                           ()
    , bEmbraye_                          ( true )
    , pKnowledgeGroup_                   ( 0 )
    , pDecision_                         ( 0 )
    , orderManager_                      ( *this )
    , pPionPC_                           ( 0 )
    , pions_                             ()
    , recycledPions_                     ()
    , bAutomateModeChanged_              ( true )
    , pTC2_                              ( 0 )
    , pNominalTC2_                       ( 0 )
    , bDotationSupplyNeeded_             ( false )
    , bDotationSupplyExplicitlyRequested_( false )
    , dotationSupplyStates_              ( )
    , nTickRcDotationSupplyQuerySent_    ( 0 )
    , pKnowledgeBlackBoard_              ( 0 )
    , bSurrendered_                      ( false )
    , bPrisoner_                         ( false )
    , pPrisonerCamp_                     ( 0 )
    , pRefugeeCamp_                      ( false )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate destructor
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_Automate::~MIL_Automate()
{
    pFormation_     ->UnregisterAutomate( *this );
    pKnowledgeGroup_->UnregisterAutomate( *this );
    delete pDecision_;
}

// =============================================================================
// CHEKPOINTS
// =============================================================================
namespace boost
{
    namespace serialization
    {
        template< typename Archive >
        inline
        void serialize( Archive& file, MIL_Automate::T_SupplyDotationStateMap& map, const uint nVersion )
        {
            split_free( file, map, nVersion );
        }

        template< typename Archive >
        void save( Archive& file, const MIL_Automate::T_SupplyDotationStateMap& map, const uint )
        {
            file << map.size();
            for ( MIL_Automate::CIT_SupplyDotationStateMap it = map.begin(); it != map.end(); ++it )
            {
                file << it->first;
                file << it->second;
            }
        }
        
        template< typename Archive >
        void load( Archive& file, MIL_Automate::T_SupplyDotationStateMap& map, const uint )
        {
            uint nNbr;
            
            file >> nNbr;
            while ( nNbr-- )
            {
                MIL_AutomateLOG* pLogAutomata;
                
                file >> pLogAutomata;
                file >> map[ pLogAutomata ];
            }
        }
    }
}
// -----------------------------------------------------------------------------
// Name: MIL_Automate::load
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_Automate::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> pTC2_
         >> pNominalTC2_;
    
    uint nTypeID;
    file >> nTypeID;
    pType_ = MIL_AutomateType::FindAutomateType( nTypeID );
    assert( pType_ );
    
    file >> const_cast< uint& >( nID_ )
         >> pFormation_
         >> strName_
         >> bEmbraye_
         >> pKnowledgeGroup_
         >> pDecision_
         >> pPionPC_
         >> pions_
         >> recycledPions_
         >> bAutomateModeChanged_
         >> bDotationSupplyNeeded_
         >> bDotationSupplyExplicitlyRequested_
         >> dotationSupplyStates_
         >> pKnowledgeBlackBoard_
         >> bSurrendered_
         >> bPrisoner_
         >> const_cast< MIL_CampPrisonniers*& >( pPrisonerCamp_ )
         >> const_cast< MIL_CampRefugies*&    >( pRefugeeCamp_  )
         >> nTickRcDotationSupplyQuerySent_;

    if( pRefugeeCamp_ )
    {
        Embraye();
        MIL_AutomateMission_ABC& mission = MIL_AutomateMissionType::GetMoveToRefugeeCampMissionType().InstanciateMission( *this );
        mission.Initialize();
        mission.Prepare   ();
        orderManager_.OnReceiveAutomateOrder( mission );
    }

    if( bPrisoner_ && bEmbraye_ )
    {
        MIL_AutomateMission_ABC& mission = MIL_AutomateMissionType::GetSurrenderingMissionType().InstanciateMission( *this );
        mission.Initialize();
        mission.Prepare   ();
        orderManager_.OnReceiveAutomateOrder( mission );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::save
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_Automate::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    assert( pType_ );
    
    file << pTC2_
         << pNominalTC2_
         << pType_->GetID()
         << nID_
         << pFormation_
         << strName_
         << bEmbraye_
         << pKnowledgeGroup_
         << pDecision_
         << pPionPC_
         << pions_
         << recycledPions_
         << bAutomateModeChanged_
         << bDotationSupplyNeeded_
         << bDotationSupplyExplicitlyRequested_
         << dotationSupplyStates_
         << pKnowledgeBlackBoard_
         << bSurrendered_
         << bPrisoner_
         << pPrisonerCamp_
         << pRefugeeCamp_
         << nTickRcDotationSupplyQuerySent_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::InitializeSubordinates
// Created: NLD 2006-10-11
// -----------------------------------------------------------------------------
void MIL_Automate::InitializeSubordinates( MIL_InputArchive& archive )
{
    while( archive.NextListElement() )
    {
        archive.Section( "unit" );

        uint        nID;
        std::string strType;
        bool        bPC = false;

        archive.ReadAttribute( "id"          , nID     );
        archive.ReadAttribute( "type"        , strType );
        archive.ReadAttribute( "command-post", bPC, MIL_InputArchive::eNothing );

        if( bPC && pPionPC_ )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Automat's command post already defined", archive.GetContext() );

        const MIL_AgentTypePion* pType = MIL_AgentTypePion::FindPionType( strType );
        if( !pType )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown pawn type", archive.GetContext() );

        MIL_AgentPion& pion = MIL_AgentServer::GetWorkspace().GetEntityManager().CreatePion( *pType, nID, *this, archive ); // Auto-registration
        if( bPC )
            pPionPC_ = &pion;

        archive.EndSection(); // unit
    }
}


// =============================================================================
// DYNAMIC PIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreatePion
// Created: NLD 2005-02-08
// -----------------------------------------------------------------------------
MIL_AgentPion& MIL_Automate::CreatePion( const MIL_AgentTypePion& type, const MT_Vector2D& vPosition )
{
    for( RIT_PionVector it = recycledPions_.rbegin(); it != recycledPions_.rend(); ++it )
    {
        MIL_AgentPion& pion = **it;
        if( pion.GetType() == type )
        {
            recycledPions_.erase( it.base() - 1 );
            pion.ChangeAutomate( *this );
            pion.GetRole< PHY_RolePion_Location >().MagicMove( vPosition );
            return pion;
        }
    }
    return MIL_AgentServer::GetWorkspace().GetEntityManager().CreatePion( type, *this, vPosition );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::DestroyPion
// Created: NLD 2005-02-08
// -----------------------------------------------------------------------------
void MIL_Automate::DestroyPion( MIL_AgentPion& pion )
{
    assert( pion.IsDead() );
    recycledPions_.push_back( &pion );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::WriteODB
// Created: NLD 2006-05-29
// -----------------------------------------------------------------------------
void MIL_Automate::WriteODB( MT_XXmlOutputArchive& archive ) const
{
    assert( pType_ );
    assert( pKnowledgeGroup_ );

    archive.Section( "automat" );
    archive.WriteAttribute( "id"             , nID_ );
    archive.WriteAttribute( "name"           , strName_ );
    archive.WriteAttribute( "engaged"        , bEmbraye_ );
    archive.WriteAttribute( "knowledge-group", pKnowledgeGroup_->GetID() ); 
    archive.WriteAttribute( "type"           , pType_->GetName() );

    for( CIT_PionVector it = pions_.begin(); it != pions_.end(); ++it )
        (**it).WriteODB( archive );

    archive.EndSection(); // automat
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::ReadOverloading
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
void MIL_Automate::ReadOverloading( MIL_InputArchive& /*archive*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::ReadLogisticLink
// Created: NLD 2006-10-19
// -----------------------------------------------------------------------------
void MIL_Automate::ReadLogisticLink( MIL_AutomateLOG& superior, MIL_InputArchive& archive )
{
    std::string strLink;
    archive.ReadAttribute( "link", strLink );

    if( sCaseInsensitiveEqual()( strLink, "tc2" ) )
    {
        pTC2_        = &superior;
        pNominalTC2_ = pTC2_;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::WriteLogisticLinksODB
// Created: NLD 2006-10-19
// -----------------------------------------------------------------------------
void MIL_Automate::WriteLogisticLinksODB( MT_XXmlOutputArchive& archive ) const
{
    if( pTC2_ )
    {
        archive.Section( "automat" );
        archive.WriteAttribute( "id", pTC2_->GetID() );
        archive.Section( "subordinate" );
        archive.WriteAttribute( "automat", GetID() );
        archive.WriteAttribute( "link", "tc2" );
        archive.EndSection(); // subordinate
        archive.EndSection(); // automat
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::CheckComposition
// Created: NLD 2003-11-21
// -----------------------------------------------------------------------------
bool MIL_Automate::CheckComposition() const
{
    assert( pType_ );
    return pType_->CheckComposition( *this );
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Automate::UpdateDecision
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
void MIL_Automate::UpdateDecision()
{
    assert( pDecision_ );
    pDecision_->UpdateDecision();
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::UpdateKnowledges
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void MIL_Automate::UpdateKnowledges()
{
    // Pions (+ PC)
    for( CIT_PionVector itPion = pions_.begin(); itPion != pions_.end(); ++itPion )
        (**itPion).UpdateKnowledges();

    assert( pKnowledgeBlackBoard_ );
    pKnowledgeBlackBoard_->Update();
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::CleanKnowledges
// Created: NLD 2005-09-01
// -----------------------------------------------------------------------------
void MIL_Automate::CleanKnowledges()
{
    assert( pKnowledgeBlackBoard_ );
    pKnowledgeBlackBoard_->Clean();

    // Pions (+ PC)
    for( CIT_PionVector itPion = pions_.begin(); itPion != pions_.end(); ++itPion )
        (**itPion).CleanKnowledges();    
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::UpdateNetwork
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
void MIL_Automate::UpdateNetwork() const
{
    assert( pDecision_ );
    if( bAutomateModeChanged_ || pDecision_->HasStateChanged() )
    {
        NET_ASN_MsgAutomateAttributes msg;
        msg.GetAsnMsg().oid_automate = nID_;
    
        if( bAutomateModeChanged_ )
        {
            msg.GetAsnMsg().m.etat_automatePresent = 1;
            msg.GetAsnMsg().etat_automate = bEmbraye_ ? EnumAutomateState::embraye : EnumAutomateState::debraye;
        }

        pDecision_->SendChangedState( msg );
        msg.Send();
    }

    for( CIT_SupplyDotationStateMap it = dotationSupplyStates_.begin(); it != dotationSupplyStates_.end(); ++it )
        it->second->SendChangedState();
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::UpdateState
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
void MIL_Automate::UpdateState()
{
    orderManager_.Update();

    if( !bDotationSupplyNeeded_ || !dotationSupplyStates_.empty() || ( !pTC2_ && !pNominalTC2_ ) )
        return;

    PHY_SupplyDotationRequestContainer supplyRequests( *this, bDotationSupplyExplicitlyRequested_ );
    bDotationSupplyNeeded_ = !supplyRequests.Execute( pTC2_, pNominalTC2_, dotationSupplyStates_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::Clean
// Created: NLD 2005-01-07
// -----------------------------------------------------------------------------
void MIL_Automate::Clean()
{
    bDotationSupplyExplicitlyRequested_ = false;
    bAutomateModeChanged_               = false;
    for( CIT_SupplyDotationStateMap it = dotationSupplyStates_.begin(); it != dotationSupplyStates_.end(); ++it )
        it->second->Clean();
    pDecision_->Clean();
}

// =============================================================================
// LOGISTIC - SUPPLY
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Automate::NotifyDotationSupplyNeeded
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
void MIL_Automate::NotifyDotationSupplyNeeded( const PHY_DotationCategory& dotationCategory )
{
    if( bDotationSupplyNeeded_ )
        return;

    for( CIT_SupplyDotationStateMap it = dotationSupplyStates_.begin(); it != dotationSupplyStates_.end(); ++it )
    {
        if( it->second->IsSupplying( dotationCategory ) )
            return;
    }
    bDotationSupplyNeeded_ = true;

    // Pas de RC si log non branchée ou si RC envoyé au tick précédent
    const uint nCurrentTick = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep();
    if( GetTC2() && ( nCurrentTick > ( nTickRcDotationSupplyQuerySent_ + 1 ) || nTickRcDotationSupplyQuerySent_ == 0 ) )
        MIL_RC::pRcDemandeRavitaillementDotations_->Send( *this, MIL_RC::eRcTypeOperational ); // Rcs uniquement quand la log est branchée
    nTickRcDotationSupplyQuerySent_ = nCurrentTick;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::RequestDotationSupply
// Created: NLD 2005-03-03
// -----------------------------------------------------------------------------
void MIL_Automate::RequestDotationSupply()
{
    bDotationSupplyNeeded_              = true;
    bDotationSupplyExplicitlyRequested_ = true;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::NotifyDotationSupplied
// Created: NLD 2005-01-28
// -----------------------------------------------------------------------------
void MIL_Automate::NotifyDotationSupplied( const PHY_SupplyDotationState& supplyState )
{
    MIL_RC::pRcRavitaillementDotationsEffectue_->Send( *this, MIL_RC::eRcTypeOperational );
    for( IT_SupplyDotationStateMap it = dotationSupplyStates_.begin(); it != dotationSupplyStates_.end(); ++it )
    {
        if( it->second == &supplyState )
        {
            dotationSupplyStates_.erase( it );
            return;
        }
    }
    assert( false );
}

//-----------------------------------------------------------------------------
// Name: MIL_Automate::Debraye
// Created: NLD 2003-04-11
//-----------------------------------------------------------------------------
void MIL_Automate::Debraye()
{
    if( !bEmbraye_ )
        return;
    orderManager_.Debraye(); // Annule tout sauf les ordres des subordonnés
    bEmbraye_             = false;
    bAutomateModeChanged_ = true;
}

//-----------------------------------------------------------------------------
// Name: MIL_Automate::Embraye
// Created: NLD 2003-04-11
//-----------------------------------------------------------------------------
void MIL_Automate::Embraye()
{
    if( bEmbraye_ )
        return;

    bEmbraye_             = true;
    bAutomateModeChanged_ = true;
    orderManager_.Embraye(); // Annule tout (même les ordres des subordonnés)
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::IsPerceived
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
bool MIL_Automate::IsPerceived( const DEC_Knowledge_Agent&  knowledge ) const
{
    for( CIT_PionVector itPion = pions_.begin(); itPion != pions_.end(); ++itPion )
        if( (**itPion).GetRole< PHY_RolePion_Perceiver >().ComputePerception( knowledge ) != PHY_PerceptionLevel::notSeen_ )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::IsPerceived
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
bool MIL_Automate::IsPerceived( const DEC_Knowledge_Object& knowledge ) const
{
    for( CIT_PionVector itPion = pions_.begin(); itPion != pions_.end(); ++itPion )
        if( (**itPion).GetRole< PHY_RolePion_Perceiver >().ComputePerception( knowledge ) != PHY_PerceptionLevel::notSeen_ )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetArmy
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
const MIL_Army& MIL_Automate::GetArmy() const
{
    assert( pFormation_ );
    return pFormation_->GetArmy();
}

// =============================================================================
// REFUGEES
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Automate::OrientateRefugee
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
bool MIL_Automate::OrientateRefugee( const MIL_CampRefugies& camp )
{
    assert( pType_ );
    
    if ( !pType_->IsRefugee() || pRefugeeCamp_ )
        return false;

    pRefugeeCamp_ = &camp;

    Embraye();

    MIL_AutomateMission_ABC& mission = MIL_AutomateMissionType::GetMoveToRefugeeCampMissionType().InstanciateMission( *this );
    mission.Initialize();
    mission.Prepare   ();
    orderManager_.OnReceiveAutomateOrder( mission );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::NotifyRefugeeManagedStateChanged
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
void MIL_Automate::NotifyRefugeeManagedStateChanged( bool bManaged )
{
    assert( pType_ );
    
    if ( !pType_->IsRefugee() )
        return;

    if( bManaged )
    {
        for( CIT_PionVector itPion = pions_.begin(); itPion != pions_.end(); ++itPion )
            (**itPion).GetRole< PHY_RolePion_Refugee >().NotifyManaged();
    }
    else
    {
        for( CIT_PionVector itPion = pions_.begin(); itPion != pions_.end(); ++itPion )
            (**itPion).GetRole< PHY_RolePion_Refugee >().NotifyUnmanaged();
        pRefugeeCamp_ = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::NotifyInsideRefugeeCamp
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
void MIL_Automate::NotifyInsideRefugeeCamp( const MIL_CampRefugies& camp )
{
    assert( pType_ );
    
    if ( pType_->IsRefugee() )
        pTC2_ = &camp.GetTC2();
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::NotifyOutsideRefugeeCamp
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
void MIL_Automate::NotifyOutsideRefugeeCamp( const MIL_CampRefugies& /*camp*/ )
{
    assert( pType_ );
    
    if ( pType_->IsRefugee() )
        pTC2_ = 0;
}

// =============================================================================
// PRISONERS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Automate::Surrender
// Created: NLD 2005-02-24
// -----------------------------------------------------------------------------
void MIL_Automate::Surrender()
{
    if( bSurrendered_ )
        return;

    orderManager_.CancelAllOrders();
    pPrisonerCamp_ = 0;
    bSurrendered_  = true;
    pTC2_          = 0;
    pNominalTC2_   = 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::TakePrisoner
// Created: NLD 2005-03-04
// -----------------------------------------------------------------------------
bool MIL_Automate::TakePrisoner( const MIL_AgentPion& pionTakingPrisoner, const MIL_CampPrisonniers& camp )
{
    if( !bSurrendered_ )
        return false;
    if( bPrisoner_ )
        return true;

    assert( !pPrisonerCamp_ );
    assert( !pTC2_ && !pNominalTC2_ );

    pPrisonerCamp_ = &camp;
    bPrisoner_     = true;
    pTC2_          = pionTakingPrisoner.GetAutomate().GetTC2();
    for( CIT_PionVector itPion = pions_.begin(); itPion != pions_.end(); ++itPion )
        (**itPion).GetRole< PHY_RolePion_Surrender >().NotifyTakenPrisoner();

    Embraye();

    MIL_AutomateMission_ABC& mission = MIL_AutomateMissionType::GetSurrenderingMissionType().InstanciateMission( *this );
    mission.Initialize();
    mission.Prepare   ();
    orderManager_.OnReceiveAutomateOrder( mission );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::NotifyInsidePrisonerCamp
// Created: NLD 2005-03-07
// -----------------------------------------------------------------------------
void MIL_Automate::NotifyInsidePrisonerCamp( const MIL_CampPrisonniers& camp )
{
    if( bPrisoner_ )
        pTC2_ = &camp.GetTC2();
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::NotifyOutsidePrisonerCamp
// Created: NLD 2005-03-07
// -----------------------------------------------------------------------------
void MIL_Automate::NotifyOutsidePrisonerCamp( const MIL_CampPrisonniers& /*camp*/ )
{
    if( bPrisoner_ )
        pTC2_ = 0;
}

// =============================================================================
// MISC
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetAlivePionsBarycenter
// Created: NLD 2004-11-08
// -----------------------------------------------------------------------------
bool MIL_Automate::GetAlivePionsBarycenter( MT_Vector2D& barycenter ) const
{
    barycenter.Reset();
    uint nTmp = 0;
    for( CIT_PionVector itPion = pions_.begin(); itPion != pions_.end(); ++itPion )
    {
        if( !(**itPion).IsDead() )
        {
            barycenter += (**itPion).GetRole< PHY_RolePion_Location >().GetPosition();
            ++nTmp ;
        }
    }
    if( nTmp == 0 )
        return false;
    
    barycenter /= nTmp;
    return true;
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Automate::SendCreation
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MIL_Automate::SendCreation() const
{
    NET_ASN_MsgAutomateCreation asn;
    asn.GetAsnMsg().oid_automate            = nID_;
    asn.GetAsnMsg().type_automate           = pType_->GetID();
    asn.GetAsnMsg().oid_camp                = GetArmy().GetID();
    asn.GetAsnMsg().oid_groupe_connaissance = GetKnowledgeGroup().GetID();
    asn.GetAsnMsg().oid_formation           = pFormation_->GetID();
    asn.GetAsnMsg().nom                     = strName_.c_str();
    asn.Send();

    for( CIT_PionVector it = pions_.begin(); it != pions_.end(); ++it )
        (**it).SendCreation();
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::SendFullState
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void MIL_Automate::SendFullState() const
{
    assert( pDecision_ );

    NET_ASN_MsgAutomateAttributes asn;
    asn.GetAsnMsg().oid_automate = nID_;
    asn.GetAsnMsg().m.etat_automatePresent = 1;
    asn.GetAsnMsg().etat_automate = bEmbraye_ ? EnumAutomateState::embraye : EnumAutomateState::debraye;
    pDecision_->SendFullState( asn );
    asn.Send();

    SendLogisticLinks();

    for( CIT_SupplyDotationStateMap it = dotationSupplyStates_.begin(); it != dotationSupplyStates_.end(); ++it )
        it->second->SendFullState();

    for( CIT_PionVector it = pions_.begin(); it != pions_.end(); ++it )
        (**it).SendFullState();
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::SendKnowledge
// Created: NLD 2006-10-13
// -----------------------------------------------------------------------------
void MIL_Automate::SendKnowledge() const
{
    assert( pKnowledgeBlackBoard_ );
    pKnowledgeBlackBoard_->SendFullState();   

    for( CIT_PionVector it = pions_.begin(); it != pions_.end(); ++it )
        (**it).SendKnowledge();
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::SendLogisticLinks
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
void MIL_Automate::SendLogisticLinks() const
{
    NET_ASN_MsgChangeLiensLogistiques asn;

    asn.GetAsnMsg().oid_automate = nID_;

    if( pTC2_ )
    {
        asn.GetAsnMsg().m.oid_tc2Present = 1;
        asn.GetAsnMsg().oid_tc2          = pTC2_->GetID();
    }

    asn.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::OnReceiveMsgSetAutomateMode
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void MIL_Automate::OnReceiveMsgSetAutomateMode( ASN1T_MsgSetAutomateMode& asnMsg, MIL_MOSContextID nCtx )
{
    NET_ASN_MsgSetAutomateModeAck asnReplyMsg;
    asnReplyMsg.GetAsnMsg().unit_id    = asnMsg.unit_id;
    asnReplyMsg.GetAsnMsg().error_code = EnumSetAutomateModeErrorCode::no_error;

//    if( IsSurrendered() )
//    {
//        asnReplyMsg.GetAsnMsg().error_code = EnumSetAutomateModeErrorCode::error_unit_surrendered;
//        asnReplyMsg.Send( nCtx );
//        return;
//    }

    if( asnMsg.mode == EnumAutomateState::debraye )
    {
        if( !IsEmbraye() )
            asnReplyMsg.GetAsnMsg().error_code = EnumSetAutomateModeErrorCode::error_already_debraye;
        else
            Debraye();
    }
    else if( asnMsg.mode == EnumAutomateState::embraye )
    {
        if( IsEmbraye() )
            asnReplyMsg.GetAsnMsg().error_code = EnumSetAutomateModeErrorCode::error_already_embraye;
        else
            Embraye();
    }
    else
        assert( false );

    asnReplyMsg.Send( nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::OnReceiveMsgUnitMagicAction
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void MIL_Automate::OnReceiveMsgUnitMagicAction( ASN1T_MsgUnitMagicAction& asnMsg, MIL_MOSContextID nCtx )
{
    if( asnMsg.action.t == T_MsgUnitMagicAction_action_move_to )
    {
        MT_Vector2D vPosTmp;
        MIL_Tools::ConvertCoordMosToSim( *asnMsg.action.u.move_to, vPosTmp );

        NET_ASN_MsgUnitMagicActionAck asnReplyMsg;
        asnReplyMsg.GetAsnMsg().oid        = asnMsg.oid;

        const MT_Vector2D vTranslation( vPosTmp - pPionPC_->GetRole< PHY_RolePion_Location >().GetPosition() );
        for( CIT_PionVector itPion = pions_.begin(); itPion != pions_.end(); ++itPion )
            (**itPion).OnReceiveMsgMagicMove( (**itPion).GetRole< PHY_RolePion_Location >().GetPosition() + vTranslation );

        pDecision_->Reset();
        orderManager_.CancelAllOrders();

        asnReplyMsg.GetAsnMsg().error_code = EnumUnitAttrErrorCode::no_error;
        asnReplyMsg.Send( nCtx );
    }
    else if( asnMsg.action.t == T_MsgUnitMagicAction_action_se_rendre )
    {
        Surrender();
        for( CIT_PionVector itPion = pions_.begin(); itPion != pions_.end(); ++itPion )
            (**itPion).OnReceiveMagicSurrender(); 
        
        NET_ASN_MsgUnitMagicActionAck asnReplyMsg;
        asnReplyMsg.GetAsnMsg().oid        = asnMsg.oid;
            asnReplyMsg.GetAsnMsg().error_code = EnumUnitAttrErrorCode::no_error;
        asnReplyMsg.Send( nCtx );
    }
    else
        pPionPC_->OnReceiveMsgUnitMagicAction( asnMsg, nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::OnReceiveMsgChangeKnowledgeGroup
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void MIL_Automate::OnReceiveMsgChangeKnowledgeGroup( ASN1T_MsgChangeGroupeConnaissance& asnMsg, MIL_MOSContextID nCtx )
{
    NET_ASN_MsgChangeGroupeConnaissanceAck asnReplyMsg;
    asnReplyMsg.GetAsnMsg().oid_automate            = asnMsg.oid_automate;
    asnReplyMsg.GetAsnMsg().oid_camp                = asnMsg.oid_camp;
    asnReplyMsg.GetAsnMsg().oid_groupe_connaissance = asnMsg.oid_groupe_connaissance;
    
    MIL_Army* pNewArmy = MIL_AgentServer::GetWorkspace().GetEntityManager().FindArmy( asnMsg.oid_camp );
    if( !pNewArmy || *pNewArmy != GetArmy() )
    {
        asnReplyMsg.GetAsnMsg().error_code = EnumChangeGroupeConnaissanceErrorCode::error_invalid_camp;
        asnReplyMsg.Send( nCtx );
        return;
    }
    
    MIL_KnowledgeGroup* pNewKnowledgeGroup = pNewArmy->FindKnowledgeGroup( asnMsg.oid_groupe_connaissance );
    if( !pNewKnowledgeGroup )
    {
        asnReplyMsg.GetAsnMsg().error_code = EnumChangeGroupeConnaissanceErrorCode::error_invalid_groupe_connaissance;
        asnReplyMsg.Send( nCtx );
        return;
    }

    if( *pKnowledgeGroup_ != *pNewKnowledgeGroup )
    {
        pKnowledgeGroup_->UnregisterAutomate( *this );
        pKnowledgeGroup_ = pNewKnowledgeGroup;
        pKnowledgeGroup_->RegisterAutomate( *this );
    }

    asnReplyMsg.GetAsnMsg().error_code = EnumChangeGroupeConnaissanceErrorCode::no_error;
    asnReplyMsg.Send( nCtx );  
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::OnReceiveMsgChangeLogisticLinks
// Created: NLD 2005-01-17
// -----------------------------------------------------------------------------
void MIL_Automate::OnReceiveMsgChangeLogisticLinks( ASN1T_MsgChangeLiensLogistiques& msg, MIL_MOSContextID nCtx )
{
    NET_ASN_MsgChangeLiensLogistiquesAck asnReplyMsg;
    asnReplyMsg.GetAsnMsg().oid_automate = msg.oid_automate;

    if( IsSurrendered() )
    {
        asnReplyMsg.GetAsnMsg().error_code = EnumChangeLiensLogistiquesErrorCode::error_unit_surrendered;
        asnReplyMsg.Send( nCtx );
        return;
    }

    if( msg.m.oid_tc2Present )
    {
        asnReplyMsg.GetAsnMsg().m.oid_tc2Present = 1;
        asnReplyMsg.GetAsnMsg().oid_tc2 = msg.oid_tc2;

        if( msg.oid_tc2 == 0 )
            pTC2_ = 0;
        else
        {
            MIL_Automate* pTC2 = MIL_AgentServer::GetWorkspace().GetEntityManager().FindAutomate( msg.oid_tc2 );
            if( !pTC2 || !pTC2->GetType().IsLogistic() )
            {
                asnReplyMsg.GetAsnMsg().error_code = EnumChangeLiensLogistiquesErrorCode::error_invalid_automate_tc2;
                asnReplyMsg.Send( nCtx );
                return;
            }
            pTC2_ = static_cast< MIL_AutomateLOG* >( pTC2 );
        }
    }
    asnReplyMsg.GetAsnMsg().error_code = EnumChangeLiensLogistiquesErrorCode::no_error;
    asnReplyMsg.Send( nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::OnReceiveMsgLogSupplyChangeQuotas
// Created: NLD 2005-02-03
// -----------------------------------------------------------------------------
void MIL_Automate::OnReceiveMsgLogSupplyChangeQuotas( ASN1T_MsgLogRavitaillementChangeQuotas& /*msg*/, MIL_MOSContextID nCtx )
{
    NET_ASN_MsgLogRavitaillementChangeQuotasAck asnReplyMsg;
    asnReplyMsg.GetAsnMsg() = MsgLogRavitaillementChangeQuotasAck::error_invalid_receveur;
    asnReplyMsg.Send( nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::OnReceiveMsgLogSupplyPushFlow
// Created: NLD 2005-02-04
// -----------------------------------------------------------------------------
void MIL_Automate::OnReceiveMsgLogSupplyPushFlow( ASN1T_MsgLogRavitaillementPousserFlux& /*msg*/, MIL_MOSContextID nCtx )
{
    NET_ASN_MsgLogRavitaillementPousserFluxAck asnReplyMsg;
    asnReplyMsg.GetAsnMsg() = MsgLogRavitaillementPousserFluxAck::error_invalid_receveur;
    asnReplyMsg.Send( nCtx );
}
