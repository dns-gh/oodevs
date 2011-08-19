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
#include "Decision/DEC_Representations.h"
#include "Entities/MIL_Army_ABC.h"
#include "Entities/MIL_Formation.h"
#include "Entities/Actions/PHY_ActionLogistic.h"
#include "Entities/Agents/MIL_AgentTypePion.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Units/Logistic/PHY_LogisticLevel.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"
#include "Entities/Agents/Roles/Logistic/PHY_SupplyDotationState.h"
#include "Entities/Agents/Roles/Logistic/PHY_SupplyDotationRequestContainer.h"
#include "Entities/Agents/Actions/Moving/PHY_RoleAction_Moving.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Logistic/PHY_SupplyDotationRequestContainer.h"
#include "Entities/Agents/Roles/Logistic/PHY_SupplyDotationState.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Objects/LogisticAttribute.h"
#include "Entities/Orders/MIL_AutomateOrderManager.h"
#include "Entities/Orders/MIL_Report.h"
#include "Entities/Specialisations/LOG/LogisticHierarchy.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Automate.h"
#include "Entities/Automates/MIL_DotationSupplyManager.h"
#include "Entities/Automates/MIL_StockSupplyManager.h"
#include "Entities/MIL_EntityVisitor_ABC.h"
#include "Network/NET_AsnException.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_Publisher_ABC.h"
#include "Tools/MIL_DictionaryExtensions.h"
#include "Tools/MIL_Tools.h"
#include "Tools/MIL_Color.h"
#include "Checkpoints/SerializationTools.h"
#include "protocol/ClientSenders.h"
#include "protocol/SimulationSenders.h"
#include <xeumeuleu/xml.hpp>
#include <boost/serialization/vector.hpp>
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
    archive << type
            << automat->nID_;
}

template< typename Archive >
void load_construct_data( Archive& archive, MIL_Automate* automat, const unsigned int /*version*/ )
{
    unsigned int type;
    unsigned int nID;
    archive >> type
            >> nID;
    const MIL_AutomateType* pType = MIL_AutomateType::FindAutomateType( type );
    assert( pType );
    ::new( automat )MIL_Automate( *pType, nID );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate constructor
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_Automate::MIL_Automate( const MIL_AutomateType& type, unsigned int nID, MIL_Entity_ABC& parent, xml::xistream& xis, unsigned int gcPause, unsigned int gcMult )
    : MIL_Entity_ABC                 ( xis )
    , pType_                         ( &type )
    , nID_                           ( nID )
    , pParentFormation_              ( dynamic_cast< MIL_Formation* >( &parent ) )
    , pParentAutomate_               ( dynamic_cast< MIL_Automate* >( &parent ) )
    , bEngaged_                      ( true )
    , pKnowledgeGroup_               ( 0 )
    , pOrderManager_                 ( new MIL_AutomateOrderManager( *this ) )
    , pPionPC_                       ( 0 )
    , pions_                         ()
    , recycledPions_                 ()
    , automates_                     ()
    , bAutomateModeChanged_          ( true )
    , nTickRcDotationSupplyQuerySent_( 0 )
    , pKnowledgeBlackBoard_          ( new DEC_KnowledgeBlackBoard_Automate( *this ) ) // $$$$ MCO : never deleted ?
    , pArmySurrenderedTo_            ( 0 )
    , pLogisticHierarchy_            ( new logistic::LogisticHierarchy( *this ) )
    , pBrainLogistic_                ( 0 )
    , pDotationSupplyManager_        ( new MIL_DotationSupplyManager( *this ) )
    , pStockSupplyManager_           ( new MIL_StockSupplyManager( *this ) )
    , pColor_                        ( new MIL_Color( xis ) )
    , symbol_                        ( xis.attribute< std::string >( "symbol", "" ) )
{
    Initialize( xis, gcPause, gcMult );
    if( pParentFormation_ )
        pParentFormation_->RegisterAutomate( *this );
    else if( pParentAutomate_ )
        pParentAutomate_->RegisterAutomate( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate constructor
// Created: LDC 2009-04-24
// -----------------------------------------------------------------------------
MIL_Automate::MIL_Automate( const MIL_AutomateType& type, unsigned int nID )
    : MIL_Entity_ABC                 ( "" )
    , pType_                         ( &type )
    , nID_                           ( nID )
    , pParentFormation_              ( 0 )
    , pParentAutomate_               ( 0 )
    , bEngaged_                      ( true )
    , pKnowledgeGroup_               ( 0 )
    , pOrderManager_                 ( new MIL_AutomateOrderManager( *this ) )
    , pPionPC_                       ( 0 )
    , bAutomateModeChanged_          ( true )
    , nTickRcDotationSupplyQuerySent_( 0 )
    , pKnowledgeBlackBoard_          ( 0 )
    , pArmySurrenderedTo_            ( 0 )
    , pLogisticHierarchy_            ( new logistic::LogisticHierarchy( *this ) )
    , pBrainLogistic_                ( 0 )
    , pDotationSupplyManager_        ( new MIL_DotationSupplyManager( *this ) )
    , pStockSupplyManager_           ( new MIL_StockSupplyManager( *this ) )
    , pExtensions_                   ( 0 )
    , pColor_                        ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate constructor
// Created: LDC 2010-10-05
// -----------------------------------------------------------------------------
MIL_Automate::MIL_Automate( const MIL_AutomateType& type, unsigned int nID, MIL_Entity_ABC& parent, unsigned int knowledgeGroup, const std::string& name,
                            unsigned int gcPause, unsigned int gcMult, unsigned int context )
    : MIL_Entity_ABC                 ( name )
    , pType_                         ( &type )
    , nID_                           ( nID )
    , pParentFormation_              ( dynamic_cast< MIL_Formation* >( &parent ) )
    , pParentAutomate_               ( dynamic_cast< MIL_Automate* >( &parent ) )
    , bEngaged_                      ( true )
    , pKnowledgeGroup_               ( 0 )
    , pOrderManager_                 ( new MIL_AutomateOrderManager( *this ) )
    , pPionPC_                       ( 0 )
    , pions_                         ()
    , recycledPions_                 ()
    , automates_                     ()
    , bAutomateModeChanged_          ( true )
    , pLogisticHierarchy_            ( new logistic::LogisticHierarchy( *this ) )
    , pBrainLogistic_                ( 0 )
    , nTickRcDotationSupplyQuerySent_( 0 )
    , pKnowledgeBlackBoard_          ( new DEC_KnowledgeBlackBoard_Automate( *this ) ) // $$$$ MCO : never deleted ?
    , pArmySurrenderedTo_            ( 0 )
    , pDotationSupplyManager_        ( new MIL_DotationSupplyManager( *this ) )
    , pStockSupplyManager_           ( new MIL_StockSupplyManager( *this ) )
    , pExtensions_                   ( new MIL_DictionaryExtensions() )
{
    pKnowledgeGroup_ = GetArmy().FindKnowledgeGroup( knowledgeGroup );
    if( !pKnowledgeGroup_ )
        throw std::runtime_error( "Unknown knowledge group" );
    pKnowledgeGroup_->RegisterAutomate( *this );

    RegisterRole( *new DEC_AutomateDecision( *this, gcPause, gcMult ) ) ;
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
const DEC_AutomateDecision& MIL_Automate::GetDecision() const
{
    return GetRole< DEC_AutomateDecision >();
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetDecision
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
DEC_AutomateDecision& MIL_Automate::GetDecision()
{
    return GetRole< DEC_AutomateDecision >();
}

namespace boost
{
    namespace serialization
    {
        template< typename Archive >
        inline
        void serialize( Archive& file, MIL_Automate::T_SupplyDotationStateMap& map, const unsigned int nVersion )
        {
            split_free( file, map, nVersion );
        }

        template< typename Archive >
        void save( Archive& file, const MIL_Automate::T_SupplyDotationStateMap& map, const unsigned int )
        {
            unsigned size = map.size();
            file << size;
            for( MIL_Automate::CIT_SupplyDotationStateMap it = map.begin(); it != map.end(); ++it )
            {
                file << it->first;
                file << it->second;
            }
        }

        template< typename Archive >
        void load( Archive& file, MIL_Automate::T_SupplyDotationStateMap& map, const unsigned int )
        {
            unsigned int nNbr;

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
void MIL_Automate::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    MIL_DictionaryExtensions* pExtensions;
    MIL_Color* pColor;
    file >> boost::serialization::base_object< MIL_Entity_ABC >( *this )
         >> boost::serialization::base_object< logistic::LogisticHierarchyOwner_ABC >( *this )
         >> const_cast< unsigned int& >( nID_ );
    {
        DEC_AutomateDecision* pRole;
        file >> pRole;
        RegisterRole( *pRole );
        RegisterRole( *new DEC_Representations() );
    }
    file >> pParentFormation_
         >> pParentAutomate_
         >> bEngaged_
         >> pKnowledgeGroup_
         >> pPionPC_
         >> pions_
         >> recycledPions_
         >> automates_
         >> bAutomateModeChanged_
         >> pKnowledgeBlackBoard_
         >> const_cast< MIL_Army_ABC*& >( pArmySurrenderedTo_ )
         >> nTickRcDotationSupplyQuerySent_
         >> pExtensions
         >> pColor
         >> symbol_
         >> pLogisticHierarchy_
         >> pBrainLogistic_
         >> pDotationSupplyManager_
         >> pStockSupplyManager_;

    if( pBrainLogistic_.get() )
    {
        pLogisticAction_.reset( new PHY_ActionLogistic<MIL_AutomateLOG>( *pBrainLogistic_.get() ) );
        RegisterAction( pLogisticAction_ );
    }
    pColor_.reset( pColor );
    pExtensions_.reset( pExtensions );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::save
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_Automate::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    const MIL_DictionaryExtensions* const pExtensions = pExtensions_.get();
    const MIL_Color* const pColor = pColor_.get();
    file << boost::serialization::base_object< MIL_Entity_ABC >( *this )
         << boost::serialization::base_object< logistic::LogisticHierarchyOwner_ABC >( *this )
         << const_cast< unsigned int& >( nID_ );
    SaveRole< DEC_AutomateDecision >( *this, file );
    file << pParentFormation_
         << pParentAutomate_
         << bEngaged_
         << pKnowledgeGroup_
         << pPionPC_
         << pions_
         << recycledPions_
         << automates_
         << bAutomateModeChanged_
         << pKnowledgeBlackBoard_
         << pArmySurrenderedTo_
         << nTickRcDotationSupplyQuerySent_
         << pExtensions
         << pColor
         << symbol_
         << pLogisticHierarchy_
         << pBrainLogistic_
         << pDotationSupplyManager_
         << pStockSupplyManager_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::Initialize
// Created: NLD 2007-03-29
// -----------------------------------------------------------------------------
void MIL_Automate::Initialize( xml::xistream& xis, unsigned int gcPause, unsigned int gcMult )
{
    xis >> xml::optional() >> xml::attribute( "engaged", bEngaged_ );
    unsigned int nKnowledgeGroup;
    xis >> xml::attribute( "knowledge-group", nKnowledgeGroup );
    RegisterRole( *new DEC_AutomateDecision( *this, gcPause, gcMult ) ) ;
    RegisterRole( *new DEC_Representations() );
    xis >> xml::list( "unit", *this, &MIL_Automate::ReadUnitSubordinate )
        >> xml::list( "automat", *this, &MIL_Automate::ReadAutomatSubordinate );
    pExtensions_.reset( new MIL_DictionaryExtensions( xis ) );
    if( !pPionPC_ )
        xis.error( "Automat's command post is not defined" );
    pKnowledgeGroup_ = GetArmy().FindKnowledgeGroup( nKnowledgeGroup );
    if( !pKnowledgeGroup_ )
        xis.error( "Unknown knowledge group" );

    std::string logLevelStr( PHY_LogisticLevel::none_.GetName() );
    xis >> xml::optional() >> xml::attribute( "logistic-level", logLevelStr );
    const PHY_LogisticLevel* pLogLevel = PHY_LogisticLevel::Find( logLevelStr );
    if( !pLogLevel )
        xis.error( "Invalid logistic level" );
    if( *pLogLevel != PHY_LogisticLevel::none_ )
    {
        pBrainLogistic_.reset( new MIL_AutomateLOG( *this, *pLogLevel ) );
        pLogisticAction_.reset( new PHY_ActionLogistic<MIL_AutomateLOG>( *pBrainLogistic_.get() ) );
        RegisterAction( pLogisticAction_ );

        // Automat having logistic brain are always there own tc2
        pLogisticHierarchy_.reset( new logistic::LogisticHierarchy( *this, *pBrainLogistic_, xis ) );
    }

    pKnowledgeGroup_->RegisterAutomate( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::ReadUnitSubordinate
// Created: ABL 2007-07-10
// -----------------------------------------------------------------------------
void MIL_Automate::ReadUnitSubordinate( xml::xistream& xis )
{
    bool isPc = false;
    xis >> xml::optional() >> xml::attribute( "command-post", isPc );
    if( isPc && pPionPC_ )
        xis.error( "Automat's command post already defined" );
    const MIL_AgentTypePion* pType = MIL_AgentTypePion::Find( xis.attribute< std::string >( "type" ) );
    if( !pType )
    {
        MT_LOG_ERROR_MSG( "Automate: Unit not created - Unknown type : " <<  xis.attribute< std::string >( "type" ) );
        return;
    }
    MIL_AgentPion& pion = MIL_AgentServer::GetWorkspace().GetEntityManager().CreatePion( *pType, *this, xis ); // Auto-registration
    if( isPc )
        pPionPC_ = &pion;
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
// Name: MIL_Automate::CreatePion
// Created: NLD 2005-02-08
// -----------------------------------------------------------------------------
MIL_AgentPion& MIL_Automate::CreatePion( const MIL_AgentTypePion& type, const MT_Vector2D& vPosition, unsigned int nCtx )
{
    for( RIT_PionVector it = recycledPions_.rbegin(); it != recycledPions_.rend(); ++it )
    {
        MIL_AgentPion& pion = **it;
        const MIL_AgentType_ABC& pionType = pion.GetType();
        if( &type == &pionType )
        {
            recycledPions_.erase( it.base() - 1 );
            pion.ChangeSuperior( *this );
            pion.GetRole< PHY_RoleInterface_Location >().MagicMove( vPosition );
            return pion;
        }
    }
    return MIL_AgentServer::GetWorkspace().GetEntityManager().CreatePion( type, *this, vPosition, nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::DestroyPion
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
void MIL_Automate::WriteODB( xml::xostream& xos ) const
{
    assert( pType_ );
    assert( pKnowledgeGroup_ );
    xos << xml::start( "automat" );
    MIL_Entity_ABC::WriteODB( xos ) ;
    xos << xml::attribute( "id", nID_ )
        << xml::attribute( "engaged", bEngaged_ )
        << xml::attribute( "knowledge-group", pKnowledgeGroup_->GetId() )
        << xml::attribute( "type", pType_->GetName() );
    if( !symbol_.empty() )
        xos << xml::attribute( "symbol", symbol_ );
    pColor_->WriteODB( xos );
    for( CIT_AutomateVector it = automates_.begin(); it != automates_.end(); ++it )
        ( **it ).WriteODB( xos );
    for( CIT_PionVector it = pions_.begin(); it != pions_.end(); ++it )
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
    if( pBrainLogistic_.get() )
        xis.error( "The logistic link of a logistic automat (usually a TC2) must not be overwritten: it should always be itself" );
    pLogisticHierarchy_.reset( new logistic::LogisticHierarchy( *this, superior, xis ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::WriteLogisticLinksODB
// Created: NLD 2006-10-19
// -----------------------------------------------------------------------------
void MIL_Automate::WriteLogisticLinksODB( xml::xostream& xos ) const
{
    if( pBrainLogistic_.get() )
        pBrainLogistic_->WriteLogisticLinksODB( xos );
    else
        pLogisticHierarchy_->WriteODB( xos );
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
    catch( std::runtime_error& /*e*/ )
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
    for( CIT_PionVector it = pions_.begin(); it != pions_.end(); ++it )
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
    for( CIT_PionVector itPion = pions_.begin(); itPion != pions_.end(); ++itPion )
        ( **itPion ).CleanKnowledges();
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::UpdateNetwork
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
void MIL_Automate::UpdateNetwork() const
{
    if( bAutomateModeChanged_ || GetRole< DEC_AutomateDecision >().HasStateChanged() || pExtensions_->HasChanged() )
    {
        client::AutomatAttributes msg;
        msg().mutable_automat()->set_id( nID_ );
        if( bAutomateModeChanged_ )
            msg().set_mode( bEngaged_ ? sword::engaged : sword::disengaged );
        GetRole< DEC_AutomateDecision >().SendChangedState( msg );
        pExtensions_->UpdateNetwork( msg );
        msg.Send( NET_Publisher_ABC::Publisher() );
    }
    if( pBrainLogistic_.get() )
        pBrainLogistic_->SendChangedState();
    else
        pLogisticHierarchy_->SendChangedState();
    pDotationSupplyManager_->SendChangedState();
    pStockSupplyManager_->SendChangedState();
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::UpdateState
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
void MIL_Automate::UpdateState()
{
    if( pBrainLogistic_.get() )
        pBrainLogistic_->UpdateState();
    pDotationSupplyManager_->Update();
    pStockSupplyManager_   ->Update();
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::Clean
// Created: NLD 2005-01-07
// -----------------------------------------------------------------------------
void MIL_Automate::Clean()
{
    if( pBrainLogistic_.get() )
        pBrainLogistic_->Clean();
    bAutomateModeChanged_ = false;
    pDotationSupplyManager_->Clean();
    pStockSupplyManager_->Clean();
    GetRole< DEC_AutomateDecision >().Clean();
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
// Name: MIL_Automate::NotifyDotationSupplied
// Created: NLD 2005-01-28
// -----------------------------------------------------------------------------
void MIL_Automate::NotifyDotationSupplied( const PHY_SupplyDotationState& supplyState )
{
    pDotationSupplyManager_->NotifyDotationSupplied( supplyState );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::NotifyStockSupplyNeeded
// Created: AHC 2010-09-27
// -----------------------------------------------------------------------------
void MIL_Automate::NotifyStockSupplyNeeded( const PHY_DotationCategory& dotationCategory )
{
    pStockSupplyManager_->NotifyStockSupplyNeeded( dotationCategory );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::NotifyDotationSupplied
// Created: AHC 2010-09-27
// -----------------------------------------------------------------------------
void MIL_Automate::NotifyStockSupplied( const PHY_SupplyStockState& supplyState )
{
    pStockSupplyManager_->NotifyStockSupplied( supplyState );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::NotifyStockSupplyCanceled
// Created: AHC 2010-09-278
// -----------------------------------------------------------------------------
void MIL_Automate::NotifyStockSupplyCanceled ( const PHY_SupplyStockState& supplyState )
{
    pStockSupplyManager_->NotifyStockSupplyCanceled( supplyState );
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
    for( CIT_AutomateVector it = automates_.begin(); it != automates_.end(); ++it )
        ( **it ).Engage();
    pOrderManager_->CancelMission();
    for( CIT_PionVector it = pions_.begin(); it != pions_.end(); ++it )
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
bool MIL_Automate::IsPerceived( const DEC_Knowledge_Agent&  knowledge ) const
{
    for( CIT_PionVector it = pions_.begin(); it != pions_.end(); ++it )
        if( ( **it ).GetRole< PHY_RoleInterface_Perceiver >().ComputePerception( knowledge ) != PHY_PerceptionLevel::notSeen_ )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::IsPerceived
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
bool MIL_Automate::IsPerceived( const DEC_Knowledge_Object& knowledge ) const
{
    for( CIT_PionVector it = pions_.begin(); it != pions_.end(); ++it )
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

// -----------------------------------------------------------------------------
// Name: MIL_Automate::CancelSurrender
// Created: NLD 2007-02-15
// -----------------------------------------------------------------------------
void MIL_Automate::CancelSurrender()
{
    pArmySurrenderedTo_ = 0;
    pLogisticHierarchy_->SwitchBackToNominalHierarchy();
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
    double rMaxSpeed = std::numeric_limits< double >::max();
    for( CIT_PionVector it = pions_.begin(); it != pions_.end(); ++it )
    {
        const MIL_AgentPion& pion = **it;
        const double rSpeed = pion.GetRole< moving::PHY_RoleAction_Moving >().GetMaxSpeed();
        if( rSpeed != 0. )
            rMaxSpeed = std::min( rMaxSpeed, rSpeed );
    }
    return rMaxSpeed;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetAlivePionsBarycenter
// Created: NLD 2004-11-08
// -----------------------------------------------------------------------------
bool MIL_Automate::GetAlivePionsBarycenter( MT_Vector2D& barycenter ) const
{
    barycenter.Reset();
    unsigned int nTmp = 0;
    for( CIT_PionVector itPion = pions_.begin(); itPion != pions_.end(); ++itPion )
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
    message().mutable_automat()->set_id( nID_ );
    message().mutable_type()->set_id( pType_->GetID() );
    message().mutable_party()->set_id( GetArmy().GetID() );
    message().mutable_knowledge_group()->set_id( GetKnowledgeGroup().GetId() );
    message().set_app6symbol( "combat" );
    if( !symbol_.empty() )
        message().set_symbol( symbol_ );
    message().set_logistic_level( pBrainLogistic_.get() ?
        (sword::EnumLogisticLevel)pBrainLogistic_->GetLogisticLevel().GetID() : sword::none );
    message().set_name( GetName() );
    pColor_->SendFullState( message );
    assert( pParentAutomate_ || pParentFormation_ );
    if( pParentAutomate_ )
        message().mutable_parent()->mutable_automat()->set_id( pParentAutomate_->GetID() );
    else if( pParentFormation_ )
        message().mutable_parent()->mutable_formation()->set_id( pParentFormation_->GetID() );
    message.Send( NET_Publisher_ABC::Publisher(), context );
    for( CIT_AutomateVector it = automates_.begin(); it != automates_.end(); ++it )
        ( **it ).SendCreation( context );
    for( CIT_PionVector it = pions_.begin(); it != pions_.end(); ++it )
        ( **it ).SendCreation( context );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::SendFullState
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void MIL_Automate::SendFullState( unsigned int contex ) const
{
    client::AutomatAttributes message;
    message().mutable_automat()->set_id( nID_ );
    message().set_mode( bEngaged_ ? sword::engaged : sword::disengaged );
    GetRole< DEC_AutomateDecision >().SendFullState( message );
    pExtensions_->SendFullState( message );
    message.Send( NET_Publisher_ABC::Publisher() );

    pLogisticHierarchy_->SendFullState();
    if( pBrainLogistic_.get() )
        pBrainLogistic_->SendFullState();
    pDotationSupplyManager_->SendFullState();
    pStockSupplyManager_->SendFullState();

    for( CIT_AutomateVector it = automates_.begin(); it != automates_.end(); ++it )
        ( **it ).SendFullState();
    for( CIT_PionVector it = pions_.begin(); it != pions_.end(); ++it )
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
    for( CIT_PionVector it = pions_.begin(); it != pions_.end(); ++it )
        ( **it ).SendKnowledge( context );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::OnReceiveSetAutomateMode
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void MIL_Automate::OnReceiveSetAutomateMode( const sword::SetAutomatMode& asnMsg )
{
    if( pParentAutomate_ && pParentAutomate_->IsEngaged() )
        throw NET_AsnException< sword::SetAutomatModeAck_ErrorCode >( sword::SetAutomatModeAck::error_not_allowed );
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
        throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_unit );
    MT_Vector2D position;
    NET_ASN_Tools::ReadPoint( msg.position(), position );
    MIL_AgentServer::GetWorkspace().GetEntityManager().CreatePion( *pType, *this, position, nCtx ); // Auto-registration
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::OnReceiveUnitCreationRequest
// Created: JSR 2010-04-16
// -----------------------------------------------------------------------------
void MIL_Automate::OnReceiveUnitCreationRequest( const sword::UnitMagicAction& msg, unsigned int nCtx )
{
    if( msg.type() != sword::UnitMagicAction_Type_unit_creation )
        throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
    if( !msg.has_parameters() || msg.parameters().elem_size() < 2 || msg.parameters().elem_size() > 4 )
        throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
    const sword::MissionParameter& id = msg.parameters().elem( 0 );
    if( id.value_size() != 1 || !id.value().Get(0).has_identifier() )
        throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
    const MIL_AgentTypePion* pType = MIL_AgentTypePion::Find( id.value().Get(0).identifier() );
    if( !pType )
        throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_unit );
    const sword::MissionParameter& location = msg.parameters().elem( 1 );
    if( location.value_size() != 1 || !location.value().Get(0).has_point() )
        throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
    const sword::Point& point = location.value().Get(0).point();
    if( point.location().type() != sword::Location::point || point.location().coordinates().elem_size() != 1 )
        throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
    if ( msg.parameters().elem_size() >= 3 )
        if( msg.parameters().elem( 2 ).value_size() != 1 || !msg.parameters().elem( 2 ).value().Get(0).has_acharstr() )
            throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
    if ( msg.parameters().elem_size() >= 4 )
        if( msg.parameters().elem( 3 ).value_size() != 1 || !msg.parameters().elem( 3 ).value().Get(0).has_booleanvalue() )
            throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );

    MT_Vector2D position;
    MIL_Tools::ConvertCoordMosToSim( point.location().coordinates().elem( 0 ), position );
    try
    {
        if ( msg.parameters().elem_size() >= 3 )
        {
            std::string name = msg.parameters().elem( 2 ).value().Get( 0 ).acharstr();
            MIL_AgentPion& pion = MIL_AgentServer::GetWorkspace().GetEntityManager().CreatePion( *pType, *this, position, name, nCtx ); // Auto-registration

            if ( msg.parameters().elem_size() >= 4 )
            {
                bool isPc =  msg.parameters().elem( 3 ).value().Get( 0 ).booleanvalue();
                if ( isPc )
                    pion.SetPionAsPostCommand();
            }
        }
        else
            MIL_AgentServer::GetWorkspace().GetEntityManager().CreatePion( *pType, *this, position, nCtx ); // Auto-registration
    }
    catch( ... )
    {
        throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_unit );
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
            const MIL_Army_ABC* pSurrenderedToArmy = armies.Find( msg.parameters().elem( 0 ).value().Get(0).party().id() );
            if( !pSurrenderedToArmy || *pSurrenderedToArmy == GetArmy() )
                throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
            else if( IsSurrendered() )
                throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_unit_surrendered );
            else
            {
                Surrender( *pSurrenderedToArmy );
                for( CIT_PionVector itPion = pions_.begin(); itPion != pions_.end(); ++itPion )
                    ( **itPion ).OnReceiveMagicSurrender();
            }
        }
        break;
    case sword::UnitMagicAction::cancel_surrender:
        CancelSurrender();
        for( CIT_PionVector itPion = pions_.begin(); itPion != pions_.end(); ++itPion )
            ( **itPion ).OnReceiveMagicCancelSurrender();
        break;
    case sword::UnitMagicAction::change_extension:
        pExtensions_->OnReceiveMsgChangeExtensions( msg );
        break;
    case sword::UnitMagicAction::reload_brain:
        GetDecision().Reload();
        break;
    default:
        pPionPC_->OnReceiveUnitMagicAction( msg, armies );
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
        throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
    if( !msg.has_parameters() || msg.parameters().elem_size() != 1 )
        throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
    const sword::MissionParameter& parametre = msg.parameters().elem( 0 );
    if( parametre.value_size() != 1 || !parametre.value().Get(0).has_point() )
        throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
    const sword::Point& point = parametre.value().Get(0).point();
    if( point.location().type() != sword::Location::point || point.location().coordinates().elem_size() != 1 )
        throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
    MT_Vector2D vPosTmp;
    MIL_Tools::ConvertCoordMosToSim( point.location().coordinates().elem( 0 ), vPosTmp );
    const MT_Vector2D vTranslation( vPosTmp - pPionPC_->GetRole< PHY_RoleInterface_Location >().GetPosition() );
    for( CIT_PionVector itPion = pions_.begin(); itPion != pions_.end(); ++itPion )
        ( **itPion ).OnReceiveMagicActionMoveTo( ( **itPion ).GetRole< PHY_RoleInterface_Location >().GetPosition() + vTranslation );
    GetRole< DEC_AutomateDecision >().Reset( GetName() );
    pOrderManager_->CancelMission();
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::OnReceiveChangeKnowledgeGroup
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void MIL_Automate::OnReceiveChangeKnowledgeGroup( const sword::UnitMagicAction& msg, const tools::Resolver< MIL_Army_ABC >& armies  )
{
    if( msg.type() != sword::UnitMagicAction::change_knowledge_group )
        throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
    if( !msg.has_parameters() || msg.parameters().elem_size() != 2 )
        throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
    if( msg.parameters().elem( 0 ).value_size() != 1 )
        throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
    if( msg.parameters().elem( 1 ).value_size() != 1 )
        throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
    
    // Inversion possible des paramètres à gérer
    bool knowledgeGroupParamFirst   = msg.parameters().elem( 0 ).value().Get( 0 ).has_knowledgegroup() && msg.parameters().elem( 1 ).value().Get( 0 ).has_party();
    bool partyParamFirst            = msg.parameters().elem( 0 ).value().Get( 0 ).has_party() && msg.parameters().elem( 1 ).value().Get( 0 ).has_knowledgegroup();
    if( ! ( knowledgeGroupParamFirst || partyParamFirst ) )
        throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );

    unsigned int knowledgeGroupId = 0, partyId = 0;
    if ( partyParamFirst )
    {
        partyId = msg.parameters().elem( 0 ).value().Get( 0 ).party().id();
        knowledgeGroupId = msg.parameters().elem( 1 ).value().Get( 0 ).knowledgegroup().id();
    }
    else
    {
        knowledgeGroupId = msg.parameters().elem( 0 ).value().Get( 0 ).knowledgegroup().id();
        partyId = msg.parameters().elem( 1 ).value().Get(0).party().id();
    }

    MIL_Army_ABC* pNewArmy = armies.Find( partyId );
    if( !pNewArmy || *pNewArmy != GetArmy() )
        throw NET_AsnException< sword::HierarchyModificationAck::ErrorCode >( sword::HierarchyModificationAck::error_invalid_party );
    MIL_KnowledgeGroup* pNewKnowledgeGroup = pNewArmy->FindKnowledgeGroup( knowledgeGroupId );
    if( !pNewKnowledgeGroup )
        throw NET_AsnException< sword::HierarchyModificationAck::ErrorCode >( sword::HierarchyModificationAck::error_invalid_knowledge_group );
    if( *pKnowledgeGroup_ != *pNewKnowledgeGroup )
    {
        pKnowledgeGroup_->UnregisterAutomate( *this );
        pKnowledgeGroup_ = pNewKnowledgeGroup;
        pKnowledgeGroup_->RegisterAutomate( *this );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::OnReceiveChangeSuperior
// Created: NLD 2007-04-11
// -----------------------------------------------------------------------------
void MIL_Automate::OnReceiveChangeSuperior( const sword::UnitMagicAction& msg, const tools::Resolver< MIL_Formation >& formations )
{
    if( msg.type() == sword::UnitMagicAction::change_formation_superior )
    {
        MIL_Formation* pNewFormation = formations.Find( msg.parameters().elem( 0 ).value().Get(0).formation().id() );
        if( !pNewFormation )
            throw NET_AsnException< sword::HierarchyModificationAck::ErrorCode >( sword::HierarchyModificationAck::error_invalid_formation );
        if( pNewFormation->GetArmy() != GetArmy() )
            throw NET_AsnException< sword::HierarchyModificationAck::ErrorCode >( sword::HierarchyModificationAck::error_parties_mismatched );
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
        MIL_Automate* pNewAutomate = MIL_AgentServer::GetWorkspace().GetEntityManager().FindAutomate( msg.parameters().elem( 0 ).value().Get(0).automat().id() );
        if( !pNewAutomate )
            throw NET_AsnException< sword::HierarchyModificationAck::ErrorCode >( sword::HierarchyModificationAck::error_invalid_automate );
        if( pNewAutomate->GetArmy() != GetArmy() )
            throw NET_AsnException< sword::HierarchyModificationAck::ErrorCode >( sword::HierarchyModificationAck::error_parties_mismatched );
        if( pNewAutomate == this )
            throw NET_AsnException< sword::HierarchyModificationAck::ErrorCode >( sword::HierarchyModificationAck::error_invalid_automate );
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
// Name: MIL_Automate::OnReceiveLogSupplyPushFlow
// Created: NLD 2005-02-04
// -----------------------------------------------------------------------------
void MIL_Automate::OnReceiveLogSupplyPushFlow( const sword::MissionParameters& msg, MIL_AutomateLOG& automatLog )
{
    assert( pStockSupplyManager_.get() );
    pStockSupplyManager_->OnReceiveLogSupplyPushFlow( msg, automatLog );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::OnReceiveLogSupplyPullFlow
// Created: AHC 2010-09-28
// -----------------------------------------------------------------------------
void MIL_Automate::OnReceiveLogSupplyPullFlow( const sword::MissionParameters& msg )
{
    assert( pStockSupplyManager_.get() );
    pStockSupplyManager_->OnReceiveLogSupplyPullFlow( msg );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::OnReceiveOrder
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void MIL_Automate::OnReceiveOrder( const sword::AutomatOrder& msg )
{
    pOrderManager_->OnReceiveMission( msg );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::OnReceiveFragOrder
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void MIL_Automate::OnReceiveFragOrder( const sword::FragOrder& msg )
{
    pOrderManager_->OnReceiveFragOrder( msg );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetQuota
// Created: AHC 2010-09-27
// -----------------------------------------------------------------------------
double MIL_Automate::GetQuota( const MIL_AutomateLOG& supplier, const PHY_DotationCategory& dotationCategory ) const
{
    const MIL_AutomateLOG* pLog = FindLogisticManager();
    return pLog ? pLog->GetQuota( supplier, dotationCategory ) : 0.;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::ConsumeQuota
// Created: AHC 2010-09-27
// -----------------------------------------------------------------------------
void MIL_Automate::ConsumeQuota( const MIL_AutomateLOG& supplier, const PHY_DotationCategory& dotationCategory, double rQuotaConsumed )
{
    MIL_AutomateLOG* pLog = FindLogisticManager();
    if( pLog )
        pLog->ConsumeQuota( supplier, dotationCategory, rQuotaConsumed );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::FindLogisticManager
// Created: AHC 2010-09-27
// -----------------------------------------------------------------------------
MIL_AutomateLOG* MIL_Automate::FindLogisticManager() const
{
    if( pBrainLogistic_.get() )
        return pBrainLogistic_.get();
    return pParentFormation_->FindLogisticManager();
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::Apply
// Created: AHC 2010-09-28
// -----------------------------------------------------------------------------
void MIL_Automate::Apply( MIL_EntityVisitor_ABC< MIL_AgentPion >& visitor ) const
{
    for( CIT_PionVector it = pions_.begin(); it != pions_.end(); ++it )
        visitor.Visit( **it );
    //$$ Manque automates_
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
// Name: MIL_Automate::GetID
// Created: NLD 2004-08-17
// -----------------------------------------------------------------------------
unsigned int MIL_Automate::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
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
MIL_AgentPion& MIL_Automate::GetPionPC() const
{
    assert( pPionPC_ );
    return *pPionPC_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetKnowledgeGroup
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
MIL_KnowledgeGroup& MIL_Automate::GetKnowledgeGroup() const
{
    assert( pKnowledgeGroup_ );
    return *pKnowledgeGroup_;
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
// Name: MIL_Automate::RegisterPion
// Created: NLD 2004-09-01
// -----------------------------------------------------------------------------
void MIL_Automate::RegisterPion( MIL_AgentPion& pion, bool registerPC )
{
    assert( std::find( pions_.begin(), pions_.end(), &pion ) == pions_.end() );
    pions_.push_back( &pion );
    if( registerPC && !pPionPC_ )
    {
        pPionPC_ = &pion;
        pion.SetPionAsPostCommand();
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::UnregisterPion
// Created: NLD 2004-09-01
// -----------------------------------------------------------------------------
void MIL_Automate::UnregisterPion( MIL_AgentPion& pion )
{
    pions_.erase( std::remove( pions_.begin(), pions_.end(), &pion ), pions_.end() );
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
    IT_AutomateVector it = std::find( automates_.begin(), automates_.end(), &automate );
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

// =============================================================================
// logistic::LogisticHierarchyOwner_ABC
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Automate::Serialize
// Created: NLD 2011-01-10
// -----------------------------------------------------------------------------
void MIL_Automate::Serialize( sword::ParentEntity& message ) const
{
    message.mutable_automat()->set_id( GetID() );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::NotifyQuotaThresholdReached
// Created: NLD 2011-01-11
// -----------------------------------------------------------------------------
void MIL_Automate::NotifyQuotaThresholdReached( const PHY_DotationCategory& dotationCategory ) const
{
    MIL_Report::PostEvent( *this, MIL_Report::eReport_QuotaAlmostConsumed, dotationCategory );
}