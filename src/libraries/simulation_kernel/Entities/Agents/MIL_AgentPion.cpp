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

#include "simulation_kernel_pch.h"

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
#include "Roles/Population/PHY_RolePion_Population.h"
#include "Roles/Logistic/Supply/PHY_RoleInterface_Supply.h"
#include "Roles/Logistic/Maintenance/PHY_RoleInterface_Maintenance.h"
#include "Roles/Logistic/Medical/PHY_RoleInterface_Medical.h"

#include "Actions/Loading/PHY_RoleAction_Loading.h"
#include "Actions/Objects/PHY_RoleAction_Objects.h"
#include "Actions/Moving/PHY_RoleAction_Moving.h"
#include "Actions/Flying/PHY_RoleAction_Flying.h"
#include "Actions/Firing/DirectFiring/PHY_RoleAction_DirectFiring.h"
#include "Actions/Firing/IndirectFiring/PHY_RoleAction_IndirectFiring.h"
#include "Actions/Transport/PHY_RoleAction_Transport.h"
#include "Actions/Emergency/PHY_RoleAction_FolkInfluence.h"

#include "Entities/Orders/MIL_Report.h"
#include "Entities/Agents/Units/PHY_UnitType.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Agents/Units/Dotations/PHY_AmmoDotationClass.h"
#include "Entities/Agents/Units/Humans/PHY_HumanRank.h"
#include "Entities/Agents/Units/HumanFactors/PHY_Morale.h"
#include "Entities/Agents/Units/HumanFactors/PHY_Experience.h"
#include "Entities/Agents/Units/HumanFactors/PHY_Tiredness.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/MIL_Army.h"
#include "Decision/DEC_Model_ABC.h"
#include "Decision/DEC_Representations.h"

#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Knowledge/DEC_KS_Fire.h"

#include "Network/NET_AgentServer.h"
#include "Network/NET_Publisher_ABC.h"
#include "Network/NET_ASN_Messages.h"
#include "Network/NET_AsnException.h"

#include "Hla/HLA_UpdateFunctor.h"

#include "Tools/MIL_Tools.h"
#include "Tools/MIL_IDManager.h"

#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_GUID( MIL_AgentPion, "MIL_AgentPion" )

namespace
{
    template< typename R >
    void SaveRole( const MIL_AgentPion& pion, MIL_CheckPointOutArchive& file )
    {
        const R* const role = & pion.GetRole< R >();
        file << role;
    }
}

template< typename Archive >
void save_construct_data( Archive& archive, const MIL_AgentPion* pion, const unsigned int /*version*/ )
{
	unsigned int nTypeID = pion->GetType().GetID();
    unsigned int nID = pion->GetID();
	archive << nTypeID
			<< nID
			<< pion->pAutomate_;
}

template< typename Archive >
void load_construct_data( Archive& archive, MIL_AgentPion* pion, const unsigned int /*version*/ )
{
	unsigned int nTypeID, nID;
	MIL_Automate* pAutomate = 0;
    archive >> nTypeID >> nID >> pAutomate;
    const MIL_AgentTypePion* pType = MIL_AgentTypePion::Find( nTypeID );
    assert( pType );
    ::new( pion )MIL_AgentPion( *pType, nID, *pAutomate );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion constructor
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_AgentPion::MIL_AgentPion( const MIL_AgentTypePion& type, uint nID, MIL_Automate& automate, xml::xistream& xis )
    : MIL_Agent_ABC            ( type.GetName(), xis, nID )
    , pType_                   ( &type )
    , bIsPC_                   ( xml::attribute< bool >( xis, "command-post", false ) )
    , pAutomate_               ( &automate )
    , pKnowledgeBlackBoard_    (  new DEC_KnowledgeBlackBoard_AgentPion( *this ) )
    , orderManager_            ( *new MIL_PionOrderManager( *this ) )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion constructor
// Created: NLD 2005-02-08
// -----------------------------------------------------------------------------
MIL_AgentPion::MIL_AgentPion( const MIL_AgentTypePion& type, uint nID, MIL_Automate& automate )
    : MIL_Agent_ABC            ( type.GetName(), nID )
    , pType_                   ( &type )
    , bIsPC_                   ( false )
    , pAutomate_               ( &automate )
    , pKnowledgeBlackBoard_    (  new DEC_KnowledgeBlackBoard_AgentPion( *this ) )
    , orderManager_            ( *new MIL_PionOrderManager( *this ) )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion constructor
// Created: LDC 2009-04-23
// -----------------------------------------------------------------------------
MIL_AgentPion::MIL_AgentPion( const MIL_AgentTypePion& type, MIL_Automate& automate )
    : MIL_Agent_ABC        ( "", 0)
    , pType_               ( &type )
    , bIsPC_               ()
    , pAutomate_           ( &automate )
    , pKnowledgeBlackBoard_( 0 )
    , orderManager_        ( *new MIL_PionOrderManager( *this ) )
{
    automate.RegisterPion( *this );
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
    file >> boost::serialization::base_object< MIL_Agent_ABC >( *this );
    
    file >> const_cast< bool& >( bIsPC_ )
         >> pAutomate_
      // >> actions_ // actions non sauvegardées
         >> pKnowledgeBlackBoard_;
           
    { NET_RolePion_Dotations        * pRole; file >> pRole; RegisterRole( pRole ); } 
    { PHY_RolePion_Reinforcement    * pRole; file >> pRole; RegisterRole( pRole ); } 
    { PHY_RolePion_Posture          * pRole; file >> pRole; RegisterRole( pRole ); } 
    { PHY_RolePion_Location         * pRole; file >> pRole; RegisterRole( pRole ); } 
    { dotation::PHY_RolePion_Dotations        * pRole; file >> pRole; RegisterRole( pRole ); } 
    { human::PHY_RolePion_Humans           * pRole; file >> pRole; RegisterRole( pRole ); } 
    { PHY_RolePion_Composantes      * pRole; file >> pRole; RegisterRole( pRole ); } 
    { PHY_RolePion_Perceiver        * pRole; file >> pRole; RegisterRole( pRole ); } 
    { nbc::PHY_RolePion_NBC         * pRole; file >> pRole; RegisterRole( pRole ); }
    { PHY_RolePion_Communications   * pRole; file >> pRole; RegisterRole( pRole ); } 
    { PHY_RolePion_HumanFactors     * pRole; file >> pRole; RegisterRole( pRole ); } 
    { transport::PHY_RolePion_Transported      * pRole; file >> pRole; RegisterRole( pRole ); }
    //{ PHY_RoleInterface_Maintenance * pRole; file >> pRole; RegisterRole( pRole ); } //@TODO refactor with new save
    //{ PHY_RoleInterface_Medical     * pRole; file >> pRole; RegisterRole( pRole ); } 
    //{ PHY_RoleInterface_Supply      * pRole; file >> pRole; RegisterRole( pRole ); } 
    { PHY_RolePion_Surrender        * pRole; file >> pRole; RegisterRole( pRole ); } 
    { PHY_RolePion_Refugee          * pRole; file >> pRole; RegisterRole( pRole ); }
    { PHY_RolePion_Population       * pRole; file >> pRole; RegisterRole( pRole ); }
    { transport::PHY_RoleAction_Loading        * pRole; file >> pRole; RegisterRole( pRole ); }
    { transport::PHY_RoleAction_Transport      * pRole; file >> pRole; RegisterRole( pRole ); }
    { PHY_RoleAction_Moving         * pRole; file >> pRole; RegisterRole( pRole ); } 
    { PHY_RoleAction_Objects        * pRole; file >> pRole; RegisterRole( pRole ); } 
    { firing::PHY_RoleAction_DirectFiring   * pRole; file >> pRole; RegisterRole( pRole ); } 
    { firing::PHY_RoleAction_IndirectFiring * pRole; file >> pRole; RegisterRole( pRole ); } 
    { DEC_RolePion_Decision         * pRole; file >> pRole; RegisterRole( pRole ); } 
    { PHY_RoleAction_InterfaceFlying* pRole; file >> pRole; RegisterRole( pRole ); }
    { PHY_RoleAction_FolkInfluence  * pRole; file >> pRole; RegisterRole( pRole ); }
    RegisterRole( new DEC_Representations() );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::save
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_AgentPion::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    assert( pType_ );

    file << boost::serialization::base_object< MIL_Agent_ABC >( *this );
    file << bIsPC_
        << pAutomate_
        // << actions_ // actions non sauvegardées
        << pKnowledgeBlackBoard_;

    SaveRole< NET_RolePion_Dotations         >( *this, file );
    SaveRole< PHY_RolePion_Reinforcement     >( *this, file );
    SaveRole< PHY_RolePion_Posture           >( *this, file );
    SaveRole< PHY_RolePion_Location          >( *this, file );
    SaveRole< dotation::PHY_RolePion_Dotations         >( *this, file );
    SaveRole< human::PHY_RolePion_Humans            >( *this, file );
    SaveRole< PHY_RolePion_Composantes       >( *this, file );
    SaveRole< PHY_RolePion_Perceiver         >( *this, file );
    SaveRole< nbc::PHY_RolePion_NBC          >( *this, file );
    SaveRole< PHY_RolePion_Communications    >( *this, file );
    SaveRole< PHY_RolePion_HumanFactors      >( *this, file );
    SaveRole< transport::PHY_RolePion_Transported       >( *this, file );
    //SaveRole< PHY_RolePion_Maintenance       >( *this, file );//@TODO refactor with new save
    //SaveRole< PHY_RolePion_Medical           >( *this, file );
    //SaveRole< PHY_RolePion_Supply            >( *this, file );
    SaveRole< PHY_RolePion_Surrender         >( *this, file );
    SaveRole< PHY_RolePion_Refugee           >( *this, file );
    SaveRole< PHY_RolePion_Population        >( *this, file );
    SaveRole< transport::PHY_RoleAction_Loading         >( *this, file );
    SaveRole< transport::PHY_RoleAction_Transport       >( *this, file );
    SaveRole< PHY_RoleAction_Moving          >( *this, file );
    SaveRole< PHY_RoleAction_Objects         >( *this, file );
    SaveRole< firing::PHY_RoleAction_DirectFiring    >( *this, file );
    SaveRole< firing::PHY_RoleAction_IndirectFiring  >( *this, file );
    SaveRole< DEC_RolePion_Decision          >( *this, file );
    SaveRole< PHY_RoleAction_InterfaceFlying >( *this, file );
    SaveRole< PHY_RoleAction_FolkInfluence   >( *this, file );    
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::WriteODB
// Created: NLD 2006-05-29
// -----------------------------------------------------------------------------
void MIL_AgentPion::WriteODB( xml::xostream& xos ) const
{
    assert( pType_ );

    xos << xml::start( "unit" ); 
       
    MIL_Entity_ABC::WriteODB( xos ) ; 

    xos     << xml::attribute( "id", GetID() )
            << xml::attribute( "type", pType_->GetName() )
            << xml::attribute( "command-post", bIsPC_ )
            << xml::attribute( "position", MIL_Tools::ConvertCoordSimToMos( GetRole< PHY_RolePion_Location >().GetPosition() ) );

    GetRole< PHY_RolePion_Composantes >().WriteODB( xos ); // Equipements
    GetRole< human::PHY_RolePion_Humans      >().WriteODB( xos ); // Personnels
    GetRole< dotation::PHY_RolePion_Dotations   >().WriteODB( xos ); // Dotations
    
    const PHY_RoleInterface_Supply* role = RetrieveRole< PHY_RoleInterface_Supply >();//@TODO verify
    if( role )
        role->WriteODB( xos ); // Stocks
       
    xos << xml::end();// unit
}
// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::ReadOverloading
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
void MIL_AgentPion::ReadOverloading( xml::xistream& xis )
{
    // Dotations overloaded by ODB
    GetRole< PHY_RolePion_Composantes  >().ReadOverloading( xis ); // Composantes + humans
    GetRole< dotation::PHY_RolePion_Dotations    >().ReadOverloading( xis );
    GetRole< PHY_RolePion_HumanFactors >().ReadOverloading( xis ); 
    
    PHY_RoleInterface_Supply* role = RetrieveRole< PHY_RoleInterface_Supply >();
    if( role )
        role->ReadOverloading( xis ); // Stocks
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion destructor
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_AgentPion::~MIL_AgentPion()
{
    pAutomate_->UnregisterPion( *this );

    delete &orderManager_;
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
    return !GetRole< PHY_RolePion_Composantes >().IsUsable();
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
void MIL_AgentPion::UpdateDecision( float duration )
{
    if( IsDead() )
        orderManager_.ReplaceMission( 0 );
    orderManager_.Update();
    GetRole< DEC_Decision_ABC >().UpdateDecision( duration );
    GetRole< DEC_Decision_ABC >().GarbageCollect();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::UpdatePhysicalState
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void MIL_AgentPion::UpdatePhysicalState()
{
    const bool bIsDead = IsDead();
    GetRole< dotation::PHY_RolePion_Dotations         >().Update( bIsDead );
    GetRole< human::PHY_RolePion_Humans            >().Update( bIsDead );
    GetRole< PHY_RolePion_Composantes       >().Update( bIsDead );
    GetRole< PHY_RolePion_Posture           >().Update( bIsDead );
    GetRole< PHY_RolePion_Reinforcement     >().Update( bIsDead );
    GetRole< PHY_RolePion_Location          >().Update( bIsDead );
    GetRole< nbc::PHY_RolePion_NBC          >().Update( bIsDead );
    GetRole< PHY_RolePion_Communications    >().Update( bIsDead );
    GetRole< PHY_RolePion_HumanFactors      >().Update( bIsDead );
    GetRole< transport::PHY_RolePion_Transported       >().Update( bIsDead );
    GetRole< PHY_RolePion_Surrender         >().Update( bIsDead );
    GetRole< PHY_RolePion_Refugee           >().Update( bIsDead );    
    GetRole< PHY_RolePion_Population        >().Update( bIsDead );    
    GetRole< PHY_RolePion_Perceiver         >().Update( bIsDead ); // Doit être après PHY_RolePion_Composantes $$$ pourri - utiliser des observers
    GetRole< transport::PHY_RoleAction_Loading         >().Update( bIsDead );
    GetRole< transport::PHY_RoleAction_Transport       >().Update( bIsDead );
    GetRole< PHY_RoleAction_Objects         >().Update( bIsDead );
    GetRole< PHY_RoleAction_Moving          >().Update( bIsDead );
    GetRole< PHY_RoleAction_InterfaceFlying >().Update( bIsDead );
    GetRole< firing::PHY_RoleAction_DirectFiring    >().Update( bIsDead );
    GetRole< firing::PHY_RoleAction_IndirectFiring  >().Update( bIsDead );
    GetRole< PHY_RoleAction_FolkInfluence   >().Update( bIsDead );

    PHY_RoleInterface_Maintenance* role = RetrieveRole< PHY_RoleInterface_Maintenance >();//@TODO add update to new role interface
    if( role )
        role->Update( bIsDead );
    PHY_RoleInterface_Medical* role2 = RetrieveRole< PHY_RoleInterface_Medical >();
    if( role2 )
        role2->Update( bIsDead );
    PHY_RoleInterface_Supply* role3 = RetrieveRole< PHY_RoleInterface_Supply >();
    if( role3 )
        role3->Update( bIsDead );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::UpdateState
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
void MIL_AgentPion::UpdateState()
{
    UpdatePhysicalState();
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
    GetRole< dotation::PHY_RolePion_Dotations         >().Clean();
    GetRole< human::PHY_RolePion_Humans            >().Clean();
    GetRole< PHY_RolePion_Composantes       >().Clean();
    GetRole< PHY_RolePion_Posture           >().Clean();
    GetRole< PHY_RolePion_Reinforcement     >().Clean();
    GetRole< nbc::PHY_RolePion_NBC          >().Clean();
    GetRole< PHY_RolePion_Communications    >().Clean();
    GetRole< PHY_RolePion_HumanFactors      >().Clean();
    GetRole< PHY_RoleInterface_Surrender    >().Clean();
    GetRole< PHY_RolePion_Refugee           >().Clean();
    GetRole< PHY_RolePion_Population        >().Clean();
    GetRole< transport::PHY_RoleInterface_Transported  >().Clean();
    GetRole< transport::PHY_RoleAction_Transport       >().Clean();
    GetRole< PHY_RoleAction_Objects         >().Clean();
    GetRole< PHY_RoleAction_Moving          >().Clean();
    GetRole< PHY_RoleAction_InterfaceFlying >().Clean();
    GetRole< firing::PHY_RoleAction_DirectFiring    >().Clean();
    GetRole< firing::PHY_RoleAction_IndirectFiring  >().Clean();
    GetRole< DEC_RolePion_Decision          >().Clean();
    GetRole< PHY_RoleAction_FolkInfluence   >().Clean();

    PHY_RoleInterface_Maintenance* role = RetrieveRole< PHY_RoleInterface_Maintenance >();//@TODO Add an interface for role with clean, update
    if( role )
        role->Clean();
    PHY_RoleInterface_Medical* role2 = RetrieveRole< PHY_RoleInterface_Medical >();
    if( role2 )
        role2->Clean();
    PHY_RoleInterface_Supply* role3 = RetrieveRole< PHY_RoleInterface_Supply >();
    if( role3 )
        role3->Clean();
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
DEC_Decision_ABC& MIL_AgentPion::GetDecision()
{
    return GetRole< DEC_RolePion_Decision >();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::GetDecision
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
const DEC_Decision_ABC& MIL_AgentPion::GetDecision() const
{
    return GetRole< DEC_RolePion_Decision >();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::GetArmy
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
MIL_Army_ABC& MIL_AgentPion::GetArmy() const
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
void MIL_AgentPion::SendCreation() const
{
    assert( pType_ );

    NET_ASN_MsgUnitCreation asnMsg;
    asnMsg().oid          = GetID();
    asnMsg().type_pion    = pType_->GetID();
    asnMsg().nom          = GetName().c_str(); // !! pointeur sur const char*
    asnMsg().oid_automate = GetAutomate().GetID();    
    asnMsg().pc           = IsPC();
    asnMsg.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::SendFullState
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void MIL_AgentPion::SendFullState() const
{
    GetRole< NET_RolePion_Dotations >().SendFullState();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::SendKnowledge
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void MIL_AgentPion::SendKnowledge() const
{
    assert( pKnowledgeBlackBoard_ );
    pKnowledgeBlackBoard_->SendFullState();
}


// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveMsgOrder
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveMsgOrder( const ASN1T_MsgUnitOrder& msg )
{
    orderManager_.OnReceiveMission( msg );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveMsgFragOrder
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------

void MIL_AgentPion::OnReceiveMsgFragOrder( const ASN1T_MsgFragOrder& msg )
{
    orderManager_.OnReceiveFragOrder( msg );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveMsgDestroyComponent
// Created: AGE 2007-06-19
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveMsgDestroyComponent()
{
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
    orderManager_.ReplaceMission();
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
void MIL_AgentPion::OnReceiveMsgMagicMove( const ASN1T_MagicActionMoveTo& asn )
{
    if( pAutomate_->IsEngaged() )
        throw NET_AsnException< ASN1T_EnumUnitErrorCode >( EnumUnitErrorCode::error_automate_embraye );

    MT_Vector2D vPosTmp;
    MIL_Tools::ConvertCoordMosToSim( asn, vPosTmp );

    MagicMove( vPosTmp );
    UpdatePhysicalState();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveMsgChangeHumanFactors
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
void  MIL_AgentPion::OnReceiveMsgChangeHumanFactors( const ASN1T_MagicActionChangeHumanFactors& asn )
{
    if( asn.m.experiencePresent )
    {
        const PHY_Experience* pExperience = PHY_Experience::Find( asn.experience );
        if( !pExperience )
            throw NET_AsnException< ASN1T_EnumUnitErrorCode >( EnumUnitErrorCode::error_invalid_attribute );
        GetRole< PHY_RolePion_HumanFactors >().SetExperience( *pExperience );
    }

    if( asn.m.moralPresent )
    {
        const PHY_Morale* pMoral = PHY_Morale::Find( asn.moral );
        if( !pMoral )
            throw NET_AsnException< ASN1T_EnumUnitErrorCode >( EnumUnitErrorCode::error_invalid_attribute );
        GetRole< PHY_RolePion_HumanFactors >().SetMorale( *pMoral );
    }

    if( asn.m.fatiguePresent )
    {
        const PHY_Tiredness* pTiredness = PHY_Tiredness::Find( asn.fatigue );
        if( !pTiredness )
            throw NET_AsnException< ASN1T_EnumUnitErrorCode >( EnumUnitErrorCode::error_invalid_attribute );
        GetRole< PHY_RolePion_HumanFactors >().SetTiredness( *pTiredness );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveMsgResupplyHumans
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveMsgResupplyHumans()
{
    GetRole< human::PHY_RolePion_Humans >().HealAllHumans();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveMsgResupplyResources
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveMsgResupplyResources()
{
    GetRole< dotation::PHY_RolePion_Dotations >().ResupplyDotations();

    PHY_RoleInterface_Supply* role = RetrieveRole< PHY_RoleInterface_Supply >();
    if( role )
        role->ResupplyStocks();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveMsgResupplyEquipement
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveMsgResupplyEquipement()
{
    GetRole< PHY_RolePion_Composantes >().RepairAllComposantes();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveMsgResupplyAll
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveMsgResupplyAll()
{
    GetRole< PHY_RolePion_Composantes >().RepairAllComposantes();
    GetRole< dotation::PHY_RolePion_Dotations   >().ResupplyDotations   ();
    PHY_RoleInterface_Supply* role = RetrieveRole< PHY_RoleInterface_Supply >();
    if( role )
        role->ResupplyStocks();
    GetRole< human::PHY_RolePion_Humans      >().HealAllHumans       ();
    GetRole< nbc::PHY_RolePion_NBC    >().Decontaminate       ();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveMsgResupply
// Created: NLD 2005-07-27
// -----------------------------------------------------------------------------
void  MIL_AgentPion::OnReceiveMsgResupply( const ASN1T_MagicActionPartialRecovery& asn )
{
    if( asn.m.equipementsPresent )
    {
        PHY_RolePion_Composantes& roleComposantes = GetRole< PHY_RolePion_Composantes >();
        for( uint i = 0; i < asn.equipements.n; ++i )
        {
            const ASN1T_EquipmentRecovery& asnEquipement = asn.equipements.elem[ i ];
            const PHY_ComposanteTypePion* pComposanteType = PHY_ComposanteTypePion::Find( asnEquipement.type_equipement );
            if( pComposanteType )
                roleComposantes.ChangeComposantesAvailability( *pComposanteType, asnEquipement.nombre_disponible );
        }
    }

    if( asn.m.personnelsPresent )
    {
        human::PHY_RolePion_Humans& roleHumans = GetRole< human::PHY_RolePion_Humans >();
        for( uint i = 0 ; i < asn.personnels.n; ++i )
        {
            const ASN1T_HumanRecovery& asnPersonnel = asn.personnels.elem[ i ];
            const PHY_HumanRank* pHumanRank = PHY_HumanRank::Find( asnPersonnel.rang );
            if( pHumanRank )
                roleHumans.ChangeHumansAvailability( *pHumanRank, asnPersonnel.nombre_disponible ); 
        }
    }
    
    if( asn.m.dotationsPresent )
    {
        dotation::PHY_RolePion_Dotations& roleDotations = GetRole< dotation::PHY_RolePion_Dotations >();
        for( uint i = 0; i < asn.dotations.n; ++i )
        {
            const ASN1T_DotationRecovery& asnDotation = asn.dotations.elem[ i ];
            const PHY_DotationType* pDotationType = PHY_DotationType::FindDotationType( asnDotation.famille_dotation ); 
            if( pDotationType )
                roleDotations.ResupplyDotations( *pDotationType, asnDotation.pourcentage / 100. );
        }
    }

    if( asn.m.munitionsPresent )
    {
        dotation::PHY_RolePion_Dotations& roleDotations = GetRole< dotation::PHY_RolePion_Dotations >();
        for( uint i = 0; i < asn.munitions.n; ++i )
        {
            const ASN1T_AmmunitionDotationRecovery& asnMunition = asn.munitions.elem[ i ];
            const PHY_AmmoDotationClass* pAmmoClass = PHY_AmmoDotationClass::Find( asnMunition.famille_munition ); 
            if( pAmmoClass )
                roleDotations.ResupplyDotations( *pAmmoClass, asnMunition.pourcentage / 100. );
        }
    }

    if( asn.m.stocksPresent )
    {
        PHY_RoleInterface_Supply* roleSupply = RetrieveRole< PHY_RoleInterface_Supply >();
        if( roleSupply )
        {
            for( uint i = 0; i < asn.stocks.n; ++i )
            {
                const ASN1T_StockRecovery& asnStock = asn.stocks.elem[ i ];
                const PHY_DotationCategory* pDotationCategory = PHY_DotationType::FindDotationCategory( asnStock.ressource_id );
                if( pDotationCategory )
                    roleSupply->ResupplyStocks( *pDotationCategory, asnStock.quantite_disponible );
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveMsgDestroyAll
// Created: NLD 2004-12-07
// -----------------------------------------------------------------------------
void  MIL_AgentPion::OnReceiveMsgDestroyAll()
{
    GetRole< PHY_RolePion_Composantes >().DestroyAllComposantes();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveMsgRecoverHumansTransporters
// Created: NLD 2005-12-27
// -----------------------------------------------------------------------------
void  MIL_AgentPion::OnReceiveMsgRecoverHumansTransporters()
{
    GetRole< transport::PHY_RoleInterface_Transported >().RecoverHumanTransporters();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveMsgUnitMagicAction
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveMsgUnitMagicAction( const ASN1T_MsgUnitMagicAction& asnMsg )
{
    switch( asnMsg.action.t )
    {
        case T_MsgUnitMagicAction_action_move_to                    : OnReceiveMsgMagicMove                  ( *asnMsg.action.u.move_to ); break;
        case T_MsgUnitMagicAction_action_recompletement_personnel   : OnReceiveMsgResupplyHumans             (); break;
        case T_MsgUnitMagicAction_action_recompletement_ressources  : OnReceiveMsgResupplyResources          (); break;
        case T_MsgUnitMagicAction_action_recompletement_equipement  : OnReceiveMsgResupplyEquipement         (); break;
        case T_MsgUnitMagicAction_action_recompletement_total       : OnReceiveMsgResupplyAll                (); break;
        case T_MsgUnitMagicAction_action_recompletement_partiel     : OnReceiveMsgResupply                   ( *asnMsg.action.u.recompletement_partiel ); break;
        case T_MsgUnitMagicAction_action_change_facteurs_humains    : OnReceiveMsgChangeHumanFactors         ( *asnMsg.action.u.change_facteurs_humains ); break;
        case T_MsgUnitMagicAction_action_destruction_totale         : OnReceiveMsgDestroyAll                 (); break;
        case T_MsgUnitMagicAction_action_destruction_composante     : OnReceiveMsgDestroyComponent           (); break;
        case T_MsgUnitMagicAction_action_recuperer_transporteurs    : OnReceiveMsgRecoverHumansTransporters  (); break;
        case T_MsgUnitMagicAction_action_se_rendre                  : pAutomate_->OnReceiveMsgUnitMagicAction( asnMsg ); return;        
        case T_MsgUnitMagicAction_action_annuler_reddition          : pAutomate_->OnReceiveMsgUnitMagicAction( asnMsg ); return;        
        default:
            assert( false );
    }
    UpdatePhysicalState();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveMagicSurrender
// Created: NLD 2005-03-04
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveMagicSurrender()
{
    GetRole< PHY_RoleInterface_Surrender >().NotifySurrendered();
    orderManager_.ReplaceMission();
    UpdatePhysicalState();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveMagicCancelSurrender
// Created: NLD 2007-02-15
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveMagicCancelSurrender()
{
    GetRole< PHY_RoleInterface_Surrender >().NotifySurrenderCanceled();
    UpdatePhysicalState();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveMsgChangeSuperior
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveMsgChangeSuperior( const MIL_EntityManager& manager, const ASN1T_MsgUnitChangeSuperior& asnMsg )
{
    MIL_Automate* pNewAutomate = manager.FindAutomate( asnMsg.oid_automate );
    if( !pNewAutomate )
        throw NET_AsnException< ASN1T_EnumChangeHierarchyErrorCode >( EnumChangeHierarchyErrorCode::error_invalid_automate );

    if( pNewAutomate->GetArmy() != GetArmy() )
        throw NET_AsnException< ASN1T_EnumChangeHierarchyErrorCode >( EnumChangeHierarchyErrorCode::error_camps_incompatibles );

    pAutomate_->UnregisterPion( *this );
    pAutomate_ = pNewAutomate;
    pAutomate_->RegisterPion  ( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::Serialize
// Created: AGE 2004-11-22
// -----------------------------------------------------------------------------
void MIL_AgentPion::Serialize( HLA_UpdateFunctor& functor ) const
{
    functor.Serialize( "armee", false, GetArmy().GetName() );
    functor.Serialize( "type" , false, GetType().GetName() );
    const bool bUpdateStatuses = GetRole< PHY_RolePion_Composantes      >().HasChanged()
                              || GetRole< PHY_RolePion_Communications   >().HasChanged()
                              || GetRole< nbc::PHY_RolePion_NBC         >().HasChanged()
                              || GetRole< PHY_RolePion_Posture          >().HLAStatusHasChanged()
                              || GetRole< transport::PHY_RoleInterface_Transported >().HasChanged()
                              || GetRole< PHY_RoleInterface_Surrender   >().HasChanged()
                              || GetRole< PHY_RolePion_Perceiver        >().HasRadarStateChanged()
                              || GetRole< PHY_RolePion_Population       >().HasChanged();

    // $$$$ AGE 2004-12-13: Test functor.MustUpdateAll() if performance issues (doubt it)
    std::vector< std::string > statuses;
    if( IsPC() )
        statuses.push_back( "pc" );
    if( GetRole< PHY_RolePion_Composantes >().IsNeutralized() )
        statuses.push_back( "neutralise" );
    if( ! GetRole< PHY_RolePion_Communications >().CanCommunicate() )
        statuses.push_back( "brouille" );
    if( GetRole< nbc::PHY_RolePion_NBC >().IsContaminated() )
        statuses.push_back( "contamine" );
    if( GetRole< PHY_RolePion_Posture >().IsStealth() )
        statuses.push_back( "furtif" );
    if( GetRole< transport::PHY_RoleInterface_Transported >().IsTransported() )
        statuses.push_back( "transporte" );
    if( GetRole< PHY_RolePion_Perceiver >().IsUsingActiveRadar() )
        statuses.push_back( "radaractif" );
    if( GetRole< PHY_RoleInterface_Surrender >().IsPrisoner() )
        statuses.push_back( "prisonnier" );
    // $$$$ NLD 2007-02-14: Surrender
    if( GetRole< PHY_RolePion_Refugee >().IsManaged() )
        statuses.push_back( "refugie_prisencompte" );
    if( GetRole< PHY_RolePion_Population >().IsInvulnerable() )
        statuses.push_back( "invulnerable_population" );
    functor.Serialize( "status", bUpdateStatuses, statuses );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::ChangeSuperior
// Created: NLD 2004-10-26
// -----------------------------------------------------------------------------
void MIL_AgentPion::ChangeSuperior( MIL_Automate& newAutomate )
{
    assert( GetArmy() == newAutomate.GetArmy() );
    pAutomate_->UnregisterPion( *this );
    pAutomate_ = &newAutomate;
    pAutomate_->RegisterPion  ( *this );

    NET_ASN_MsgUnitChangeSuperior asnMsg;
    asnMsg().oid          = GetID();
    asnMsg().oid_automate = newAutomate.GetID();
    asnMsg.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::NotifyAttackedBy
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
void MIL_AgentPion::NotifyAttackedBy( MIL_AgentPion& attacker )
{
    if( attacker.GetType().IsRefugee() )
        MIL_Report::PostEvent( *this, MIL_Report::eReport_FiredByCivilian );
    else if( GetArmy().IsNeutral( attacker.GetArmy() ) == eTristate_True )
        MIL_Report::PostEvent( *this, MIL_Report::eReport_FiredByNeutralSide );
    else if( GetArmy().IsAFriend( attacker.GetArmy() ) == eTristate_True )
        MIL_Report::PostEvent( *this, MIL_Report::eReport_FiredByFriendSide );

    GetKnowledge().GetKsFire().NotifyAttackedBy( attacker );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::NotifyAttackedBy
// Created: NLD 2005-12-01
// -----------------------------------------------------------------------------
void MIL_AgentPion::NotifyAttackedBy( MIL_Population& attacker )
{
    //$$$ RCs ??
    GetKnowledge().GetKsFire().NotifyAttackedBy( attacker );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::NotifyAttacking
// Created: NLD 2005-03-30
// -----------------------------------------------------------------------------
void MIL_AgentPion::NotifyAttacking( MIL_Agent_ABC& target ) const
{
    if( target.GetType().IsRefugee() )
        MIL_Report::PostEvent( *this, MIL_Report::eReport_FireOnCivilian );
    else if( GetArmy().IsNeutral( target.GetArmy() ) == eTristate_True )
        MIL_Report::PostEvent( *this, MIL_Report::eReport_FireOnNeutralSide );
    else if( GetArmy().IsAFriend( target.GetArmy() ) == eTristate_True )
        MIL_Report::PostEvent( *this, MIL_Report::eReport_FireOnFriendSide );
}            

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::NotifyAttacking
// Created: NLD 2005-12-01
// -----------------------------------------------------------------------------
void MIL_AgentPion::NotifyAttacking( MIL_Population& /*target*/ ) const
{
    //$$$ CRS ??
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::IsPerceiving
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
bool MIL_AgentPion::IsPerceived( const MIL_Agent_ABC& agent ) const
{
    return GetKnowledge().IsPerceived( agent );    
}

int MIL_AgentPion::GetNumberOfFireHoses( int /*bestExtinguisherAgent*/ )
{
    return 4;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::GetKnowledge
// Created: NLD 2004-09-01
// -----------------------------------------------------------------------------
DEC_KnowledgeBlackBoard_AgentPion& MIL_AgentPion::GetKnowledge() const
{
    assert( pKnowledgeBlackBoard_ );
    return *pKnowledgeBlackBoard_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::GetAutomate
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
const MIL_Automate& MIL_AgentPion::GetAutomate() const
{
    assert( pAutomate_ );
    return *pAutomate_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::GetAutomate
// Created: NLD 2004-12-27
// -----------------------------------------------------------------------------
MIL_Automate& MIL_AgentPion::GetAutomate()
{
    assert( pAutomate_ );
    return *pAutomate_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::GetType
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
const MIL_AgentTypePion& MIL_AgentPion::GetType() const
{
    assert( pType_ );
    return *pType_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::IsPC
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
bool MIL_AgentPion::IsPC() const
{
    return bIsPC_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::GetOrderManager
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
MIL_PionOrderManager& MIL_AgentPion::GetOrderManager()
{
    return orderManager_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::GetOrderManager
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
const MIL_PionOrderManager& MIL_AgentPion::GetOrderManager() const
{
    return orderManager_;
}
