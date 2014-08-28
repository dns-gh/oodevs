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
#include "MIL_AgentServer.h"
#include "KnowledgesVisitor_ABC.h"
#include "DEC_BlackBoard_CanContainKnowledgeObject.h"
#include "CheckPoints/SerializationTools.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Communications/PHY_RolePion_Communications.h" // LTO
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/MIL_Army.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/MIL_EntityVisitor_ABC.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Populations/MIL_PopulationConcentration.h"
#include "Entities/Populations/MIL_PopulationFlow.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgeAgent.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgeAgentPerception.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgeObject.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgeObjectCollision.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgeObjectPerception.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgePopulation.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgePopulationCollision.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgePopulationPerception.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_Knowledge_AgentPerception.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/DEC_Knowledge_ObjectCollision.h"
#include "Knowledge/DEC_Knowledge_ObjectPerception.h"
#include "Knowledge/DEC_Knowledge_Population.h"
#include "Knowledge/DEC_Knowledge_PopulationCollision.h"
#include "Knowledge/DEC_Knowledge_PopulationPerception.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "Knowledge/DEC_KS_ObjectKnowledgeSynthetizer.h"
#include "Tools/NET_AsnException.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "simulation_terrain/TER_PopulationConcentration_ABC.h"
#include "simulation_terrain/TER_PopulationFlow_ABC.h"
#include "Tools/MIL_IDManager.h"
#include "Tools/MIL_MessageParameters.h"
#include "MT_Tools/MT_FormatString.h"
#include <boost/serialization/scoped_ptr.hpp>
#include <boost/lexical_cast.hpp>

MIL_IDManager MIL_KnowledgeGroup::idManager_;

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_KnowledgeGroup )

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup constructor
// Created: JVT 2005-03-15
// -----------------------------------------------------------------------------
MIL_KnowledgeGroup::MIL_KnowledgeGroup()
    : type_( 0 )
    , id_( 0 )
    , army_( 0 )
    , parent_( 0 )
    , timeToDiffuse_( 0 )
    , isActivated_( true )
    , hasBeenUpdated_( false )
    , isJammed_( false )
    , createdByJamming_( false )
    , crowd_( false )
    , bDiffuseToKnowledgeGroup_( false )
    , hasSavedCurrentKnowledge_( false )
    , jammedPion_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup constructor
// Created: MCO 2013-07-01
// -----------------------------------------------------------------------------
MIL_KnowledgeGroup::MIL_KnowledgeGroup( const MIL_KnowledgeGroupType& type, MIL_KnowledgeGroup& parent )
    : type_( &type )
    , id_( idManager_.GetId() )
    , name_( "knowledge group[" + boost::lexical_cast< std::string >( id_ ) + "]" )
    , army_( parent.army_ )
    , parent_( &parent )
    , knowledgeBlackBoard_( new DEC_KnowledgeBlackBoard_KnowledgeGroup( this ) )
    , timeToDiffuse_( parent.GetType().GetKnowledgeCommunicationDelay() ) // LTO
    , isActivated_( true ) // LTO
    , hasBeenUpdated_( false )
    , isJammed_( false )
    , createdByJamming_( false )
    , crowd_( false )
    , bDiffuseToKnowledgeGroup_( false )
    , hasSavedCurrentKnowledge_( false )
    , jammedPion_( 0 )
{
    SendCreation();
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup constructor
// Created: MCO 2013-07-05
// -----------------------------------------------------------------------------
MIL_KnowledgeGroup::MIL_KnowledgeGroup( const MIL_KnowledgeGroupType& type, MIL_Army_ABC& army, bool crowd )
    : type_( &type )
    , id_( idManager_.GetId() )
    , name_( "knowledge group[" + boost::lexical_cast< std::string >( id_ ) + "]" )
    , army_( &army )
    , parent_( 0 )
    , knowledgeBlackBoard_( new DEC_KnowledgeBlackBoard_KnowledgeGroup( this ) )
    , timeToDiffuse_( 0 ) // LTO
    , isActivated_( true ) // LTO
    , hasBeenUpdated_( false )
    , isJammed_( false )
    , createdByJamming_( false )
    , crowd_( crowd )
    , bDiffuseToKnowledgeGroup_( false )
    , hasSavedCurrentKnowledge_( false )
    , jammedPion_( 0 )
{
    if( !crowd_ )
        SendCreation();
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup constructor
// Created: SLG 2009-11-11
// -----------------------------------------------------------------------------
MIL_KnowledgeGroup::MIL_KnowledgeGroup( xml::xistream& xis, MIL_Army_ABC& army, MIL_KnowledgeGroup* parent )
    : type_( MIL_KnowledgeGroupType::FindType( xis.attribute< std::string >( "type" ) ) )
    , id_( idManager_.GetId( xis.attribute< unsigned int >( "id" ), true ) )
    , name_( xis.attribute< std::string >( "name" ) )
    , army_( &army )
    , parent_( parent )
    , knowledgeBlackBoard_( new DEC_KnowledgeBlackBoard_KnowledgeGroup( this ) )
    , timeToDiffuse_( parent ? parent->GetType().GetKnowledgeCommunicationDelay() : 0 )
    , isActivated_( true )
    , hasBeenUpdated_( true )
    , isJammed_( false )
    , createdByJamming_( false )
    , crowd_( xis.attribute< bool >( "crowd", false ) )
    , bDiffuseToKnowledgeGroup_( false )
    , hasSavedCurrentKnowledge_( false )
    , jammedPion_( 0 )
{
    if( ! type_ )
        throw MASA_EXCEPTION( "Knowledge group '" + boost::lexical_cast< std::string >( id_ )
            + "' cannot be created because its type does not exist: " + xis.attribute< std::string >( "type" ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::Create
// Created: FDS 2010-03-17
// -----------------------------------------------------------------------------
MIL_KnowledgeGroup::MIL_KnowledgeGroup( const MIL_KnowledgeGroup& source, const MIL_Agent_ABC& pion, MIL_KnowledgeGroup* parent )
    : type_( source.type_ )
    , id_( idManager_.GetId() )
    , name_( source.name_ + " (" + pion.GetName() + ")" )
    , army_( source.army_ )
    , parent_( parent )
    , knowledgeBlackBoard_( new DEC_KnowledgeBlackBoard_KnowledgeGroup( this ) )
    , timeToDiffuse_( 0 )
    , isActivated_( true )
    , hasBeenUpdated_( true )
    , isJammed_( true )
    , createdByJamming_( true )
    , crowd_( false )
    , bDiffuseToKnowledgeGroup_( false )
    , hasSavedCurrentKnowledge_( false )
    , jammedPion_( &pion )
{
    SendCreation();
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup destructor
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_KnowledgeGroup::~MIL_KnowledgeGroup()
{
    if( army_ )
    {
        try
        {
            client::KnowledgeGroupDestruction msg;
            msg().mutable_knowledge_group()->set_id( id_ );
            msg().mutable_party()->set_id( army_->GetID() );
            msg.Send( NET_Publisher_ABC::Publisher() );
        }
        catch( const std::exception& )
        {} // Never mind if no publisher registered, just don't throw.
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::Clone
// Created: LDC 2012-08-30
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::Clone( const MIL_KnowledgeGroup& source )
{
    auto functorAgent = boost::bind( &MIL_KnowledgeGroup::CreateKnowledgeFromAgentPerception, this, _1 );
    source.ApplyOnKnowledgesAgent( functorAgent );
    auto functorPopulation = boost::bind( &MIL_KnowledgeGroup::CreateKnowledgeFromPopulationPerception, this, _1 );
    source.ApplyOnKnowledgesPopulation( functorPopulation );
    auto functorObject = boost::bind( &MIL_KnowledgeGroup::CreateKnowledgeFromObjectPerception, this, _1 );
    source.ApplyOnKnowledgesObject( functorObject );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::CreateKnowledgeFromAgentPerception
// Created: FDS 2010-03-16
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::CreateKnowledgeFromAgentPerception( const DEC_Knowledge_Agent& agent )
{
    if( agent.IsValid() )
        CreateKnowledgeAgent( agent.GetAgentKnown() ).CopyFrom( agent );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::CreateKnowledgeFromPopulationPerception
// Created: FDS 2010-04-01
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::CreateKnowledgeFromPopulationPerception( const DEC_Knowledge_Population& population )
{
    CreateKnowledgePopulation( population.GetPopulationKnown() ).CopyFrom( population );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::CreateKnowledgeFromObjectPerception
// Created: JSR 2013-06-19
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::CreateKnowledgeFromObjectPerception( boost::shared_ptr< DEC_Knowledge_Object >& object )
{
    if( object && object->IsValid() )
    {
        boost::shared_ptr< DEC_Knowledge_Object > knowledge =
            object->GetObjectKnown()? CreateKnowledgeObject( *object->GetObjectKnown() )
                                    : CreateKnowledgeObject( object );
        if( knowledge )
        {
            knowledge->CopyFrom( *object );
            knowledge->UpdateOnNetwork();
        }
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
        if( parent_ )
            parent_->UnregisterKnowledgeGroup( shared_from_this() );
        else if( army_ )
            army_->UnregisterKnowledgeGroup( shared_from_this() );
        knowledgeBlackBoard_.reset();
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::Merge
// Created: LDC 2012-04-27
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::Merge( const MIL_KnowledgeGroup& subGroup )
{
    knowledgeBlackBoard_->GetKnowledgeAgentContainer().Merge( subGroup.knowledgeBlackBoard_->GetKnowledgeAgentContainer() );
    if( GetKnowledgeObjectContainer() )
        GetKnowledgeObjectContainer()->Merge( subGroup.knowledgeBlackBoard_->GetKnowledgeObjectContainer() );
    knowledgeBlackBoard_->GetKnowledgePopulationContainer().Merge( subGroup.knowledgeBlackBoard_->GetKnowledgePopulationContainer() );
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
         >> name_
         >> army_
         >> parent_ // LTO
         >> knowledgeBlackBoard_
         >> automates_
         >> knowledgeGroups_ // LTO
         >> timeToDiffuse_ // LTO
         >> isActivated_ // LTO
         >> isJammed_
         >> hasSavedCurrentKnowledge_
         >> crowd_;
    idManager_.GetId( id_, true );
    hasBeenUpdated_ = true;
    knowledgeBlackBoard_->SetKnowledgeGroup( this );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::save
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    if( ! type_ )
        throw MASA_EXCEPTION( MT_FormatString( "imposible to save knowledge group with undefined type. Knowledge group Id : '%d' ", id_ ) );
    unsigned int type = type_->GetID();
    file << type
         << id_
         << name_
         << army_
         << parent_ // LTO
         << knowledgeBlackBoard_
         << automates_
         << knowledgeGroups_ // LTO
         << timeToDiffuse_ // LTO
         << isActivated_ // LTO
         << isJammed_
         << hasSavedCurrentKnowledge_
         << crowd_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::WriteODB
// Created: NLD 2006-05-29
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::WriteODB( xml::xostream& xos ) const
{
    if( crowd_ )
        return;
    assert( type_ );
    xos << xml::start( "knowledge-group" )
            << xml::attribute( "id", id_ )
            << xml::attribute( "name", name_ )
            << xml::attribute( "type", type_->GetName() )
        << xml::end;
    for( auto it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it ) // LTO
        ( **it ).WriteODB( xos );     // LTO
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::WriteKnowledges
// Created: NPT 2012-08-08
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::WriteKnowledges( xml::xostream& xos ) const
{
    assert( type_ );
    xos << xml::start( "knowledge-group" )
        << xml::attribute( "id", id_ )
        << xml::attribute( "name", name_ )
        << xml::attribute( "type", type_->GetName() );
    xos     << xml::start( "objects" );
    if( knowledgeBlackBoard_ )
    {
        const DEC_BlackBoard_CanContainKnowledgeObject::T_KnowledgeObjectMap& objectMap = knowledgeBlackBoard_->GetKnowledgeObjectContainer().GetKnowledgeObjects();
        for( auto it = objectMap.begin(); it != objectMap.end(); ++it )
            it->second->WriteKnowledges( xos );
    }
    xos     << xml::end;

    xos     << xml::start( "populations" );
    if( knowledgeBlackBoard_ )
    {
        const DEC_BlackBoard_CanContainKnowledgePopulation::T_KnowledgePopulationMap& populationMap = knowledgeBlackBoard_->GetKnowledgePopulationContainer().GetKnowledgePopulations();
        for( auto it = populationMap.begin(); it != populationMap.end(); ++it )
            it->second->WriteKnowledges( xos );
    }
    xos     << xml::end;

    xos     << xml::start( "units" );
    if( knowledgeBlackBoard_ )
    {
        const DEC_BlackBoard_CanContainKnowledgeAgent::T_KnowledgeAgentMap& agentMap =  knowledgeBlackBoard_->GetKnowledgeAgentContainer().GetKnowledgeAgents();
        for( auto it = agentMap.begin(); it != agentMap.end(); ++it )
            it->second->WriteKnowledges( xos );
    }
    xos     << xml::end;

    for( auto it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it )
        ( **it ).WriteKnowledges( xos );
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::UpdateKnowledges
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::UpdateKnowledges(int currentTimeStep)
{
    for( auto it = automates_.begin(); it != automates_.end(); ++it )
        (**it).UpdateKnowledges( currentTimeStep );
    for( auto it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it ) // LTO
        (**it).UpdateKnowledges( currentTimeStep ); // LTO
    if( knowledgeBlackBoard_ )
    {
        if( createdByJamming_ )
        {
            knowledgeBlackBoard_->SendChangedState();
            createdByJamming_ = false;
        }
        else
            knowledgeBlackBoard_->Update( currentTimeStep );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::UpdateObjectKnowledges
// Created: LDC 2011-08-12
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::UpdateObjectKnowledges(int currentTimeStep)
{
    if( knowledgeBlackBoard_ )
        knowledgeBlackBoard_->UpdateUniversalObjects();
    for( auto it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it ) // LTO
        (**it).UpdateObjectKnowledges( currentTimeStep );
    if( knowledgeBlackBoard_ )
        knowledgeBlackBoard_->SendObjectChangedState();
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::CleanKnowledges
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::CleanKnowledges()
{
    if( knowledgeBlackBoard_ )
        knowledgeBlackBoard_->Clean();
    for( auto it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it ) // LTO
        (**it).CleanKnowledges(); // LTO
    for( auto it = automates_.begin(); it != automates_.end(); ++it )
        (**it).CleanKnowledges();
    if( jammedPion_ )
        jammedPion_->GetKnowledge().Clean();
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::CleanDeletedAgentKnowledges
// Created: JSR 2013-02-21
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::CleanDeletedAgentKnowledges()
{
    knowledgeBlackBoard_->CleanDeletedAgentKnowledges();
    for( auto it = automates_.begin(); it != automates_.end(); ++it )
        ( *it )->CleanDeletedAgentKnowledges();
    for( auto it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it )
        ( *it )->CleanDeletedAgentKnowledges();
    if( jammedPion_ )
        jammedPion_->GetKnowledge().CleanDeletedAgentKnowledges();
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::IsPerceived
// Created: NLD 2004-03-22
// -----------------------------------------------------------------------------
bool MIL_KnowledgeGroup::IsPerceived( const DEC_Knowledge_Object& knowledge ) const
{
    for( auto it = automates_.begin(); it != automates_.end(); ++it )
        if( (*it)->IsPerceived( knowledge ) )
            return true;
    for( auto it = populations_.begin(); it != populations_.end(); ++it )
        if( (*it)->IsInZone( knowledge.GetLocalisation() ) )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::IsPerceptionDistanceHacked
// Created: MMC 2011-06-14
// -----------------------------------------------------------------------------
bool MIL_KnowledgeGroup::IsPerceptionDistanceHacked( MIL_Agent_ABC& agentKnown ) const
{
    if( knowledgeBlackBoard_ )
    {
        boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge = knowledgeBlackBoard_->GetKnowledgeAgentContainer().GetKnowledgeAgent( agentKnown );
        if( pKnowledge )
            return pKnowledge->IsPerceptionDistanceHacked();
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::IsPerceptionDistanceHacked
// Created: MMC 2011-06-14
// -----------------------------------------------------------------------------
bool MIL_KnowledgeGroup::IsPerceptionDistanceHacked( const MIL_Object_ABC& objectKnown ) const
{
    if( DEC_BlackBoard_CanContainKnowledgeObject* knowledgeObjectContainer = GetKnowledgeObjectContainer() )
        if( DEC_Knowledge_Object* pKnowledge = knowledgeObjectContainer->RetrieveKnowledgeObject( objectKnown ) )
            return pKnowledge->IsPerceptionDistanceHacked();
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::IsPerceptionDistanceHacked
// Created: MMC 2011-06-14
// -----------------------------------------------------------------------------
bool MIL_KnowledgeGroup::IsPerceptionDistanceHacked( MIL_Population& populationKnown ) const
{
    if( knowledgeBlackBoard_ )
    {
        boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = knowledgeBlackBoard_->GetKnowledgePopulationContainer().GetKnowledgePopulation( populationKnown );
        if( pKnowledge )
            return pKnowledge->IsPerceptionDistanceHacked();
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::GetPerceptionLevel
// Created: MMC 2011-06-14
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& MIL_KnowledgeGroup::GetPerceptionLevel( MIL_Agent_ABC& agentKnown ) const
{
    if( knowledgeBlackBoard_ )
    {
        boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge = knowledgeBlackBoard_->GetKnowledgeAgentContainer().GetKnowledgeAgent( agentKnown );
        if( pKnowledge )
            return pKnowledge->GetCurrentPerceptionLevel();
    }
    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::GetPerceptionLevel
// Created: MMC 2011-06-14
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& MIL_KnowledgeGroup::GetPerceptionLevel( const MIL_Object_ABC& objectKnown ) const
{
    if( DEC_BlackBoard_CanContainKnowledgeObject*  knowledgeObjectContainer = GetKnowledgeObjectContainer() )
        if( DEC_Knowledge_Object* pKnowledge = knowledgeObjectContainer->RetrieveKnowledgeObject( objectKnown ) )
            return pKnowledge->GetCurrentPerceptionLevel();
    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::GetPerceptionLevel
// Created: MMC 2011-06-14
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& MIL_KnowledgeGroup::GetPerceptionLevel( MIL_Population& populationKnown ) const
{
    if( knowledgeBlackBoard_ )
    {
        boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = knowledgeBlackBoard_->GetKnowledgePopulationContainer().GetKnowledgePopulation( populationKnown );
        if( pKnowledge )
            return *pKnowledge->GetHackedPerceptionLevel();
    }
    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::SendCreation
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::SendCreation( unsigned int context /*= 0*/ ) const
{
    assert( army_ );
    client::KnowledgeGroupCreation msg;
    msg().mutable_knowledge_group()->set_id( id_ );
    msg().set_name( name_ );
    msg().mutable_party()->set_id( army_->GetID() );
    msg().set_type( GetType().GetName() );
    msg().set_crowd( crowd_ );
    if( parent_ )
        msg().mutable_parent()->set_id( parent_->GetId() );
    if( isJammed_ )
        msg().set_jam( true );
    msg.Send( NET_Publisher_ABC::Publisher(), context );
    for( auto it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it )
        (**it).SendCreation( context );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::SendFullState
// Created: SLG 2009-11-13
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::SendFullState() const
{
    for( auto it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it )
        (**it).SendFullState();
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::UpdateKnowledgeGroup
// Created: SLG 2009-12-23
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::UpdateKnowledgeGroup()
{
    if( hasBeenUpdated_ )
    {
        client::KnowledgeGroupUpdate message;
        message().mutable_knowledge_group()->set_id( id_ );
        if( parent_ )
            message().mutable_parent()->set_id( parent_->GetId() );
        else
            // army is the parent
            message().mutable_parent()->set_id( 0 );
        message().set_type( GetType().GetName() );
        message().set_enabled( IsEnabled() );
        message.Send( NET_Publisher_ABC::Publisher() );
    }
    for( auto it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it )
        (**it).UpdateKnowledgeGroup();
    hasBeenUpdated_ = false;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::SendKnowledge
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::SendKnowledge( unsigned int context ) const
{
    if( knowledgeBlackBoard_ )
        knowledgeBlackBoard_->SendFullState( context );
    for( auto it = automates_.begin(); it != automates_.end(); ++it )
        (**it).SendKnowledge( context );
    for( auto it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it )
        (**it).SendKnowledge( context );
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
// -----------------------------------------------------------------------------
const MIL_KnowledgeGroup::T_KnowledgeGroupVector& MIL_KnowledgeGroup::GetKnowledgeGroups() const
{
    return knowledgeGroups_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::GetKnowledge
// Created: NLD 2004-11-08
// -----------------------------------------------------------------------------
const DEC_KnowledgeBlackBoard_KnowledgeGroup* MIL_KnowledgeGroup::GetKnowledge() const
{
    return knowledgeBlackBoard_.get();
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::ResolveKnowledgeObject
// Created: LDC 2011-06-10
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > MIL_KnowledgeGroup::ResolveKnowledgeObject( unsigned int id ) const
{
    if( !knowledgeBlackBoard_ )
        return boost::shared_ptr< DEC_Knowledge_Object >();
    boost::shared_ptr< DEC_Knowledge_Object > result = knowledgeBlackBoard_->ResolveKnowledgeObject( id );
    if( !result && parent_ )
        return parent_->ResolveKnowledgeObject( id );
    return result;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::ResolveKnowledgeObject
// Created: LGY 2011-07-11
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > MIL_KnowledgeGroup::ResolveKnowledgeObject( const MIL_Object_ABC& object ) const
{
    if( !knowledgeBlackBoard_ )
        return boost::shared_ptr< DEC_Knowledge_Object >();
    boost::shared_ptr< DEC_Knowledge_Object > result = knowledgeBlackBoard_->ResolveKnowledgeObject( object );
    if( !result && parent_ )
        return parent_->ResolveKnowledgeObject( object );
    return result;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::ResolveKnowledgeObjectByObjectID
// Created: JSR 2011-09-29
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > MIL_KnowledgeGroup::ResolveKnowledgeObjectByObjectID( unsigned int id ) const
{
    if( !knowledgeBlackBoard_ )
        return boost::shared_ptr< DEC_Knowledge_Object >();
    boost::shared_ptr< DEC_Knowledge_Object > result = knowledgeBlackBoard_->ResolveKnowledgeObjectByObjectID( id );
    if( !result && parent_ )
        return parent_->ResolveKnowledgeObjectByObjectID( id );
    return result;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::GetArmy
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
MIL_Army_ABC& MIL_KnowledgeGroup::GetArmy() const
{
    return *army_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::RegisterKnowledgeGroup
// Created: NLD 2006-10-13
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::RegisterKnowledgeGroup( const boost::shared_ptr< MIL_KnowledgeGroup >& knowledgeGroup )
{
    knowledgeGroups_.push_back( knowledgeGroup );
    for( auto it = additionalPerceptions_.begin(); it != additionalPerceptions_.end(); ++it )
        knowledgeGroup->additionalPerceptions_.insert( *it ); 
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::UnregisterKnowledgeGroup
// Created: FHD 2009-12-16:
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::UnregisterKnowledgeGroup( const boost::shared_ptr< MIL_KnowledgeGroup >& knowledgeGroup )
{
    knowledgeGroups_.erase( std::remove( knowledgeGroups_.begin(), knowledgeGroups_.end(), knowledgeGroup ), knowledgeGroups_.end() );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::RegisterAutomate
// Created: NLD 2004-09-01
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::RegisterAutomate( MIL_Automate& automate )
{
    automates_.push_back( &automate );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::UnregisterAutomate
// Created: NLD 2004-09-01
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::UnregisterAutomate( MIL_Automate& automate )
{
    automates_.erase( std::remove( automates_.begin(), automates_.end(), &automate ), automates_.end() );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::RegisterPopulation
// Created: JSR 2013-07-10
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::RegisterPopulation( MIL_Population& population )
{
    populations_.push_back( &population );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::UnregisterPopulation
// Created: JSR 2013-07-10
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::UnregisterPopulation( MIL_Population& population )
{
    populations_.erase( std::remove( populations_.begin(), populations_.end(), &population ), populations_.end() );
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
// -----------------------------------------------------------------------------
boost::shared_ptr< MIL_KnowledgeGroup > MIL_KnowledgeGroup::FindKnowledgeGroup( unsigned int id ) const
{
    for( auto it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it )
    {
        if( (*it)->GetId() == id )
            return *it;
        const auto& group = (*it)->FindKnowledgeGroup( id );
        if( group )
            return group;
    }
    return boost::shared_ptr< MIL_KnowledgeGroup >();
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
// Name: MIL_KnowledgeGroup::OnReceiveKnowledgeGroupUpdate
// Created: FDS 2010-01-13
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::OnReceiveKnowledgeGroupUpdate( const sword::KnowledgeMagicAction& message, sword::KnowledgeGroupMagicActionAck &ack, const tools::Resolver< MIL_Army_ABC >& armies  )
{
    if( IsJammed() && message.type() != sword::KnowledgeMagicAction::add_knowledge )
        throw MASA_BADGROUP_KNOWLEDGE( "cannot update jammed knowledge group" );
    const auto& params = message.parameters();
    bool updated = false;
    switch( message.type() )
    {
    case sword::KnowledgeMagicAction::enable:
        updated = OnReceiveKnowledgeGroupEnable( params );
        break;
    case sword::KnowledgeMagicAction::update_party:
    case sword::KnowledgeMagicAction::update_party_parent:
        updated = OnReceiveKnowledgeGroupChangeSuperior( params, armies,
            message.type() == sword::KnowledgeMagicAction::update_party_parent );
        break;
    case sword::KnowledgeMagicAction::update_type:
        updated = OnReceiveKnowledgeGroupSetType( params );
        break;
    case sword::KnowledgeMagicAction::add_knowledge:
        {
            unsigned long knowledgeId = OnReceiveKnowledgeGroupAddKnowledge( params );
            ack.mutable_result()->add_elem()->add_value()->set_identifier( knowledgeId );
        }
        break;
    case sword::KnowledgeMagicAction::rename:
        OnReceiveRename( params );
        break;
    default:
        throw MASA_BADPARAM_ASN( sword::KnowledgeGroupAck_ErrorCode,
            sword::KnowledgeGroupAck::error_invalid_type, "unknown magic action type" );
    }
    hasBeenUpdated_ = hasBeenUpdated_ || updated;
    UpdateKnowledgeGroup();
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::OnReceiveKnowledgeGroupEnable
// Created: SLG 2009-12-17
// -----------------------------------------------------------------------------
bool MIL_KnowledgeGroup::OnReceiveKnowledgeGroupEnable( const sword::MissionParameters& msg )
{
    protocol::CheckCount( msg, 1 );
    const bool before = isActivated_;
    isActivated_ = protocol::GetBool( msg, 0 );
    return before != isActivated_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::OnReceiveKnowledgeGroupChangeSuperior
// Created: FHD 2009-12-17:
// -----------------------------------------------------------------------------
bool MIL_KnowledgeGroup::OnReceiveKnowledgeGroupChangeSuperior( const sword::MissionParameters& msg, const tools::Resolver< MIL_Army_ABC >& armies, bool hasParent )
{
    protocol::CheckCount( msg, hasParent ? 2 : 1 );
    const auto partyId = protocol::GetPartyId( msg, 0 );
    MIL_Army_ABC* pTargetArmy = armies.Find( partyId );
    protocol::Check( pTargetArmy == army_, "army is null or not the knowledge group one" );
    boost::shared_ptr< MIL_KnowledgeGroup > pNewParent;
    if( hasParent )
    {
        const auto id = protocol::GetKnowledgeGroup( msg, 1 );
        pNewParent = pTargetArmy->FindKnowledgeGroup( id );
        protocol::Check( pNewParent, "parent knowledge group is unknown" );
        protocol::Check( !pNewParent->IsJammed(), "parent knowledge group is jammed" );
    }
    if( pNewParent.get() == parent_ )
        return false;
    if( parent_ )
        parent_->UnregisterKnowledgeGroup( shared_from_this() );
    else
        army_->UnregisterKnowledgeGroup( shared_from_this() );
    parent_ = pNewParent.get();
    if( parent_ )
        parent_->RegisterKnowledgeGroup( shared_from_this() );
    else
        army_->RegisterKnowledgeGroup( shared_from_this() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::OnReceiveKnowledgeGroupSetType
// Created: FHD 2009-12-17:
// -----------------------------------------------------------------------------
bool MIL_KnowledgeGroup::OnReceiveKnowledgeGroupSetType( const sword::MissionParameters& msg )
{
    protocol::CheckCount( msg, 1 );
    const std::string type = protocol::GetString( msg, 0 );
    const MIL_KnowledgeGroupType* pFoundType = MIL_KnowledgeGroupType::FindType( type );
    if( pFoundType && pFoundType->GetID() != GetType().GetID() )
    {
        type_ = pFoundType;
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::OnReceiveKnowledgeGroupAddKnowledge
// Created: MMC 2011-06-09:
// -----------------------------------------------------------------------------
unsigned long MIL_KnowledgeGroup::OnReceiveKnowledgeGroupAddKnowledge( const sword::MissionParameters& msg )
{
    protocol::CheckCount( msg, 2 );
    const auto id = protocol::GetIdentifier( msg, 0 );
    additionalPerceptions_.insert( id );
    unsigned int perception = 0;
    if( const auto quantity = protocol::TryGetQuantity( msg, 1 ) )
        perception = *quantity;
    else
        perception = PHY_PerceptionLevel::ConvertFromMsgIdToSimId(
            GET_ENUMERATION( sword::UnitIdentification_Level, msg, 1 ));
    protocol::Check(
        perception == PHY_PerceptionLevel::identified_.GetID() ||
        perception == PHY_PerceptionLevel::recognized_.GetID() ||
        perception == PHY_PerceptionLevel::detected_.GetID(),
        "invalid perception level" );

    unsigned long ret = 0;
    if( MIL_AgentPion* pAgent = MIL_AgentServer::GetWorkspace().GetEntityManager().FindAgentPion( id ))
    {
        if( !pAgent->IsMarkedForDestruction() && pAgent->GetKnowledgeGroup()->GetId() != id_ )
        {
            DEC_Knowledge_Agent& knowledgeAgent = GetAgentKnowledgeToUpdate( *pAgent );
            knowledgeAgent.HackPerceptionLevel( &PHY_PerceptionLevel::FindPerceptionLevel( perception ) );
            HackPerceptionLevelFromParentKnowledgeGroup( *pAgent, perception );
            ret = knowledgeAgent.GetID();
        }
    }
    else if( MIL_Object_ABC* pObject = MIL_AgentServer::GetWorkspace().GetEntityManager().FindObject( id ) )
    {
        protocol::Check( !pObject->IsMarkedForDestruction(), "invalid unit" );
        auto knowledgeObject = GetObjectKnowledgeToUpdate( *pObject );
        protocol::Check( knowledgeObject, "invalid unit" );
        knowledgeObject->HackPerceptionLevel( &PHY_PerceptionLevel::FindPerceptionLevel( perception ) );
        HackPerceptionLevelFromParentKnowledgeGroup( *pObject, perception );
        ret = knowledgeObject->GetID();
    }
    else if( MIL_Population* pPopulation = MIL_AgentServer::GetWorkspace().GetEntityManager().FindPopulation( id ) )
    {
        DEC_Knowledge_Population& knowledgePopulation = GetPopulationKnowledgeToUpdate( *pPopulation );
        knowledgePopulation.HackPerceptionLevel( &PHY_PerceptionLevel::FindPerceptionLevel( perception ) );
        HackPerceptionLevelFromParentKnowledgeGroup( *pPopulation, perception );
        ret = knowledgePopulation.GetID();
    }
    else
        protocol::Check( false, "invalid unit" );
    return ret;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::HackPerceptionLevelFromParentKnowledgeGroup
// Created: MMC 2011-06-14
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::HackPerceptionLevelFromParentKnowledgeGroup( MIL_Agent_ABC& agent, unsigned int perception )
{
    if( agent.IsMarkedForDestruction() )
        return;
    additionalPerceptions_.insert( agent.GetID() );
    for( auto it( knowledgeGroups_.begin() ); it != knowledgeGroups_.end(); ++it )
    {
        boost::shared_ptr< MIL_KnowledgeGroup > pKnowledgeGroup = *it;
        if ( pKnowledgeGroup )
        {
            DEC_Knowledge_Agent& curKnowledgeAgent = pKnowledgeGroup->GetAgentKnowledgeToUpdate( agent );
            curKnowledgeAgent.HackPerceptionLevel( &PHY_PerceptionLevel::FindPerceptionLevel( perception ) );
            pKnowledgeGroup->HackPerceptionLevelFromParentKnowledgeGroup( agent, perception );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::HackPerceptionLevelFromParentKnowledgeGroup
// Created: MMC 2011-06-14
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::HackPerceptionLevelFromParentKnowledgeGroup( MIL_Object_ABC& object, unsigned int perception )
{
    additionalPerceptions_.insert( object.GetID() );
    if( knowledgeBlackBoard_ )
    {
        DEC_Knowledge_Object* knowledgeObject = knowledgeBlackBoard_->GetKnowledgeObjectContainer().RetrieveKnowledgeObject( object );
        if ( knowledgeObject )
            knowledgeObject->HackPerceptionLevel( &PHY_PerceptionLevel::FindPerceptionLevel( perception ) );
        else
            for( auto it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it )
            {
                boost::shared_ptr< MIL_KnowledgeGroup > pKnowledgeGroup = *it;
                if ( pKnowledgeGroup )
                {
                    boost::shared_ptr< DEC_Knowledge_Object > curKnowledgeObject = pKnowledgeGroup->GetObjectKnowledgeToUpdate( object );
                    if( curKnowledgeObject )
                        curKnowledgeObject->HackPerceptionLevel( &PHY_PerceptionLevel::FindPerceptionLevel( perception ) );
                    pKnowledgeGroup->HackPerceptionLevelFromParentKnowledgeGroup( object, perception );
                }
            }
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::HackPerceptionLevelFromParentKnowledgeGroup
// Created: MMC 2011-06-14
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::HackPerceptionLevelFromParentKnowledgeGroup( MIL_Population& population, unsigned int perception )
{
    additionalPerceptions_.insert( population.GetID() );
    for( auto it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it )
    {
        boost::shared_ptr< MIL_KnowledgeGroup > pKnowledgeGroup = *it;
        if( pKnowledgeGroup )
        {
            DEC_Knowledge_Population& curKnowledgePopulation = pKnowledgeGroup->GetPopulationKnowledgeToUpdate( population );
            curKnowledgePopulation.HackPerceptionLevel( &PHY_PerceptionLevel::FindPerceptionLevel( perception ) );
            pKnowledgeGroup->HackPerceptionLevelFromParentKnowledgeGroup( population, perception );
        }
    }
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
// Name: MIL_KnowledgeGroup::IsCrowd
// Created: JSR 2013-07-10
// -----------------------------------------------------------------------------
bool MIL_KnowledgeGroup::IsCrowd() const
{
    return crowd_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::CreateKnowledgeObject
// Created: LDC 2010-04-07
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > MIL_KnowledgeGroup::CreateKnowledgeObject( MIL_Object_ABC& objectKnown )
{
    if( knowledgeBlackBoard_ )
        return knowledgeBlackBoard_->CreateKnowledgeObject( objectKnown );
    return boost::shared_ptr< DEC_Knowledge_Object >();
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::CreateKnowledgeObject
// Created: MMC 2013-07-15
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > MIL_KnowledgeGroup::CreateKnowledgeObject( boost::shared_ptr< DEC_Knowledge_Object >& object )
{
    if( knowledgeBlackBoard_ )
        return knowledgeBlackBoard_->CreateKnowledgeObject( object );
    return boost::shared_ptr< DEC_Knowledge_Object >();
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::AddEphemeralObjectKnowledge
// Created: JSR 2013-12-11
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::AddEphemeralObjectKnowledge( MIL_Object_ABC& objectKnown )
{
    for( auto it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it )
        ( *it )->AddEphemeralObjectKnowledge( objectKnown );
    if( knowledgeBlackBoard_ )
        knowledgeBlackBoard_->GetKsObjectKnowledgeSynthetizer().AddEphemeralObjectKnowledge( objectKnown );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::ApplyOnKnowledgesPerception
// Created: FDS 2010-04-08
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::ApplyOnKnowledgesPerception( int currentTimeStep )
{
    ApplyOnKnowledgesPopulationPerception( currentTimeStep );
    ApplyOnKnowledgesAgentPerception( currentTimeStep );
    ApplyOnKnowledgesObjectPerception( currentTimeStep );
    if( bDiffuseToKnowledgeGroup_ )
    {
        bDiffuseToKnowledgeGroup_ = false;
        RefreshTimeToDiffuseToKnowledgeGroup();
    }
    if( !hasSavedCurrentKnowledge_ )
    {
        GetKnowledge()->GetKnowledgeAgentContainer().SaveAllCurrentKnowledgeAgent();
        hasSavedCurrentKnowledge_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::ApplyPopulationPerception
// Created: LGY 2013-05-07
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::ApplyPopulationPerception( const MIL_Agent_ABC& pion, int currentTimeStep )
{
    std::function< void( DEC_Knowledge_PopulationPerception& ) > functorPopulationPerception = boost::bind( & MIL_KnowledgeGroup::UpdatePopulationKnowledgeFromPerception, this, _1, boost::ref(currentTimeStep) );
    pion.GetKnowledge().GetKnowledgePopulationPerceptionContainer().ApplyOnKnowledgesPopulationPerception( functorPopulationPerception );
    std::function< void( DEC_Knowledge_PopulationCollision& ) > functorPopulationCollision = boost::bind( & MIL_KnowledgeGroup::UpdatePopulationKnowledgeFromCollision, this, _1, boost::ref(currentTimeStep) );
    pion.GetKnowledge().GetKnowledgePopulationCollisionContainer ().ApplyOnKnowledgesPopulationCollision ( functorPopulationCollision );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::ApplyAgentPerception
// Created: LGY 2013-05-07
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::ApplyAgentPerception( const MIL_Agent_ABC& pion, int currentTimeStep )
{
    std::function< void( DEC_Knowledge_AgentPerception& ) > functorAgent = boost::bind( & MIL_KnowledgeGroup::UpdateAgentKnowledgeFromAgentPerception, this, _1, boost::ref( currentTimeStep ) );
    pion.GetKnowledge().GetKnowledgeAgentPerceptionContainer().ApplyOnKnowledgesAgentPerception( functorAgent );
}

namespace
{
    template< class T >
    class sCollidingPopulationVisitor : public MIL_EntityVisitor_ABC< MIL_PopulationElement_ABC >
    {
        typedef const std::vector< T* >&( MIL_PopulationElement_ABC::*T_Getter )() const;
    public:
        explicit sCollidingPopulationVisitor( std::function< void( T& ) > fun, T_Getter getter )
            : fun_( fun )
            , getter_( getter )
        {
            //NOTHING
        }
        virtual void Visit( const MIL_PopulationElement_ABC& element )
        {
            auto collidingAgents = ( element.*getter_)();
            for( auto it = collidingAgents.begin(); it != collidingAgents.end(); ++it )
                fun_( **it );
        }
    private:
        std::function< void( T& ) > fun_;
        T_Getter getter_;
    };
        
    class sPopulationsCollidingPopulationVisitor : public MIL_EntityVisitor_ABC< MIL_PopulationElement_ABC >
    {
    public:
        sPopulationsCollidingPopulationVisitor( MIL_KnowledgeGroup& group, const MIL_Population& perceiver )
            : group_( group )
            , perceiver_( perceiver )
        {
            //NOTHING
        }
        virtual void Visit( const MIL_PopulationElement_ABC& element )
        {
            auto collidingPopulationFlows = element.GetCollidingPopulationFlows();
            unsigned int perceiverId = perceiver_.GetID();
            for( auto it = collidingPopulationFlows.begin(); it != collidingPopulationFlows.end(); ++it )
            {
                MIL_PopulationFlow* flow = static_cast< MIL_PopulationFlow* >( *it );
                if( flow->GetPopulation().GetID() == perceiverId )
                    continue;
                DEC_Knowledge_Population& knownPopulation = group_.GetPopulationKnowledgeToUpdate( flow->GetPopulation() );
                DEC_Knowledge_PopulationCollision collision( flow->GetPopulation() );
                collision.Update( *flow );
                knownPopulation.Update( collision, *flow );
            }
            auto collidingPopulationConcentrations = element.GetCollidingPopulationConcentrations();
            for( auto it = collidingPopulationConcentrations.begin(); it != collidingPopulationConcentrations.end(); ++it )
            {
                MIL_PopulationConcentration* concentration = static_cast< MIL_PopulationConcentration* >( *it );
                if( concentration->GetPopulation().GetID() == perceiverId )
                    continue;
                DEC_Knowledge_Population& knownPopulation = group_.GetPopulationKnowledgeToUpdate( concentration->GetPopulation() );
                DEC_Knowledge_PopulationCollision collision( concentration->GetPopulation() );
                collision.Update( *concentration );
                knownPopulation.Update( collision, *concentration );
            }
        }
    private:
        MIL_KnowledgeGroup& group_;
        const MIL_Population& perceiver_;
    };
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::ApplyOnKnowledgesPopulationPerception
// Created: FDS 2010-04-08
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::ApplyOnKnowledgesPopulationPerception( int currentTimeStep )
{
    if( ! IsJammed() )
    {
        const T_AutomateVector& automates = GetAutomates();
        for( auto itAutomate = automates.begin(); itAutomate != automates.end(); ++itAutomate )
        {
            const MIL_Automate::T_PionVector& pions = (**itAutomate).GetPions();
            for( auto itPion = pions.begin(); itPion != pions.end(); ++itPion )
            {
                MIL_AgentPion& pion = **itPion;
                if( pion.CallRole( &PHY_RoleInterface_Communications::CanReceive, false ) )
                {
                    auto functorPopulationPerception = boost::bind( & MIL_KnowledgeGroup::UpdatePopulationKnowledgeFromPerception, this, _1, boost::ref(currentTimeStep) );
                    pion.GetKnowledge().GetKnowledgePopulationPerceptionContainer().ApplyOnKnowledgesPopulationPerception( functorPopulationPerception );
                    auto functorPopulationCollision = boost::bind( & MIL_KnowledgeGroup::UpdatePopulationKnowledgeFromCollision, this, _1, boost::ref(currentTimeStep) );
                    pion.GetKnowledge().GetKnowledgePopulationCollisionContainer ().ApplyOnKnowledgesPopulationCollision ( functorPopulationCollision );
                }
            }
        }

        // acquisition des connaissances des groupes fils /!\ Transfert de connaissance appui
        for( auto itKG = GetKnowledgeGroups().begin(); itKG != GetKnowledgeGroups().end(); ++itKG )
        {
            const MIL_KnowledgeGroup& innerKg = **itKG;
            if( innerKg.IsEnabled() && IsEnabled() && innerKg.IsJammed() && innerKg.CanReport() && innerKg.GetKnowledge() )
            {
                std::function< void( DEC_Knowledge_PopulationPerception& ) > functorPopulationPerception = boost::bind( & MIL_KnowledgeGroup::UpdatePopulationKnowledgeFromPerception, this, _1, boost::ref(currentTimeStep) );
                innerKg.ApplyOnKnowledgesPopulationPerception( functorPopulationPerception );
                std::function< void( DEC_Knowledge_PopulationCollision& ) > functorPopulationCollision = boost::bind( & MIL_KnowledgeGroup::UpdatePopulationKnowledgeFromCollision, this, _1, boost::ref(currentTimeStep) );
                innerKg.ApplyOnKnowledgesPopulationCollision( functorPopulationCollision );
            }
        }
    }
    else if( jammedPion_ )
    {
        DEC_KnowledgeBlackBoard_AgentPion& knowledgeBlackboard = jammedPion_->GetKnowledge();
        auto functorPopulationPerception = boost::bind( & MIL_KnowledgeGroup::UpdatePopulationKnowledgeFromPerception, this, _1, boost::ref(currentTimeStep)  );
        knowledgeBlackboard.GetKnowledgePopulationPerceptionContainer().ApplyOnKnowledgesPopulationPerception( functorPopulationPerception );
        auto functorPopulationCollision = boost::bind( & MIL_KnowledgeGroup::UpdatePopulationKnowledgeFromCollision, this, _1, boost::ref(currentTimeStep)  );
        knowledgeBlackboard.GetKnowledgePopulationCollisionContainer ().ApplyOnKnowledgesPopulationCollision ( functorPopulationCollision  );
    }
    // Crowds
    for( auto it = populations_.begin(); it != populations_.end(); ++it )
    {
        sPopulationsCollidingPopulationVisitor visitor( *this, **it );
        ( *it )->Apply( visitor );
    }
    
    // Acquisition des connaissances parents /!\ Transfert de connaissance appui
    if( IsJammed() && IsEnabled() && CanReport( ) && parent_ && parent_->GetKnowledge() )
    {
        std::function< void( DEC_Knowledge_Population& ) > functorObject = boost::bind( &MIL_KnowledgeGroup::UpdatePopulationKnowledgeFromParentKnowledgeGroup, this, _1, boost::ref(currentTimeStep) );
        parent_->GetKnowledge()->GetKnowledgePopulationContainer().ApplyOnKnowledgesPopulation( functorObject );
    }

    // Mise à jour des groupes de connaissance avec les pions partageant les mêmes perceptions
    for( auto it = sharingPercetionsGroup_.begin(); it != sharingPercetionsGroup_.end(); ++it )
        ApplyPopulationPerception( **it, currentTimeStep );

    // Mise à jour des groupes de connaissance avec les pions partageant les mêmes connaissances
    for( auto it = sharingKnowledgesGroup_.begin(); it != sharingKnowledgesGroup_.end(); ++it )
    {
        std::function< void( DEC_Knowledge_Population& ) > functorPopulation = boost::bind( & MIL_KnowledgeGroup::UpdatePopulationKnowledgeFromAgent, this, _1, boost::ref( currentTimeStep ) );
        (*it)->GetKnowledgeGroup()->knowledgeBlackBoard_->GetKnowledgePopulationContainer().ApplyOnKnowledgesPopulation( functorPopulation );
    }
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
        for( auto itAutomate = GetAutomates().begin(); itAutomate != GetAutomates().end(); ++itAutomate )
        {
            const MIL_Automate::T_PionVector& pions = (**itAutomate).GetPions();
            for( auto itPion = pions.begin(); itPion != pions.end(); ++itPion )
            {
                MIL_AgentPion& pion = **itPion;
                 // Les perceptions des subordonnées sont envoyées uniquement dans le cas ou celui ci peut communiquer.
                if( pion.CallRole( &PHY_RoleInterface_Communications::CanEmit, false ) )
                {
                    std::function< void( DEC_Knowledge_AgentPerception& ) > functorAgent = boost::bind( & MIL_KnowledgeGroup::UpdateAgentKnowledgeFromAgentPerception, this, _1, boost::ref(currentTimeStep) );
                    pion.GetKnowledge().GetKnowledgeAgentPerceptionContainer().ApplyOnKnowledgesAgentPerception( functorAgent );
                }
            }
        }

        // acquisition des connaissances des groupes fils
        for( auto it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it )
        {
            const MIL_KnowledgeGroup& innerKg = **it;
            if( innerKg.IsEnabled() && IsEnabled() && !innerKg.IsJammed() && innerKg.GetKnowledge() )
            {
                std::function< void( DEC_Knowledge_Agent& ) > functorAgent = boost::bind( &MIL_KnowledgeGroup::UpdateAgentKnowledgeFromParentKnowledgeGroup, this, _1, boost::ref(currentTimeStep) );
                innerKg.GetKnowledge()->GetKnowledgeAgentContainer().ApplyOnKnowledgesAgent( functorAgent );
            }
        }
        for( auto itKG = GetKnowledgeGroups().begin(); itKG != GetKnowledgeGroups().end(); ++itKG )
        {
            const MIL_KnowledgeGroup& innerKg = **itKG;
            if( innerKg.IsEnabled() && IsEnabled() && innerKg.IsJammed() && innerKg.CanReport() && innerKg.GetKnowledge() )
            {
                std::function< void( DEC_Knowledge_AgentPerception& ) > functorAgent = boost::bind( & MIL_KnowledgeGroup::UpdateAgentKnowledgeFromAgentPerception, this, _1, boost::ref(currentTimeStep) );
                innerKg.ApplyOnKnowledgesAgentPerception( functorAgent );
            }
        }
    }
    // Crowds

    for( auto it = populations_.begin(); it != populations_.end(); ++it )
    {
        sCollidingPopulationVisitor< MIL_Agent_ABC > visitor1( boost::bind( &MIL_KnowledgeGroup::UpdateAgentKnowledgeFromCrowdPerception, this, _1, currentTimeStep ), &MIL_PopulationElement_ABC::GetCollidingAgents );
        ( *it )->Apply( visitor1 );
    }

    const PHY_RolePion_Communications* communications = jammedPion_ ? jammedPion_->RetrieveRole< PHY_RolePion_Communications >() : 0;
    if( !IsJammed() || ( communications && communications->CanReceive() ) )
    {
        // LTO begin
        //mis à jour des groupes de connaissances depuis leur parent avec un délai
        if( timeToDiffuse_ < currentTimeStep )
        {
            if( parent_ && IsEnabled() )
            {
                auto bbKg = parent_->GetKnowledge();
                if( bbKg )
                {
                    std::function< void( DEC_Knowledge_Agent& ) > functorAgent = boost::bind( &MIL_KnowledgeGroup::UpdateAgentKnowledgeFromParentKnowledgeGroup, this, _1, boost::ref(currentTimeStep) );
                    bbKg->GetKnowledgeAgentContainer().ApplyOnPreviousKnowledgesAgent( functorAgent );
                    bbKg->GetKnowledgeAgentContainer().SaveAllCurrentKnowledgeAgent();
                }
            }
            bDiffuseToKnowledgeGroup_ = true;
        }
        // LTO end
    }

    // Mise à jour pour groupe de connaissance non restreint (silence partiel ou brouillage)
    if( IsJammed() && jammedPion_ )
        ApplyAgentPerception( *jammedPion_, currentTimeStep );

    // Mise à jour des groupes de connaissance avec les pions partageant les mêmes perceptions
    for( auto it = sharingPercetionsGroup_.begin(); it != sharingPercetionsGroup_.end(); ++it )
        ApplyAgentPerception( **it, currentTimeStep );

    // Mise à jour des groupes de connaissance avec les pions partageant les mêmes connaissances
    for( auto it = sharingKnowledgesGroup_.begin(); it != sharingKnowledgesGroup_.end(); ++it )
    {
        std::function< void( DEC_Knowledge_Agent& ) > functorAgent = boost::bind( & MIL_KnowledgeGroup::UpdateAgentKnowledgeFromAgent, this, _1, boost::ref( currentTimeStep ) );
        (*it)->GetKnowledgeGroup()->knowledgeBlackBoard_->GetKnowledgeAgentContainer().ApplyOnKnowledgesAgent( functorAgent );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::UpdateObjectPerception
// Created: LGY 2013-08-26
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::UpdateObjectPerception( const MIL_KnowledgeGroup& group, int currentTimeStep )
{
    std::function< void( DEC_Knowledge_ObjectPerception& ) > functorPerception = boost::bind( & MIL_KnowledgeGroup::UpdateObjectKnowledgeFromPerception, this, _1, boost::ref(currentTimeStep) );
    group.ApplyOnKnowledgesObjectPerception( functorPerception );
    std::function< void( DEC_Knowledge_ObjectCollision& ) > functorCollision = boost::bind( & MIL_KnowledgeGroup::UpdateObjectKnowledgeFromCollision, this, _1, boost::ref(currentTimeStep) );
    group.ApplyOnKnowledgesObjectCollision( functorCollision );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::ApplyOnKnowledgesObjectPerception
// Created: MMC 2013-07-03
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::ApplyOnKnowledgesObjectPerception( int currentTimeStep )
{
    // Crowds
    for( auto it = populations_.begin(); it != populations_.end(); ++it )
    {
        sCollidingPopulationVisitor< MIL_Object_ABC > visitor( boost::bind( &MIL_KnowledgeGroup::UpdateObjectKnowledgeFromCrowdPerception, this, _1 ), &MIL_PopulationElement_ABC::GetCollidingObjects );
        auto& population = *it;
        population->Apply( visitor );
        population->ClearObjectCollisions();
    }

    const PHY_RolePion_Communications* communications = jammedPion_ ? jammedPion_->RetrieveRole< PHY_RolePion_Communications >() : 0;
    if( !IsJammed() || ( communications && communications->CanReceive() ) )
    {
        if( timeToDiffuse_ < currentTimeStep )
        {
            if( parent_ && IsEnabled() && parent_->GetKnowledge() )
            {
                std::function< void( DEC_Knowledge_Object& ) > functorObject = boost::bind( &MIL_KnowledgeGroup::UpdateObjectKnowledgeFromParentKnowledgeGroup, this, _1, boost::ref(currentTimeStep) );
                parent_->GetKnowledge()->GetKnowledgeObjectContainer().ApplyOnPreviousKnowledgesObject( functorObject );
                parent_->GetKnowledge()->GetKnowledgeObjectContainer().SaveAllCurrentKnowledgeObject();
            }
            bDiffuseToKnowledgeGroup_ = true;
        }

        // acquisition des connaissances des groupes fils /!\ Transfert de connaissance appui
        for( auto itKG = GetKnowledgeGroups().begin(); itKG != GetKnowledgeGroups().end(); ++itKG )
        {
            const MIL_KnowledgeGroup& innerKg = **itKG;
            if( innerKg.IsEnabled() && IsEnabled() && innerKg.IsJammed() && innerKg.CanReport() && innerKg.GetKnowledge() )
                UpdateObjectPerception( innerKg, currentTimeStep );
        }
    }

    // Acquisition des connaissances parents /!\ Transfert de connaissance appui
    if( IsJammed() && IsEnabled() && CanReport( ) && parent_ &&  parent_->GetKnowledge() )
    {
        std::function< void( DEC_Knowledge_Object& ) > functorObject = boost::bind( &MIL_KnowledgeGroup::UpdateObjectKnowledgeFromParentKnowledgeGroup, this, _1, boost::ref(currentTimeStep) );
        parent_->GetKnowledge()->GetKnowledgeObjectContainer().ApplyOnPreviousKnowledgesObject( functorObject );
        parent_->GetKnowledge()->GetKnowledgeObjectContainer().SaveAllCurrentKnowledgeObject();
    }

    // Mise à jour des groupes de connaissance avec les pions partageant les mêmes perceptions
    for( auto it = sharingPercetionsGroup_.begin(); it != sharingPercetionsGroup_.end(); ++it )
        UpdateObjectPerception( *(*it)->GetKnowledgeGroup(), currentTimeStep );

    // Mise à jour des groupes de connaissance avec les pions partageant les mêmes connaissances
    for( auto it = sharingKnowledgesGroup_.begin(); it != sharingKnowledgesGroup_.end(); ++it )
    {
        std::function< void( boost::shared_ptr< DEC_Knowledge_Object >) > functorObject = boost::bind( & MIL_KnowledgeGroup::UpdateObjectKnowledgeFromAgent, this, _1, boost::ref( currentTimeStep ) );
        (*it)->GetKnowledgeGroup()->knowledgeBlackBoard_->GetKnowledgeObjectContainer().ApplyOnKnowledgesObject( functorObject );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::GetPopulationKnowledgeToUpdate
// Created: FDS 2010-04-08
// -----------------------------------------------------------------------------
DEC_Knowledge_Population& MIL_KnowledgeGroup::GetPopulationKnowledgeToUpdate( MIL_Population& populationKnown )
{
    if( knowledgeBlackBoard_ )
    {
        boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = knowledgeBlackBoard_->GetKnowledgePopulationContainer().GetKnowledgePopulation( populationKnown );
        if( pKnowledge )
            return *pKnowledge;
    }
    return CreateKnowledgePopulation( populationKnown );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::UpdateAgentKnowledgeFromCrowdPerception
// Created: JSR 2013-07-10
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::UpdateAgentKnowledgeFromCrowdPerception( MIL_Agent_ABC& agent, int currentTimeStep )
{
    if( agent.IsMarkedForDestruction() )
        return;
    DEC_Knowledge_Agent& knowledgeAgent = GetAgentKnowledgeToUpdate( agent );
    knowledgeAgent.UpdateFromCrowdPerception( currentTimeStep );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::UpdateConcentrationKnowledgeFromCrowdPerception
// Created: JSR 2013-07-11
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::UpdateConcentrationKnowledgeFromCrowdPerception( TER_PopulationConcentration_ABC& c, int currentTimeStep )
{
    const MIL_PopulationConcentration& concentration = static_cast< const MIL_PopulationConcentration& >( c );
    if( std::find( populations_.begin(), populations_.end(), &concentration.GetPopulation() ) == populations_.end() )
    {
        DEC_Knowledge_Population& knowledgePopulation = GetPopulationKnowledgeToUpdate( concentration.GetPopulation() );
        knowledgePopulation.UpdateFromCrowdPerception( concentration, currentTimeStep );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::UpdateFlowKnowledgeFromCrowdPerception
// Created: JSR 2013-07-11
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::UpdateFlowKnowledgeFromCrowdPerception( TER_PopulationFlow_ABC& f, int currentTimeStep )
{
    const MIL_PopulationFlow& flow = static_cast< const MIL_PopulationFlow& >( f );
    if( std::find( populations_.begin(), populations_.end(), &flow.GetPopulation() ) == populations_.end() )
    {
        DEC_Knowledge_Population& knowledgePopulation = GetPopulationKnowledgeToUpdate( flow.GetPopulation() );
        knowledgePopulation.UpdateFromCrowdPerception( flow, currentTimeStep );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::UpdateObjectKnowledgeFromCrowdPerception
// Created: JSR 2013-07-10
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::UpdateObjectKnowledgeFromCrowdPerception( MIL_Object_ABC& object )
{
    if( object.IsMarkedForDestruction() )
        return;
    boost::shared_ptr< DEC_Knowledge_Object > knowledgeObject = GetObjectKnowledgeToUpdate( object );
    if( knowledgeObject )
        knowledgeObject->UpdateFromCrowdPerception();
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

// // -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::UpdateObjectKnowledgeFromCollision
// Created: LGY 2013-08-06
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::UpdateObjectKnowledgeFromCollision( const DEC_Knowledge_ObjectCollision& collision, int /*currentTimeStep*/ )
{
    if( !collision.GetObject().IsMarkedForDestruction() )
        GetObjectKnowledgeToUpdate( collision.GetObject() )->Update( collision );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::UpdateObjectKnowledgeFromPerception
// Created: LGY 2013-08-06
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::UpdateObjectKnowledgeFromPerception( const DEC_Knowledge_ObjectPerception& perception, int /*currentTimeStep*/ )
{
    if( !perception.GetObjectPerceived().IsMarkedForDestruction() )
        GetObjectKnowledgeToUpdate( perception.GetObjectPerceived() )->Update( perception );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::UpdateAgentKnowledgeFromAgent
// Created: LGY 2013-09-09
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::UpdateAgentKnowledgeFromAgent( const DEC_Knowledge_Agent& agentKnowledge, int currentTimeStep )
{
    GetAgentKnowledgeToUpdate( agentKnowledge.GetAgentKnown() ).Update( agentKnowledge, currentTimeStep );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::UpdatePopulationKnowledgeFromParentKnowledgeGroup
// Created: LGY 2013-08-06
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::UpdatePopulationKnowledgeFromParentKnowledgeGroup( const DEC_Knowledge_Population& pKnowledge, int currentTimeStep )
{
    if( !parent_ || parent_->GetType().GetKnowledgeCommunicationDelay() <= currentTimeStep )
        GetPopulationKnowledgeToUpdate( pKnowledge.GetPopulationKnown() ).Update( pKnowledge, currentTimeStep );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::UpdatePopulationKnowledgeFromAgent
// Created: LGY 2013-09-09
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::UpdatePopulationKnowledgeFromAgent( const DEC_Knowledge_Population& knowledge, int currentTimeStep )
{
    GetPopulationKnowledgeToUpdate( knowledge.GetPopulationKnown() ).Update( knowledge, currentTimeStep );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::UpdateAgentKnowledgeFromAgentPerception
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::UpdateAgentKnowledgeFromAgentPerception( const DEC_Knowledge_AgentPerception& perception, int currentTimeStep )
{
    if( perception.IsAvailable() )
    {
        const MIL_Agent_ABC& agent = perception.GetAgentPerceived();
        if( agent.IsMarkedForDestruction() )
            return;
        GetAgentKnowledgeToUpdate( agent ).Update( perception, currentTimeStep );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::UpdateAgentKnowledgeFromParentKnowledgeGroup
// Created: SLG 2009-11-26
// LTO
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::UpdateAgentKnowledgeFromParentKnowledgeGroup( const DEC_Knowledge_Agent& agentKnowledge, int currentTimeStep )
{
    if( agentKnowledge.IsValid() && ( !parent_ || parent_->GetType().GetKnowledgeCommunicationDelay() <= currentTimeStep ) )
    {
        const MIL_Agent_ABC& agent = agentKnowledge.GetAgentKnown();
        if( agent.IsMarkedForDestruction() )
            return;
        GetAgentKnowledgeToUpdate( agent ).Update( agentKnowledge, currentTimeStep );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::UpdateObjectKnowledgeFromParentKnowledgeGroup
// Created: MMC 2013-07-03
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::UpdateObjectKnowledgeFromParentKnowledgeGroup( const DEC_Knowledge_Object& objectKnowledge, int currentTimeStep )
{
    if( objectKnowledge.IsValid() && ( !parent_ || parent_->GetType().GetKnowledgeCommunicationDelay() <= currentTimeStep ) )
    {
        MIL_Object_ABC* object = MIL_AgentServer::GetWorkspace().GetEntityManager().FindObject( objectKnowledge.GetObjectId() );
        if( object && !object->IsMarkedForDestruction() )
        {
            boost::shared_ptr< DEC_Knowledge_Object > pKnowledgeObject = GetObjectKnowledgeToUpdate( *object );
            if( pKnowledgeObject.get() )
                pKnowledgeObject->Update( objectKnowledge, currentTimeStep );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::UpdateObjectKnowledgeFromAgent
// Created: LGY 2013-07-18
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::UpdateObjectKnowledgeFromAgent( boost::shared_ptr< DEC_Knowledge_Object >& objectKnowledge, int currentTimeStep )
{
    MIL_Object_ABC* object = MIL_AgentServer::GetWorkspace().GetEntityManager().FindObject( objectKnowledge->GetObjectId() );
    if( object && !object->IsMarkedForDestruction() )
    {
        boost::shared_ptr< DEC_Knowledge_Object > pKnowledgeObject = GetObjectKnowledgeToUpdate( *object );
        if( pKnowledgeObject.get() )
            pKnowledgeObject->Update( *objectKnowledge, currentTimeStep );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::GetAgentKnowledgeToUpdate
// Created: NLD 2004-03-19
// -----------------------------------------------------------------------------
DEC_Knowledge_Agent& MIL_KnowledgeGroup::GetAgentKnowledgeToUpdate( const MIL_Agent_ABC& agentKnown )
{
    if( knowledgeBlackBoard_ )
    {
        boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge = knowledgeBlackBoard_->GetKnowledgeAgentContainer().GetKnowledgeAgent( agentKnown );
        if( pKnowledge )
            return *pKnowledge;
    }
    return CreateKnowledgeAgent( agentKnown );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::CreateKnowledgeAgent
// Created: FDS 2010-04-12
// -----------------------------------------------------------------------------
DEC_Knowledge_Agent& MIL_KnowledgeGroup::CreateKnowledgeAgent( const MIL_Agent_ABC& perceived )
{
    return knowledgeBlackBoard_->CreateKnowledgeAgent( *this, const_cast< MIL_Agent_ABC& >( perceived ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::CreateKnowledgePopulation
// Created: FDS 2010-04-12
// -----------------------------------------------------------------------------
DEC_Knowledge_Population& MIL_KnowledgeGroup::CreateKnowledgePopulation( MIL_Population& perceived )
{
    return knowledgeBlackBoard_->CreateKnowledgePopulation( *this, perceived );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::UpdateKnowledgeFromTransported
// Created: LDC 2012-06-12
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::UpdateKnowledgeFromTransported( const MIL_Agent_ABC& perceived )
{
    if( perceived.IsMarkedForDestruction() )
        return;
    DEC_Knowledge_Agent& agent = GetAgentKnowledgeToUpdate( perceived );
    agent.Extrapolate();
    agent.UpdateOnNetwork();
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::GetJammedPion
// Created: LDC 2014-02-18
// -----------------------------------------------------------------------------
const MIL_Agent_ABC* MIL_KnowledgeGroup::GetJammedPion() const
{
    return jammedPion_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::GetObjectKnowledgeToUpdate
// Created: MMC 2011-06-15
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > MIL_KnowledgeGroup::GetObjectKnowledgeToUpdate( MIL_Object_ABC& objectKnown )
{
    if( DEC_BlackBoard_CanContainKnowledgeObject*  knowledgeObjectContainer = GetKnowledgeObjectContainer() )
    {
        boost::shared_ptr< DEC_Knowledge_Object > knowledgeObject = knowledgeObjectContainer->GetKnowledgeObject( objectKnown );
        if( knowledgeObject )
            return knowledgeObject;
        return knowledgeObjectContainer->CreateKnowledgeObject( objectKnown );
    }
    return boost::shared_ptr< DEC_Knowledge_Object >();
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::GetKnowledgeObjectContainer
// Created: LDC 2011-08-24
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgeObject* MIL_KnowledgeGroup::GetKnowledgeObjectContainer() const
{
    if( knowledgeBlackBoard_ )
        return &knowledgeBlackBoard_->GetKnowledgeObjectContainer();
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::Accept
// Created: LGY 2011-08-29
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::Accept( KnowledgesVisitor_ABC& visitor ) const
{
    visitor.VisitKnowledgesGroup( knowledgeGroups_.size() );
    for( auto it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it )
        (*it)->Accept( visitor );
    if( knowledgeBlackBoard_ )
        knowledgeBlackBoard_->Accept( visitor );
}

namespace
{
    class PopulationElementPerceptionVisitor : public MIL_EntityVisitor_ABC< MIL_PopulationElement_ABC >
    {
    public:
        PopulationElementPerceptionVisitor( TER_PopulationConcentration_ABC::T_ConstPopulationConcentrationVector& perceivablePopulationDensity, TER_PopulationFlow_ABC::T_ConstPopulationFlowVector& perceivablePopulationFlow )
          : concentrations_( perceivablePopulationDensity )
          , flows_         ( perceivablePopulationFlow )
        {
        }

        virtual void Visit( const MIL_PopulationElement_ABC& element )
        {
            const TER_PopulationConcentration_ABC* concentration = dynamic_cast< const TER_PopulationConcentration_ABC* >( &element );
            if( concentration )
                concentrations_.push_back( concentration );
            else
            {
                const TER_PopulationFlow_ABC* flow = dynamic_cast< const TER_PopulationFlow_ABC* >( &element );
                if( flow )
                    flows_.push_back( flow );
            }
        }

    private:
        TER_PopulationConcentration_ABC::T_ConstPopulationConcentrationVector& concentrations_;
        TER_PopulationFlow_ABC::T_ConstPopulationFlowVector& flows_;
    };
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::AppendAddedKnowledge
// Created: LDC 2012-01-02
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::AppendAddedKnowledge( TER_Agent_ABC::T_AgentPtrVector& perceivableAgents, TER_Object_ABC::T_ObjectVector& perceivableObjects, TER_PopulationConcentration_ABC::T_ConstPopulationConcentrationVector& perceivablePopulationDensity, TER_PopulationFlow_ABC::T_ConstPopulationFlowVector& perceivablePopulationFlow ) const
{
    PopulationElementPerceptionVisitor populationElementVisitor( perceivablePopulationDensity, perceivablePopulationFlow ); // $$$$ RC LDC Population concentration and flow should be merged - need refactor TER_Populationconcentration_ABC etc.
    for( auto it = additionalPerceptions_.begin(); it != additionalPerceptions_.end(); ++it )
    {
        unsigned int identifier = *it;
        MIL_AgentPion* pAgent = MIL_AgentServer::GetWorkspace().GetEntityManager().FindAgentPion( identifier );
        if( pAgent )
            perceivableAgents.push_back( pAgent->Retrieve< PHY_RoleInterface_Location >() );
        else
        {
            MIL_Object_ABC* pObject = MIL_AgentServer::GetWorkspace().GetEntityManager().FindObject( identifier );
            if( pObject )
                perceivableObjects.push_back( pObject );
            else
            {
                MIL_Population* pPopulation = MIL_AgentServer::GetWorkspace().GetEntityManager().FindPopulation( identifier );
                if( pPopulation )
                    pPopulation->Apply( populationElementVisitor );
            }
        }
    }
}

namespace
{
    void AddElement( std::vector< const MIL_Agent_ABC* >& group, const MIL_Agent_ABC& agent )
    {
        auto it = std::find( group.begin(), group.end(), &agent );
        if( it == group.end() )
            group.push_back( &agent );
    }
    void RemoveElement( std::vector< const MIL_Agent_ABC* >& group, const MIL_Agent_ABC& agent )
    {
        group.erase( std::remove( group.begin(), group.end(), &agent ), group.end() );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::RegisterSharingPerceptions
// Created: LGY 2013-05-07
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::RegisterSharingPerceptions( const MIL_Agent_ABC& agent )
{
    AddElement( sharingPercetionsGroup_, agent );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::UnregisterSharingPerceptions
// Created: LGY 2013-05-07
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::UnregisterSharingPerceptions( const MIL_Agent_ABC& agent )
{
    RemoveElement( sharingPercetionsGroup_, agent );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::RegisterSharingKnowledges
// Created: LGY 2013-09-09
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::RegisterSharingKnowledges( const MIL_Agent_ABC& agent )
{
    AddElement( sharingKnowledgesGroup_, agent );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::UnregisterSharingKnowledges
// Created: LGY 2013-09-09
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::UnregisterSharingKnowledges( const MIL_Agent_ABC& agent )
{
    RemoveElement( sharingKnowledgesGroup_, agent );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::CanReport
// Created: LGY 2013-07-03
// -----------------------------------------------------------------------------
bool MIL_KnowledgeGroup::CanReport() const
{
    if( !jammedPion_ )
        return false;
    if( const PHY_RolePion_Communications* pCommunication = jammedPion_->RetrieveRole< PHY_RolePion_Communications >() )
        return pCommunication->CanReport();
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::OnReceiveRename
// Created: ABR 2014-08-27
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::OnReceiveRename( const sword::MissionParameters& parameters )
{
    protocol::CheckCount( parameters, 1 );
    const auto& name = protocol::GetString( parameters, 0 );
    name_ = name;
    client::KnowledgeGroupUpdate asn;
    asn().mutable_knowledge_group()->set_id( id_ );
    asn().set_name( name.c_str() );
    asn.Send( NET_Publisher_ABC::Publisher() );
}
