// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/MIL_KnowledgeGroup.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 18:07 $
// $Revision: 9 $
// $Workfile: MIL_KnowledgeGroup.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "MIL_KnowledgeGroup.h"

#include "Network/NET_AS_MOSServerMsgMgr.h"
#include "Network/NET_AgentServer.h"

#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/MIL_Army.h"

#include "DEC_Knowledge_Agent.h"
#include "DEC_KnowledgeBlackBoard.h"
#include "DEC_KS_AgentKnowledgeSynthetizer.h"
#include "DEC_KS_Alzheimer.h"
#include "DEC_KS_RapFor.h"
#include "DEC_KS_NetworkUpdater.h"
#include "DEC_KS_KnowledgeGroupQuerier.h"
#include "DEC_KS_Sharing.h"

std::set< uint > MIL_KnowledgeGroup::ids_;

BOOST_CLASS_EXPORT_GUID( MIL_KnowledgeGroup, "MIL_KnowledgeGroup" )

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup constructor
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_KnowledgeGroup::MIL_KnowledgeGroup( const MIL_KnowledgeGroupType& type, uint nID, MIL_Army& army, MIL_InputArchive& /*archive*/ )
    : pType_                       ( &type )
    , nID_                         ( nID )
    , pArmy_                       ( &army )
    , pKnowledgeBlackBoard_        ( new DEC_KnowledgeBlackBoard )
    , pKsAgentKnowledgeSynthetizer_( new DEC_KS_AgentKnowledgeSynthetizer( *pKnowledgeBlackBoard_, *this ) )
    , pKsAlzheimer_                ( new DEC_KS_Alzheimer                ( *pKnowledgeBlackBoard_ ) )
    , pKsRapFor_                   ( new DEC_KS_RapFor                   ( *pKnowledgeBlackBoard_, *this ) )
    , pKsSharing_                  ( new DEC_KS_Sharing                  ( *pKnowledgeBlackBoard_, *this ) )
    , pKsNetworkUpdater_           ( new DEC_KS_NetworkUpdater           ( *pKnowledgeBlackBoard_ ) )
    , pKsQuerier_                  ( new DEC_KS_KnowledgeGroupQuerier    ( *pKnowledgeBlackBoard_, *this ) )
    , automates_                   ()
{
    if ( !ids_.insert( nID_ ).second )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "KnowledgeGroup id %d is already used", nID_ ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup constructor
// Created: JVT 2005-03-15
// -----------------------------------------------------------------------------
MIL_KnowledgeGroup::MIL_KnowledgeGroup()
    : pType_                       ( 0 )
    , nID_                         ( 0 )
    , pArmy_                       ( 0 )
    , pKnowledgeBlackBoard_        ( 0 )
    , pKsAgentKnowledgeSynthetizer_( 0 )
    , pKsAlzheimer_                ( 0 )
    , pKsRapFor_                   ( 0 )
    , pKsSharing_                  ( 0 )
    , pKsNetworkUpdater_           ( 0 )
    , pKsQuerier_                  ( 0 )
    , automates_                   ()
{
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup destructor
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_KnowledgeGroup::~MIL_KnowledgeGroup()
{
    delete pKnowledgeBlackBoard_;
    delete pKsAgentKnowledgeSynthetizer_;
    delete pKsAlzheimer_;                
    delete pKsRapFor_;                 
    delete pKsSharing_;          
    delete pKsNetworkUpdater_;
    delete pKsQuerier_;

    ids_.erase( nID_ );
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::load
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::load( MIL_CheckPointInArchive& file, const uint )
{
    uint nTypeID;
    file >> nTypeID;
    pType_ = MIL_KnowledgeGroupType::FindType( nTypeID );
    
    file >> const_cast< uint& >( nID_ )
         >> pArmy_
         >> pKnowledgeBlackBoard_
         >> automates_;
         
    pKsAgentKnowledgeSynthetizer_ = new DEC_KS_AgentKnowledgeSynthetizer( *pKnowledgeBlackBoard_, *this );
    pKsAlzheimer_ =                 new DEC_KS_Alzheimer                ( *pKnowledgeBlackBoard_ );
    pKsRapFor_ =                    new DEC_KS_RapFor                   ( *pKnowledgeBlackBoard_, *this );
    pKsSharing_ =                   new DEC_KS_Sharing                  ( *pKnowledgeBlackBoard_, *this );
    pKsNetworkUpdater_ =            new DEC_KS_NetworkUpdater           ( *pKnowledgeBlackBoard_ );
    pKsQuerier_ =                   new DEC_KS_KnowledgeGroupQuerier    ( *pKnowledgeBlackBoard_, *this );

    ids_.insert( nID_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::save
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    assert( pType_ );
    
    file << pType_->GetID()
         << nID_
         << pArmy_
         << pKnowledgeBlackBoard_
         << automates_;
}


// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::UpdateKnowledges
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::UpdateKnowledges()
{
    assert( pKnowledgeBlackBoard_ );
    pKnowledgeBlackBoard_->Update();
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::IsPerceived
// Created: NLD 2004-03-22
// -----------------------------------------------------------------------------
bool MIL_KnowledgeGroup::IsPerceived( const DEC_Knowledge_Agent& knowledge ) const
{
    // Hack : Pour éviter les destructions de connaissances dues à la posture du l'unité source
    //  => On triche en forcant la connaissance à ne pas être détruite si la connaissance et l'
    //     unité réelle se trouvent au même endroit
    if( knowledge.GetPosition() == knowledge.GetAgentKnown().GetRole< PHY_RoleInterface_Location >().GetPosition() )
        return false;

    for( CIT_AutomateVector itAutomate = automates_.begin(); itAutomate != automates_.end(); ++itAutomate )
        if( (*itAutomate)->IsPerceived( knowledge ) )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::IsPerceived
// Created: NLD 2004-03-22
// -----------------------------------------------------------------------------
bool MIL_KnowledgeGroup::IsPerceived( const DEC_Knowledge_Object& knowledge ) const
{
    for( CIT_AutomateVector itAutomate = automates_.begin(); itAutomate != automates_.end(); ++itAutomate )
        if( (*itAutomate)->IsPerceived( knowledge ) )
            return true;
    return false;
}


// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::SendCreation
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::SendCreation( )
{
    assert( pArmy_ );

    NET_AS_MOSServerMsgMgr& msgMgr = MIL_AgentServer::GetWorkspace().GetAgentServer().GetMessageMgr();

    DIN::DIN_BufferedMessage msg = msgMgr.BuildMessage();
    msg << (uint32)nID_;
    msg << (uint32)pArmy_->GetID();
    msgMgr.SendMsgKnowledgeGroup( msg );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::SendKnowledge
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::SendKnowledge()
{
    assert( pKsNetworkUpdater_ );
    pKsNetworkUpdater_->SendFullState();   
}
