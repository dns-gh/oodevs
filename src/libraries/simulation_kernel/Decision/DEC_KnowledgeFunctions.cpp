// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_KnowledgeFunctions.cpp $
// $Author: Nld $
// $Modtime: 23/03/05 18:22 $
// $Revision: 7 $
// $Workfile: DEC_KnowledgeFunctions.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_KnowledgeFunctions.h"
#include "Brain.h"
#include "Decision/DEC_Decision_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/MIL_Army_ABC.h"
#include "Entities/MIL_Army.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Objects/MIL_ObjectFilter.h"
#include "Entities/Populations/MIL_Population.h"
#include "Urban/MIL_UrbanObject_ABC.h"
#include "Entities/Orders/MIL_Fuseau.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Entities/Objects/DisasterCapacity.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgeObject.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Automate.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/DEC_KS_Sharing.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_Knowledge_Def.h"
#include "MIL_AgentServer.h"
#include "MT_Tools/MT_Logger.h"
#include "Urban/MIL_UrbanCache.h"
#include "Tools/MIL_Tools.h"

void DEC_KnowledgeFunctions::Register( sword::Brain& brain )
{
    brain.RegisterFunction( "_DEC_Connaissances_PartageConnaissancesAvec", &DEC_KnowledgeFunctions::ShareKnowledgesWith );
    brain.RegisterFunction( "_DEC_Connaissances_PartageConnaissancesDansZoneAvec", &DEC_KnowledgeFunctions::ShareKnowledgesInZoneWith );
    brain.RegisterFunction( "_DEC_Knowledges_ObjectsInCircle", &DEC_KnowledgeFunctions::GetObjectsInCircle );
    brain.RegisterFunction( "_DEC_Connaissances_ObjetsDansFuseau", &DEC_KnowledgeFunctions::GetObjectsInFuseau );
    brain.RegisterFunction( "_DEC_Connaissances_UnitesEnnemiesVivantesPercuesParPion", &DEC_KnowledgeFunctions::GetLivingEnemiesPerceivedByPion );
    brain.RegisterFunction( "_DEC_Connaissances_Populations", &DEC_KnowledgeFunctions::GetPopulations );
    brain.RegisterFunction( "_DEC_ObjectKnowledgesInZone", &DEC_KnowledgeFunctions::GetObjectsInZone );
    brain.RegisterFunction( "_DEC_Connaissances_BlocUrbainPourPosition", std::function< const MIL_UrbanObject_ABC*( const DEC_Decision_ABC*, boost::shared_ptr< MT_Vector2D > ) >( &DEC_KnowledgeFunctions::GetUrbanBlockForPosition ) );
    brain.RegisterFunction( "_DEC_Connaissances_BlocUrbainDansZone", &DEC_KnowledgeFunctions::GetUrbanBlockInZone );
    brain.RegisterFunction( "_DEC_Connaissances_BlocUrbainDansOuIntersectentZone", &DEC_KnowledgeFunctions::GetUrbanBlockInOrIntersectZone );
    brain.RegisterFunction( "_DEC_RapportDeForceLocal", &DEC_KnowledgeFunctions::GetRapForLocal );
    brain.RegisterFunction( "_DEC_Connaissances_UnitesEnnemiesDangereuses", &DEC_KnowledgeFunctions::GetDangerousEnemies );
    brain.RegisterFunction( "_DEC_Connaissances_UnitesDetecteesDansFuseau", &DEC_KnowledgeFunctions::GetDetectedAgentsInFuseau );
    brain.RegisterFunction( "_DEC_Connaissances_UnitesDetecteesDansZone",&DEC_KnowledgeFunctions::GetDetectedAgentsInZone );
    brain.RegisterFunction( "_DEC_Connaissances_UnitesPrenantAPartie", &DEC_KnowledgeFunctions::GetAgentsAttacking );
    brain.RegisterFunction( "_DEC_Connaissances_UnitesEnnemiesVivantesPercues", &DEC_KnowledgeFunctions::GetLivingEnemiesPerceived );
    brain.RegisterFunction( "_DEC_Connaissances_UnitesEnnemiesVivantesPercuesDansFuseau", &DEC_KnowledgeFunctions::GetLivingEnemiesPerceivedInFuseau );
    brain.RegisterFunction( "_DEC_Connaissances_UnitesEnnemiesVivantesDansFuseau", &DEC_KnowledgeFunctions::GetLivingEnemiesInFuseau );
    brain.RegisterFunction( "_DEC_Connaissances_UnitesEnnemiesVivantesDansZone", &DEC_KnowledgeFunctions::GetLivingEnemiesInZone );
    brain.RegisterFunction( "_DEC_Connaissances_UnitesBlesseesDansZone", &DEC_KnowledgeFunctions::GetWoundedUnitsInZone );
    brain.RegisterFunction( "_DEC_Connaissances_UnitesCivilesDansZone", &DEC_KnowledgeFunctions::GetCiviliansInZone );
    brain.RegisterFunction( "_DEC_Connaissances_UnitesEnnemiesVivantesDansCercle", &DEC_KnowledgeFunctions::GetLivingEnemiesInCircle );
    brain.RegisterFunction( "_DEC_Connaissances_UnitesBlesseesOuTueesDansCercle", &DEC_KnowledgeFunctions::GetWoundedOrDeadUnitsInCircle );
    brain.RegisterFunction( "_DEC_Connaissances_RefugiesAProximite", &DEC_KnowledgeFunctions::GetNearbyRefugees );
    brain.RegisterFunction( "_DEC_Connaissances_TerroristsAProximite", &DEC_KnowledgeFunctions::GetNearbyTerrorists );
    brain.RegisterFunction( "_DEC_Connaissances_UnitesRenduesAProximite", DEC_KnowledgeFunctions::GetNearbySurrenderedAgents );
    brain.RegisterFunction( "_DEC_Connaissances_PresenceEnnemieDansCercle", &DEC_KnowledgeFunctions::EnemyPresenceInCircle );
    brain.RegisterFunction( "_DEC_Connaissances_UnitesAmiesDansZone",  &DEC_KnowledgeFunctions::GetFriendsInZone );
    brain.RegisterFunction( "_DEC_ObjectKnowledgesIntersectingInZone", &DEC_KnowledgeFunctions::GetObjectsIntersectingInZone );
    brain.RegisterFunction( "_DEC_Connaissances_CollisionsObjets",  &DEC_KnowledgeFunctions::GetObjectsColliding );
    brain.RegisterFunction( "_DEC_Connaissances_CollisionsDesastres", &DEC_KnowledgeFunctions::GetCollidingDisasters );
    brain.RegisterFunction( "_DEC_Connaissances_Desastres", &DEC_KnowledgeFunctions::GetDisasters );
    brain.RegisterFunction( "_DEC_Connaissances_CollisionsObjetsDeType",  &DEC_KnowledgeFunctions::GetObjectsCollidingFromType );
    brain.RegisterFunction( "_DEC_Connaissances_ObjetLePlusProche", &DEC_KnowledgeFunctions::GetClosestObject );
    brain.RegisterFunction( "_DEC_Connaissances_ObjetAmiLePlusProche", &DEC_KnowledgeFunctions::GetClosestFriendObject );
    brain.RegisterFunction( "_DEC_Connaissances_CollisionsPopulations", &DEC_KnowledgeFunctions::GetPopulationsColliding );
    brain.RegisterFunction( "_DEC_Connaissances_PopulationsPrenantAPartie", &DEC_KnowledgeFunctions::GetPopulationsAttacking );
    brain.RegisterFunction( "_DEC_Connaissances_EstPrisAPartieParPopulation", &DEC_KnowledgeFunctions::IsPopulationAttacking );
    brain.RegisterFunction( "DEC_Knowledge_GetUrbanBlocks", &DEC_KnowledgeFunctions::GetUrbanBlocks );
    brain.RegisterFunction( "DEC_Knowledge_GetEnemies", &DEC_KnowledgeFunctions::GetEnemies );
    brain.RegisterFunction( "DEC_Knowledge_GetObjects", &DEC_KnowledgeFunctions::GetObjects );
    brain.RegisterFunction( "DEC_Connaissances_BlocUrbainDansCercle", &DEC_KnowledgeFunctions::GetUrbanBlockInCircle );
    brain.RegisterFunction( "DEC_Connaissances_UnitesDansZone", &DEC_KnowledgeFunctions::GetAllAgentsInZone );
    brain.RegisterFunction( "DEC_Connaissances_UnitesPrenantAPartieUnAmi", &DEC_KnowledgeFunctions::GetEnemiesAttacking );
    brain.RegisterFunction( "DEC_Connaissances_UniteLaPlusProcheDunAmi", &DEC_KnowledgeFunctions::GetNearestToFriend );
    brain.RegisterFunction( "DEC_Connaissances_UnitesEnnemiesDansZone", &DEC_KnowledgeFunctions::GetEnemyAgentsInZone );
    brain.RegisterFunction( "DEC_Connaissances_UnitesPrenantAPartieSurAmi" , &DEC_KnowledgeFunctions::GetAgentsAttackingAlly );
    brain.RegisterFunction( "DEC_ObjectKnowledge_GetObjectsInZone", &DEC_KnowledgeFunctions::GetObjectsWithCapacityInZone );
    brain.RegisterFunction( "DEC_ObjectKnowledge_IsPositionInside", &DEC_KnowledgeFunctions::IsPositionInsideObjectOfType );
    brain.RegisterFunction( "DEC_GetAgentKnowledge", &DEC_KnowledgeFunctions::GetAgentKnowledge );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetAllAgentsInZone
// Created: FAI 2014-04-03
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetAllAgentsInZone( const DEC_Decision_ABC* callerAgent, const TER_Localisation* area )
{
    T_ConstKnowledgeAgentVector knowledges;
    auto bbKg = callerAgent->GetPion().GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
        bbKg->GetAllAgentsInZone( knowledges, *area );
    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetDetectedAgentsInFuseau
// Created: NLD 2006-04-14
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetDetectedAgentsInFuseau( const DEC_Decision_ABC* callerAgent )
{
    T_ConstKnowledgeAgentVector knowledges;
    auto bbKg = callerAgent->GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
        bbKg->GetDetectedAgentsInZone( knowledges, callerAgent->GetOrderManager().GetFuseau() );
    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetDetectedAgentsInZone
// Created: NLD 2006-04-14
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetDetectedAgentsInZone( const DEC_Decision_ABC* callerAgent, const TER_Localisation* area )
{
    if( !area )
        throw MASA_EXCEPTION( "invalid parameter." );
    T_ConstKnowledgeAgentVector knowledges;
    auto bbKg = callerAgent->GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
        bbKg->GetDetectedAgentsInZone( knowledges, *area );
    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetEnemyAgentsInZone
// Created: LDC 2011-06-24
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetEnemyAgentsInZone( const DEC_Decision_ABC* callerAgent, const TER_Localisation* area )
{
    T_ConstKnowledgeAgentVector knowledges;
    if( !callerAgent )
        throw MASA_EXCEPTION( "invalid parameter." );
    auto bbKg = callerAgent->GetPion().GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
        bbKg->GetEnemyAgentsInZone( knowledges, *area );
    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetAgentsAttacking
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetAgentsAttacking( const DEC_Decision_ABC* callerAgent )
{
    T_ConstKnowledgeAgentVector attackers;
    callerAgent->GetPion().GetKnowledge().GetAgentsAttacking( attackers );
    return attackers;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetAgentsAttackingAlly
// Created: DDA 2010-06-02
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector  DEC_KnowledgeFunctions::GetAgentsAttackingAlly( const DEC_Decision_ABC* agentAlly )
{
    if( !agentAlly )
        throw MASA_EXCEPTION( "invalid parameter." );
    return DEC_KnowledgeFunctions::GetAgentsAttacking( agentAlly );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetEnemiesAttacking
// Created: LDC 2011-06-24
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetEnemiesAttacking( const DEC_Decision_ABC* callerAgent )
{
    if( !callerAgent )
        throw MASA_EXCEPTION( "invalid parameter." );
    T_ConstKnowledgeAgentVector attackers;
    std::set< boost::shared_ptr< DEC_Knowledge_Agent > > buffer;
    const std::vector< MIL_Automate* >& allies = callerAgent->GetPion().GetKnowledgeGroup()->GetAutomates();
    for( auto it = allies.begin(); it != allies.end(); ++it )
    {
        if( !(*it) )
            throw MASA_EXCEPTION( "invalid parameter." );
        const std::vector< MIL_AgentPion* >& pions = (*it)->GetPions();
        for( auto pit = pions.begin(); pit != pions.end(); ++pit )
        {
            T_ConstKnowledgeAgentVector enemies = DEC_KnowledgeFunctions::GetAgentsAttacking( &( *pit )->GetDecision() );
            for( auto cit = enemies.begin(); cit != enemies.end(); ++cit )
            {
                if( buffer.insert( *cit ).second )
                    attackers.push_back( *cit );
            }
        }
    }
    return attackers;
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< DEC_Knowledge_Agent > DEC_KnowledgeFunctions::GetNearestToFriend
// Created: LDC 2011-06-24
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Agent > DEC_KnowledgeFunctions::GetNearestToFriend( const DEC_Decision_ABC* callerAgent, T_ConstKnowledgeAgentVector units )
{
    if( !callerAgent )
        throw MASA_EXCEPTION( "invalid parameter." );
    double minSquareDistance = std::numeric_limits< double >::max();
    boost::shared_ptr< DEC_Knowledge_Agent > result;
    const std::vector< MIL_Automate* >& allies = callerAgent->GetPion().GetKnowledgeGroup()->GetAutomates();
    for( std::vector< MIL_Automate* >::const_iterator it = allies.begin(); it != allies.end(); ++it )
    {
        if( !(*it) )
            throw MASA_EXCEPTION( "invalid parameter." );
        const std::vector< MIL_AgentPion* >& pions = (*it)->GetPions();
        for( std::vector< MIL_AgentPion* >::const_iterator pit = pions.begin(); pit != pions.end(); ++pit )
        {
            if( !(*pit) )
                throw MASA_EXCEPTION( "invalid parameter." );
            const MT_Vector2D& allyPosition = (*pit)->Get< PHY_RoleInterface_Location >().GetPosition();
            for( auto uit = units.begin(); uit != units.end(); ++uit )
            {
                if( !(*uit) )
                    throw MASA_EXCEPTION( "invalid parameter." );
                const MT_Vector2D& uPosition = (*uit)->GetPosition();
                double squareDistance = allyPosition.SquareDistance( uPosition );
                if( squareDistance < minSquareDistance )
                {
                    result = *uit;
                    minSquareDistance = squareDistance;
                }
            }
        }
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetDangerousEnemies
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetDangerousEnemies( const DEC_Decision_ABC* callerAgent )
{
    return callerAgent->GetPion().GetKnowledge().GetDangerousEnemies();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetLivingEnemiesPerceived
// Created: NLD 2005-03-23
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetLivingEnemiesPerceived( const DEC_Decision_ABC* callerAgent )
{
    T_ConstKnowledgeAgentVector knowledges;
    callerAgent->GetPion().GetKnowledge().GetLivingEnemiesPerceived( knowledges );
    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetLivingEnemiesPerceivedInFuseau
// Created: NLD 2005-03-23
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetLivingEnemiesPerceivedInFuseau( const DEC_Decision_ABC* callerAgent )
{
    T_ConstKnowledgeAgentVector knowledges;
    callerAgent->GetPion().GetKnowledge().GetLivingEnemiesPerceivedInZone( knowledges, callerAgent->GetOrderManager().GetFuseau() );
    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetLivingEnemiesInZone
// Created: NLD 2006-04-14
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetLivingEnemiesInZone( const DEC_Decision_ABC* callerAgent, TER_Localisation* pZone )
{
    if( !pZone )
        throw MASA_EXCEPTION( "invalid parameter." );
    T_ConstKnowledgeAgentVector knowledges;
    auto bbKg = callerAgent->GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
        bbKg->GetLivingEnemiesInZone( knowledges, *pZone );
    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetWoundedUnitsInZone
// Created: LGY 2013-01-04
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetWoundedUnitsInZone( const DEC_Decision_ABC* callerAgent, TER_Localisation* pZone )
{
    if( !pZone )
        throw MASA_EXCEPTION( "invalid parameter." );
    T_ConstKnowledgeAgentVector knowledges;
    auto bbKg = callerAgent->GetKnowledgeGroup()->GetKnowledge();
    if( bbKg)
        bbKg->GetWoundedUnitsInZone( knowledges, *pZone );
    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetCiviliansInZone
// Created: LGY 2012-12-20
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetCiviliansInZone( const DEC_Decision_ABC* callerAgent, TER_Localisation* pZone )
{
    if( !pZone )
        throw MASA_EXCEPTION( "invalid parameter." );
    T_ConstKnowledgeAgentVector knowledges;
    auto bbKg = callerAgent->GetKnowledgeGroup()->GetKnowledge();
    if( bbKg)
        bbKg->GetCiviliansInZone( knowledges, *pZone );
    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetLivingEnemiesInFuseau
// Created: NLD 2006-04-14
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetLivingEnemiesInFuseau( const DEC_Decision_ABC* callerAgent )
{
    T_ConstKnowledgeAgentVector knowledges;
    auto bbKg = callerAgent->GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
        bbKg->GetLivingEnemiesInZone( knowledges, callerAgent->GetOrderManager().GetFuseau() );
    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetLivingEnemiesInFuseau
// Created: NLD 2006-04-14
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetLivingEnemiesInCircle( const DEC_Decision_ABC* callerAgent, const MT_Vector2D* pCenter, float radius )
{
    if( !pCenter )
        throw MASA_EXCEPTION( "invalid parameter." );
    T_ConstKnowledgeAgentVector knowledges;
    auto bbKg = callerAgent->GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
        bbKg->GetLivingEnemiesInCircle( knowledges, *pCenter, radius );
    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetWoundedOrDeadUnitsInCircle
// Created: LGY 2012-12-10
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetWoundedOrDeadUnitsInCircle( const DEC_Decision_ABC* callerAgent, const MT_Vector2D* pCenter, float radius )
{
    if( !pCenter )
        throw MASA_EXCEPTION( "invalid parameter." );
    T_ConstKnowledgeAgentVector knowledges;
    auto bbKg = callerAgent->GetKnowledgeGroup()->GetKnowledge();
    if( bbKg)
        bbKg->GetWoundedOrDeadUnitsInCircle( knowledges, *pCenter, radius );
    return knowledges;
}

T_KnowledgeObjectVector DEC_KnowledgeFunctions::GetObjects( const DEC_Decision_ABC* agent )
{
    T_KnowledgeObjectVector objects;
    if( auto bbKg = agent->GetKnowledgeGroup()->GetKnowledge() )
        bbKg->GetKnowledgeObjectContainer().GetObjects( objects );
    return objects;
}

T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetEnemies( const DEC_Decision_ABC* agent )
{
    if( auto bbKg = agent->GetKnowledgeGroup()->GetKnowledge() )
        return bbKg->GetEnemies();
    return T_ConstKnowledgeAgentVector();
}

T_UrbanObjectVector DEC_KnowledgeFunctions::GetUrbanBlocks( DEC_Decision_ABC* agent )
{
    T_UrbanObjectVector blocks;
    agent->GetPion().GetArmy().GetKnowledge().GetUrbanObjects( blocks );
    return blocks;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetUrbanBlockInCircle
// Created: DDA 2010-03-16
// -----------------------------------------------------------------------------
T_UrbanObjectVector DEC_KnowledgeFunctions::GetUrbanBlockInCircle( boost::shared_ptr< MT_Vector2D > center, float radius )
{
    if( !center )
        throw MASA_EXCEPTION( "invalid parameter." );
    //Urban
    T_UrbanObjectVector result;
    MIL_AgentServer::GetWorkspace().GetUrbanCache().GetListWithinCircle( *center, radius, result );
    return result;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetUrbanBlockForPosition
// Created: DDA 2011-06-20
// -----------------------------------------------------------------------------
const MIL_UrbanObject_ABC* DEC_KnowledgeFunctions::GetUrbanBlockForPosition( const DEC_Decision_ABC* pion, boost::shared_ptr< MT_Vector2D >& point )
{
    if( !point )
        throw MASA_EXCEPTION( "invalid parameter." );
    T_UrbanObjectVector blocks;
    pion->GetEntity().GetArmy().GetKnowledge().GetUrbanObjects( blocks );
    for( T_UrbanObjectVector::iterator it = blocks.begin(); it != blocks.end(); ++it )
        if( ( *it ) && ( *it )->GetLocalisation().IsInside( *point ) )
            return ( *it );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetUrbanBlockInZone
// Created: BCI 2011-03-01
// -----------------------------------------------------------------------------
T_UrbanObjectVector DEC_KnowledgeFunctions::GetUrbanBlockInZone( const DEC_Decision_ABC* pion, TER_Localisation* pLocalisation )
{
    T_UrbanObjectVector result;
    if( pLocalisation )
    {
        T_UrbanObjectVector blocks;
        pion->GetEntity().GetArmy().GetKnowledge().GetUrbanObjects( blocks );
        for( T_UrbanObjectVector::iterator it = blocks.begin(); it != blocks.end(); ++it )
            if( ( *it ) && pLocalisation->Contains( ( *it )->GetLocalisation() ) )
                result.push_back( *it );
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetUrbanBlockInOrIntersectZone
// Created: GGE 2014-08-04
// -----------------------------------------------------------------------------
T_UrbanObjectVector DEC_KnowledgeFunctions::GetUrbanBlockInOrIntersectZone( const DEC_Decision_ABC* pion, TER_Localisation* pLocalisation )
{
    T_UrbanObjectVector result;
    if( pLocalisation )
    {
        T_UrbanObjectVector blocks;
        pion->GetEntity().GetArmy().GetKnowledge().GetUrbanObjects( blocks );
        for( T_UrbanObjectVector::iterator it = blocks.begin(); it != blocks.end(); ++it )
            if( ( *it ) && pLocalisation->IsIntersecting( ( *it )->GetLocalisation() ) )
                result.push_back( *it );
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetLivingEnemiesPerceivedByPion
// Created: NLD 2006-04-18
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetLivingEnemiesPerceivedByPion( const DEC_Decision_ABC* caller, const DEC_Decision_ABC* perceiver )
{
    if( !perceiver )
        throw MASA_EXCEPTION( "invalid parameter." );
    const MIL_AgentPion& source = perceiver->GetPion();
    T_ConstKnowledgeAgentVector sourceKnowledge;
    source.GetKnowledge().GetLivingEnemiesPerceived( sourceKnowledge );
    T_ConstKnowledgeAgentVector results;
    auto bbKg = caller->GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
    {
        boost::shared_ptr< MIL_KnowledgeGroup > sourceKnowledgeGroup = source.GetKnowledgeGroup();
        bbKg->TranslateKnowledges( sourceKnowledge, sourceKnowledgeGroup, results );
    }
    return results;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetNearbyRefugees
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetNearbyRefugees( const DEC_Decision_ABC* callerAgent, double radius )
{
    T_ConstKnowledgeAgentVector knowledges;
    auto bbKg = callerAgent->GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
        bbKg->GetRefugeesInCircle( knowledges, callerAgent->GetPion().GetRole< PHY_RoleInterface_Location >().GetPosition(), radius );
    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetNearbyTerrorists
// Created: DDA 2011-08-10
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetNearbyTerrorists( const DEC_Decision_ABC* callerAgent, double radius )
{
    T_ConstKnowledgeAgentVector knowledges;
    auto bbKg = callerAgent->GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
        bbKg->GetTerroristsInCircle( knowledges, callerAgent->GetPion().GetRole< PHY_RoleInterface_Location >().GetPosition(), radius );
    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetNearbySurrenderedAgents
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetNearbySurrenderedAgents( const DEC_Decision_ABC* callerAgent, double radius )
{
    T_ConstKnowledgeAgentVector knowledges;
    auto bbKg = callerAgent->GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
        bbKg->GetSurrenderedAgentsInCircle( knowledges, callerAgent->GetPion().GetRole< PHY_RoleInterface_Location >().GetPosition(), radius );
    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::EnemyPresenceInCircle
// Created: NLD 2006-04-14
// -----------------------------------------------------------------------------
bool DEC_KnowledgeFunctions::EnemyPresenceInCircle( const DEC_Decision_ABC* callerAgent, const MT_Vector2D* center, double radius )
{
    if( !center )
        throw MASA_EXCEPTION( "invalid parameter." );
    auto bbKg = callerAgent->GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
        return bbKg->EnemyPresenceInCircle( *center, radius );
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetFriendsInZone
// Created: NLD 2005-05-11
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetFriendsInZone( const DEC_Decision_ABC* caller, const TER_Localisation* location )
{
    if( !location )
        throw MASA_EXCEPTION( "invalid parameter." );
    T_ConstKnowledgeAgentVector results;
    auto bbKg = caller->GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
        bbKg->GetFriendsInZone( results, *location );
    return results;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::ShareKnowledgesWith
// Created: NLD 2004-11-08
// -----------------------------------------------------------------------------
void DEC_KnowledgeFunctions::ShareKnowledgesWith( const DEC_Decision_ABC* caller, DEC_Decision_ABC* receiver, float minutes )
{
    if( !receiver )
        throw MASA_EXCEPTION( "invalid parameter." );        
    auto receiverKg = receiver->GetAutomate().GetKnowledgeGroup();
    auto bbKg = receiverKg->GetKnowledge();
    if( !bbKg )
        return;
    boost::shared_ptr< MIL_KnowledgeGroup > callerKnowledgeGroup = caller->GetKnowledgeGroup();
    if( receiverKg == callerKnowledgeGroup )
        return;
    const unsigned int sharingTimeStep = MIL_Time_ABC::GetTime().GetCurrentTimeStep() + unsigned int( MIL_Tools::ConvertMinutesToSim( minutes ) );
    bbKg->GetKsSharing().ShareFromSource( callerKnowledgeGroup, sharingTimeStep );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::ShareKnowledgesInZoneWith
// Created: NLD 2004-11-08
// -----------------------------------------------------------------------------
void DEC_KnowledgeFunctions::ShareKnowledgesInZoneWith( const DEC_Decision_ABC* caller, DEC_Decision_ABC* receiver, const MT_Vector2D* center, float radius )
{
    if( !receiver )
        throw MASA_EXCEPTION( "invalid parameter." );
    auto receiverKg = receiver->GetAutomate().GetKnowledgeGroup();
    auto bbKg = receiverKg->GetKnowledge();
    if( !bbKg )
        return;
    boost::shared_ptr< MIL_KnowledgeGroup > callerKnowledgeGroup = caller->GetKnowledgeGroup();
    if( receiverKg == callerKnowledgeGroup )
        return;
    bbKg->GetKsSharing().ShareFromSource( callerKnowledgeGroup, MIL_Time_ABC::GetTime().GetCurrentTimeStep(), *center, MIL_Tools::ConvertMeterToSim( radius ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetRapForLocal
// Created: NLD 2004-04-08
// -----------------------------------------------------------------------------
double DEC_KnowledgeFunctions::GetRapForLocal( const DEC_Decision_ABC* callerAgent )
{
    return callerAgent->GetPion().GetKnowledge().GetRapForLocalValue();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetClosestObject
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_KnowledgeFunctions::GetClosestObject( const DEC_Decision_ABC* callerAgent, const std::string& type )
{
    std::vector< std::string > typeList;
    typeList.push_back( type );
    const MIL_ObjectFilter filter( typeList );
    if( DEC_BlackBoard_CanContainKnowledgeObject* container = callerAgent->GetKnowledgeGroup()->GetKnowledgeObjectContainer() )
        return container->GetClosestObject( callerAgent->GetPion().GetRole< PHY_RoleInterface_Location >().GetPosition(), filter );
    return boost::shared_ptr< DEC_Knowledge_Object >();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetClosestFriendObject
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_KnowledgeFunctions::GetClosestFriendObject( const DEC_Decision_ABC* callerAgent, const std::string& type )
{
    std::vector< std::string > typeList;
    typeList.push_back( type );
    const MIL_ObjectFilter filter( typeList );
    MIL_AgentPion& agent = callerAgent->GetPion();
    if( DEC_BlackBoard_CanContainKnowledgeObject* container = agent.GetKnowledgeGroup()->GetKnowledgeObjectContainer() )
        return container->GetClosestFriendObject( agent.GetRole< PHY_RoleInterface_Location >().GetPosition(), filter, agent.GetArmy() );
    return boost::shared_ptr< DEC_Knowledge_Object >();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetObjectsColliding
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
T_KnowledgeObjectDiaIDVector DEC_KnowledgeFunctions::GetObjectsColliding( const DEC_Decision_ABC* callerAgent )
{
    T_KnowledgeObjectDiaIDVector objectsColliding;
    callerAgent->GetPion().GetKnowledge().GetObjectsColliding( objectsColliding );
    return objectsColliding;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetObjectsCollidingFromType
// Created: GGE 2012-03-06
// -----------------------------------------------------------------------------
T_KnowledgeObjectDiaIDVector DEC_KnowledgeFunctions::GetObjectsCollidingFromType( const DEC_Decision_ABC* callerAgent, const std::string& objectType )
{
    T_KnowledgeObjectDiaIDVector result;
    T_KnowledgeObjectDiaIDVector objectsColliding;
    callerAgent->GetPion().GetKnowledge().GetObjectsColliding( objectsColliding );
    for( auto it = objectsColliding.begin(); it != objectsColliding.end(); ++it )
    {
        if( *it )
        {
            const MIL_ObjectType_ABC& type = (*it)->GetType();
            if( type.GetName() == objectType )
                result.push_back( *it );
        }
    }

    return result;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetCollidingDisasters
// Created: LGY 2012-11-27
// -----------------------------------------------------------------------------
T_KnowledgeObjectDiaIDVector DEC_KnowledgeFunctions::GetCollidingDisasters( const DEC_Decision_ABC* callerAgent )
{
    T_KnowledgeObjectDiaIDVector results;
    T_KnowledgeObjectDiaIDVector disasters = GetDisasters( callerAgent );

    for( auto it = disasters.begin(); it != disasters.end(); ++it )
        if( *it && (*it)->GetLocalisation().IsInside( callerAgent->GetPion().GetRole< PHY_RoleInterface_Location >().GetPosition() ) )
            results.push_back( *it );
    return results;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetDisasters
// Created: LGY 2013-01-03
// -----------------------------------------------------------------------------
T_KnowledgeObjectDiaIDVector DEC_KnowledgeFunctions::GetDisasters( const DEC_Decision_ABC* callerAgent )
{
    T_KnowledgeObjectDiaIDVector disasters;
    T_KnowledgeObjectDiaIDVector objects;
    if( DEC_BlackBoard_CanContainKnowledgeObject* container = callerAgent->GetKnowledgeGroup()->GetKnowledgeObjectContainer() )
        container->GetObjects( objects );
    for( auto it = objects.begin(); it != objects.end(); ++it )
        if( *it )
        {
            const MIL_ObjectType_ABC& type = ( *it )->GetType();
            if( type.GetCapacity< DisasterCapacity >() )
                disasters.push_back( *it );
        }
    return disasters;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetObjectsInCircle
// Created: NLD 2006-05-05
// -----------------------------------------------------------------------------
T_KnowledgeObjectDiaIDVector DEC_KnowledgeFunctions::GetObjectsInCircle( const DEC_Decision_ABC* caller, const MT_Vector2D* pCenter, double rRadius, const std::vector< std::string >& filters, bool nonActivatedObstacles )
{
    if( !pCenter )
        throw MASA_EXCEPTION( "invalid parameter." );
    MIL_ObjectFilter filter( filters );
    T_KnowledgeObjectDiaIDVector knowledges;
    if( DEC_BlackBoard_CanContainKnowledgeObject* container = caller->GetKnowledgeGroup()->GetKnowledgeObjectContainer() )
        container->GetObjectsInCircle( knowledges, filter, *pCenter, rRadius, nonActivatedObstacles );
    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetObjectsInZone
// Created: NLD 2006-05-05
// -----------------------------------------------------------------------------
T_KnowledgeObjectDiaIDVector DEC_KnowledgeFunctions::GetObjectsInZone( const DEC_Decision_ABC* caller, const TER_Localisation* pLoc, const std::vector< std::string >& parameters )
{
    if( !pLoc )
        throw MASA_EXCEPTION( "invalid parameter." );
    MIL_ObjectFilter filter( parameters );
    T_KnowledgeObjectDiaIDVector knowledges;
    const DEC_KnowledgeBlackBoard_KnowledgeGroup* bbKg = caller->GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
    {
        T_KnowledgeObjectDiaIDVector knowledgesTmp;
        bbKg->GetKnowledgeObjectContainer().GetObjectsInZone( knowledgesTmp, filter, *pLoc );
        knowledges.insert( knowledges.end(), knowledgesTmp.begin(), knowledgesTmp.end() );
    }
    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetObjectsIntersectingInZone
// Created: ABR 2013-01-18
// -----------------------------------------------------------------------------
T_KnowledgeObjectDiaIDVector DEC_KnowledgeFunctions::GetObjectsIntersectingInZone( const DEC_Decision_ABC* caller, const TER_Localisation* pLoc, const std::vector< std::string >& parameters )
{
    if( !pLoc )
        throw MASA_EXCEPTION( "invalid parameter." );
    MIL_ObjectFilter filter( parameters );
    T_KnowledgeObjectDiaIDVector knowledges;
    if( DEC_BlackBoard_CanContainKnowledgeObject* container = caller->GetKnowledgeGroup()->GetKnowledgeObjectContainer() )
        container->GetObjectsIntersectingInZone( knowledges, filter, *pLoc );
    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetObjectsInFuseau
// Created: NLD 2006-05-05
// -----------------------------------------------------------------------------
T_KnowledgeObjectDiaIDVector DEC_KnowledgeFunctions::GetObjectsInFuseau( const DEC_Decision_ABC* caller, const std::string& type )
{
    std::vector< std::string > types;
    types.push_back( type );
    MIL_ObjectFilter filter( types );
    T_KnowledgeObjectDiaIDVector knowledges;
    auto bbKg = caller->GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
    {
        T_KnowledgeObjectDiaIDVector knowledgesTmp;
        bbKg->GetKnowledgeObjectContainer().GetObjectsInZone( knowledgesTmp, filter, caller->GetOrderManager().GetFuseau() );
        knowledges.insert( knowledges.end(), knowledgesTmp.begin(), knowledgesTmp.end() );
    }
    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetObjectsWithCapacityInZone
// Created: JSR 2012-04-17
// -----------------------------------------------------------------------------
T_KnowledgeObjectDiaIDVector DEC_KnowledgeFunctions::GetObjectsWithCapacityInZone( const DEC_Decision_ABC* callerAgent, const std::string& capacity, const TER_Localisation* pLoc )
{
    if( !pLoc || !callerAgent )
        throw MASA_EXCEPTION( "invalid parameter." );

    T_KnowledgeObjectDiaIDVector knowledges;
    if( DEC_BlackBoard_CanContainKnowledgeObject* container = callerAgent->GetKnowledgeGroup()->GetKnowledgeObjectContainer() )
        container->GetObjectsWithCapacityInZone( knowledges, capacity, *pLoc );
    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::IsPositionInsideObjectOfType
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
bool DEC_KnowledgeFunctions::IsPositionInsideObjectOfType( const DEC_Decision_ABC* callerAgent, const std::string& capacity, const MT_Vector2D* pCenter )
{
    if( !callerAgent || !pCenter )
        throw MASA_EXCEPTION( "invalid parameter." );
    if( DEC_BlackBoard_CanContainKnowledgeObject* container = callerAgent->GetKnowledgeGroup()->GetKnowledgeObjectContainer() )
        return container->IsPositionInsideObjectOfType( capacity, *pCenter );
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetPopulationsColliding
// Created: NLD 2005-10-21
// -----------------------------------------------------------------------------
T_KnowledgePopulationDiaIDVector DEC_KnowledgeFunctions::GetPopulationsColliding( const DEC_Decision_ABC* callerAgent )
{
    T_KnowledgePopulationDiaIDVector populationsColliding;
    callerAgent->GetPion().GetKnowledge().GetPopulationsColliding( populationsColliding );
    return populationsColliding;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetPopulationsAttacking
// Created: NLD 2005-11-10
// -----------------------------------------------------------------------------
T_KnowledgePopulationDiaIDVector DEC_KnowledgeFunctions::GetPopulationsAttacking( const DEC_Decision_ABC* callerAgent )
{
    T_KnowledgePopulationDiaIDVector attackers;
    callerAgent->GetPion().GetKnowledge().GetPopulationsAttacking( attackers );
    return attackers;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::IsPopulationAttacking
// Created: DDA 2011-05-03
// -----------------------------------------------------------------------------
bool DEC_KnowledgeFunctions::IsPopulationAttacking( const DEC_Decision_ABC* callerAgent, int knowledgeId )
{
    auto bbKg = callerAgent->GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
    {
        boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = bbKg->GetKnowledgePopulationFromID( knowledgeId );
        if( pKnowledge )
            return callerAgent->GetPion().GetKnowledge().IsPopulationAttacking( *pKnowledge );
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetPopulations
// Created: NLD 2005-12-02
// -----------------------------------------------------------------------------
std::vector< unsigned int > DEC_KnowledgeFunctions::GetPopulations( const DEC_Decision_ABC* caller )
{
    std::vector< unsigned int > results;
    auto bbKg = caller->GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
        bbKg->GetPopulations( results );
    return results;
}

boost::shared_ptr< DEC_Knowledge_Agent > DEC_KnowledgeFunctions::GetAgentKnowledge( int callerId, int knowledgeId )
{
    const auto& entities = MIL_AgentServer::GetWorkspace().GetEntityManager();
    if( MIL_AgentPion* caller = entities.FindAgentPion( callerId ) )
    {
        return caller->GetKnowledge().ResolveKnowledgeAgent( knowledgeId );
    }
    return boost::shared_ptr< DEC_Knowledge_Agent >();
}
