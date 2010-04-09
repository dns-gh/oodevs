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

#include "simulation_kernel_pch.h"
#include "MIL_KnowledgeGroup.h"
#include "KnowledgeGroupFactory_ABC.h" // LTO
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Communications/PHY_RolePion_Communications.h" // LTO
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/MIL_Army.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgeAgent.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgeAgentPerception.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgePopulation.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgePopulationCollision.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgePopulationPerception.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_Knowledge_AgentPerception.h"
#include "Knowledge/DEC_Knowledge_Population.h"
#include "Knowledge/DEC_Knowledge_PopulationCollision.h"
#include "Knowledge/DEC_Knowledge_PopulationPerception.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Network/NET_AsnException.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "Tools/MIL_IDManager.h" 
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/ref.hpp>
#include <xeumeuleu/xml.h>
#include <boost/serialization/export.hpp>

std::set< unsigned int > MIL_KnowledgeGroup::ids_;

MIL_IDManager MIL_KnowledgeGroup::idManager_;

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_KnowledgeGroup )

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup constructor
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_KnowledgeGroup::MIL_KnowledgeGroup( const MIL_KnowledgeGroupType& type, unsigned int nID, MIL_Army_ABC& army )
    : pType_               ( &type )
    , nID_                 ( nID )
    , pArmy_               ( &army )
    , pParent_             ( 0 ) // LTO
    , pKnowledgeBlackBoard_( new DEC_KnowledgeBlackBoard_KnowledgeGroup( *this ) )
    , automates_           ()
    , timeToDiffuse_       ( 0 ) // LTO
    , isActivated_         ( true ) // LTO
    , hasBeenUpdated_      ( false )
    , isJammedKnowledgeGroup_ ( false )
    , jamedPion_           ( 0 ) // LTO
{
    idManager_.Lock( nID );
    pArmy_->RegisterKnowledgeGroup( *this );
    if( !ids_.insert( nID_ ).second )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "KnowledgeGroup id %d is already used", nID_ ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup constructor
// Created: SLG 2009-11-11
// LTO
// -----------------------------------------------------------------------------
MIL_KnowledgeGroup::MIL_KnowledgeGroup( xml::xistream& xis, MIL_Army_ABC& army, MIL_KnowledgeGroup* pParent, KnowledgeGroupFactory_ABC& knowledgeGroupFactory )
    : nID_                  ( xml::attribute< unsigned int >( xis, "id" ) )
    , pType_                ( MIL_KnowledgeGroupType::FindType( xml::attribute< std::string >( xis, "type" ) ) )
    , pArmy_                ( &army )
    , pParent_              ( pParent ) // LTO
    , pKnowledgeBlackBoard_ ( new DEC_KnowledgeBlackBoard_KnowledgeGroup( *this ) )
    , automates_            ()
    , timeToDiffuse_        ( 0 ) // LTO
    , isActivated_          ( true ) // LTO
    , hasBeenUpdated_       ( true )
    , isJammedKnowledgeGroup_ ( false )
    , jamedPion_           ( 0 ) // LTO
{
    idManager_.Lock( nID_ );
    if( pParent_ )
    {
        pParent_->RegisterKnowledgeGroup( *this );
        timeToDiffuse_ = pParent_->GetType().GetKnowledgeCommunicationDelay();
    }
    else
        pArmy_->RegisterKnowledgeGroup( *this );

    xis >> xml::list( "knowledge-group", *this, &MIL_KnowledgeGroup::InitializeKnowledgeGroup, knowledgeGroupFactory );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup constructor
// Created: JVT 2005-03-15
// -----------------------------------------------------------------------------
MIL_KnowledgeGroup::MIL_KnowledgeGroup()
    : pType_               ( 0 )
    , nID_                 ( 0 )
    , pArmy_               ( 0 )
    , pParent_             ( 0 ) // LTO
    , pKnowledgeBlackBoard_( 0 )
    , automates_           ()
    , timeToDiffuse_        ( 0 ) // LTO
    , isActivated_         ( true ) // LTO
    , hasBeenUpdated_      ( false )
    , isJammedKnowledgeGroup_ ( false )
    , jamedPion_           ( 0 ) // LTO
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: KnowledgeGroupFactory::Create
// Created: FDS 2010-03-17
// -----------------------------------------------------------------------------
MIL_KnowledgeGroup::MIL_KnowledgeGroup( const MIL_KnowledgeGroup& source )
    : pType_               ( source.pType_ )
    , nID_                 ( idManager_.GetFreeId() )
    , pArmy_               ( source.pArmy_ )
    , pParent_             ( 0 ) // LTO
    , pKnowledgeBlackBoard_( new DEC_KnowledgeBlackBoard_KnowledgeGroup( *this ) )
    , automates_           ()
    , timeToDiffuse_        ( 0 ) // LTO
    , isActivated_         ( true ) // LTO
    , hasBeenUpdated_      ( true )
    , isJammedKnowledgeGroup_ ( false )
    , jamedPion_           ( 0 ) // LTO
{   
    
    ids_.insert( nID_ );
    pArmy_->RegisterKnowledgeGroup( *this );
    SendCreation();

    source.GetKnowledge().GetKnowledgeAgentContainer().ApplyOnKnowledgesAgent( boost::bind( &MIL_KnowledgeGroup::CreateKnowledgesFromAgentPerception, this, _1 ) ); 
    source.GetKnowledge().GetKnowledgePopulationContainer().ApplyOnKnowledgesPopulation( boost::bind( &MIL_KnowledgeGroup::CreateKnowledgesFromPopulationPerception, this, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::CreateKnowledgesFromAgentPerception
// Created: FDS 2010-03-16
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::CreateKnowledgesFromAgentPerception( const DEC_Knowledge_Agent& agent )
{
    if( agent.IsValid() )
    {
        GetKnowledge().GetKnowledgeAgentContainer().CreateKnowledgeAgent( GetKnowledge().GetKnowledgeGroup(), agent.GetAgentKnown() );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::CreateKnowledgesFromPopulationPerception
// Created: FDS 2010-04-01
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::CreateKnowledgesFromPopulationPerception( const DEC_Knowledge_Population& population )
{
    GetKnowledge().GetKnowledgePopulationContainer().CreateKnowledgePopulation( GetKnowledge().GetKnowledgeGroup(), population.GetPopulationKnown() );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup destructor
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_KnowledgeGroup::~MIL_KnowledgeGroup()
{
    if( pKnowledgeBlackBoard_)
    {
        // LTO begin
        if( GetParent() )
            GetParent()->UnregisterKnowledgeGroup( *this );
        else if( pArmy_ )
        // LTO end
            pArmy_->UnregisterKnowledgeGroup( *this );

        delete pKnowledgeBlackBoard_;
        ids_.erase( nID_ );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::Destroy
// Created: LDC 2010-04-01
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::Destroy()
{
    if( pKnowledgeBlackBoard_)
    {
        // LTO begin
        if( GetParent() )
            GetParent()->UnregisterKnowledgeGroup( *this );
        else if( pArmy_ )
        // LTO end
            pArmy_->UnregisterKnowledgeGroup( *this );

        delete pKnowledgeBlackBoard_;
        ids_.erase( nID_ );

        // myself destruction
        client::KnowledgeGroupDestruction msg;   
        msg().set_oid( nID_ );
        msg().set_oid_camp( pArmy_->GetID() );
        msg.Send( NET_Publisher_ABC::Publisher() );

        pKnowledgeBlackBoard_ = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::InitializeKnowledgeGroup
// Created: SLG 2009-11-11
// LTO
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::InitializeKnowledgeGroup( xml::xistream& xis, KnowledgeGroupFactory_ABC& knowledgeGroupFactory )
{
    knowledgeGroupFactory.Create( xis, *pArmy_, this );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::load
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    unsigned int nTypeID;
    file >> nTypeID;
    pType_ = MIL_KnowledgeGroupType::FindType( nTypeID );
    
    file >> nID_
         >> pArmy_
         >> pParent_ // LTO
         >> pKnowledgeBlackBoard_
         >> automates_
         >> knowledgeGroups_ // LTO
         >> timeToDiffuse_ // LTO
         >> isActivated_ // LTO
         >> isJammedKnowledgeGroup_;
    
    idManager_.Lock( nID_ );
         
    ids_.insert( nID_ );
 
    hasBeenUpdated_ = true;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::save
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    assert( pType_ );
    unsigned int type = pType_->GetID();
    file << type
         << nID_
         << pArmy_
         << pParent_ // LTO
         << pKnowledgeBlackBoard_
         << automates_
         << knowledgeGroups_ // LTO
         << timeToDiffuse_ // LTO
         << isActivated_ // LTO
         << isJammedKnowledgeGroup_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::WriteODB
// Created: NLD 2006-05-29
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::WriteODB( xml::xostream& xos ) const
{
    assert( pType_ );
    xos << xml::start( "knowledge-group" )
            << xml::attribute( "id", nID_ )
            << xml::attribute( "type", pType_->GetName() )
        << xml::end();

    for( CIT_KnowledgeGroupVector it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it ) // LTO
        (**it).WriteODB( boost::ref(xos) );     // LTO
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::UpdateKnowledges
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::UpdateKnowledges(int currentTimeStep)
{
    for( CIT_AutomateVector it = automates_.begin(); it != automates_.end(); ++it )
        (**it).UpdateKnowledges(currentTimeStep);

    for( CIT_KnowledgeGroupVector it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it ) // LTO
        (**it).UpdateKnowledges(currentTimeStep); // LTO

    assert( pKnowledgeBlackBoard_ );
    pKnowledgeBlackBoard_->Update(currentTimeStep);
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::CleanKnowledges
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::CleanKnowledges()
{
    assert( pKnowledgeBlackBoard_ );
    pKnowledgeBlackBoard_->Clean();

    for( CIT_KnowledgeGroupVector it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it ) // LTO
        (**it).CleanKnowledges(); // LTO

    for( CIT_AutomateVector it = automates_.begin(); it != automates_.end(); ++it )
        (**it).CleanKnowledges();
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

    for( CIT_AutomateVector it = automates_.begin(); it != automates_.end(); ++it )
        if( (*it)->IsPerceived( knowledge ) )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::IsPerceived
// Created: NLD 2004-03-22
// -----------------------------------------------------------------------------
bool MIL_KnowledgeGroup::IsPerceived( const DEC_Knowledge_Object& knowledge ) const
{
    for( CIT_AutomateVector it = automates_.begin(); it != automates_.end(); ++it )
        if( (*it)->IsPerceived( knowledge ) )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::SendCreation
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::SendCreation() const
{
    assert( pArmy_ );

    client::KnowledgeGroupCreation msg;   
    msg().set_oid( nID_ );
    msg().set_oid_camp( pArmy_->GetID() );
    msg().set_type(GetType().GetName());
    // LTO begin
    if( pParent_ )
        msg().set_oid_parent( pParent_->GetID() );
    // LTO end
    msg.Send( NET_Publisher_ABC::Publisher() );
    //SLG : @TODO MGD Move to factory
    // LTO begin
    for( CIT_KnowledgeGroupVector it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it )
        (**it).SendCreation();
    // LTO end
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::SendFullState
// Created: SLG 2009-11-13
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::SendFullState() const
{
    for( CIT_KnowledgeGroupVector it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it ) // LTO
        (**it).SendFullState(); // LTO
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::UpdateKnowledgeGroup
// Created: SLG 2009-12-23
// LTO
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::UpdateKnowledgeGroup()
{
    if( hasBeenUpdated_ )
    {
        client::KnowledgeGroupUpdate message;
        message().set_oid( nID_ );
        if( MIL_KnowledgeGroup* pParent = GetParent() )
            message().set_oid_parent( pParent->GetID() );
        else
            // army is the parent
            message().set_oid_parent( 0 );
        message().set_type( GetType().GetName() );
        message().set_enabled( IsEnabled() );
        message.Send( NET_Publisher_ABC::Publisher() ); 
    }

    for( CIT_KnowledgeGroupVector it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it )
        (**it).UpdateKnowledgeGroup(); 

    hasBeenUpdated_ = false;

}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::MoveKnowledgeGroup
// Created: NLD 2004-09-06
// LTO
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::MoveKnowledgeGroup( MIL_KnowledgeGroup *pNewParent )
{
    MIL_KnowledgeGroup *pParent = GetParent();
    if( pNewParent && pParent )
    {
        pParent->UnregisterKnowledgeGroup( *this );
        pNewParent->RegisterKnowledgeGroup( *this );
        hasBeenUpdated_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::SendKnowledge
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::SendKnowledge() const
{
    assert( pKnowledgeBlackBoard_ );
    pKnowledgeBlackBoard_->SendFullState();   

    for( CIT_AutomateVector it = automates_.begin(); it != automates_.end(); ++it )
        (**it).SendKnowledge();

    for( CIT_KnowledgeGroupVector it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it ) // LTO
        (**it).SendKnowledge(); // LTO
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::operator==
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
bool MIL_KnowledgeGroup::operator==( const MIL_KnowledgeGroup& rhs ) const
{
    return nID_ == rhs.nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::operator!=
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
bool MIL_KnowledgeGroup::operator!=( const MIL_KnowledgeGroup& rhs ) const
{
    return nID_ != rhs.nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::GetID
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
unsigned int MIL_KnowledgeGroup::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::GetAutomates
// Created: NLD 2004-09-01
// -----------------------------------------------------------------------------
const MIL_KnowledgeGroup::T_AutomateVector& MIL_KnowledgeGroup::GetAutomates() const
{
    return automates_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::GetKnowledgeGroups
// Created: 
// LTO
// -----------------------------------------------------------------------------
const MIL_KnowledgeGroup::T_KnowledgeGroupVector& MIL_KnowledgeGroup::GetKnowledgeGroups() const
{
    return knowledgeGroups_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::GetKnowledge
// Created: NLD 2004-11-08
// -----------------------------------------------------------------------------
const DEC_KnowledgeBlackBoard_KnowledgeGroup& MIL_KnowledgeGroup::GetKnowledge() const
{
    assert( pKnowledgeBlackBoard_ );
    return *pKnowledgeBlackBoard_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::GetArmy
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
MIL_Army_ABC& MIL_KnowledgeGroup::GetArmy() const
{
   // assert( pArmy_ );
    return *pArmy_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::GetParent
// Created: NLD 2004-09-07
// LTO
// -----------------------------------------------------------------------------
MIL_KnowledgeGroup* MIL_KnowledgeGroup::GetParent() const
{
    return pParent_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::SetParent
// Created: FHD 2009-12-22
// LTO
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::SetParent( MIL_KnowledgeGroup* pParent )
{
    pParent_ = pParent;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::RegisterKnowledgeGroup
// Created: NLD 2006-10-13
// LTO
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::RegisterKnowledgeGroup( MIL_KnowledgeGroup& knowledgeGroup )
{
    assert( std::find( knowledgeGroups_.begin(), knowledgeGroups_.end(), &knowledgeGroup ) == knowledgeGroups_.end() );
    knowledgeGroups_.push_back( &knowledgeGroup );    
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::UnregisterKnowledgeGroup
// Created: FHD 2009-12-16: 
// LTO
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::UnregisterKnowledgeGroup( const MIL_KnowledgeGroup& knowledgeGroup )
{
    IT_KnowledgeGroupVector it = std::find( knowledgeGroups_.begin(), knowledgeGroups_.end(), &knowledgeGroup );
    assert( it != knowledgeGroups_.end() );
    knowledgeGroups_.erase( it );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::RegisterAutomate
// Created: NLD 2004-09-01
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::RegisterAutomate( MIL_Automate& automate )
{
    assert( std::find( automates_.begin(), automates_.end(), &automate ) == automates_.end() );
    automates_.push_back( &automate );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::UnregisterAutomate
// Created: NLD 2004-09-01
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::UnregisterAutomate( MIL_Automate& automate )
{
    IT_AutomateVector it = std::find( automates_.begin(), automates_.end(), &automate );
    assert( it != automates_.end() );
    automates_.erase( it );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::GetType
// Created: JVT 2004-11-29
// -----------------------------------------------------------------------------
const MIL_KnowledgeGroupType& MIL_KnowledgeGroup::GetType() const
{
    assert( pType_ );
    return *pType_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::IsEnabled
// Created: SLG 2009-12-17
// LTO
// -----------------------------------------------------------------------------
bool MIL_KnowledgeGroup::IsEnabled() const
{
    return isActivated_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::FindKnowledgeGroup
// Created: SLG 2009-11-30
// LTO
// -----------------------------------------------------------------------------
MIL_KnowledgeGroup* MIL_KnowledgeGroup::FindKnowledgeGroup( uint nID ) const
{
    MIL_KnowledgeGroup* knowledgeGroup = 0;
    for( CIT_KnowledgeGroupVector itKG( GetKnowledgeGroups().begin() ); itKG != GetKnowledgeGroups().end(); ++itKG )
        if ( (*itKG)->GetID() == nID )
            knowledgeGroup = *itKG;
    if( knowledgeGroup == 0 )
    {
        for( MIL_KnowledgeGroup::CIT_KnowledgeGroupVector itKG( GetKnowledgeGroups().begin() ); itKG != GetKnowledgeGroups().end(); ++itKG )
         {
            knowledgeGroup = (*itKG)->FindKnowledgeGroup( nID );
            if ( knowledgeGroup )
                return knowledgeGroup;         
         }
         return 0;
     }
     return knowledgeGroup;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::GetTimeToDiffuseToKnowledgeGroup
// Created: SLG 2009-11-30
// LTO
// -----------------------------------------------------------------------------
MT_Float MIL_KnowledgeGroup::GetTimeToDiffuseToKnowledgeGroup() const
{
    return timeToDiffuse_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::RefreshTimeToDiffuseToKnowledgeGroup
// Created: SLG 2009-11-30
// LTO
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::RefreshTimeToDiffuseToKnowledgeGroup()
{
    if ( pParent_ )
        timeToDiffuse_ += pParent_->GetType().GetKnowledgeCommunicationDelay();
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::OnReceiveMsgKnowledgeGroupUpdate
// Created: FDS 2010-01-13
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::OnReceiveMsgKnowledgeGroupUpdate( const MsgsClientToSim::MsgKnowledgeGroupUpdateRequest& message, const tools::Resolver< MIL_Army_ABC >& armies  )
{
    if( message.has_enabled() )
       hasBeenUpdated_ = OnReceiveMsgKnowledgeGroupEnable( message ) || hasBeenUpdated_;  
 
    if( message.has_oid_parent() &&  message.has_oid_camp() )
       hasBeenUpdated_ = OnReceiveMsgKnowledgeGroupChangeSuperior( message, armies ) || hasBeenUpdated_;

    if( message.has_type() ) 
       hasBeenUpdated_ = OnReceiveMsgKnowledgeGroupSetType( message ) || hasBeenUpdated_;    
   
    UpdateKnowledgeGroup();
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::OnReceiveMsgKnowledgeGroupEnable
// Created: SLG 2009-12-17
// Modified: FDS 2010-01-13 returns bool to use in OnReceiveMsgKnowledgeGroupUpdate
// -----------------------------------------------------------------------------
bool MIL_KnowledgeGroup::OnReceiveMsgKnowledgeGroupEnable( const MsgsClientToSim::MsgKnowledgeGroupUpdateRequest& message )
{
    if( message.has_enabled() )
    {
        isActivated_ = message.enabled();
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::OnReceiveMsgKnowledgeGroupChangeSuperior
// Created: FHD 2009-12-17:  
// Modified: FDS 2010-01-13 return bool to use in OnReceiveMsgKnowledgeGroupUpdate
// Modified: FDS 2010-01-13 refactor algorithm
// -----------------------------------------------------------------------------
bool MIL_KnowledgeGroup::OnReceiveMsgKnowledgeGroupChangeSuperior( const MsgsClientToSim::MsgKnowledgeGroupUpdateRequest& message, const tools::Resolver< MIL_Army_ABC >& armies )
{
    MIL_Army_ABC* pTargetArmy = armies.Find( message.oid_camp() );
    if( !pTargetArmy || *pTargetArmy != GetArmy() )
        throw NET_AsnException< MsgsSimToClient::KnowledgeGroupAck_ErrorCode >( MsgsSimToClient::KnowledgeGroupAck_ErrorCode_error_invalid_camp );

    MIL_KnowledgeGroup* pNewParent = pTargetArmy->FindKnowledgeGroup( message.oid_parent() );
    if( !pNewParent && message.oid_parent() != 0 )
        throw NET_AsnException< MsgsSimToClient::KnowledgeGroupAck_ErrorCode >( MsgsSimToClient::KnowledgeGroupAck_ErrorCode_error_invalid_superior );
    if( pNewParent )
    {
        MIL_KnowledgeGroup* pParent = GetParent();
        if( pParent && pParent != pNewParent )
        {
            // moving knowledge group from knowledgegroup under knowledgegroup
            pParent->UnregisterKnowledgeGroup( *this );
            pNewParent->RegisterKnowledgeGroup( *this );
            SetParent( pNewParent );
            return true;
        }
        else if( pParent == NULL )
        {
            // moving knowledge group from army node under knowledgegroup
            GetArmy().UnregisterKnowledgeGroup( *this );
            pNewParent->RegisterKnowledgeGroup( *this );
            SetParent( pNewParent );
            return true;
        }
    }
    else
    {
        // moving knowledge group under army node
        MIL_KnowledgeGroup* pParent = GetParent();
        if( pParent )
        {
            pParent->UnregisterKnowledgeGroup( *this );
            GetArmy().RegisterKnowledgeGroup( *this );
            SetParent( NULL );
            return true;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::OnReceiveMsgKnowledgeGroupSetType
// Created: FHD 2009-12-17: 
// Modified: FDS 2010-01-13 return bool to use in OnReceiveMsgKnowledgeGroupUpdate
// -----------------------------------------------------------------------------
bool MIL_KnowledgeGroup::OnReceiveMsgKnowledgeGroupSetType( const MsgsClientToSim::MsgKnowledgeGroupUpdateRequest& message )
{
    const MIL_KnowledgeGroupType* pFoundType = MIL_KnowledgeGroupType::FindType( message.type() );
    if( pFoundType && pFoundType->GetID() != GetType().GetID() )
    {
        SetType( const_cast< MIL_KnowledgeGroupType* >( pFoundType ) );
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::OnReceiveMsgKnowledgeGroupCreation
// Created: FHD 2009-12-17: 
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::OnReceiveMsgKnowledgeGroupCreation( const MsgsClientToSim::MsgKnowledgeGroupCreationRequest& /*message*/ )
{
    // $$$$ _RC_ FDS 2010-01-26: TODO ???? $$$$ TODO
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::IsJammedKnowledgeGroup
// Created: FDS 2010-04-01
// -----------------------------------------------------------------------------
bool MIL_KnowledgeGroup::IsJammedKnowledgeGroup() const
{
    return isJammedKnowledgeGroup_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::SetJammedKnowledgeGroup
// Created: FDS 2010-04-01
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::Jam( const MIL_Agent_ABC& pion )
{
    isJammedKnowledgeGroup_ = true;
    jamedPion_              = &pion;
    pKnowledgeBlackBoard_->Jam();
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< DEC_Knowledge_Object > MIL_KnowledgeGroup::CreateKnowledgeObject
// Created: LDC 2010-04-07
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > MIL_KnowledgeGroup::CreateKnowledgeObject( const MIL_Army_ABC& teamKnowing, MIL_Object_ABC& objectKnown )
{
    return pKnowledgeBlackBoard_->CreateKnowledgeObject( teamKnowing, objectKnown );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::ApplyOnKnowledgesPopulationPerception
// Created: FDS 2010-04-08
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::ApplyOnKnowledgesPopulationPerception()
{
    // $$$$ _RC_ FDS 2010-04-07: Ne fonctionne pas, crash de l'optimiseur. 
    // boost::function< void ( DEC_Knowledge_PopulationPerception& ) > methodPerception = boost::bind( & DEC_KS_PopulationKnowledgeSynthetizer::UpdateKnowledgesFromPerception, this );
    // boost::function< void ( DEC_Knowledge_PopulationCollision& ) > methodCollision   = boost::bind( & DEC_KS_PopulationKnowledgeSynthetizer::UpdateKnowledgesFromCollision, this );

    class_mem_fun_void_const_t< MIL_KnowledgeGroup, DEC_Knowledge_PopulationPerception > methodPerception( & MIL_KnowledgeGroup::UpdatePopulationKnowledgeFromPerception, *this );
    class_mem_fun_void_const_t< MIL_KnowledgeGroup, DEC_Knowledge_PopulationCollision  > methodCollision ( & MIL_KnowledgeGroup::UpdatePopulationKnowledgeFromCollision , *this );

    if( ! IsJammedKnowledgeGroup() )
    {
        
        const MIL_KnowledgeGroup::T_AutomateVector& automates = GetAutomates();
        for( MIL_KnowledgeGroup::CIT_AutomateVector itAutomate = automates.begin(); itAutomate != automates.end(); ++itAutomate )
        {
            const MIL_Automate::T_PionVector& pions = (**itAutomate).GetPions();
            for( MIL_Automate::CIT_PionVector itPion = pions.begin(); itPion != pions.end(); ++itPion )
            {
                MIL_AgentPion& pion = **itPion;
                if( pion.GetRole< PHY_RolePion_Communications >().CanCommunicate() )
                {
                    pion.GetKnowledge().GetKnowledgePopulationPerceptionContainer().ApplyOnKnowledgesPopulationPerception( methodPerception );
                    pion.GetKnowledge().GetKnowledgePopulationCollisionContainer ().ApplyOnKnowledgesPopulationCollision ( methodCollision  );
                }
            }
        }
    }
    else
    {
        if( jamedPion_ )
        {
            jamedPion_->GetKnowledge().GetKnowledgePopulationPerceptionContainer().ApplyOnKnowledgesPopulationPerception( methodPerception );
            jamedPion_->GetKnowledge().GetKnowledgePopulationCollisionContainer ().ApplyOnKnowledgesPopulationCollision ( methodCollision  );
        }
    }

   // pKnowledgeBlackBoard_->SendFullState(); 
}


// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::ApplyOnKnowledgesAgentPerception
// Created: FDS 2010-04-09
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::ApplyOnKnowledgesAgentPerception( int currentTimeStep )
{
    if( IsJammedKnowledgeGroup() )
    {
        // Synthèse de la perception des subordonnés
        // Ajout automatique de la connaissance de chaque subordonné    
        for( MIL_KnowledgeGroup::CIT_AutomateVector itAutomate = GetAutomates().begin(); itAutomate != GetAutomates().end(); ++itAutomate )
        {
            const MIL_Automate::T_PionVector& pions = (**itAutomate).GetPions();
            for( MIL_Automate::CIT_PionVector itPion = pions.begin(); itPion != pions.end(); ++itPion )
            {
                MIL_AgentPion& pion = **itPion;
                 // $$$$ FDS 2010-03-25: Les perceptions des subordonnées sont envoyées uniquement dans le cas ou celui ci peut communiquer.
                if ( pion.GetRole< PHY_RolePion_Communications >().CanCommunicate() )
                {
                        pion.GetKnowledge().GetKnowledgeAgentPerceptionContainer().ApplyOnKnowledgesAgentPerception( boost::bind( & MIL_KnowledgeGroup::UpdateAgentKnowledgeFromAgentPerception, this, _1, boost::ref(currentTimeStep) ) );
                }
            }
        }

        // mis à jour des groupes de connaissances parents
        for( MIL_KnowledgeGroup::CIT_KnowledgeGroupVector itKG( GetKnowledgeGroups().begin() ); itKG != GetKnowledgeGroups().end(); ++itKG )
        {
            const MIL_KnowledgeGroup& innerKg = **itKG;
            if ( innerKg.IsEnabled() && IsEnabled() )
                innerKg.GetKnowledge().GetKnowledgeAgentContainer().ApplyOnKnowledgesAgent( boost::bind( &MIL_KnowledgeGroup::UpdateAgentKnowledgeFromParentKnowledgeGroup, this, _1, boost::ref(currentTimeStep) ) );
        }

        // LTO begin
        //mis à jour des groupes de connaissances fils avec un délai
        //boost::function< void ( const DEC_Knowledge_Agent& ) > functorChildren = boost::bind( &DEC_KS_AgentKnowledgeSynthetizer::UpdateKnowledgesFromParentKnowledgeGroup, this, _1, boost::ref(currentTimeStep) );
        MIL_KnowledgeGroup* pParent = GetParent();
        if ( GetTimeToDiffuseToKnowledgeGroup() < currentTimeStep )
        {
            if ( pParent && pParent->IsEnabled() && IsEnabled() )
                pParent->GetKnowledge().GetKnowledgeAgentContainer().ApplyOnKnowledgesAgent( boost::bind( &MIL_KnowledgeGroup::UpdateAgentKnowledgeFromParentKnowledgeGroup, this, _1, boost::ref(currentTimeStep) ) );
            RefreshTimeToDiffuseToKnowledgeGroup();
        }
        // LTO end
    }
    else
    {
        if( jamedPion_ )
        {
            jamedPion_->GetKnowledge().GetKnowledgeAgentPerceptionContainer().ApplyOnKnowledgesAgentPerception( boost::bind( & MIL_KnowledgeGroup::UpdateAgentKnowledgeFromAgentPerception, this, _1, boost::ref(currentTimeStep) ) );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::GetPopulationKnowledgeToUpdate
// Created: FDS 2010-04-08
// -----------------------------------------------------------------------------
DEC_Knowledge_Population& MIL_KnowledgeGroup::GetPopulationKnowledgeToUpdate( MIL_Population& populationKnown ) const
{
    DEC_Knowledge_Population* pKnowledge = pKnowledgeBlackBoard_->GetKnowledgePopulationContainer().GetKnowledgePopulation( populationKnown );
    if( pKnowledge )
        return *pKnowledge;
    
    return pKnowledgeBlackBoard_->GetKnowledgePopulationContainer().CreateKnowledgePopulation( *this, populationKnown );
}
 
// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::UpdatePopulationKnowledgeFromPerception
// Created: FDS 2010-04-08
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::UpdatePopulationKnowledgeFromPerception( const DEC_Knowledge_PopulationPerception& perception )
{
    GetPopulationKnowledgeToUpdate( perception.GetPopulationPerceived() ).Update( perception );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::UpdatePopulationKnowledgeFromCollision
// Created: FDS 2010-04-08
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::UpdatePopulationKnowledgeFromCollision( const DEC_Knowledge_PopulationCollision& collision )
{
    GetPopulationKnowledgeToUpdate( collision.GetPopulation() ).Update( collision );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::UpdateAgentKnowledgeFromAgentPerception
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
inline
void MIL_KnowledgeGroup::UpdateAgentKnowledgeFromAgentPerception( const DEC_Knowledge_AgentPerception& perception, int currentTimeStep )
{
    if( perception.IsAvailable() )
        GetAgentKnowledgeToUpdate( perception.GetAgentPerceived() ).Update( perception, currentTimeStep );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::UpdateAgentKnowledgeFromParentKnowledgeGroup
// Created: SLG 2009-11-26
// LTO
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::UpdateAgentKnowledgeFromParentKnowledgeGroup( const DEC_Knowledge_Agent& agentKnowledge, int currentTimeStep )
{
    if( agentKnowledge.IsValid() )
        GetAgentKnowledgeToUpdate( agentKnowledge.GetAgentKnown() ).Update( agentKnowledge, currentTimeStep );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::GetAgentKnowledgeToUpdate
// Created: NLD 2004-03-19
// -----------------------------------------------------------------------------
inline
DEC_Knowledge_Agent& MIL_KnowledgeGroup::GetAgentKnowledgeToUpdate( MIL_Agent_ABC& agentKnown ) const
{
    boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge = GetKnowledge().GetKnowledgeAgentContainer().GetKnowledgeAgent( agentKnown );
    if( pKnowledge.get() )
        return *pKnowledge;
    
    return GetKnowledge().GetKnowledgeAgentContainer().CreateKnowledgeAgent( *this, agentKnown );
}