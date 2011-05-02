// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_KnowledgeFunctions.inl $
// $Author: Nld $
// $Modtime: 11/05/05 16:04 $
// $Revision: 6 $
// $Workfile: DEC_KnowledgeFunctions.inl $
//
// *****************************************************************************

#include "MIL_AgentServer.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/MIL_Army_ABC.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Objects/MIL_ObjectFilter.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Automate.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "Knowledge/DEC_KS_Sharing.h"

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetLivingEnemiesPerceivedByPion
// Created: NLD 2006-04-18
// -----------------------------------------------------------------------------
template< typename T > 
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetLivingEnemiesPerceivedByPion( const T& caller, const DEC_Decision_ABC* perceiver )
{
    assert( perceiver );
    const MIL_AgentPion& source = perceiver->GetPion();
    T_ConstKnowledgeAgentVector sourceKnowledge;
    source.GetKnowledge().GetLivingEnemiesPerceived( sourceKnowledge );
    T_ConstKnowledgeAgentVector results;
    caller.GetKnowledgeGroup().GetKnowledge().TranslateKnowledges( sourceKnowledge, source.GetKnowledgeGroup(), results );
    return results;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::ShareKnowledgesWith
// Created: NLD 2004-11-08
// -----------------------------------------------------------------------------
template< typename T > 
void DEC_KnowledgeFunctions::ShareKnowledgesWith( const T& caller, DEC_Decision_ABC* receiver, float minutes )
{
    assert( receiver );
    const unsigned int sharingTimeStep = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() + unsigned int( MIL_Tools::ConvertMinutesToSim( minutes ) );
    receiver->GetAutomate().GetKnowledgeGroup().GetKnowledge().GetKsSharing().ShareFromSource( caller.GetKnowledgeGroup(), sharingTimeStep );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::ShareKnowledgesInZoneWith
// Created: NLD 2004-11-08
// -----------------------------------------------------------------------------
template< typename T > 
void DEC_KnowledgeFunctions::ShareKnowledgesInZoneWith( const T& caller, DEC_Decision_ABC* receiver, const MT_Vector2D* center, float radius )
{
    assert( receiver );
    receiver->GetAutomate().GetKnowledgeGroup().GetKnowledge().GetKsSharing().ShareFromSource( caller.GetKnowledgeGroup(), MIL_AgentServer::GetWorkspace().GetCurrentTimeStep(), *center, MIL_Tools::ConvertMeterToSim( radius ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetObjectsInCircle
// Created: NLD 2006-05-05
// -----------------------------------------------------------------------------
template< typename T > 
T_KnowledgeObjectDiaIDVector DEC_KnowledgeFunctions::GetObjectsInCircle( const T& caller, const MT_Vector2D* pCenter, double rRadius, const std::vector< std::string >& filters )
{
    assert( pCenter );
    MIL_ObjectFilter filter( filters );
    T_KnowledgeObjectDiaIDVector knowledges;
    caller.GetArmy().GetKnowledge().GetObjectsInCircle( knowledges, filter, *pCenter, rRadius );
    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetObjectsInZone
// Created: NLD 2006-05-05
// -----------------------------------------------------------------------------
template< typename T > 
T_KnowledgeObjectDiaIDVector DEC_KnowledgeFunctions::GetObjectsInZone( const T& caller, const TER_Localisation* pLoc, const std::vector< std::string >& parameters )
{
    assert( pLoc );
    MIL_ObjectFilter filter( parameters );
    T_KnowledgeObjectDiaIDVector knowledges;
    caller.GetArmy().GetKnowledge().GetObjectsInZone( knowledges, filter, *pLoc );
    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetObjectsInFuseau
// Created: NLD 2006-05-05
// -----------------------------------------------------------------------------
template< typename T > 
T_KnowledgeObjectDiaIDVector DEC_KnowledgeFunctions::GetObjectsInFuseau( const T& caller, const std::string& type )
{
    std::vector< std::string > types;
    types.push_back( type );
    MIL_ObjectFilter filter( types );
    T_KnowledgeObjectDiaIDVector results;
    caller.GetArmy().GetKnowledge().GetObjectsInZone( results, filter, caller.GetOrderManager().GetFuseau() );
    return results;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetFriendsInZone
// Created: NLD 2005-05-11
// -----------------------------------------------------------------------------
template< typename T > 
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetFriendsInZone( const T& caller, const TER_Localisation* location )
{
    assert( location );
    T_ConstKnowledgeAgentVector results;
    caller.GetKnowledgeGroup().GetKnowledge().GetFriendsInZone( results, *location );
    return results;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetPopulations
// Created: NLD 2005-12-02
// -----------------------------------------------------------------------------
template< typename T > 
std::vector< unsigned int > DEC_KnowledgeFunctions::GetPopulations( const T& caller )
{
    std::vector< unsigned int > results;
    caller.GetKnowledgeGroup().GetKnowledge().GetPopulations( results );
    return results;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::ComputeEnemiesRatio
// Created: NLD 2007-04-19
// -----------------------------------------------------------------------------
template< typename T, typename B >
float DEC_KnowledgeFunctions::ComputeEnemiesRatio( const T& caller, const B& boundaries, bool unloaded )
{
    // $$$$ _RC_ JSR 2011-01-28: Semble non utilisé. à vérifier et supprimer? surchargé par T = MIL_Automate dans le cpp
    unsigned int nNbrTotalLivingEnemies = 0;
    unsigned int nNbrSelectedLivingEnemies = 0;
    const T_KnowledgeAgentVector& enemies = caller.GetKnowledgeGroup().GetKnowledge().GetEnemies();
    for( CIT_KnowledgeAgentVector itKnowledgeAgent = enemies.begin(); itKnowledgeAgent != enemies.end(); ++itKnowledgeAgent )
    {
        boost::shared_ptr< DEC_Knowledge_Agent > knowledge = **itKnowledgeAgent;
        if( knowledge->IsValid() && !knowledge->IsDead() && boundaries.IsInside( knowledge->GetPosition() ) )
        {
            ++ nNbrTotalLivingEnemies;
            if( knowledge->IsHuman() == unloaded )
                ++ nNbrSelectedLivingEnemies;
        }
    }
    if( nNbrTotalLivingEnemies == 0 )
        return 0.;
    return static_cast< float >( nNbrSelectedLivingEnemies ) / nNbrTotalLivingEnemies;
}

namespace 
{
    template< typename Entity, typename Area >
    struct CompareBoundariesEnemies
    {
        CompareBoundariesEnemies( const Entity& caller, bool unloaded )
            : pCaller_( &caller ), unloaded_( unloaded ) {}
        bool operator()( boost::shared_ptr< Area > area1, boost::shared_ptr< Area > area2 )
        {
            return DEC_KnowledgeFunctions::ComputeEnemiesRatio( *pCaller_, *area1, unloaded_ )
                 < DEC_KnowledgeFunctions::ComputeEnemiesRatio( *pCaller_, *area2, unloaded_ );
        }
        const Entity* pCaller_;
        bool unloaded_;
    };    
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::SortAccordingToUnloadedEnemies
// Created: NLD 2007-04-19
// -----------------------------------------------------------------------------
template< typename T > 
std::vector< boost::shared_ptr< TER_Localisation > > DEC_KnowledgeFunctions::SortAccordingToUnloadedEnemies( const T& caller, const std::vector< boost::shared_ptr< TER_Localisation > >& locations )
{
    std::vector< boost::shared_ptr< TER_Localisation > > result( locations );
    std::sort( result.begin(), result.end(), CompareBoundariesEnemies< T, TER_Localisation >( caller, true ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::SortAccordingToLoadedEnemies
// Created: AGE 2007-10-16
// -----------------------------------------------------------------------------
template< typename T >
std::vector< boost::shared_ptr< TER_Localisation > > DEC_KnowledgeFunctions::SortAccordingToLoadedEnemies( const T& caller, const std::vector< boost::shared_ptr< TER_Localisation > >& locations )
{
    std::vector< boost::shared_ptr< TER_Localisation > > result( locations );    
    std::sort( result.begin(), result.end(), CompareBoundariesEnemies< T, TER_Localisation >( caller, false ) );
    return result;
}
