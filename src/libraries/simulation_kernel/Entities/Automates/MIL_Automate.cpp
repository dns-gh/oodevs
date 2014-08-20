// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_Automate.h"

#include "CheckPoints/SerializationTools.h"
#include "DEC_AutomateDecision.h"
#include "Decision/DEC_Representations.h"
#include "Decision/DEC_Workspace.h"
#include "Entities/Actions/PHY_ActionLogistic.h"
#include "Entities/Agents/Actions/Moving/PHY_RoleAction_InterfaceMoving.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/MIL_AgentTypePion.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"
#include "Entities/Agents/Units/Logistic/PHY_LogisticLevel.h"
#include "Entities/Automates/MIL_DotationSupplyManager.h"
#include "Entities/Automates/MIL_StockSupplyManager.h"
#include "Entities/MIL_Army.h"
#include "Entities/MIL_EntityVisitor_ABC.h"
#include "Entities/MIL_Formation.h"
#include "Entities/Objects/LogisticAttribute.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Orders/MIL_AutomateOrderManager.h"
#include "Entities/Orders/MIL_Report.h"
#include "Entities/Specialisations/LOG/LogisticHierarchy.h"
#include "Entities/Specialisations/LOG/LogisticLink_ABC.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Automate.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "MIL_AutomateType.h"
#include "MIL_AgentServer.h"
#include "MissionController_ABC.h"
#include "MT_Tools/MT_FormatString.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "protocol/SimulationSenders.h"
#include "Tools/MIL_Color.h"
#include "Tools/MIL_DictionaryExtensions.h"
#include "Tools/MIL_MessageParameters.h"
#include "Tools/MIL_Tools.h"

#include <boost/foreach.hpp>

using namespace sword;

namespace
{
    template< typename R >
    void SaveRole( const MIL_Automate& automate, MIL_CheckPointOutArchive& file )
    {
        const R* const role = & automate.GetRole< R >();
        file << role;
    }
}

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_Automate )

template< typename Archive >
void save_construct_data( Archive& archive, const MIL_Automate* automat, const unsigned int /*version*/ )
{
    assert( automat->pType_ );
    unsigned int type = automat->pType_->GetID();
    unsigned int id = automat->GetID();
    const MissionController_ABC* const controller = &automat->pOrderManager_->GetController();
    archive << type
            << id
            << controller;
}

template< typename Archive >
void load_construct_data( Archive& archive, MIL_Automate* automat, const unsigned int /*version*/ )
{
    unsigned int type;
    unsigned int nID;
    MissionController_ABC* controller = 0;
    archive >> type
            >> nID
            >> controller;
    const MIL_AutomateType* pType = MIL_AutomateType::FindAutomateType( type );
    assert( pType );
    ::new( automat ) MIL_Automate( *pType, nID, *controller );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate constructor
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_Automate::MIL_Automate( const MIL_AutomateType& type,
                            unsigned int nID,
                            MissionController_ABC& controller,
                            MIL_Entity_ABC& parent,
                            xml::xistream& xis,
                            unsigned int gcPause,
                            unsigned int gcMult,
                            sword::DEC_Logger* logger )
    : MIL_Entity_ABC         ( xis, nID )
    , pType_                 ( &type )
    , pParentFormation_      ( dynamic_cast< MIL_Formation* >( &parent ) )
    , pParentAutomate_       ( dynamic_cast< MIL_Automate* >( &parent ) )
    , pOrderManager_         ( new MIL_AutomateOrderManager( controller, *this ) )
    , pPionPC_               ( 0 )
    , bEngaged_              ( true )
    , bAutomateModeChanged_  ( true )
    , pKnowledgeBlackBoard_  ( new DEC_KnowledgeBlackBoard_Automate( *this ) ) // $$$$ MCO : never deleted ?
    , pArmySurrenderedTo_    ( 0 )
    , pLogisticHierarchy_    ( new logistic::LogisticHierarchy( *this, false /* no quotas*/ ) )
    , pDotationSupplyManager_( new MIL_DotationSupplyManager( *this ) )
    , pStockSupplyManager_   ( new MIL_StockSupplyManager( *this ) )
    , pColor_                ( new MIL_Color( xis ) )
    , symbol_                ( xis.attribute< std::string >( "symbol", "" ) )
{
    Initialize( xis, gcPause, gcMult, logger );
    if( pParentFormation_ )
        pParentFormation_->RegisterAutomate( *this );
    else if( pParentAutomate_ )
        pParentAutomate_->RegisterAutomate( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate constructor
// Created: LDC 2009-04-24
// -----------------------------------------------------------------------------
MIL_Automate::MIL_Automate( const MIL_AutomateType& type,
                            unsigned int nID,
                            MissionController_ABC& controller )
    : MIL_Entity_ABC         ( "", nID )
    , pType_                 ( &type )
    , pParentFormation_      ( 0 )
    , pParentAutomate_       ( 0 )
    , bEngaged_              ( true )
    , pOrderManager_         ( new MIL_AutomateOrderManager( controller, *this ) )
    , pPionPC_               ( 0 )
    , bAutomateModeChanged_  ( true )
    , pKnowledgeBlackBoard_  ( 0 )
    , pArmySurrenderedTo_    ( 0 )
    , pLogisticHierarchy_    ( new logistic::LogisticHierarchy( *this, false /* no quotas*/ ) )
    , pDotationSupplyManager_( new MIL_DotationSupplyManager( *this ) )
    , pStockSupplyManager_   ( new MIL_StockSupplyManager( *this ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate constructor
// Created: LDC 2010-10-05
// -----------------------------------------------------------------------------
MIL_Automate::MIL_Automate( const MIL_AutomateType& type,
                            unsigned int nID,
                            MissionController_ABC& controller,
                            MIL_Entity_ABC& parent,
                            unsigned int knowledgeGroup,
                            const std::string& name,
                            unsigned int gcPause,
                            unsigned int gcMult,
                            sword::DEC_Logger* logger,
                            unsigned int context,
                            const MIL_DictionaryExtensions& extensions )
    : MIL_Entity_ABC         ( name, nID )
    , pType_                 ( &type )
    , pParentFormation_      ( dynamic_cast< MIL_Formation* >( &parent ) )
    , pParentAutomate_       ( dynamic_cast< MIL_Automate* >( &parent ) )
    , bEngaged_              ( true )
    , pOrderManager_         ( new MIL_AutomateOrderManager( controller, *this ) )
    , pPionPC_               ( 0 )
    , bAutomateModeChanged_  ( true )
    , pLogisticHierarchy_    ( new logistic::LogisticHierarchy( *this, false /* no quotas*/ ) )
    , pKnowledgeBlackBoard_  ( new DEC_KnowledgeBlackBoard_Automate( *this ) ) // $$$$ MCO : never deleted ?
    , pArmySurrenderedTo_    ( 0 )
    , pDotationSupplyManager_( new MIL_DotationSupplyManager( *this ) )
    , pStockSupplyManager_   ( new MIL_StockSupplyManager( *this ) )
    , pExtensions_           ( new MIL_DictionaryExtensions( extensions ) )
{
    pKnowledgeGroup_ = GetArmy().FindKnowledgeGroup( knowledgeGroup );
    if( !pKnowledgeGroup_ )
        throw MASA_EXCEPTION( "Unknown knowledge group" );
    pKnowledgeGroup_->RegisterAutomate( *this );

    RegisterRole( *new DEC_AutomateDecision( *this, gcPause, gcMult, logger ) ) ;
    RegisterRole( *new DEC_Representations() );

    if( pParentFormation_ )
    {
        pColor_.reset( new MIL_Color( pParentFormation_->GetColor() ) );
        pParentFormation_->RegisterAutomate( *this );
    }
    else if( pParentAutomate_ )
    {
        pColor_.reset( new MIL_Color( pParentAutomate_->GetColor() ) );
        pParentAutomate_->RegisterAutomate( *this );
    }

    if( type.IsLogisticBase() )
    {
        pBrainLogistic_.reset( new MIL_AutomateLOG( *this, PHY_LogisticLevel::logistic_base_ ) );
        pLogisticAction_.reset( new PHY_ActionLogistic<MIL_AutomateLOG>( *pBrainLogistic_ ) );
        RegisterAction( pLogisticAction_ );
        pLogisticHierarchy_.reset( new logistic::LogisticHierarchy( *this, *pBrainLogistic_, false /* no quotas*/ ) );
    }

    SendCreation ( context );
    SendFullState();
    SendKnowledge();
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate destructor
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_Automate::~MIL_Automate()
{
    if( pKnowledgeGroup_ )
        pKnowledgeGroup_->UnregisterAutomate( *this );
    if( pParentAutomate_ )
        pParentAutomate_->UnregisterAutomate( *this );
    if( pParentFormation_ )
        pParentFormation_->UnregisterAutomate( *this );
    delete pOrderManager_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetDecision
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
const DEC_Decision_ABC& MIL_Automate::GetDecision() const
{
    return GetRole< DEC_AutomateDecision >();
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetDecision
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
DEC_Decision_ABC& MIL_Automate::GetDecision()
{
    return GetRole< DEC_AutomateDecision >();
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::load
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_Automate::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< MIL_Entity_ABC >( *this );
    file >> boost::serialization::base_object< logistic::LogisticHierarchyOwner_ABC >( *this );
    {
        DEC_AutomateDecision* pRole;
        file >> pRole;
        RegisterRole( *pRole );
        RegisterRole( *new DEC_Representations() );
    }
    file >> pParentFormation_;
    file >> pParentAutomate_;
    file >> bEngaged_;
    file >> pKnowledgeGroup_;
    file >> pPionPC_;
    file >> pions_;
    file >> automates_;
    file >> bAutomateModeChanged_;
    file >> pKnowledgeBlackBoard_;
    file >> const_cast< MIL_Army_ABC*& >( pArmySurrenderedTo_ );
    file >> pExtensions_;
    file >> pColor_;
    file >> symbol_;
    file >> pLogisticHierarchy_;
    file >> pBrainLogistic_;
    file >> pDotationSupplyManager_;
    file >> pStockSupplyManager_;

    if( pBrainLogistic_ )
    {
        pLogisticAction_.reset( new PHY_ActionLogistic<MIL_AutomateLOG>( *pBrainLogistic_ ) );
        RegisterAction( pLogisticAction_ );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::save
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_Automate::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< MIL_Entity_ABC >( *this );
    file << boost::serialization::base_object< logistic::LogisticHierarchyOwner_ABC >( *this );
    SaveRole< DEC_AutomateDecision >( *this, file );
    file << pParentFormation_;
    file << pParentAutomate_;
    file << bEngaged_;
    file << pKnowledgeGroup_;
    file << pPionPC_;
    file << pions_;
    file << automates_;
    file << bAutomateModeChanged_;
    file << pKnowledgeBlackBoard_;
    file << pArmySurrenderedTo_;
    file << pExtensions_;
    file << pColor_;
    file << symbol_;
    file << pLogisticHierarchy_;
    file << pBrainLogistic_;
    file << pDotationSupplyManager_;
    file << pStockSupplyManager_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::Initialize
// Created: NLD 2007-03-29
// -----------------------------------------------------------------------------
void MIL_Automate::Initialize( xml::xistream& xis, unsigned int gcPause, unsigned int gcMult, sword::DEC_Logger* logger )
{
    xis >> xml::optional >> xml::attribute( "engaged", bEngaged_ );
    unsigned int nKnowledgeGroup;
    xis >> xml::attribute( "knowledge-group", nKnowledgeGroup );
    RegisterRole( *new DEC_AutomateDecision( *this, gcPause, gcMult, logger ) ) ;
    RegisterRole( *new DEC_Representations() );
    xis >> xml::list( "unit", *this, &MIL_Automate::ReadUnitSubordinate )
        >> xml::list( "automat", *this, &MIL_Automate::ReadAutomatSubordinate );
    pExtensions_.reset( new MIL_DictionaryExtensions( xis ) );
    if( !pPionPC_ )
        throw MASA_EXCEPTION( MT_FormatString( "Automat with id %d has no command post", GetID() ) );
    pKnowledgeGroup_ = GetArmy().FindKnowledgeGroup( nKnowledgeGroup );
    if( !pKnowledgeGroup_ )
        throw MASA_EXCEPTION( MT_FormatString( "Automat with id %d has no knowledge group", GetID() ) );
    std::string logLevelStr = PHY_LogisticLevel::none_.GetName();
    xis >> xml::optional >> xml::attribute( "logistic-level", logLevelStr );
    const PHY_LogisticLevel* pLogLevel = PHY_LogisticLevel::Find( logLevelStr );
    if( !pLogLevel )
        throw MASA_EXCEPTION( MT_FormatString( "Automat with id %d has an invalid logistic level", GetID() ) );
    if( *pLogLevel != PHY_LogisticLevel::none_ )
    {
        pBrainLogistic_.reset( new MIL_AutomateLOG( *this, *pLogLevel ) );
        pLogisticAction_.reset( new PHY_ActionLogistic<MIL_AutomateLOG>( *pBrainLogistic_ ) );
        RegisterAction( pLogisticAction_ );
        pLogisticHierarchy_.reset( new logistic::LogisticHierarchy( *this, *pBrainLogistic_, false /* no quotas*/, xis ) );
    }
    pKnowledgeGroup_->RegisterAutomate( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::ReadUnitSubordinate
// Created: ABL 2007-07-10
// -----------------------------------------------------------------------------
void MIL_Automate::ReadUnitSubordinate( xml::xistream& xis )
{
    const MIL_AgentTypePion* pType = MIL_AgentTypePion::Find( xis.attribute< std::string >( "type" ) );
    if( !pType )
    {
        MT_LOG_ERROR_MSG( "Automate: Unit not created - Unknown type : " <<  xis.attribute< std::string >( "type" ) );
        return;
    }
    MIL_AgentServer::GetWorkspace().GetEntityManager().CreatePion( *pType, *this, xis ); // Auto-registration
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::ReadAutomatSubordinate
// Created: NLD 2007-03-29
// -----------------------------------------------------------------------------
void MIL_Automate::ReadAutomatSubordinate( xml::xistream& xis )
{
    MIL_AgentServer::GetWorkspace().GetEntityManager().CreateAutomat( xis, *this ); // Auto-registration
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::DestroyPion
// Created: NLD 2005-02-08
// -----------------------------------------------------------------------------
void MIL_Automate::DestroyPion( MIL_AgentPion& pion )
{
    pionsToDelete_.push_back( &pion );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::WriteODB
// Created: NLD 2006-05-29
// -----------------------------------------------------------------------------
void MIL_Automate::WriteODB( xml::xostream& xos ) const
{
    assert( pType_ );
    assert( pKnowledgeGroup_ );
    xos << xml::start( "automat" );
    MIL_Entity_ABC::WriteODB( xos ) ;
    xos     << xml::attribute( "id", GetID() )
            << xml::attribute( "engaged", bEngaged_ )
            << xml::attribute( "knowledge-group", pKnowledgeGroup_->GetId() );
    if( pBrainLogistic_ )
        xos << xml::attribute( "logistic-level", pBrainLogistic_->GetLogisticLevel().GetName() );
    xos << xml::attribute( "type", pType_->GetName() );
    if( !symbol_.empty() )
        xos << xml::attribute( "symbol", symbol_ );
    pColor_->WriteODB( xos );
    for( auto it = automates_.begin(); it != automates_.end(); ++it )
        ( **it ).WriteODB( xos );
    for( auto it = pions_.begin(); it != pions_.end(); ++it )
        ( **it ).WriteODB( xos );
    pExtensions_->WriteODB( xos );
    xos << xml::end; // automat
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::ReadOverloading
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
void MIL_Automate::ReadOverloading( xml::xistream& /*refMission*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::ReadLogisticLink
// Created: NLD 2006-10-19
// -----------------------------------------------------------------------------
void MIL_Automate::ReadLogisticLink( MIL_AutomateLOG& superior, xml::xistream& xis )
{
    if( pBrainLogistic_ )
        throw MASA_EXCEPTION( xis.context() + "The logistic link of a logistic automat (usually a TC2) must not be overwritten: it should always be itself" );
    pLogisticHierarchy_.reset( new logistic::LogisticHierarchy( *this, superior, false /* no quotas*/, xis ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::WriteLogisticLinksODB
// Created: NLD 2006-10-19
// -----------------------------------------------------------------------------
void MIL_Automate::WriteLogisticLinksODB( xml::xostream& xos ) const
{
    if( pBrainLogistic_ )
        pBrainLogistic_->WriteLogisticLinksODB( xos );
    else
        pLogisticHierarchy_->WriteODB( xos );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::Finalize
// Created: JSR 2013-01-22
// -----------------------------------------------------------------------------
void MIL_Automate::Finalize()
{
    for( auto itPion = pions_.begin(); itPion != pions_.end(); ++itPion )
        ( *itPion )->Finalize();
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

// -----------------------------------------------------------------------------
// Name: MIL_Automate::UpdateDecision
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
void MIL_Automate::UpdateDecision( float duration )
{
    try
    {
        pOrderManager_->Update();
        GetRole< DEC_Decision_ABC >().UpdateDecision( duration );
    }
    catch( const std::exception& /*e*/ )
    {
        pOrderManager_->CancelMission();
        MT_LOG_ERROR_MSG( "Entity " << GetID() << "('" << GetName() << "') : Mission impossible" );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::UpdateKnowledges
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void MIL_Automate::UpdateKnowledges( int currentTimeStep )
{
    // Pions (+ PC)
    for( auto it = pions_.begin(); it != pions_.end(); ++it )
        ( **it ).UpdateKnowledges( currentTimeStep );
    assert( pKnowledgeBlackBoard_ );
    pKnowledgeBlackBoard_->Update( currentTimeStep );
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
    for( auto itPion = pions_.begin(); itPion != pions_.end(); ++itPion )
        ( **itPion ).CleanKnowledges();
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::CleanDeletedAgentKnowledges
// Created: JSR 2013-02-21
// -----------------------------------------------------------------------------
void MIL_Automate::CleanDeletedAgentKnowledges()
{
    for( auto it = pions_.begin(); it != pions_.end(); ++it )
        ( *it )->CleanDeletedAgentKnowledges();
    pKnowledgeBlackBoard_->CleanDeletedAgentKnowledges();
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::DeleteRequestsForRequester
// Created: JSR 2013-03-01
// -----------------------------------------------------------------------------
void MIL_Automate::DeleteRequestsForRequester( MIL_AgentPion& pion )
{
    pDotationSupplyManager_->DeleteRequestsForRequester( pion );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::UpdateNetwork
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
void MIL_Automate::UpdateNetwork() const
{
    try
    {
        client::AutomatAttributes msg;
        msg().mutable_automat()->set_id( GetID() );
        bool mustSend = bAutomateModeChanged_ || GetRole< DEC_AutomateDecision >().HasStateChanged() || pExtensions_->HasChanged();
        if( mustSend )
        {
            if( bAutomateModeChanged_ )
                msg().set_mode( bEngaged_ ? sword::engaged : sword::disengaged );
            GetRole< DEC_AutomateDecision >().SendChangedState( msg );
            pExtensions_->UpdateNetwork( msg );
        }
        if( pBrainLogistic_ )
            mustSend |= pBrainLogistic_->SendChangedState( msg );
        else
            pLogisticHierarchy_->SendChangedState();
        if( mustSend )
            msg.Send( NET_Publisher_ABC::Publisher() );
        pDotationSupplyManager_->SendChangedState();
        pStockSupplyManager_->SendChangedState();
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Error updating network for " << GetID() << " : " << tools::GetExceptionMsg( e ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::UpdateState
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
void MIL_Automate::UpdateState()
{
    try
    {
        pDotationSupplyManager_->Update();
        pStockSupplyManager_->Update();
        for( auto it = pionsToDelete_.begin(); it != pionsToDelete_.end(); ++it )
            ( *it )->DeleteUnit( 0, 0 );
        pionsToDelete_.clear();
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Error updating automat " << GetID() << " : " << tools::GetExceptionMsg( e ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::Clean
// Created: NLD 2005-01-07
// -----------------------------------------------------------------------------
void MIL_Automate::Clean()
{
    if( pBrainLogistic_ )
        pBrainLogistic_->Clean();
    bAutomateModeChanged_ = false;
    pDotationSupplyManager_->Clean();
    pStockSupplyManager_->Clean();
    DEC_AutomateDecision* roleDec = RetrieveRole< DEC_AutomateDecision >();
    if( roleDec )
        roleDec->Clean();
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::NotifyDotationSupplyNeeded
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
void MIL_Automate::NotifyDotationSupplyNeeded( const PHY_DotationCategory& dotationCategory )
{
    pDotationSupplyManager_->NotifyDotationSupplyNeeded( dotationCategory );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::RequestDotationSupply
// Created: NLD 2005-03-03
// -----------------------------------------------------------------------------
void MIL_Automate::RequestDotationSupply()
{
    pDotationSupplyManager_->RequestDotationSupply();
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::NotifyStockSupplyNeeded
// Created: AHC 2010-09-27
// -----------------------------------------------------------------------------
void MIL_Automate::NotifyStockSupplyNeeded( const PHY_DotationCategory& dotationCategory )
{
    pStockSupplyManager_->NotifyStockSupplyNeeded( dotationCategory );
}

//-----------------------------------------------------------------------------
// Name: MIL_Automate::Disengage
// Created: NLD 2003-04-11
//-----------------------------------------------------------------------------
void MIL_Automate::Disengage()
{
    pOrderManager_->CancelMission();
    if( bEngaged_ )
    {
        bEngaged_ = false;
        bAutomateModeChanged_ = true;
    }
}

//-----------------------------------------------------------------------------
// Name: MIL_Automate::Engage
// Created: NLD 2003-04-11
//-----------------------------------------------------------------------------
void MIL_Automate::Engage()
{
    for( auto it = automates_.begin(); it != automates_.end(); ++it )
        ( **it ).Engage();
    pOrderManager_->CancelMission();
    for( auto it = pions_.begin(); it != pions_.end(); ++it )
        ( **it ).GetOrderManager().CancelMission();
    if( !bEngaged_ )
    {
        bEngaged_ = true;
        bAutomateModeChanged_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::IsPerceived
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
bool MIL_Automate::IsPerceived( const DEC_Knowledge_Object& knowledge ) const
{
    for( auto it = pions_.begin(); it != pions_.end(); ++it )
        if( ( **it ).GetRole< PHY_RoleInterface_Perceiver >().ComputePerception( knowledge ) != PHY_PerceptionLevel::notSeen_ )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetArmy
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
MIL_Army_ABC& MIL_Automate::GetArmy() const
{
    assert( pParentFormation_ || pParentAutomate_ );
    if( pParentFormation_ )
        return pParentFormation_->GetArmy();
    return pParentAutomate_->GetArmy();
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::NotifyRefugeeOrientated
// Created: NLD 2007-02-15
// -----------------------------------------------------------------------------
void MIL_Automate::NotifyRefugeeOriented( const MIL_AgentPion& pionManaging )
{
    if( !pType_->IsRefugee() )
        return;
    pLogisticHierarchy_->SwitchToHierarchy( pionManaging.GetLogisticHierarchy() );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::NotifyRefugeeReleased
// Created: NLD 2007-02-15
// -----------------------------------------------------------------------------
void MIL_Automate::NotifyRefugeeReleased()
{
    if( !pType_->IsRefugee() )
        return;
    pLogisticHierarchy_->SwitchBackToNominalHierarchy();
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::NotifyRefugeeReleased
// Created: NLD 2007-02-15
// -----------------------------------------------------------------------------
void MIL_Automate::NotifyRefugeeReleased( const MIL_Object_ABC& camp )
{
    if( !pType_->IsRefugee() )
        return;
    pLogisticHierarchy_->SwitchToHierarchy( camp.GetAttribute< LogisticAttribute >().GetLogisticHierarchy() );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::Surrender
// Created: NLD 2005-02-24
// -----------------------------------------------------------------------------
void MIL_Automate::Surrender( const MIL_Army_ABC& armySurrenderedTo )
{
    if( pArmySurrenderedTo_ )
        return;
    pOrderManager_->CancelMission();
    pArmySurrenderedTo_ = &armySurrenderedTo;
    pLogisticHierarchy_->DisconnectFromHierarchy();
}

namespace
{
    class SurrenderedEntitiesVisitor : public MIL_EntityVisitor_ABC< MIL_AgentPion >
    {
    public:
        explicit SurrenderedEntitiesVisitor( MIL_AgentPion& pion ) : pion_( pion ) {}
        virtual ~SurrenderedEntitiesVisitor() {}
        virtual void Visit( const MIL_AgentPion& element )
        {
            auto knowledgeGroup = element.GetKnowledgeGroup();
            if( knowledgeGroup )
            {
                const PHY_PerceptionLevel& level = knowledgeGroup->GetPerceptionLevel( pion_ );
                if( level == PHY_PerceptionLevel::identified_ )
                    MIL_Report::PostEvent( element, report::eRC_IdentifiedUnitSurrendered, pion_ );
                else if( level == PHY_PerceptionLevel::recognized_ )
                    MIL_Report::PostEvent( element, report::eRC_RecognizedUnitSurrendered );
            }
        }
    private:
        MIL_AgentPion& pion_;
    };
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::SurrenderWithUnits
// Created: MMC 2013-04-02
// -----------------------------------------------------------------------------
void MIL_Automate::SurrenderWithUnits( const MIL_Army_ABC& armySurrenderedTo )
{
    Surrender( armySurrenderedTo );
    for( auto itPion = pions_.begin(); itPion != pions_.end(); ++itPion )
    {
        MIL_AgentPion& agent = **itPion;
        agent.UpdateSurrenderedState();
        SurrenderedEntitiesVisitor visitor( agent );
        const MIL_Army& army = static_cast< const MIL_Army& >( armySurrenderedTo );
        auto itFormation = army.tools::Resolver< MIL_Formation >::CreateIterator();
        while( itFormation.HasMoreElements() )
        {
            const MIL_Formation& formation = itFormation.NextElement();
            formation.Apply( visitor );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::CancelSurrender
// Created: NLD 2007-02-15
// -----------------------------------------------------------------------------
void MIL_Automate::CancelSurrender()
{
    if( pArmySurrenderedTo_ )
    {
        pArmySurrenderedTo_ = 0;
        pLogisticHierarchy_->SwitchBackToNominalHierarchy();
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::NotifyCaptured
// Created: NLD 2005-03-04
// -----------------------------------------------------------------------------
bool MIL_Automate::NotifyCaptured( const MIL_AgentPion& pionTakingPrisoner )
{
    if( !IsSurrendered() )
        return false;
    pLogisticHierarchy_->SwitchToHierarchy( pionTakingPrisoner.GetLogisticHierarchy() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::NotifyReleased
// Created: NLD 2007-02-14
// -----------------------------------------------------------------------------
bool MIL_Automate::NotifyReleased()
{
    if( !IsSurrendered() )
        return false;
    pLogisticHierarchy_->SwitchBackToNominalHierarchy();
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::NotifyImprisoned
// Created: NLD 2005-03-07
// -----------------------------------------------------------------------------
bool MIL_Automate::NotifyImprisoned( const MIL_Object_ABC& camp )
{
    if( !IsSurrendered() )
        return false;
    pLogisticHierarchy_->SwitchToHierarchy( camp.GetAttribute< LogisticAttribute >().GetLogisticHierarchy() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetAlivePionsMaxSpeed
// Created: NLD 2007-04-30
// -----------------------------------------------------------------------------
double MIL_Automate::GetAlivePionsMaxSpeed() const
{
    double maxSpeed = std::numeric_limits< double >::max();
    for( auto it = pions_.begin(); it != pions_.end(); ++it )
    {
        const double speed = (*it)->GetRole< moving::PHY_RoleAction_InterfaceMoving >().GetMaxSpeed();
        if( speed != 0 )
            maxSpeed = std::min( maxSpeed, speed );
    }
    return maxSpeed;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetAlivePionsBarycenter
// Created: NLD 2004-11-08
// -----------------------------------------------------------------------------
bool MIL_Automate::GetAlivePionsBarycenter( MT_Vector2D& barycenter ) const
{
    barycenter.Reset();
    unsigned int nTmp = 0;
    for( auto itPion = pions_.begin(); itPion != pions_.end(); ++itPion )
    {
        if( !( **itPion ).IsDead() )
        {
            barycenter += ( **itPion ).GetRole< PHY_RoleInterface_Location >().GetPosition();
            ++nTmp ;
        }
    }
    if( nTmp == 0 )
        return false;
    barycenter /= nTmp;
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::SendCreation
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MIL_Automate::SendCreation( unsigned int context ) const
{
    client::AutomatCreation message;
    message().mutable_automat()->set_id( GetID() );
    message().mutable_type()->set_id( pType_->GetID() );
    message().mutable_party()->set_id( GetArmy().GetID() );
    message().mutable_knowledge_group()->set_id( GetKnowledgeGroup()->GetId() );
    message().set_symbol( symbol_ );
    message().set_app6symbol( symbol_ );
    message().set_logistic_level( pBrainLogistic_ ?
        (sword::EnumLogisticLevel)pBrainLogistic_->GetLogisticLevel().GetID() : sword::none );
    message().set_log_maintenance_manual( pBrainLogistic_ && pBrainLogistic_->IsMaintenanceManual() );
    message().set_log_supply_manual( pBrainLogistic_ && pBrainLogistic_->IsSupplyManual() );
    message().set_name( GetName() );
    pColor_->SendFullState( message );
    pExtensions_->SendFullState( message );
    assert( pParentAutomate_ || pParentFormation_ );
    if( pParentAutomate_ )
        message().mutable_parent()->mutable_automat()->set_id( pParentAutomate_->GetID() );
    else if( pParentFormation_ )
        message().mutable_parent()->mutable_formation()->set_id( pParentFormation_->GetID() );
    message.Send( NET_Publisher_ABC::Publisher(), context );
    for( auto it = automates_.begin(); it != automates_.end(); ++it )
        ( **it ).SendCreation( context );
    for( auto it = pions_.begin(); it != pions_.end(); ++it )
        ( **it ).SendCreation( context );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::SendFullState
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void MIL_Automate::SendFullState( unsigned int contex ) const
{
    client::AutomatAttributes message;
    message().mutable_automat()->set_id( GetID() );
    message().set_mode( bEngaged_ ? sword::engaged : sword::disengaged );
    const DEC_AutomateDecision* roleDec = RetrieveRole< DEC_AutomateDecision >();
    if( roleDec )
        roleDec->SendFullState( message );
    pExtensions_->SendFullState( message );
    message.Send( NET_Publisher_ABC::Publisher() );

    pLogisticHierarchy_->SendFullState();
    if( pBrainLogistic_ )
        pBrainLogistic_->SendFullState( message );
    pDotationSupplyManager_->SendFullState();
    pStockSupplyManager_->SendFullState();

    for( auto it = automates_.begin(); it != automates_.end(); ++it )
        ( **it ).SendFullState();
    for( auto it = pions_.begin(); it != pions_.end(); ++it )
        ( **it ).SendFullState( contex );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::SendKnowledge
// Created: NLD 2006-10-13
// -----------------------------------------------------------------------------
void MIL_Automate::SendKnowledge( unsigned int context ) const
{
    assert( pKnowledgeBlackBoard_ );
    pKnowledgeBlackBoard_->SendFullState( context );
    for( auto it = pions_.begin(); it != pions_.end(); ++it )
        ( **it ).SendKnowledge( context );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::OnReceiveSetAutomateMode
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void MIL_Automate::OnReceiveSetAutomateMode( const sword::SetAutomatMode& asnMsg )
{
    if( pParentAutomate_ && pParentAutomate_->IsEngaged() )
        throw MASA_EXCEPTION_ASN( sword::SetAutomatModeAck_ErrorCode, sword::SetAutomatModeAck::error_not_allowed );
    switch( asnMsg.mode() )
    {
    case sword::disengaged:
        Disengage();
        break;
    case sword::engaged:
        Engage();
        break;
    default:
        assert( false );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::OnReceiveUnitCreationRequest
// Created: AGE 2007-06-18
// -----------------------------------------------------------------------------
void MIL_Automate::OnReceiveUnitCreationRequest( const sword::UnitCreationRequest& msg, unsigned int nCtx )
{
    const MIL_AgentTypePion* pType = MIL_AgentTypePion::Find( msg.type().id() );
    if( !pType )
        throw MASA_EXCEPTION_ASN( sword::UnitActionAck_ErrorCode, sword::UnitActionAck::error_invalid_unit );
    MT_Vector2D position;
    NET_ASN_Tools::ReadPoint( msg.position(), position );
    MIL_AgentServer::GetWorkspace().GetEntityManager().CreatePion( *pType, *this, position, boost::none, nCtx, 0 ); // Auto-registration
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::OnReceiveUnitCreationRequest
// Created: JSR 2010-04-16
// -----------------------------------------------------------------------------
unsigned int MIL_Automate::OnReceiveUnitCreationRequest( const sword::UnitMagicAction& msg, unsigned int nCtx )
{
    if( msg.type() != sword::UnitMagicAction_Type_unit_creation || !msg.has_parameters() )
        throw MASA_BADPARAM_UNIT( "invalid message type" );
    const sword::MissionParameters& parameters = msg.parameters();
    protocol::CheckCount( parameters, 2, 5 );
    const uint32_t id = protocol::GetIdentifier( parameters, 0 );
    const MIL_AgentTypePion* pType = MIL_AgentTypePion::Find( id );
    protocol::Check( pType, "invalid unit type" );
    const sword::CoordLatLong point = protocol::GetPoint( parameters, 1 );
    MT_Vector2D position;
    MIL_Tools::ConvertCoordMosToSim( point, position );
    try
    {
        MIL_AgentPion* pion = 0;
        if( !protocol::IsNull( parameters, 2 ) )
        {
            MIL_DictionaryExtensions extensions;
            if( !protocol::IsNull( parameters, 4 ) )
                extensions.ReadExtensions( protocol::GetExtensionList( parameters, 4 ) );
            const std::string name = protocol::GetString( parameters, 2 );
            // Auto-registration
            pion = &MIL_AgentServer::GetWorkspace().GetEntityManager().CreatePion(
                    *pType, *this, position, name, nCtx, &extensions );
            if( !protocol::IsNull( parameters, 3 ) )
                pion->SetPionAsCommandPost( protocol::GetBool( parameters, 3 ) );
        }
        else
        {
            // Auto-registration
            pion = &MIL_AgentServer::GetWorkspace().GetEntityManager()
                .CreatePion( *pType, *this, position, boost::none, nCtx, 0 );
        }
        const MIL_AutomateLOG* superior = pLogisticHierarchy_->GetPrimarySuperior();
        if( superior && superior->GetPC() )
            MIL_Report::PostEvent( *superior->GetPC(), report::eRC_LogSuperiorAdded, *pion );
        return pion->GetID();
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( tools::GetExceptionMsg( e ) );
        throw MASA_EXCEPTION_ASN( sword::UnitActionAck_ErrorCode, sword::UnitActionAck::error_invalid_unit );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::OnReceiveUnitMagicAction
// Created: JSR 2010-04-14
// -----------------------------------------------------------------------------
void MIL_Automate::OnReceiveUnitMagicAction( const sword::UnitMagicAction& msg, const tools::Resolver< MIL_Army_ABC >& armies )
{
    switch( msg.type() )
    {
    case sword::UnitMagicAction::surrender_to:
        {
            if( !msg.has_parameters() || msg.parameters().elem_size() != 1 )
                throw MASA_BADPARAM_UNIT( "invalid parameters count" );
            const sword::MissionParameter& id = msg.parameters().elem( 0 );
            if( id.value_size() != 1 || !id.value( 0 ).has_party() )
                throw MASA_BADPARAM_UNIT( "parameters[0] must be a Party" );
            const MIL_Army_ABC* pSurrenderedToArmy = armies.Find( msg.parameters().elem( 0 ).value( 0 ).party().id() );
            if( !pSurrenderedToArmy )
                throw MASA_BADPARAM_UNIT( "party not found" );
            else if( *pSurrenderedToArmy == GetArmy() )
                throw MASA_BADPARAM_UNIT( "cannot surrender to your own party" );
            else if( IsSurrendered() )
            {
                // Surrendering to the same party again is a no-op
                if( pArmySurrenderedTo_->GetID() != pSurrenderedToArmy->GetID()  )
                    throw MASA_BADPARAM_ASN( sword::UnitActionAck_ErrorCode,
                        sword::UnitActionAck::error_unit_surrendered,
                        "automat already surrendered" );
            }
            else
                SurrenderWithUnits( *pSurrenderedToArmy );
        }
        break;
    case sword::UnitMagicAction::cancel_surrender:
        // Cancelling the surrender of a free automat is a no-op
        if( IsSurrendered() )
        {
            CancelSurrender();
            for( auto itPion = pions_.begin(); itPion != pions_.end(); ++itPion )
                ( **itPion ).UpdateSurrenderedState();
        }
        break;
    case sword::UnitMagicAction::change_extension:
        pExtensions_->OnReceiveMsgChangeExtensions( msg );
        break;
    case sword::UnitMagicAction::reload_brain:
        OnReloadBrain( msg.parameters() );
        break;
    case sword::UnitMagicAction::change_brain_debug:
        OnChangeBrainDebug( msg.parameters() );
        break;
    case sword::UnitMagicAction::log_finish_handlings:
        if( !pBrainLogistic_ )
            throw MASA_BADUNIT_UNIT( "automat must be a logistic base" );
        if( ! pBrainLogistic_->FinishAllHandlingsSuccessfullyWithoutDelay() )
            throw MASA_BADUNIT_UNIT( "automat must have logistic handlings pending" );
        break;
    case sword::UnitMagicAction::log_maintenance_set_manual:
        if( !pBrainLogistic_ )
            throw MASA_BADUNIT_UNIT( "automat must be a logistic base" );
        pBrainLogistic_->OnReceiveLogMaintenanceSetManual( msg.parameters() );
        break;
    case sword::UnitMagicAction::log_supply_set_manual:
        if( !pBrainLogistic_ )
            throw MASA_BADUNIT_UNIT( "automat must be a logistic base" );
        pBrainLogistic_->OnReceiveLogSupplySetManual( msg.parameters() );
        break;
    default:
        {
            if( !pPionPC_ )
                throw MASA_EXCEPTION_ASN( sword::UnitActionAck_ErrorCode, sword::UnitActionAck::error_invalid_unit );
            pPionPC_->OnReceiveUnitMagicAction( msg, armies, 0, 0 );
        }
        break;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::OnReceiveMagicActionMoveTo
// Created: JSR 2010-04-07
// -----------------------------------------------------------------------------
void MIL_Automate::OnReceiveMagicActionMoveTo( const sword::UnitMagicAction& msg )
{
    if( msg.type() != sword::UnitMagicAction::move_to )
        throw MASA_EXCEPTION_ASN( sword::UnitActionAck_ErrorCode, sword::UnitActionAck::error_invalid_parameter );
    if( !msg.has_parameters() || msg.parameters().elem_size() != 1 )
        throw MASA_EXCEPTION_ASN( sword::UnitActionAck_ErrorCode, sword::UnitActionAck::error_invalid_parameter );
    const sword::MissionParameter& parametre = msg.parameters().elem( 0 );
    if( parametre.value_size() != 1 || !parametre.value(0).has_point() )
        throw MASA_EXCEPTION_ASN( sword::UnitActionAck_ErrorCode, sword::UnitActionAck::error_invalid_parameter );
    const sword::Point& point = parametre.value(0).point();
    if( point.location().type() != sword::Location::point || point.location().coordinates().elem_size() != 1 )
        throw MASA_EXCEPTION_ASN( sword::UnitActionAck_ErrorCode, sword::UnitActionAck::error_invalid_parameter );
    MT_Vector2D vPosTmp;
    MIL_Tools::ConvertCoordMosToSim( point.location().coordinates().elem( 0 ), vPosTmp );
    if( !pPionPC_ )
        throw MASA_EXCEPTION_ASN( sword::UnitActionAck_ErrorCode, sword::UnitActionAck::error_invalid_parameter );
    const MT_Vector2D vTranslation( vPosTmp - pPionPC_->GetRole< PHY_RoleInterface_Location >().GetPosition() );
    bool bCancelMission = !IsMasaLife();
    for( auto it = pions_.begin(); it != pions_.end(); ++it )
    {
        (*it )->OnReceiveMagicActionMoveTo( (*it )->GetRole< PHY_RoleInterface_Location >().GetPosition() + vTranslation );
        if( !(*it)->IsMasaLife() )
            bCancelMission = true;
    }
    if( bCancelMission )
        pOrderManager_->CancelMission();
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::OnReceiveChangeKnowledgeGroup
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void MIL_Automate::OnReceiveChangeKnowledgeGroup( const sword::UnitMagicAction& msg )
{
    if( msg.type() != sword::UnitMagicAction::change_knowledge_group )
        throw MASA_BADPARAM_UNIT( "invalid message type" );
    if( !msg.has_parameters() || msg.parameters().elem_size() < 1 )
        throw MASA_BADPARAM_UNIT( "invalid parameters count" );
    // change_knowledge_group used to receive the knowledge group and its
    // party identifier, in any order. Tolerate the former behaviour.
    unsigned int knowledgeGroupId = 0;
    for( int i = 0; i != msg.parameters().elem_size(); ++i )
    {
        const auto& elem = msg.parameters().elem( i );
        if( elem.value_size() != 1 || !elem.value( 0 ).has_knowledgegroup() )
            continue;
        knowledgeGroupId = elem.value( 0 ).knowledgegroup().id();
        break;
    }
    if( !knowledgeGroupId )
        throw MASA_BADPARAM_UNIT( "missing knowledge group identifier" );
    auto pNewKnowledgeGroup = GetArmy().FindKnowledgeGroup( knowledgeGroupId );
    if( !pNewKnowledgeGroup )
        throw MASA_BADPARAM_UNIT( "unknown or disallowed knowledge group identifier" );
    if( *pKnowledgeGroup_ == *pNewKnowledgeGroup )
        return;
    pKnowledgeGroup_->UnregisterAutomate( *this );
    pKnowledgeGroup_ = pNewKnowledgeGroup;
    pKnowledgeGroup_->RegisterAutomate( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::OnReceiveChangeSuperior
// Created: NLD 2007-04-11
// -----------------------------------------------------------------------------
void MIL_Automate::OnReceiveChangeSuperior( const sword::UnitMagicAction& msg, const tools::Resolver< MIL_Formation >& formations )
{
    if( msg.type() == sword::UnitMagicAction::change_formation_superior )
    {
        MIL_Formation* pNewFormation = formations.Find( msg.parameters().elem( 0 ).value(0).formation().id() );
        if( !pNewFormation )
            throw MASA_EXCEPTION_ASN( sword::HierarchyModificationAck::ErrorCode, sword::HierarchyModificationAck::error_invalid_formation );
        if( pNewFormation->GetArmy() != GetArmy() )
            throw MASA_EXCEPTION_ASN( sword::HierarchyModificationAck::ErrorCode, sword::HierarchyModificationAck::error_parties_mismatched );
        if( pParentAutomate_ )
            pParentAutomate_->UnregisterAutomate( *this );
        if( pParentFormation_ )
            pParentFormation_->UnregisterAutomate( *this );
        pParentAutomate_  = 0;
        pParentFormation_ = pNewFormation;
        pNewFormation->RegisterAutomate( *this );
    }
    else if( msg.type() == sword::UnitMagicAction::change_automat_superior )
    {
        MIL_Automate* pNewAutomate = MIL_AgentServer::GetWorkspace().GetEntityManager().FindAutomate( msg.parameters().elem( 0 ).value(0).automat().id() );
        if( !pNewAutomate )
            throw MASA_EXCEPTION_ASN( sword::HierarchyModificationAck::ErrorCode, sword::HierarchyModificationAck::error_invalid_automate );
        if( pNewAutomate->GetArmy() != GetArmy() )
            throw MASA_EXCEPTION_ASN( sword::HierarchyModificationAck::ErrorCode, sword::HierarchyModificationAck::error_parties_mismatched );
        if( pNewAutomate == this )
            throw MASA_EXCEPTION_ASN( sword::HierarchyModificationAck::ErrorCode, sword::HierarchyModificationAck::error_invalid_automate );
        if( pParentAutomate_ )
            pParentAutomate_->UnregisterAutomate( *this );
        if( pParentFormation_ )
            pParentFormation_->UnregisterAutomate( *this );
        pParentFormation_ = 0;
        pParentAutomate_  = pNewAutomate;
        pNewAutomate->RegisterAutomate( *this );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::OnReceiveLogSupplyPullFlow
// Created: AHC 2010-09-28
// -----------------------------------------------------------------------------
bool MIL_Automate::OnReceiveLogSupplyPullFlow( const sword::PullFlowParameters& parameters, MIL_AutomateLOG& supplier )
{
    return pStockSupplyManager_->OnReceiveLogSupplyPullFlow( parameters, supplier );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::OnReceiveOrder
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
uint32_t MIL_Automate::OnReceiveOrder( const sword::AutomatOrder& msg )
{
    return pOrderManager_->OnReceiveMission( msg );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::OnReceiveFragOrder
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void MIL_Automate::OnReceiveFragOrder( const sword::FragOrder& msg, const std::function< void( uint32_t ) >& sendAck )
{
    pOrderManager_->OnReceiveFragOrder( msg, sendAck );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::FindLogisticManager
// Created: AHC 2010-09-27
// -----------------------------------------------------------------------------
MIL_AutomateLOG* MIL_Automate::FindLogisticManager() const
{
    if( pBrainLogistic_ )
        return pBrainLogistic_.get();
    return pParentFormation_->FindLogisticManager();
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::Apply
// Created: AHC 2010-09-28
// -----------------------------------------------------------------------------
void MIL_Automate::Apply( MIL_EntityVisitor_ABC< MIL_AgentPion >& visitor ) const
{
    for( auto it = pions_.begin(); it != pions_.end(); ++it )
        visitor.Visit( **it );
    //$$ Manque automates_
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::Apply
// Created: NLD 2012-03-20
// -----------------------------------------------------------------------------
void MIL_Automate::Apply( MIL_EntitiesVisitor_ABC& visitor ) const
{
    if( visitor.Visit( *this ) )
        for( auto it = pions_.begin(); it != pions_.end(); ++it )
            visitor.Visit( **it );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::function< void
// Created: LDC 2014-08-20
// -----------------------------------------------------------------------------
void MIL_Automate::Apply( const std::function< void( const MIL_AgentPion& ) >& visitor ) const
{
    for( auto it = pions_.begin(); it != pions_.end(); ++it )
        visitor( **it );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::Apply2
// Created: NLD 2011-08-03
// -----------------------------------------------------------------------------
void MIL_Automate::Apply2( std::function< void( const MIL_AgentPion&, PHY_Dotation& ) > visitor ) const
{
    BOOST_FOREACH( MIL_AgentPion* pion, pions_ )
    {
        std::function< void( PHY_Dotation& ) > f = boost::bind( visitor, boost::ref( *pion ), _1 );
        pion->Apply2( f );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::Apply2
// Created: NLD 2011-08-03
// -----------------------------------------------------------------------------
void MIL_Automate::Apply2( std::function< void( const MIL_AgentPion&, PHY_DotationStock& ) > visitor ) const
{
    BOOST_FOREACH( MIL_AgentPion* pion, pions_ )
    {
        std::function< void( PHY_DotationStock& ) > f = boost::bind( visitor, boost::ref( *pion ), _1 );
        pion->Apply2( f );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetKnowledge
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
DEC_KnowledgeBlackBoard_Automate& MIL_Automate::GetKnowledge() const
{
    assert( pKnowledgeBlackBoard_ );
    return *pKnowledgeBlackBoard_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetLogisticHierarchy
// Created: NLD 2011-01-10
// -----------------------------------------------------------------------------
logistic::LogisticHierarchy_ABC& MIL_Automate::GetLogisticHierarchy() const
{
    return *pLogisticHierarchy_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetStockSupplyManager
// Created: NLD 2011-01-10
// -----------------------------------------------------------------------------
MIL_StockSupplyManager& MIL_Automate::GetStockSupplyManager() const
{
    return *pStockSupplyManager_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetDotationSupplyManager
// Created: JSR 2013-02-20
// -----------------------------------------------------------------------------
MIL_DotationSupplyManager& MIL_Automate::GetDotationSupplyManager() const
{
    return *pDotationSupplyManager_;
}

namespace
{
    const MT_Vector2D emptyPosition; // $$$$ LGY 2012-03-30: for empty automat
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetPosition
// Created: LGY 2012-03-30
// -----------------------------------------------------------------------------
const MT_Vector2D& MIL_Automate::GetPosition() const
{
    if( pPionPC_ )
        return pPionPC_->GetRole< PHY_RoleInterface_Location >().GetPosition();
    return emptyPosition;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::IsSurrendered
// Created: NLD 2005-02-24
// -----------------------------------------------------------------------------
bool MIL_Automate::IsSurrendered() const
{
    return pArmySurrenderedTo_ != 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetArmySurrenderedTo
// Created: NLD 2007-02-14
// -----------------------------------------------------------------------------
const MIL_Army_ABC* MIL_Automate::GetArmySurrenderedTo() const
{
    return pArmySurrenderedTo_;
}

// -----------------------------------------------------------------------------
unsigned int MIL_Automate::GetLogisticId() const
{
    return GetID();
}

// Name: MIL_Automate::GetType
// Created: NLD 2004-08-17
// -----------------------------------------------------------------------------
const MIL_AutomateType& MIL_Automate::GetType() const
{
    assert( pType_ );
    return *pType_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetPionPC
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
MIL_AgentPion* MIL_Automate::GetPionPC() const
{
    return pPionPC_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetKnowledgeGroup
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
boost::shared_ptr< MIL_KnowledgeGroup > MIL_Automate::GetKnowledgeGroup() const
{
    return pKnowledgeGroup_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetOrderManager
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
const MIL_AutomateOrderManager& MIL_Automate::GetOrderManager() const
{
    return *pOrderManager_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetOrderManager
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
MIL_AutomateOrderManager& MIL_Automate::GetOrderManager()
{
    return *pOrderManager_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetColor
// Created: LGY 2011-06-27
// -----------------------------------------------------------------------------
const MIL_Color& MIL_Automate::GetColor() const
{
    return *pColor_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetPions
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
const MIL_Automate::T_PionVector& MIL_Automate::GetPions() const
{
    return pions_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetAutomates
// Created: NLD 2007-04-03
// -----------------------------------------------------------------------------
const MIL_Automate::T_AutomateVector& MIL_Automate::GetAutomates() const
{
    return automates_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetParentAutomate
// Created: NLD 2007-04-24
// -----------------------------------------------------------------------------
MIL_Automate* MIL_Automate::GetParentAutomate() const
{
    return pParentAutomate_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::IsEngaged
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
bool MIL_Automate::IsEngaged() const
{
    return bEngaged_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::CanEmitReports
// Created: NPT 2013-02-07
// -----------------------------------------------------------------------------
bool MIL_Automate::CanEmitReports() const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::IsMasaLife
// Created: MMC 2013-08-01
// -----------------------------------------------------------------------------
bool MIL_Automate::IsMasaLife() const
{
    const DEC_AutomateDecision* roleDec = RetrieveRole< DEC_AutomateDecision >();
    return roleDec && roleDec->GetModel().IsMasalife();
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::RegisterPion
// Created: NLD 2004-09-01
// -----------------------------------------------------------------------------
void MIL_Automate::RegisterPion( MIL_AgentPion& pion )
{
    assert( std::find( pions_.begin(), pions_.end(), &pion ) == pions_.end() );
    pions_.push_back( &pion );
    if( !pPionPC_ )
        SetCommandPost( &pion );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::SetCommandPost
// Created: LDC 2012-05-03
// -----------------------------------------------------------------------------
void MIL_Automate::SetCommandPost( MIL_AgentPion* pion )
{
    if( pion == pPionPC_ )
        return;
    if( pion )
        pion->NotifySendHeadquarters();
    if( pPionPC_ )
        pPionPC_->NotifySendHeadquarters();
    pPionPC_ = pion;
    if( !pPionPC_ && !pions_.empty() )
    {
        MIL_AgentPion* newPC = pions_[0];
        newPC->SetPionAsCommandPost( true );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::UnregisterPion
// Created: NLD 2004-09-01
// -----------------------------------------------------------------------------
void MIL_Automate::UnregisterPion( MIL_AgentPion& pion )
{
    pions_.erase( std::remove( pions_.begin(), pions_.end(), &pion ), pions_.end() );
    if( pPionPC_ == &pion )
        SetCommandPost( 0 );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::RegisterAutomate
// Created: NLD 2007-03-29
// -----------------------------------------------------------------------------
void MIL_Automate::RegisterAutomate( MIL_Automate& automate )
{
    assert( std::find( automates_.begin(), automates_.end(), &automate ) == automates_.end() );
    automates_.push_back( &automate );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::UnregisterAutomate
// Created: NLD 2007-03-29
// -----------------------------------------------------------------------------
void MIL_Automate::UnregisterAutomate( MIL_Automate& automate )
{
    auto it = std::find( automates_.begin(), automates_.end(), &automate );
    assert( it != automates_.end() );
    automates_.erase( it );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetBrainLogistic
// Created: AHC 2010-09-24

// -----------------------------------------------------------------------------
MIL_AutomateLOG* MIL_Automate::GetBrainLogistic () const
{
    return pBrainLogistic_.get();
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::Serialize
// Created: NLD 2011-01-10
// -----------------------------------------------------------------------------
void MIL_Automate::Serialize( sword::ParentEntity& message ) const
{
    message.mutable_automat()->set_id( GetID() );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::NotifyLinkAdded
// Created: MCO 2013-04-24
// -----------------------------------------------------------------------------
void MIL_Automate::NotifyLinkAdded( const logistic::LogisticLink_ABC& link ) const
{
    if( link.GetSuperior().GetPC() )
        BOOST_FOREACH( const auto& pion, pions_ )
            MIL_Report::PostEvent( *link.GetSuperior().GetPC(), report::eRC_LogSuperiorAdded, *pion );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::NotifyLinkRemoved
// Created: MCO 2013-04-24
// -----------------------------------------------------------------------------
void MIL_Automate::NotifyLinkRemoved( const logistic::LogisticLink_ABC& link ) const
{
    if( link.GetSuperior().GetPC() )
        BOOST_FOREACH( const auto& pion, pions_ )
            MIL_Report::PostEvent( *link.GetSuperior().GetPC(), report::eRC_LogSuperiorRemoved, *pion );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::NotifyQuotaThresholdReached
// Created: NLD 2011-01-11
// -----------------------------------------------------------------------------
void MIL_Automate::NotifyQuotaThresholdReached( const PHY_DotationCategory& dotationCategory ) const
{
    MIL_Report::PostEvent( *this, report::eRC_AllocationConsentieBientotEpuisee, dotationCategory );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::NotifyQuotaExceeded
// Created: MCO 2012-11-19
// -----------------------------------------------------------------------------
void MIL_Automate::NotifyQuotaExceeded( const PHY_DotationCategory& dotationCategory, const T_Requesters& requesters ) const
{
    BOOST_FOREACH( T_Requesters::value_type pion, requesters )
    {
        MIL_Report::PostEvent( *pion, report::eRC_LogQuotaExceeded, dotationCategory );
        MIL_Report::PostEvent( *this, report::eRC_LogQuotaExceededForAgent, dotationCategory, *pion );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::OnReloadBrain
// Created: AHC 2012-01-24
// -----------------------------------------------------------------------------
void MIL_Automate::OnReloadBrain( const sword::MissionParameters& msg )
{
    CancelAllActions();
    auto model = parameters::GetModel( msg, []( const std::string& model ){
        return MIL_AgentServer::GetWorkspace().GetWorkspaceDIA().FindModelAutomate( model );
    } );
    auto& role = GetRole< DEC_AutomateDecision >();
    const bool modified = model && model != &role.GetModel();
    if( modified )
        role.SetModel( *model );
    GetDecision().Reload( true, !modified );
    pOrderManager_->CancelMission();
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::OnChangeBrainDebug
// Created: SLI 2013-06-21
// -----------------------------------------------------------------------------
void MIL_Automate::OnChangeBrainDebug( const sword::MissionParameters& msg )
{
    protocol::CheckCount( msg, 1 );
    const bool activate = protocol::GetBool( msg, 0 );
    GetRole< DEC_AutomateDecision >().ActivateBrainDebug( activate );
}
