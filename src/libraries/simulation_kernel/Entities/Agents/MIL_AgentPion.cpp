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
#include "Roles/Protection/PHY_RolePion_ActiveProtection.h"
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
#include "Roles/Deployment/PHY_RoleInterface_Deployment.h"
#include "Actions/Loading/PHY_RoleAction_Loading.h"
#include "Actions/Objects/PHY_RoleAction_Objects.h"
#include "Actions/Moving/PHY_RoleAction_Moving.h"
#include "Actions/Flying/PHY_RoleAction_Flying.h"
#include "Actions/Firing/DirectFiring/PHY_RoleAction_DirectFiring.h"
#include "Actions/Firing/IndirectFiring/PHY_RoleAction_IndirectFiring.h"
#include "Actions/Transport/PHY_RoleAction_Transport.h"
#include "Actions/CrowdTransport/PHY_RoleAction_CrowdTransport.h"
#include "Actions/Underground/PHY_RoleAction_MovingUnderground.h"
#include "Actions/Emergency/PHY_RoleAction_FolkInfluence.h"
#include "Entities/Orders/MIL_Report.h"
#include "Entities/Agents/Actions/Firing/PHY_FireResults_Pion.h"
#include "Entities/Agents/Units/PHY_UnitType.h"
#include "Entities/Agents/Units/Categories/PHY_NatureLevel.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Agents/Units/Dotations/PHY_AmmoDotationClass.h"
#include "Entities/Agents/Units/Humans/PHY_HumanRank.h"
#include "Entities/Agents/Units/Humans/PHY_HumanWound.h"
#include "Entities/Agents/Units/Humans/MIL_Injury_Wound.h"
#include "Entities/Agents/Units/HumanFactors/PHY_Morale.h"
#include "Entities/Agents/Units/HumanFactors/PHY_Experience.h"
#include "Entities/Agents/Units/HumanFactors/PHY_Stress.h"
#include "Entities/Agents/Units/HumanFactors/PHY_Tiredness.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/MIL_Army_ABC.h"
#include "Decision/DEC_Model_ABC.h"
#include "Decision/DEC_Representations.h"
#include "Decision/DEC_Workspace.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Knowledge/DEC_KS_Fire.h"
#include "Network/NET_Publisher_ABC.h"
#include "Network/NET_AsnException.h"
#include "protocol/ClientSenders.h"
#include "Tools/MIL_AffinitiesMap.h"
#include "Tools/MIL_DictionaryExtensions.h"
#include "Tools/MIL_Tools.h"
#include "Tools/MIL_Color.h"
#include "Tools/MIL_HumanRepartition.h"
#include "Tools/MIL_IDManager.h"
#include "simulation_kernel/AlgorithmsFactories.h"
#include "simulation_kernel/NetworkNotificationHandler_ABC.h"
#include "MT_Tools/MT_ScipioException.h"
#include "MT_Tools/MT_FormatString.h"
#include <boost/serialization/vector.hpp>
#include <boost/foreach.hpp>
#include <boost/regex.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_AgentPion )

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion constructor
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_AgentPion::MIL_AgentPion( const MIL_AgentTypePion& type, MIL_Automate& automate, const AlgorithmsFactories& algorithmFactories, xml::xistream& xis )
    : MIL_Agent_ABC( xis )
    , pType_               ( &type )
    , bHasChanged_         ( false )
    , pAutomate_           ( &automate )
    , pKnowledgeBlackBoard_( new DEC_KnowledgeBlackBoard_AgentPion( *this ) )
    , pOrderManager_       ( new MIL_PionOrderManager( *this ) )
    , algorithmFactories_  ( algorithmFactories )
    , pAffinities_         ( new MIL_AffinitiesMap( xis ) )
    , pExtensions_         ( new MIL_DictionaryExtensions( xis ) )
    , pColor_              ( new MIL_Color( xis ) )
    , pHumanRepartition_   ( new MIL_HumanRepartition( type.GetHumanRepartition() ) )
    , app6Symbol_          ( "" )
    , level_               ( "" )
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
// Created: MMC 2011-05-27
// -----------------------------------------------------------------------------
MIL_AgentPion::MIL_AgentPion( const MIL_AgentTypePion& type, MIL_Automate& automate, const AlgorithmsFactories& algorithmFactories, const std::string& name )
    : MIL_Agent_ABC( name )
    , pType_               ( &type )
    , bHasChanged_         ( false )
    , pAutomate_           ( &automate )
    , pKnowledgeBlackBoard_( new DEC_KnowledgeBlackBoard_AgentPion( *this ) )
    , pOrderManager_       ( new MIL_PionOrderManager( *this ) )
    , algorithmFactories_  ( algorithmFactories )
    , pAffinities_         ( new MIL_AffinitiesMap() )
    , pExtensions_         ( new MIL_DictionaryExtensions() )
    , pHumanRepartition_   ( new MIL_HumanRepartition( type.GetHumanRepartition() ) )
    , app6Symbol_          ( "" )
    , level_               ( "" )
{
    pColor_.reset( new MIL_Color( automate.GetColor() ) );
    automate.RegisterPion( *this );

    const std::string& longNameBase = automate.GetExtensions().GetExtension( "NomLong" ); 
    if( !longNameBase.empty() )
    {
        boost::regex regex( "[^a-zA-Z0-9.]" );
        std::string longName = GetName() + "." + longNameBase;
        longName = boost::regex_replace( longName , regex, "" );
        pExtensions_->SetExtension( "NomLong", longName );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion constructor
// Created: LDC 2010-02-22
// -----------------------------------------------------------------------------
MIL_AgentPion::MIL_AgentPion( const MIL_AgentTypePion& type, const AlgorithmsFactories& algorithmFactories )
    : MIL_Agent_ABC( type.GetName() )
    , pType_               ( &type )
    , bHasChanged_         ( false )
    , pAutomate_           ( 0 )
    , pKnowledgeBlackBoard_( new DEC_KnowledgeBlackBoard_AgentPion( *this ) )
    , pOrderManager_       ( new MIL_PionOrderManager( *this ) )
    , algorithmFactories_  ( algorithmFactories )
    , pAffinities_         ( 0 )
    , pExtensions_         ( 0 )
    , pColor_              ( 0 )
    , pHumanRepartition_   ( new MIL_HumanRepartition( type.GetHumanRepartition() ) )
    , app6Symbol_          ( "" )
    , level_               ( "" )
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
    MIL_AffinitiesMap* pAffinities;
    MIL_DictionaryExtensions* pExtensions;
    MIL_Color* pColor;
    MIL_HumanRepartition* pRepartition;
    file >> boost::serialization::base_object< MIL_Agent_ABC >( *this );
    file >> pAutomate_
         >> pKnowledgeBlackBoard_
         >> pAffinities
         >> pExtensions
         >> pColor
         >> pRepartition
         >> criticalIntelligence_
         >> app6Symbol_
         >> level_;
    LoadRole< network::NET_RolePion_Dotations >( file, *this );
    LoadRole< PHY_RolePion_Reinforcement >( file, *this );
    LoadRole< PHY_RolePion_Posture >( file, *this );
    LoadRole< PHY_RolePion_Location >( file, *this );
    LoadRole< PHY_RolePion_UrbanLocation >( file, *this );
    LoadRole< dotation::PHY_RolePion_Dotations >( file, *this );
    LoadRole< human::PHY_RolePion_Humans >( file, *this );
    LoadRole< PHY_RolePion_Composantes >( file, *this );
    LoadRole< PHY_RolePion_ActiveProtection >( file, *this );
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
    LoadRole< crowdtransport::PHY_RoleAction_CrowdTransport >( file, *this );
    LoadRole< moving::PHY_RoleAction_Moving >( file, *this );
    LoadRole< PHY_RoleAction_Objects >( file, *this );
    LoadRole< firing::PHY_RoleAction_DirectFiring >( file, *this );
    LoadRole< firing::PHY_RoleAction_IndirectFiring >( file, *this );
    LoadRole< DEC_RolePion_Decision >( file, *this );
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
    pHumanRepartition_.reset( pRepartition );
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
    const MIL_HumanRepartition* const pRepartition = pHumanRepartition_.get();
    file << boost::serialization::base_object< MIL_Agent_ABC >( *this );
    file << pAutomate_
        << pKnowledgeBlackBoard_
        << pAffinities
        << pExtensions
        << pColor
        << pRepartition
        << criticalIntelligence_
        << app6Symbol_
        << level_;
    SaveRole< network::NET_RolePion_Dotations >( *this, file );
    SaveRole< PHY_RolePion_Reinforcement >( *this, file );
    SaveRole< PHY_RolePion_Posture >( *this, file );
    SaveRole< PHY_RolePion_Location >( *this, file );
    SaveRole< PHY_RolePion_UrbanLocation >( *this, file );
    SaveRole< dotation::PHY_RolePion_Dotations >( *this, file );
    SaveRole< human::PHY_RolePion_Humans >( *this, file );
    SaveRole< PHY_RolePion_Composantes >( *this, file );
    SaveRole< PHY_RolePion_ActiveProtection >( *this, file );
    SaveRole< PHY_RolePion_Perceiver >( *this, file );
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
    SaveRole< moving::PHY_RoleAction_Moving >( *this, file );
    SaveRole< PHY_RoleAction_Objects >( *this, file );
    SaveRole< firing::PHY_RoleAction_DirectFiring >( *this, file );
    SaveRole< firing::PHY_RoleAction_IndirectFiring >( *this, file );
    SaveRole< DEC_RolePion_Decision >( *this, file );
    SaveRole< PHY_RoleAction_InterfaceFlying >( *this, file );
    SaveRole< PHY_RoleAction_FolkInfluence >( *this, file );
    SaveRole< PHY_RolePion_Illumination >( *this, file ); // LTO
    SaveRole< PHY_RoleAction_MovingUnderground >( *this, file );
    SaveRole< PHY_RoleInterface_Deployment >( *this, file );
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
    xos << xml::attribute( "id", GetID() )
        << xml::attribute( "type", pType_->GetName() )
        << xml::attribute( "command-post", IsPC() )
        << xml::attribute( "position", MIL_Tools::ConvertCoordSimToMos( GetRole< PHY_RolePion_Location >().GetPosition() ) );
    if( !level_.empty() )
        xos << xml::attribute( "level", level_ );
    if( !app6Symbol_.empty() )
        xos << xml::attribute( "overridden-symbol", true )
            << xml::attribute( "nature", app6Symbol_ );
    pColor_->WriteODB( xos );
    GetRole< PHY_RolePion_Composantes >().WriteODB( xos );         // Equipments
    GetRole< human::PHY_RolePion_Humans >().WriteODB( xos );       // Humans
    GetRole< dotation::PHY_RolePion_Dotations >().WriteODB( xos ); // Dotations
    const PHY_RoleInterface_Supply* role = RetrieveRole< PHY_RoleInterface_Supply >();//@TODO verify
    if( role )
        role->WriteODB( xos ); // Stocks
    if( criticalIntelligence_ != "" )
    {
        xos << xml::start( "critical-intelligence" )
                << xml::attribute( "content", criticalIntelligence_ )
            << xml::end;
    }
    pAffinities_->WriteODB( xos );
    pExtensions_->WriteODB( xos );
    pHumanRepartition_->WriteODB( xos );
    xos << xml::end;// unit
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::ReadOverloading
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
void MIL_AgentPion::ReadOverloading( xml::xistream& xis )
{
    // Dotations overloaded by ODB
    GetRole< PHY_RolePion_Composantes >().ReadOverloading( xis );         // Equipments + Humans
    GetRole< dotation::PHY_RolePion_Dotations >().ReadOverloading( xis ); // Dotations
    GetRole< PHY_RolePion_HumanFactors >().ReadOverloading( xis );        // Human factor
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
    if( pAutomate_ )
        pAutomate_->UnregisterPion( *this );
    delete pKnowledgeBlackBoard_;
}

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
// Name: MIL_AgentPion::UpdateUnderIndirectFire
// Created: LMT 2012-06-13
// -----------------------------------------------------------------------------
bool MIL_AgentPion::UpdateUnderIndirectFire()
{
    bool returnValue = GetRole< PHY_RolePion_Composantes >().IsUnderIndirectFire();
    GetRole< PHY_RolePion_Composantes >().ResetUnderIndirectFire();
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
// Created: JSR 2012-05-03
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
    if( GetRole< PHY_RolePion_Composantes >().IsImmobilized() )
        pOrderManager_->CancelMission();
    try
    {
        pOrderManager_->Update();
        GetRole< DEC_Decision_ABC >().UpdateDecision( duration );
    }
    catch( std::exception& e )
    {
        DEC_Decision_ABC* role = RetrieveRole< DEC_Decision_ABC >();
        if( role )
            role->LogError( &e );
        MIL_Report::PostEvent( *this, MIL_Report::eReport_MissionImpossible_ );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::UpdatePhysicalState
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void MIL_AgentPion::UpdatePhysicalState()
{
    try
    {
        const bool bIsDead = IsDead();
        GetRole< dotation::PHY_RolePion_Dotations >().Update( bIsDead );
        GetRole< human::PHY_RolePion_Humans >().Update( bIsDead );
        GetRole< PHY_RolePion_Composantes >().Update( bIsDead );
        GetRole< PHY_RolePion_Posture >().Update( bIsDead );
        GetRole< PHY_RolePion_Reinforcement >().Update( bIsDead );
        GetRole< PHY_RolePion_Location >().Update( bIsDead );
        GetRole< nbc::PHY_RolePion_NBC >().Update( bIsDead );
        GetRole< PHY_RolePion_Communications >().Update( bIsDead );
        GetRole< PHY_RolePion_HumanFactors >().Update( bIsDead );
        GetRole< transport::PHY_RolePion_Transported >().Update( bIsDead );
        GetRole< surrender::PHY_RolePion_Surrender >().Update( bIsDead );
        GetRole< refugee::PHY_RolePion_Refugee >().Update( bIsDead );
        GetRole< PHY_RolePion_Population >().Update( bIsDead );
        GetRole< PHY_RolePion_Perceiver >().Update( bIsDead ); // Doit �tre apr�s PHY_RolePion_Composantes $$$ pourri - utiliser des observers
        GetRole< transport::PHY_RoleAction_Loading >().Update( bIsDead );
        GetRole< transport::PHY_RoleAction_Transport >().Update( bIsDead );
        GetRole< crowdtransport::PHY_RoleAction_CrowdTransport >().Update( bIsDead );
        GetRole< PHY_RoleAction_Objects >().Update( bIsDead );
        GetRole< moving::PHY_RoleAction_Moving >().Update( bIsDead );
        GetRole< PHY_RoleAction_InterfaceFlying >().Update( bIsDead );
        GetRole< firing::PHY_RoleAction_DirectFiring >().Update( bIsDead );
        GetRole< firing::PHY_RoleAction_IndirectFiring >().Update( bIsDead );
        GetRole< PHY_RoleAction_FolkInfluence >().Update( bIsDead );
        GetRole< PHY_RoleAction_MovingUnderground >().Update( bIsDead );
        GetRole< PHY_RoleInterface_Deployment >().Update( bIsDead );
        PHY_RoleInterface_Maintenance* role = RetrieveRole< PHY_RoleInterface_Maintenance >(); //@TODO add update to new role interface
        if( role )
            role->Update( bIsDead );
        PHY_RoleInterface_Medical* role2 = RetrieveRole< PHY_RoleInterface_Medical >();
        if( role2 )
            role2->Update( bIsDead );
        PHY_RoleInterface_Supply* role3 = RetrieveRole< PHY_RoleInterface_Supply >();
        if( role3 )
            role3->Update( bIsDead );
    }
    catch( std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Error updating unit " << GetID() << " : " << e.what() );
    }
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
    try
    {
        GetRole< network::NET_RolePion_Dotations >().SendChangedState();
        GetRole< network::NET_RolePion_Dotations >().Clean();
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
    catch( std::exception & e )
    {
        MT_LOG_ERROR_MSG( "Error updating network for unit " << GetID() << " : " << e.what() );
    }
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
    GetRole< PHY_RolePion_Location >().Clean();
    GetRole< PHY_RolePion_Perceiver >().Clean();
    GetRole< dotation::PHY_RolePion_Dotations >().Clean();
    GetRole< human::PHY_RolePion_Humans >().Clean();
    GetRole< PHY_RolePion_Composantes >().Clean();
    GetRole< PHY_RolePion_Posture >().Clean();
    GetRole< PHY_RolePion_Reinforcement >().Clean();
    GetRole< nbc::PHY_RolePion_NBC >().Clean();
    GetRole< PHY_RolePion_Communications >().Clean();
    GetRole< PHY_RolePion_HumanFactors >().Clean();
    GetRole< surrender::PHY_RoleInterface_Surrender >().Clean();
    GetRole< refugee::PHY_RolePion_Refugee >().Clean();
    GetRole< PHY_RolePion_Population >().Clean();
    GetRole< transport::PHY_RoleInterface_Transported >().Clean();
    GetRole< transport::PHY_RoleAction_Transport >().Clean();
    GetRole< crowdtransport::PHY_RoleAction_CrowdTransport >().Clean();
    GetRole< transport::PHY_RoleAction_Loading >().Clean();
    GetRole< PHY_RoleAction_Objects >().Clean();
    GetRole< moving::PHY_RoleAction_Moving >().Clean();
    GetRole< PHY_RoleAction_InterfaceFlying >().Clean();
    GetRole< firing::PHY_RoleAction_DirectFiring >().Clean();
    GetRole< firing::PHY_RoleAction_IndirectFiring >().Clean();
    DEC_RolePion_Decision* roleDec = RetrieveRole< DEC_RolePion_Decision >();
    if( roleDec )
        roleDec->Clean();
    GetRole< PHY_RoleAction_FolkInfluence >().Clean();
    GetRole< PHY_RoleAction_MovingUnderground >().Clean();
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
boost::shared_ptr< MIL_KnowledgeGroup > MIL_AgentPion::GetKnowledgeGroup() const
{
    if( GetRole< PHY_RolePion_Communications >().CanEmit() )
    {
        if( ! pAutomate_ )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Automate is undefined for agent id %d ", GetID() ) );
        return pAutomate_->GetKnowledgeGroup();
    }
    return GetRole< PHY_RolePion_Communications >().GetKnowledgeGroup();
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
boost::shared_ptr< DEC_Knowledge_Agent > MIL_AgentPion::CreateKnowledge( boost::shared_ptr< MIL_KnowledgeGroup >& knowledgeGroup )
{
    boost::shared_ptr< DEC_Knowledge_Agent > result( new DEC_Knowledge_Agent( knowledgeGroup, *this ) );
    return result;
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
    pHumanRepartition_->SendFullState( creationMsg );
    pColor_->SendFullState( creationMsg );
    if( !level_.empty() )
        if( const PHY_NatureLevel* nature = PHY_NatureLevel::Find( level_ ) )
            creationMsg().set_level( nature->GetAsnID() );
    if( !app6Symbol_.empty() )
        creationMsg().set_app6symbol( app6Symbol_ );
    creationMsg.Send( NET_Publisher_ABC::Publisher(), nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::SendFullState
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void MIL_AgentPion::SendFullState( unsigned int nCtx ) const
{
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
    GetRole< network::NET_RolePion_Dotations >().SendFullState( nCtx );
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
void MIL_AgentPion::OnReceiveOrder( const sword::UnitOrder& msg )
{
    pOrderManager_->OnReceiveMission( msg );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveFragOrder
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveFragOrder( const sword::FragOrder& msg )
{
    pOrderManager_->OnReceiveFragOrder( msg );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveDestroyComponent
// Created: AGE 2007-06-19
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveDestroyComponent()
{
    GetRole< PHY_RolePion_Composantes >().DestroyRandomComposante();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::MagicMove
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void MIL_AgentPion::MagicMove( const MT_Vector2D& vNewPos )
{
    GetRole< PHY_RoleAction_MovingUnderground >().GetOutFromUndergroundNetwork();
    GetRole< PHY_RolePion_Location >().MagicMove( vNewPos );
    GetRole< PHY_RolePion_UrbanLocation >().MagicMove( vNewPos );
    CancelAllActions();
    DEC_RolePion_Decision* roleDec = RetrieveRole< DEC_RolePion_Decision >();
    if( roleDec )
        roleDec->Reset( GetAutomate().GetName() );
    pOrderManager_->CancelMission();
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
    if( asn.type() != sword::move_to )
        throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
    if( pAutomate_->IsEngaged() )
        throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_automat_engaged );
    if( !asn.has_parameters() || asn.parameters().elem_size() != 1)
        throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
    const sword::MissionParameter& parametre = asn.parameters().elem( 0 );
    if( !parametre.value_size() == 1 || !parametre.value().Get(0).has_point() )
        throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
    const sword::Point& point = parametre.value().Get(0).point();
    if( point.location().type() != sword::Location::point  || point.location().coordinates().elem_size() != 1 )
        throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
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
    if( msg.elem( 0 ).value_size() == 1 && msg.elem( 0 ).value().Get(0).has_enumeration() )
    {
        sword::UnitAttributes::EnumUnitTiredness tiredness = static_cast< sword::UnitAttributes::EnumUnitTiredness >( msg.elem( 0 ).value().Get(0).enumeration() );
        const PHY_Tiredness* pTiredness = PHY_Tiredness::Find( tiredness );
        if( !pTiredness )
            throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
        GetRole< PHY_RolePion_HumanFactors >().SetTiredness( *pTiredness, true );
    }
    if( msg.elem( 1 ).value_size() == 1 && msg.elem( 1 ).value().Get(0).has_enumeration() )
    {
        sword::UnitAttributes::EnumUnitMorale morale = static_cast< sword::UnitAttributes::EnumUnitMorale >( msg.elem( 1 ).value().Get(0).enumeration() );
        const PHY_Morale* pMoral = PHY_Morale::Find( morale );
        if( !pMoral )
            throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
        GetRole< PHY_RolePion_HumanFactors >().SetMorale( *pMoral );
    }
    if( msg.elem( 2 ).value_size() == 1 && msg.elem( 2 ).value().Get(0).has_enumeration() )
    {
        sword::UnitAttributes::EnumUnitExperience experience = static_cast< sword::UnitAttributes::EnumUnitExperience >( msg.elem( 2 ).value().Get(0).enumeration() );
        const PHY_Experience* pExperience = PHY_Experience::Find( experience );
        if( !pExperience )
            throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
        GetRole< PHY_RolePion_HumanFactors >().SetExperience( *pExperience );
    }
    if( msg.elem( 3 ).value_size() == 1 && msg.elem( 3 ).value().Get(0).has_enumeration() )
    {
        sword::UnitAttributes::EnumUnitStress stress = static_cast< sword::UnitAttributes::EnumUnitStress >( msg.elem( 3 ).value().Get(0).enumeration() );
        const PHY_Stress* pStress = PHY_Stress::Find( stress );
        if( !pStress )
            throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
        GetRole< PHY_RolePion_HumanFactors >().SetStress( *pStress, true );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveResupplyHumans
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveResupplyHumans()
{
    GetRole< human::PHY_RolePion_Humans >().HealAllHumans();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveResupplyResources
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveResupplyResources()
{
    GetRole< dotation::PHY_RolePion_Dotations >().ResupplyDotations();
    PHY_RoleInterface_Supply* role = RetrieveRole< PHY_RoleInterface_Supply >();
    if( role )
        role->ResupplyStocks();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveResupplyEquipement
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveResupplyEquipement()
{
    GetRole< PHY_RolePion_Composantes >().RepairAllComposantes();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveResupplyAll
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveResupplyAll()
{
    GetRole< PHY_RolePion_Composantes >().RepairAllComposantes();
    GetRole< dotation::PHY_RolePion_Dotations >().ResupplyDotations();
    PHY_RoleInterface_Supply* role = RetrieveRole< PHY_RoleInterface_Supply >();
    if( role )
        role->ResupplyStocks();
    GetRole< human::PHY_RolePion_Humans >().HealAllHumans();
    GetRole< nbc::PHY_RolePion_NBC >().Decontaminate();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveResupply
// Created: JSR 2010-04-15
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveResupply( const sword::MissionParameters& msg )
{
    if( msg.elem_size() < 5 )
        throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );

    if( msg.elem( 0 ).value_size() > 0 ) // Equipments
    {
        PHY_RolePion_Composantes& roleComposantes = GetRole< PHY_RolePion_Composantes >();
        for( int i = 0; i < msg.elem( 0 ).value_size(); ++i )
        {
            if ( msg.elem( 0 ).value().Get( i ).list_size() < 2 )
                throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
            if ( !msg.elem( 0 ).value().Get( i ).list( 0 ).has_identifier() )
                throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
            if ( !msg.elem( 0 ).value().Get( i ).list( 1 ).has_quantity() )
                throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
            
            sword::EquipmentType type;
            type.set_id( msg.elem( 0 ).value().Get( i ).list( 0 ).identifier() );
            int number = msg.elem( 0 ).value().Get( i ).list( 1 ).quantity();
            const PHY_ComposanteTypePion* pComposanteType = PHY_ComposanteTypePion::Find( type );
            if( pComposanteType )
                roleComposantes.ChangeComposantesAvailability( *pComposanteType, number );
        }
    }
    if( msg.elem( 1 ).value_size() > 0 ) // Humans
    {
        human::PHY_RolePion_Humans& roleHumans = GetRole< human::PHY_RolePion_Humans >();
        for( int i = 0 ; i < msg.elem( 1 ).value_size(); ++i )
        {
            if ( msg.elem( 1 ).value().Get( i ).list_size() < 2 )
                throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
            if ( !msg.elem( 1 ).value().Get( i ).list( 0 ).has_identifier() )
                throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
            if ( !msg.elem( 1 ).value().Get( i ).list( 1 ).has_quantity() )
                throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );

            unsigned int rank = msg.elem( 1 ).value().Get( i ).list( 0 ).identifier();
            int number = msg.elem( 1 ).value().Get( i ).list( 1 ).quantity();
            const PHY_HumanRank* pHumanRank = PHY_HumanRank::Find( rank );
            if( pHumanRank )
                roleHumans.ChangeHumansAvailability( *pHumanRank, number );
        }
    }
    if( msg.elem( 2 ).value_size() > 0 ) // Dotations
    {
        dotation::PHY_RolePion_Dotations& roleDotations = GetRole< dotation::PHY_RolePion_Dotations >();
        for( int i = 0; i < msg.elem( 2 ).value_size(); ++i )
        {
            if ( msg.elem( 2 ).value().Get( i ).list_size() < 2 )
                throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
            if ( !msg.elem( 2 ).value().Get( i ).list( 0 ).has_identifier() )
                throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
            if ( !msg.elem( 2 ).value().Get( i ).list( 1 ).has_quantity() )
                throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );

            unsigned int dotation = msg.elem( 2 ).value().Get( i ).list( 0 ).identifier();
            int number = msg.elem( 2 ).value().Get( i ).list( 1 ).quantity();
            const PHY_DotationType* pDotationType = PHY_DotationType::FindDotationType( dotation );
            if( pDotationType )
                roleDotations.ResupplyDotations( *pDotationType, number / 100. );
        }
    }
    if( msg.elem( 3 ).value_size() > 0 ) // Ammunition
    {
        dotation::PHY_RolePion_Dotations& roleDotations = GetRole< dotation::PHY_RolePion_Dotations >();
        for( int i = 0; i < msg.elem( 3 ).value_size(); ++i )
        {
            if ( msg.elem( 3 ).value().Get( i ).list_size() < 2 )
                throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
            if ( !msg.elem( 3 ).value().Get( i ).list( 0 ).has_identifier() )
                throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
            if ( !msg.elem( 3 ).value().Get( i ).list( 1 ).has_quantity() )
                throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );

            unsigned int munition = msg.elem( 3 ).value().Get( i ).list( 0 ).identifier();
            int number = msg.elem( 3 ).value().Get( i ).list( 1 ).quantity();
            const PHY_AmmoDotationClass* pAmmoClass = PHY_AmmoDotationClass::Find( munition );
            if( pAmmoClass )
                roleDotations.ResupplyDotations( *pAmmoClass, number / 100. );
        }
    }
    PHY_RoleInterface_Supply* roleSupply = RetrieveRole< PHY_RoleInterface_Supply >();
    if( roleSupply && msg.elem( 4 ).value_size() > 0 ) // stocks
        for( int i = 0; i < msg.elem( 4 ).value_size(); ++i )
        {
            if ( msg.elem( 4 ).value().Get( i ).list_size() < 2 )
                throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
            if ( !msg.elem( 4 ).value().Get( i ).list( 0 ).has_identifier() )
                throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
            if ( !msg.elem( 4 ).value().Get( i ).list( 1 ).has_quantity() )
                throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );

            unsigned int stock = msg.elem( 4 ).value().Get( i ).list( 0 ).identifier();
            int number = msg.elem( 4 ).value().Get( i ).list( 1 ).quantity();
            const PHY_DotationCategory* pDotationCategory = PHY_DotationType::FindDotationCategory( stock );
            if( pDotationCategory )
                roleSupply->ResupplyStocks( *pDotationCategory, number );
        }
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveDestroyAll
// Created: NLD 2004-12-07
// -----------------------------------------------------------------------------
void  MIL_AgentPion::OnReceiveDestroyAll()
{
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
// Name: MIL_AgentPion::OnReceiveCreateWound
// Created: LDC 2010-07-02
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveCreateWound( const sword::MissionParameters& msg )
{
    if( msg.elem_size() > 2 )
        throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );

    if( msg.elem( 0 ).value_size() == 1 )
    {
        MIL_Injury_Wound* injury = new MIL_Injury_Wound( msg.elem( 0 ).value().Get( 0 ).identifier() );
        if( msg.elem_size() == 2 && msg.elem( 1 ).value_size() == 1 )
            injury->SetInjuryCategory( static_cast< MIL_MedicalTreatmentType::E_InjuryCategories >( msg.elem( 1 ).value().Get( 0 ).identifier() ) );
        else
            injury->SetInjuryCategory( static_cast< MIL_MedicalTreatmentType::E_InjuryCategories >( MIL_Random::rand32_oi( MIL_MedicalTreatmentType::eNone, MIL_MedicalTreatmentType::eDead ) ) );
        GetRole< PHY_RolePion_Composantes >().ApplyInjury( *injury );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveUnitMagicAction
// Created: JSR 2010-04-14
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveUnitMagicAction( const sword::UnitMagicAction& msg, const tools::Resolver< MIL_Army_ABC >& armies )
{
    switch( msg.type() )
    {
    case sword::surrender_to:
        pAutomate_->OnReceiveUnitMagicAction( msg, armies );
        return;
    case sword::cancel_surrender:
        pAutomate_->OnReceiveUnitMagicAction( msg, armies );
        return;
    case sword::recover_transporters:
        OnReceiveRecoverHumansTransporters();
        break;
    case sword::destroy_component:
        OnReceiveDestroyComponent();
        break;
    case sword::recover_all:
        OnReceiveResupplyAll();
        break;
    case sword::recover_troops:
        OnReceiveResupplyHumans();
        break;
    case sword::recover_equipments:
        OnReceiveResupplyEquipement();
        break;
    case sword::recover_resources:
        OnReceiveResupplyResources();
        break;
    case sword::destroy_all:
        OnReceiveDestroyAll();
        break;
    case sword::change_human_factors:
        OnReceiveChangeHumanFactors( msg.parameters() );
        break;
    case sword::partial_recovery:
        OnReceiveResupply( msg.parameters() );
        break;
    case sword::create_wound:
        OnReceiveCreateWound( msg.parameters() );
        break;
    case sword::unit_change_affinities:
        OnReceiveMsgChangeAffinities( msg );
        break;
    case sword::change_extension:
        pExtensions_->OnReceiveMsgChangeExtensions( msg );
        break;
    case sword::change_critical_intelligence:
        OnReceiveCriticalIntelligence( msg );
        break;
    case sword::reload_brain:
        CancelAllActions();
        GetDecision().Reload(); 
        pOrderManager_->CancelMission();
        break;
    case sword::create_breakdowns:
        OnReceiveCreateBreakdowns( msg.parameters() );
        break;
    case sword::create_wounds:
        OnReceiveCreateWounds( msg.parameters() );
        break;
    case sword::change_equipment_state:
        OnReceiveChangeEquipmentState( msg.parameters() );
        break;
    case sword::change_human_state:
        OnReceiveChangeHumanState( msg.parameters() );
        break;
    case sword::change_dotation:
        OnReceiveChangeDotation( msg.parameters() );
        break;
    case sword::create_direct_fire_order:
        OnReceiveCreateDirectFireOrder( msg.parameters() );
        break;
    case sword::change_equipment_human_size:
        OnReceiveChangeEquipmentHumanSize( msg.parameters() );
        break;
    case sword::load_unit:
        OnReceiveLoadUnit( msg.parameters() );
        break;
    case sword::log_finish_handlings:
        OnReceiveFinishLogisticHandlings();
        break;
    case sword::unload_unit:
        OnReceiveUnloadUnit( msg.parameters() );
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
    pOrderManager_->CancelMission();
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
// Name: MIL_AgentPion::OnReceiveChangeSuperior
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveChangeSuperior( const MIL_EntityManager& manager, const sword::UnitMagicAction& msg )
{
    MIL_Automate* pNewAutomate = manager.FindAutomate( msg.parameters().elem( 0 ).value().Get(0).automat().id() );
    if( !pNewAutomate )
        throw NET_AsnException< sword::HierarchyModificationAck::ErrorCode >( sword::HierarchyModificationAck::error_invalid_automate );
    if( pNewAutomate->GetArmy() != GetArmy() )
        throw NET_AsnException< sword::HierarchyModificationAck::ErrorCode >( sword::HierarchyModificationAck::error_parties_mismatched );
    if( pAutomate_ == pNewAutomate )
        return;
    ChangeSuperiorSilently( *pNewAutomate );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::ChangeSuperior
// Created: NLD 2004-10-26
// -----------------------------------------------------------------------------
void MIL_AgentPion::ChangeSuperior( MIL_Automate& newAutomate )
{
    if( pAutomate_ == &newAutomate )
        return;
    ChangeSuperiorSilently( newAutomate );
    client::UnitChangeSuperior asnMsg;
    asnMsg().mutable_unit()->set_id( GetID() );
    asnMsg().mutable_parent()->set_id( newAutomate.GetID() );
    asnMsg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::ChangeSuperiorSilently
// Created: LDC 2012-05-03
// -----------------------------------------------------------------------------
void MIL_AgentPion::ChangeSuperiorSilently( MIL_Automate& newAutomate )
{
    assert( GetArmy() == newAutomate.GetArmy() );
    SetPionAsCommandPost( false );
    pAutomate_->UnregisterPion( *this );
    pAutomate_ = &newAutomate;
    pAutomate_->RegisterPion( *this );
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
            MIL_Report::PostEvent( *this, MIL_Report::eReport_FiredByCivilian );
        else if( GetArmy().IsNeutral( attacker.GetArmy() ) == eTristate_True )
            MIL_Report::PostEvent( *this, MIL_Report::eReport_FiredByNeutralSide );
        else if( GetArmy().IsAFriend( attacker.GetArmy() ) == eTristate_True )
            MIL_Report::PostEvent( *this, MIL_Report::eReport_FiredByFriendSide );
        else if( GetArmy().IsAnEnemy( attacker.GetArmy() ) == eTristate_True )
            MIL_Report::PostEvent( *this, MIL_Report::eReport_FiredByEnemySide );
    }
    GetKnowledge().GetKsFire().NotifyAttackedBy( attacker );
    GetRole< PHY_RolePion_HumanFactors >().NotifyAttacked();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::NotifyAttackedBy
// Created: NLD 2005-12-01
// -----------------------------------------------------------------------------
void MIL_AgentPion::NotifyAttackedBy( MIL_Population& attacker )
{
    GetKnowledge().GetKsFire().NotifyAttackedBy( attacker );
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
        MIL_Report::PostEvent( *this, MIL_Report::eReport_FireOnCivilian );
    else if( GetArmy().IsNeutral( target.GetArmy() ) == eTristate_True )
        MIL_Report::PostEvent( *this, MIL_Report::eReport_FireOnNeutralSide );
    else if( GetArmy().IsAFriend( target.GetArmy() ) == eTristate_True )
        MIL_Report::PostEvent( *this, MIL_Report::eReport_FireOnFriendSide );
    else if( GetArmy().IsAnEnemy( target.GetArmy() ) == eTristate_True )
        MIL_Report::PostEvent( *this, MIL_Report::eReport_FireOnEnemySide );
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
// Name: MIL_AgentPion::InteractWithTraffic
// Created: JSR 2012-01-12
// -----------------------------------------------------------------------------
void MIL_AgentPion::InteractWithTraffic( const std::vector< TER_Agent_ABC* >& agents )
{
    assert( pType_ );
    double speedModifier = pType_->GetUnitType().GetSpeedModifier();
    if( speedModifier < 1. && CanInteractWithTraffic() )
    {
        MT_Vector2D position = GetRole< PHY_RoleInterface_Location >().GetPosition();
        MT_Vector2D direction = GetRole< PHY_RoleInterface_Location >().GetDirection();
        for( std::vector< TER_Agent_ABC* >::const_iterator itAgent = agents.begin(); itAgent != agents.end(); ++itAgent )
        {
            MIL_Agent_ABC& agent = static_cast< PHY_RoleInterface_Location& >( **itAgent ).GetAgent();
            if( &agent != this && agent.CanInteractWithTraffic() )
            {
                MT_Vector2D positionAgent = ( *itAgent )->GetPosition();
                unsigned int distance = pType_->GetUnitType().GetFootprintRadius() + agent.GetType().GetUnitType().GetFootprintRadius();
                if( position.SquareDistance( positionAgent ) < distance * distance )
                {
                    MT_Vector2D directionAgent = positionAgent - position;
                    if( directionAgent * direction > 0 )
                        GetRole< moving::PHY_RoleAction_Moving >().ApplyTrafficModifier();
                }
            }
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
    MIL_AgentPion* agent = pAutomate_->GetPionPC();
    return ( pAutomate_ && agent && ( *agent == *this ) );
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
        MT_Vector2D realFirerPosition = firerUrbanRole.GetFirerPosition( const_cast< MIL_Agent_ABC& >( target ) );
        MT_Vector2D realTargetPosition = targetUrbanRole.GetTargetPosition( const_cast< MIL_AgentPion& >( *this ) );
        MT_Vector3D vFirerPosition( realFirerPosition.rX_, realFirerPosition.rY_, GetRole< PHY_RoleInterface_Location >().GetAltitude() );
        MT_Vector3D vTargetPosition(  realTargetPosition.rX_, realTargetPosition.rY_, target.GetRole< PHY_RoleInterface_Location >().GetAltitude() );
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
    return *pHumanRepartition_;
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
void MIL_AgentPion::OnReceiveCriticalIntelligence( const sword::UnitMagicAction& msg )
{
    if( !msg.has_parameters() || msg.parameters().elem_size() != 1 )
        throw NET_AsnException< sword::UnitActionAck::ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
    criticalIntelligence_ = msg.parameters().elem( 0 ).value( 0 ).acharstr();
    client::UnitAttributes message;
    message().mutable_unit()->set_id( GetID() );
    message().set_critical_intelligence( criticalIntelligence_ );
    message.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::Register
// Created: LGY 2011-06-14
// -----------------------------------------------------------------------------
void MIL_AgentPion::Register( MissionController_ABC& pController )
{
    pOrderManager_->Register( pController );
}

// =============================================================================
// Logistic
// =============================================================================

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
    else
        MT_LOG_ERROR_MSG( "Unit " << GetID() << " is not a logistic unit." );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveMsgChangeAffinities
// Created: LGY 2012-05-09
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveMsgChangeAffinities( const sword::UnitMagicAction& msg )
{
    pAffinities_->OnReceiveMsgChangeAffinities( msg );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveCreateBreakdowns
// Created: ABR 2011-08-09
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveCreateBreakdowns( const sword::MissionParameters& msg )
{
    if( msg.elem( 0 ).value_size() < 1 )
        throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );

    PHY_RolePion_Composantes& roleComposantes = GetRole< PHY_RolePion_Composantes >();
    for( int i = 0; i < msg.elem( 0 ).value_size(); ++i )
    {
        const sword::MissionParameter_Value& elem = msg.elem( 0 ).value().Get( i );
        if( elem.list_size() < 2 || elem.list_size() > 3 || !elem.list( 0 ).has_identifier() || !elem.list( 1 ).has_quantity() )
            throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );

        sword::EquipmentType type;
        type.set_id( elem.list( 0 ).identifier() );
        int number = elem.list( 1 ).quantity();
        unsigned int breakdownId = 0;
        if( elem.list_size() == 3 )
        {
            if( !elem.list( 2 ).has_identifier() )
                throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
            breakdownId = elem.list( 2 ).identifier();
        }
        const PHY_ComposanteTypePion* pComposanteType = PHY_ComposanteTypePion::Find( type );
        if( !pComposanteType || !pComposanteType->CanHaveBreakdown() )
            throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
        roleComposantes.CreateBreakdowns( *pComposanteType, static_cast< unsigned int >( number ), breakdownId );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveCreateWounds
// Created: ABR 2011-08-09
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveCreateWounds( const sword::MissionParameters& msg )
{
    if( msg.elem( 0 ).value_size() < 1 )
        throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );

    PHY_RolePion_Composantes& roleComposantes = GetRole< PHY_RolePion_Composantes >();
    for( int i = 0; i < msg.elem( 0 ).value_size(); ++i )
    {
        const sword::MissionParameter_Value& elem = msg.elem( 0 ).value().Get( i );
        if( elem.list_size() < 1 || elem.list_size() > 2 || !elem.list( 0 ).has_quantity() )
            throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );

        int number = elem.list( 0 ).quantity();
        sword::EnumHumanWound wound = sword::unwounded;
        bool randomWound = ( elem.list_size() != 2 );
        if( !randomWound )
        {
            if( !elem.list( 1 ).has_enumeration() )
                throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
            wound = static_cast< sword::EnumHumanWound >( elem.list( 1 ).enumeration() );
        }
        roleComposantes.CreateWounds( static_cast< unsigned int >( number ), randomWound, wound );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveChangeEquipmentState
// Created: ABR 2011-08-09
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveChangeEquipmentState( const sword::MissionParameters& msg )
{
    if( msg.elem( 0 ).value_size() < 1 )
        throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );

    PHY_RolePion_Composantes& roleComposantes = GetRole< PHY_RolePion_Composantes >();
    for( int i = 0; i < msg.elem( 0 ).value_size(); ++i )
    {
        const sword::MissionParameter_Value& elem = msg.elem( 0 ).value().Get( i );
        if( elem.list_size() != 8 || !elem.list( 0 ).has_identifier() ||
            !elem.list( 1 ).has_quantity() || !elem.list( 2 ).has_quantity() || !elem.list( 3 ).has_quantity() ||
            !elem.list( 4 ).has_quantity() || !elem.list( 5 ).has_quantity() || !elem.list( 6 ).has_quantity() ||
            elem.list( 7 ).list_size() != elem.list( 3 ).quantity() )
            throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );

        sword::EquipmentType type;
        type.set_id( elem.list( 0 ).identifier() );
        const PHY_ComposanteTypePion* pComposanteType = PHY_ComposanteTypePion::Find( type );
        if( !pComposanteType )
            throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
        roleComposantes.ChangeEquipmentState( *pComposanteType, elem );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveChangeHumanState
// Created: ABR 2011-08-09
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveChangeHumanState( const sword::MissionParameters& msg )
{
    if( msg.elem( 0 ).value_size() < 1 )
        throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
    PHY_RolePion_Composantes& roleComposantes = GetRole< PHY_RolePion_Composantes >();
    for( int i = 0 ; i < msg.elem( 0 ).value_size(); ++i )
    {
        const sword::MissionParameter_Value& elem = msg.elem( 0 ).value().Get( i );
        if( elem.list_size() != 6 || !elem.list( 0 ).has_quantity() || !elem.list( 1 ).has_enumeration() || !elem.list( 2 ).has_enumeration() ||
            !elem.list( 4 ).has_booleanvalue() || !elem.list( 5 ).has_booleanvalue() )
            throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
        // rank
        if( elem.list( 1 ).enumeration() < sword::EnumHumanRank_MIN || elem.list( 1 ).enumeration() > sword::EnumHumanRank_MAX )
            throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
        const PHY_HumanRank* pHumanRank = PHY_HumanRank::Find( static_cast< unsigned int >( elem.list( 1 ).enumeration() ) );
        if( !pHumanRank )
            throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
        // state
        if( elem.list( 2 ).enumeration() < sword::EnumHumanState_MIN || elem.list( 2 ).enumeration() > sword::EnumHumanState_MAX )
            throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
        sword::EnumHumanState state = static_cast< sword::EnumHumanState >( elem.list( 2 ).enumeration() );
        if( state == sword::injured )
        {
            // $$$$ ABR 2011-08-10: waiting story 660, for each injury check the following
            if( elem.list( 3 ).list_size() != 1 || elem.list( 3 ).list( 0 ).list_size() != 2 ||
                !elem.list( 3 ).list( 0 ).list( 1 ).has_enumeration() ||
                elem.list( 3 ).list( 0 ).list( 1 ).enumeration() < sword::EnumInjuriesSeriousness_MIN ||
                elem.list( 3 ).list( 0 ).list( 1 ).enumeration() > sword::EnumInjuriesSeriousness_MAX )
                throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
        }
    }
    roleComposantes.ChangeHumanState( msg );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveChangeDotation
// Created: ABR 2011-08-09
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveChangeDotation( const sword::MissionParameters& msg )
{
    if( msg.elem( 0 ).value_size() < 1 )
        throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
    dotation::PHY_RolePion_Dotations& roleDotations = GetRole< dotation::PHY_RolePion_Dotations >();
    for( int i = 0; i < msg.elem( 0 ).value_size(); ++i )
    {
        const sword::MissionParameter_Value& elem = msg.elem( 0 ).value().Get( i );
        if( elem.list_size() != 3 || !elem.list( 0 ).has_identifier() || !elem.list( 1 ).has_quantity() || !elem.list( 2 ).has_areal() )
            throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
        unsigned int dotationId = elem.list( 0 ).identifier();
        int number = elem.list( 1 ).quantity();
        float thresholdPercentage = elem.list( 2 ).areal();

        const PHY_DotationCategory* pDotationCategory = PHY_DotationType::FindDotationCategory( dotationId );
        if( !pDotationCategory || number < 0 || thresholdPercentage < 0.f || thresholdPercentage > 100.f )
            throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
        roleDotations.ChangeDotation( *pDotationCategory, static_cast< unsigned int >( number ), thresholdPercentage );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveCreateDirectFireOrder
// Created: SLI 2011-09-23
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveCreateDirectFireOrder( const sword::MissionParameters& msg )
{
    // target
    if( msg.elem_size() < 1 || msg.elem( 0 ).value_size() != 1 || !msg.elem( 0 ).value( 0 ).has_agent() )
        throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
    MIL_AgentPion* target = MIL_AgentServer::GetWorkspace().GetEntityManager().FindAgentPion( msg.elem( 0 ).value( 0 ).agent().id() );
    if( target == 0 )
        throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
    boost::shared_ptr< MIL_KnowledgeGroup > knowledgeGroup = GetKnowledgeGroup();
    boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge = target->CreateKnowledge( knowledgeGroup );
    // firing mode
    firing::PHY_DirectFireData::E_FiringMode firingMode = firing::PHY_DirectFireData::eFiringModeNormal;
    if( msg.elem_size() >= 2 && msg.elem( 1 ).value_size() == 1 && msg.elem( 1 ).value( 1 ).has_enumeration() )
        firingMode = static_cast< firing::PHY_DirectFireData::E_FiringMode >( msg.elem( 1 ).value( 1 ).enumeration() );
    // component percentage to use
    double percentageToUse = 1.;
    if( msg.elem_size() >= 3 && msg.elem( 2 ).value_size() == 1 && msg.elem( 2 ).value( 1 ).has_areal() )
        percentageToUse = msg.elem( 2 ).value( 1 ).areal();
    // component firing type
    firing::PHY_DirectFireData::E_ComposanteFiringType firingType = firing::PHY_DirectFireData::eFireUsingAllComposantes;
    if( msg.elem_size() >= 4 && msg.elem( 3 ).value_size() == 1 && msg.elem( 3 ).value( 1 ).has_enumeration() )
        firingType = static_cast< firing::PHY_DirectFireData::E_ComposanteFiringType >( msg.elem( 3 ).value( 1 ).enumeration() );
    // component fired type
    firing::PHY_DirectFireData::E_ComposanteFiredType firedType = firing::PHY_DirectFireData::eFireOnAllComposantes;
    if( msg.elem_size() >= 5 && msg.elem( 4 ).value_size() == 1 && msg.elem( 4 ).value( 1 ).has_enumeration() )
        firedType = static_cast< firing::PHY_DirectFireData::E_ComposanteFiredType >( msg.elem( 4 ).value( 1 ).enumeration() );
    PHY_FireResults_Pion* results = 0;
    GetRole< firing::PHY_RoleAction_DirectFiring >().FirePion( pKnowledge, firingMode, percentageToUse, firingType, firedType, results, false );
    if( results )
    {
        results->IncRef();
        results->DecRef();
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveChangeEquipmentHumanSize
// Created: ABR 2011-12-02
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveChangeEquipmentHumanSize( const sword::MissionParameters& msg )
{
    if( msg.elem_size() != 2 || msg.elem( 0 ).value_size() != 1 || !msg.elem( 0 ).value( 0 ).has_equipmenttype() || 
        msg.elem( 1 ).value_size() != 1 || !msg.elem( 1 ).value( 0 ).has_quantity() )
        throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );

    PHY_RolePion_Composantes& roleComposantes = GetRole< PHY_RolePion_Composantes >();
    roleComposantes.ChangeHumanSize( msg.elem( 0 ).value( 0 ).equipmenttype().id(), msg.elem( 1 ).value( 0 ).quantity() );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::ChangeNationality
// Created: JSR 2011-12-09
// -----------------------------------------------------------------------------
void MIL_AgentPion::ChangeNationality( const std::string& nationality )
{
    pExtensions_->SetExtension( "Nationalite", nationality );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveChangeEquipmentHumanSize
// Created: NLD 2012-01-09
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveFinishLogisticHandlings()
{
    PHY_RoleInterface_Maintenance* roleMaintenance = RetrieveRole< PHY_RoleInterface_Maintenance >();
    if( roleMaintenance )
        roleMaintenance->FinishAllHandlingsSuccessfullyWithoutDelay();

    PHY_RoleInterface_Medical* roleMedical = RetrieveRole< PHY_RoleInterface_Medical >();
    if( roleMedical )
        roleMedical->FinishAllHandlingsSuccessfullyWithoutDelay();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveLoadUnit
// Created: SLI 2011-10-17
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveLoadUnit( const sword::MissionParameters& msg )
{
    if( msg.elem_size() < 1 || msg.elem( 0 ).value_size() != 1 || !msg.elem( 0 ).value( 0 ).has_agent() )
        throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
    MIL_AgentPion* target = MIL_AgentServer::GetWorkspace().GetEntityManager().FindAgentPion( msg.elem( 0 ).value( 0 ).agent().id() );
    if( target == 0 )
        throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
    transport::PHY_RoleAction_Transport* role = RetrieveRole< transport::PHY_RoleAction_Transport >();
    if( !role )
        throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
    role->MagicLoadPion( *target, false );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReceiveUnloadUnit
// Created: SLI 2011-10-17
// -----------------------------------------------------------------------------
void MIL_AgentPion::OnReceiveUnloadUnit( const sword::MissionParameters& msg )
{
    if( msg.elem_size() < 1 || msg.elem( 0 ).value_size() != 1 || !msg.elem( 0 ).value( 0 ).has_agent() )
        throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
    MIL_AgentPion* target = MIL_AgentServer::GetWorkspace().GetEntityManager().FindAgentPion( msg.elem( 0 ).value( 0 ).agent().id() );
    if( target == 0 )
        throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
    transport::PHY_RoleAction_Transport* role = RetrieveRole< transport::PHY_RoleAction_Transport >();
    if( !role )
        throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
    role->MagicUnloadPion( *target );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::IsCivilian
// Created: LDC 2012-03-13
// -----------------------------------------------------------------------------
bool MIL_AgentPion::IsCivilian() const
{
    return pHumanRepartition_->IsCivilian();
}
