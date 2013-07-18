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
#include "KnowledgeGroupFactory_ABC.h" // LTO
#include "DEC_BlackBoard_CanContainKnowledgeObject.h"
#include "Checkpoints/SerializationTools.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Communications/PHY_RolePion_Communications.h" // LTO
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/MIL_Army.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/MIL_EntityVisitor_ABC.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Populations/MIL_PopulationElement_ABC.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgeAgent.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgeAgentPerception.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgePopulation.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgePopulationCollision.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgePopulationPerception.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_Knowledge_AgentPerception.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/DEC_Knowledge_Population.h"
#include "Knowledge/DEC_Knowledge_PopulationCollision.h"
#include "Knowledge/DEC_Knowledge_PopulationPerception.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "Network/NET_AsnException.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "simulation_terrain/TER_PopulationConcentration_ABC.h"
#include "simulation_terrain/TER_PopulationFlow_ABC.h"
#include "Tools/MIL_IDManager.h"
#include "MT_Tools/MT_ScipioException.h"
#include "MT_Tools/MT_FormatString.h"
#include <xeumeuleu/xml.hpp>
#include <boost/serialization/split_free.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/bind.hpp>


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
    , parent_             ( 0 )
    , knowledgeBlackBoard_( new DEC_KnowledgeBlackBoard_KnowledgeGroup( this ) )
    , timeToDiffuse_      ( 0 ) // LTO
    , isActivated_        ( true ) // LTO
    , hasBeenUpdated_     ( false )
    , isJammed_           ( false )
    , createdByJamming_   ( false )
    , jammedPion_         ( 0 )
    , bDiffuseToKnowledgeGroup_( false )
    , isCrowd_            ( false )
{
    idManager_.Lock( id_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup constructor
// Created: JSR 2013-07-04
// For crowds
// -----------------------------------------------------------------------------
MIL_KnowledgeGroup::MIL_KnowledgeGroup( MIL_Army_ABC& army )
    : id_                 ( idManager_.GetFreeId() )
    , type_               ( MIL_KnowledgeGroupType::FindTypeOrAny( "Standard" ) )
    , name_               ( "Foule" ) // translation?
    , army_               ( &army )
    , parent_             ( 0 )
    , knowledgeBlackBoard_( new DEC_KnowledgeBlackBoard_KnowledgeGroup( this ) )
    , timeToDiffuse_      ( 0 ) // LTO
    , isActivated_        ( true ) // LTO
    , hasBeenUpdated_     ( true )
    , isJammed_           ( false )
    , createdByJamming_   ( false )
    , jammedPion_         ( 0 )
    , isCrowd_            ( true )
{
    if( ! type_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Knowledge group '%d' cannot be created because there are no knowledge group types defined: %s ", id_ ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup constructor
// Created: SLG 2009-11-11
// LTO
// -----------------------------------------------------------------------------
MIL_KnowledgeGroup::MIL_KnowledgeGroup( xml::xistream& xis, MIL_Army_ABC& army, boost::shared_ptr< MIL_KnowledgeGroup >& parent, KnowledgeGroupFactory_ABC& knowledgeGroupFactory )
    : id_                 ( xis.attribute< unsigned int >( "id" ) )
    , type_               ( MIL_KnowledgeGroupType::FindType( xis.attribute< std::string >( "type" ) ) )
    , name_               ( xis.attribute< std::string >( "name" ) )
    , army_               ( &army )
    , parent_             ( parent.get() )
    , knowledgeBlackBoard_( new DEC_KnowledgeBlackBoard_KnowledgeGroup( this ) )
    , timeToDiffuse_      ( 0 ) // LTO
    , isActivated_        ( true ) // LTO
    , hasBeenUpdated_     ( true )
    , isJammed_           ( false )
    , createdByJamming_   ( false )
    , jammedPion_         ( 0 )
    , bDiffuseToKnowledgeGroup_( false )
    , isCrowd_            ( xis.has_attribute( "crowd" ) && xis.attribute< bool >( "crowd" ) == true )
{
    if( ! type_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Knowledge group '%d' cannot be created because its type does not exist: %s ", id_, xis.attribute< std::string >( "type" ).c_str() ) );
    idManager_.Lock( id_ );
    if( parent_ )
        timeToDiffuse_ = parent_->GetType().GetKnowledgeCommunicationDelay();
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
    , parent_             ( 0 )
    , knowledgeBlackBoard_( 0 )
    , timeToDiffuse_      ( 0 ) // LTO
    , isActivated_        ( true ) // LTO
    , hasBeenUpdated_     ( false )
    , isJammed_           ( false )
    , createdByJamming_   ( false )
    , jammedPion_         ( 0 )
    , bDiffuseToKnowledgeGroup_( false )
    , isCrowd_            ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupFactory::Create
// Created: FDS 2010-03-17
// -----------------------------------------------------------------------------
MIL_KnowledgeGroup::MIL_KnowledgeGroup( const MIL_KnowledgeGroup& source, const MIL_Agent_ABC& pion, boost::shared_ptr< MIL_KnowledgeGroup >& parent )
    : type_               ( source.type_ )
    , id_                 ( idManager_.GetFreeId() )
    , name_               ( source.name_ + " (" + pion.GetName() + ")" )
    , army_               ( source.army_ )
    , parent_             ( parent.get() )
    , knowledgeBlackBoard_( new DEC_KnowledgeBlackBoard_KnowledgeGroup( this ) )
    , timeToDiffuse_      ( 0 ) // LTO
    , isActivated_        ( true ) // LTO
    , hasBeenUpdated_     ( true )
    , isJammed_           ( true )
    , createdByJamming_   ( true )
    , jammedPion_         ( &pion )
    , bDiffuseToKnowledgeGroup_( false )
    , isCrowd_            ( false )
{
    if( parent_ )
        timeToDiffuse_ = parent_->GetType().GetKnowledgeCommunicationDelay();
    SendCreation();
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::Clone
// Created: LDC 2012-08-30
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::Clone( const MIL_KnowledgeGroup& source )
{
    boost::function< void( DEC_Knowledge_Agent& ) > functorAgent = boost::bind( &MIL_KnowledgeGroup::CreateKnowledgeFromAgentPerception, this, _1 );
    source.ApplyOnKnowledgesAgent( functorAgent );
    boost::function< void( DEC_Knowledge_Population& ) > functorPopulation = boost::bind( &MIL_KnowledgeGroup::CreateKnowledgeFromPopulationPerception, this, _1 );
    source.ApplyOnKnowledgesPopulation( functorPopulation );
    boost::function< void( boost::shared_ptr< DEC_Knowledge_Object >& ) > functorObject = boost::bind( &MIL_KnowledgeGroup::CreateKnowledgeFromObjectPerception, this, _1 );
    source.ApplyOnKnowledgesObject( functorObject );
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
// Name: MIL_KnowledgeGroup::CreateKnowledgeFromObjectPerception
// Created: JSR 2013-06-19
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::CreateKnowledgeFromObjectPerception( boost::shared_ptr< DEC_Knowledge_Object >& object )
{
    if( object && object->IsValid() && object->GetObjectKnown() )
    {
        boost::shared_ptr< DEC_Knowledge_Object > knowledge = CreateKnowledgeObject( *object->GetObjectKnown() );
        if( knowledge )
            knowledge->CopyFrom( *object );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup destructor
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_KnowledgeGroup::~MIL_KnowledgeGroup()
{
    delete knowledgeBlackBoard_;
    try
    {
        client::KnowledgeGroupDestruction msg;
        msg().mutable_knowledge_group()->set_id( id_ );
        msg().mutable_party()->set_id( army_ ? army_->GetID() : 0 );
        msg.Send( NET_Publisher_ABC::Publisher() );
    }
    catch( std::exception& )
    {} // Never mind if no publisher registered, just don't throw.
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::Destroy
// Created: LDC 2010-04-01
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::Destroy()
{
    if( knowledgeBlackBoard_ )
    {
        boost::shared_ptr< MIL_KnowledgeGroup > shared = shared_from_this();
        if( parent_ )
            parent_->UnregisterKnowledgeGroup( shared );
        else if( army_ )
            army_->UnregisterKnowledgeGroup( shared );
        delete knowledgeBlackBoard_;
        // myself destruction
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
    knowledgeGroupFactory.Create( xis, *army_, shared_from_this() );
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
         >> isCrowd_;
    idManager_.Lock( id_ );
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
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "imposible to save knowledge group with undefined type. Knowledge group Id : '%d' ", id_ ) );
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
         << isCrowd_;
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
            << xml::attribute( "name", name_ )
            << xml::attribute( "type", type_->GetName() );
    if( isCrowd_ )
        xos << xml::attribute( "crowd", true );
    xos    << xml::end;
    for( auto it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it ) // LTO
        (**it).WriteODB( boost::ref(xos) );     // LTO
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::UpdateKnowledges
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::UpdateKnowledges( int currentTimeStep )
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
void MIL_KnowledgeGroup::UpdateObjectKnowledges( int currentTimeStep )
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
    for( auto it = automates_.begin(); it != automates_.end(); ++it )
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
        if( pKnowledge.get() )
            return pKnowledge->IsPerceptionDistanceHacked();
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::IsPerceptionDistanceHacked
// Created: MMC 2011-06-14
// -----------------------------------------------------------------------------
bool MIL_KnowledgeGroup::IsPerceptionDistanceHacked( MIL_Object_ABC& objectKnown ) const
{
    if( knowledgeBlackBoard_ )
    {
        DEC_Knowledge_Object* pKnowledge = knowledgeBlackBoard_->GetKnowledgeObjectContainer().RetrieveKnowledgeObject( objectKnown );
        if( pKnowledge )
            return pKnowledge->IsPerceptionDistanceHacked();
    }
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
        if( pKnowledge.get() )
            return pKnowledge->GetCurrentPerceptionLevel();
    }
    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::GetPerceptionLevel
// Created: MMC 2011-06-14
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& MIL_KnowledgeGroup::GetPerceptionLevel( MIL_Object_ABC& objectKnown ) const
{
    if( knowledgeBlackBoard_ )
    {
        DEC_Knowledge_Object* pKnowledge = knowledgeBlackBoard_->GetKnowledgeObjectContainer().RetrieveKnowledgeObject( objectKnown );
        if( pKnowledge )
            return pKnowledge->GetCurrentPerceptionLevel();
    }
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
        if( pKnowledge)
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
    msg().set_type(GetType().GetName());
    // LTO begin
    if( parent_ )
        msg().mutable_parent()->set_id( parent_->GetId() );
    // LTO end
    if( isJammed_ )
        msg().set_jam( true );
    msg.Send( NET_Publisher_ABC::Publisher(), context );
    //SLG : @TODO MGD Move to factory
    // LTO begin
    for( auto it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it )
        (**it).SendCreation( context );
    // LTO end
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::SendFullState
// Created: SLG 2009-11-13
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::SendFullState() const
{
    for( auto it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it ) // LTO
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
// Name: MIL_KnowledgeGroup::MoveKnowledgeGroup
// Created: NLD 2004-09-06
// LTO
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::MoveKnowledgeGroup( MIL_KnowledgeGroup *newParent )
{
    if( newParent && parent_ )
    {
        boost::shared_ptr< MIL_KnowledgeGroup > shared = shared_from_this();
        parent_->UnregisterKnowledgeGroup( shared );
        newParent->RegisterKnowledgeGroup( shared );
        hasBeenUpdated_ = true;
    }
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
    for( auto it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it ) // LTO
        (**it).SendKnowledge( context ); // LTO
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
const DEC_KnowledgeBlackBoard_KnowledgeGroup* MIL_KnowledgeGroup::GetKnowledge() const
{
    return knowledgeBlackBoard_;
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
        result = parent_->ResolveKnowledgeObject( id );
    if( !result && !isJammed_ && army_ )
        result = army_->GetKnowledge().ResolveKnowledgeObject( id );
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
        result = parent_->ResolveKnowledgeObject( object );
    if( !result && !isJammed_ && army_ )
        result = army_->GetKnowledge().ResolveKnowledgeObject( object );
    return result;
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< DEC_Knowledge_Object > MIL_KnowledgeGroup::ResolveKnowledgeObjectByObjectID
// Created: JSR 2011-09-29
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > MIL_KnowledgeGroup::ResolveKnowledgeObjectByObjectID( unsigned int id ) const
{
    if( !knowledgeBlackBoard_ )
        return boost::shared_ptr< DEC_Knowledge_Object >();
    boost::shared_ptr< DEC_Knowledge_Object > result = knowledgeBlackBoard_->ResolveKnowledgeObjectByObjectID( id );
    if( !result && parent_ )
        result = parent_->ResolveKnowledgeObjectByObjectID( id );
    if( !result && !isJammed_ && army_ )
        result = army_->GetKnowledge().ResolveKnowledgeObjectByObjectID( id );
    return result;
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
// Name: MIL_KnowledgeGroup::SetParent
// Created: FHD 2009-12-22
// LTO
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::SetParent( boost::shared_ptr< MIL_KnowledgeGroup >& parent )
{
    parent_ = parent.get();
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::RegisterKnowledgeGroup
// Created: NLD 2006-10-13
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::RegisterKnowledgeGroup( boost::shared_ptr< MIL_KnowledgeGroup >& knowledgeGroup )
{
    assert( std::find( knowledgeGroups_.begin(), knowledgeGroups_.end(), knowledgeGroup ) == knowledgeGroups_.end() );
    knowledgeGroups_.push_back( knowledgeGroup );
    for( std::set< unsigned int >::const_iterator it = additionalPerceptions_.begin(); it != additionalPerceptions_.end(); ++it )
        knowledgeGroup->additionalPerceptions_.insert( *it ); 
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::UnregisterKnowledgeGroup
// Created: FHD 2009-12-16:
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::UnregisterKnowledgeGroup( boost::shared_ptr< MIL_KnowledgeGroup >& knowledgeGroup )
{
    knowledgeGroups_.erase( std::remove( knowledgeGroups_.begin(), knowledgeGroups_.end(), knowledgeGroup ), knowledgeGroups_.end() );
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
    automates_.erase( std::remove( automates_.begin(), automates_.end(), &automate ), automates_.end() );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::RegisterPopulation
// Created: JSR 2013-07-05
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::RegisterPopulation( MIL_Population& population )
{
    assert( std::find( populations_.begin(), populations_.end(), &population ) == populations_.end() );
    populations_.push_back( &population );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::UnregisterPopulation
// Created: JSR 2013-07-05
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
    boost::shared_ptr< MIL_KnowledgeGroup > knowledgeGroup;

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
         return boost::shared_ptr< MIL_KnowledgeGroup >();
     }
     return knowledgeGroup;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::GetTimeToDiffuseToKnowledgeGroup
// Created: SLG 2009-11-30
// LTO
// -----------------------------------------------------------------------------
double MIL_KnowledgeGroup::GetTimeToDiffuseToKnowledgeGroup() const
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
// Name: MIL_KnowledgeGroup::OnReceiveKnowledgeGroupUpdate
// Created: FDS 2010-01-13
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::OnReceiveKnowledgeGroupUpdate( const sword::KnowledgeMagicAction& message, const tools::Resolver< MIL_Army_ABC >& armies  )
{
    switch( message.type() )
    {
    case sword::enable:
        hasBeenUpdated_ = OnReceiveKnowledgeGroupEnable( message.parameters() ) || hasBeenUpdated_;
        break;
    case sword::update_party:
        hasBeenUpdated_ = OnReceiveKnowledgeGroupChangeSuperior( message.parameters(), armies, false ) || hasBeenUpdated_;
        break;
    case sword::update_party_parent:
        hasBeenUpdated_ = OnReceiveKnowledgeGroupChangeSuperior( message.parameters(), armies, true ) || hasBeenUpdated_;
        break;
    case sword::update_type:
        hasBeenUpdated_ = OnReceiveKnowledgeGroupSetType( message.parameters() ) || hasBeenUpdated_;
        break;
    case sword::add_knowledge:
        hasBeenUpdated_ = OnReceiveKnowledgeGroupAddKnowledge( message.parameters() ) || hasBeenUpdated_;
        break;
    default:
        break;
    }
    UpdateKnowledgeGroup();
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::OnReceiveKnowledgeGroupEnable
// Created: SLG 2009-12-17
// -----------------------------------------------------------------------------
bool MIL_KnowledgeGroup::OnReceiveKnowledgeGroupEnable( const sword::MissionParameters& message )
{
    isActivated_ = message.elem( 0 ).value().Get( 0 ).booleanvalue();
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::OnReceiveKnowledgeGroupChangeSuperior
// Created: FHD 2009-12-17:
// -----------------------------------------------------------------------------
bool MIL_KnowledgeGroup::OnReceiveKnowledgeGroupChangeSuperior( const sword::MissionParameters& message, const tools::Resolver< MIL_Army_ABC >& armies, bool hasParent )
{
    MIL_Army_ABC* pTargetArmy = armies.Find( message.elem( 0 ).value().Get( 0 ).party().id() );
    if( !pTargetArmy || *pTargetArmy != GetArmy() )
        throw NET_AsnException< sword::KnowledgeGroupAck::ErrorCode >( sword::KnowledgeGroupAck::error_invalid_party );
    boost::shared_ptr< MIL_KnowledgeGroup > pNewParent;
    if( hasParent )
    {
        pNewParent = pTargetArmy->FindKnowledgeGroup( message.elem( 1 ).value().Get( 0 ).knowledgegroup().id() );
        if( !pNewParent || pNewParent->IsJammed() )
            throw NET_AsnException< sword::KnowledgeGroupAck::ErrorCode >( sword::KnowledgeGroupAck::error_invalid_superior );
    }
    if( pNewParent.get() )
    {
        if( parent_ && parent_ != pNewParent.get() )
        {
            boost::shared_ptr< MIL_KnowledgeGroup > shared = shared_from_this();
            // moving knowledge group from knowledgegroup under knowledgegroup
            parent_->UnregisterKnowledgeGroup( shared );
            pNewParent->RegisterKnowledgeGroup( shared );
            SetParent( pNewParent );
            return true;
        }
        else if( !parent_ )
        {
            boost::shared_ptr< MIL_KnowledgeGroup > shared = shared_from_this();
            // moving knowledge group from army node under knowledgegroup
            GetArmy().UnregisterKnowledgeGroup( shared );
            pNewParent->RegisterKnowledgeGroup( shared );
            SetParent( pNewParent );
            return true;
        }
    }
    else
    {
        // moving knowledge group under army node
        if( parent_ )
        {
            boost::shared_ptr< MIL_KnowledgeGroup > shared = shared_from_this();
            parent_->UnregisterKnowledgeGroup( shared );
            GetArmy().RegisterKnowledgeGroup( shared );
            boost::shared_ptr< MIL_KnowledgeGroup > noParent;
            SetParent( noParent );
            return true;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::OnReceiveKnowledgeGroupSetType
// Created: FHD 2009-12-17:
// -----------------------------------------------------------------------------
bool MIL_KnowledgeGroup::OnReceiveKnowledgeGroupSetType( const sword::MissionParameters& message )
{
    const MIL_KnowledgeGroupType* pFoundType = MIL_KnowledgeGroupType::FindType( message.elem( 0 ).value().Get( 0 ).acharstr() );
    if( pFoundType && pFoundType->GetID() != GetType().GetID() )
    {
        SetType( const_cast< MIL_KnowledgeGroupType* >( pFoundType ) );
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::OnReceiveKnowledgeGroupCreation
// Created: FHD 2009-12-17:
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::OnReceiveKnowledgeGroupCreation( const sword::KnowledgeGroupCreationRequest& /*message*/ )
{
    // TODO
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::OnReceiveKnowledgeGroupAddKnowledge
// Created: MMC 2011-06-09:
// -----------------------------------------------------------------------------
bool MIL_KnowledgeGroup::OnReceiveKnowledgeGroupAddKnowledge( const sword::MissionParameters& message )
{
    unsigned int identifier = message.elem( 0 ).value().Get( 0 ).identifier();
    additionalPerceptions_.insert( identifier );
    MIL_AgentPion* pAgent       = MIL_AgentServer::GetWorkspace().GetEntityManager().FindAgentPion( identifier );
    MIL_Object_ABC* pObject     = pAgent? 0 : MIL_AgentServer::GetWorkspace().GetEntityManager().FindObject( identifier );
    MIL_Population* pPopulation = ( pAgent || pObject )? 0 : MIL_AgentServer::GetWorkspace().GetEntityManager().FindPopulation( identifier );
    unsigned int perception = 0;
    if( message.elem( 1 ).value().Get( 0 ).has_quantity() )
        perception = message.elem( 1 ).value().Get( 0 ).quantity();
    else if( message.elem( 1 ).value().Get( 0 ).has_enumeration() )
        perception = PHY_PerceptionLevel::ConvertFromMsgIdToSimId( static_cast< sword::UnitIdentification_Level >( message.elem( 1 ).value().Get( 0 ).enumeration() ) );
    if ( perception < 1 || 3 < perception )
        throw NET_AsnException< sword::KnowledgeGroupAck::ErrorCode >( sword::KnowledgeGroupAck::error_invalid_perception );

    if ( pAgent )
    {
        if ( pAgent->GetKnowledgeGroup()->GetId() == this->GetId() )
            return false;

        DEC_Knowledge_Agent& knowledgeAgent = GetAgentKnowledgeToUpdate( *pAgent );
        knowledgeAgent.HackPerceptionLevel( &PHY_PerceptionLevel::FindPerceptionLevel( perception ) );
        HackPerceptionLevelFromParentKnowledgeGroup( *pAgent, perception );
    }
    else if ( pObject )
    {
        boost::shared_ptr< DEC_Knowledge_Object > knowledgeObject = GetObjectKnowledgeToUpdate( *pObject );
        if( !knowledgeObject.get() )
            throw NET_AsnException< sword::KnowledgeGroupAck::ErrorCode >( sword::KnowledgeGroupAck::error_invalid_unit );
        knowledgeObject->HackPerceptionLevel( &PHY_PerceptionLevel::FindPerceptionLevel( perception ) );
        HackPerceptionLevelFromParentKnowledgeGroup( *pObject, perception );
    }
    else if ( pPopulation )
    {
        DEC_Knowledge_Population& knowledgePopulation = GetPopulationKnowledgeToUpdate( *pPopulation );
        knowledgePopulation.HackPerceptionLevel( &PHY_PerceptionLevel::FindPerceptionLevel( perception ) );
        HackPerceptionLevelFromParentKnowledgeGroup( *pPopulation, perception );
    }
    else
        throw NET_AsnException< sword::KnowledgeGroupAck::ErrorCode >( sword::KnowledgeGroupAck::error_invalid_unit );

    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::HackPerceptionLevelFromParentKnowledgeGroup
// Created: MMC 2011-06-14
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::HackPerceptionLevelFromParentKnowledgeGroup( MIL_Agent_ABC& agent, unsigned int perception )
{
    additionalPerceptions_.insert( agent.GetID() );
    for( MIL_KnowledgeGroup::IT_KnowledgeGroupVector itKG( knowledgeGroups_.begin() ); itKG != knowledgeGroups_.end(); ++itKG )
    {
        boost::shared_ptr< MIL_KnowledgeGroup > pKnowledgeGroup = *itKG;
        if ( pKnowledgeGroup.get() )
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
            for( MIL_KnowledgeGroup::IT_KnowledgeGroupVector itKG( knowledgeGroups_.begin() ); itKG != knowledgeGroups_.end(); ++itKG )
            {
                boost::shared_ptr< MIL_KnowledgeGroup > pKnowledgeGroup = *itKG;
                if ( pKnowledgeGroup.get() )
                {
                    boost::shared_ptr< DEC_Knowledge_Object > curKnowledgeObject = pKnowledgeGroup->GetObjectKnowledgeToUpdate( object );
                    if ( curKnowledgeObject.get() )
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
    for( MIL_KnowledgeGroup::IT_KnowledgeGroupVector itKG( knowledgeGroups_.begin() ); itKG != knowledgeGroups_.end(); ++itKG )
    {
        boost::shared_ptr< MIL_KnowledgeGroup > pKnowledgeGroup = *itKG;
        if ( pKnowledgeGroup.get() )
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
// Created: JSR 2013-07-04
// -----------------------------------------------------------------------------
bool MIL_KnowledgeGroup::IsCrowd() const
{
    return isCrowd_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::CreateKnowledgeObject
// Created: LDC 2010-04-07
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > MIL_KnowledgeGroup::CreateKnowledgeObject( MIL_Object_ABC& objectKnown )
{
    if( knowledgeBlackBoard_ )
        return knowledgeBlackBoard_->GetKnowledgeObjectContainer().CreateKnowledgeObject( objectKnown.GetArmy(), objectKnown );
    return boost::shared_ptr< DEC_Knowledge_Object >();
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
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::ApplyPopulationPerception
// Created: LGY 2013-05-07
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::ApplyPopulationPerception( const MIL_Agent_ABC& pion, int currentTimeStep )
{
    boost::function< void( DEC_Knowledge_PopulationPerception& ) > functorPopulationPerception = boost::bind( & MIL_KnowledgeGroup::UpdatePopulationKnowledgeFromPerception, this, _1, boost::ref(currentTimeStep) );
    pion.GetKnowledge().GetKnowledgePopulationPerceptionContainer().ApplyOnKnowledgesPopulationPerception( functorPopulationPerception );
    boost::function< void( DEC_Knowledge_PopulationCollision& ) > functorPopulationCollision = boost::bind( & MIL_KnowledgeGroup::UpdatePopulationKnowledgeFromCollision, this, _1, boost::ref(currentTimeStep) );
    pion.GetKnowledge().GetKnowledgePopulationCollisionContainer ().ApplyOnKnowledgesPopulationCollision ( functorPopulationCollision );
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
                if( pion.GetRole< PHY_RolePion_Communications >().CanEmit() )
                    ApplyPopulationPerception( pion, currentTimeStep );
            }
        }
    }
    else if( jammedPion_ )
            ApplyPopulationPerception( *jammedPion_, currentTimeStep );

    // Mise à jour des groupes de connaissance avec les pions partageant les mêmes perceptions
    for( auto it = pions_.begin(); it != pions_.end(); ++it )
        ApplyPopulationPerception( **it, currentTimeStep );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::ApplyAgentPerception
// Created: LGY 2013-05-07
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::ApplyAgentPerception( const MIL_Agent_ABC& pion, int currentTimeStep )
{
    boost::function< void( DEC_Knowledge_AgentPerception& ) > functorAgent = boost::bind( & MIL_KnowledgeGroup::UpdateAgentKnowledgeFromAgentPerception, this, _1, boost::ref( currentTimeStep ) );
    pion.GetKnowledge().GetKnowledgeAgentPerceptionContainer().ApplyOnKnowledgesAgentPerception( functorAgent );
}

namespace
{
    class sAgentsCollidingPopulationVisitor : public MIL_EntityVisitor_ABC< MIL_PopulationElement_ABC >
    {
    public:
        sAgentsCollidingPopulationVisitor( boost::function< void( const MIL_Agent_ABC&, const MIL_Population& population ) > fun )
            : fun_( fun )
        {
            //NOTHING
        }
        ~sAgentsCollidingPopulationVisitor()
        {

        }
        virtual void Visit( const MIL_PopulationElement_ABC& element )
        {
            auto collidingAgents = element.GetCollidingAgents();
            for( auto it = collidingAgents.begin(); it != collidingAgents.end(); ++it )
                fun_( **it, element.GetPopulation() );
        }
    private:
        boost::function< void( const MIL_Agent_ABC&, const MIL_Population& ) > fun_;
    };

    class sObjectsCollidingPopulationVisitor : public MIL_EntityVisitor_ABC< MIL_PopulationElement_ABC >
    {
    public:
        sObjectsCollidingPopulationVisitor( boost::function< void( MIL_Object_ABC&, const MIL_Population& population ) > fun )
            : fun_( fun )
        {
            //NOTHING
        }
        ~sObjectsCollidingPopulationVisitor()
        {

        }
        virtual void Visit( const MIL_PopulationElement_ABC& element )
        {
            auto collidingObjects = element.GetCollidingObjects();
            for( auto it = collidingObjects.begin(); it != collidingObjects.end(); ++it )
                fun_( **it, element.GetPopulation() );
        }
    private:
        boost::function< void( MIL_Object_ABC&, const MIL_Population& ) > fun_;
    };

}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::UpdateAgentKnowledgeFromCrowdPerception
// Created: JSR 2013-07-08
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::UpdateAgentKnowledgeFromCrowdPerception( const MIL_Agent_ABC& agent, const MIL_Population& population, int currentTimeStep )
{
    DEC_Knowledge_Agent& knowledgeAgent = GetAgentKnowledgeToUpdate( agent );
    knowledgeAgent.UpdateFromCrowdPerception( population, currentTimeStep);
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::ApplyOnKnowledgesAgentPerception
// Created: FDS 2010-04-09
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::ApplyOnKnowledgesAgentPerception( int currentTimeStep )
{
    // Groupe de connaissance non restreint (émission ou reception)
    if( ! IsJammed() )
    {
        // Synthèse de la perception des subordonnés
        // Ajout automatique de la connaissance de chaque subordonné
        for( MIL_KnowledgeGroup::CIT_AutomateVector itAutomate = automates_.begin(); itAutomate != automates_.end(); ++itAutomate )
        {
            const MIL_Automate::T_PionVector& pions = (**itAutomate).GetPions();
            for( MIL_Automate::CIT_PionVector itPion = pions.begin(); itPion != pions.end(); ++itPion )
            {
                MIL_AgentPion& pion = **itPion;
                 // Les perceptions des subordonnées sont envoyées uniquement dans le cas ou celui ci peut communiquer.
                if( pion.GetRole< PHY_RolePion_Communications >().CanEmit() )
                    ApplyAgentPerception( pion, currentTimeStep );
            }
        }

        // Crowds
        for( auto it = populations_.begin(); it != populations_.end(); ++it )
        {
            sAgentsCollidingPopulationVisitor visitor( boost::bind( &MIL_KnowledgeGroup::UpdateAgentKnowledgeFromCrowdPerception, this, _1, _2, currentTimeStep ) );
            ( *it )->Apply( visitor );
        }

        // LTO begin
        // acquisition des connaissances des groupes fils
        for( MIL_KnowledgeGroup::CIT_KnowledgeGroupVector itKG( GetKnowledgeGroups().begin() ); itKG != GetKnowledgeGroups().end(); ++itKG )
        {
            const MIL_KnowledgeGroup& innerKg = **itKG;
            if( innerKg.IsEnabled() && IsEnabled() && !innerKg.IsJammed() && innerKg.GetKnowledge() )
            {
                boost::function< void( DEC_Knowledge_Agent& ) > functorAgent = boost::bind( &MIL_KnowledgeGroup::UpdateAgentKnowledgeFromParentKnowledgeGroup, this, _1, boost::ref(currentTimeStep) );
                innerKg.GetKnowledge()->GetKnowledgeAgentContainer().ApplyOnKnowledgesAgent( functorAgent );
            }
        }
        for( MIL_KnowledgeGroup::CIT_KnowledgeGroupVector itKG( GetKnowledgeGroups().begin() ); itKG != GetKnowledgeGroups().end(); ++itKG )
        {
            const MIL_KnowledgeGroup& innerKg = **itKG;
            if( innerKg.IsEnabled() && IsEnabled() && innerKg.IsJammed() && innerKg.CanReport() && innerKg.GetKnowledge() )
            {
                boost::function< void( DEC_Knowledge_AgentPerception& ) > functorAgent = boost::bind( & MIL_KnowledgeGroup::UpdateAgentKnowledgeFromAgentPerception, this, _1, boost::ref(currentTimeStep) );
                innerKg.ApplyOnKnowledgesAgentPerception( functorAgent );
            }
        }
        // LTO end
    }

    const PHY_RolePion_Communications* communications = jammedPion_ ? jammedPion_->RetrieveRole< PHY_RolePion_Communications >() : 0;
    if( !IsJammed() || ( communications && communications->CanReceive() ) )
    {
        // LTO begin
        //mis à jour des groupes de connaissances depuis leur parent avec un délai
        if( GetTimeToDiffuseToKnowledgeGroup() < currentTimeStep )
        {
            if( parent_ && IsEnabled() && parent_->GetKnowledge() )
            {
                boost::function< void( DEC_Knowledge_Agent& ) > functorAgent = boost::bind( &MIL_KnowledgeGroup::UpdateAgentKnowledgeFromParentKnowledgeGroup, this, _1, boost::ref(currentTimeStep) );
                parent_->GetKnowledge()->GetKnowledgeAgentContainer().ApplyOnPreviousKnowledgesAgent( functorAgent );
                parent_->GetKnowledge()->GetKnowledgeAgentContainer().SaveAllCurrentKnowledgeAgent();
            }
            bDiffuseToKnowledgeGroup_ = true;
        }
        // LTO end
    }

    // Mise à jour pour groupe de connaissance non restreint (silence partiel ou brouillage)
    if( IsJammed() && jammedPion_ )
        ApplyAgentPerception( *jammedPion_, currentTimeStep );

    // Mise à jour des groupes de connaissance avec les pions partageant les mêmes perceptions
    for( auto it = pions_.begin(); it != pions_.end(); ++it )
        ApplyAgentPerception( **it, currentTimeStep );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::UpdateObjectKnowledgeFromCrowdPerception
// Created: JSR 2013-07-08
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::UpdateObjectKnowledgeFromCrowdPerception( MIL_Object_ABC& object, const MIL_Population& population )
{
    boost::shared_ptr< DEC_Knowledge_Object > knowledgeObject = GetObjectKnowledgeToUpdate( object );
    if( knowledgeObject )
        knowledgeObject->UpdateFromCrowdPerception( population );
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
        sObjectsCollidingPopulationVisitor visitor( boost::bind( &MIL_KnowledgeGroup::UpdateObjectKnowledgeFromCrowdPerception, this, _1, _2 ) );
        ( *it )->Apply( visitor );
    }

    const PHY_RolePion_Communications* communications = jammedPion_ ? jammedPion_->RetrieveRole< PHY_RolePion_Communications >() : 0;
    if( !IsJammed() || ( communications && communications->CanReceive() ) )
    {
        if( GetTimeToDiffuseToKnowledgeGroup() < currentTimeStep )
        {
            if( parent_ && IsEnabled() && parent_->GetKnowledge() )
            {
                boost::function< void( DEC_Knowledge_Object& ) > functorObject = boost::bind( &MIL_KnowledgeGroup::UpdateObjectKnowledgeFromParentKnowledgeGroup, this, _1, boost::ref(currentTimeStep) );
                parent_->GetKnowledge()->GetKnowledgeObjectContainer().ApplyOnPreviousKnowledgesObject( functorObject );
                parent_->GetKnowledge()->GetKnowledgeObjectContainer().SaveAllCurrentKnowledgeObject();
            }
            bDiffuseToKnowledgeGroup_ = true;
        }
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
// Name: MIL_KnowledgeGroup::UpdateObjectKnowledgeFromParentKnowledgeGroup
// Created: MMC 2013-07-03
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::UpdateObjectKnowledgeFromParentKnowledgeGroup( const DEC_Knowledge_Object& objectKnowledge, int currentTimeStep )
{
    if( objectKnowledge.IsValid() && ( !parent_ || parent_->GetType().GetKnowledgeCommunicationDelay() <= currentTimeStep ) )
        if( objectKnowledge.GetObjectKnown() )
        {
            boost::shared_ptr< DEC_Knowledge_Object > pKnowledgeObject = GetObjectKnowledgeToUpdate( *objectKnowledge.GetObjectKnown() );
            if( pKnowledgeObject.get() )
                pKnowledgeObject->Update( objectKnowledge, currentTimeStep );
        }
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::GetAgentKnowledgeToUpdate
// Created: NLD 2004-03-19
// -----------------------------------------------------------------------------
inline
DEC_Knowledge_Agent& MIL_KnowledgeGroup::GetAgentKnowledgeToUpdate( const MIL_Agent_ABC& agentKnown )
{
    if( knowledgeBlackBoard_ )
    {
        boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge = knowledgeBlackBoard_->GetKnowledgeAgentContainer().GetKnowledgeAgent( agentKnown );
        if( pKnowledge.get() )
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
    boost::shared_ptr< MIL_KnowledgeGroup > shared = shared_from_this();
    return knowledgeBlackBoard_->CreateKnowledgeAgent( shared, const_cast< MIL_Agent_ABC& >( perceived ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::CreateKnowledgePopulation
// Created: FDS 2010-04-12
// -----------------------------------------------------------------------------
DEC_Knowledge_Population& MIL_KnowledgeGroup::CreateKnowledgePopulation( MIL_Population& perceived )
{
    boost::shared_ptr< MIL_KnowledgeGroup > shared = shared_from_this();
    return knowledgeBlackBoard_->CreateKnowledgePopulation( shared, perceived );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::UpdateKnowledgeFromTransported
// Created: LDC 2012-06-12
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::UpdateKnowledgeFromTransported( const MIL_Agent_ABC& perceived )
{
    DEC_Knowledge_Agent& agent = GetAgentKnowledgeToUpdate( perceived );
    agent.Extrapolate();
    agent.UpdateOnNetwork();
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::GetObjectKnowledgeToUpdate
// Created: MMC 2011-06-15
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > MIL_KnowledgeGroup::GetObjectKnowledgeToUpdate( MIL_Object_ABC& objectKnown )
{
    if( knowledgeBlackBoard_ )
    {
        boost::shared_ptr< DEC_Knowledge_Object > knowledgeObject = knowledgeBlackBoard_->GetKnowledgeObjectContainer().GetKnowledgeObject( objectKnown );
        if( knowledgeObject.get() )
            return knowledgeObject;
    }
    return CreateKnowledgeObject( objectKnown );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::Accept
// Created: LGY 2011-08-29
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::Accept( KnowledgesVisitor_ABC& visitor ) const
{
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
    for( std::set< unsigned int >::const_iterator it = additionalPerceptions_.begin(); it != additionalPerceptions_.end(); ++it )
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

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::RegisterPion
// Created: LGY 2013-05-07
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::RegisterPion( const MIL_Agent_ABC& agent )
{
    auto it = std::find( pions_.begin(), pions_.end(), &agent );
    if( it == pions_.end() )
        pions_.push_back( &agent );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::UnregisterPion
// Created: LGY 2013-05-07
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::UnregisterPion( const MIL_Agent_ABC& agent )
{
    pions_.erase( std::remove( pions_.begin(), pions_.end(), &agent ), pions_.end() );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::CanReport
// Created: LGY 2013-07-03
// -----------------------------------------------------------------------------
bool MIL_KnowledgeGroup::CanReport() const
{
    if( !jammedPion_ )
        return false;
    return jammedPion_->GetRole< PHY_RolePion_Communications >().CanReport();
}
