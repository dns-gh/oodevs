// *****************************************************************************
//
// $Created: JVT 2004-10-21 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Perceptions/PHY_PerceptionRecoPoint.cpp $
// $Author: Nld $
// $Modtime: 2/05/05 17:17 $
// $Revision: 8 $
// $Workfile: PHY_PerceptionRecoPoint.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_PerceptionRecoPoint.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Objects/MIL_ObjectManipulator_ABC.h"
#include "Decision/DEC_Decision_ABC.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "DetectionComputer.h"
#include "simulation_terrain/TER_World.h"
#include "simulation_terrain/TER_ObjectManager.h"
#include "simulation_terrain/TER_AgentManager.h"

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoPoint::PHY_PerceptionRecoPoint
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
PHY_PerceptionRecoPoint::PHY_PerceptionRecoPoint( PHY_RoleInterface_Perceiver& perceiver )
    : PHY_PerceptionWithLocation< PHY_PerceptionRecoPointReco >( perceiver )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoPoint::~PHY_PerceptionRecoPoint
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
PHY_PerceptionRecoPoint::~PHY_PerceptionRecoPoint()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoPoint::AddPoint
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
int PHY_PerceptionRecoPoint::AddPoint( const MT_Vector2D& center, double rSize, double rSpeed, DEC_Decision_ABC& callerAgent )
{
    return Add( new PHY_PerceptionRecoPointReco( center, rSize, rSpeed, callerAgent ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoPoint::RemovePoint
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
void PHY_PerceptionRecoPoint::RemovePoint( int id )
{
    Remove( id );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoPoint::Update
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
void PHY_PerceptionRecoPoint::Update()
{
    for( auto it = recos_.begin(); it != recos_.end(); ++it )
    {
        if( !it->bProcessed_ )
        {
            // Agrandissement de la zone de reconnaissance
            it->rCurrentSize_ = std::min( it->rFinalSize_, it->rCurrentSize_ + it->rGrowthSpeed_ );
            if( it->rCurrentSize_ == it->rFinalSize_ )
            {
                it->callerAgent_.CallbackPerception( it->Id() );
                it->bProcessed_ = true;
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoPoint::Compute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionRecoPoint::Compute( const MT_Vector2D& vPoint ) const
{
    for( auto it = recos_.begin(); it != recos_.end(); ++it )
        if( it->vCenter_.SquareDistance( vPoint ) <= it->rCurrentSize_ * it->rCurrentSize_ )
            return PHY_PerceptionLevel::recognized_;
    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoPoint::Execute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
void PHY_PerceptionRecoPoint::Execute( const TER_Agent_ABC::T_AgentPtrVector& /*perceivableAgents*/ )
{
    TER_Agent_ABC::T_AgentPtrVector perceivableAgents;
    for( auto itReco = recos_.begin(); itReco != recos_.end(); ++itReco )
    {
        perceivableAgents.clear();
        TER_World::GetWorld().GetAgentManager().GetListWithinCircle( itReco->vCenter_, itReco->rCurrentSize_, perceivableAgents );
        for( auto it = perceivableAgents.begin(); it != perceivableAgents.end(); ++it )
        {
            MIL_Agent_ABC& target = static_cast< PHY_RoleInterface_Location& >( **it ).GetAgent();
            detection::DetectionComputer detectionComputer( target );
            perceiver_.GetPion().Execute( detectionComputer );
            target.Execute( detectionComputer );
            if( detectionComputer.CanBeSeen() )
                perceiver_.NotifyPerception( target, PHY_PerceptionLevel::recognized_ );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoPoint::Compute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionRecoPoint::Compute( const MIL_Agent_ABC& agent ) const
{
    return Compute( agent.GetRole< PHY_RoleInterface_Location >().GetPosition() );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoPoint::Compute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionRecoPoint::Compute( const DEC_Knowledge_Object& knowledge ) const
{
    for( auto it = recos_.begin(); it != recos_.end(); ++it )
        if( knowledge.GetLocalisation().Intersect2DWithCircle( it->vCenter_, it->rCurrentSize_ ) )
            return PHY_PerceptionLevel::recognized_;
    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoPoint::Execute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
void PHY_PerceptionRecoPoint::Execute( const TER_Object_ABC::T_ObjectVector& /*perceivableObjects*/ )
{
    TER_Object_ABC::T_ObjectVector perceivableObjects;
    for( auto itReco = recos_.begin(); itReco != recos_.end(); ++itReco )
    {
        perceivableObjects.clear();
        TER_World::GetWorld().GetObjectManager().GetListWithinCircle( itReco->vCenter_, itReco->rCurrentSize_, perceivableObjects );
        for( auto it = perceivableObjects.begin(); it != perceivableObjects.end(); ++it )
        {
            MIL_Object_ABC& object = static_cast< MIL_Object_ABC& >( **it );
            if( object().CanBePerceived() )
                perceiver_.NotifyPerception( object, PHY_PerceptionLevel::identified_ ); // Identifié ou not seen pour les objets
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoPoint::HasPointToHandle
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
bool PHY_PerceptionRecoPoint::HasPointToHandle() const
{
    return !recos_.empty();
}
