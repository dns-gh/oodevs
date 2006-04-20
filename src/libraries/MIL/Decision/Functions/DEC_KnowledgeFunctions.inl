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
#include "Entities/Objects/MIL_RealObjectTypeFilter.h"
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
void DEC_KnowledgeFunctions::GetLivingEnemiesPerceivedByPion( DIA_Call_ABC& call, const T& /*caller*/ )
{
    assert( DEC_Tools::CheckTypePion( call.GetParameter( 0 ) ) );

    const DEC_RolePion_Decision* pPion = call.GetParameter( 0 ).ToUserObject( pPion );
    assert( pPion );

    T_KnowledgeAgentDiaIDVector knowledges;
    pPion->GetPion().GetKnowledge().GetLivingEnemiesPerceived( knowledges );

    DIA_Variable_ObjectList& diaObjectList = static_cast< DIA_Variable_ObjectList& >( call.GetResult() );
    diaObjectList.SetValueUserType( knowledges, DEC_Tools::GetTypeConnaissanceAgent() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetDangerousEnemiesForPion
// Created: NLD 2006-04-20
// -----------------------------------------------------------------------------
template< typename T > 
void DEC_KnowledgeFunctions::GetDangerousEnemiesForPion( DIA_Call_ABC& call, const T& /*caller*/ )
{
    assert( DEC_Tools::CheckTypePion( call.GetParameter( 0 ) ) );

    const DEC_RolePion_Decision* pPion = call.GetParameter( 0 ).ToUserObject( pPion );
    assert( pPion );

    DIA_Variable_ObjectList& diaObjectList = static_cast< DIA_Variable_ObjectList& >( call.GetResult() );
    diaObjectList.SetValueUserType( pPion->GetPion().GetKnowledge().GetDangerousEnemies(), DEC_Tools::GetTypeConnaissanceAgent() );
}

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
    assert( DEC_Tools::CheckTypePoint( call.GetParameter( 1 ) ) );

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
    MIL_RealObjectTypeFilter objectsFilter( call.GetParameters(), 0 );
    
    T_KnowledgeObjectDiaIDVector knowledges;
    caller.GetArmy().GetKnowledge().GetObjects( knowledges, objectsFilter );

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

