//*****************************************************************************
// Created: DFT 02-02-26
//*****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_PerceptionView.h"
#include "PHY_PerceptionLevel.h"
#include "PHY_ZURBPerceptionComputer.h"
#include "PHY_ZOPerceptionComputer.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"
#include "Entities/Agents/Roles/Urban/PHY_RoleInterface_UrbanLocation.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Objects/MIL_ObjectManipulator_ABC.h"
#include "Entities/Orders/MIL_Report.h"
#include "Entities/Populations/MIL_PopulationFlow.h"
#include "Entities/Populations/MIL_PopulationConcentration.h"
#include "DetectionComputer_ABC.h"
#include "DetectionComputerFactory_ABC.h"
#include "MIL_Random.h"
#include "Knowledge/MIL_KnowledgeGroup.h"

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionView constructor
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
PHY_PerceptionView::PHY_PerceptionView( PHY_RoleInterface_Perceiver& perceiver, MIL_Agent_ABC& pion )
    : PHY_Perception_ABC( perceiver )
    , bIsEnabled_       ( true )
    , wasInCity_        ( pion.GetRole< PHY_RoleInterface_UrbanLocation >().IsInCity() ) // TODO SLG : Passer par algo car dépendance de role en perceiver et urbanlocation
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionView destructor
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
PHY_PerceptionView::~PHY_PerceptionView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionView::Compute
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionView::Compute( const MT_Vector2D& vPoint ) const
{
    const PHY_PerceptionLevel* pBestLevel = &PHY_PerceptionLevel::notSeen_;

    if( bIsEnabled_ )
    {
        const PHY_RoleInterface_Perceiver::T_SurfaceAgentMap& surfaces = perceiver_.GetSurfacesAgent();
        for( PHY_RoleInterface_Perceiver::CIT_SurfaceAgentMap itSurface = surfaces.begin(); itSurface != surfaces.end(); ++itSurface )
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

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionView::Compute
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionView::Compute( const DEC_Knowledge_Agent & knowledge ) const
{
    const PHY_PerceptionLevel* pBestLevel = &PHY_PerceptionLevel::notSeen_;

    if( bIsEnabled_ )
    {
        const PHY_RoleInterface_Perceiver::T_SurfaceAgentMap& surfaces = perceiver_.GetSurfacesAgent();
        for( PHY_RoleInterface_Perceiver::CIT_SurfaceAgentMap itSurface = surfaces.begin(); itSurface != surfaces.end(); ++itSurface )
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
const PHY_PerceptionLevel& PHY_PerceptionView::Compute( const MIL_Agent_ABC& target )
{
    TransfertPerception();
    if( target.BelongsTo( perceiver_.GetKnowledgeGroup() ) || perceiver_.IsIdentified( target ) )
        return PHY_PerceptionLevel::identified_;

    if( !bIsEnabled_ )
        return PHY_PerceptionLevel::notSeen_;

    if( !perceiver_.GetPion().GetRole< PHY_RoleInterface_UrbanLocation >().IsInCity() )
    {
        PHY_ZOPerceptionComputer computer( perceiver_.GetPion() );
        return computer.ComputePerception( target );
    }
    else
    {
        CIT_PerceptionTickMap it = perceptionsUnderway_.find( &target );
        unsigned int tick = 0;
        double roll = 0.f;
        if( it != perceptionsUnderway_.end() )
        {
            tick = it->second.first;
            roll = it->second.second;
        }
        else
            roll = static_cast< double >( MIL_Random::rand_ii( MIL_Random::ePerception ) );
        perceptionsBuffer_[ &target ] = std::pair< unsigned int, double >( tick + 1, roll );
        PHY_ZURBPerceptionComputer computer( perceiver_.GetPion(), roll, tick );
        return computer.ComputePerception( target );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionView::Execute
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
void PHY_PerceptionView::Execute( const TER_Agent_ABC::T_AgentPtrVector& perceivableAgents, const detection::DetectionComputerFactory_ABC& detectionComputerFactory )
{
    if( bIsEnabled_ )
    {
        bool civiliansEncountered = false;
        for ( TER_Agent_ABC::CIT_AgentPtrVector itAgent = perceivableAgents.begin(); itAgent != perceivableAgents.end(); ++itAgent )
        {
            MIL_Agent_ABC& agent = static_cast< PHY_RoleInterface_Location& >( **itAgent ).GetAgent();

            if( agent.BelongsTo( perceiver_.GetKnowledgeGroup() ) )
                continue;

            std::auto_ptr< detection::DetectionComputer_ABC > detectionComputer( detectionComputerFactory.Create( agent ) );
            perceiver_.GetPion().Execute( *detectionComputer );
            agent.Execute( *detectionComputer );

            if ( perceiver_.GetKnowledgeGroup().IsPerceptionDistanceHacked( agent ) )
                perceiver_.NotifyPerception( agent, perceiver_.GetKnowledgeGroup().GetPerceptionLevel( agent ) );
            else if( detectionComputer->CanBeSeen() && perceiver_.NotifyPerception( agent, Compute( agent ) ) )
                if( !civiliansEncountered && agent.GetType().IsRefugee() )
                {
                    MIL_Report::PostEvent( perceiver_.GetPion(), MIL_Report::eReport_CiviliansEncountered );
                    civiliansEncountered = true;
                }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionView::Compute
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionView::Compute( const DEC_Knowledge_Object& knowledge ) const
{
    const PHY_PerceptionLevel* pBestLevel = &PHY_PerceptionLevel::notSeen_;

    if( bIsEnabled_ )
    {
        const PHY_RoleInterface_Perceiver::T_SurfaceObjectMap& surfaces = perceiver_.GetSurfacesObject();
        for( PHY_RoleInterface_Perceiver::CIT_SurfaceObjectMap itSurface = surfaces.begin(); itSurface != surfaces.end(); ++itSurface )
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
const PHY_PerceptionLevel& PHY_PerceptionView::Compute( const MIL_Object_ABC& target ) const
{
    if( !bIsEnabled_ || !target().CanBePerceived() )
        return PHY_PerceptionLevel::notSeen_;

    if( target.IsUniversal() )
        return PHY_PerceptionLevel::identified_;

    if( perceiver_.IsIdentified( target ) )
        return PHY_PerceptionLevel::identified_;

    const PHY_PerceptionLevel* pBestLevel = &PHY_PerceptionLevel::notSeen_;
    const PHY_RoleInterface_Perceiver::T_SurfaceObjectMap& surfaces = perceiver_.GetSurfacesObject();
    for( PHY_RoleInterface_Perceiver::CIT_SurfaceObjectMap itSurface = surfaces.begin(); itSurface != surfaces.end(); ++itSurface )
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

            if ( perceiver_.GetKnowledgeGroup().IsPerceptionDistanceHacked( object ) )
                perceiver_.NotifyPerception( object, perceiver_.GetKnowledgeGroup().GetPerceptionLevel( object ) );
            else
                perceiver_.NotifyPerception( object, Compute( object ) );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionView::Compute
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionView::Compute( const MIL_PopulationFlow& flow, T_PointVector& shape ) const
{
    if( !bIsEnabled_ || !flow.CanBePerceived() )
        return PHY_PerceptionLevel::notSeen_;

    const PHY_PerceptionSurfaceAgent* pBestSurface = 0;
          double                    rBestCost    = std::numeric_limits< double >::min();

    const PHY_RoleInterface_Perceiver::T_SurfaceAgentMap& surfaces = perceiver_.GetSurfacesAgent();
    for( PHY_RoleInterface_Perceiver::CIT_SurfaceAgentMap itSurface = surfaces.begin(); itSurface != surfaces.end(); ++itSurface )
    {
        const PHY_PerceptionSurfaceAgent& surface = itSurface->second;
        const double                    rCost   = surface.ComputePerceptionAccuracy( perceiver_, flow );
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
void PHY_PerceptionView::Execute( const TER_PopulationFlow_ABC::T_ConstPopulationFlowVector& perceivableFlows )
{
    if( bIsEnabled_ )
    {
        for( TER_PopulationFlow_ABC::T_ConstPopulationFlowVector::const_iterator it = perceivableFlows.begin(); it != perceivableFlows.end(); ++it )
        {
            MIL_PopulationFlow& flow = const_cast< MIL_PopulationFlow& >( static_cast< const MIL_PopulationFlow& >( **it ) ); // $$$ RC LDC Should propagate constness to called methods instead

            T_PointVector shape;
            const PHY_PerceptionLevel& level = Compute( flow, shape );

            if ( perceiver_.GetKnowledgeGroup().IsPerceptionDistanceHacked( flow.GetPopulation() ) )
                perceiver_.NotifyPerception( flow, perceiver_.GetKnowledgeGroup().GetPerceptionLevel( flow.GetPopulation()), shape );
            else
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
    if( !bIsEnabled_ || !target.CanBePerceived() )
        return PHY_PerceptionLevel::notSeen_;

    if( perceiver_.IsIdentified( target ) )
        return PHY_PerceptionLevel::identified_;

    const PHY_PerceptionLevel* pBestLevel = &PHY_PerceptionLevel::notSeen_;
    const PHY_RoleInterface_Perceiver::T_SurfaceAgentMap& surfaces = perceiver_.GetSurfacesAgent();
    for( PHY_RoleInterface_Perceiver::CIT_SurfaceAgentMap itSurface = surfaces.begin(); itSurface != surfaces.end(); ++itSurface )
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
void PHY_PerceptionView::Execute( const TER_PopulationConcentration_ABC::T_ConstPopulationConcentrationVector perceivableConcentrations )
{
    if( bIsEnabled_ )
        for( TER_PopulationConcentration_ABC::T_ConstPopulationConcentrationVector::const_iterator it = perceivableConcentrations.begin(); it != perceivableConcentrations.end(); ++it )
        {
            MIL_PopulationConcentration& concentration = const_cast< MIL_PopulationConcentration& >( static_cast< const MIL_PopulationConcentration& >( **it ) ); // $$$ RC LDC Should propagate constness to called methods instead

            if ( perceiver_.GetKnowledgeGroup().IsPerceptionDistanceHacked( concentration.GetPopulation() ) )
                perceiver_.NotifyPerception( concentration, perceiver_.GetKnowledgeGroup().GetPerceptionLevel( concentration.GetPopulation() ) );
            else
                perceiver_.NotifyPerception( concentration, Compute( concentration ) );
        }
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionView::Enable
// Created: JVT 2005-04-29
// -----------------------------------------------------------------------------
void PHY_PerceptionView::Enable()
{
    bIsEnabled_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionView::Disable
// Created: JVT 2005-04-29
// -----------------------------------------------------------------------------
void PHY_PerceptionView::Disable()
{
    bIsEnabled_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionView::FinalizePerception
// Created: LDC 2010-05-05
// -----------------------------------------------------------------------------
void PHY_PerceptionView::FinalizePerception()
{
    if( !perceiver_.GetPion().GetRole< PHY_RoleInterface_UrbanLocation >().IsInCity() )
    {
        const PHY_RoleInterface_Perceiver::T_SurfaceAgentMap& surfaces = perceiver_.GetSurfacesAgent();
        for( PHY_RoleInterface_Perceiver::CIT_SurfaceAgentMap itSurface = surfaces.begin(); itSurface != surfaces.end(); ++itSurface )
            const_cast< PHY_PerceptionSurfaceAgent& >( itSurface->second ).FinalizePerception();
    }
    else
    {
        perceptionsUnderway_ = perceptionsBuffer_;
        perceptionsBuffer_.clear();
    }

}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionView::TransfertPerception
// Created: SLG 2010-05-07
// -----------------------------------------------------------------------------
void PHY_PerceptionView::TransfertPerception()
{
    bool isInCity = perceiver_.GetPion().GetRole< PHY_RoleInterface_UrbanLocation >().IsInCity();
    if( isInCity && !wasInCity_ )
    {
        perceptionsBuffer_.clear();
        perceptionsUnderway_.clear();
        const PHY_RoleInterface_Perceiver::T_SurfaceAgentMap& surfaces = perceiver_.GetSurfacesAgent();
        for( PHY_RoleInterface_Perceiver::CIT_SurfaceAgentMap itSurface = surfaces.begin(); itSurface != surfaces.end(); ++itSurface )
        {
            std::map< const void*, unsigned int > perceptionMap = itSurface->second.GetTargetsPerception();
            for( std::map< const void*, unsigned int >::const_iterator it = perceptionMap.begin(); it != perceptionMap.end(); ++it )
            {
                T_PerceptionTickMap::iterator it2 = perceptionsBuffer_.find( it->first );
                if( it2 != perceptionsBuffer_.end() )
                    perceptionsBuffer_[ it2->first ] = std::pair< unsigned int, double >( std::max( it2->second.first, it->second ), MIL_Random::rand_ii( MIL_Random::ePerception )  );
                else
                    perceptionsBuffer_[ it->first ] = std::pair< unsigned int, double >( it->second, MIL_Random::rand_ii( MIL_Random::ePerception ) );
            }
        }

        wasInCity_ = true;
    }
    else if( !isInCity && wasInCity_ )
    {
        const PHY_RoleInterface_Perceiver::T_SurfaceAgentMap& surfaces = perceiver_.GetSurfacesAgent();
        for( PHY_RoleInterface_Perceiver::IT_SurfaceAgentMap itSurface = surfaces.begin(); itSurface != surfaces.end(); ++itSurface )
            itSurface->second.TransfertPerception( perceptionsUnderway_ );
        wasInCity_ = false;
    }

}
