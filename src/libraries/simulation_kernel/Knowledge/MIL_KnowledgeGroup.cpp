// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
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
#include "tools/MIL_IDManager.h"
#include <MT/MT_IO/MT_FormatString.h>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/ref.hpp>
#include <xeumeuleu/xml.hpp>
#include <boost/serialization/export.hpp>

MIL_IDManager MIL_KnowledgeGroup::idManager_;

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_KnowledgeGroup )

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup constructor
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_KnowledgeGroup::MIL_KnowledgeGroup( const MIL_KnowledgeGroupType& type, unsigned int id, MIL_Army_ABC& army )
    : type_               ( &type )
    , id_                 ( id )
    , army_               ( &army )
    , parent_             ( 0 ) // LTO
    , knowledgeBlackBoard_( new DEC_KnowledgeBlackBoard_KnowledgeGroup( *this ) )
    , timeToDiffuse_      ( 0 ) // LTO
    , isActivated_        ( true ) // LTO
    , hasBeenUpdated_     ( false )
    , isJammed_           ( false )
    , jammedPion_         ( 0 )
{
    idManager_.Lock( id_ );
    army_->RegisterKnowledgeGroup( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup constructor
// Created: SLG 2009-11-11
// LTO
// -----------------------------------------------------------------------------
MIL_KnowledgeGroup::MIL_KnowledgeGroup( xml::xistream& xis, MIL_Army_ABC& army, MIL_KnowledgeGroup* parent, KnowledgeGroupFactory_ABC& knowledgeGroupFactory )
    : id_                 ( xis.attribute< unsigned int >( "id" ) )
    , type_               ( MIL_KnowledgeGroupType::FindType( xis.attribute< std::string >( "type" ) ) )
    , army_               ( &army )
    , parent_             ( parent ) // LTO
    , knowledgeBlackBoard_( new DEC_KnowledgeBlackBoard_KnowledgeGroup( *this ) )
    , timeToDiffuse_      ( 0 ) // LTO
    , isActivated_        ( true ) // LTO
    , hasBeenUpdated_     ( true )
    , isJammed_           ( false )
    , jammedPion_         ( 0 )
{
    idManager_.Lock( id_ );
    if( parent_ )
    {
        parent_->RegisterKnowledgeGroup( *this );
        timeToDiffuse_ = parent_->GetType().GetKnowledgeCommunicationDelay();
    }
    else
        army_->RegisterKnowledgeGroup( *this );
    xis >> xml::list( "knowledge-group", *this, &MIL_KnowledgeGroup::InitializeKnowledgeGroup, knowledgeGroupFactory );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup constructor
// Created: JVT 2005-03-15
// -----------------------------------------------------------------------------
MIL_KnowledgeGroup::MIL_KnowledgeGroup()
    : type_               ( 0 )
    , id_                 ( 0 )
    , army_               ( 0 )
    , parent_             ( 0 ) // LTO
    , knowledgeBlackBoard_( 0 )
    , timeToDiffuse_      ( 0 ) // LTO
    , isActivated_        ( true ) // LTO
    , hasBeenUpdated_     ( false )
    , isJammed_           ( false )
    , jammedPion_         ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupFactory::Create
// Created: FDS 2010-03-17
// -----------------------------------------------------------------------------
MIL_KnowledgeGroup::MIL_KnowledgeGroup( const MIL_KnowledgeGroup& source, const MIL_Agent_ABC& pion, MIL_KnowledgeGroup* parent )
    : type_               ( source.type_ )
    , id_                 ( idManager_.GetFreeId() )
    , army_               ( source.army_ )
    , parent_             ( parent ) // LTO
    , knowledgeBlackBoard_( new DEC_KnowledgeBlackBoard_KnowledgeGroup( *this ) )
    , timeToDiffuse_      ( 0 ) // LTO
    , isActivated_        ( true ) // LTO
    , hasBeenUpdated_     ( true )
    , isJammed_           ( true )
    , jammedPion_         ( &pion )
{
    if( parent )
    {
        parent->RegisterKnowledgeGroup( *this );
        timeToDiffuse_ = parent_->GetType().GetKnowledgeCommunicationDelay();
    }
    else
        army_->RegisterKnowledgeGroup( *this );
    SendCreation();
    source.ApplyOnKnowledgesAgent( boost::bind( &MIL_KnowledgeGroup::CreateKnowledgeFromAgentPerception, this, _1 ) );
    source.ApplyOnKnowledgesPopulation( boost::bind( &MIL_KnowledgeGroup::CreateKnowledgeFromPopulationPerception, this, _1 ) );
    knowledgeBlackBoard_->Jam();
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::CreateKnowledgeFromAgentPerception
// Created: FDS 2010-03-16
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::CreateKnowledgeFromAgentPerception( const DEC_Knowledge_Agent& agent )
{
    if( agent.IsValid() )
    {
        DEC_Knowledge_Agent& knowledge = CreateKnowledgeAgent( agent.GetAgentKnown() );
        knowledge.CopyFrom( agent );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::CreateKnowledgeFromPopulationPerception
// Created: FDS 2010-04-01
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::CreateKnowledgeFromPopulationPerception( const DEC_Knowledge_Population& population )
{
    DEC_Knowledge_Population& knowledge = CreateKnowledgePopulation( population.GetPopulationKnown() );
    knowledge.CopyFrom( population );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup destructor
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_KnowledgeGroup::~MIL_KnowledgeGroup()
{
    if( knowledgeBlackBoard_)
    {
        // LTO begin
        if( GetParent() )
            GetParent()->UnregisterKnowledgeGroup( *this );
        else if( army_ )
        // LTO end
            army_->UnregisterKnowledgeGroup( *this );
        delete knowledgeBlackBoard_;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::Destroy
// Created: LDC 2010-04-01
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::Destroy()
{
    if( knowledgeBlackBoard_ )
    {
        // LTO begin
        if( GetParent() )
            GetParent()->UnregisterKnowledgeGroup( *this );
        else if( army_ )
        // LTO end
            army_->UnregisterKnowledgeGroup( *this );
        delete knowledgeBlackBoard_;
        // myself destruction
        client::KnowledgeGroupDestruction msg;
        msg().mutable_id()->set_id( id_ );
        msg().mutable_party()->set_id( army_->GetID() );
        msg.Send( NET_Publisher_ABC::Publisher() );
        knowledgeBlackBoard_ = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::InitializeKnowledgeGroup
// Created: SLG 2009-11-11
// LTO
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::InitializeKnowledgeGroup( xml::xistream& xis, KnowledgeGroupFactory_ABC& knowledgeGroupFactory )
{
    knowledgeGroupFactory.Create( xis, *army_, this );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::load
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    unsigned int typeId;
    file >> typeId;
    type_ = MIL_KnowledgeGroupType::FindType( typeId );
    file >> id_
         >> army_
         >> parent_ // LTO
         >> knowledgeBlackBoard_
         >> automates_
         >> knowledgeGroups_ // LTO
         >> timeToDiffuse_ // LTO
         >> isActivated_ // LTO
         >> isJammed_;
    idManager_.Lock( id_ );
//    ids_.insert( id_ );
    hasBeenUpdated_ = true;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::save
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    if( ! type_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "imposible to save knowledge group with undefined type. Knowledge group Id : '%d' ", id_ ) );
    unsigned int type = type_->GetID();
    file << type
         << id_
         << army_
         << parent_ // LTO
         << knowledgeBlackBoard_
         << automates_
         << knowledgeGroups_ // LTO
         << timeToDiffuse_ // LTO
         << isActivated_ // LTO
         << isJammed_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::WriteODB
// Created: NLD 2006-05-29
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::WriteODB( xml::xostream& xos ) const
{
    assert( type_ );
    xos << xml::start( "knowledge-group" )
            << xml::attribute( "id", id_ )
            << xml::attribute( "type", type_->GetName() )
        << xml::end;
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
    if( jammedPion_ )
        jammedPion_->GetKnowledge().Update(currentTimeStep);
    assert( knowledgeBlackBoard_ );
    knowledgeBlackBoard_->Update(currentTimeStep);
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::CleanKnowledges
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::CleanKnowledges()
{
    assert( knowledgeBlackBoard_ );
    knowledgeBlackBoard_->Clean();
    for( CIT_KnowledgeGroupVector it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it ) // LTO
        (**it).CleanKnowledges(); // LTO
    for( CIT_AutomateVector it = automates_.begin(); it != automates_.end(); ++it )
        (**it).CleanKnowledges();
    if( jammedPion_ )
        jammedPion_->GetKnowledge().Clean();
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
    assert( army_ );
    client::KnowledgeGroupCreation msg;
    msg().mutable_id()->set_id( id_ );
    msg().mutable_party()->set_id( army_->GetID() );
    msg().set_type(GetType().GetName());
    // LTO begin
    if( parent_ )
        msg().mutable_parent()->set_id( parent_->GetId() );
    // LTO end
    if( isJammed_ )
        msg().set_jam( true );
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
        message().mutable_id()->set_id( id_ );
        if( MIL_KnowledgeGroup* parent = GetParent() )
            message().mutable_parent()->set_id( parent->GetId() );
        else
            // army is the parent
            message().mutable_parent()->set_id( 0 );
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
void MIL_KnowledgeGroup::MoveKnowledgeGroup( MIL_KnowledgeGroup *newParent )
{
    MIL_KnowledgeGroup *parent = GetParent();
    if( newParent && parent )
    {
        parent->UnregisterKnowledgeGroup( *this );
        newParent->RegisterKnowledgeGroup( *this );
        hasBeenUpdated_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::SendKnowledge
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::SendKnowledge() const
{
    assert( knowledgeBlackBoard_ );
    knowledgeBlackBoard_->SendFullState();
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
    return id_ == rhs.id_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::operator!=
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
bool MIL_KnowledgeGroup::operator!=( const MIL_KnowledgeGroup& rhs ) const
{
    return id_ != rhs.id_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::GetId
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
unsigned int MIL_KnowledgeGroup::GetId() const
{
    return id_;
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
    assert( knowledgeBlackBoard_ );
    return *knowledgeBlackBoard_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::GetArmy
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
MIL_Army_ABC& MIL_KnowledgeGroup::GetArmy() const
{
   // assert( army_ );
    return *army_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::GetParent
// Created: NLD 2004-09-07
// LTO
// -----------------------------------------------------------------------------
MIL_KnowledgeGroup* MIL_KnowledgeGroup::GetParent() const
{
    return parent_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::SetParent
// Created: FHD 2009-12-22
// LTO
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::SetParent( MIL_KnowledgeGroup* parent )
{
    parent_ = parent;
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
    assert( type_ );
    return *type_;
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
MIL_KnowledgeGroup* MIL_KnowledgeGroup::FindKnowledgeGroup( unsigned int id ) const
{
    MIL_KnowledgeGroup* knowledgeGroup = 0;
    for( CIT_KnowledgeGroupVector itKG( GetKnowledgeGroups().begin() ); itKG != GetKnowledgeGroups().end(); ++itKG )
        if( (*itKG)->GetId() == id )
            knowledgeGroup = *itKG;
    if( knowledgeGroup == 0 )
    {
        for( MIL_KnowledgeGroup::CIT_KnowledgeGroupVector itKG( GetKnowledgeGroups().begin() ); itKG != GetKnowledgeGroups().end(); ++itKG )
         {
            knowledgeGroup = (*itKG)->FindKnowledgeGroup( id );
            if( knowledgeGroup )
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
    if( parent_ )
        timeToDiffuse_ += parent_->GetType().GetKnowledgeCommunicationDelay();
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::OnReceiveMsgKnowledgeGroupUpdate
// Created: FDS 2010-01-13
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::OnReceiveMsgKnowledgeGroupUpdate( const MsgsClientToSim::MsgKnowledgeMagicAction& message, const tools::Resolver< MIL_Army_ABC >& armies  )
{
    switch( message.type() )
    {
    case MsgsClientToSim::MsgKnowledgeMagicAction_Type_enable :
        hasBeenUpdated_ = OnReceiveMsgKnowledgeGroupEnable( message.parametres() ) || hasBeenUpdated_;
        break;
    case MsgsClientToSim::MsgKnowledgeMagicAction_Type_update_side :
        hasBeenUpdated_ = OnReceiveMsgKnowledgeGroupChangeSuperior( message.parametres(), armies, false ) || hasBeenUpdated_;
        break;
    case MsgsClientToSim::MsgKnowledgeMagicAction_Type_update_side_parent :
        hasBeenUpdated_ = OnReceiveMsgKnowledgeGroupChangeSuperior( message.parametres(), armies, true ) || hasBeenUpdated_;
        break;
    case MsgsClientToSim::MsgKnowledgeMagicAction_Type_update_type :
        hasBeenUpdated_ = OnReceiveMsgKnowledgeGroupSetType( message.parametres() ) || hasBeenUpdated_;
        break;
    default:
        break;
    }
    UpdateKnowledgeGroup();
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::OnReceiveMsgKnowledgeGroupEnable
// Created: SLG 2009-12-17
// -----------------------------------------------------------------------------
bool MIL_KnowledgeGroup::OnReceiveMsgKnowledgeGroupEnable( const Common::MsgMissionParameters& message )
{
    isActivated_ = message.elem( 0 ).value().abool();
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::OnReceiveMsgKnowledgeGroupChangeSuperior
// Created: FHD 2009-12-17:
// -----------------------------------------------------------------------------
bool MIL_KnowledgeGroup::OnReceiveMsgKnowledgeGroupChangeSuperior( const Common::MsgMissionParameters& message, const tools::Resolver< MIL_Army_ABC >& armies, bool hasParent )
{
    MIL_Army_ABC* pTargetArmy = armies.Find( message.elem( 0 ).value().party().id() );
    if( !pTargetArmy || *pTargetArmy != GetArmy() )
        throw NET_AsnException< MsgsSimToClient::KnowledgeGroupAck_ErrorCode >( MsgsSimToClient::KnowledgeGroupAck_ErrorCode_error_invalid_camp );
    MIL_KnowledgeGroup* pNewParent = 0;
    if( hasParent )
    {
        pNewParent = pTargetArmy->FindKnowledgeGroup( message.elem( 1 ).value().knowledgegroup().id() );
        if( !pNewParent || pNewParent->IsJammed() )
            throw NET_AsnException< MsgsSimToClient::KnowledgeGroupAck_ErrorCode >( MsgsSimToClient::KnowledgeGroupAck_ErrorCode_error_invalid_superior );
    }
    if( pNewParent )
    {
        MIL_KnowledgeGroup* parent = GetParent();
        if( parent && parent != pNewParent )
        {
            // moving knowledge group from knowledgegroup under knowledgegroup
            parent->UnregisterKnowledgeGroup( *this );
            pNewParent->RegisterKnowledgeGroup( *this );
            SetParent( pNewParent );
            return true;
        }
        else if( parent == NULL )
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
        MIL_KnowledgeGroup* parent = GetParent();
        if( parent )
        {
            parent->UnregisterKnowledgeGroup( *this );
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
// -----------------------------------------------------------------------------
bool MIL_KnowledgeGroup::OnReceiveMsgKnowledgeGroupSetType( const Common::MsgMissionParameters& message )
{
    const MIL_KnowledgeGroupType* pFoundType = MIL_KnowledgeGroupType::FindType( message.elem( 0 ).value().acharstr() );
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
    // TODO
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::IsJammed
// Created: FDS 2010-04-01
// -----------------------------------------------------------------------------
bool MIL_KnowledgeGroup::IsJammed() const
{
    return isJammed_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::CreateKnowledgeObject
// Created: LDC 2010-04-07
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > MIL_KnowledgeGroup::CreateKnowledgeObject( const MIL_Army_ABC& teamKnowing, MIL_Object_ABC& objectKnown )
{
    return knowledgeBlackBoard_->CreateKnowledgeObject( teamKnowing, objectKnown );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::ApplyOnKnowledgesPerception
// Created: FDS 2010-04-08
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::ApplyOnKnowledgesPerception( int currentTimeStep )
{
    ApplyOnKnowledgesPopulationPerception( currentTimeStep );
    ApplyOnKnowledgesAgentPerception( currentTimeStep );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::ApplyOnKnowledgesPopulationPerception
// Created: FDS 2010-04-08
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::ApplyOnKnowledgesPopulationPerception( int currentTimeStep )
    {
    if( ! IsJammed() )
    {
        const MIL_KnowledgeGroup::T_AutomateVector& automates = GetAutomates();
        for( MIL_KnowledgeGroup::CIT_AutomateVector itAutomate = automates.begin(); itAutomate != automates.end(); ++itAutomate )
        {
            const MIL_Automate::T_PionVector& pions = (**itAutomate).GetPions();
            for( MIL_Automate::CIT_PionVector itPion = pions.begin(); itPion != pions.end(); ++itPion )
            {
                MIL_AgentPion& pion = **itPion;
                if( pion.GetRole< PHY_RolePion_Communications >().CanReceive() )
                {
                    pion.GetKnowledge().GetKnowledgePopulationPerceptionContainer().ApplyOnKnowledgesPopulationPerception( boost::bind( & MIL_KnowledgeGroup::UpdatePopulationKnowledgeFromPerception, this, _1, boost::ref(currentTimeStep) ) );
                    pion.GetKnowledge().GetKnowledgePopulationCollisionContainer ().ApplyOnKnowledgesPopulationCollision ( boost::bind( & MIL_KnowledgeGroup::UpdatePopulationKnowledgeFromCollision, this, _1, boost::ref(currentTimeStep) )  );
                }
            }
        }
    }
    else
    {
        if( jammedPion_ )
        {
            DEC_KnowledgeBlackBoard_AgentPion& knowledgeBlackboard = jammedPion_->GetKnowledge();
            knowledgeBlackboard.GetKnowledgePopulationPerceptionContainer().ApplyOnKnowledgesPopulationPerception( boost::bind( & MIL_KnowledgeGroup::UpdatePopulationKnowledgeFromPerception, this, _1, boost::ref(currentTimeStep)  ) );
            knowledgeBlackboard.GetKnowledgePopulationCollisionContainer ().ApplyOnKnowledgesPopulationCollision ( boost::bind( & MIL_KnowledgeGroup::UpdatePopulationKnowledgeFromCollision, this, _1, boost::ref(currentTimeStep)  )  );
        }
    }
   // knowledgeBlackBoard_->SendFullState();
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::ApplyOnKnowledgesAgentPerception
// Created: FDS 2010-04-09
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::ApplyOnKnowledgesAgentPerception( int currentTimeStep )
{
    if( ! IsJammed() )
    {
        // Synthèse de la perception des subordonnés
        // Ajout automatique de la connaissance de chaque subordonné
        for( MIL_KnowledgeGroup::CIT_AutomateVector itAutomate = GetAutomates().begin(); itAutomate != GetAutomates().end(); ++itAutomate )
        {
            const MIL_Automate::T_PionVector& pions = (**itAutomate).GetPions();
            for( MIL_Automate::CIT_PionVector itPion = pions.begin(); itPion != pions.end(); ++itPion )
            {
                MIL_AgentPion& pion = **itPion;
                 // Les perceptions des subordonnées sont envoyées uniquement dans le cas ou celui ci peut communiquer.
                if( pion.GetRole< PHY_RolePion_Communications >().CanEmit() )
                {
                        pion.GetKnowledge().GetKnowledgeAgentPerceptionContainer().ApplyOnKnowledgesAgentPerception( boost::bind( & MIL_KnowledgeGroup::UpdateAgentKnowledgeFromAgentPerception, this, _1, boost::ref(currentTimeStep) ) );
                }
            }
        }
        // LTO begin
        // acquisition des connaissances des groupes fils
        for( MIL_KnowledgeGroup::CIT_KnowledgeGroupVector itKG( GetKnowledgeGroups().begin() ); itKG != GetKnowledgeGroups().end(); ++itKG )
        {
            const MIL_KnowledgeGroup& innerKg = **itKG;
            if( innerKg.IsEnabled() && IsEnabled() && !innerKg.IsJammed() )
                innerKg.GetKnowledge().GetKnowledgeAgentContainer().ApplyOnKnowledgesAgent( boost::bind( &MIL_KnowledgeGroup::UpdateAgentKnowledgeFromParentKnowledgeGroup, this, _1, boost::ref(currentTimeStep) ) );
        }
        // LTO end
    }
    const PHY_RolePion_Communications* communications = jammedPion_ ? jammedPion_->RetrieveRole< PHY_RolePion_Communications >() : 0;
    if( !IsJammed() || ( communications && communications->CanReceive() ) )
    {
        // LTO begin
        //mis à jour des groupes de connaissances depuis leur parent avec un délai
        MIL_KnowledgeGroup* parent = GetParent();
        if( GetTimeToDiffuseToKnowledgeGroup() < currentTimeStep )
        {
            if( parent && IsEnabled() )
            {
                parent->GetKnowledge().GetKnowledgeAgentContainer().ApplyOnPreviousKnowledgesAgent( boost::bind( &MIL_KnowledgeGroup::UpdateAgentKnowledgeFromParentKnowledgeGroup, this, _1, boost::ref(currentTimeStep) ) );
                parent->GetKnowledge().GetKnowledgeAgentContainer().SaveAllCurrentKnowledgeAgent();
            }
            RefreshTimeToDiffuseToKnowledgeGroup();
        }
        // LTO end
    }
    if( IsJammed() && jammedPion_ )
        jammedPion_->GetKnowledge().GetKnowledgeAgentPerceptionContainer().ApplyOnKnowledgesAgentPerception( boost::bind( & MIL_KnowledgeGroup::UpdateAgentKnowledgeFromAgentPerception, this, _1, boost::ref(currentTimeStep) ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::GetPopulationKnowledgeToUpdate
// Created: FDS 2010-04-08
// -----------------------------------------------------------------------------
DEC_Knowledge_Population& MIL_KnowledgeGroup::GetPopulationKnowledgeToUpdate( MIL_Population& populationKnown )
{
    DEC_Knowledge_Population* pKnowledge = GetKnowledge().GetKnowledgePopulationContainer().GetKnowledgePopulation( populationKnown );
    if( pKnowledge )
        return *pKnowledge;
    return CreateKnowledgePopulation( populationKnown );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::UpdatePopulationKnowledgeFromPerception
// Created: FDS 2010-04-08
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::UpdatePopulationKnowledgeFromPerception( const DEC_Knowledge_PopulationPerception& perception, int /*currentTimeStep*/  )
{
    GetPopulationKnowledgeToUpdate( perception.GetPopulationPerceived() ).Update( perception );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::UpdatePopulationKnowledgeFromCollision
// Created: FDS 2010-04-08
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::UpdatePopulationKnowledgeFromCollision( const DEC_Knowledge_PopulationCollision& collision, int /*currentTimeStep*/  )
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
    if( agentKnowledge.IsValid() && ( !parent_ || parent_->GetType().GetKnowledgeCommunicationDelay() <= currentTimeStep ) )
        GetAgentKnowledgeToUpdate( agentKnowledge.GetAgentKnown() ).Update( agentKnowledge, currentTimeStep );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::GetAgentKnowledgeToUpdate
// Created: NLD 2004-03-19
// -----------------------------------------------------------------------------
inline
DEC_Knowledge_Agent& MIL_KnowledgeGroup::GetAgentKnowledgeToUpdate( MIL_Agent_ABC& agentKnown )
{
    boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge = GetKnowledge().GetKnowledgeAgentContainer().GetKnowledgeAgent( agentKnown );
    if( pKnowledge.get() )
        return *pKnowledge;
    return CreateKnowledgeAgent( agentKnown );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::CreateKnowledgeAgent
// Created: FDS 2010-04-12
// -----------------------------------------------------------------------------
DEC_Knowledge_Agent& MIL_KnowledgeGroup::CreateKnowledgeAgent( MIL_Agent_ABC& perceived )
{
    return knowledgeBlackBoard_->CreateKnowledgeAgent( *this, perceived );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::CreateKnowledgePopulation
// Created: FDS 2010-04-12
// -----------------------------------------------------------------------------
DEC_Knowledge_Population& MIL_KnowledgeGroup::CreateKnowledgePopulation( MIL_Population& perceived )
{
    return knowledgeBlackBoard_->CreateKnowledgePopulation( *this, perceived );
}
