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
#include "DEC_KnowledgeBlackBoardObjects_ABC.h"
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
#include <boost/bind.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_KS_ObjectKnowledgeSynthetizer )

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectKnowledgeSynthetizer::serialize
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
template< typename Archive >
void DEC_KS_ObjectKnowledgeSynthetizer::serialize( Archive& archive, const unsigned int )
{
    archive & boost::serialization::base_object< DEC_KnowledgeSource_ABC >( *this )
            & pBlackBoard_;
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectKnowledgeSynthetizer constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_ObjectKnowledgeSynthetizer::DEC_KS_ObjectKnowledgeSynthetizer( DEC_KnowledgeBlackBoardObjects_ABC& blackBoard )
    : DEC_KnowledgeSource_ABC( blackBoard, 1 )
    , pBlackBoard_( &blackBoard )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectKnowledgeSynthetizer constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_ObjectKnowledgeSynthetizer::DEC_KS_ObjectKnowledgeSynthetizer()
    : DEC_KnowledgeSource_ABC()
    , pBlackBoard_( 0 )
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
    boost::shared_ptr< DEC_Knowledge_Object > pKnowledge = pBlackBoard_->ResolveKnowledgeObject( objectKnown );
    if( pKnowledge && pKnowledge->IsValid() )
        return pKnowledge;
    pKnowledge = pBlackBoard_->GetKnowledgeObjectContainer().CreateKnowledgeObject( &pBlackBoard_->GetArmy(), objectKnown );
    return pKnowledge;
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectKnowledgeSynthetizer::UpdateKnowledgesFromObjectPerception
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
inline
void DEC_KS_ObjectKnowledgeSynthetizer::UpdateKnowledgesFromObjectPerception( const DEC_Knowledge_ObjectPerception& perception )
{
    boost::shared_ptr< MIL_KnowledgeGroup > kg = pBlackBoard_->GetKnowledgeGroup();
    if( perception.GetObjectPerceived().IsUniversal() && ( kg.get() == 0 || !kg->IsJammed() ) )
        return;
    if( kg.get() && &kg.get()->GetArmy() == perception.GetObjectPerceived().GetArmy() )
        return;
    boost::shared_ptr< DEC_Knowledge_Object > pKnowledge = GetKnowledgeToUpdate( perception.GetObjectPerceived() );
    if( pKnowledge )
        pKnowledge ->Update( perception );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectKnowledgeSynthetizer::UpdateKnowledgesFromObjectCollision
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
inline
void DEC_KS_ObjectKnowledgeSynthetizer::UpdateKnowledgesFromObjectCollision( const DEC_Knowledge_ObjectCollision& collision )
{
    boost::shared_ptr< MIL_KnowledgeGroup > kg = pBlackBoard_->GetKnowledgeGroup();
    if( collision.GetObject().IsUniversal() && ( kg.get() == 0 || !kg->IsJammed() ) )
        return;
    if( kg.get() && &kg.get()->GetArmy() == collision.GetObject().GetArmy() )
        return;
    boost::shared_ptr< DEC_Knowledge_Object > pKnowledge = GetKnowledgeToUpdate( collision.GetObject() );
    if( pKnowledge )
        pKnowledge->Update( collision );
}

namespace
{
    typedef class_mem_fun_void_const_t< DEC_KS_ObjectKnowledgeSynthetizer, DEC_Knowledge_ObjectPerception > PerceptionFun;
    typedef class_mem_fun_void_const_t< DEC_KS_ObjectKnowledgeSynthetizer, DEC_Knowledge_ObjectCollision > CollisionFun;

    void SynthetizeSubordinatesPerception( const MIL_KnowledgeGroup::T_AutomateVector& automates, PerceptionFun& perceptionFun, CollisionFun& collisionFun )
    {
        for( MIL_KnowledgeGroup::CIT_AutomateVector itAutomate = automates.begin(); itAutomate != automates.end(); ++itAutomate )
        {
            const MIL_Automate::T_PionVector& pions = (**itAutomate).GetPions();
            for( MIL_Automate::CIT_PionVector itPion = pions.begin(); itPion != pions.end(); ++itPion )
            {
                MIL_AgentPion& pion = **itPion;
                if( pion.IsDead() )
                    continue;
                PHY_RoleInterface_Communications& communication = pion.Get< PHY_RoleInterface_Communications >();
                DEC_KnowledgeBlackBoard_AgentPion& blackboard = pion.GetKnowledge();
                DEC_BlackBoard_CanContainKnowledgeObjectPerception& perceptions = blackboard.GetKnowledgeObjectPerceptionContainer();
                DEC_BlackBoard_CanContainKnowledgeObjectCollision& collisions = blackboard.GetKnowledgeObjectCollisionContainer ();
                if( communication.CanEmit() )
                {
                    perceptions.ApplyOnKnowledgesObjectPerception( perceptionFun );
                    collisions.ApplyOnKnowledgesObjectCollision ( collisionFun);
                }
                else
                {
                    class_mem_fun_void_const_t< PHY_RoleInterface_Communications, DEC_Knowledge_ObjectPerception> methodUpdateAgentKnowledgesFromObjectPerception( & PHY_RoleInterface_Communications::UpdateKnowledgesFromObjectPerception, communication );
                    class_mem_fun_void_const_t< PHY_RoleInterface_Communications, DEC_Knowledge_ObjectCollision > methodUpdateAgentKnowledgesFromObjectCollision ( & PHY_RoleInterface_Communications::UpdateKnowledgesFromObjectCollision , communication );
                    perceptions.ApplyOnKnowledgesObjectPerception( methodUpdateAgentKnowledgesFromObjectPerception );
                    collisions.ApplyOnKnowledgesObjectCollision ( methodUpdateAgentKnowledgesFromObjectCollision );
                }
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
    PerceptionFun methodUpdateKnowledgesFromObjectPerception( & DEC_KS_ObjectKnowledgeSynthetizer::UpdateKnowledgesFromObjectPerception, *this );
    CollisionFun methodUpdateKnowledgesFromObjectCollision ( & DEC_KS_ObjectKnowledgeSynthetizer::UpdateKnowledgesFromObjectCollision , *this );
    boost::shared_ptr< MIL_KnowledgeGroup > kg = pBlackBoard_->GetKnowledgeGroup();
    if( kg.get() )
    {
        const MIL_KnowledgeGroup::T_AutomateVector& automates = kg->GetAutomates();
        ::SynthetizeSubordinatesPerception( automates, methodUpdateKnowledgesFromObjectPerception, methodUpdateKnowledgesFromObjectCollision );
    }
    else
    {
        const MIL_Army::T_KnowledgeGroupMap& knowledgeGroups = pBlackBoard_->GetArmy().GetKnowledgeGroups();
        for( MIL_Army::CIT_KnowledgeGroupMap itKnowledgeGroup = knowledgeGroups.begin(); itKnowledgeGroup != knowledgeGroups.end(); ++itKnowledgeGroup )
            ::SynthetizeSubordinatesPerception( itKnowledgeGroup->second->GetAutomates(), methodUpdateKnowledgesFromObjectPerception, methodUpdateKnowledgesFromObjectCollision );
    }
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
        boost::shared_ptr< DEC_Knowledge_Object > pKnowledge = GetKnowledgeToUpdate( object );
        if( pKnowledge )
            pKnowledge->Update( PHY_PerceptionLevel::identified_ );
    }
    ephemeralKnowledges_.clear();
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectKnowledgeSynthetizer::ProcessObjectsToForget
// Created: NLD 2004-06-04
// -----------------------------------------------------------------------------
void DEC_KS_ObjectKnowledgeSynthetizer::ProcessObjectsToForget()
{
    for( auto itObject = objectsToForget_.begin(); itObject != objectsToForget_.end(); ++itObject )
    {
        boost::shared_ptr< DEC_Knowledge_Object > pKnowledge = pBlackBoard_->ResolveKnowledgeObject( **itObject );
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
    for( CIT_KnowledgeObjectVector itKnowledge = knowledgesObjectToForget_.begin(); itKnowledge != knowledgesObjectToForget_.end(); ++itKnowledge )
        pBlackBoard_->GetKnowledgeObjectContainer().DestroyKnowledgeObject( **itKnowledge );
    knowledgesObjectToForget_.clear();
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectKnowledgeSynthetizer::UpdateKnowledgeRelevance
// Created: NLD 2005-10-19
// -----------------------------------------------------------------------------
void DEC_KS_ObjectKnowledgeSynthetizer::UpdateKnowledgeRelevance( boost::shared_ptr< DEC_Knowledge_Object >& knowledge )
{
    //$$$$ VIRER TOUT CA quand stockage dans blackboard avec comme clé l'ID de l'objet réel plutot que le pointeur de l'objet réél ...
    const MIL_Object_ABC* pObjectKnown = knowledge->GetObjectKnown();
    knowledge->UpdateRelevance();
    if( pObjectKnown && !knowledge->GetObjectKnown() )
        pBlackBoard_->GetKnowledgeObjectContainer().NotifyKnowledgeObjectDissociatedFromRealObject( *pObjectKnown, *knowledge );
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
void DEC_KS_ObjectKnowledgeSynthetizer::CleanKnowledgeObject( boost::shared_ptr< DEC_Knowledge_Object >& knowledge )
{
    if( knowledge->Clean() )
        pBlackBoard_->GetKnowledgeObjectContainer().DestroyKnowledgeObject( *knowledge ); // The knowledge will be deleted
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectKnowledgeSynthetizer::Clean
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
void DEC_KS_ObjectKnowledgeSynthetizer::Clean()
{
    class_mem_fun_void_t< DEC_KS_ObjectKnowledgeSynthetizer, boost::shared_ptr< DEC_Knowledge_Object > > methodObject( & DEC_KS_ObjectKnowledgeSynthetizer::CleanKnowledgeObject, *this );
    pBlackBoard_->GetKnowledgeObjectContainer().ApplyOnKnowledgesObject( methodObject );
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
    if( !Contains( knowledgesObjectToForget_, knowledge ) && pBlackBoard_->GetKnowledgeObjectContainer().GetKnowledgeObjectFromID( knowledge->GetID() ) )
        knowledgesObjectToForget_.push_back( knowledge );
}
