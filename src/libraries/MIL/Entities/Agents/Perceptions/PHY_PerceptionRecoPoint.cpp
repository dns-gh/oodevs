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

#include "MIL_pch.h"
#include "PHY_PerceptionRecoPoint.h"

#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Objects/MIL_RealObject_ABC.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_Knowledge_Object.h"

#include "TER/TER_World.h"

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoPoint::PHY_PerceptionRecoPoint
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
PHY_PerceptionRecoPoint::PHY_PerceptionRecoPoint( PHY_RolePion_Perceiver& perceiver )
    : PHY_Perception_ABC( perceiver )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoPoint::~PHY_PerceptionRecoPoint
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
PHY_PerceptionRecoPoint::~PHY_PerceptionRecoPoint()
{
    for( IT_RecoVector it = recos_.begin(); it != recos_.end(); ++it )
        delete *it;
    recos_.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoPoint::AddPoint
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
void* PHY_PerceptionRecoPoint::AddPoint( const MT_Vector2D& center, MT_Float rSize, MT_Float rSpeed, DIA_Variable_ABC& result )
{
    sReco* pNewReco = new sReco( center, rSize, rSpeed, result );

    result.SetValue( false );
    recos_.push_back( pNewReco );
    return pNewReco;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoPoint::RemovePoint
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
void PHY_PerceptionRecoPoint::RemovePoint( void* pId )
{
    sReco* pReco = static_cast< sReco* >( pId );

    IT_RecoVector it = std::find( recos_.begin(), recos_.end(), pReco );
    if ( it != recos_.end() )
    {
        delete pReco;
        recos_.erase( it );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoPoint::Update
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
void PHY_PerceptionRecoPoint::Update()
{
    for ( IT_RecoVector it = recos_.begin(); it != recos_.end(); ++it )
    {
        sReco& reco = **it;

        if ( reco.pReturn_ )
        {
            // Agrandissement de la zone de reconnaissance
            if ( reco.rCurrentSize_ < reco.rFinalSize_ )
                reco.rCurrentSize_ += reco.rGrowthSpeed_;
            if ( reco.rCurrentSize_ > reco.rFinalSize_ )
                reco.rCurrentSize_ = reco.rFinalSize_;
            if ( reco.rCurrentSize_ == reco.rFinalSize_ )
            {
                reco.pReturn_->SetValue( true );
                reco.pReturn_ = 0;
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
    for ( CIT_RecoVector it = recos_.begin(); it != recos_.end(); ++it )
        if ( (*it)->vCenter_.SquareDistance( vPoint ) <= (*it)->rCurrentSize_ * (*it)->rCurrentSize_ )
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

    for ( CIT_RecoVector itReco = recos_.begin(); itReco != recos_.end(); ++itReco )
    {
        perceivableAgents.clear();

        TER_World::GetWorld().GetAgentManager().GetListWithinCircle( (*itReco)->vCenter_, (*itReco)->rCurrentSize_, perceivableAgents );

        for ( TER_Agent_ABC::CIT_AgentPtrVector it = perceivableAgents.begin(); it != perceivableAgents.end(); ++it )
        {
            MIL_Agent_ABC& target = static_cast< PHY_RoleInterface_Location& >( **it ).GetAgent();
            if( target.GetRole< PHY_RoleInterface_Posture >().CanBePerceived( perceiver_.GetPion() ) )
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
    if( !agent.GetRole< PHY_RoleInterface_Posture >().CanBePerceived( perceiver_.GetPion() ) )
        return PHY_PerceptionLevel::notSeen_;
    return Compute( agent.GetRole< PHY_RoleInterface_Location >().GetPosition() );
}


// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoPoint::Compute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionRecoPoint::Compute( const DEC_Knowledge_Agent & knowledge ) const
{
    return Compute( knowledge.GetPosition() );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoPoint::Compute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionRecoPoint::Compute( const MIL_RealObject_ABC& object ) const
{
    for ( CIT_RecoVector it = recos_.begin(); it != recos_.end(); ++it )
        if ( object.Intersect2DWithCircle( (*it)->vCenter_, (*it)->rCurrentSize_ ) )
            return PHY_PerceptionLevel::recognized_;
    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoPoint::Compute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionRecoPoint::Compute( const DEC_Knowledge_Object& knowledge ) const
{
    for ( CIT_RecoVector it = recos_.begin(); it != recos_.end(); ++it )
        if ( knowledge.GetLocalisation().Intersect2DWithCircle( (*it)->vCenter_, (*it)->rCurrentSize_ ) )
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

    for ( CIT_RecoVector itReco = recos_.begin(); itReco != recos_.end(); ++itReco )
    {
        perceivableObjects.clear();

        TER_World::GetWorld().GetObjectManager().GetListWithinCircle( (*itReco)->vCenter_, (*itReco)->rCurrentSize_, perceivableObjects );
        for ( TER_Object_ABC::CIT_ObjectVector it = perceivableObjects.begin(); it != perceivableObjects.end(); ++it )
        {
            MIL_Object_ABC& object = static_cast< MIL_Object_ABC& >( **it );
            if( !object.IsReal() )
                continue;
            MIL_RealObject_ABC& realObject = static_cast< MIL_RealObject_ABC& >( object );
            if( realObject.CanBePerceived() )
                perceiver_.NotifyPerception( realObject, PHY_PerceptionLevel::identified_ ); // Identifié ou not seen pour les objets
        }
    }
}
