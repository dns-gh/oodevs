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
#include "Roles/Logistic/PHY_RoleInterface_Supply.h"
#include "Roles/Logistic/PHY_RoleInterface_Maintenance.h"
#include "Roles/Logistic/PHY_RoleInterface_Medical.h"
#include "Roles/Illumination/PHY_RolePion_Illumination.h" // LTO
#include "Roles/Terrain/PHY_RolePion_TerrainAnalysis.h"
#include "Roles/Urban/PHY_RolePion_UrbanLocation.h"

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
#include "Entities/MIL_Army_ABC.h"
#include "Decision/DEC_Model_ABC.h"
#include "Decision/DEC_Representations.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Knowledge/DEC_KS_Fire.h"
#include "Network/NET_AgentServer.h"
#include "Network/NET_Publisher_ABC.h"
#include "Network/NET_AsnException.h"
#include "protocol/ClientSenders.h"
#include "Hla/HLA_UpdateFunctor.h"
#include "Tools/MIL_Tools.h"
#include "Tools/MIL_IDManager.h"
#include "simulation_kernel/AlgorithmsFactories.h"
#include "simulation_kernel/NetworkNotificationHandler_ABC.h"
#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_AgentPion )

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion constructor
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_AgentPion::MIL_AgentPion( const MIL_AgentTypePion& type, MIL_Automate& automate, const AlgorithmsFactories& algorithmFactories, xml::xistream& xis )
    : MIL_Agent_ABC        ( xis )
    , pType_               ( &type )
    , bIsPC_               ( xml::attribute< bool >( xis, "command-post", false ) )
    , pAutomate_           ( &automate )
    , pKnowledgeBlackBoard_(  new DEC_KnowledgeBlackBoard_AgentPion( *this ) )
    , orderManager_        ( *new MIL_PionOrderManager( *this ) )
    , algorithmFactories_  ( algorithmFactories )
{
    automate.RegisterPion( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion constructor
// Created: NLD 2005-02-08
// -----------------------------------------------------------------------------
MIL_AgentPion::MIL_AgentPion( const MIL_AgentTypePion& type, MIL_Automate& automate, const AlgorithmsFactories& algorithmFactories )
    : MIL_Agent_ABC            ( type.GetName() )
    , pType_                   ( &type )
    , bIsPC_                   ( false )
    , pAutomate_               ( &automate )
    , pKnowledgeBlackBoard_    (  new DEC_KnowledgeBlackBoard_AgentPion( *this ) )
    , orderManager_            ( *new MIL_PionOrderManager( *this ) )
    , algorithmFactories_      ( algorithmFactories )
{
    automate.RegisterPion( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion constructor
// Created: LDC 2010-02-22
// -----------------------------------------------------------------------------
MIL_AgentPion::MIL_AgentPion( const MIL_AgentTypePion& type, const AlgorithmsFactories& algorithmFactories )
    : MIL_Agent_ABC            ( type.GetName() )
    , pType_                   ( &type )
    , bIsPC_                   ( false )
    , pAutomate_               ( 0 )
    , pKnowledgeBlackBoard_    (  new DEC_KnowledgeBlackBoard_AgentPion( *this ) )
    , orderManager_            ( *new MIL_PionOrderManager( *this ) )
    , algorithmFactories_      ( algorithmFactories )
{
    // NOTHING
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

namespace
{
    template< typename R >
    R& LoadRole( MIL_CheckPointInArchive& archive, tools::RoleContainer& container )
    {
        R* role;
        archive >> role;
        if( !role )
            throw std::runtime_error( __FUNCTION__ ": Failed to load role " + std::string( typeid( role ).name() ) );
        container.RegisterRole( *role );
        return *role;
    }

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
    const AlgorithmsFactories* const algorithmFactories = &pion->algorithmFactories_;
    archive << nTypeID
            << algorithmFactories;
}

template< typename Archive >
void load_construct_data( Archive& archive, MIL_AgentPion* pion, const unsigned int /*version*/ )
{
    unsigned int nTypeID;
    AlgorithmsFactories* algorithmFactories = 0;
    archive >> nTypeID
            >> algorithmFactories;
    const MIL_AgentTypePion* pType = MIL_AgentTypePion::Find( nTypeID );
    assert( pType );
    ::new( pion )MIL_AgentPion( *pType, *algorithmFactories );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::load
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_AgentPion::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< MIL_Agent_ABC >( *this );
    
    file >> const_cast< bool& >( bIsPC_ )
         >> pAutomate_
      // >> actions_ // actions non sauvegardées
         >> pKnowledgeBlackBoard_;
           
    LoadRole< network::NET_RolePion_Dotations >( file, *this );
    LoadRole< PHY_RolePion_Reinforcement >( file, *this );
    LoadRole< PHY_RolePion_Posture >( file, *this );
    LoadRole< PHY_RolePion_Location >( file, *this );
    LoadRole< PHY_RolePion_UrbanLocation >( file, *this );
    LoadRole< dotation::PHY_RolePion_Dotations >( file, *this );
    LoadRole< human::PHY_RolePion_Humans >( file, *this );
    LoadRole< PHY_RolePion_Composantes >( file, *this );
    LoadRole< PHY_RolePion_Perceiver >( file, *this ).Initialization( GetRole< PHY_RoleInterface_Location >().GetPosition(), GetRole< PHY_RoleInterface_Location >().GetDirection() );
    LoadRole< nbc::PHY_RolePion_NBC >( file, *this );
    LoadRole< PHY_RolePion_Communications >( file, *this );
    LoadRole< PHY_RolePion_HumanFactors >( file, *this );
    LoadRole< transport::PHY_RolePion_Transported >( file, *this );
    //LoadRole< PHY_RoleInterface_Maintenance >( file, *this ); //@TODO refactor with new save
    //LoadRole< PHY_RoleInterface_Medical >( file, *this );
    //LoadRole< PHY_RoleInterface_Supply >( file, *this );
    LoadRole< surrender::PHY_RolePion_Surrender >( file, *this );
    LoadRole< refugee::PHY_RolePion_Refugee >( file, *this );
    LoadRole< PHY_RolePion_Population >( file, *this );
    LoadRole< transport::PHY_RoleAction_Loading >( file, *this );
    LoadRole< transport::PHY_RoleAction_Transport >( file, *this );
    LoadRole< moving::PHY_RoleAction_Moving >( file, *this );
    LoadRole< PHY_RoleAction_Objects >( file, *this );
    LoadRole< firing::PHY_RoleAction_DirectFiring >( file, *this );
    LoadRole< firing::PHY_RoleAction_IndirectFiring >( file, *this );
    LoadRole< DEC_RolePion_Decision >( file, *this );
    LoadRole< PHY_RoleAction_InterfaceFlying >( file, *this );
    LoadRole< PHY_RoleAction_FolkInfluence >( file, *this );
    LoadRole< PHY_RolePion_Illumination >( file, *this ); // LTO
    RegisterRole( *new DEC_Representations() );
    RegisterRole( *new PHY_RolePion_TerrainAnalysis( *this ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::save
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_AgentPion::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    assert( pType_ );

    file << boost::serialization::base_object< MIL_Agent_ABC >( *this );
    file << bIsPC_
        << pAutomate_
        // << actions_ // actions non sauvegardées
        << pKnowledgeBlackBoard_;

    SaveRole< network::NET_RolePion_Dotations >( *this, file );
    SaveRole< PHY_RolePion_Reinforcement     >( *this, file );
    SaveRole< PHY_RolePion_Posture           >( *this, file );
    SaveRole< PHY_RolePion_Location          >( *this, file );
    SaveRole< PHY_RolePion_UrbanLocation     >( *this, file );
    SaveRole< dotation::PHY_RolePion_Dotations >( *this, file );
    SaveRole< human::PHY_RolePion_Humans     >( *this, file );
    SaveRole< PHY_RolePion_Composantes       >( *this, file );
    SaveRole< PHY_RolePion_Perceiver         >( *this, file );
    SaveRole< nbc::PHY_RolePion_NBC          >( *this, file );
    SaveRole< PHY_RolePion_Communications    >( *this, file );
    SaveRole< PHY_RolePion_HumanFactors      >( *this, file );
    SaveRole< transport::PHY_RolePion_Transported >( *this, file );
    //SaveRole< PHY_RolePion_Maintenance       >( *this, file );//@TODO refactor with new save
    //SaveRole< PHY_RolePion_Medical           >( *this, file );
    //SaveRole< PHY_RolePion_Supply            >( *this, file );
    SaveRole< surrender::PHY_RolePion_Surrender >( *this, file );
    SaveRole< refugee::PHY_RolePion_Refugee >( *this, file );
    SaveRole< PHY_RolePion_Population >( *this, file );
    SaveRole< transport::PHY_RoleAction_Loading >( *this, file );
    SaveRole< transport::PHY_RoleAction_Transport >( *this, file );
    SaveRole< moving::PHY_RoleAction_Moving  >( *this, file );
    SaveRole< PHY_RoleAction_Objects         >( *this, file );
    SaveRole< firing::PHY_RoleAction_DirectFiring    >( *this, file );
    SaveRole< firing::PHY_RoleAction_IndirectFiring  >( *this, file );
    SaveRole< DEC_RolePion_Decision          >( *this, file );
    SaveRole< PHY_RoleAction_InterfaceFlying >( *this, file );
    SaveRole< PHY_RoleAction_FolkInfluence   >( *this, file );  
    SaveRole< PHY_RolePion_Illumination >     ( *this, file ); // LTO
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

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::GetMajorWeight
// Created: RPD 2009-08-19
// -----------------------------------------------------------------------------
MT_Float MIL_AgentPion::GetMajorComponentWeight() const
{
    return GetRole< PHY_RolePion_Composantes >().GetMajorComponentWeight();
}

// =============================================================================
// UPDATES
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::UpdateKnowledges
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
void MIL_AgentPion::UpdateKnowledges( int currentTimeStep )
{
    assert( pKnowledgeBlackBoard_ );
    pKnowledgeBlackBoard_->Update(currentTimeStep);
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
        orderManager_.CancelMission();
    try
    {
        orderManager_.Update();
        GetRole< DEC_Decision_ABC >().UpdateDecision( duration );
    }
    catch( std::runtime_error& /*e*/ )
    {        
        MIL_Report::PostEvent( *this, MIL_Report::eReport_MissionImpossible_ );
    }
    GetRole< DEC_Decision_ABC >().GarbageCollect();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::UpdatePhysicalState
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void MIL_AgentPion::UpdatePhysicalState()
{
    const bool bIsDead = IsDead();
    GetRole< dotation::PHY_RolePion_Dotations     >().Update( bIsDead );
    GetRole< human::PHY_RolePion_Humans           >().Update( bIsDead );
    GetRole< PHY_RolePion_Composantes             >().Update( bIsDead );
    GetRole< PHY_RolePion_Posture                 >().Update( bIsDead );
    GetRole< PHY_RolePion_Reinforcement           >().Update( bIsDead );
    GetRole< PHY_RolePion_Location                >().Update( bIsDead );
    GetRole< nbc::PHY_RolePion_NBC                >().Update( bIsDead );
    GetRole< PHY_RolePion_Communications          >().Update( bIsDead );
    GetRole< PHY_RolePion_HumanFactors            >().Update( bIsDead );
    GetRole< transport::PHY_RolePion_Transported  >().Update( bIsDead );
    GetRole< surrender::PHY_RolePion_Surrender    >().Update( bIsDead );
    GetRole< refugee::PHY_RolePion_Refugee        >().Update( bIsDead );    
    GetRole< PHY_RolePion_Population              >().Update( bIsDead );    
    GetRole< PHY_RolePion_Perceiver               >().Update( bIsDead ); // Doit être après PHY_RolePion_Composantes $$$ pourri - utiliser des observers
    GetRole< transport::PHY_RoleAction_Loading    >().Update( bIsDead );
    GetRole< transport::PHY_RoleAction_Transport  >().Update( bIsDead );
    GetRole< PHY_RoleAction_Objects               >().Update( bIsDead );
    GetRole< moving::PHY_RoleAction_Moving        >().Update( bIsDead );
    GetRole< PHY_RoleAction_InterfaceFlying       >().Update( bIsDead );
    GetRole< firing::PHY_RoleAction_DirectFiring  >().Update( bIsDead );
    GetRole< firing::PHY_RoleAction_IndirectFiring>().Update( bIsDead );
    GetRole< PHY_RoleAction_FolkInfluence         >().Update( bIsDead );

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
    GetRole< network::NET_RolePion_Dotations >().SendChangedState();
    GetRole< network::NET_RolePion_Dotations >().Clean();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::PreprocessRandomBreakdowns
// Created: NLD 2005-01-06
// -----------------------------------------------------------------------------
void MIL_AgentPion::PreprocessRandomBreakdowns( unsigned int nEndDayTimeStep ) const
{
    GetRole< PHY_RolePion_Composantes >().PreprocessRandomBreakdowns( nEndDayTimeStep );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::Clean
// Created: AGE 2004-11-23
// -----------------------------------------------------------------------------
void MIL_AgentPion::Clean()
{
    GetRole< PHY_RolePion_Location                   >().Clean();
    GetRole< PHY_RolePion_Perceiver                  >().Clean();
    GetRole< dotation::PHY_RolePion_Dotations        >().Clean();
    GetRole< human::PHY_RolePion_Humans              >().Clean();
    GetRole< PHY_RolePion_Composantes                >().Clean();
    GetRole< PHY_RolePion_Posture                    >().Clean();
    GetRole< PHY_RolePion_Reinforcement              >().Clean();
    GetRole< nbc::PHY_RolePion_NBC                   >().Clean();
    GetRole< PHY_RolePion_Communications             >().Clean();
    GetRole< PHY_RolePion_HumanFactors               >().Clean();
    GetRole< surrender::PHY_RoleInterface_Surrender  >().Clean();
    GetRole< refugee::PHY_RolePion_Refugee           >().Clean();
    GetRole< PHY_RolePion_Population                 >().Clean();
    GetRole< transport::PHY_RoleInterface_Transported>().Clean();
    GetRole< transport::PHY_RoleAction_Transport     >().Clean();
    GetRole< transport::PHY_RoleAction_Loading       >().Clean();
    GetRole< PHY_RoleAction_Objects                  >().Clean();
    GetRole< moving::PHY_RoleAction_Moving           >().Clean();
    GetRole< PHY_RoleAction_InterfaceFlying          >().Clean();
    GetRole< firing::PHY_RoleAction_DirectFiring     >().Clean();
    GetRole< firing::PHY_RoleAction_IndirectFiring   >().Clean();
    GetRole< DEC_RolePion_Decision                   >().Clean();
    GetRole< PHY_RoleAction_FolkInfluence            >().Clean();

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
// returns the knowledge group of the automate if can communicate
// else return the knowledge group of communication role
// Created: NLD 2004-08-30
// Modified: FDS 2010-03-17
// -----------------------------------------------------------------------------
MIL_KnowledgeGroup& MIL_AgentPion::GetKnowledgeGroup() const
{
    if( GetRole< PHY_RolePion_Communications >().CanEmit() )
    {
        if( pAutomate_ == 0 )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Automate is undefined for agent id %d ", GetID() ) );

        return pAutomate_->GetKnowledgeGroup();
    } else {
        return GetRole< PHY_RolePion_Communications >().GetKnowledgeGroup();
    }
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
boost::shared_ptr< DEC_Knowledge_Agent > MIL_AgentPion::CreateKnowledge( const MIL_KnowledgeGroup& knowledgeGroup )
{
    boost::shared_ptr< DEC_Knowledge_Agent > result( new DEC_Knowledge_Agent( knowledgeGroup, *this ) );
    return result;
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

    client::UnitCreation asnMsg;
    asnMsg().set_oid         ( GetID() );
    asnMsg().set_type_pion   ( pType_->GetID() );
    asnMsg().set_nom         ( GetName().c_str() ); // !! pointeur sur const char*
    asnMsg().set_oid_automate( GetAutomate().GetID() );
    asnMsg().set_pc          ( IsPC() );
    asnMsg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::SendFullState
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void MIL_AgentPion::SendFullState() const
{
    GetRole< network::NET_RolePion_Dotations >().SendFullState();
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
void MIL_AgentPion::OnReceiveMsgOrder( const Common::MsgUnitOrder& msg )
{
    orderManager_.OnReceiveMission( msg );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveMsgFragOrder
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------

void MIL_AgentPion::OnReceiveMsgFragOrder( const MsgsClientToSim::MsgFragOrder& msg )
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
    GetRole< PHY_RolePion_UrbanLocation >().MagicMove( vNewPos );
    CancelAllActions();
    GetRole< DEC_RolePion_Decision >().Reset( GetAutomate().GetName() );
    orderManager_.CancelMission();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveMsgMagicActionMoveTo
// Created: NLD 2005-08-18
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveMsgMagicActionMoveTo( const MT_Vector2D& vPosition )
{
    MagicMove( vPosition );
    UpdatePhysicalState();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveMsgMagicActionMoveTo
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveMsgMagicActionMoveTo( const MsgsClientToSim::MsgUnitMagicAction& asn )
{
    if( asn.type() != MsgsClientToSim::MsgUnitMagicAction_Type_move_to )
        throw NET_AsnException< MsgsSimToClient::UnitActionAck_ErrorCode >( MsgsSimToClient::UnitActionAck_ErrorCode_error_invalid_attribute );

    if( pAutomate_->IsEngaged() )
        throw NET_AsnException< MsgsSimToClient::UnitActionAck_ErrorCode >( MsgsSimToClient::UnitActionAck_ErrorCode_error_automate_embraye );

    if( !asn.has_parametres() || asn.parametres().elem_size() != 1)
        throw NET_AsnException< MsgsSimToClient::UnitActionAck_ErrorCode >( MsgsSimToClient::UnitActionAck_ErrorCode_error_invalid_attribute );
    
    const Common::MsgMissionParameter& parametre = asn.parametres().elem( 0 );
    if( !parametre.has_value() || !parametre.value().has_point() )
        throw NET_AsnException< MsgsSimToClient::UnitActionAck_ErrorCode >( MsgsSimToClient::UnitActionAck_ErrorCode_error_invalid_attribute );

    const Common::MsgPoint& point = parametre.value().point();

    if( point.location().type() != Common::MsgLocation_Geometry_point 
        || point.location().coordinates().elem_size() != 1 )
        throw NET_AsnException< MsgsSimToClient::UnitActionAck_ErrorCode >( MsgsSimToClient::UnitActionAck_ErrorCode_error_invalid_attribute );

    MT_Vector2D vPosTmp;
    MIL_Tools::ConvertCoordMosToSim( point.location().coordinates().elem(0), vPosTmp );

    MagicMove( vPosTmp );
    UpdatePhysicalState();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveMsgChangeHumanFactors
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
void  MIL_AgentPion::OnReceiveMsgChangeHumanFactors( const Common::MsgMissionParameters& msg )
{
    if( msg.elem( 0 ).has_value() && msg.elem( 0 ).value().has_enumeration() )
    {
        Common::EnumUnitTiredness tiredness = ( Common::EnumUnitTiredness ) msg.elem( 0 ).value().enumeration();
        const PHY_Tiredness* pTiredness = PHY_Tiredness::Find( tiredness );
        if( !pTiredness )
            throw NET_AsnException< MsgsSimToClient::UnitActionAck_ErrorCode >( MsgsSimToClient::UnitActionAck_ErrorCode_error_invalid_attribute );
        GetRole< PHY_RolePion_HumanFactors >().SetTiredness( *pTiredness );
    }

    if( msg.elem( 1 ).has_value() && msg.elem( 1 ).value().has_enumeration() )
    {
        Common::EnumUnitMorale morale = ( Common::EnumUnitMorale ) msg.elem( 1 ).value().enumeration();
        const PHY_Morale* pMoral = PHY_Morale::Find( morale );
        if( !pMoral )
            throw NET_AsnException< MsgsSimToClient::UnitActionAck_ErrorCode >( MsgsSimToClient::UnitActionAck_ErrorCode_error_invalid_attribute );
        GetRole< PHY_RolePion_HumanFactors >().SetMorale( *pMoral );
    }

    if( msg.elem( 2 ).has_value() && msg.elem( 2 ).value().has_enumeration() )
    {
        Common::EnumUnitExperience experience = ( Common::EnumUnitExperience ) msg.elem( 2 ).value().enumeration();
        const PHY_Experience* pExperience = PHY_Experience::Find( experience );
        if( !pExperience )
            throw NET_AsnException< MsgsSimToClient::UnitActionAck_ErrorCode >( MsgsSimToClient::UnitActionAck_ErrorCode_error_invalid_attribute );
        GetRole< PHY_RolePion_HumanFactors >().SetExperience( *pExperience );
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
// Created: JSR 2010-04-15
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveMsgResupply( const Common::MsgMissionParameters& msg )
{
    if( msg.elem( 0 ).has_value() ) // Equipments
    {
        PHY_RolePion_Composantes& roleComposantes = GetRole< PHY_RolePion_Composantes >();
        for( int i = 0; i < msg.elem( 0 ).value().list_size(); ++i )
        {
            Common::MsgEquipmentType type;
            type.set_equipment( msg.elem( 0 ).value().list( i ).list( 0 ).identifier() );
            int number = msg.elem( 0 ).value().list( i ).list( 1 ).quantity();
            const PHY_ComposanteTypePion* pComposanteType = PHY_ComposanteTypePion::Find( type );
            if( pComposanteType )
                roleComposantes.ChangeComposantesAvailability( *pComposanteType, number );
        }
    }

    if( msg.elem( 1 ).has_value() ) // Humans
    {
        human::PHY_RolePion_Humans& roleHumans = GetRole< human::PHY_RolePion_Humans >();
        for( int i = 0 ; i < msg.elem( 1 ).value().list_size(); ++i )
        {
            unsigned int rank = msg.elem( 1 ).value().list( i ).list( 0 ).identifier();
            int number = msg.elem( 1 ).value().list( i ).list( 1 ).quantity();
            const PHY_HumanRank* pHumanRank = PHY_HumanRank::Find( rank );
            if( pHumanRank )
                roleHumans.ChangeHumansAvailability( *pHumanRank, number ); 
        }
    }
    
    if( msg.elem( 2 ).has_value() ) // Dotations
    {
        dotation::PHY_RolePion_Dotations& roleDotations = GetRole< dotation::PHY_RolePion_Dotations >();
        for( int i = 0; i < msg.elem( 2 ).value().list_size(); ++i )
        {
            unsigned int dotation = msg.elem( 2 ).value().list( i ).list( 0 ).identifier();
            int number = msg.elem( 2 ).value().list( i ).list( 1 ).quantity();
            const PHY_DotationType* pDotationType = PHY_DotationType::FindDotationType( dotation ); 
            if( pDotationType )
                roleDotations.ResupplyDotations( *pDotationType, number / 100. );
        }
    }

    if( msg.elem( 3 ).has_value() ) // Ammunition
    {
        dotation::PHY_RolePion_Dotations& roleDotations = GetRole< dotation::PHY_RolePion_Dotations >();
        for( int i = 0; i < msg.elem( 3 ).value().list_size(); ++i )
        {
            unsigned int munition = msg.elem( 3 ).value().list( i ).list( 0 ).identifier();
            int number = msg.elem( 3 ).value().list( i ).list( 1 ).quantity();
            const PHY_AmmoDotationClass* pAmmoClass = PHY_AmmoDotationClass::Find( munition ); 
            if( pAmmoClass )
                roleDotations.ResupplyDotations( *pAmmoClass, number / 100. );
        }
    }

    if( msg.elem( 4 ).has_value() ) // stocks
    {
        if( PHY_RoleInterface_Supply* roleSupply = RetrieveRole< PHY_RoleInterface_Supply >() )
        {
            for( int i = 0; i < msg.elem( 4 ).value().list_size(); ++i )
            {
                unsigned int stock = msg.elem( 4 ).value().list( i ).list( 0 ).identifier();
                int number = msg.elem( 4 ).value().list( i ).list( 1 ).quantity();
                const PHY_DotationCategory* pDotationCategory = PHY_DotationType::FindDotationCategory( stock );
                if( pDotationCategory )
                    roleSupply->ResupplyStocks( *pDotationCategory, number );
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
// Created: JSR 2010-04-14
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveMsgUnitMagicAction( const MsgsClientToSim::MsgUnitMagicAction& msg, const tools::Resolver< MIL_Army_ABC >& armies )
{
    switch( msg.type() )
    {
    case MsgsClientToSim::MsgUnitMagicAction_Type_surrender_to:
        pAutomate_->OnReceiveMsgUnitMagicAction( msg, armies );
        return;
    case MsgsClientToSim::MsgUnitMagicAction_Type_cancel_surrender:
        pAutomate_->OnReceiveMsgUnitMagicAction( msg, armies );
        return;
    case MsgsClientToSim::MsgUnitMagicAction_Type_recover_transporters:
        OnReceiveMsgRecoverHumansTransporters(); 
        break;
    case MsgsClientToSim::MsgUnitMagicAction_Type_destroy_component:
        OnReceiveMsgDestroyComponent(); 
        break;
    case MsgsClientToSim::MsgUnitMagicAction_Type_recover_all:
        OnReceiveMsgResupplyAll(); 
        break;
    case MsgsClientToSim::MsgUnitMagicAction_Type_recover_troops:
        OnReceiveMsgResupplyHumans(); 
        break;
    case MsgsClientToSim::MsgUnitMagicAction_Type_recover_equipments:
        OnReceiveMsgResupplyEquipement(); 
        break;
    case MsgsClientToSim::MsgUnitMagicAction_Type_recover_resources:
        OnReceiveMsgResupplyResources(); 
        break;
    case MsgsClientToSim::MsgUnitMagicAction_Type_destroy_all:
        OnReceiveMsgDestroyAll(); 
        break;
    case MsgsClientToSim::MsgUnitMagicAction_Type_change_human_factors:
        OnReceiveMsgChangeHumanFactors( msg.parametres() );
        break;
    case MsgsClientToSim::MsgUnitMagicAction_Type_partial_recovery:
        OnReceiveMsgResupply( msg.parametres() );
        break;
    default:
        assert( false );
        break;
    }

    UpdatePhysicalState();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveMagicSurrender
// Created: NLD 2005-03-04
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveMagicSurrender()
{
    GetRole< surrender::PHY_RoleInterface_Surrender >().NotifySurrendered();
    orderManager_.CancelMission();
    UpdatePhysicalState();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveMagicCancelSurrender
// Created: NLD 2007-02-15
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveMagicCancelSurrender()
{
    GetRole< surrender::PHY_RoleInterface_Surrender >().NotifySurrenderCanceled();
    UpdatePhysicalState();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveMsgChangeSuperior
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveMsgChangeSuperior( const MIL_EntityManager& manager, const MsgsClientToSim::MsgUnitMagicAction& msg )
{
    MIL_Automate* pNewAutomate = manager.FindAutomate( msg.parametres().elem( 0 ).value().automat().oid() );
    if( !pNewAutomate )
        throw NET_AsnException< MsgsSimToClient::HierarchyModificationAck_ErrorCode >( MsgsSimToClient::HierarchyModificationAck_ErrorCode_error_invalid_automate );

    if( pNewAutomate->GetArmy() != GetArmy() )
        throw NET_AsnException< MsgsSimToClient::HierarchyModificationAck_ErrorCode >( MsgsSimToClient::HierarchyModificationAck_ErrorCode_error_teams_mismatched );

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
    //@TODO MGD REPLACE BY COMPUTER HasChanged is not longer public
    functor.Serialize( "armee", false, GetArmy().GetName() );
    functor.Serialize( "type" , false, GetType().GetName() );
    const bool bUpdateStatuses = true;/*GetRole< PHY_RolePion_Composantes      >().HasChanged()
                              || GetRole< PHY_RolePion_Communications   >().HasChanged()
                              || GetRole< nbc::PHY_RolePion_NBC         >().HasChanged()
                              || GetRole< PHY_RolePion_Posture          >().HLAStatusHasChanged()
                              || GetRole< transport::PHY_RoleInterface_Transported >().HasChanged()
                              || GetRole< surrender::PHY_RoleInterface_Surrender   >().HasChanged()
                              || GetRole< PHY_RolePion_Perceiver        >().HasRadarStateChanged()
                              || GetRole< PHY_RolePion_Population       >().HasChanged();*/

    // $$$$ AGE 2004-12-13: Test functor.MustUpdateAll() if performance issues (doubt it)
    std::vector< std::string > statuses;
    if( IsPC() )
        statuses.push_back( "pc" );
    if( GetRole< PHY_RolePion_Composantes >().IsNeutralized() )
        statuses.push_back( "neutralise" );
    if( !GetRole< PHY_RolePion_Communications >().CanEmit() &&  ! GetRole< PHY_RolePion_Communications >().CanReceive() )
        statuses.push_back( "brouille" );
    if( GetRole< PHY_RolePion_Communications >().CanReceive() && !GetRole< PHY_RolePion_Communications >().CanEmit() )
        statuses.push_back( "silenceradiopartiel" );
    if( GetRole< nbc::PHY_RolePion_NBC >().IsContaminated() )
        statuses.push_back( "contamine" );
    if( GetRole< PHY_RolePion_Posture >().IsStealth() )
        statuses.push_back( "furtif" );
    if( GetRole< transport::PHY_RoleInterface_Transported >().IsTransported() )
        statuses.push_back( "transporte" );
    if( GetRole< PHY_RolePion_Perceiver >().IsUsingActiveRadar() )
        statuses.push_back( "radaractif" );
    if( GetRole< surrender::PHY_RoleInterface_Surrender >().IsPrisoner() )
        statuses.push_back( "prisonnier" );
    // $$$$ NLD 2007-02-14: Surrender
    if( GetRole< refugee::PHY_RolePion_Refugee >().IsManaged() )
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

    client::UnitChangeSuperior asnMsg;
    asnMsg().set_oid( GetID() );
    asnMsg().set_oid_automate( newAutomate.GetID() );
    asnMsg.Send( NET_Publisher_ABC::Publisher() );
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
const MIL_AgentType_ABC& MIL_AgentPion::GetType() const
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

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::GetAlgorithms
// Created: MGD 2009-09-30
// -----------------------------------------------------------------------------
const AlgorithmsFactories& MIL_AgentPion::GetAlgorithms() const
{
    return algorithmFactories_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::GetDangerosity
// Created: MGD 2010-02-04
// -----------------------------------------------------------------------------
double MIL_AgentPion::GetDangerosity( boost::shared_ptr< DEC_Knowledge_Agent > pTargetKnowledge ) const
{
    if(     pTargetKnowledge->GetMaxPerceptionLevel() < PHY_PerceptionLevel::recognized_ 
        ||  GetArmy().IsAFriend( *pTargetKnowledge->GetArmy() ) == eTristate_True
        ||  pTargetKnowledge->IsSurrendered() )
        return 0.;

    // Target is dead ....
    const DEC_Knowledge_AgentComposante* pTargetMajorComposante = pTargetKnowledge->GetMajorComposante();
    if( !pTargetMajorComposante )
        return 0.;

    float rDangerosity = 0.;   

    // Fight score
    const PHY_RoleInterface_Location& sourceLocation = GetRole< PHY_RoleInterface_Location >();
    const MT_Vector3D sourcePosition( sourceLocation.GetPosition().rX_, sourceLocation.GetPosition().rY_, sourceLocation.GetAltitude() );
    const MT_Vector3D targetPosition  ( pTargetKnowledge->GetPosition().rX_, pTargetKnowledge->GetPosition().rY_, pTargetKnowledge->GetAltitude() );
    const MT_Float    rDistBtwSourceAndTarget = sourcePosition.Distance( targetPosition );

    rDangerosity = GetRole< PHY_RoleInterface_Composantes >().GetDangerosity( *pTargetMajorComposante, rDistBtwSourceAndTarget ); 
    
    //DegradeDangerosity( rDangerosity );//@TODO MGD before commit
    return rDangerosity;  
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::Distance
// Created: SLG 2010-04-12
// -----------------------------------------------------------------------------
double MIL_AgentPion::Distance( const MIL_Agent_ABC& target ) const
{
    const PHY_RoleInterface_Location& firerLocation = GetRole< PHY_RoleInterface_Location >();
    const geometry::Point2f firerPosition( firerLocation.GetPosition().rX_, firerLocation.GetPosition().rY_ );

    const PHY_RoleInterface_Location& targetLocation = target.GetRole< PHY_RoleInterface_Location >();
    const geometry::Point2f targetPosition( targetLocation.GetPosition().rX_, targetLocation.GetPosition().rY_ );

    const PHY_RoleInterface_UrbanLocation& firerUrbanRole = GetRole< PHY_RoleInterface_UrbanLocation >();
    const PHY_RoleInterface_UrbanLocation& targetUrbanRole = target.GetRole< PHY_RoleInterface_UrbanLocation >();
    
    if( firerUrbanRole.GetCurrentUrbanBlock() == targetUrbanRole.GetCurrentUrbanBlock() && firerUrbanRole.GetCurrentUrbanBlock() )
        return firerUrbanRole.ComputeDistanceInsideSameUrbanBlock( const_cast< MIL_Agent_ABC& >( target ) );
    else
    {
        geometry::Point2f realFirerPosition = firerUrbanRole.GetFirerPosition( const_cast< MIL_Agent_ABC& >( target ) );
        geometry::Point2f realTargetPosition = targetUrbanRole.GetTargetPosition( const_cast< MIL_AgentPion& >( *this ) );
        
        MT_Vector3D vFirerPosition(  realFirerPosition.X(), realFirerPosition.Y(), firerLocation.GetAltitude() );
        MT_Vector3D vTargetPosition(  realTargetPosition.X(), realTargetPosition.Y(), targetLocation.GetAltitude() );
        return vFirerPosition.Distance( vTargetPosition );
    }
}
