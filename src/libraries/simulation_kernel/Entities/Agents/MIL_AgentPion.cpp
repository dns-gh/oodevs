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
#include "Actions/CrowdTransport/PHY_RoleAction_CrowdTransport.h"
#include "Actions/Emergency/PHY_RoleAction_FolkInfluence.h"
#include "Actions/Firing/DirectFiring/PHY_RoleAction_DirectFiring.h"
#include "Actions/Firing/IndirectFiring/PHY_RoleAction_IndirectFiring.h"
#include "Actions/Flying/PHY_RoleAction_Flying.h"
#include "Actions/Loading/PHY_RoleAction_Loading.h"
#include "Actions/Moving/PHY_RoleAction_InterfaceMoving.h"
#include "Actions/Objects/PHY_RoleAction_Objects.h"
#include "Actions/Transport/PHY_RoleAction_Transport.h"
#include "Actions/Underground/PHY_RoleAction_MovingUnderground.h"
#include "Adapters/RoleAdapterInterface.h"
#include "Decision/DEC_Model_ABC.h"
#include "Decision/DEC_PathFind_Manager.h"
#include "Decision/DEC_Representations.h"
#include "Decision/DEC_Workspace.h"
#include "Entities/Agents/Actions/Firing/PHY_FireResults_Pion.h"
#include "Entities/Agents/Units/Categories/PHY_NatureLevel.h"
#include "Entities/Agents/Units/Dotations/PHY_AmmoDotationClass.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Agents/Units/HumanFactors/PHY_Experience.h"
#include "Entities/Agents/Units/HumanFactors/PHY_Morale.h"
#include "Entities/Agents/Units/HumanFactors/PHY_Stress.h"
#include "Entities/Agents/Units/HumanFactors/PHY_Tiredness.h"
#include "Entities/Agents/Units/Humans/PHY_HumanRank.h"
#include "Entities/Agents/Units/Humans/PHY_HumanWound.h"
#include "Entities/Agents/Units/Logistic/PHY_BreakdownType.h"
#include "Entities/Agents/Units/Postures/PHY_Posture.h"
#include "Entities/Agents/Units/PHY_UnitType.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/MIL_Army_ABC.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Orders/MIL_Report.h"
#include "Entities/Specialisations/LOG/LogisticHierarchy_ABC.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"
#include "Knowledge/DEC_KS_DirectFire.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "MIL_AgentServer.h"
#include "MissionController_ABC.h"
#include "MT_Tools/MT_FormatString.h"
#include "NetworkNotificationHandler_ABC.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "protocol/MessageParameters.h"
#include "Roles/Communications/PHY_RolePion_Communications.h"
#include "Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Roles/Decision/DEC_RolePion_Decision.h"
#include "Roles/Deployment/PHY_RoleInterface_Deployment.h"
#include "Roles/Dotations/PHY_RolePion_Dotations.h"
#include "Roles/HumanFactors/PHY_RolePion_HumanFactors.h"
#include "Roles/Humans/PHY_RolePion_Humans.h"
#include "Roles/Illumination/PHY_RolePion_Illumination.h" // LTO
#include "Roles/Location/PHY_RoleInterface_Location.h"
#include "Roles/Logistic/PHY_RoleInterface_Maintenance.h"
#include "Roles/Logistic/PHY_RoleInterface_Medical.h"
#include "Roles/Logistic/PHY_RoleInterface_Supply.h"
#include "Roles/NBC/PHY_RolePion_NBC.h"
#include "Roles/Network/NET_RolePion_Dotations.h"
#include "Roles/Perception/PHY_RoleInterface_Perceiver.h"
#include "Roles/Population/PHY_RolePion_Population.h"
#include "Roles/Posture/PHY_RolePion_Posture.h"
#include "Roles/Protection/PHY_RolePion_ActiveProtection.h"
#include "Roles/Refugee/PHY_RolePion_Refugee.h"
#include "Roles/Reinforcement/PHY_RolePion_Reinforcement.h"
#include "Roles/Surrender/PHY_RolePion_Surrender.h"
#include "Roles/Terrain/PHY_RolePion_TerrainAnalysis.h"
#include "Roles/Transported/PHY_RolePion_Transported.h"
#include "Roles/Urban/PHY_RolePion_UrbanLocation.h"
#include "Tools/MIL_AffinitiesMap.h"
#include "Tools/MIL_Color.h"
#include "Tools/MIL_DictionaryExtensions.h"
#include "Tools/MIL_HumanRepartition.h"
#include "Tools/MIL_IDManager.h"
#include "Tools/MIL_MessageParameters.h"
#include "Tools/MIL_Tools.h"
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>
#include <tuple>
#include <vector>

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_AgentPion )

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion constructor
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_AgentPion::MIL_AgentPion( const MIL_AgentTypePion& type,
                              MissionController_ABC& controller,
                              MIL_Automate& automate,
                              xml::xistream& xis )
    : MIL_Agent_ABC( xis )
    , pType_               ( &type )
    , bHasChanged_         ( false )
    , markedForDestruction_( false )
    , brainDeleted_        ( false )
    , pAutomate_           ( &automate )
    , pKnowledgeBlackBoard_( new DEC_KnowledgeBlackBoard_AgentPion( *this ) )
    , pOrderManager_       ( new MIL_PionOrderManager( controller, *this ) )
    , pAffinities_         ( new MIL_AffinitiesMap( xis ) )
    , pExtensions_         ( new MIL_DictionaryExtensions( xis ) )
    , pColor_              ( new MIL_Color( xis ) )
    , app6Symbol_          ( "" )
    , level_               ( "" )
    , teleported_          ( false )
{
    automate.RegisterPion( *this );
    xis >> xml::optional
            >> xml::start( "critical-intelligence" ) // $$$$ ABR 2011-05-12: Factorize critical intelligence like affinities or extensions
                >> xml::attribute( "content", criticalIntelligence_ )
            >> xml::end;
    bool isPC( xis.attribute< bool >( "command-post", false ) );
    SetPionAsCommandPost( isPC );
    xis >> xml::optional
            >> xml::attribute( "level", level_ );
    bool bOverridenSymbol = xis.attribute< bool >( "overridden-symbol", false );
    if( bOverridenSymbol && xis.has_attribute( "nature" ) )
        xis >> xml::attribute( "nature", app6Symbol_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion constructor
// Created: LDC 2010-02-22
// -----------------------------------------------------------------------------
MIL_AgentPion::MIL_AgentPion( const MIL_AgentTypePion& type,
                              MissionController_ABC& controller )
    : MIL_Agent_ABC( type.GetName() )
    , pType_               ( &type )
    , bHasChanged_         ( false )
    , markedForDestruction_( false )
    , brainDeleted_        ( false )
    , pAutomate_           ( 0 )
    , pKnowledgeBlackBoard_( new DEC_KnowledgeBlackBoard_AgentPion( *this ) )
    , pOrderManager_       ( new MIL_PionOrderManager( controller, *this ) )
    , pAffinities_         ( 0 )
    , pExtensions_         ( 0 )
    , pColor_              ( 0 )
    , app6Symbol_          ( "" )
    , level_               ( "" )
    , teleported_          ( false )
{
    // NOTHING
}

namespace
{
    template< typename R >
    R& LoadRole( MIL_CheckPointInArchive& archive, tools::RoleContainer& container )
    {
        R* role;
        archive >> role;
        if( !role )
            throw MASA_EXCEPTION( "Failed to load role " + std::string( typeid( role ).name() ) );
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
    const MissionController_ABC* const controller = &pion->GetController();
    archive << nTypeID
            << controller;
}

template< typename Archive >
void load_construct_data( Archive& archive, MIL_AgentPion* pion, const unsigned int /*version*/ )
{
    unsigned int nTypeID;
    MissionController_ABC* controller = 0;
    archive >> nTypeID
            >> controller;
    const MIL_AgentTypePion* pType = MIL_AgentTypePion::Find( nTypeID );
    assert( pType );
    ::new( pion ) MIL_AgentPion( *pType, *controller );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::load
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_AgentPion::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    MIL_AffinitiesMap* pAffinities;
    MIL_DictionaryExtensions* pExtensions;
    MIL_Color* pColor;
    file >> boost::serialization::base_object< MIL_Agent_ABC >( *this );
    file >> pAutomate_
         >> pKnowledgeBlackBoard_
         >> pAffinities
         >> pExtensions
         >> pColor
         >> criticalIntelligence_
         >> app6Symbol_
         >> level_;
    LoadRole< network::NET_RolePion_Dotations >( file, *this );
    LoadRole< PHY_RolePion_Reinforcement >( file, *this );
    LoadRole< PHY_RolePion_Posture >( file, *this );
    LoadRole< PHY_RoleInterface_Location >( file, *this );
    LoadRole< PHY_RolePion_UrbanLocation >( file, *this );
    LoadRole< dotation::PHY_RolePion_Dotations >( file, *this );
    LoadRole< human::PHY_RolePion_Humans >( file, *this );
    LoadRole< PHY_RoleInterface_Composantes >( file, *this );
    LoadRole< PHY_RolePion_ActiveProtection >( file, *this );
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
    LoadRole< crowdtransport::PHY_RoleAction_CrowdTransport >( file, *this );
    LoadRole< moving::PHY_RoleAction_InterfaceMoving >( file, *this );
    LoadRole< PHY_RoleAction_Objects >( file, *this );
    LoadRole< firing::PHY_RoleAction_DirectFiring >( file, *this );
    LoadRole< firing::PHY_RoleAction_IndirectFiring >( file, *this );
    LoadRole< DEC_RolePion_Decision >( file, *this );
    LoadRole< PHY_RoleInterface_Perceiver >( file, *this );
    LoadRole< PHY_RoleAction_InterfaceFlying >( file, *this );
    LoadRole< PHY_RoleAction_FolkInfluence >( file, *this );
    LoadRole< PHY_RolePion_Illumination >( file, *this ); // LTO
    LoadRole< PHY_RoleAction_MovingUnderground >( file, *this );
    LoadRole< PHY_RoleInterface_Deployment >( file, *this );
    RegisterRole( *new DEC_Representations() );
    RegisterRole( *new PHY_RolePion_TerrainAnalysis( *this ) );
    pAffinities_.reset( pAffinities );
    pExtensions_.reset( pExtensions );
    pColor_.reset( pColor );
    LoadRole< sword::RoleAdapterInterface >( file, *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::save
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_AgentPion::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    assert( pType_ );
    const MIL_AffinitiesMap* const pAffinities = pAffinities_.get();
    const MIL_DictionaryExtensions* const pExtensions = pExtensions_.get();
    const MIL_Color* const pColor = pColor_.get();
    file << boost::serialization::base_object< MIL_Agent_ABC >( *this );
    file << pAutomate_
        << pKnowledgeBlackBoard_
        << pAffinities
        << pExtensions
        << pColor
        << criticalIntelligence_
        << app6Symbol_
        << level_;
    SaveRole< network::NET_RolePion_Dotations >( *this, file );
    SaveRole< PHY_RolePion_Reinforcement >( *this, file );
    SaveRole< PHY_RolePion_Posture >( *this, file );
    SaveRole< PHY_RoleInterface_Location >( *this, file );
    SaveRole< PHY_RolePion_UrbanLocation >( *this, file );
    SaveRole< dotation::PHY_RolePion_Dotations >( *this, file );
    SaveRole< human::PHY_RolePion_Humans >( *this, file );
    SaveRole< PHY_RoleInterface_Composantes >( *this, file );
    SaveRole< PHY_RolePion_ActiveProtection >( *this, file );
    SaveRole< nbc::PHY_RolePion_NBC >( *this, file );
    SaveRole< PHY_RolePion_Communications >( *this, file );
    SaveRole< PHY_RolePion_HumanFactors >( *this, file );
    SaveRole< transport::PHY_RolePion_Transported >( *this, file );
    //SaveRole< PHY_RolePion_Maintenance >( *this, file );//@TODO refactor with new save
    //SaveRole< PHY_RolePion_Medical >( *this, file );
    //SaveRole< PHY_RolePion_Supply >( *this, file );
    SaveRole< surrender::PHY_RolePion_Surrender >( *this, file );
    SaveRole< refugee::PHY_RolePion_Refugee >( *this, file );
    SaveRole< PHY_RolePion_Population >( *this, file );
    SaveRole< transport::PHY_RoleAction_Loading >( *this, file );
    SaveRole< transport::PHY_RoleAction_Transport >( *this, file );
    SaveRole< crowdtransport::PHY_RoleAction_CrowdTransport >( *this, file );
    SaveRole< moving::PHY_RoleAction_InterfaceMoving >( *this, file );
    SaveRole< PHY_RoleAction_Objects >( *this, file );
    SaveRole< firing::PHY_RoleAction_DirectFiring >( *this, file );
    SaveRole< firing::PHY_RoleAction_IndirectFiring >( *this, file );
    SaveRole< DEC_RolePion_Decision >( *this, file );
    SaveRole< PHY_RoleInterface_Perceiver >( *this, file );
    SaveRole< PHY_RoleAction_InterfaceFlying >( *this, file );
    SaveRole< PHY_RoleAction_FolkInfluence >( *this, file );
    SaveRole< PHY_RolePion_Illumination >( *this, file ); // LTO
    SaveRole< PHY_RoleAction_MovingUnderground >( *this, file );
    SaveRole< PHY_RoleInterface_Deployment >( *this, file );
    SaveRole< sword::RoleAdapterInterface >( *this, file );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::WriteODB
// Created: NLD 2006-05-29
// -----------------------------------------------------------------------------
void MIL_AgentPion::WriteODB( xml::xostream& xos ) const
{
    if( !GetType().IsWrittenInODB() )
        return;
    xos << xml::start( "unit" );
    MIL_Entity_ABC::WriteODB( xos ) ;
    xos << xml::attribute( "id", GetID() )
        << xml::attribute( "type", pType_->GetName() )
        << xml::attribute( "command-post", IsPC() )
        << xml::attribute( "position", MIL_Tools::ConvertCoordSimToMos( GetRole< PHY_RoleInterface_Location >().GetPosition() ) );
    if( !level_.empty() )
        xos << xml::attribute( "level", level_ );
    if( !app6Symbol_.empty() )
        xos << xml::attribute( "overridden-symbol", true )
            << xml::attribute( "nature", app6Symbol_ );
    pColor_->WriteODB( xos );
    CallRole( &PHY_RolePion_Composantes::WriteODB, xos );
    CallRole( &human::PHY_RolePion_Humans::WriteODB, xos );
    CallRole( &dotation::PHY_RolePion_Dotations::WriteODB, xos );
    CallRole( &PHY_RoleInterface_Supply::WriteODB, xos );
    if( criticalIntelligence_ != "" )
    {
        xos << xml::start( "critical-intelligence" )
                << xml::attribute( "content", criticalIntelligence_ )
            << xml::end;
    }
    pAffinities_->WriteODB( xos );
    pExtensions_->WriteODB( xos );
    xos << xml::end;// unit
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::ReadOverloading
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
void MIL_AgentPion::ReadOverloading( xml::xistream& xis )
{
    // Dotations overloaded by ODB
    CallRole( &PHY_RolePion_Composantes::ReadOverloading, xis ); // Equipments + Humans
    CallRole( &dotation::PHY_RoleInterface_Dotations::ReadOverloading, xis );
    CallRole( &PHY_RoleInterface_HumanFactors::ReadOverloading, xis );
    CallRole( &PHY_RoleInterface_Supply::ReadOverloading, xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::Finalize
// Created: JSR 2013-01-22
// -----------------------------------------------------------------------------
void MIL_AgentPion::Finalize()
{
    CallRole( &PHY_RolePion_Composantes::Finalize );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion destructor
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_AgentPion::~MIL_AgentPion()
{
    delete pKnowledgeBlackBoard_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::IsMarkedForDestruction
// Created: JSR 2013-01-29
// -----------------------------------------------------------------------------
bool MIL_AgentPion::IsMarkedForDestruction() const
{
    return markedForDestruction_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::IsDead
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
bool MIL_AgentPion::IsDead() const
{
    return !CallRole( &PHY_RoleInterface_Composantes::IsUsable, true );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::IsNeutralized
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
bool MIL_AgentPion::IsNeutralized() const
{
    return CallRole( &PHY_RoleInterface_Composantes::IsNeutralized, false) ;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::UpdateUnderIndirectFire
// Created: LMT 2012-06-13
// -----------------------------------------------------------------------------
bool MIL_AgentPion::UpdateUnderIndirectFire()
{
    bool returnValue = false;
    PHY_RolePion_Composantes* role = RetrieveRole< PHY_RolePion_Composantes >();
    if( role )
    {
        returnValue = role->IsUnderIndirectFire();
        role->ResetUnderIndirectFire();
    }
    return returnValue;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::UpdateKnowledges
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
void MIL_AgentPion::UpdateKnowledges( int currentTimeStep )
{
    assert( pKnowledgeBlackBoard_ );
    pKnowledgeBlackBoard_->Update( currentTimeStep );
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
// Name: MIL_AgentPion::CleanDeletedAgentKnowledges
// Created: JSR 2013-02-21
// -----------------------------------------------------------------------------
void MIL_AgentPion::CleanDeletedAgentKnowledges()
{
    pKnowledgeBlackBoard_->CleanDeletedAgentKnowledges();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::SetPionAsCommandPost
// Created: HBD 2011-02-21
// -----------------------------------------------------------------------------
void MIL_AgentPion::SetPionAsCommandPost( bool pc )
{
    if( pc )
        pAutomate_->SetCommandPost( this );
    else if( IsPC() )
    {
        pAutomate_->SetCommandPost( 0 );
        NotifySendHeadquarters();
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::NotifySendHeadquarters
// Created: LDC 2012-05-03
// -----------------------------------------------------------------------------
void MIL_AgentPion::NotifySendHeadquarters()
{
    bHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::UpdateDecision
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
void MIL_AgentPion::UpdateDecision( float duration )
{
    if( !RetrieveRole< DEC_Decision_ABC >() )
        return;

    if( markedForDestruction_ )
    {
        if( !brainDeleted_ )
        {
            GetRole< DEC_RolePion_Decision >().DeleteBrain();
            pAutomate_->GetRole< DEC_AutomateDecision >().GarbageCollect();
            brainDeleted_ = true;
        }
        return;
    }

    if( GetRole< PHY_RolePion_Composantes >().IsImmobilized() )
        pOrderManager_->CancelMission();
    try
    {
        pOrderManager_->Update();
        if( !IsDead() )
            GetRole< DEC_Decision_ABC >().UpdateDecision( duration );
        else
            CancelAllActions();
    }
    catch( const std::exception& e )
    {
        DEC_Decision_ABC* role = RetrieveRole< DEC_Decision_ABC >();
        if( role )
            role->LogError( &e );
        MIL_Report::PostEvent( *this, report::eRC_MissionImpossible );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::UpdatePhysicalState
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void MIL_AgentPion::UpdatePhysicalState()
{
    if( markedForDestruction_ )
        return;
    try
    {
        const bool bIsDead = IsDead();
        CallRole( &dotation::PHY_RolePion_Dotations::Update, bIsDead );
        CallRole( &human::PHY_RolePion_Humans::Update, bIsDead );
        CallRole( &PHY_RolePion_Composantes::Update, bIsDead );
        CallRole( &PHY_RolePion_Posture::Update, bIsDead );
        CallRole( &PHY_RolePion_Reinforcement::Update, bIsDead );
        CallRole( &PHY_RoleInterface_Location::Update, bIsDead );
        CallRole( &nbc::PHY_RolePion_NBC::Update, bIsDead );
        CallRole( &PHY_RolePion_Communications::Update, bIsDead );
        CallRole( &PHY_RolePion_HumanFactors::Update, bIsDead );
        CallRole( &transport::PHY_RolePion_Transported::Update, bIsDead );
        CallRole( &surrender::PHY_RolePion_Surrender::Update, bIsDead );
        CallRole( &refugee::PHY_RolePion_Refugee::Update, bIsDead );
        CallRole( &PHY_RolePion_Population::Update, bIsDead );
        CallRole( &PHY_RoleInterface_Perceiver::Update, bIsDead ); // Doit être après PHY_RolePion_Composantes $$$ pourri - utiliser des observers
        CallRole( &transport::PHY_RoleAction_Loading::Update, bIsDead );
        CallRole( &transport::PHY_RoleAction_Transport::Update, bIsDead );
        CallRole( &crowdtransport::PHY_RoleAction_CrowdTransport::Update, bIsDead );
        CallRole( &PHY_RoleAction_Objects::Update, bIsDead );
        CallRole( &moving::PHY_RoleAction_InterfaceMoving::Update, bIsDead );
        CallRole( &PHY_RoleAction_InterfaceFlying::Update, bIsDead );
        CallRole( &firing::PHY_RoleAction_DirectFiring::Update, bIsDead );
        CallRole( &firing::PHY_RoleAction_IndirectFiring::Update, bIsDead );
        CallRole( &PHY_RoleAction_FolkInfluence::Update, bIsDead );
        CallRole( &PHY_RoleAction_MovingUnderground::Update, bIsDead );
        CallRole( &PHY_RoleInterface_Deployment::Update, bIsDead );
        CallRole( &PHY_RoleInterface_Illumination::Update, bIsDead );
        CallRole( &PHY_RoleInterface_Maintenance::Update, bIsDead );  //@TODO add update to new role interface
        CallRole( &PHY_RoleInterface_Medical::Update, bIsDead );
        CallRole( &PHY_RoleInterface_Supply::Update, bIsDead );

    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Error updating unit " << GetID() << " : " << tools::GetExceptionMsg( e ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::UpdateState
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
void MIL_AgentPion::UpdateState()
{
    UpdatePhysicalState();
    ResetTeleported();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::UpdateNetwork
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void MIL_AgentPion::UpdateNetwork()
{
    if( markedForDestruction_ )
        return;
    try
    {
        CallRole( &network::NET_RolePion_Dotations::SendChangedState, 0 );
        CallRole( &network::NET_RolePion_Dotations::Clean );
        if( bHasChanged_ || pAffinities_->HasChanged() || pExtensions_->HasChanged() )
        {
            client::UnitAttributes msg;
            msg().mutable_unit()->set_id( GetID() );
            pAffinities_->UpdateNetwork( msg );
            pExtensions_->UpdateNetwork( msg );
            if( bHasChanged_ )
                msg().set_headquarters( IsPC() );
            msg.Send( NET_Publisher_ABC::Publisher() );
            bHasChanged_ = false;
        }
    }
    catch( const std::exception & e )
    {
        MT_LOG_ERROR_MSG( "Error updating network for unit " << GetID() << " : " << tools::GetExceptionMsg( e ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::PreprocessRandomBreakdowns
// Created: NLD 2005-01-06
// -----------------------------------------------------------------------------
void MIL_AgentPion::PreprocessRandomBreakdowns( unsigned int nEndDayTimeStep ) const
{
    CallRole( &PHY_RolePion_Composantes::PreprocessRandomBreakdowns, nEndDayTimeStep );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::Clean
// Created: AGE 2004-11-23
// -----------------------------------------------------------------------------
void MIL_AgentPion::Clean()
{
    CallRole( &PHY_RoleInterface_Location::Clean );
    CallRole( &PHY_RoleInterface_Perceiver::Clean );
    CallRole( &dotation::PHY_RolePion_Dotations::Clean );
    CallRole( &human::PHY_RolePion_Humans::Clean );
    CallRole( &PHY_RolePion_Composantes::Clean );
    CallRole( &PHY_RolePion_Posture::Clean );
    CallRole( &PHY_RolePion_Reinforcement::Clean );
    CallRole( &nbc::PHY_RolePion_NBC::Clean );
    CallRole( &PHY_RolePion_Communications::Clean );
    CallRole( &PHY_RolePion_HumanFactors::Clean );
    CallRole( &surrender::PHY_RoleInterface_Surrender::Clean );
    CallRole( &refugee::PHY_RolePion_Refugee::Clean );
    CallRole( &PHY_RolePion_Population::Clean );
    CallRole( &transport::PHY_RoleInterface_Transported::Clean );
    CallRole( &transport::PHY_RoleAction_Transport::Clean );
    CallRole( &crowdtransport::PHY_RoleAction_CrowdTransport::Clean );
    CallRole( &transport::PHY_RoleAction_Loading::Clean );
    CallRole( &PHY_RoleAction_Objects::Clean );
    CallRole( &moving::PHY_RoleAction_InterfaceMoving::Clean );
    CallRole( &PHY_RoleAction_InterfaceFlying::Clean );
    CallRole( &firing::PHY_RoleAction_DirectFiring::Clean );
    CallRole( &firing::PHY_RoleAction_IndirectFiring::Clean );
    CallRole( &DEC_RolePion_Decision::Clean );
    CallRole( &PHY_RoleAction_FolkInfluence::Clean );
    CallRole( &PHY_RoleAction_MovingUnderground::Clean );
    CallRole( &PHY_RoleInterface_Maintenance::Clean );//@TODO Add an interface for role with clean, update
    CallRole( &PHY_RoleInterface_Medical::Clean );
    CallRole( &PHY_RoleInterface_Supply::Clean );
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
// Name: MIL_AgentPion::IsJammed
// Created: MMC 2013-04-24
// -----------------------------------------------------------------------------
bool MIL_AgentPion::IsJammed() const
{
    if( const PHY_RolePion_Communications* pCommunications = RetrieveRole< PHY_RolePion_Communications >() )
        return pCommunications->IsJammed();
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::IsLogisticJammed
// Created: MMC 2013-04-24
// -----------------------------------------------------------------------------
bool MIL_AgentPion::IsLogisticJammed() const
{
    MIL_AutomateLOG* pTC2 = GetLogisticHierarchy().GetPrimarySuperior();
    return pTC2 && pTC2->GetPC() && pTC2->GetPC()->IsJammed();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::HasBeenTeleported
// Created: MMC 2013-07-24
// -----------------------------------------------------------------------------
bool MIL_AgentPion::HasBeenTeleported() const
{
    return teleported_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::IsMasaLife
// Created: MMC 2013-08-01
// -----------------------------------------------------------------------------
bool MIL_AgentPion::IsMasaLife() const
{
    const DEC_RolePion_Decision* roleDec = RetrieveRole< DEC_RolePion_Decision >();
    return roleDec && roleDec->GetModel().IsMasalife();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::HasBeenTeleported
// Created: MMC 2013-07-24
// -----------------------------------------------------------------------------
void MIL_AgentPion::ResetTeleported()
{
    teleported_ = false;
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
boost::shared_ptr< MIL_KnowledgeGroup > MIL_AgentPion::GetKnowledgeGroup() const
{
    if( !CallRole( &PHY_RoleInterface_Communications::CanEmit, true ) )
    {
        boost::shared_ptr< MIL_KnowledgeGroup > jamKg = CallRole( &PHY_RolePion_Communications::GetJammedKnowledgeGroup, boost::shared_ptr< MIL_KnowledgeGroup >() );
        if( jamKg )
            return jamKg;
    }
    if( !pAutomate_ )
        throw MASA_EXCEPTION( MT_FormatString( "Automate is undefined for agent id %d ", GetID() ) );
    return pAutomate_->GetKnowledgeGroup();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::BelongsTo
// Created: AGE 2004-11-23
// -----------------------------------------------------------------------------
bool MIL_AgentPion::BelongsTo( const MIL_KnowledgeGroup& group ) const
{
    return *GetKnowledgeGroup() == group;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::CreateKnowledge
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Agent > MIL_AgentPion::CreateKnowledge( const MIL_KnowledgeGroup& group )
{
    return boost::make_shared< DEC_Knowledge_Agent >( group, boost::ref( *this ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::SendCreation
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MIL_AgentPion::SendCreation( unsigned int nCtx ) const
{
    assert( pType_ );
    client::UnitCreation creationMsg;
    creationMsg().mutable_unit()->set_id( GetID() );
    creationMsg().mutable_type()->set_id( pType_->GetID() );
    creationMsg().set_name( GetName() );
    creationMsg().mutable_automat()->set_id( GetAutomate().GetID() );
    creationMsg().set_pc( IsPC() );
    pType_->GetHumanRepartition().SendFullState( creationMsg );
    pColor_->SendFullState( creationMsg );
    if( !level_.empty() )
        if( const PHY_NatureLevel* nature = PHY_NatureLevel::Find( level_ ) )
            creationMsg().set_level( nature->GetAsnID() );
    if( !app6Symbol_.empty() )
        creationMsg().set_app6symbol( app6Symbol_ );
    pExtensions_->SendFullState( creationMsg );
    creationMsg.Send( NET_Publisher_ABC::Publisher(), nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::SendFullState
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void MIL_AgentPion::SendFullState( unsigned int nCtx ) const
{
    if( markedForDestruction_ )
        return;
    bool critical = !criticalIntelligence_.empty();
    if( critical || !pExtensions_->IsEmpty() || !pAffinities_->IsEmpty() )
    {
        client::UnitAttributes msg;
        msg().mutable_unit()->set_id( GetID() );
        if( critical )
            msg().set_critical_intelligence( criticalIntelligence_ );
        pAffinities_->SendFullState( msg );
        pExtensions_->SendFullState( msg );
        msg.Send( NET_Publisher_ABC::Publisher(), nCtx );
    }
    CallRole( &network::NET_RolePion_Dotations::SendFullState, nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::SendKnowledge
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void MIL_AgentPion::SendKnowledge( unsigned int nCtx ) const
{
    assert( pKnowledgeBlackBoard_ );
    pKnowledgeBlackBoard_->SendFullState( nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveOrder
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
uint32_t MIL_AgentPion::OnReceiveOrder( const sword::UnitOrder& msg )
{
    return pOrderManager_->OnReceiveMission( msg );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveFragOrder
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveFragOrder( const sword::FragOrder& msg, const std::function< void( uint32_t ) >& sendAck )
{
    pOrderManager_->OnReceiveFragOrder( msg, sendAck );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveDestroyComponent
// Created: AGE 2007-06-19
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveDestroyComponent()
{
    if( markedForDestruction_ )
        throw MASA_BADUNIT_UNIT( "invalid unit: " << GetID() );
    GetRole< PHY_RolePion_Composantes >().DestroyRandomComposante();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::DeleteUnit
// Created: JSR 2013-02-19
// -----------------------------------------------------------------------------
void MIL_AgentPion::DeleteUnit( unsigned int nCtx, unsigned int clientId )
{
    if( markedForDestruction_ )
        return;
    CancelCurrentMission();
    pOrderManager_->StopAllMissions();
    MIL_AgentServer::GetWorkspace().GetPathFindManager().CancelJobForUnit( this );

    GetType().DeleteUnit( *this );

    GetRole< PHY_RoleInterface_Location >().RemoveFromPatch();
    GetRole< PHY_RolePion_Composantes >().RetrieveAllLentComposantes();
    GetRole< PHY_RolePion_Composantes >().ReturnAllBorrowedComposantes();
    GetRole< PHY_RolePion_Composantes >().CancelLogisticRequests();
    GetRole< transport::PHY_RoleAction_Transport >().Cancel();

    if( pAutomate_ )
    {
        pAutomate_->DeleteRequestsForRequester( *this );
        pAutomate_->UnregisterPion( *this );
        const MIL_AutomateLOG* superior = GetLogisticHierarchy().GetPrimarySuperior();
        if( superior && superior->GetPC() )
            MIL_Report::PostEvent( *superior->GetPC(), report::eRC_LogSuperiorRemoved, *this );
    }

    markedForDestruction_ = true;

    MIL_AgentServer::GetWorkspace().GetEntityManager().CleanDeletedAgentKnowledges();

    client::UnitDestruction msg;
    msg().mutable_unit()->set_id( GetID() );
    msg.Send( NET_Publisher_ABC::Publisher(), nCtx, clientId );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveDeleteUnit
// Created: JSR 2013-01-29
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveDeleteUnit( unsigned int nCtx, unsigned int clientId )
{
    DeleteUnit( nCtx, clientId );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::MagicMove
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void MIL_AgentPion::MagicMove( const MT_Vector2D& vNewPos )
{
    CallRole( &PHY_RoleAction_MovingUnderground::GetOutFromUndergroundNetwork );
    CallRole( &PHY_RoleInterface_Location::MagicMove, vNewPos );
    CallRole( &PHY_RolePion_UrbanLocation::MagicMove, vNewPos );

    if( IsMasaLife() && GetAutomate().IsMasaLife() )
        teleported_ = true;
    else
        CancelCurrentMission();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveMagicActionMoveTo
// Created: NLD 2005-08-18
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveMagicActionMoveTo( const MT_Vector2D& vPosition )
{
    MagicMove( vPosition );
    UpdatePhysicalState();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveMagicActionMoveTo
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveMagicActionMoveTo( const sword::UnitMagicAction& asn )
{
    if( asn.type() != sword::UnitMagicAction::move_to )
        throw MASA_EXCEPTION_ASN( sword::UnitActionAck_ErrorCode, sword::UnitActionAck::error_invalid_parameter );
    if( !asn.has_parameters() || asn.parameters().elem_size() != 1)
        throw MASA_EXCEPTION_ASN( sword::UnitActionAck_ErrorCode, sword::UnitActionAck::error_invalid_parameter );
    const sword::MissionParameter& parametre = asn.parameters().elem( 0 );
    if( !parametre.value_size() == 1 || !parametre.value(0).has_point() )
        throw MASA_EXCEPTION_ASN( sword::UnitActionAck_ErrorCode, sword::UnitActionAck::error_invalid_parameter );
    const sword::Point& point = parametre.value(0).point();
    if( point.location().type() != sword::Location::point  || point.location().coordinates().elem_size() != 1 )
        throw MASA_EXCEPTION_ASN( sword::UnitActionAck_ErrorCode, sword::UnitActionAck::error_invalid_parameter );
    MT_Vector2D vPosTmp;
    MIL_Tools::ConvertCoordMosToSim( point.location().coordinates().elem(0), vPosTmp );
    MagicMove( vPosTmp );
    UpdatePhysicalState();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveChangeHumanFactors
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
void  MIL_AgentPion::OnReceiveChangeHumanFactors( const sword::MissionParameters& msg )
{
    protocol::CheckCount( msg, 4 );

    const auto tiredness = GET_ENUMERATION( sword::UnitAttributes::EnumUnitTiredness, msg, 0 );
    const PHY_Tiredness* pTiredness = PHY_Tiredness::Find( tiredness );
    protocol::Check( pTiredness, "must be a valid tiredness", 0 );

    const auto morale = GET_ENUMERATION( sword::UnitAttributes::EnumUnitMorale, msg, 1 );
    const PHY_Morale* pMoral = PHY_Morale::Find( morale );
    protocol::Check( pMoral, "must be a valid morale", 1 );

    const auto experience = GET_ENUMERATION( sword::UnitAttributes::EnumUnitExperience, msg, 2 );
    const PHY_Experience* pExperience = PHY_Experience::Find( experience );
    protocol::Check( pExperience, "must be a valid experience", 2 );

    const auto stress = GET_ENUMERATION( sword::UnitAttributes::EnumUnitStress, msg, 3 );
    const PHY_Stress* pStress = PHY_Stress::Find( stress );
    protocol::Check( pStress, "must be a valid stress", 3 );

    auto& role = GetRole< PHY_RolePion_HumanFactors >();
    role.SetTiredness( *pTiredness, true );
    role.SetMorale( *pMoral );
    role.SetExperience( *pExperience );
    role.SetStress( *pStress, true );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveResupplyHumans
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveResupplyHumans( bool withLog )
{
    if( markedForDestruction_ )
        throw MASA_BADUNIT_UNIT( "invalid unit: " << GetID() );
    GetRole< human::PHY_RolePion_Humans >().HealAllHumans( withLog );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveResupplyResources
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveResupplyResources( bool withLog )
{
    if( markedForDestruction_ )
        throw MASA_BADUNIT_UNIT( "invalid unit: " << GetID() );
    GetRole< dotation::PHY_RolePion_Dotations >().ResupplyDotations( withLog );
    PHY_RoleInterface_Supply* role = RetrieveRole< PHY_RoleInterface_Supply >();
    if( role )
        role->ResupplyStocks( withLog );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveResupplyEquipement
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveResupplyEquipement( bool withLog )
{
    if( markedForDestruction_ )
        throw MASA_BADUNIT_UNIT( "invalid unit: " << GetID() );
    GetRole< PHY_RolePion_Composantes >().RepairAllComposantes( withLog );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveResupplyAll
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveResupplyAll( bool withLog )
{
    if( markedForDestruction_ )
        throw MASA_BADUNIT_UNIT( "invalid unit: " << GetID() );
    GetRole< PHY_RolePion_Composantes >().RepairAllComposantes( withLog );
    GetRole< dotation::PHY_RolePion_Dotations >().ResupplyDotations( withLog );
    PHY_RoleInterface_Supply* role = RetrieveRole< PHY_RoleInterface_Supply >();
    if( role )
        role->ResupplyStocks( withLog );
    GetRole< human::PHY_RolePion_Humans >().HealAllHumans( withLog );
    GetRole< nbc::PHY_RolePion_NBC >().Decontaminate();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveResupply
// Created: JSR 2010-04-15
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveResupply( const sword::MissionParameters& msg )
{
    protocol::CheckCount( msg, 5 );
    int idx = -1;
    if( !protocol::IsNull( msg, ++idx ) )
    {
        auto& composantes = GetRole< PHY_RolePion_Composantes >();
        for( int i = 0; i < protocol::GetCount( msg, idx ); ++i )
        {
            protocol::CheckCount( idx, i, msg, 2 );
            const auto id = protocol::GetIdentifier( msg, idx, i, 0 );
            const auto quantity = protocol::GetQuantity( msg, idx, i, 1 );
            const auto* type = PHY_ComposanteTypePion::Find( id );
            protocol::Check( type, "must be a valid composante type identifier", idx, i, 0 );
            composantes.ChangeComposantesAvailability( *type, quantity );
        }
    }
    if( !protocol::IsNull( msg, ++idx ) )
    {
        auto& humans = GetRole< human::PHY_RolePion_Humans >();
        for( int i = 0; i < protocol::GetCount( msg, idx ); ++i )
        {
            protocol::CheckCount( idx, i, msg, 2 );
            const auto id = protocol::GetIdentifier( msg, idx, i, 0 );
            const auto quantity = protocol::GetQuantity( msg, idx, i, 1 );
            const auto* rank = PHY_HumanRank::Find( id );
            protocol::Check( rank, "must be a valid human rank identifier", idx, i, 0 );
            humans.ChangeHumansAvailability( *rank, quantity );
        }
    }
    if( !protocol::IsNull( msg, ++idx ) )
    {
        auto& dotations = GetRole< dotation::PHY_RolePion_Dotations >();
        for( int i = 0; i < protocol::GetCount( msg, idx ); ++i )
        {
            protocol::CheckCount( idx, i, msg, 2 );
            const auto id = protocol::GetIdentifier( msg, idx, i, 0 );
            const auto quantity = protocol::GetQuantity( msg, idx, i, 1 );
            const auto* dotation = PHY_DotationType::FindDotationType( id );
            protocol::Check( dotation, "must be a valid dotation type identifier", idx, i, 0 );
            dotations.ResupplyDotations( *dotation, quantity / 100. );
        }
    }
    if( !protocol::IsNull( msg, ++idx ) )
    {
        auto& dotations = GetRole< dotation::PHY_RolePion_Dotations >();
        for( int i = 0; i < protocol::GetCount( msg, idx ); ++i )
        {
            protocol::CheckCount( idx, i, msg, 2 );
            const auto id = protocol::GetIdentifier( msg, idx, i, 0 );
            const auto quantity = protocol::GetQuantity( msg, idx, i, 1 );
            const auto* ammo = PHY_AmmoDotationClass::Find( id );
            protocol::Check( ammo, "must be a valid ammo type identifier", idx, i, 0 );
            dotations.ResupplyDotations( *ammo, quantity / 100. );
        }
    }
    auto* supply = RetrieveRole< PHY_RoleInterface_Supply >();
    if( supply && !protocol::IsNull( msg, ++idx ) )
        for( int i = 0; i < protocol::GetCount( msg, idx ); ++i )
        {
            protocol::CheckCount( idx, i, msg, 2 );
            const auto id = protocol::GetIdentifier( msg, idx, i, 0 );
            const auto quantity = protocol::GetQuantity( msg, idx, i, 1 );
            const auto* category = PHY_DotationType::FindDotationCategory( id );
            protocol::Check( category, "must be a valid dotation category identifier", idx, i, 0 );
            supply->ResupplyStocks( *category, quantity );
        }
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveDestroyAll
// Created: NLD 2004-12-07
// -----------------------------------------------------------------------------
void  MIL_AgentPion::OnReceiveDestroyAll()
{
    if( markedForDestruction_ )
        throw MASA_BADUNIT_UNIT( "invalid unit: " << GetID() );
    GetRole< PHY_RolePion_Composantes >().DestroyAllComposantes();
    GetRole< PHY_RolePion_Composantes >().KillAllHumans();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveRecoverHumansTransporters
// Created: NLD 2005-12-27
// -----------------------------------------------------------------------------
void  MIL_AgentPion::OnReceiveRecoverHumansTransporters()
{
    GetRole< transport::PHY_RoleInterface_Transported >().RecoverHumanTransporters();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveUnitMagicAction
// Created: JSR 2010-04-14
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveUnitMagicAction( const sword::UnitMagicAction& msg, const tools::Resolver< MIL_Army_ABC >& armies, unsigned int nCtx,
                                              unsigned int clientId )
{
    switch( msg.type() )
    {
    case sword::UnitMagicAction::surrender_to:
        pAutomate_->OnReceiveUnitMagicAction( msg, armies );
        return;
    case sword::UnitMagicAction::cancel_surrender:
        pAutomate_->OnReceiveUnitMagicAction( msg, armies );
        return;
    case sword::UnitMagicAction::recover_transporters:
        OnReceiveRecoverHumansTransporters();
        break;
    case sword::UnitMagicAction::destroy_component:
        OnReceiveDestroyComponent();
        break;
    case sword::UnitMagicAction_Type_recover_all:
        OnReceiveResupplyAll( false );
        break;
    case sword::UnitMagicAction_Type_recover_troops:
        OnReceiveResupplyHumans( false );
        break;
    case sword::UnitMagicAction_Type_recover_equipments:
        OnReceiveResupplyEquipement( false );
        break;
    case sword::UnitMagicAction_Type_recover_resources:
        OnReceiveResupplyResources( false );
        break;
    case sword::UnitMagicAction_Type_recover_all_except_log:
        OnReceiveResupplyAll( true );
        break;
    case sword::UnitMagicAction_Type_recover_troops_except_log:
        OnReceiveResupplyHumans( true );
        break;
    case sword::UnitMagicAction_Type_recover_equipments_except_log:
        OnReceiveResupplyEquipement( true );
        break;
    case sword::UnitMagicAction_Type_recover_resources_except_log:
        OnReceiveResupplyResources( true );
        break;
    case sword::UnitMagicAction::destroy_all:
        OnReceiveDestroyAll();
        break;
    case sword::UnitMagicAction::delete_unit:
        OnReceiveDeleteUnit( nCtx, clientId );
        break;
    case sword::UnitMagicAction::change_human_factors:
        OnReceiveChangeHumanFactors( msg.parameters() );
        break;
    case sword::UnitMagicAction::partial_recovery:
        OnReceiveResupply( msg.parameters() );
        break;
    case sword::UnitMagicAction::unit_change_affinities:
        pAffinities_->OnReceiveMsgChangeAffinities( msg );
        break;
    case sword::UnitMagicAction::change_extension:
        pExtensions_->OnReceiveMsgChangeExtensions( msg );
        break;
    case sword::UnitMagicAction::change_critical_intelligence:
        OnReceiveCriticalIntelligence( msg.parameters() );
        break;
    case sword::UnitMagicAction::reload_brain:
        OnReloadBrain( msg.parameters() );
        break;
    case sword::UnitMagicAction::change_brain_debug:
        OnChangeBrainDebug( msg.parameters() );
        break;
    case sword::UnitMagicAction::change_posture:
        OnChangePosture( msg.parameters() );
        break;
    case sword::UnitMagicAction::create_breakdowns:
        OnReceiveCreateBreakdowns( msg.parameters() );
        break;
    case sword::UnitMagicAction::create_wounds:
        OnReceiveCreateWounds( msg.parameters() );
        break;
    case sword::UnitMagicAction::change_equipment_state:
        OnReceiveChangeEquipmentState( msg.parameters() );
        break;
    case sword::UnitMagicAction::change_human_state:
        OnReceiveChangeHumanState( msg.parameters() );
        break;
    case sword::UnitMagicAction::change_dotation:
        OnReceiveChangeDotation( msg.parameters() );
        break;
    case sword::UnitMagicAction::create_direct_fire_order:
        OnReceiveCreateDirectFireOrder( msg.parameters() );
        break;
    case sword::UnitMagicAction::load_unit:
    case sword::UnitMagicAction::unload_unit:
        OnReceiveLoadUnit( msg.parameters(), msg.type() == sword::UnitMagicAction::load_unit );
        break;
    case sword::UnitMagicAction::log_finish_handlings:
        OnReceiveFinishLogisticHandlings();
        break;
    default:
        throw MASA_EXCEPTION_ASN( sword::UnitActionAck_ErrorCode,
            sword::UnitActionAck::error_invalid_unit );
    }
    UpdatePhysicalState();
}

void MIL_AgentPion::UpdateSurrenderedState()
{
    auto& role = GetRole< surrender::PHY_RoleInterface_Surrender >();
    if( role.UpdateSurrenderedState() )
    {
        if( role.IsSurrendered() )
            pOrderManager_->CancelMission();
        UpdatePhysicalState();
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveChangeSuperior
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveChangeSuperior( const MIL_EntityManager& manager, unsigned int automatId )
{
    MIL_Automate* pNewAutomate = manager.FindAutomate( automatId );
    if( !pNewAutomate )
        throw MASA_BADPARAM_ASN( sword::UnitActionAck::ErrorCode, sword::UnitActionAck::error_invalid_parameter,
            "parameters[0] must be an automat identifier" );
    if( pNewAutomate->GetArmy() != GetArmy() )
        throw MASA_BADPARAM_ASN( sword::UnitActionAck::ErrorCode, sword::UnitActionAck::error_invalid_parameter,
            "parameters[0] must be an automat in the same side" );

    ChangeSuperior( *pNewAutomate );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::ChangeSuperior
// Created: NLD 2004-10-26
// -----------------------------------------------------------------------------
void MIL_AgentPion::ChangeSuperior( MIL_Automate& newAutomate )
{
    if( pAutomate_ == &newAutomate )
        return;
    assert( GetArmy() == newAutomate.GetArmy() );
    SetPionAsCommandPost( false );
    const MIL_AutomateLOG* before = GetLogisticHierarchy().GetPrimarySuperior();
    pAutomate_->UnregisterPion( *this );
    pAutomate_ = &newAutomate;
    pAutomate_->RegisterPion( *this );
    UpdateSurrenderedState();
    const MIL_AutomateLOG* after = GetLogisticHierarchy().GetPrimarySuperior();
    if( before != after )
    {
        if( before && before->GetPC() )
            MIL_Report::PostEvent( *before->GetPC(), report::eRC_LogSuperiorRemoved, *this );
        if( after && after->GetPC() )
            MIL_Report::PostEvent( *after->GetPC(), report::eRC_LogSuperiorAdded, *this );
    }
    client::UnitChangeSuperior asnMsg;
    asnMsg().mutable_unit()->set_id( GetID() );
    asnMsg().mutable_parent()->set_id( newAutomate.GetID() );
    asnMsg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::CancelCurrentMission
// Created: JSR 2013-01-29
// -----------------------------------------------------------------------------
void MIL_AgentPion::CancelCurrentMission()
{
    CancelAllActions();
    DEC_RolePion_Decision* roleDec = RetrieveRole< DEC_RolePion_Decision >();
    if( roleDec )
        roleDec->Reset( GetAutomate().GetName() );
    pOrderManager_->CancelMission();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::NotifyAttackedBy
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
void MIL_AgentPion::NotifyAttackedBy( MIL_AgentPion& attacker, bool mustReport )
{
    if( mustReport )
    {
        if( attacker.GetType().IsRefugee() )
            MIL_Report::PostEvent( *this, report::eRC_TireParCivil );
        else if( GetArmy().IsNeutral( attacker.GetArmy() ) == eTristate_True )
            MIL_Report::PostEvent( *this, report::eRC_TireParCampNeutre );
        else if( GetArmy().IsAFriend( attacker.GetArmy() ) == eTristate_True )
            MIL_Report::PostEvent( *this, report::eRC_TireParCampAmi );
        else if( GetArmy().IsAnEnemy( attacker.GetArmy() ) == eTristate_True )
            MIL_Report::PostEvent( *this, report::eRC_TireParCampEnnemi );
    }
    GetKnowledge().GetKsDirectFire().NotifyAttackedBy( attacker );
    GetRole< PHY_RolePion_HumanFactors >().NotifyAttacked();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::NotifyAttackedBy
// Created: NLD 2005-12-01
// -----------------------------------------------------------------------------
void MIL_AgentPion::NotifyAttackedBy( MIL_Population& attacker )
{
    GetKnowledge().GetKsDirectFire().NotifyAttackedBy( attacker );
    GetRole< PHY_RolePion_HumanFactors >().NotifyAttacked();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::NotifyAttacking
// Created: NLD 2005-03-30
// -----------------------------------------------------------------------------
void MIL_AgentPion::NotifyAttacking( MIL_Agent_ABC& target, bool mustReport ) const
{
    if( !mustReport )
        return;
    if( target.GetType().IsRefugee() )
        MIL_Report::PostEvent( *this, report::eRC_TirSurCivil );
    else if( GetArmy().IsNeutral( target.GetArmy() ) == eTristate_True )
        MIL_Report::PostEvent( *this, report::eRC_TirSurCampNeutre );
    else if( GetArmy().IsAFriend( target.GetArmy() ) == eTristate_True )
        MIL_Report::PostEvent( *this, report::eRC_TirSurCampAmi );
    else if( GetArmy().IsAnEnemy( target.GetArmy() ) == eTristate_True )
        MIL_Report::PostEvent( *this, report::eRC_TirSurCampEnnemi );
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
// Name: MIL_AgentPion::CanInteractWithTraffic
// Created: JSR 2012-01-12
// -----------------------------------------------------------------------------
bool MIL_AgentPion::CanInteractWithTraffic() const
{
    if ( GetRole< PHY_RoleInterface_UrbanLocation >().GetCurrentUrbanBlock() != 0 ||
        GetRole< PHY_RoleAction_InterfaceFlying >().IsFlying() ||
        GetRole< PHY_RoleAction_MovingUnderground >().IsUnderground() ||
        GetRole< transport::PHY_RolePion_Transported >().IsTransported() )
        return false;
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::CanBeImpactedByTraffic
// Created: MMC 2013-04-15
// -----------------------------------------------------------------------------
bool MIL_AgentPion::CanBeImpactedByTraffic() const
{
    assert( pType_ );
    return ( pType_->GetUnitType().GetSpeedModifier() < 1. ) && CanInteractWithTraffic();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::InteractWithTraffic
// Created: JSR 2012-01-12
// -----------------------------------------------------------------------------
void MIL_AgentPion::InteractWithTraffic( const std::vector< TER_Agent_ABC* >& agents )
{
    if( CanBeImpactedByTraffic() )
        for( std::vector< TER_Agent_ABC* >::const_iterator itAgent = agents.begin(); itAgent != agents.end(); ++itAgent )
            InteractWithTraffic( static_cast< PHY_RoleInterface_Location& >( **itAgent ).GetAgent() );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::InteractWithTraffic
// Created: MMC 2013-04-15
// -----------------------------------------------------------------------------
void MIL_AgentPion::InteractWithTraffic( const MIL_Agent_ABC& agent )
{
    assert( pType_ );
    if( CanBeImpactedByTraffic() && &agent != this && agent.CanInteractWithTraffic() )
    {
        MT_Vector2D position = GetRole< PHY_RoleInterface_Location >().GetPosition();
        MT_Vector2D positionAgent = agent.GetRole< PHY_RoleInterface_Location >().GetPosition();
        MT_Vector2D direction = GetRole< PHY_RoleInterface_Location >().GetDirection();
        unsigned int distance = pType_->GetUnitType().GetFootprintRadius() + agent.GetType().GetUnitType().GetFootprintRadius();
        if( position.SquareDistance( positionAgent ) < distance * distance )
        {
            MT_Vector2D directionAgent = positionAgent - position;
            if( directionAgent * direction > 0 )
                GetRole< moving::PHY_RoleAction_InterfaceMoving >().ApplyTrafficModifier();
        }
    }
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
    return ( pAutomate_ && ( pAutomate_->GetPionPC() == this ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::GetOrderManager
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
MIL_PionOrderManager& MIL_AgentPion::GetOrderManager()
{
    return *pOrderManager_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::GetOrderManager
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
const MIL_PionOrderManager& MIL_AgentPion::GetOrderManager() const
{
    return *pOrderManager_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::GetDangerosity
// Created: MGD 2010-02-04
// -----------------------------------------------------------------------------
double MIL_AgentPion::GetDangerosity( boost::shared_ptr< DEC_Knowledge_Agent > pTargetKnowledge,  boost::shared_ptr< MT_Vector2D > position, bool bUseAmmo ) const
{
    if( pTargetKnowledge->GetMaxPerceptionLevel() < PHY_PerceptionLevel::recognized_
        ||  GetArmy().IsAFriend( *pTargetKnowledge->GetArmy() ) == eTristate_True
        ||  pTargetKnowledge->IsSurrendered()
        || GetRole< surrender::PHY_RoleInterface_Surrender >().IsSurrendered() )
        return 0.;
    // Target is dead ....
    const DEC_Knowledge_AgentComposante* pTargetMajorComposante = pTargetKnowledge->GetMajorComposante();
    if( !pTargetMajorComposante )
        return 0.;
    double rDangerosity = 0.;
    // Fight score
    const MT_Vector3D sourcePosition( position->rX_, position->rY_, 0.);
    const MT_Vector3D targetPosition  ( pTargetKnowledge->GetPosition().rX_, pTargetKnowledge->GetPosition().rY_, pTargetKnowledge->GetAltitude() );
    const float rDistBtwSourceAndTarget = static_cast< const float >( sourcePosition.Distance( targetPosition ) );
    rDangerosity = GetRole< PHY_RoleInterface_Composantes >().GetDangerosity( *pTargetMajorComposante, rDistBtwSourceAndTarget, bUseAmmo );
    //DegradeDangerosity( rDangerosity );//@TODO MGD before commit
    return rDangerosity;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::Distance
// Created: SLG 2010-04-12
// -----------------------------------------------------------------------------
double MIL_AgentPion::Distance( const MIL_Agent_ABC& target ) const
{
    const PHY_RoleInterface_UrbanLocation& firerUrbanRole = GetRole< PHY_RoleInterface_UrbanLocation >();
    const PHY_RoleInterface_UrbanLocation& targetUrbanRole = target.GetRole< PHY_RoleInterface_UrbanLocation >();
    if( firerUrbanRole.GetCurrentUrbanBlock() == targetUrbanRole.GetCurrentUrbanBlock() && firerUrbanRole.GetCurrentUrbanBlock() )
        return firerUrbanRole.ComputeDistanceInsideSameUrbanBlock( const_cast< MIL_Agent_ABC& >( target ) );
    else
    {
        const PHY_RoleInterface_Location& targetLocationRole = target.GetRole< PHY_RoleInterface_Location >();
        const MT_Vector2D realFirerPosition = firerUrbanRole.GetFirerPosition( targetLocationRole.GetPosition() );
        const MT_Vector2D realTargetPosition = targetUrbanRole.GetTargetPosition( const_cast< MIL_AgentPion& >( *this ) );
        const MT_Vector3D vFirerPosition( realFirerPosition.rX_, realFirerPosition.rY_, GetRole< PHY_RoleInterface_Location >().GetAltitude() );
        const MT_Vector3D vTargetPosition(  realTargetPosition.rX_, realTargetPosition.rY_, targetLocationRole.GetAltitude() );
        return vFirerPosition.Distance( vTargetPosition );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::GetCriticalIntelligence
// Created: ABR 2011-03-16
// -----------------------------------------------------------------------------
const std::string& MIL_AgentPion::GetCriticalIntelligence() const
{
    return criticalIntelligence_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::GetAffinity
// Created: ABR 2011-03-16
// -----------------------------------------------------------------------------
float MIL_AgentPion::GetAffinity( unsigned long teamID ) const
{
    return pAffinities_->GetAffinity( teamID );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::GetHumanRepartition
// Created: MMC 2011-10-10
// -----------------------------------------------------------------------------
const MIL_HumanRepartition& MIL_AgentPion::GetHumanRepartition() const
{
    return pType_->GetHumanRepartition();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::IsImmobilized
// Created: JSR 2011-09-15
// -----------------------------------------------------------------------------
bool MIL_AgentPion::IsImmobilized() const
{
    return GetRole< PHY_RolePion_Composantes >().IsImmobilized();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveCriticalIntelligence
// Created: LGY 2011-05-27
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveCriticalIntelligence( const sword::MissionParameters& msg )
{
    if( markedForDestruction_ )
        return;
    protocol::CheckCount( msg, 1 );
    criticalIntelligence_ = protocol::GetString( msg, 0 );
    client::UnitAttributes message;
    message().mutable_unit()->set_id( GetID() );
    message().set_critical_intelligence( criticalIntelligence_ );
    message.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::RegisterPath
// Created: MMC 2013-05-22
// -----------------------------------------------------------------------------
void MIL_AgentPion::RegisterPath( const DEC_Agent_Path& agentPath )
{
    agentPaths_.insert( &agentPath );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::UnregisterPath
// Created: MMC 2013-05-22
// -----------------------------------------------------------------------------
void MIL_AgentPion::UnregisterPath( const DEC_Agent_Path& agentPath )
{
    agentPaths_.erase( &agentPath );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::HasPath
// Created: MMC 2013-05-22
// -----------------------------------------------------------------------------
bool MIL_AgentPion::HasPath() const
{
    return !agentPaths_.empty();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::GetLogisticHierarchy
// Created: NLD 2011-01-10
// -----------------------------------------------------------------------------
logistic::LogisticHierarchy_ABC& MIL_AgentPion::GetLogisticHierarchy() const
{
    return GetAutomate().GetLogisticHierarchy();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::Apply2
// Created: NLD 2011-01-10
// -----------------------------------------------------------------------------
void MIL_AgentPion::Apply2( boost::function< void( PHY_Dotation& ) > visitor ) const
{
    GetRole< dotation::PHY_RoleInterface_Dotations >().Apply( visitor );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::Apply2
// Created: NLD 2011-01-10
// -----------------------------------------------------------------------------
void MIL_AgentPion::Apply2( boost::function< void( PHY_DotationStock& ) > visitor ) const
{
    const PHY_RoleInterface_Supply* itf = RetrieveRole< PHY_RoleInterface_Supply >();
    if( itf )
        itf->Apply( visitor );
}

#define CHECK_PARAM( PARAMETER, INDEX, SUBINDEX, TYPE, TEXT )                                                    \
    if( !PARAMETER.list( SUBINDEX ).has_##TYPE##() )                                                             \
        throw MASA_BADPARAM_ASN( sword::UnitActionAck::ErrorCode, sword::UnitActionAck::error_invalid_parameter, \
            "parameters[0][" + boost::lexical_cast< std::string >( INDEX ) +                                     \
            "]["+ boost::lexical_cast< std::string >( SUBINDEX ) +"] " + TEXT );

namespace
{
    void CheckNoNegativeQuantity( unsigned int index, unsigned int subIndex, const sword::MissionParameter_Value& elem )
    {
        if( elem.list( subIndex ).quantity() < 0 )
            throw MASA_BADPARAM_ASN( sword::UnitActionAck::ErrorCode, sword::UnitActionAck::error_invalid_parameter,
                "parameters[0][" + boost::lexical_cast< std::string >( index ) + "][" +
                    boost::lexical_cast< std::string >( subIndex ) +"] must be a positive number" );
    }
    void CheckParameterCount( bool invalid, const std::string& text )
    {
        if( invalid )
            throw MASA_BADPARAM_ASN( sword::UnitActionAck::ErrorCode, sword::UnitActionAck::error_invalid_parameter, text );
    }
    void CheckSubParameterCount( bool invalid, unsigned int i, const std::string& text )
    {
        if( invalid )
            CheckParameterCount( invalid, "parameters[0][" + boost::lexical_cast< std::string >( i ) + "] " + text );
    }
    void CheckSubSubParameterCount( bool invalid, unsigned int i, unsigned int j, const std::string& text )
    {
        if( invalid )
            throw MASA_BADPARAM_ASN( sword::UnitActionAck::ErrorCode, sword::UnitActionAck::error_invalid_parameter,
                "parameters[0][" + boost::lexical_cast< std::string >( i ) + "][" +
                boost::lexical_cast< std::string >( j ) + "] " + text );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveCreateBreakdowns
// Created: ABR 2011-08-09
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveCreateBreakdowns( const sword::MissionParameters& msg )
{
    protocol::CheckCount( msg, 1 );
    std::vector< std::tuple< const PHY_ComposanteTypePion*, unsigned int, const PHY_BreakdownType* > > content;
    for( int i = 0; i < protocol::GetCount( msg, 0 ); ++ i )
    {
        const int count = protocol::CheckCount( 0, i, msg, 2, 3 );
        const auto type = protocol::GetIdentifier( msg, 0, i, 0 );
        const PHY_ComposanteTypePion* pComposanteType = PHY_ComposanteTypePion::Find( type );
        protocol::Check( pComposanteType && pComposanteType->CanHaveBreakdown(),
            "must be a composante type which can breakdown", 0, i, 0 );
        const int quantity = protocol::GetQuantity( msg, 0, i, 1 );
        protocol::Check( quantity > 0, "must be positive", 0, i, 1 );
        const PHY_BreakdownType* breakdown = nullptr;
        if( count == 3 )
        {
            const uint32_t id = protocol::GetIdentifier( msg, 0, i, 2 );
            breakdown = PHY_BreakdownType::Find( id );
            protocol::Check( breakdown, "must be a breakdown type identifier", 0, i, 2 );
        }
        content.push_back( std::make_tuple( pComposanteType, quantity, breakdown ) );
    }
    PHY_RolePion_Composantes& roleComposantes = GetRole< PHY_RolePion_Composantes >();
    for( auto it = content.begin(); it != content.end(); ++it )
        roleComposantes.CreateBreakdowns( *std::get< 0 >(*it), std::get< 1 >(*it), std::get< 2 >(*it) );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveCreateWounds
// Created: ABR 2011-08-09
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveCreateWounds( const sword::MissionParameters& msg )
{
    protocol::CheckCount( msg, 1 );
    std::vector< std::pair< unsigned int, const PHY_HumanWound* > > content;
    for( int i = 0; i < protocol::GetCount( msg, 0 ); ++i )
    {
        const int count = protocol::CheckCount( 0, i, msg, 1, 2 );
        const int quantity = protocol::GetQuantity( msg, 0, i, 0 );
        protocol::Check( quantity > 0, "must be positive", 0, i, 0 );
        const PHY_HumanWound* pHumanWound = PHY_HumanWound::GetRandomWoundSeriousness();
        if( count == 2 )
        {
            // FIXME do not use private enumerations
            const unsigned enumeration = static_cast< unsigned >( protocol::GetUnsafeEnumeration( msg, 0, i, 1 ) );
            pHumanWound = PHY_HumanWound::Find( enumeration );
            protocol::Check( pHumanWound, "must be a valid wound enumeration", 0, i, 1 );
        }
        content.push_back( std::make_pair( quantity, pHumanWound ) );
    }
    PHY_RolePion_Composantes& roleComposantes = GetRole< PHY_RolePion_Composantes >();
    for( auto it = content.begin(); it != content.end(); ++it )
        roleComposantes.CreateWounds( it->first, it->second );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveChangeEquipmentState
// Created: ABR 2011-08-09
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveChangeEquipmentState( const sword::MissionParameters& msg )
{
    CheckParameterCount( msg.elem_size() != 1, "invalid parameters count, 1 parameter expected" );
    std::vector< std::pair< const PHY_ComposanteTypePion*, const sword::MissionParameter_Value > > content;
    for( int i = 0; i < msg.elem( 0 ).value_size(); ++i )
    {
        const sword::MissionParameter_Value& elem = msg.elem( 0 ).value( i );
        CheckSubParameterCount( elem.list_size() != 8, i, "must have 8 parameters" );

        CHECK_PARAM( elem, i, 0, identifier, "must be an Identifer" );
        CHECK_PARAM( elem, i, 1, quantity, "must be a Quantity" );
        CheckNoNegativeQuantity( i, 1, elem );
        CHECK_PARAM( elem, i, 2, quantity, "must be a Quantity" );
        CheckNoNegativeQuantity( i, 2, elem );
        CHECK_PARAM( elem, i, 3, quantity, "must be a Quantity" );
        CheckNoNegativeQuantity( i, 3, elem );
        CHECK_PARAM( elem, i, 4, quantity, "must be a Quantity" );
        CheckNoNegativeQuantity( i, 4, elem );
        CHECK_PARAM( elem, i, 5, quantity, "must be a Quantity" );
        CheckNoNegativeQuantity( i, 5, elem );
        CHECK_PARAM( elem, i, 6, quantity, "must be a Quantity" );
        CheckNoNegativeQuantity( i, 6, elem );

        if( elem.list( 7 ).list_size() != elem.list( 3 ).quantity() )
            throw MASA_BADPARAM_ASN( sword::UnitActionAck::ErrorCode, sword::UnitActionAck::error_invalid_parameter,
                "parameters[0][" + boost::lexical_cast< std::string >( i ) + "][7] size must be equal to parameters[0]["
                + boost::lexical_cast< std::string >( i ) + "][3]" );

        const auto type = elem.list( 0 ).identifier();
        const PHY_ComposanteTypePion* pComposanteType = PHY_ComposanteTypePion::Find( type );
        CheckSubSubParameterCount( !pComposanteType, i, 0, "must be a valid dotation category identifier" );

        for( int i = 0; i < elem.list( 7 ).list_size(); ++i )
        {
            CheckSubSubParameterCount( !elem.list( 7 ).list( i ).has_identifier(), i, 7, "must be an Identifier list" );

            unsigned int identifier = elem.list( 7 ).list( i ).identifier();
            const PHY_BreakdownType* breakdownType = PHY_BreakdownType::Find( identifier );
            CheckSubSubParameterCount( identifier != 0 && !breakdownType, i, 7, "must be a breakdown identifier" );
            CheckSubSubParameterCount( breakdownType && !pComposanteType->CanHaveBreakdown( breakdownType ), i, 7,
                "invalid breakdown identifier for the equipment" );
        }
        content.push_back( std::make_pair( pComposanteType, elem ) );
    }

    PHY_RolePion_Composantes& roleComposantes = GetRole< PHY_RolePion_Composantes >();
    for( auto it = content.begin(); it != content.end(); ++it )
        roleComposantes.ChangeEquipmentState( *it->first, it->second );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveChangeHumanState
// Created: ABR 2011-08-09
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveChangeHumanState( const sword::MissionParameters& msg )
{
    CheckParameterCount( msg.elem_size() != 1, "invalid parameters count, 1 parameter expected" );
    for( int i = 0 ; i < msg.elem( 0 ).value_size(); ++i )
    {
        const sword::MissionParameter_Value& elem = msg.elem( 0 ).value( i );
        CheckSubParameterCount( elem.list_size() != 6, i, "must have 6 parameters" );

        CHECK_PARAM( elem, i, 0, quantity, "must be a Quantity" );
        CheckSubSubParameterCount( elem.list( 0 ).quantity() <= 0, i, 0, "must be positive a non-zero positive number" );

        CHECK_PARAM( elem, i, 1, enumeration, "must be an Enumeration" );
        CHECK_PARAM( elem, i, 2, enumeration, "must be an Enumeration" );
        CHECK_PARAM( elem, i, 4, booleanvalue, "must be a Boolean" );
        CHECK_PARAM( elem, i, 5, booleanvalue, "must be a Boolean" );
        const PHY_HumanRank* pHumanRank = PHY_HumanRank::Find( static_cast< unsigned int >( elem.list( 1 ).enumeration() ) );
        CheckSubSubParameterCount( !pHumanRank, i, 1, "invalid, bad human rank enumeration" );

        auto nstate = elem.list( 2 ).enumeration();
        CheckSubSubParameterCount( !sword::EnumHumanState_IsValid( nstate ), i, 2, "invalid, bad human state enumeration" );

        sword::EnumHumanState state = static_cast< sword::EnumHumanState >( nstate );
        if( state == sword::injured )
            CheckSubSubParameterCount( elem.list( 3 ).list_size() != 1 || elem.list( 3 ).list( 0 ).list_size() != 2 ||
                                       !elem.list( 3 ).list( 0 ).list( 1 ).has_enumeration() ||
                                       !sword::EnumInjuriesSeriousness_IsValid( elem.list( 3 ).list( 0 ).list( 1 ).enumeration() ),
                                       i, 3, "invalid injuries seriousness" );
    }
    GetRole< PHY_RolePion_Composantes >().ChangeHumanState( msg );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveChangeDotation
// Created: ABR 2011-08-09
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveChangeDotation( const sword::MissionParameters& msg )
{
    CheckParameterCount( msg.elem_size() != 1, "invalid parameters count, 1 parameter expected" );
    std::vector< std::tuple< const PHY_DotationCategory*, unsigned int, float, float > > content;
    for( int i = 0; i < msg.elem( 0 ).value_size(); ++i )
    {
        const sword::MissionParameter_Value& elem = msg.elem( 0 ).value( i );
        CheckSubParameterCount( elem.list_size() != 4, i, "must have 4 parameters" );

        CHECK_PARAM( elem, i, 0, identifier, "must be an Identifer" );
        CHECK_PARAM( elem, i, 1, quantity, "must be a Quantity" );
        CHECK_PARAM( elem, i, 2, areal, "must be an Real" );
        CHECK_PARAM( elem, i, 3, areal, "must be an Real" );

        const int number = protocol::GetQuantity( msg, 0, i, 1 );
        float lowThresholdPercentage = protocol::GetReal( msg, 0, i, 2 );
        float highThresholdPercentage = protocol::GetReal( msg, 0, i, 3 );

        const PHY_DotationCategory* pDotationCategory = PHY_DotationType::FindDotationCategory( elem.list( 0 ).identifier() );
        CheckSubSubParameterCount( !pDotationCategory, i, 0, "must be a valid dotation category identifier" );
        CheckSubSubParameterCount( number < 0, i, 1, "must be a positive number" );
        CheckSubSubParameterCount( lowThresholdPercentage < 0.f || lowThresholdPercentage > 100.f, i, 2,
            "must be a number between 0 and 100" );
        CheckSubSubParameterCount( highThresholdPercentage < 0.f || highThresholdPercentage > 100.f, i, 3,
            "must be a number between 0 and 100" );
        if( lowThresholdPercentage > highThresholdPercentage )
            highThresholdPercentage = lowThresholdPercentage;
        content.push_back( std::make_tuple( pDotationCategory, number, lowThresholdPercentage, highThresholdPercentage ) );
    }

    dotation::PHY_RolePion_Dotations& roleDotations = GetRole< dotation::PHY_RolePion_Dotations >();
    for( auto it = content.begin(); it != content.end(); ++it )
        roleDotations.ChangeDotation( *std::get< 0 >( *it ), std::get< 1 >( *it ), std::get< 2 >( *it ), std::get< 3 >( *it ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveCreateDirectFireOrder
// Created: SLI 2011-09-23
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveCreateDirectFireOrder( const sword::MissionParameters& msg )
{
    const uint32_t id = protocol::GetAgentId( msg, 0 );
    auto target = MIL_AgentServer::GetWorkspace().GetEntityManager().FindAgentPion( id );
    protocol::Check( target && target != this, "is an invalid target", 0 );
    auto kg = GetKnowledgeGroup();
    protocol::Check( kg, "must have a valid knowledge group" );
    auto knowledge = target->CreateKnowledge( *kg );
    auto role = RetrieveRole< firing::PHY_RoleAction_DirectFiring >();
    protocol::Check( role, "missing direct firing role" );
    PHY_FireResults_Pion* results = nullptr;
    role->FirePion( knowledge, firing::PHY_DirectFireData::eFiringModeNormal, 1,
        firing::PHY_DirectFireData::eFireUsingAllComposantes,
        firing::PHY_DirectFireData::eFireOnAllComposantes, results, false );
    if( !results )
        return;
    results->IncRef();
    results->DecRef();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveLoadUnit
// Created: SLI 2011-10-17
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveLoadUnit( const sword::MissionParameters& msg, bool load )
{
    const uint32_t id = protocol::GetAgentId( msg, 0 );
    auto target = MIL_AgentServer::GetWorkspace().GetEntityManager().FindAgentPion( id );
    protocol::Check( target && target != this, "is an invalid target", 0 );
    auto role = RetrieveRole< transport::PHY_RoleAction_Transport >();
    protocol::Check( role, "missing transport role" );
    if( load )
        role->MagicLoadPion( *target, false );
    else
        role->MagicUnloadPion( *target );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::IsCivilian
// Created: LDC 2012-03-13
// -----------------------------------------------------------------------------
bool MIL_AgentPion::IsCivilian() const
{
    return pType_->GetHumanRepartition().IsCivilian();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::CanEmitReports
// Created: NPT 2013-02-07
// -----------------------------------------------------------------------------
bool MIL_AgentPion::CanEmitReports() const
{
    return !IsDead();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::CanBeDeleted
// Created: MMC 2013-05-22
// -----------------------------------------------------------------------------
bool MIL_AgentPion::CanBeDeleted() const
{
    return IsMarkedForDestruction() && !CallRole( &DEC_RolePion_Decision::IsUsedByDIA, false ) && !HasPath();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::CanTransportDestroyed
// Created: JSR 2014-02-07
// -----------------------------------------------------------------------------
bool MIL_AgentPion::CanTransportDestroyed() const
{
    return CallRole( &PHY_RoleInterface_Composantes::CanTransportDestroyed, false );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReloadBrain
// Created: SLI 2011-09-20
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReloadBrain( const sword::MissionParameters& msg )
{
    CancelAllActions();
    auto model = parameters::GetModel( msg, []( const std::string& model ){
        return MIL_AgentServer::GetWorkspace().GetWorkspaceDIA().FindModelPion( model );
    } );
    auto& role = GetRole< DEC_RolePion_Decision >();
    const bool modified = model && model != &role.GetModel();
    if( modified )
        role.SetModel( *model );
    GetDecision().Reload( !modified );
    pOrderManager_->CancelMission();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnChangeBrainDebug
// Created: SLI 2013-06-18
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnChangeBrainDebug( const sword::MissionParameters& msg )
{
    protocol::CheckCount( msg, 1 );
    const bool activate = protocol::GetBool( msg, 0 );
    GetRole< DEC_RolePion_Decision >().ActivateBrainDebug( activate );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnChangePosture
// Created: MCO 2013-08-26
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnChangePosture( const sword::MissionParameters& msg )
{
    protocol::CheckCount( msg, 1 );
    const auto posture = GET_ENUMERATION( sword::UnitAttributes::Posture, msg, 0 );
    GetRole< PHY_RoleInterface_Posture >().SetPosture( PHY_Posture::FindPosture( posture ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveFinishLogisticHandlings
// Created: NLD 2012-01-09
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveFinishLogisticHandlings()
{
    bool handlings = CallRole( &PHY_RoleInterface_Maintenance::FinishAllHandlingsSuccessfullyWithoutDelay, false );
    handlings = CallRole( &PHY_RoleInterface_Medical::FinishAllHandlingsSuccessfullyWithoutDelay, false ) || handlings;
    if( ! handlings )
        throw MASA_BADUNIT_UNIT( "unit must have logistic handlings pending" );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::SetExtensions
// Created: AHC 2012-10-01
// -----------------------------------------------------------------------------
void MIL_AgentPion::SetExtensions( const MIL_DictionaryExtensions& ext )
{
    pExtensions_.reset( new MIL_DictionaryExtensions( ext ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::GetExtensions
// Created: AHC 2013-03-01
// -----------------------------------------------------------------------------
MIL_DictionaryExtensions* const MIL_AgentPion::GetExtensions() const
{
    return pExtensions_.get();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::SetColor
// Created: AHC 2013-03-01
// -----------------------------------------------------------------------------
void MIL_AgentPion::SetColor( std::auto_ptr< MIL_Color > color )
{
    pColor_ = color;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::GetColor
// Created: AHC 2013-03-01
// -----------------------------------------------------------------------------
MIL_Color* const MIL_AgentPion::GetColor() const
{
    return pColor_.get();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::GetLevel
// Created: AHC 2013-03-01
// -----------------------------------------------------------------------------
const std::string& MIL_AgentPion::GetLevel() const
{
    return level_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::SetLevel
// Created: AHC 2013-03-01
// -----------------------------------------------------------------------------
void MIL_AgentPion::SetLevel( const std::string& level )
{
    level_ = level;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::GetSymbol
// Created: AHC 2013-03-01
// -----------------------------------------------------------------------------
const std::string& MIL_AgentPion::GetSymbol() const
{
    return app6Symbol_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::SetSymbol
// Created: AHC 2013-03-01
// -----------------------------------------------------------------------------
void MIL_AgentPion::SetSymbol( const std::string& symbol )
{
    app6Symbol_ = symbol;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::GetAffinities
// Created: AHC 2013-03-01
// -----------------------------------------------------------------------------
MIL_AffinitiesMap* const MIL_AgentPion::GetAffinities() const
{
    return pAffinities_.get();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::SetAffinities
// Created: AHC 2013-03-01
// -----------------------------------------------------------------------------
void MIL_AgentPion::SetAffinities( std::auto_ptr< MIL_AffinitiesMap > affinities )
{
    pAffinities_ = affinities;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::SetKnowledge
// Created: AHC 2013-03-01
// -----------------------------------------------------------------------------
void MIL_AgentPion::SetKnowledge( DEC_KnowledgeBlackBoard_AgentPion* knowledge )
{
    pKnowledgeBlackBoard_ = knowledge;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::SetAutomate
// Created: AHC 2013-03-01
// -----------------------------------------------------------------------------
void MIL_AgentPion::SetAutomate( MIL_Automate* automate )
{
    pAutomate_ = automate;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::GetController
// Created: BAX 2013-08-30
// -----------------------------------------------------------------------------
const MissionController_ABC& MIL_AgentPion::GetController() const
{
    return pOrderManager_->GetController();
}
