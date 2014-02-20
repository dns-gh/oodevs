// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KS_ObjectKnowledgeSynthetizer.cpp $
// $Author: Jvt $
// $Modtime: 23/03/05 18:57 $
// $Revision: 3 $
// $Workfile: DEC_KS_ObjectKnowledgeSynthetizer.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_KS_ObjectKnowledgeSynthetizer.h"
#include "DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "DEC_KnowledgeBlackBoard_AgentPion.h"
#include "DEC_BlackBoard_CanContainKnowledgeObject.h"
#include "DEC_BlackBoard_CanContainKnowledgeObjectCollision.h"
#include "DEC_BlackBoard_CanContainKnowledgeObjectPerception.h"
#include "DEC_Knowledge_ObjectPerception.h"
#include "DEC_Knowledge_ObjectCollision.h"
#include "DEC_Knowledge_Object.h"
#include "MIL_KnowledgeGroup.h"
#include "Entities/MIL_Army.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Communications/PHY_RoleInterface_Communications.h"
#include "Entities/Objects/MIL_Object_ABC.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_KS_ObjectKnowledgeSynthetizer )

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectKnowledgeSynthetizer constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_ObjectKnowledgeSynthetizer::DEC_KS_ObjectKnowledgeSynthetizer( DEC_KnowledgeBlackBoard_KnowledgeGroup& blackBoard )
    : DEC_KnowledgeSource_ABC( blackBoard, 1 )
    , pBlackBoard_           ( &blackBoard )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectKnowledgeSynthetizer constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_ObjectKnowledgeSynthetizer::DEC_KS_ObjectKnowledgeSynthetizer()
    : DEC_KnowledgeSource_ABC(  )
    , pBlackBoard_           ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectKnowledgeSynthetizer destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_ObjectKnowledgeSynthetizer::~DEC_KS_ObjectKnowledgeSynthetizer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectKnowledgeSynthetizer::Prepare
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
void DEC_KS_ObjectKnowledgeSynthetizer::Prepare()
{
    std::mem_fun_ref_t< void, DEC_Knowledge_Object > objectFunctor = std::mem_fun_ref( &DEC_Knowledge_Object::Prepare );
    pBlackBoard_->GetKnowledgeObjectContainer().ApplyOnKnowledgesObjectRef( objectFunctor );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectKnowledgeSynthetizer::GetKnowledgeToUpdate
// Created: NLD 2004-03-19
// -----------------------------------------------------------------------------
inline
boost::shared_ptr< DEC_Knowledge_Object > DEC_KS_ObjectKnowledgeSynthetizer::GetKnowledgeToUpdate( MIL_Object_ABC& objectKnown ) const
{
    assert( pBlackBoard_ );
    boost::shared_ptr< DEC_Knowledge_Object > pKnowledge = pBlackBoard_->GetKnowledgeObjectContainer().GetKnowledgeObject( objectKnown );

    if( pKnowledge && pKnowledge->IsValid() )
        return pKnowledge;

    return pBlackBoard_->GetKnowledgeObjectContainer().CreateKnowledgeObject( objectKnown );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectKnowledgeSynthetizer::UpdateKnowledgesFromObjectPerception
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
inline
void DEC_KS_ObjectKnowledgeSynthetizer::UpdateKnowledgesFromObjectPerception( const DEC_Knowledge_ObjectPerception& perception )
{
    boost::shared_ptr< DEC_Knowledge_Object > pKnowledge = GetKnowledgeToUpdate( perception.GetObjectPerceived() );
    if( pKnowledge )
        pKnowledge ->Update( perception );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectKnowledgeSynthetizer::UpdateKnowledgesFromObjectCollision
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
void DEC_KS_ObjectKnowledgeSynthetizer::UpdateKnowledgesFromObjectCollision( const DEC_Knowledge_ObjectCollision& collision )
{
    GetKnowledgeToUpdate( collision.GetObject() )->Update( collision );
}

namespace
{
    template< typename PerceptionFun, typename CollisionFun >
    void SynthetizeAgentPerception( MIL_AgentPion& pion, const MIL_KnowledgeGroup& knowledgeGroup, PerceptionFun& methodUpdateKnowledgesFromObjectPerception, CollisionFun& methodUpdateKnowledgesFromObjectCollision )
    {
        if( pion.IsDead() || pion.GetKnowledgeGroup().get() != &knowledgeGroup )
            return;
        PHY_RoleInterface_Communications* communication = pion.RetrieveRole< PHY_RoleInterface_Communications >();
        if( communication )
        {
            DEC_KnowledgeBlackBoard_AgentPion& blackboard = pion.GetKnowledge();
            DEC_BlackBoard_CanContainKnowledgeObjectPerception& perceptions = blackboard.GetKnowledgeObjectPerceptionContainer();
            DEC_BlackBoard_CanContainKnowledgeObjectCollision& collisions = blackboard.GetKnowledgeObjectCollisionContainer();
            if( communication->CanEmit() )
            {
                perceptions.ApplyOnKnowledgesObjectPerception( methodUpdateKnowledgesFromObjectPerception );
                collisions.ApplyOnKnowledgesObjectCollision( methodUpdateKnowledgesFromObjectCollision  );
            }
            else
            {
                auto methodUpdateAgentKnowledgesFromObjectPerception = boost::bind( &PHY_RoleInterface_Communications::UpdateKnowledgesFromObjectPerception, communication, _1 );
                perceptions.ApplyOnKnowledgesObjectPerception( methodUpdateAgentKnowledgesFromObjectPerception );
                auto methodUpdateAgentKnowledgesFromObjectCollision = boost::bind ( &PHY_RoleInterface_Communications::UpdateKnowledgesFromObjectCollision , communication, _1 );
                collisions.ApplyOnKnowledgesObjectCollision ( methodUpdateAgentKnowledgesFromObjectCollision );
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectKnowledgeSynthetizer::SynthetizeSubordinatesPerception
// Created: NLD 2004-06-04
// -----------------------------------------------------------------------------
void DEC_KS_ObjectKnowledgeSynthetizer::SynthetizeSubordinatesPerception()
{
    auto methodUpdateKnowledgesFromObjectPerception = boost::bind( &DEC_KS_ObjectKnowledgeSynthetizer::UpdateKnowledgesFromObjectPerception, this, _1 );
    auto methodUpdateKnowledgesFromObjectCollision = boost::bind( &DEC_KS_ObjectKnowledgeSynthetizer::UpdateKnowledgesFromObjectCollision , this, _1 );
    auto knowledgeGroup = pBlackBoard_->GetKnowledgeGroup();
    const auto& automates = knowledgeGroup->GetAutomates();
    for( auto itAutomate = automates.begin(); itAutomate != automates.end(); ++itAutomate )
    {
        const MIL_Automate::T_PionVector& pions = (**itAutomate).GetPions();
        for( auto itPion = pions.begin(); itPion != pions.end(); ++itPion )
            ::SynthetizeAgentPerception( **itPion, *knowledgeGroup, methodUpdateKnowledgesFromObjectPerception, methodUpdateKnowledgesFromObjectCollision );
    }
    MIL_AgentPion* jammed = const_cast< MIL_AgentPion* >( static_cast< const MIL_AgentPion* >( knowledgeGroup->GetJammedPion() ) );
    if( jammed )
        ::SynthetizeAgentPerception( *jammed, *knowledgeGroup, methodUpdateKnowledgesFromObjectPerception, methodUpdateKnowledgesFromObjectCollision );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectKnowledgeSynthetizer::ProcessEphemeralKnowledges
// Created: NLD 2004-06-04
// -----------------------------------------------------------------------------
void DEC_KS_ObjectKnowledgeSynthetizer::ProcessEphemeralKnowledges()
{
    for( auto itObject = ephemeralKnowledges_.begin(); itObject != ephemeralKnowledges_.end(); ++itObject )
    {
        MIL_Object_ABC& object = **itObject;
        if( object.IsMarkedForDestruction() )
            continue;
        GetKnowledgeToUpdate( object )->Update( PHY_PerceptionLevel::identified_ );
    }
    ephemeralKnowledges_.clear();
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectKnowledgeSynthetizer::ProcessObjectsToForget
// Created: NLD 2004-06-04
// -----------------------------------------------------------------------------
void DEC_KS_ObjectKnowledgeSynthetizer::ProcessObjectsToForget()
{
    assert( pBlackBoard_ );

    for( auto itObject = objectsToForget_.begin(); itObject != objectsToForget_.end(); ++itObject )
    {
        DEC_Knowledge_Object* pKnowledge = pBlackBoard_->GetKnowledgeObjectContainer().RetrieveKnowledgeObject( **itObject );
        if( pKnowledge && pKnowledge->IsValid() )
            pBlackBoard_->GetKnowledgeObjectContainer().DestroyKnowledgeObject( *pKnowledge );
    }
    objectsToForget_.clear();
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectKnowledgeSynthetizer::ProcessKnowledgesObjectToForget
// Created: NLD 2004-06-08
// -----------------------------------------------------------------------------
void DEC_KS_ObjectKnowledgeSynthetizer::ProcessKnowledgesObjectToForget()
{
    assert( pBlackBoard_ );

    for( auto itKnowledge = knowledgesObjectToForget_.begin(); itKnowledge != knowledgesObjectToForget_.end(); ++itKnowledge )
        pBlackBoard_->GetKnowledgeObjectContainer().DestroyKnowledgeObject( **itKnowledge );
    knowledgesObjectToForget_.clear();
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectKnowledgeSynthetizer::UpdateUniversalKnowledge
// Created: JSR 2013-12-16
// -----------------------------------------------------------------------------
void DEC_KS_ObjectKnowledgeSynthetizer::UpdateUniversalKnowledge( const boost::shared_ptr< DEC_Knowledge_Object >& knowledge )
{
    assert( pBlackBoard_ );
    knowledge->UpdateUniversalKnowledge();
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectKnowledgeSynthetizer::UpdateKnowledgeRelevance
// Created: NLD 2005-10-19
// -----------------------------------------------------------------------------
void DEC_KS_ObjectKnowledgeSynthetizer::UpdateKnowledgeRelevance( const boost::shared_ptr< DEC_Knowledge_Object >& knowledge )
{
    assert( pBlackBoard_ );
    knowledge->UpdateRelevance();
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectKnowledgeSynthetizer::Talk
// Created: NLD 2004-03-12
// -----------------------------------------------------------------------------
void DEC_KS_ObjectKnowledgeSynthetizer::Talk( int /*currentTimeStep*/ )
{
    // Synthesis of the perceptions of the subordinates
    SynthetizeSubordinatesPerception();

    // Ephemeral knowledges
    ProcessEphemeralKnowledges();

    // Universal objects
    boost::function< void( boost::shared_ptr< DEC_Knowledge_Object > ) > universalFunctor = boost::bind( &DEC_KS_ObjectKnowledgeSynthetizer::UpdateUniversalKnowledge, this, _1 );
    pBlackBoard_->GetKnowledgeObjectContainer().ApplyOnKnowledgesObject( universalFunctor );

    // Objects to forget
    ProcessObjectsToForget();

    // Knowledges object to forget (i.e. after an interaction on a knowledge dissociated from its real object)
    ProcessKnowledgesObjectToForget();

    // Relevance
    boost::function< void( boost::shared_ptr< DEC_Knowledge_Object > ) > objectFunctor = boost::bind( &DEC_KS_ObjectKnowledgeSynthetizer::UpdateKnowledgeRelevance, this, _1 );
    pBlackBoard_->GetKnowledgeObjectContainer().ApplyOnKnowledgesObject( objectFunctor );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectKnowledgeSynthetizer::CleanKnowledgeObject
// Created: NLD 2004-03-17
// -----------------------------------------------------------------------------
void DEC_KS_ObjectKnowledgeSynthetizer::CleanKnowledgeObject( const boost::shared_ptr< DEC_Knowledge_Object >& knowledge )
{
    if( knowledge->Clean() )
    {
        assert( pBlackBoard_ );
        pBlackBoard_->GetKnowledgeObjectContainer().DestroyKnowledgeObject( *knowledge ); // The knowledge will be deleted
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectKnowledgeSynthetizer::Clean
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
void DEC_KS_ObjectKnowledgeSynthetizer::Clean()
{
    assert( pBlackBoard_ );
    auto methodObject = boost::bind( &DEC_KS_ObjectKnowledgeSynthetizer::CleanKnowledgeObject, this, _1 );
    pBlackBoard_->GetKnowledgeObjectContainer().ApplyOnKnowledgesObject( methodObject );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectKnowledgeSynthetizer::CleanDeletedAgentKnowledges
// Created: JSR 2013-02-21
// -----------------------------------------------------------------------------
void DEC_KS_ObjectKnowledgeSynthetizer::CleanDeletedAgentKnowledges()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectKnowledgeSynthetizer::AddEphemeralObjectKnowledge
// Created: NLD 2004-03-30
// -----------------------------------------------------------------------------
void DEC_KS_ObjectKnowledgeSynthetizer::AddEphemeralObjectKnowledge( MIL_Object_ABC& objectKnown )
{
    ephemeralKnowledges_.push_back( &objectKnown );
}

namespace
{
    template< class T >
    bool Contains( const std::vector< T >& v, T value )
    {
        return std::find( v.begin(), v.end(), value ) != v.end();
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectKnowledgeSynthetizer::AddObjectKnowledgeToForget
// Created: NLD 2004-06-04
// -----------------------------------------------------------------------------
void DEC_KS_ObjectKnowledgeSynthetizer::AddObjectKnowledgeToForget( MIL_Object_ABC& objectKnown )
{
    if( !Contains( objectsToForget_, &objectKnown ) && pBlackBoard_->ResolveKnowledgeObject( objectKnown ) )
        objectsToForget_.push_back( &objectKnown );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectKnowledgeSynthetizer::AddObjectKnowledgeToForget
// Created: NLD 2004-06-08
// -----------------------------------------------------------------------------
void DEC_KS_ObjectKnowledgeSynthetizer::AddObjectKnowledgeToForget( boost::shared_ptr< DEC_Knowledge_Object > knowledge )
{
    if( !Contains( knowledgesObjectToForget_, knowledge ) && pBlackBoard_->ResolveKnowledgeObject( knowledge->GetID() ) )
        knowledgesObjectToForget_.push_back( knowledge );
}
