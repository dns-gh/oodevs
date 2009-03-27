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
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Agents/MIL_AgentPion.h"
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
void DEC_KnowledgeFunctions::GetLivingEnemiesPerceivedByPion( DIA_Call_ABC& call, const T& caller )
{
    assert( DEC_Tools::CheckTypePion( call.GetParameter( 0 ) ) );

    const DEC_RolePion_Decision* pSourcePionTmp = call.GetParameter( 0 ).ToUserObject( pSourcePionTmp );
    assert( pSourcePionTmp );
    const MIL_AgentPion& sourcePion = pSourcePionTmp->GetPion();

    T_KnowledgeAgentDiaIDVector sourceKnowledges;
    sourcePion.GetKnowledge().GetLivingEnemiesPerceived( sourceKnowledges );

    T_KnowledgeAgentDiaIDVector translatedKnowledges;
    caller.GetKnowledgeGroup().GetKnowledge().TranslateKnowledges( sourceKnowledges, sourcePion.GetKnowledgeGroup(), translatedKnowledges );

    DIA_Variable_ObjectList& diaObjectList = static_cast< DIA_Variable_ObjectList& >( call.GetResult() );
    diaObjectList.SetValueUserType( translatedKnowledges, DEC_Tools::GetTypeConnaissanceAgent() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetDangerousEnemiesInZoneOfPion
// Created: NLD 2006-04-20
// -----------------------------------------------------------------------------
template< typename T > 
void DEC_KnowledgeFunctions::GetDangerousEnemiesInZoneOfPion( DIA_Call_ABC& call, const T& caller )
{
    assert( DEC_Tools::CheckTypePion        ( call.GetParameter( 0 ) ) );
    assert( DEC_Tools::CheckTypeLocalisation( call.GetParameter( 1 ) ) );

    const DEC_RolePion_Decision* pSourcePionTmp = call.GetParameter( 0 ).ToUserObject( pSourcePionTmp );
    assert( pSourcePionTmp );
    const MIL_AgentPion& sourcePion = pSourcePionTmp->GetPion();

    const TER_Localisation* pZone = call.GetParameter( 1 ).ToUserPtr( pZone );
    assert( pZone );

    T_KnowledgeAgentDiaIDVector sourceKnowledges;
    sourcePion.GetKnowledge().GetDangerousEnemiesInZone( sourceKnowledges, *pZone );
    
    T_KnowledgeAgentDiaIDVector translatedKnowledges;
    caller.GetKnowledgeGroup().GetKnowledge().TranslateKnowledges( sourceKnowledges, sourcePion.GetKnowledgeGroup(), translatedKnowledges );

    DIA_Variable_ObjectList& diaObjectList = static_cast< DIA_Variable_ObjectList& >( call.GetResult() );
    diaObjectList.SetValueUserType( translatedKnowledges, DEC_Tools::GetTypeConnaissanceAgent() );
}

// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::ShareKnowledgesWith
// Created: NLD 2004-11-08
// -----------------------------------------------------------------------------
template< typename T > 
void DEC_KnowledgeFunctions::ShareKnowledgesWith( DIA_Call_ABC& call, const T& caller )
{
    assert( DEC_Tools::CheckTypeAutomate( call.GetParameter( 0 ) ) );

          DEC_AutomateDecision* pAutomate      = call.GetParameter( 0 ).ToUserObject( pAutomate );
    const MT_Float              rTime          = MIL_Tools::ConvertMinutesToSim( call.GetParameter( 1 ).ToFloat() );
    const uint                  nShareTimeStep = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() + (uint)rTime;

    assert( pAutomate );
    pAutomate->GetAutomate().GetKnowledgeGroup().GetKnowledge().GetKsSharing().ShareFromSource( caller.GetKnowledgeGroup(), nShareTimeStep );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::ShareKnowledgesInZoneWith
// Created: NLD 2004-11-08
// -----------------------------------------------------------------------------
template< typename T > 
void DEC_KnowledgeFunctions::ShareKnowledgesInZoneWith( DIA_Call_ABC& call, const T& caller )
{
    assert( DEC_Tools::CheckTypeAutomate( call.GetParameter( 0 ) ) );
    assert( DEC_Tools::CheckTypePoint   ( call.GetParameter( 1 ) ) );

          DEC_AutomateDecision* pAutomate           = call.GetParameter( 0 ).ToUserObject( pAutomate );
    const MT_Vector2D*          pSharedCircleCenter = call.GetParameter( 1 ).ToUserPtr   ( pSharedCircleCenter );
          MT_Float              rSharedCircleRadius = MIL_Tools::ConvertMeterToSim( call.GetParameter( 2 ).ToFloat() );

    assert( pAutomate );
    pAutomate->GetAutomate().GetKnowledgeGroup().GetKnowledge().GetKsSharing().ShareFromSource( caller.GetKnowledgeGroup(), MIL_AgentServer::GetWorkspace().GetCurrentTimeStep(), *pSharedCircleCenter, rSharedCircleRadius );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetObstacles
// Created: NLD 2004-04-09
// -----------------------------------------------------------------------------
template< typename T > 
void DEC_KnowledgeFunctions::GetObjects( DIA_Call_ABC& call, const T& caller )
{
    MIL_ObjectFilter filter( call.GetParameters(), 0 );
    
    T_KnowledgeObjectDiaIDVector knowledges;
    caller.GetArmy().GetKnowledge().GetObjects( knowledges, filter );

    DIA_Variable_ObjectList& diaObjectList = static_cast< DIA_Variable_ObjectList& >( call.GetResult() );
    diaObjectList.SetValueUserType( knowledges, DEC_Tools::GetTypeConnaissanceObjet() );
}

// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_KnowledgeFunctions::GetObjectsInCircle
// Created: NLD 2006-05-05
// -----------------------------------------------------------------------------
template< typename T > 
void DEC_KnowledgeFunctions::GetObjectsInCircle( DIA_Call_ABC& call, const T& caller )
{
    assert( DEC_Tools::CheckTypePoint( call.GetParameter( 0 ) ) );
    const MT_Vector2D* pCenter = call.GetParameter( 0 ).ToUserPtr( pCenter );
    assert( pCenter );

    MIL_ObjectFilter filter( call.GetParameters(), 2 );
    
    T_KnowledgeObjectDiaIDVector knowledges;
    caller.GetArmy().GetKnowledge().GetObjectsInCircle( knowledges, filter, *pCenter, call.GetParameter( 1 ).ToFloat() );

    DIA_Variable_ObjectList& diaObjectList = static_cast< DIA_Variable_ObjectList& >( call.GetResult() );
    diaObjectList.SetValueUserType( knowledges, DEC_Tools::GetTypeConnaissanceObjet() );
}

// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_KnowledgeFunctions::GetObjectsInZone
// Created: NLD 2006-05-05
// -----------------------------------------------------------------------------
template< typename T > 
void DEC_KnowledgeFunctions::GetObjectsInZone( DIA_Call_ABC& call, const T& caller )
{
    assert( DEC_Tools::CheckTypeLocalisation( call.GetParameter( 0 ) ) );

    const TER_Localisation* pLoc = call.GetParameter( 0 ).ToUserPtr( pLoc );
    assert( pLoc );

    MIL_ObjectFilter filter( call.GetParameters(), 1 );
    
    T_KnowledgeObjectDiaIDVector knowledges;
    caller.GetArmy().GetKnowledge().GetObjectsInZone( knowledges, filter, *pLoc );

    DIA_Variable_ObjectList& diaObjectList = static_cast< DIA_Variable_ObjectList& >( call.GetResult() );
    diaObjectList.SetValueUserType( knowledges, DEC_Tools::GetTypeConnaissanceObjet() );
}

// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_KnowledgeFunctions::GetObjectsInFuseau
// Created: NLD 2006-05-05
// -----------------------------------------------------------------------------
template< typename T > 
void DEC_KnowledgeFunctions::GetObjectsInFuseau( DIA_Call_ABC& call, const T& caller )
{
    MIL_ObjectFilter filter( call.GetParameters(), 0 );
    
    T_KnowledgeObjectDiaIDVector knowledges;
    caller.GetArmy().GetKnowledge().GetObjectsInZone( knowledges, filter, caller.GetFuseau() );

    DIA_Variable_ObjectList& diaObjectList = static_cast< DIA_Variable_ObjectList& >( call.GetResult() );
    diaObjectList.SetValueUserType( knowledges, DEC_Tools::GetTypeConnaissanceObjet() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetFriendsInZone
// Created: NLD 2005-05-11
// -----------------------------------------------------------------------------
template< typename T > 
void DEC_KnowledgeFunctions::GetFriendsInZone( DIA_Call_ABC& call, const T& caller )
{
    assert( DEC_Tools::CheckTypeLocalisation( call.GetParameter( 0 ) ) );
    TER_Localisation* pZone = call.GetParameter( 0 ).ToUserPtr( pZone );
    assert( pZone );

    T_KnowledgeAgentDiaIDVector knowledges;
    caller.GetKnowledgeGroup().GetKnowledge().GetFriendsInZone( knowledges, *pZone );

    DIA_Variable_ObjectList& diaObjectList = static_cast< DIA_Variable_ObjectList& >( call.GetResult() );
    diaObjectList.SetValueUserType( knowledges, DEC_Tools::GetTypeConnaissanceAgent() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetPopulations
// Created: NLD 2005-12-02
// -----------------------------------------------------------------------------
template< typename T > 
void DEC_KnowledgeFunctions::GetPopulations( DIA_Call_ABC& call, const T& caller )
{
    T_KnowledgePopulationDiaIDVector knowledges;
    caller.GetKnowledgeGroup().GetKnowledge().GetPopulations( knowledges );

    DIA_Variable_ObjectList& diaObjectList = static_cast< DIA_Variable_ObjectList& >( call.GetResult() );
    diaObjectList.SetValueUserType( knowledges, DEC_Tools::GetTypeConnaissancePopulation() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::ComputeEnemiesRatio
// Created: NLD 2007-04-19
// -----------------------------------------------------------------------------
template< typename T, typename B >
float DEC_KnowledgeFunctions::ComputeEnemiesRatio( const T& caller, const B& boundaries, bool unloaded )
{
    uint nNbrTotalLivingEnemies    = 0;
    uint nNbrSelectedLivingEnemies = 0;

    const T_KnowledgeAgentVector& enemies = caller.GetKnowledgeGroup().GetKnowledge().GetEnemies();
    for( CIT_KnowledgeAgentVector itKnowledgeAgent = enemies.begin(); itKnowledgeAgent != enemies.end(); ++itKnowledgeAgent )
    {
        const DEC_Knowledge_Agent& knowledge = **itKnowledgeAgent;
        if( !knowledge.IsDead() && boundaries.IsInside( knowledge.GetPosition() ) )
        {
            ++ nNbrTotalLivingEnemies;
            if( knowledge.IsHuman() == unloaded )
                ++ nNbrSelectedLivingEnemies;
        }
    }
    if( nNbrTotalLivingEnemies == 0 )
        return 0.;
    return (float)nNbrSelectedLivingEnemies / (float)nNbrTotalLivingEnemies;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::ComputeUnloadedEnemiesRatio
// Created: NLD 2007-04-19
// -----------------------------------------------------------------------------
template< typename T > 
void DEC_KnowledgeFunctions::ComputeUnloadedEnemiesRatio( DIA_Call_ABC& call, const T& caller )
{
    if( DEC_Tools::CheckTypeFuseau( call.GetParameter( 0 ) ) )
    {
        if( const MIL_Fuseau* pFuseau = call.GetParameter( 0 ).ToUserPtr( pFuseau ) )
            call.GetResult().SetValue( ComputeEnemiesRatio( caller, *pFuseau, true ) );
    }
    else if( DEC_Tools::CheckTypeLocalisation( call.GetParameter( 0 ) ) )
    {
        if( const TER_Localisation* location = call.GetParameter( 0 ).ToUserPtr( location ) )
            call.GetResult().SetValue( ComputeEnemiesRatio( caller, *location, true ) );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::ComputeLoadedEnemiesRatio
// Created: NLD 2007-04-19
// -----------------------------------------------------------------------------
template< typename T > 
void DEC_KnowledgeFunctions::ComputeLoadedEnemiesRatio( DIA_Call_ABC& call, const T& caller )
{
    if( DEC_Tools::CheckTypeFuseau( call.GetParameter( 0 ) ) )
    {
        if( const MIL_Fuseau* pFuseau = call.GetParameter( 0 ).ToUserPtr( pFuseau ) )
            call.GetResult().SetValue( ComputeEnemiesRatio( caller, *pFuseau, false ) );
    }
    else if( DEC_Tools::CheckTypeLocalisation( call.GetParameter( 0 ) ) )
    {
        if( const TER_Localisation* location = call.GetParameter( 0 ).ToUserPtr( location ) )
            call.GetResult().SetValue( ComputeEnemiesRatio( caller, *location, false ) );
    }
}

namespace 
{
    template< typename T >
    struct CompareBoundariesEnemies
    {
        CompareBoundariesEnemies( const T& caller, bool unloaded )
            : pCaller_( &caller ), unloaded_( unloaded ) {}
        bool operator()( DIA_Variable_ABC* dia1, DIA_Variable_ABC* dia2 )
        {
            if( DEC_Tools::CheckTypeFuseau( *dia1 ) && DEC_Tools::CheckTypeFuseau( *dia2 ) )
            {
                MIL_Fuseau* pFuseau1 = dia1->ToUserPtr( pFuseau1 );
                MIL_Fuseau* pFuseau2 = dia2->ToUserPtr( pFuseau2 );
                return DEC_KnowledgeFunctions::ComputeEnemiesRatio( *pCaller_, *pFuseau1, unloaded_ )
                     < DEC_KnowledgeFunctions::ComputeEnemiesRatio( *pCaller_, *pFuseau2, unloaded_ );
            }
            else if( DEC_Tools::CheckTypeLocalisation( *dia1 ) && DEC_Tools::CheckTypeLocalisation( *dia2 ) )
            {
                TER_Localisation* location1 = dia1->ToUserPtr( location1 );
                TER_Localisation* location2 = dia2->ToUserPtr( location2 );
                return DEC_KnowledgeFunctions::ComputeEnemiesRatio( *pCaller_, *location1, unloaded_ )
                     < DEC_KnowledgeFunctions::ComputeEnemiesRatio( *pCaller_, *location2, unloaded_ );
            }
            return false;
        }

        const T* pCaller_;
        bool unloaded_;
    };    
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::SortAccordingToUnloadedEnemies
// Created: NLD 2007-04-19
// -----------------------------------------------------------------------------
template< typename T > 
void DEC_KnowledgeFunctions::SortAccordingToUnloadedEnemies( DIA_Call_ABC& call, const T& caller )
{
    assert( DEC_Tools::CheckTypeListeFuseaux( call.GetParameter( 0 ) ) || DEC_Tools::CheckTypeListeLocalisations( call.GetParameter( 0 ) ) );

    call.GetResult() = call.GetParameter( 0 );
    
    T_ObjectVariableVector& boundaries = const_cast< T_ObjectVariableVector& >( static_cast< DIA_Variable_ObjectList& >( call.GetResult() ).GetContainer() );
    std::sort( boundaries.begin(), boundaries.end(), CompareBoundariesEnemies< MIL_Automate >( caller, true ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::SortAccordingToLoadedEnemies
// Created: AGE 2007-10-16
// -----------------------------------------------------------------------------
template< typename T >
void DEC_KnowledgeFunctions::SortAccordingToLoadedEnemies( DIA_Call_ABC& call, const T& caller )
{
    assert( DEC_Tools::CheckTypeListeFuseaux( call.GetParameter( 0 ) ) || DEC_Tools::CheckTypeListeLocalisations( call.GetParameter( 0 ) ) );

    call.GetResult() = call.GetParameter( 0 );
    
    T_ObjectVariableVector& boundaries = const_cast< T_ObjectVariableVector& >( static_cast< DIA_Variable_ObjectList& >( call.GetResult() ).GetContainer() );
    std::sort( boundaries.begin(), boundaries.end(), CompareBoundariesEnemies< MIL_Automate >( caller, false ) );
}
