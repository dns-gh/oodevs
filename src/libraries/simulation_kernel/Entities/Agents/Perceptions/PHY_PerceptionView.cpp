//*****************************************************************************
// Created: DFT 02-02-26
//*****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_PerceptionView.h"
#include "DetectionComputer.h"
#include "MIL_Random.h"
#include "PHY_PerceptionLevel.h"
#include "PHY_ZURBPerceptionComputer.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"
#include "Entities/Agents/Roles/Urban/PHY_RoleInterface_UrbanLocation.h"
#include "Entities/Agents/Units/Sensors/PHY_SensorTypeDisaster.h"
#include "Entities/Objects/DisasterCapacity.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Objects/MIL_ObjectManipulator_ABC.h"
#include "Entities/Orders/MIL_Report.h"
#include "Entities/Populations/MIL_PopulationFlow.h"
#include "Entities/Populations/MIL_PopulationConcentration.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Urban/MIL_UrbanObject_ABC.h"

unsigned int PHY_PerceptionView::nUrbanCoefficient_ = 100;

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionView constructor
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
PHY_PerceptionView::PHY_PerceptionView( PHY_RoleInterface_Perceiver& perceiver, bool inCity )
    : PHY_Perception_ABC( perceiver )
    , bIsEnabled_       ( true )
    , wasInCity_        ( inCity )
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
    if( !bIsEnabled_ )
        return *pBestLevel;
    const auto& surfaces = perceiver_.GetSurfacesAgent();
    for( auto it = surfaces.begin(); it != surfaces.end(); ++it )
    {
        const auto& currentLevel = it->second.ComputePerception( perceiver_, vPoint );
        if( currentLevel > *pBestLevel )
        {
            pBestLevel = &currentLevel;
            if( pBestLevel->IsBestLevel() )
                return *pBestLevel;
        }
    }
    return *pBestLevel;
}

PHY_PerceptionView::T_PerceptionParameterPair PHY_PerceptionView::GetParameter( const MIL_Agent_ABC& target )
{
    unsigned int tick = 0;
    double roll = 0;
    auto it = perceptionsUnderway_.find( &target );
    if( it != perceptionsUnderway_.end() )
    {
        tick = it->second.first;
        if( it->second.second == -1 )
            it->second.second = MIL_Random::rand_ii( MIL_Random::ePerception );
        roll = it->second.second;
    }
    else
        roll = MIL_Random::rand_ii( MIL_Random::ePerception );
    return T_PerceptionParameterPair( tick, roll );
}

namespace
{
    const PHY_PerceptionLevel& ComputePerception( const PHY_RoleInterface_Perceiver& perceiver, const MIL_Agent_ABC& target )
    {
        const PHY_PerceptionLevel* pBestLevel = &PHY_PerceptionLevel::notSeen_;
        const auto& surfaces = perceiver.GetSurfacesAgent();
        for( auto itSurface = surfaces.begin(); itSurface != surfaces.end(); ++itSurface )
        {
            const PHY_PerceptionLevel& currentLevel = itSurface->second.ComputePerception( perceiver, target );
            if( currentLevel > *pBestLevel )
            {
                pBestLevel = &currentLevel;
                if( pBestLevel->IsBestLevel() )
                    return *pBestLevel;
            }
        }
        return *pBestLevel;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionView::Compute
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionView::Compute( const MIL_Agent_ABC& target )
{
    if( target.IsMarkedForDestruction() )
        return PHY_PerceptionLevel::notSeen_;

    TransfertPerception();
    if( target.BelongsTo( *perceiver_.GetKnowledgeGroup() ) ||
        perceiver_.IsIdentified( target ) )
        return PHY_PerceptionLevel::identified_;

    if( !bIsEnabled_ )
        return PHY_PerceptionLevel::notSeen_;

    if( FailDirectView( target ) )
        return PHY_PerceptionLevel::notSeen_;

    const PHY_PerceptionLevel& result = ComputePerception( perceiver_, target );
    if( result == PHY_PerceptionLevel::notSeen_ ||
        !perceiver_.GetPion().GetRole< PHY_RoleInterface_UrbanLocation >().IsInCity() )
        return result;

    const T_PerceptionParameterPair p = GetParameter( target );
    perceptionsBuffer_[ &target ] = std::make_pair( p.first + 1, p.second );
    const PHY_ZURBPerceptionComputer urbanComputer( perceiver_.GetPion(), p.second, p.first );
    const PHY_PerceptionLevel& urbanResult = urbanComputer.ComputePerception( target );
    return std::min( result, urbanResult );
}

bool PHY_PerceptionView::FailDirectView( const MIL_Agent_ABC& target ) const
{
    const auto perceiverBlock = perceiver_.GetPion().GetRole< PHY_RoleInterface_UrbanLocation >().GetCurrentUrbanBlock();
    const auto targetBlock = target.GetRole< PHY_RoleInterface_UrbanLocation >().GetCurrentUrbanBlock();
    if( ( !perceiverBlock || perceiverBlock->GetStructuralState() <= 0 ) &&
        ( !targetBlock || targetBlock->GetStructuralState() <= 0 ) )
        return false;
    if( !perceiver_.GetKnowledgeGroup()->GetKnowledge()->ResolveKnowledgeAgent( target ) )
        return false;
    return MIL_Random::rand32_ii( 0, 100, MIL_Random::ePerception ) > nUrbanCoefficient_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionView::Execute
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
void PHY_PerceptionView::Execute( const TER_Agent_ABC::T_AgentPtrVector& perceivableAgents )
{
    if( !bIsEnabled_ )
        return;
    bool civiliansEncountered = false;
    for( auto itAgent = perceivableAgents.begin(); itAgent != perceivableAgents.end(); ++itAgent )
    {
        MIL_Agent_ABC& agent = static_cast< PHY_RoleInterface_Location& >( **itAgent ).GetAgent();
        if( !DEC_Knowledge_Agent::detectDestroyedUnits_ && agent.IsDead() )
            continue;
        if( agent.BelongsTo( *perceiver_.GetKnowledgeGroup() ) && !agent.IsDead() )
            continue;
        if( agent.IsMarkedForDestruction() )
        {
            perceiver_.NotifyPerception( agent, PHY_PerceptionLevel::notSeen_ );
            continue;
        }
        detection::DetectionComputer detectionComputer( agent );
        perceiver_.GetPion().Execute( detectionComputer );
        agent.Execute( detectionComputer );
        const bool hacked = perceiver_.GetKnowledgeGroup()->IsPerceptionDistanceHacked( agent );
        if( !hacked && !detectionComputer.CanBeSeen() )
            continue;
        const auto perceived = perceiver_.NotifyPerception( agent, Compute( agent ) );
        if( perceived && !hacked && !perceiver_.IsKnown( agent ) )
            civiliansEncountered |= agent.IsCivilian();
    }
    if( civiliansEncountered )
        MIL_Report::PostEvent( perceiver_.GetPion(), report::eRC_CiviliansEncountered );
}

namespace
{
    template< typename Object, typename Sensors, typename Functor >
    const PHY_PerceptionLevel& ComputeKnowledge( PHY_RoleInterface_Perceiver& perceiver,
            const Object& object, const MT_Vector2D& position, const Sensors& sensors,
            Functor perceptionComputer )
    {
        const PHY_PerceptionLevel* pBestLevel = &PHY_PerceptionLevel::notSeen_;
        for( auto it = sensors.begin(); it != sensors.end(); ++it )
        {
             const auto& currentLevel = perceptionComputer( it, position, object, perceiver );
             if( currentLevel > *pBestLevel )
             {
                pBestLevel = &currentLevel;
                if( pBestLevel->IsBestLevel() )
                    return *pBestLevel;
            }
        }
        return *pBestLevel;
    }

    template< typename Sensors, typename Functor >
    const PHY_PerceptionLevel& Compute( PHY_RoleInterface_Perceiver& perceiver,
            const MIL_Object_ABC& object, const Sensors& sensors,
            const MT_Vector2D& position, Functor perceptionComputer )
    {
        if( !object().CanBePerceived() )
            return PHY_PerceptionLevel::notSeen_;
        if( object.IsUniversal() )
            return PHY_PerceptionLevel::identified_;
        if( perceiver.IsIdentified( object ) )
            return PHY_PerceptionLevel::identified_;
        return ComputeKnowledge( perceiver, object, position, sensors, perceptionComputer );
    }

    const PHY_PerceptionLevel& ComputeKnowledgeObject(
         PHY_RoleInterface_Perceiver::T_SurfaceObjectMap::const_iterator it,
         const DEC_Knowledge_Object& knowledge, PHY_RoleInterface_Perceiver& perceiver )
    {
        return it->second.ComputePerception( perceiver, knowledge );
    }

    const PHY_PerceptionLevel& ComputeObject(
            PHY_RoleInterface_Perceiver::T_SurfaceObjectMap::const_iterator it,
            const MIL_Object_ABC& object, PHY_RoleInterface_Perceiver& perceiver )
    {
        return it->second.ComputePerception( perceiver, object );
    }

    const PHY_PerceptionLevel& ComputeKnowledgeDisaster(
            PHY_RoleInterface_Perceiver::T_DisasterDetectors::const_iterator it,
            const MT_Vector2D& position, const DEC_Knowledge_Object& knowledge )
     {
         return (*it)->ComputePerception( position, knowledge );
     }

    const PHY_PerceptionLevel& ComputeDisaster(
            PHY_RoleInterface_Perceiver::T_DisasterDetectors::const_iterator it,
            const MT_Vector2D& position, const MIL_Object_ABC& object )
    {
        return (*it)->ComputePerception( position, object );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionView::Compute
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionView::Compute( const DEC_Knowledge_Object& knowledge ) const
{
    if( !bIsEnabled_ )
        return PHY_PerceptionLevel::notSeen_;
    const auto& location = perceiver_.GetPion().GetRole< PHY_RoleInterface_Location >();
    const auto position = location.GetPosition();
    const PHY_PerceptionLevel& objectLevel = ::ComputeKnowledge(
            perceiver_, knowledge, position, perceiver_.GetSurfacesObject(),
            boost::bind( &ComputeKnowledgeObject, _1, _3, _4 ) );
    if( objectLevel.IsBestLevel() )
        return objectLevel;
    const PHY_PerceptionLevel& disasterLevel = ::ComputeKnowledge(
            perceiver_, knowledge, position, perceiver_.GetDisasterDetectors(),
            boost::bind( &ComputeKnowledgeDisaster, _1, _2, _3 ) );
    return disasterLevel > objectLevel ? disasterLevel : objectLevel;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionView::Execute
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_PerceptionView::Execute( const TER_Object_ABC::T_ObjectVector& perceivableObjects )
{
    if( !bIsEnabled_ )
        return;
    const auto& location = perceiver_.GetPion().GetRole< PHY_RoleInterface_Location >();
    const MT_Vector2D position = location.GetPosition();
    for( auto it = perceivableObjects.begin(); it != perceivableObjects.end(); ++it )
    {
        MIL_Object_ABC& object = static_cast< MIL_Object_ABC& >( **it );
        if( perceiver_.GetKnowledgeGroup()->IsPerceptionDistanceHacked( object ) )
        {
            if( object.IsMarkedForDestruction() )
                perceiver_.NotifyPerception( object, PHY_PerceptionLevel::notSeen_ );
            else
                perceiver_.NotifyPerception( object,
                        perceiver_.GetKnowledgeGroup()->GetPerceptionLevel( object ) );
        }
        else
            perceiver_.NotifyPerception( object, ::Compute(
                perceiver_, object, perceiver_.GetSurfacesObject(), position,
                boost::bind( &ComputeObject, _1, _3, _4 ) ) );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionView::ExecuteCollisions
// Created: LGY 2013-01-22
// -----------------------------------------------------------------------------
void PHY_PerceptionView::ExecuteCollisions( const TER_Object_ABC::T_ObjectVector& perceivableObjects )
{
    if( !bIsEnabled_ )
        return;
    const auto& location = perceiver_.GetPion().GetRole< PHY_RoleInterface_Location >();
    const MT_Vector2D position = location.GetPosition();
    for( auto it = perceivableObjects.begin(); it != perceivableObjects.end(); ++it )
    {
        MIL_Object_ABC& object = static_cast< MIL_Object_ABC& >( **it );
        // disaster detection
        if( !object.Retrieve< DisasterCapacity >() || !object.IsInside( position ) )
            continue;
        const PHY_PerceptionLevel& level = ::Compute(
                perceiver_, object, perceiver_.GetDisasterDetectors(),
                position, boost::bind( &ComputeDisaster, _1, _2, _3 ) );
        if( level > PHY_PerceptionLevel::notSeen_ )
            perceiver_.NotifyPerception( object, position );
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
    double rBestCost = std::numeric_limits< double >::min();

    const auto& surfaces = perceiver_.GetSurfacesAgent();
    for( auto itSurface = surfaces.begin(); itSurface != surfaces.end(); ++itSurface )
    {
        const PHY_PerceptionSurfaceAgent& surface = itSurface->second;
        const double rCost = surface.ComputePerceptionAccuracy( perceiver_, flow );
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
    if( !bIsEnabled_ )
        return;
    bool civiliansEncountered = false;
    for( auto it = perceivableFlows.begin(); it != perceivableFlows.end(); ++it )
    { 
        // $$$ RC LDC Should propagate constness to called methods instead
        MIL_PopulationFlow& flow = const_cast< MIL_PopulationFlow& >(
                static_cast< const MIL_PopulationFlow& >( **it ) );

        T_PointVector shape;
        const PHY_PerceptionLevel& level = Compute( flow, shape );

        bool mustReport = false;
        if ( perceiver_.GetKnowledgeGroup()->IsPerceptionDistanceHacked( flow.GetPopulation() ) )
            mustReport = perceiver_.NotifyPerception( flow,
                perceiver_.GetKnowledgeGroup()->GetPerceptionLevel( flow.GetPopulation() ),
                shape );
        else
            mustReport = perceiver_.NotifyPerception( flow, level, shape );
        civiliansEncountered |= mustReport;
    }
    if( civiliansEncountered )
        MIL_Report::PostEvent( perceiver_.GetPion(), report::eRC_CiviliansEncountered );
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
    const auto& surfaces = perceiver_.GetSurfacesAgent();
    for( auto itSurface = surfaces.begin(); itSurface != surfaces.end(); ++itSurface )
    {
        const auto& currentLevel = itSurface->second.ComputePerception( perceiver_, target );
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
    if( !bIsEnabled_ )
        return;
    bool civiliansEncountered = false;
    for( auto it = perceivableConcentrations.begin(); it != perceivableConcentrations.end(); ++it )
    {
        // $$$ RC LDC Should propagate constness to called methods instead
        auto& concentration = const_cast< MIL_PopulationConcentration& >(
                static_cast< const MIL_PopulationConcentration& >( **it ) );

        auto& pop = concentration.GetPopulation();
        bool mustReport = false;
        if ( perceiver_.GetKnowledgeGroup()->IsPerceptionDistanceHacked( pop ) )
            mustReport = perceiver_.NotifyPerception( concentration,
                perceiver_.GetKnowledgeGroup()->GetPerceptionLevel( pop ));
        else
            mustReport = perceiver_.NotifyPerception( concentration, Compute( concentration ) );
        civiliansEncountered |= mustReport;
    }
    if( civiliansEncountered )
        MIL_Report::PostEvent( perceiver_.GetPion(), report::eRC_CiviliansEncountered );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionView::Enable
// Created: JVT 2005-04-29
// -----------------------------------------------------------------------------
void PHY_PerceptionView::Enable( bool enable )
{
    bIsEnabled_ = enable;
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
        for( auto it = surfaces.begin(); it != surfaces.end(); ++it )
            const_cast< PHY_PerceptionSurfaceAgent& >( it->second ).FinalizePerception();
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
        for( auto itSurface = surfaces.begin(); itSurface != surfaces.end(); ++itSurface )
        {
            PHY_PerceptionSurfaceAgent::T_PerceptionTickMap perceptions = itSurface->second.GetTargetsPerception();
            for( auto it = perceptions.begin(); it != perceptions.end(); ++it )
            {
                unsigned int& tick = perceptionsBuffer_[ it->first ].first;
                tick = std::max( tick, it->second );
                perceptionsBuffer_[ it->first ].second = -1;
            }
        }
        wasInCity_ = true;
    }
    else if( !isInCity && wasInCity_ )
    {
        PHY_PerceptionSurfaceAgent::T_PerceptionTickMap perceptions;
        for( auto it = perceptionsUnderway_.begin(); it != perceptionsUnderway_.end(); ++it )
            perceptions[ it->first ] = it->second.first;
        const PHY_RoleInterface_Perceiver::T_SurfaceAgentMap& surfaces = perceiver_.GetSurfacesAgent();
        for( auto itSurface = surfaces.begin(); itSurface != surfaces.end(); ++itSurface )
            itSurface->second.TransfertPerception( perceptions );
        wasInCity_ = false;
    }
}
