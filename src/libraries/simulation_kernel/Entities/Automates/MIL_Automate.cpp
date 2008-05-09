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
#include "Entities/Agents/Actions/Moving/PHY_RoleAction_Moving.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Objects/MIL_RealObject_ABC.h"
#include "Entities/Objects/MIL_RealObjectType.h"
#include "Entities/Objects/MIL_CampRefugies.h"
#include "Entities/Objects/MIL_CampPrisonniers.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Entities/Specialisations/LOG/MIL_AutomateTypeLOG.h"
#include "Entities/Orders/MIL_AutomateMissionType.h"
#include "Entities/Orders/MIL_Report.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/MIL_Army.h"
#include "Network/NET_Publisher_ABC.h"
#include "Network/NET_AgentServer.h"
#include "Network/NET_ASN_Messages.h"
#include "Network/NET_AsnException.h"
#include "Network/NET_ASN_Tools.h"
#include "Decision/DEC_ModelAutomate.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Automate.h"
#include "Tools/MIL_Tools.h"
#include "Tools/MIL_IDManager.h"
#include "xeumeuleu/xml.h"

namespace
{
    template< typename R >
    void SaveRole( const MIL_Automate& automate, MIL_CheckPointOutArchive& file )
    {
        const R* const role = & automate.GetRole< R >();
        file << role;
    }
}

BOOST_CLASS_EXPORT_GUID( MIL_Automate, "MIL_Automate" )

// -----------------------------------------------------------------------------
// Name: MIL_Automate constructor
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_Automate::MIL_Automate( const MIL_AutomateType& type, uint nID, MIL_Formation& parent, xml::xistream& xis )
    : MIL_Entity_ABC                     ( type.GetName() , xis ) 
    , pType_                             ( &type )
    , nID_                               ( nID )
    , pParentFormation_                  ( &parent )
    , pParentAutomate_                   ( 0 )
    , bEngaged_                          ( true )
    , pKnowledgeGroup_                   ( 0 )
    , orderManager_                      ( *this )
    , pPionPC_                           ( 0 )
    , pions_                             ()
    , recycledPions_                     ()
    , automates_                         ()
    , bAutomateModeChanged_              ( true )
    , pTC2_                              ( 0 )
    , pNominalTC2_                       ( 0 )
    , bDotationSupplyNeeded_             ( false )
    , bDotationSupplyExplicitlyRequested_( false )
    , dotationSupplyStates_              ()
    , nTickRcDotationSupplyQuerySent_    ( 0 )
    , pKnowledgeBlackBoard_              ( new DEC_KnowledgeBlackBoard_Automate( *this ) )
    , pArmySurrenderedTo_                ( 0 )
{
    Initialize( xis );
    pParentFormation_->RegisterAutomate( *this );
}
    
// -----------------------------------------------------------------------------
// Name: MIL_Automate constructor
// Created: NLD 2007-03-29
// -----------------------------------------------------------------------------
MIL_Automate::MIL_Automate( const MIL_AutomateType& type, uint nID, MIL_Automate& parent, xml::xistream& xis )
    : MIL_Entity_ABC                     ( type.GetName(), xis ) 
    , pType_                             ( &type )
    , nID_                               ( nID )
    , pParentFormation_                  ( 0 )
    , pParentAutomate_                   ( &parent )
    , bEngaged_                          ( true )
    , pKnowledgeGroup_                   ( 0 )
    , orderManager_                      ( *this )
    , pPionPC_                           ( 0 )
    , pions_                             ()
    , recycledPions_                     ()
    , automates_                         ()
    , bAutomateModeChanged_              ( true )
    , pTC2_                              ( 0 )
    , pNominalTC2_                       ( 0 )
    , bDotationSupplyNeeded_             ( false )
    , bDotationSupplyExplicitlyRequested_( false )
    , dotationSupplyStates_              ( )
    , nTickRcDotationSupplyQuerySent_    ( 0 )
    , pKnowledgeBlackBoard_              ( new DEC_KnowledgeBlackBoard_Automate( *this ) )
    , pArmySurrenderedTo_                ( 0 )
{
    Initialize( xis );
    pParentAutomate_->RegisterAutomate( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate constructor
// Created: JVT 2005-03-15
// -----------------------------------------------------------------------------
MIL_Automate::MIL_Automate()
    : MIL_Entity_ABC                     ( "" ) 
    , pType_                             ( 0 )
    , nID_                               ( 0 )
    , pParentFormation_                  ( 0 )
    , pParentAutomate_                   ( 0 )
    , bEngaged_                          ( true )
    , pKnowledgeGroup_                   ( 0 )
    , orderManager_                      ( *this )
    , pPionPC_                           ( 0 )
    , pions_                             ()
    , recycledPions_                     ()
    , automates_                         ()
    , bAutomateModeChanged_              ( true )
    , pTC2_                              ( 0 )
    , pNominalTC2_                       ( 0 )
    , bDotationSupplyNeeded_             ( false )
    , bDotationSupplyExplicitlyRequested_( false )
    , dotationSupplyStates_              ()
    , nTickRcDotationSupplyQuerySent_    ( 0 )
    , pKnowledgeBlackBoard_              ( 0 )
    , pArmySurrenderedTo_                ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate destructor
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_Automate::~MIL_Automate()
{
    pKnowledgeGroup_->UnregisterAutomate( *this );
    if( pParentAutomate_ )
        pParentAutomate_->UnregisterAutomate( *this );
    if( pParentFormation_ )
        pParentFormation_->UnregisterAutomate( *this );    
}

// =============================================================================
// Accessors
// =============================================================================

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
            unsigned size = map.size();
            file << size;
            for( MIL_Automate::CIT_SupplyDotationStateMap it = map.begin(); it != map.end(); ++it )
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
    file >> boost::serialization::base_object< MIL_Entity_ABC >( *this )  
         >> pTC2_
         >> pNominalTC2_;

    uint nTypeID;
    file >> nTypeID;
    pType_ = MIL_AutomateType::FindAutomateType( nTypeID );
    assert( pType_ );

    file >> const_cast< uint& >( nID_ )
         >> pParentFormation_
         >> pParentAutomate_
         >> bEngaged_
         >> pKnowledgeGroup_
         >> pPionPC_
         >> pions_
         >> recycledPions_
         >> automates_
         >> bAutomateModeChanged_
         >> bDotationSupplyNeeded_
         >> bDotationSupplyExplicitlyRequested_
         >> dotationSupplyStates_
         >> pKnowledgeBlackBoard_
         >> const_cast< MIL_Army*& >( pArmySurrenderedTo_ )
         >> nTickRcDotationSupplyQuerySent_;
    { DEC_AutomateDecision         * pRole; file >> pRole; RegisterRole( pRole ); }
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::save
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_Automate::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    assert( pType_ );
    unsigned type = pType_->GetID();
    file << boost::serialization::base_object< MIL_Entity_ABC >( *this ) 
         << pTC2_
         << pNominalTC2_
         << type
         << const_cast< uint& >( nID_ )
         << pParentFormation_
         << pParentAutomate_
         << bEngaged_
         << pKnowledgeGroup_
         << pPionPC_
         << pions_
         << recycledPions_
         << automates_
         << bAutomateModeChanged_
         << bDotationSupplyNeeded_
         << bDotationSupplyExplicitlyRequested_
         << dotationSupplyStates_
         << pKnowledgeBlackBoard_
         << pArmySurrenderedTo_
         << nTickRcDotationSupplyQuerySent_;
    SaveRole< DEC_AutomateDecision >( *this, file );

}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::Initialize
// Created: NLD 2007-03-29
// -----------------------------------------------------------------------------
void MIL_Automate::Initialize( xml::xistream& xis )
{
    xis >> xml::optional() >> xml::attribute( "engaged", bEngaged_ ); 

    uint nKnowledgeGroup;
    xis >> xml::attribute( "knowledge-group", nKnowledgeGroup );
    pKnowledgeGroup_ = GetArmy().FindKnowledgeGroup( nKnowledgeGroup );
    if( !pKnowledgeGroup_ )
        xis.error( "Unknown knowledge group" );
    pKnowledgeGroup_->RegisterAutomate( *this );
      
    RegisterRole( new DEC_AutomateDecision( *this ) ) ;  //$$$ BULLSHIT : strName_ must be initialized ...
    
    xis >> xml::list( "unit"    , *this, &MIL_Automate::ReadUnitSubordinate    )
        >> xml::list( "automat" , *this, &MIL_Automate::ReadAutomatSubordinate );
        
    if( !pPionPC_ )
        xis.error( "Automat's command post is not defined" );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::ReadUnitSubordinate
// Created: ABL 2007-07-10
// -----------------------------------------------------------------------------
void MIL_Automate::ReadUnitSubordinate( xml::xistream& xis )
{
    uint        id;
    std::string strType;
    bool isPc = false;
    xis >> xml::attribute( "id", id )
        >> xml::attribute( "type", strType )
        >> xml::optional() >> xml::attribute( "command-post", isPc );

    if( isPc && pPionPC_ )
        xis.error( "Automat's command post already defined" );

    const MIL_AgentTypePion* pType = MIL_AgentTypePion::Find( strType );
    if( !pType )
        xis.error( "Unknown pawn type" );

    MIL_AgentPion& pion = MIL_AgentServer::GetWorkspace().GetEntityManager().CreatePion( *pType, id, *this, xis ); // Auto-registration
    if( isPc )
        pPionPC_ = &pion;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::ReadAutomatSubordinate
// Created: NLD 2007-03-29
// -----------------------------------------------------------------------------
void MIL_Automate::ReadAutomatSubordinate( xml::xistream& xis )
{
    uint        id;
    std::string strType;

    xis >> xml::attribute( "id", id )
        >> xml::attribute( "type", strType );

    const MIL_AutomateType* pAutomateType = MIL_AutomateType::FindAutomateType( strType );
    if( !pAutomateType )
        xis.error( "Unknown automat type" );

    MIL_AgentServer::GetWorkspace().GetEntityManager().CreateAutomat( xis, *this ); // Auto-registration
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
            pion.ChangeSuperior( *this );
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
void MIL_Automate::WriteODB( xml::xostream& xos ) const
{
    assert( pType_ );
    assert( pKnowledgeGroup_ );

    xos << xml::start( "automat" ); 
    
    MIL_Entity_ABC::WriteODB( xos ) ;  
    
    xos << xml::attribute( "id", nID_ )
        << xml::attribute( "engaged", bEngaged_ )
        << xml::attribute( "knowledge-group", pKnowledgeGroup_->GetID() )
        << xml::attribute( "type", pType_->GetName() );

    for( CIT_AutomateVector it = automates_.begin(); it != automates_.end(); ++it )
        (**it).WriteODB( xos );

    for( CIT_PionVector it = pions_.begin(); it != pions_.end(); ++it )
        (**it).WriteODB( xos );

    xos << xml::end(); // automat
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::ReadOverloading
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
void MIL_Automate::ReadOverloading( xml::xistream& xis )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::ReadLogisticLink
// Created: NLD 2006-10-19
// -----------------------------------------------------------------------------
void MIL_Automate::ReadLogisticLink( MIL_AutomateLOG& superior, xml::xistream& xis )
{
    std::string strLink;
    xis >> xml::attribute( "link", strLink );

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
void MIL_Automate::WriteLogisticLinksODB( xml::xostream& xos ) const
{
    if( pTC2_ )
    {
        xos << xml::start( "automat" )
                << xml::attribute( "id", pTC2_->GetID() )
                << xml::start( "subordinate" )
                    << xml::attribute( "id", GetID() )
                    << xml::attribute( "link", "tc2" )
                << xml::end()
            << xml::end();
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
    orderManager_.Update();
    GetRole< DEC_AutomateDecision >().UpdateDecision();
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::UpdateKnowledges
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void MIL_Automate::UpdateKnowledges()
{
    // Pions (+ PC)
    for( CIT_PionVector it = pions_.begin(); it != pions_.end(); ++it )
        (**it).UpdateKnowledges();

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
    if( bAutomateModeChanged_ || GetRole< DEC_AutomateDecision >().HasStateChanged() )
    {
        NET_ASN_MsgAutomatAttributes msg;
        msg().oid = nID_;

        if( bAutomateModeChanged_ )
        {
            msg().m.etat_automatePresent = 1;
            msg().etat_automate = bEngaged_ ? EnumAutomatMode::embraye : EnumAutomatMode::debraye;
        }

        GetRole< DEC_AutomateDecision >().SendChangedState( msg );
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
    if( !bDotationSupplyNeeded_ || !dotationSupplyStates_.empty() || ( !pTC2_ && !GetNominalTC2() ) )
        return;

    PHY_SupplyDotationRequestContainer supplyRequests( *this, bDotationSupplyExplicitlyRequested_ );
    bDotationSupplyNeeded_ = !supplyRequests.Execute( pTC2_, GetNominalTC2(), dotationSupplyStates_ );
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
    GetRole< DEC_AutomateDecision >().Clean();
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
        MIL_Report::PostEvent( *this, MIL_Report::eReport_DotationSupplyRequest );

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
    MIL_Report::PostEvent( *this, MIL_Report::eReport_DotationSupplyDone );
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
// Name: MIL_Automate::Disengage
// Created: NLD 2003-04-11
//-----------------------------------------------------------------------------
void MIL_Automate::Disengage()
{
    orderManager_.ReplaceMission();
    if( bEngaged_ )
    {
        bEngaged_             = false;
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
        (**it).Engage();

    orderManager_.ReplaceMission();
    for( CIT_PionVector it = pions_.begin(); it != pions_.end(); ++it )
        (**it).GetOrderManager().ReplaceMission();

    if( !bEngaged_ )
    {
        bEngaged_             = true;
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
        if( (**it).GetRole< PHY_RolePion_Perceiver >().ComputePerception( knowledge ) != PHY_PerceptionLevel::notSeen_ )
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
        if( (**it).GetRole< PHY_RolePion_Perceiver >().ComputePerception( knowledge ) != PHY_PerceptionLevel::notSeen_ )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetArmy
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
MIL_Army& MIL_Automate::GetArmy() const
{
    assert( pParentFormation_ || pParentAutomate_ );

    if( pParentFormation_ )
        return pParentFormation_->GetArmy();
    else
        return pParentAutomate_->GetArmy();
}

// =============================================================================
// REFUGEES
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Automate::NotifyRefugeeOrientated
// Created: NLD 2007-02-15
// -----------------------------------------------------------------------------
void MIL_Automate::NotifyRefugeeOriented( const MIL_AgentPion& pionManaging )
{
    if( !pType_->IsRefugee() )
        return;
    pTC2_ = pionManaging.GetAutomate().GetTC2();
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::NotifyRefugeeReleased
// Created: NLD 2007-02-15
// -----------------------------------------------------------------------------
void MIL_Automate::NotifyRefugeeReleased()
{
    if( !pType_->IsRefugee() )
        return;
    pTC2_ = pNominalTC2_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::NotifyRefugeeReleased
// Created: NLD 2007-02-15
// -----------------------------------------------------------------------------
void MIL_Automate::NotifyRefugeeReleased( const MIL_CampRefugies& camp )
{
    if( !pType_->IsRefugee() )
        return;
    pTC2_ = &camp.GetTC2();
}


// =============================================================================
// PRISONERS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Automate::Surrender
// Created: NLD 2005-02-24
// -----------------------------------------------------------------------------
void MIL_Automate::Surrender( const MIL_Army& amrySurrenderedTo )
{
    if( pArmySurrenderedTo_ )
        return;

    orderManager_.ReplaceMission();
    pArmySurrenderedTo_ = &amrySurrenderedTo;
    pTC2_               = 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::CancelSurrender
// Created: NLD 2007-02-15
// -----------------------------------------------------------------------------
void MIL_Automate::CancelSurrender()
{
    pArmySurrenderedTo_ = 0;
    pTC2_               = pNominalTC2_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::NotifyCaptured
// Created: NLD 2005-03-04
// -----------------------------------------------------------------------------
bool MIL_Automate::NotifyCaptured( const MIL_AgentPion& pionTakingPrisoner )
{
    if( !IsSurrendered() )
        return false;

    pTC2_ = pionTakingPrisoner.GetAutomate().GetTC2();
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
    pTC2_ = 0;
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::NotifyImprisoned
// Created: NLD 2005-03-07
// -----------------------------------------------------------------------------
bool MIL_Automate::NotifyImprisoned( const MIL_CampPrisonniers& camp )
{
    if( !IsSurrendered() )
        return false;

    pTC2_ = &camp.GetTC2();
    return true;
}

// =============================================================================
// MISC
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetAlivePionsMaxSpeed
// Created: NLD 2007-04-30
// -----------------------------------------------------------------------------
MT_Float MIL_Automate::GetAlivePionsMaxSpeed() const
{
    MT_Float rMaxSpeed = std::numeric_limits< MT_Float >::max();
    for( CIT_PionVector it = pions_.begin(); it != pions_.end(); ++it )
    {
        const MIL_AgentPion& pion = **it;
        const MT_Float rSpeed = pion.GetRole< PHY_RoleAction_Moving >().GetMaxSpeed();
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
    NET_ASN_MsgAutomatCreation asn;
    asn().oid                     = nID_;
    asn().type_automate           = pType_->GetID();
    asn().oid_camp                = GetArmy().GetID();
    asn().oid_groupe_connaissance = GetKnowledgeGroup().GetID();
    asn().nom                     = GetName().c_str();

    assert( pParentAutomate_ || pParentFormation_ );
    if( pParentAutomate_ )
    {
        asn().oid_parent.t          = T_MsgAutomatCreation_oid_parent_automate;
        asn().oid_parent.u.automate = pParentAutomate_->GetID();
    }
    else if( pParentFormation_ )
    {
        asn().oid_parent.t           = T_MsgAutomatCreation_oid_parent_formation;
        asn().oid_parent.u.formation = pParentFormation_->GetID();
    }
    asn.Send();

    for( CIT_AutomateVector it = automates_.begin(); it != automates_.end(); ++it )
        (**it).SendCreation();

    for( CIT_PionVector it = pions_.begin(); it != pions_.end(); ++it )
        (**it).SendCreation();
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::SendFullState
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void MIL_Automate::SendFullState() const
{

    NET_ASN_MsgAutomatAttributes asn;
    asn().oid = nID_;
    asn().m.etat_automatePresent = 1;
    asn().etat_automate = bEngaged_ ? EnumAutomatMode::embraye : EnumAutomatMode::debraye;
    GetRole< DEC_AutomateDecision >().SendFullState( asn );
    asn.Send();

    SendLogisticLinks();

    for( CIT_SupplyDotationStateMap it = dotationSupplyStates_.begin(); it != dotationSupplyStates_.end(); ++it )
        it->second->SendFullState();

    for( CIT_AutomateVector it = automates_.begin(); it != automates_.end(); ++it )
        (**it).SendFullState();

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
    NET_ASN_MsgAutomatChangeLogisticLinks asn;
    asn().oid = nID_;
    if( pTC2_ )
    {
        asn().m.oid_tc2Present = 1;
        asn().oid_tc2          = pTC2_->GetID();
    }
    asn.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::OnReceiveMsgSetAutomateMode
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void MIL_Automate::OnReceiveMsgSetAutomateMode( const ASN1T_MsgSetAutomatMode& asnMsg )
{
    if( pParentAutomate_ && pParentAutomate_->IsEngaged() )
        throw NET_AsnException< ASN1T_EnumSetAutomatModeErrorCode >( EnumSetAutomatModeErrorCode::error_not_allowed );
    switch( asnMsg.mode )
    {
        case EnumAutomatMode::debraye: Disengage(); break;
        case EnumAutomatMode::embraye: Engage   (); break;
        default:
            assert( false );
    };
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::OnReceiveMsgUnitCreationRequest
// Created: AGE 2007-06-18
// -----------------------------------------------------------------------------
void MIL_Automate::OnReceiveMsgUnitCreationRequest( const ASN1T_MsgUnitCreationRequest& msg )
{
    const MIL_AgentTypePion* pType = MIL_AgentTypePion::Find( msg.type_pion );
    if( !pType )
        throw NET_AsnException< ASN1T_EnumUnitErrorCode >( EnumUnitErrorCode::error_invalid_unit );

    MT_Vector2D position;
    NET_ASN_Tools::ReadPoint( msg.position, position );
    MIL_AgentServer::GetWorkspace().GetEntityManager().CreatePion( *pType, *this, position ); // Auto-registration
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::OnReceiveMsgUnitMagicAction
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void MIL_Automate::OnReceiveMsgUnitMagicAction( const ASN1T_MsgUnitMagicAction& asnMsg )
{
    if( asnMsg.action.t == T_MsgUnitMagicAction_action_move_to )
    {
        MT_Vector2D vPosTmp;
        MIL_Tools::ConvertCoordMosToSim( *asnMsg.action.u.move_to, vPosTmp );

        const MT_Vector2D vTranslation( vPosTmp - pPionPC_->GetRole< PHY_RolePion_Location >().GetPosition() );
        for( CIT_PionVector itPion = pions_.begin(); itPion != pions_.end(); ++itPion )
            (**itPion).OnReceiveMsgMagicMove( (**itPion).GetRole< PHY_RolePion_Location >().GetPosition() + vTranslation );

        GetRole< DEC_AutomateDecision >().Reset();
        orderManager_.ReplaceMission();
    }
    else if( asnMsg.action.t == T_MsgUnitMagicAction_action_se_rendre )
    {
        const MIL_Army* pSurrenderedToArmy = MIL_AgentServer::GetWorkspace().GetEntityManager().FindArmy( asnMsg.action.u.se_rendre );
        if( !pSurrenderedToArmy || *pSurrenderedToArmy == GetArmy() )
            throw NET_AsnException< ASN1T_EnumUnitErrorCode >( EnumUnitErrorCode::error_invalid_attribute );
        else if( IsSurrendered() )
            throw NET_AsnException< ASN1T_EnumUnitErrorCode >( EnumUnitErrorCode::error_unit_surrendered );
        else
        {
            Surrender( *pSurrenderedToArmy );
            for( CIT_PionVector itPion = pions_.begin(); itPion != pions_.end(); ++itPion )
                (**itPion).OnReceiveMagicSurrender();
        }
    }
    else if( asnMsg.action.t == T_MsgUnitMagicAction_action_annuler_reddition )
    {
        CancelSurrender();
        for( CIT_PionVector itPion = pions_.begin(); itPion != pions_.end(); ++itPion )
            (**itPion).OnReceiveMagicCancelSurrender();
    }
    else
        pPionPC_->OnReceiveMsgUnitMagicAction( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::OnReceiveMsgChangeKnowledgeGroup
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void MIL_Automate::OnReceiveMsgChangeKnowledgeGroup( const ASN1T_MsgAutomatChangeKnowledgeGroup& asnMsg )
{
    MIL_Army* pNewArmy = MIL_AgentServer::GetWorkspace().GetEntityManager().FindArmy( asnMsg.oid_camp );
    if( !pNewArmy || *pNewArmy != GetArmy() )
        throw NET_AsnException< ASN1T_EnumChangeHierarchyErrorCode >( EnumChangeHierarchyErrorCode::error_invalid_camp );

    MIL_KnowledgeGroup* pNewKnowledgeGroup = pNewArmy->FindKnowledgeGroup( asnMsg.oid_groupe_connaissance );
    if( !pNewKnowledgeGroup )
        throw NET_AsnException< ASN1T_EnumChangeHierarchyErrorCode >( EnumChangeHierarchyErrorCode::error_invalid_groupe_connaissance );

    if( *pKnowledgeGroup_ != *pNewKnowledgeGroup )
    {
        pKnowledgeGroup_->UnregisterAutomate( *this );
        pKnowledgeGroup_ = pNewKnowledgeGroup;
        pKnowledgeGroup_->RegisterAutomate( *this );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::OnReceiveMsgChangeLogisticLinks
// Created: NLD 2005-01-17
// -----------------------------------------------------------------------------
void MIL_Automate::OnReceiveMsgChangeLogisticLinks( const ASN1T_MsgAutomatChangeLogisticLinks& msg )
{
    if( IsSurrendered() )
        throw NET_AsnException< ASN1T_EnumChangeHierarchyErrorCode >( EnumChangeHierarchyErrorCode::error_unit_surrendered );

    if( msg.m.oid_tc2Present )
    {
        if( msg.oid_tc2 == 0 )
            pTC2_ = 0;
        else
        {
            MIL_Automate* pTC2 = MIL_AgentServer::GetWorkspace().GetEntityManager().FindAutomate( msg.oid_tc2 );
            if( !pTC2 || !pTC2->GetType().IsLogistic() )
                throw NET_AsnException< ASN1T_EnumChangeHierarchyErrorCode >( EnumChangeHierarchyErrorCode::error_invalid_automate_tc2 );
            pTC2_ = static_cast< MIL_AutomateLOG* >( pTC2 );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::OnReceiveMsgChangeSuperior
// Created: NLD 2007-04-11
// -----------------------------------------------------------------------------
void MIL_Automate::OnReceiveMsgChangeSuperior( const ASN1T_MsgAutomatChangeSuperior& msg )
{
    if( msg.oid_superior.t == T_MsgAutomatChangeSuperior_oid_superior_formation )
    {
        MIL_Formation* pNewFormation = MIL_AgentServer::GetWorkspace().GetEntityManager().FindFormation( msg.oid_superior.u.formation );
        if( !pNewFormation )
            throw NET_AsnException< ASN1T_EnumChangeHierarchyErrorCode >( EnumChangeHierarchyErrorCode::error_invalid_formation );
        if( pNewFormation->GetArmy() != GetArmy() )
            throw NET_AsnException< ASN1T_EnumChangeHierarchyErrorCode >( EnumChangeHierarchyErrorCode::error_camps_incompatibles );
        if( pParentAutomate_ )
            pParentAutomate_->UnregisterAutomate( *this );
        if( pParentFormation_ )
            pParentFormation_->UnregisterAutomate( *this );    
        pParentAutomate_  = 0;
        pParentFormation_ = pNewFormation;
        pNewFormation->RegisterAutomate( *this );
    }
    else if( msg.oid_superior.t == T_MsgAutomatChangeSuperior_oid_superior_automate )
    {
        MIL_Automate* pNewAutomate = MIL_AgentServer::GetWorkspace().GetEntityManager().FindAutomate( msg.oid_superior.u.automate );
        if( !pNewAutomate )
            throw NET_AsnException< ASN1T_EnumChangeHierarchyErrorCode >( EnumChangeHierarchyErrorCode::error_invalid_automate );
        if( pNewAutomate->GetArmy() != GetArmy() )
            throw NET_AsnException< ASN1T_EnumChangeHierarchyErrorCode >( EnumChangeHierarchyErrorCode::error_camps_incompatibles );
        if( pNewAutomate == this ) 
            throw NET_AsnException< ASN1T_EnumChangeHierarchyErrorCode >( EnumChangeHierarchyErrorCode::error_invalid_automate );
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
// Name: MIL_Automate::OnReceiveMsgLogSupplyChangeQuotas
// Created: NLD 2005-02-03
// -----------------------------------------------------------------------------
void MIL_Automate::OnReceiveMsgLogSupplyChangeQuotas( const ASN1T_MsgLogSupplyChangeQuotas& /*msg*/ )
{
    throw NET_AsnException< ASN1T_MsgLogSupplyChangeQuotasAck >( MsgLogSupplyChangeQuotasAck::error_invalid_receveur );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::OnReceiveMsgLogSupplyPushFlow
// Created: NLD 2005-02-04
// -----------------------------------------------------------------------------
void MIL_Automate::OnReceiveMsgLogSupplyPushFlow( const ASN1T_MsgLogSupplyPushFlow& /*msg*/ )
{
    throw NET_AsnException< ASN1T_MsgLogSupplyPushFlowAck >( MsgLogSupplyPushFlowAck::error_invalid_receveur );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::OnReceiveMsgOrder
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void MIL_Automate::OnReceiveMsgOrder( const ASN1T_MsgAutomatOrder& msg )
{
    orderManager_.OnReceiveMission( msg );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::OnReceiveMsgFragOrder
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void MIL_Automate::OnReceiveMsgFragOrder( const ASN1T_MsgFragOrder& msg )
{
    orderManager_.OnReceiveFragOrder( msg );
}
