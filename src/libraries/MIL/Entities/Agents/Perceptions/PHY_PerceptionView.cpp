//*****************************************************************************
// Created: DFT 02-02-26
//*****************************************************************************

#include "MIL_pch.h"

#include "PHY_PerceptionView.h"

#include "PHY_PerceptionLevel.h"

#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/MIL_Army.h"
#include "Entities/Objects/MIL_RealObject_ABC.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"
#include "Entities/Populations/MIL_PopulationFlow.h"
#include "Entities/Populations/MIL_PopulationConcentration.h"

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionView constructor
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
PHY_PerceptionView::PHY_PerceptionView( PHY_RolePion_Perceiver& perceiver )
    : PHY_Perception_ABC( perceiver )
    , bIsEnabled_       ( true )
{

}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionView destructor
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
PHY_PerceptionView::~PHY_PerceptionView()
{
}

// =============================================================================
// PERCEPTION POINT
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionView::Compute
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionView::Compute( const MT_Vector2D& vPoint ) const
{
    const PHY_PerceptionLevel* pBestLevel = &PHY_PerceptionLevel::notSeen_;
    
    if( bIsEnabled_ )
    {
        const PHY_RolePion_Perceiver::T_SurfaceAgentMap& surfaces = perceiver_.GetSurfacesAgent();
        for( PHY_RolePion_Perceiver::CIT_SurfaceAgentMap itSurface = surfaces.begin(); itSurface != surfaces.end(); ++itSurface )
        {
            const PHY_PerceptionLevel& currentLevel = itSurface->second.ComputePerception( perceiver_, vPoint );
            if( currentLevel > *pBestLevel )
            {
                pBestLevel = &currentLevel;
                if( pBestLevel->IsBestLevel() )
                    return *pBestLevel;
            }
        }
    }

    return *pBestLevel;
}

// =============================================================================
// PERCEPTION AGENTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionView::Compute
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionView::Compute( const DEC_Knowledge_Agent & knowledge ) const
{
    const PHY_PerceptionLevel* pBestLevel = &PHY_PerceptionLevel::notSeen_;
    
    if( bIsEnabled_ )
    {
        const PHY_RolePion_Perceiver::T_SurfaceAgentMap& surfaces = perceiver_.GetSurfacesAgent();
        for( PHY_RolePion_Perceiver::CIT_SurfaceAgentMap itSurface = surfaces.begin(); itSurface != surfaces.end(); ++itSurface )
        {
            const PHY_PerceptionLevel& currentLevel = itSurface->second.ComputePerception( perceiver_, knowledge );
            if( currentLevel > *pBestLevel )
            {
                pBestLevel = &currentLevel;
                if( pBestLevel->IsBestLevel() )
                    return *pBestLevel;
            }
        }
    }

    return *pBestLevel;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionView::Compute
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionView::Compute( const MIL_Agent_ABC& target ) const
{
    if( target.BelongsTo( perceiver_.GetKnowledgeGroup() ) || perceiver_.IsIdentified( target ) )
        return PHY_PerceptionLevel::identified_;

    if( !( bIsEnabled_ && target.GetRole< PHY_RoleInterface_Posture >().CanBePerceived( perceiver_.GetPion() ) ) )
        return PHY_PerceptionLevel::notSeen_;

    const PHY_PerceptionLevel* pBestLevel = &PHY_PerceptionLevel::notSeen_;
    const PHY_RolePion_Perceiver::T_SurfaceAgentMap& surfaces = perceiver_.GetSurfacesAgent();
    for ( PHY_RolePion_Perceiver::CIT_SurfaceAgentMap itSurface = surfaces.begin(); itSurface != surfaces.end(); ++itSurface )
    {
        const PHY_PerceptionLevel& currentLevel = itSurface->second.ComputePerception( perceiver_, target );
        if( currentLevel > *pBestLevel )
        {
            pBestLevel = &currentLevel;
            if( pBestLevel->IsBestLevel() )
                return *pBestLevel;
        }       
    }
    return *pBestLevel;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionView::Execute
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
void PHY_PerceptionView::Execute( const TER_Agent_ABC::T_AgentPtrVector& perceivableAgents )
{
    if( bIsEnabled_ )
    {
        for ( TER_Agent_ABC::CIT_AgentPtrVector itAgent = perceivableAgents.begin(); itAgent != perceivableAgents.end(); ++itAgent )
        {
            MIL_Agent_ABC& agent = static_cast< PHY_RoleInterface_Location& >( **itAgent ).GetAgent();
            perceiver_.NotifyPerception( agent, Compute( agent ) );
        }
    }
}


// =============================================================================
// PERCEPTION OBJECTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionView::Compute
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionView::Compute( const DEC_Knowledge_Object& knowledge ) const
{
    const PHY_PerceptionLevel* pBestLevel = &PHY_PerceptionLevel::notSeen_;
    
    if( bIsEnabled_ )
    {
        const PHY_RolePion_Perceiver::T_SurfaceObjectMap& surfaces = perceiver_.GetSurfacesObject();
        for( PHY_RolePion_Perceiver::CIT_SurfaceObjectMap itSurface = surfaces.begin(); itSurface != surfaces.end(); ++itSurface )
        {
            const PHY_PerceptionLevel& currentLevel = itSurface->second.ComputePerception( perceiver_, knowledge );
            if( currentLevel > *pBestLevel )
            {
                pBestLevel = &currentLevel;
                if( pBestLevel->IsBestLevel() )
                    return *pBestLevel;
            }
        }
    }

    return *pBestLevel;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionView::Compute
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionView::Compute( const MIL_RealObject_ABC& target ) const
{
    if( !bIsEnabled_ )
        return PHY_PerceptionLevel::notSeen_;

    if( perceiver_.IsIdentified( target ) )
        return PHY_PerceptionLevel::identified_;

    const PHY_PerceptionLevel* pBestLevel = &PHY_PerceptionLevel::notSeen_;
    const PHY_RolePion_Perceiver::T_SurfaceObjectMap& surfaces = perceiver_.GetSurfacesObject();
    for( PHY_RolePion_Perceiver::CIT_SurfaceObjectMap itSurface = surfaces.begin(); itSurface != surfaces.end(); ++itSurface )
    {
        const PHY_PerceptionLevel& currentLevel = itSurface->second.ComputePerception( perceiver_, target );
        if( currentLevel > *pBestLevel )
        {
            pBestLevel = &currentLevel;
            if( pBestLevel->IsBestLevel() )
                return *pBestLevel;
        }       
    }
    return *pBestLevel;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionView::Execute
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_PerceptionView::Execute( const TER_Object_ABC::T_ObjectVector& perceivableObjects )
{
    if( bIsEnabled_ )
    {
        for( TER_Object_ABC::CIT_ObjectVector itObject = perceivableObjects.begin(); itObject != perceivableObjects.end(); ++itObject )
        {
            MIL_Object_ABC& object = static_cast< MIL_Object_ABC& >( **itObject );
            if( !object.IsReal() )
                continue;
            MIL_RealObject_ABC& realObject = static_cast< MIL_RealObject_ABC& >( object );
            perceiver_.NotifyPerception( realObject, Compute( realObject ) );
        }
    }
}

// =============================================================================
// PERCEPTION POPULATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionView::Compute
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionView::Compute( const MIL_PopulationFlow& flow, T_PointVector& shape ) const
{
    if( !bIsEnabled_ )
        return PHY_PerceptionLevel::notSeen_;

    const PHY_PerceptionSurfaceAgent* pBestSurface = 0;
          MT_Float                    rBestCost    = std::numeric_limits< MT_Float >::min();

    const PHY_RolePion_Perceiver::T_SurfaceAgentMap& surfaces = perceiver_.GetSurfacesAgent();
    for( PHY_RolePion_Perceiver::CIT_SurfaceAgentMap itSurface = surfaces.begin(); itSurface != surfaces.end(); ++itSurface )
    {
        const PHY_PerceptionSurfaceAgent& surface = itSurface->second;
        const MT_Float                    rCost   = surface.ComputePerceptionAccuracy( perceiver_, flow );
        if( rCost > rBestCost )
        {
            rBestCost    = rCost;
            pBestSurface = &surface;
        }
    }
    if( !pBestSurface )
        return PHY_PerceptionLevel::notSeen_;
    return pBestSurface->ComputePerception( perceiver_, flow, shape );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionView::Execute
// Created: NLD 2005-10-11
// -----------------------------------------------------------------------------
void PHY_PerceptionView::Execute( const TER_PopulationFlow_ABC::T_PopulationFlowVector& perceivableFlows )
{
    if( bIsEnabled_ )
    {
        for( TER_PopulationFlow_ABC::CIT_PopulationFlowVector it = perceivableFlows.begin(); it != perceivableFlows.end(); ++it )
        {
            MIL_PopulationFlow& flow = static_cast< MIL_PopulationFlow& >( **it );

            T_PointVector shape;
            const PHY_PerceptionLevel& level = Compute( flow, shape );
            perceiver_.NotifyPerception( flow, level, shape );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionView::Compute
// Created: NLD 2005-10-11
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionView::Compute( const MIL_PopulationConcentration& target ) const
{
    if( !bIsEnabled_ )
        return PHY_PerceptionLevel::notSeen_;

    if( perceiver_.IsIdentified( target ) )
        return PHY_PerceptionLevel::identified_;

    const PHY_PerceptionLevel* pBestLevel = &PHY_PerceptionLevel::notSeen_;
    const PHY_RolePion_Perceiver::T_SurfaceAgentMap& surfaces = perceiver_.GetSurfacesAgent();
    for( PHY_RolePion_Perceiver::CIT_SurfaceAgentMap itSurface = surfaces.begin(); itSurface != surfaces.end(); ++itSurface )
    {
        const PHY_PerceptionLevel& currentLevel = itSurface->second.ComputePerception( perceiver_, target );
        if( currentLevel > *pBestLevel )
        {
            pBestLevel = &currentLevel;
            if( pBestLevel->IsBestLevel() )
                return *pBestLevel;
        }       
    }
    return *pBestLevel;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionView::Execute
// Created: NLD 2005-10-11
// -----------------------------------------------------------------------------
void PHY_PerceptionView::Execute( const TER_PopulationConcentration_ABC::T_PopulationConcentrationVector perceivableConcentrations )
{
    if( bIsEnabled_ )
    {
        for( TER_PopulationConcentration_ABC::CIT_PopulationConcentrationVector it = perceivableConcentrations.begin(); it != perceivableConcentrations.end(); ++it )
        {
            MIL_PopulationConcentration& concentration = static_cast< MIL_PopulationConcentration& >( **it );
            perceiver_.NotifyPerception( concentration, Compute( concentration ) );
        }
    }
}

