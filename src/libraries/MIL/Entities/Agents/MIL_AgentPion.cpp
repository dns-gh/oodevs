// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/MIL_AgentPion.cpp $
// $Author: Nld $
// $Modtime: 2/05/05 18:40 $
// $Revision: 42 $
// $Workfile: MIL_AgentPion.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "MIL_AgentPion.h"

#include "Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Roles/Humans/PHY_RolePion_Humans.h"
#include "Roles/Dotations/PHY_RolePion_Dotations.h"
#include "Roles/Network/NET_RolePion_Dotations.h"
#include "Roles/Decision/DEC_RolePion_Decision.h"
#include "Roles/Perception/PHY_RolePion_Perceiver.h"    
#include "Roles/Posture/PHY_RolePion_Posture.h"
#include "Roles/Location/PHY_RolePion_Location.h"
#include "Roles/Reinforcement/PHY_RolePion_Reinforcement.h"
#include "Roles/NBC/PHY_RolePion_NBC.h"
#include "Roles/Communications/PHY_RolePion_Communications.h"
#include "Roles/HumanFactors/PHY_RolePion_HumanFactors.h"
#include "Roles/Transported/PHY_RolePion_Transported.h"
#include "Roles/Surrender/PHY_RolePion_Surrender.h"
#include "Roles/Refugee/PHY_RolePion_Refugee.h"
#include "Roles/Logistic/Maintenance/PHY_RolePion_Maintenance.h"
#include "Roles/Logistic/Medical/PHY_RolePion_Medical.h"
#include "Roles/Logistic/Supply/PHY_RolePion_Supply.h"

#include "Actions/PHY_Action_ABC.h"
#include "Actions/Loading/PHY_RoleAction_Loading.h"
#include "Actions/Objects/PHY_RoleAction_Objects.h"
#include "Actions/Moving/PHY_RoleAction_Moving.h"
#include "Actions/Flying/PHY_RoleAction_Flying.h"
#include "Actions/Firing/DirectFiring/PHY_RoleAction_DirectFiring.h"
#include "Actions/Firing/IndirectFiring/PHY_RoleAction_IndirectFiring.h"
#include "Actions/Transport/PHY_RoleAction_Transport.h"

#include "Entities/Agents/Units/PHY_UnitType.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Agents/Units/Dotations/PHY_AmmoDotationClass.h"
#include "Entities/Agents/Units/Humans/PHY_HumanRank.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Automates/MIL_AutomateType.h"
#include "Entities/RC/MIL_RC.h"
#include "Entities/RC/MIL_RC_TirSurCivil.h"
#include "Entities/RC/MIL_RC_TirSurCampAmi.h"
#include "Entities/RC/MIL_RC_TirSurCampNeutre.h"
#include "Entities/RC/MIL_RC_TireParCivil.h"
#include "Entities/RC/MIL_RC_TireParCampAmi.h"
#include "Entities/RC/MIL_RC_TireParCampNeutre.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/MIL_Army.h"

#include "Decision/DEC_ModelPion.h"

#include "Knowledge/DEC_KnowledgeBlackBoard.h"
#include "Knowledge/DEC_KS_NetworkUpdater.h"
#include "Knowledge/DEC_KS_ObjectInteraction.h"
#include "Knowledge/DEC_KS_Fire.h"
#include "Knowledge/DEC_KS_AgentQuerier.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/MIL_KnowledgeGroup.h"

#include "Network/NET_AgentServer.h"
#include "Network/NET_AS_MOSServer.h"
#include "Network/NET_AS_MOSServerMsgMgr.h"
#include "Network/NET_ASN_Messages.h"

#include "Hla/HLA_UpdateFunctor.h"

#include "Tools/MIL_Tools.h"

BOOST_CLASS_EXPORT_GUID( MIL_AgentPion, "MIL_AgentPion" )

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion constructor
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_AgentPion::MIL_AgentPion( const MIL_AgentTypePion& type, uint nID, MIL_InputArchive& archive )
    : MIL_Agent_ABC        ( nID )
    , pType_               ( &type )
    , bIsPC_               ( false )
    , strName_             ( pType_->GetName() )
    , pAutomate_           ( 0 )
    , pKnowledgeBlackBoard_(  new DEC_KnowledgeBlackBoard() )
    , pKsObjectInteraction_(  new DEC_KS_ObjectInteraction( *pKnowledgeBlackBoard_, *this ) )
    , pKsFire_             (  new DEC_KS_Fire             ( *pKnowledgeBlackBoard_, *this ) )
    , pKsQuerier_          (  new DEC_KS_AgentQuerier     ( *pKnowledgeBlackBoard_, *this ) )
    , pKsNetworkUpdater_   (  new DEC_KS_NetworkUpdater   ( *pKnowledgeBlackBoard_ ) )
    , orderManager_        ( *new MIL_PionOrderManager    ( *this ) )
{
    // Liens hiérarchiques
    archive.Section( "LiensHierarchiques" );

    uint nAutomateID;
    archive.ReadField( "Automate", nAutomateID );
    pAutomate_ = MIL_AgentServer::GetWorkspace().GetEntityManager().FindAutomate( nAutomateID );
    if( !pAutomate_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Unknwon automata id %d", nAutomateID ), archive.GetContext() );
    archive.EndSection(); //  LiensHierarchiques

    Initialize( archive );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion constructor
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_AgentPion::MIL_AgentPion( MIL_Automate& automate, MIL_InputArchive& archive )
    : MIL_Agent_ABC        ( automate.GetID() )
    , pType_               ( &automate.GetType().GetTypePionPC() )
    , bIsPC_               ( true )
    , strName_             ( pType_->GetName() )
    , pAutomate_           ( &automate )
    , pKnowledgeBlackBoard_(  new DEC_KnowledgeBlackBoard () )
    , pKsObjectInteraction_(  new DEC_KS_ObjectInteraction( *pKnowledgeBlackBoard_, *this ) )
    , pKsFire_             (  new DEC_KS_Fire             ( *pKnowledgeBlackBoard_, *this ) )
    , pKsQuerier_          (  new DEC_KS_AgentQuerier     ( *pKnowledgeBlackBoard_, *this ) )
    , pKsNetworkUpdater_   (  new DEC_KS_NetworkUpdater   ( *pKnowledgeBlackBoard_ ) )
    , orderManager_        ( *new MIL_PionOrderManager    ( *this ) )
{
    Initialize( archive );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion constructor
// Created: NLD 2005-02-08
// -----------------------------------------------------------------------------
MIL_AgentPion::MIL_AgentPion( const MIL_AgentTypePion& type, uint nID, MIL_Automate& automate, const MT_Vector2D& vPosition )
    : MIL_Agent_ABC        ( nID )
    , pType_               ( &type )
    , bIsPC_               ( false )
    , strName_             ( pType_->GetName() )
    , pAutomate_           ( &automate )
    , pKnowledgeBlackBoard_(  new DEC_KnowledgeBlackBoard () )
    , pKsObjectInteraction_(  new DEC_KS_ObjectInteraction( *pKnowledgeBlackBoard_, *this ) )
    , pKsFire_             (  new DEC_KS_Fire             ( *pKnowledgeBlackBoard_, *this ) )
    , pKsQuerier_          (  new DEC_KS_AgentQuerier     ( *pKnowledgeBlackBoard_, *this ) )
    , pKsNetworkUpdater_   (  new DEC_KS_NetworkUpdater   ( *pKnowledgeBlackBoard_ ) )
    , orderManager_        ( *new MIL_PionOrderManager    ( *this ) )
{
    Initialize( vPosition );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion constructor
// Created: JVT 2005-03-15
// -----------------------------------------------------------------------------
MIL_AgentPion::MIL_AgentPion()
    : MIL_Agent_ABC         ()
    , pType_                ( 0 )
    , bIsPC_                ()
    , pKnowledgeBlackBoard_ ( 0 )
    , pKsObjectInteraction_ ( 0 )
    , pKsFire_              ( 0 )
    , pKsQuerier_           ( 0 )
    , pKsNetworkUpdater_    ( 0 )
    , actions_              ()
    , orderManager_         ( *new MIL_PionOrderManager( *this ) )
{
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::load
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_AgentPion::load( MIL_CheckPointInArchive& file, const uint )
{
    uint nTypeID;
    
    file >> boost::serialization::base_object< MIL_Agent_ABC >( *this );

    file >> nTypeID;
    pType_ = MIL_AgentTypePion::FindPionType( nTypeID );
    assert( pType_ );
    
    file >> const_cast< bool& >( bIsPC_ )
         >> strName_
         >> pAutomate_
      // >> actions_ // actions non sauvegardées
         >> pKnowledgeBlackBoard_;
         
    pKsObjectInteraction_ = new DEC_KS_ObjectInteraction( *pKnowledgeBlackBoard_, *this );
    pKsFire_              = new DEC_KS_Fire             ( *pKnowledgeBlackBoard_, *this );
    pKsQuerier_           = new DEC_KS_AgentQuerier     ( *pKnowledgeBlackBoard_, *this );
    pKsNetworkUpdater_    = new DEC_KS_NetworkUpdater   ( *pKnowledgeBlackBoard_ );
    

    { NET_RolePion_Dotations        * pRole; file >> pRole; RegisterRole( pRole ); } 
    { PHY_RolePion_Reinforcement    * pRole; file >> pRole; RegisterRole( pRole ); } 
    { PHY_RolePion_Posture          * pRole; file >> pRole; RegisterRole( pRole ); } 
    { PHY_RolePion_Location         * pRole; file >> pRole; RegisterRole( pRole ); } 
    { PHY_RolePion_Dotations        * pRole; file >> pRole; RegisterRole( pRole ); } 
    { PHY_RolePion_Humans           * pRole; file >> pRole; RegisterRole( pRole ); } 
    { PHY_RolePion_Composantes      * pRole; file >> pRole; RegisterRole( pRole ); } 
    { PHY_RolePion_Perceiver        * pRole; file >> pRole; RegisterRole( pRole ); } 
    { PHY_RolePion_NBC              * pRole; file >> pRole; RegisterRole( pRole ); } 
    { PHY_RolePion_Communications   * pRole; file >> pRole; RegisterRole( pRole ); } 
    { PHY_RolePion_HumanFactors     * pRole; file >> pRole; RegisterRole( pRole ); } 
    { PHY_RolePion_Transported      * pRole; file >> pRole; RegisterRole( pRole ); } 
    { PHY_RolePion_Maintenance      * pRole; file >> pRole; RegisterRole( pRole ); } 
    { PHY_RolePion_Medical          * pRole; file >> pRole; RegisterRole( pRole ); } 
    { PHY_RolePion_Supply           * pRole; file >> pRole; RegisterRole( pRole ); } 
    { PHY_RolePion_Surrender        * pRole; file >> pRole; RegisterRole( pRole ); } 
    { PHY_RolePion_Refugee          * pRole; file >> pRole; RegisterRole( pRole ); } 
    { PHY_RoleAction_Loading        * pRole; file >> pRole; RegisterRole( pRole ); } 
    { PHY_RoleAction_Transport      * pRole; file >> pRole; RegisterRole( pRole ); } 
    { PHY_RoleAction_Moving         * pRole; file >> pRole; RegisterRole( pRole ); } 
    { PHY_RoleAction_Objects        * pRole; file >> pRole; RegisterRole( pRole ); } 
    { PHY_RoleAction_DirectFiring   * pRole; file >> pRole; RegisterRole( pRole ); } 
    { PHY_RoleAction_IndirectFiring * pRole; file >> pRole; RegisterRole( pRole ); } 
    { DEC_RolePion_Decision         * pRole; file >> pRole; RegisterRole( pRole ); } 
    { PHY_RoleAction_InterfaceFlying* pRole; file >> pRole; RegisterRole( pRole ); }
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::save
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_AgentPion::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    assert( pType_ );
    
    file << boost::serialization::base_object< MIL_Agent_ABC >( *this );
    file << pType_->GetID()
         << bIsPC_
         << strName_
         << pAutomate_
      // << actions_ // actions non sauvegardées
         << pKnowledgeBlackBoard_;
         
    file << &GetRole< NET_RolePion_Dotations         >();
    file << &GetRole< PHY_RolePion_Reinforcement     >();
    file << &GetRole< PHY_RolePion_Posture           >();
    file << &GetRole< PHY_RolePion_Location          >();
    file << &GetRole< PHY_RolePion_Dotations         >();
    file << &GetRole< PHY_RolePion_Humans            >();
    file << &GetRole< PHY_RolePion_Composantes       >();
    file << &GetRole< PHY_RolePion_Perceiver         >();
    file << &GetRole< PHY_RolePion_NBC               >();
    file << &GetRole< PHY_RolePion_Communications    >();
    file << &GetRole< PHY_RolePion_HumanFactors      >();
    file << &GetRole< PHY_RolePion_Transported       >();
    file << &GetRole< PHY_RolePion_Maintenance       >();
    file << &GetRole< PHY_RolePion_Medical           >();
    file << &GetRole< PHY_RolePion_Supply            >();
    file << &GetRole< PHY_RolePion_Surrender         >();
    file << &GetRole< PHY_RolePion_Refugee           >();
    file << &GetRole< PHY_RoleAction_Loading         >();
    file << &GetRole< PHY_RoleAction_Transport       >();
    file << &GetRole< PHY_RoleAction_Moving          >();
    file << &GetRole< PHY_RoleAction_Objects         >();
    file << &GetRole< PHY_RoleAction_DirectFiring    >();
    file << &GetRole< PHY_RoleAction_IndirectFiring  >();
    file << &GetRole< DEC_RolePion_Decision          >();
    file << &GetRole< PHY_RoleAction_InterfaceFlying >();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::Initialize
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
void MIL_AgentPion::Initialize( const MT_Vector2D& vPosition )
{
    assert( pKnowledgeBlackBoard_ );

    std::stringstream strTmp;
    strTmp << strName_ << " - [" << GetID() << "]";
    strName_ = strTmp.str();

    RegisterRole< NET_RolePion_Dotations         >( *this );
    RegisterRole< PHY_RolePion_Reinforcement     >( *this );
    RegisterRole< PHY_RolePion_Posture           >( *this );
    RegisterRole< PHY_RolePion_Location          >( *this );
    RegisterRole< PHY_RolePion_Dotations         >( *this );
    RegisterRole< PHY_RolePion_Humans            >( *this );
    RegisterRole< PHY_RolePion_Composantes       >( *this );
    RegisterRole< PHY_RolePion_Perceiver         >( *this, *pKnowledgeBlackBoard_ );
    RegisterRole< PHY_RolePion_NBC               >( *this );
    RegisterRole< PHY_RolePion_Communications    >( *this );
    RegisterRole< PHY_RolePion_HumanFactors      >();
    RegisterRole< PHY_RolePion_Transported       >();
    RegisterRole< PHY_RolePion_Maintenance       >();
    RegisterRole< PHY_RolePion_Medical           >();
    RegisterRole< PHY_RolePion_Supply            >();
    RegisterRole< PHY_RolePion_Surrender         >( *this );
    RegisterRole< PHY_RolePion_Refugee           >( *this );
    RegisterRole< PHY_RoleAction_Loading         >();
    RegisterRole< PHY_RoleAction_Transport       >( *this );
    RegisterRole< PHY_RoleAction_Moving          >( *this );
    RegisterRole< PHY_RoleAction_Objects         >( *this );
    RegisterRole< PHY_RoleAction_DirectFiring    >( *this );
    RegisterRole< PHY_RoleAction_IndirectFiring  >( *this );
    RegisterRole< DEC_RolePion_Decision          >( *this );

    if( CanFly() )
        RegisterRole< PHY_RoleAction_Flying >( *this );
    else
        RegisterRole< PHY_RoleAction_InterfaceFlying >();

    pAutomate_->RegisterPion( *this );

    GetRole< PHY_RolePion_Location >().Move( vPosition, MT_Vector2D( 0., 1. ), 0. );
    GetRole< PHY_RolePion_Location >().Fly ( 0. );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::Initialize
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
void MIL_AgentPion::Initialize( MIL_InputArchive& archive )
{
    // Position - $$$ DEGEU
    std::string strPosition;
    archive.ReadField( "Position", strPosition );
    MT_Vector2D vPosTmp;
    MIL_Tools::ConvertCoordMosToSim( strPosition, vPosTmp );

    archive.ReadField( "Nom", strName_, MIL_InputArchive::eNothing );
    Initialize( vPosTmp );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::ReadOverloading
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
void MIL_AgentPion::ReadOverloading( MIL_InputArchive& archive )
{
    // Dotations overloaded by ODB
    GetRole< PHY_RolePion_Composantes  >().ReadOverloading( archive ); // Composantes + humans
    GetRole< PHY_RolePion_Dotations    >().ReadOverloading( archive );
    GetRole< PHY_RolePion_HumanFactors >().ReadOverloading( archive ); 
    GetRole< PHY_RolePion_Supply       >().ReadOverloading( archive );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion destructor
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_AgentPion::~MIL_AgentPion()
{
    for( CIT_ActionSet itAction = actions_.begin(); itAction != actions_.end(); ++itAction )
        delete *itAction;
    actions_.clear();

    pAutomate_->UnregisterPion( *this );

    delete &orderManager_;
    delete pKsNetworkUpdater_;
    delete pKsQuerier_;
    delete pKsFire_;
    delete pKsObjectInteraction_;
    delete pKnowledgeBlackBoard_;
}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::IsDead
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
bool MIL_AgentPion::IsDead() const
{
    return     !GetRole< PHY_RolePion_Humans      >().IsUsable()
            || !GetRole< PHY_RolePion_Composantes >().IsUsable();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::IsNeutralized
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
bool MIL_AgentPion::IsNeutralized() const
{
    return GetRole< PHY_RolePion_Composantes >().IsNeutralized();
}

// =============================================================================
// UPDATES
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::UpdateKnowledges
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
void MIL_AgentPion::UpdateKnowledges()
{
    assert( pKnowledgeBlackBoard_ );
    pKnowledgeBlackBoard_->Update();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::CleanKnowledges
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
void MIL_AgentPion::CleanKnowledges()
{
    assert( pKnowledgeBlackBoard_ );
    pKnowledgeBlackBoard_->Clean();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::UpdateDecision
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
void MIL_AgentPion::UpdateDecision()
{
    GetRole< DEC_RolePion_Decision >().UpdateDecision();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::UpdateActions
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
void MIL_AgentPion::UpdateActions()
{
    for( CIT_ActionSet itAction = actions_.begin(); itAction != actions_.end(); ++itAction )
        (**itAction).Update();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::UpdatePhysicalState
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void MIL_AgentPion::UpdatePhysicalState()
{
    const bool bIsDead = IsDead();
    GetRole< PHY_RolePion_Dotations         >().Update( bIsDead );
    GetRole< PHY_RolePion_Humans            >().Update( bIsDead );
    GetRole< PHY_RolePion_Composantes       >().Update( bIsDead );
    GetRole< PHY_RolePion_Posture           >().Update( bIsDead );
    GetRole< PHY_RolePion_Reinforcement     >().Update( bIsDead );
    GetRole< PHY_RolePion_Location          >().Update( bIsDead );
    GetRole< PHY_RolePion_NBC               >().Update( bIsDead );
    GetRole< PHY_RolePion_Communications    >().Update( bIsDead );
    GetRole< PHY_RolePion_HumanFactors      >().Update( bIsDead );
    GetRole< PHY_RolePion_Transported       >().Update( bIsDead );
    GetRole< PHY_RolePion_Maintenance       >().Update( bIsDead );
    GetRole< PHY_RolePion_Medical           >().Update( bIsDead );
    GetRole< PHY_RolePion_Supply            >().Update( bIsDead );
    GetRole< PHY_RolePion_Surrender         >().Update( bIsDead );
    GetRole< PHY_RolePion_Refugee           >().Update( bIsDead );    
    GetRole< PHY_RolePion_Perceiver         >().Update( bIsDead ); // Doit être après PHY_RolePion_Composantes
    GetRole< PHY_RoleAction_Loading         >().Update( bIsDead );
    GetRole< PHY_RoleAction_Transport       >().Update( bIsDead );
    GetRole< PHY_RoleAction_Objects         >().Update( bIsDead );
    GetRole< PHY_RoleAction_Moving          >().Update( bIsDead );
    GetRole< PHY_RoleAction_InterfaceFlying >().Update( bIsDead );
    GetRole< PHY_RoleAction_DirectFiring    >().Update( bIsDead );
    GetRole< PHY_RoleAction_IndirectFiring  >().Update( bIsDead );    
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::UpdateState
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
void MIL_AgentPion::UpdateState()
{
    UpdatePhysicalState();

    orderManager_.Update( IsDead() );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::UpdateNetwork
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void MIL_AgentPion::UpdateNetwork()
{
    GetRole< NET_RolePion_Dotations >().SendChangedState();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::PreprocessRandomBreakdowns
// Created: NLD 2005-01-06
// -----------------------------------------------------------------------------
void MIL_AgentPion::PreprocessRandomBreakdowns( uint nEndDayTimeStep ) const
{
    GetRole< PHY_RolePion_Composantes >().PreprocessRandomBreakdowns( nEndDayTimeStep );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::Clean
// Created: AGE 2004-11-23
// -----------------------------------------------------------------------------
void MIL_AgentPion::Clean()
{
    GetRole< PHY_RolePion_Location          >().Clean();
    GetRole< PHY_RolePion_Perceiver         >().Clean();
    GetRole< PHY_RolePion_Dotations         >().Clean();
    GetRole< PHY_RolePion_Humans            >().Clean();
    GetRole< PHY_RolePion_Composantes       >().Clean();
    GetRole< PHY_RolePion_Posture           >().Clean();
    GetRole< PHY_RolePion_Reinforcement     >().Clean();
    GetRole< PHY_RolePion_NBC               >().Clean();
    GetRole< PHY_RolePion_Communications    >().Clean();
    GetRole< PHY_RolePion_HumanFactors      >().Clean();
    GetRole< PHY_RolePion_Surrender         >().Clean();
    GetRole< PHY_RolePion_Refugee           >().Clean();
    GetRole< PHY_RolePion_Transported       >().Clean();
    GetRole< PHY_RolePion_Maintenance       >().Clean();
    GetRole< PHY_RolePion_Medical           >().Clean();
    GetRole< PHY_RolePion_Supply            >().Clean();
    GetRole< PHY_RoleAction_Loading         >().Clean();
    GetRole< PHY_RoleAction_Transport       >().Clean();    
    GetRole< PHY_RoleAction_Objects         >().Clean();
    GetRole< PHY_RoleAction_Moving          >().Clean();
    GetRole< PHY_RoleAction_InterfaceFlying >().Clean();
    GetRole< PHY_RoleAction_DirectFiring    >().Clean();
    GetRole< PHY_RoleAction_IndirectFiring  >().Clean();
    GetRole< DEC_RolePion_Decision          >().Clean();
}

// =============================================================================
// ACCESSORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::CanFly
// Created: NLD 2005-08-08
// -----------------------------------------------------------------------------
bool MIL_AgentPion::CanFly() const
{
    assert( pType_ );
    return pType_->GetUnitType().CanFly();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::IsAutonomous
// Created: NLD 2005-08-08
// -----------------------------------------------------------------------------
bool MIL_AgentPion::IsAutonomous() const
{
    assert( pType_ );
    return pType_->GetUnitType().IsAutonomous();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::GetDecision
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
DEC_RolePion_Decision& MIL_AgentPion::GetDecision()
{
    return GetRole< DEC_RolePion_Decision >();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::GetDecision
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
const DEC_RolePion_Decision& MIL_AgentPion::GetDecision() const
{
    return GetRole< DEC_RolePion_Decision >();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::GetArmy
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
MIL_Army& MIL_AgentPion::GetArmy() const
{
    assert( pAutomate_ );
    return pAutomate_->GetArmy();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::GetKnowledgeGroup
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
MIL_KnowledgeGroup& MIL_AgentPion::GetKnowledgeGroup() const
{
    assert( pAutomate_ );
    return pAutomate_->GetKnowledgeGroup();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::GetEtatOps
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
MT_Float MIL_AgentPion::GetEtatOps() const
{
    return GetRole< PHY_RolePion_Composantes >().GetEtatOps();
}

// =============================================================================
// KNOWLEDGE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::BelongsTo
// Created: AGE 2004-11-23
// -----------------------------------------------------------------------------
bool MIL_AgentPion::BelongsTo( const MIL_KnowledgeGroup& group ) const
{
    return GetKnowledgeGroup() == group;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::CreateKnowledge
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
DEC_Knowledge_Agent& MIL_AgentPion::CreateKnowledge( const MIL_KnowledgeGroup& knowledgeGroup )
{
    return *new DEC_Knowledge_Agent( knowledgeGroup, *this );
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::SendCreation
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MIL_AgentPion::SendCreation()
{
    if( bIsPC_ )
        return;

    NET_ASN_MsgPionCreation asnMsg;
   
    assert( pType_ );
    asnMsg.GetAsnMsg().oid_pion     = GetID();
    asnMsg.GetAsnMsg().type_pion    = pType_->GetID();
    asnMsg.GetAsnMsg().nom          = strName_.c_str(); // !! pointeur sur const char*
    asnMsg.GetAsnMsg().oid_automate = GetAutomate().GetID();
    
    asnMsg.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::SendFullState
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void MIL_AgentPion::SendFullState()
{
    GetRole< NET_RolePion_Dotations >().SendFullState();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::SendKnowledge
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void MIL_AgentPion::SendKnowledge()
{
    assert( pKsNetworkUpdater_ );
    pKsNetworkUpdater_->SendFullState();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveMsgUnitMagicAction
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveMsgUnitMagicAction( DIN::DIN_Input& msg )
{
    uint8 nMagicAction;
    msg >> nMagicAction;
    if( nMagicAction == 0 /*eUnitMagicActionDestroyComposante*/ )
        GetRole< PHY_RolePion_Composantes >().DestroyRandomComposante();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::MagicMove
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void MIL_AgentPion::MagicMove( const MT_Vector2D& vNewPos )
{
    GetRole< PHY_RolePion_Location >().MagicMove( vNewPos );
    GetRole< DEC_RolePion_Decision >().Reset();
    orderManager_.CancelAllOrders();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveMsgMagicMove
// Created: NLD 2005-08-18
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveMsgMagicMove( const MT_Vector2D& vPosition )
{
    MagicMove( vPosition );
    UpdatePhysicalState();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveMsgMagicMove
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
ASN1T_EnumUnitAttrErrorCode MIL_AgentPion::OnReceiveMsgMagicMove( ASN1T_MagicActionMoveTo& asn )
{
    if( pAutomate_->IsEmbraye() )
        return EnumUnitAttrErrorCode::error_automate_embraye;

    MT_Vector2D vPosTmp;
    MIL_Tools::ConvertCoordMosToSim( asn, vPosTmp );

    MagicMove( vPosTmp );
    UpdatePhysicalState();

    return EnumUnitAttrErrorCode::no_error;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveMsgChangeHumanFactors
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
ASN1T_EnumUnitAttrErrorCode MIL_AgentPion::OnReceiveMsgChangeHumanFactors( ASN1T_MagicActionChangeFacteursHumains& asn )
{
    if( asn.m.experiencePresent )
    {
        const PHY_Experience* pExperience = PHY_Experience::Find( asn.experience );
        if( !pExperience )
            return EnumUnitAttrErrorCode::error_invalid_attribute;
        GetRole< PHY_RolePion_HumanFactors >().SetExperience( *pExperience );
    }

    if( asn.m.moralPresent )
    {
        const PHY_Morale* pMoral = PHY_Morale::Find( asn.moral );
        if( !pMoral )
            return EnumUnitAttrErrorCode::error_invalid_attribute;
        GetRole< PHY_RolePion_HumanFactors >().SetMorale( *pMoral );
    }

    if( asn.m.fatiguePresent )
    {
        const PHY_Tiredness* pTiredness = PHY_Tiredness::Find( asn.fatigue );
        if( !pTiredness )
            return EnumUnitAttrErrorCode::error_invalid_attribute;
        GetRole< PHY_RolePion_HumanFactors >().SetTiredness( *pTiredness );
    }

    return EnumUnitAttrErrorCode::no_error;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveMsgResupplyHumans
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
ASN1T_EnumUnitAttrErrorCode MIL_AgentPion::OnReceiveMsgResupplyHumans()
{
    GetRole< PHY_RolePion_Humans >().HealAllHumans();
    return EnumUnitAttrErrorCode::no_error;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveMsgResupplyResources
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
ASN1T_EnumUnitAttrErrorCode MIL_AgentPion::OnReceiveMsgResupplyResources()
{
    GetRole< PHY_RolePion_Dotations >().ResupplyDotations();
    GetRole< PHY_RolePion_Supply    >().ResupplyStocks   ();
    return EnumUnitAttrErrorCode::no_error;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveMsgResupplyEquipement
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
ASN1T_EnumUnitAttrErrorCode MIL_AgentPion::OnReceiveMsgResupplyEquipement()
{
    GetRole< PHY_RolePion_Composantes >().RepairAllComposantes();
    return EnumUnitAttrErrorCode::no_error;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveMsgResupplyAll
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
ASN1T_EnumUnitAttrErrorCode MIL_AgentPion::OnReceiveMsgResupplyAll()
{
    GetRole< PHY_RolePion_Composantes >().RepairAllComposantes();
    GetRole< PHY_RolePion_Dotations   >().ResupplyDotations   ();
    GetRole< PHY_RolePion_Supply      >().ResupplyStocks      ();
    GetRole< PHY_RolePion_Humans      >().HealAllHumans       ();
    GetRole< PHY_RolePion_NBC         >().Decontaminate       ();
    return EnumUnitAttrErrorCode::no_error;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveMsgResupply
// Created: NLD 2005-07-27
// -----------------------------------------------------------------------------
ASN1T_EnumUnitAttrErrorCode MIL_AgentPion::OnReceiveMsgResupply( ASN1T_MagicActionRecompletementPartiel& asn )
{
    if( asn.m.equipementsPresent )
    {
        PHY_RolePion_Composantes& roleComposantes = GetRole< PHY_RolePion_Composantes >();
        for( uint i = 0; i < asn.equipements.n; ++i )
        {
            const ASN1T_RecompletementEquipement& asnEquipement = asn.equipements.elem[ i ];
            const PHY_ComposanteTypePion* pComposanteType = PHY_ComposanteTypePion::FindComposanteType( asnEquipement.type_equipement );
            if( pComposanteType )
                roleComposantes.ChangeComposantesAvailability( *pComposanteType, asnEquipement.nombre_disponible );
        }
    }

    if( asn.m.personnelsPresent )
    {
        PHY_RolePion_Humans& roleHumans = GetRole< PHY_RolePion_Humans >();
        for( uint i = 0 ; i < asn.personnels.n; ++i )
        {
            const ASN1T_RecompletementPersonnel& asnPersonnel = asn.personnels.elem[ i ];
            const PHY_HumanRank* pHumanRank = PHY_HumanRank::Find( asnPersonnel.rang );
            if( pHumanRank )
                roleHumans.ChangeHumansAvailability( *pHumanRank, asnPersonnel.nombre_disponible ); 
        }
    }
    
    if( asn.m.dotationsPresent )
    {
        PHY_RolePion_Dotations& roleDotations = GetRole< PHY_RolePion_Dotations >();
        for( uint i = 0; i < asn.dotations.n; ++i )
        {
            const ASN1T_RecompletementDotation& asnDotation = asn.dotations.elem[ i ];
            const PHY_DotationType* pDotationType = PHY_DotationType::FindDotationType( asnDotation.famille_dotation ); 
            if( pDotationType )
                roleDotations.ResupplyDotations( *pDotationType, asnDotation.pourcentage / 100. );
        }
    }

    if( asn.m.munitionsPresent )
    {
        PHY_RolePion_Dotations& roleDotations = GetRole< PHY_RolePion_Dotations >();
        for( uint i = 0; i < asn.munitions.n; ++i )
        {
            const ASN1T_RecompletementDotationMunition& asnMunition = asn.munitions.elem[ i ];
            const PHY_AmmoDotationClass* pAmmoClass = PHY_AmmoDotationClass::Find( asnMunition.famille_munition ); 
            if( pAmmoClass )
                roleDotations.ResupplyDotations( *pAmmoClass, asnMunition.pourcentage / 100. );
        }
    }

    return EnumUnitAttrErrorCode::no_error;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveMsgDestroyAll
// Created: NLD 2004-12-07
// -----------------------------------------------------------------------------
ASN1T_EnumUnitAttrErrorCode MIL_AgentPion::OnReceiveMsgDestroyAll()
{
    GetRole< PHY_RolePion_Composantes >().DestroyAllComposantes();
    return EnumUnitAttrErrorCode::no_error;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveMsgUnitMagicAction
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveMsgUnitMagicAction( ASN1T_MsgUnitMagicAction& asnMsg, MIL_MOSContextID nCtx )
{
    NET_ASN_MsgUnitMagicActionAck asnReplyMsg;
    asnReplyMsg.GetAsnMsg().oid        = asnMsg.oid;

    switch( asnMsg.action.t )
    {
        case T_MsgUnitMagicAction_action_move_to                    : asnReplyMsg.GetAsnMsg().error_code = OnReceiveMsgMagicMove         ( *asnMsg.action.u.move_to ); break;
        case T_MsgUnitMagicAction_action_recompletement_personnel   : asnReplyMsg.GetAsnMsg().error_code = OnReceiveMsgResupplyHumans    (); break;
        case T_MsgUnitMagicAction_action_recompletement_ressources  : asnReplyMsg.GetAsnMsg().error_code = OnReceiveMsgResupplyResources (); break;
        case T_MsgUnitMagicAction_action_recompletement_equipement  : asnReplyMsg.GetAsnMsg().error_code = OnReceiveMsgResupplyEquipement(); break;
        case T_MsgUnitMagicAction_action_recompletement_total       : asnReplyMsg.GetAsnMsg().error_code = OnReceiveMsgResupplyAll       (); break;
        case T_MsgUnitMagicAction_action_recompletement_partiel     : asnReplyMsg.GetAsnMsg().error_code = OnReceiveMsgResupply          ( *asnMsg.action.u.recompletement_partiel ); break;
        case T_MsgUnitMagicAction_action_change_facteurs_humains    : asnReplyMsg.GetAsnMsg().error_code = OnReceiveMsgChangeHumanFactors( *asnMsg.action.u.change_facteurs_humains ); break;
        case T_MsgUnitMagicAction_action_destruction_totale         : asnReplyMsg.GetAsnMsg().error_code = OnReceiveMsgDestroyAll        (); break;
        case T_MsgUnitMagicAction_action_se_rendre                  : pAutomate_->OnReceiveMsgUnitMagicAction( asnMsg, nCtx ); return;
        default:
            assert( false );
    }
    UpdatePhysicalState();
    asnReplyMsg.Send( nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveMagicSurrender
// Created: NLD 2005-03-04
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveMagicSurrender()
{
    GetRole< PHY_RolePion_Surrender >().Surrender();
    orderManager_.CancelAllOrders();
    UpdatePhysicalState();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveMsgChangeAutomate
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveMsgChangeAutomate( ASN1T_MsgChangeAutomate& asnMsg, MIL_MOSContextID nCtx )
{
    NET_ASN_MsgChangeAutomateAck asnReplyMsg;
    asnReplyMsg.GetAsnMsg().oid_pion     = asnMsg.oid_pion;
    asnReplyMsg.GetAsnMsg().oid_automate = asnMsg.oid_automate;

    if( IsPC() )
    {
        asnReplyMsg.GetAsnMsg().error_code = EnumChangeAutomateErrorCode::error_invalid_pion;
        asnReplyMsg.Send( nCtx );
        return;
    }

    MIL_Automate* pNewAutomate = MIL_AgentServer::GetWorkspace().GetEntityManager().FindAutomate( asnMsg.oid_automate );
    if( !pNewAutomate )
    {
        asnReplyMsg.GetAsnMsg().error_code = EnumChangeAutomateErrorCode::error_invalid_automate;
        asnReplyMsg.Send( nCtx );
        return;
    }

    if( pNewAutomate->GetArmy() != GetArmy() )
    {
        asnReplyMsg.GetAsnMsg().error_code = EnumChangeAutomateErrorCode::error_camps_incompatibles;
        asnReplyMsg.Send( nCtx );
        return;
    }

    pAutomate_->UnregisterPion( *this );
    pAutomate_ = pNewAutomate;
    pAutomate_->RegisterPion  ( *this );

    asnReplyMsg.GetAsnMsg().error_code = EnumChangeAutomateErrorCode::no_error;
    asnReplyMsg.Send( nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::Serialize
// Created: AGE 2004-11-22
// -----------------------------------------------------------------------------
void MIL_AgentPion::Serialize( HLA_UpdateFunctor& functor ) const
{
    functor.Serialize( "armee", false, GetArmy().GetName() );
    functor.Serialize( "type", false, GetType().GetName() );
    const bool bUpdateStatuses = GetRole< PHY_RolePion_Composantes    >().HasChanged()
                              || GetRole< PHY_RolePion_Communications >().HasChanged()
                              || GetRole< PHY_RolePion_NBC            >().HasChanged()
                              || GetRole< PHY_RolePion_Posture        >().HLAStatusHasChanged()
                              || GetRole< PHY_RolePion_Transported    >().HasChanged()
                              || GetRole< PHY_RolePion_Surrender      >().HasChanged()
                              || GetRole< PHY_RolePion_Perceiver      >().HasRadarStateChanged();

    // $$$$ AGE 2004-12-13: Test functor.MustUpdateAll() if performance issues (doubt it)
    std::vector< std::string > statuses;
    if( IsPC() )
        statuses.push_back( "pc" );
    if( GetRole< PHY_RolePion_Composantes >().IsNeutralized() )
        statuses.push_back( "neutralise" );
    if( ! GetRole< PHY_RolePion_Communications >().CanCommunicate() )
        statuses.push_back( "brouille" );
    if( GetRole< PHY_RolePion_NBC >().IsContaminated() )
        statuses.push_back( "contamine" );
    if( GetRole< PHY_RolePion_Posture >().IsStealth() )
        statuses.push_back( "furtif" );
    if( GetRole< PHY_RolePion_Transported >().IsTransported() )
        statuses.push_back( "transporte" );
    if( GetRole< PHY_RolePion_Perceiver >().IsUsingActiveRadar() )
        statuses.push_back( "radaractif" );
    if( GetRole< PHY_RolePion_Surrender >().IsPrisoner() )
        statuses.push_back( "prisonnier" );
    if( GetRole< PHY_RolePion_Surrender >().IsSurrendered() )
        statuses.push_back( "rendu" );
    if( GetRole< PHY_RolePion_Refugee >().IsManaged() )
        statuses.push_back( "refugie_prisencompte" );
    functor.Serialize( "status", bUpdateStatuses, statuses );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::ChangeAutomate
// Created: NLD 2004-10-26
// -----------------------------------------------------------------------------
void MIL_AgentPion::ChangeAutomate( MIL_Automate& newAutomate )
{
    assert( GetArmy() == newAutomate.GetArmy() );
    pAutomate_->UnregisterPion( *this );
    pAutomate_ = &newAutomate;
    pAutomate_->RegisterPion  ( *this );

    GetRole< DEC_RolePion_Decision >().NotifyAutomateChanged(); //$$$ à gicler quand myself.automate_ sera remplacé par une fonction DEC

    NET_ASN_MsgChangeAutomate asnMsg;
    asnMsg.GetAsnMsg().oid_pion     = GetID();
    asnMsg.GetAsnMsg().oid_automate = newAutomate.GetID();
    asnMsg.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::Surrender
// Created: NLD 2005-02-24
// -----------------------------------------------------------------------------
void MIL_AgentPion::Surrender()
{
    orderManager_.CancelAllOrders();
    GetRole< PHY_RolePion_Surrender >().Surrender();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::NotifyAttackedBy
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
void MIL_AgentPion::NotifyAttackedBy( MIL_AgentPion& attacker )
{
    if( attacker.GetType().IsRefugee() )
        MIL_RC::pRcTireParCivil_->Send( *this, MIL_RC::eRcTypeEvent, attacker );
    else if( GetArmy().IsNeutral( attacker.GetArmy() ) == eTristate_True )
        MIL_RC::pRcTireParCampNeutre_->Send( *this, MIL_RC::eRcTypeEvent, attacker );
    else if( GetArmy().IsAFriend( attacker.GetArmy() ) == eTristate_True )
        MIL_RC::pRcTireParCampAmi_->Send( *this, MIL_RC::eRcTypeEvent, attacker );

    pKsFire_->NotifyAttackedBy( attacker );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::NotifyAttacking
// Created: NLD 2005-03-30
// -----------------------------------------------------------------------------
void MIL_AgentPion::NotifyAttacking( MIL_Agent_ABC& target ) const
{
    if( target.GetType().IsRefugee() )
        MIL_RC::pRcTirSurCivil_->Send( *this, MIL_RC::eRcTypeEvent, target );
    else if( GetArmy().IsNeutral( target.GetArmy() ) == eTristate_True )
        MIL_RC::pRcTirSurCampNeutre_->Send( *this, MIL_RC::eRcTypeEvent, target );
    else if( GetArmy().IsAFriend( target.GetArmy() ) == eTristate_True )
        MIL_RC::pRcTirSurCampAmi_->Send( *this, MIL_RC::eRcTypeEvent, target );
}            


// =============================================================================
// $$$ DEGUEU - VOIR AVEC LES MODELISATEURS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::IsPerceiving
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
bool MIL_AgentPion::IsAgentPerceived( const MIL_Agent_ABC& agent ) const
{
    assert( pKsQuerier_ );
    return pKsQuerier_->IsAgentPerceived( agent );    
}

