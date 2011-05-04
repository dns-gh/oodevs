// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_KnowledgePopulationFunctions.cpp $
// $Author: Nld $
// $Modtime: 23/03/05 13:57 $
// $Revision: 9 $
// $Workfile: DEC_KnowledgePopulationFunctions.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_KnowledgePopulationFunctions.h"
#include "Entities/Agents/Roles/Dotations/PHY_RoleInterface_Dotations.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/MIL_Army.h"
#include "Entities/Populations/MIL_Population.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/QueryValidity.h"

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::Recon
// Created: NLD 2004-10-29
// -----------------------------------------------------------------------------
void DEC_KnowledgePopulationFunctions::Recon( const MIL_AgentPion& callerAgent, int knowledgeId )
{
    DEC_Knowledge_Population* pKnowledge = callerAgent.GetKnowledgeGroup().GetKnowledge().GetKnowledgePopulationFromID(knowledgeId);
    if( pKnowledge )
        pKnowledge->Recon();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::GetDangerosity
// Created: NLD 2005-11-10
// -----------------------------------------------------------------------------
float DEC_KnowledgePopulationFunctions::GetDangerosity( const MIL_AgentPion& caller, int knowledgeId )
{
    DEC_Knowledge_Population* pKnowledge = caller.GetKnowledgeGroup().GetKnowledge().GetKnowledgePopulationFromID(knowledgeId);
    if( pKnowledge )
    {
        // For DIA, the dangerosity value is 1 <= dangerosity <= 2
        const double rDangerosity = pKnowledge->GetDangerosity( caller );
        return (float)( rDangerosity + 1. ) ;
    }
    return 0.f;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::Secure
// Created: NLD 2005-12-02
// -----------------------------------------------------------------------------
void DEC_KnowledgePopulationFunctions::Secure( const MIL_AgentPion& caller, int knowledgeId )
{
    DEC_Knowledge_Population* pKnowledge = caller.GetKnowledgeGroup().GetKnowledge().GetKnowledgePopulationFromID(knowledgeId);
    if( pKnowledge )
        pKnowledge->Secure( caller );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::SecuringPoint
// Created: SBO 2005-12-16
// -----------------------------------------------------------------------------
boost::shared_ptr<MT_Vector2D> DEC_KnowledgePopulationFunctions::SecuringPoint( const MIL_AgentPion& caller, int knowledgeId )
{
    DEC_Knowledge_Population* pKnowledge = caller.GetKnowledgeGroup().GetKnowledge().GetKnowledgePopulationFromID(knowledgeId);
    if( !pKnowledge )
        return boost::shared_ptr< MT_Vector2D >();
    else
    {
        MT_Vector2D* pResult = new MT_Vector2D( pKnowledge->GetSecuringPoint( caller ) );
        return boost::shared_ptr< MT_Vector2D >( pResult );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::ClosestPoint
// Created: NLD 2005-12-05
// -----------------------------------------------------------------------------
boost::shared_ptr<MT_Vector2D> DEC_KnowledgePopulationFunctions::ClosestPoint( const MIL_AgentPion& caller, int knowledgeId )
{
    DEC_Knowledge_Population* pKnowledge = caller.GetKnowledgeGroup().GetKnowledge().GetKnowledgePopulationFromID(knowledgeId);
    if( !pKnowledge )
        return boost::shared_ptr<MT_Vector2D>();
    else
    {
        MT_Vector2D* pResult = new MT_Vector2D( pKnowledge->GetClosestPoint( caller.GetRole< PHY_RoleInterface_Location >().GetPosition() ) ); //$$$ RAM
        return boost::shared_ptr< MT_Vector2D >( pResult );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::IsEnemy
// Created: HME 2005-12-29
// -----------------------------------------------------------------------------
bool DEC_KnowledgePopulationFunctions::IsEnemy( const MIL_AgentPion& caller, int knowledgeId )
{
    DEC_Knowledge_Population* pKnowledge = caller.GetKnowledgeGroup().GetKnowledge().GetKnowledgePopulationFromID(knowledgeId);
    if( !pKnowledge )
        return false;
    return eTristate_True == caller.GetArmy().IsAnEnemy( pKnowledge->GetArmy() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::Exterminate
// Created: SBO 2005-12-22
// -----------------------------------------------------------------------------
int DEC_KnowledgePopulationFunctions::Exterminate( MIL_AgentPion& caller, int knowledgeId, float surface, const PHY_DotationCategory* dotation )
{
    DEC_Knowledge_Population* pKnowledge = caller.GetKnowledgeGroup().GetKnowledge().GetKnowledgePopulationFromID(knowledgeId);
    if( !pKnowledge )
        return eQueryInvalid;
    pKnowledge->Exterminate( caller, surface );
    caller.Get< dotation::PHY_RoleInterface_Dotations >().AddFireReservation( *dotation, 1 );
    return eQueryValid;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::IsPerceivedByAgent
// Created: SBO 2006-01-18
// -----------------------------------------------------------------------------
bool DEC_KnowledgePopulationFunctions::IsPerceivedByAgent( const MIL_AgentPion& caller, int knowledgeId )
{
    DEC_Knowledge_Population* pKnowledge = caller.GetKnowledgeGroup().GetKnowledge().GetKnowledgePopulationFromID(knowledgeId);
    if( !pKnowledge )
        return false;
    return caller.GetKnowledge().IsPerceived( pKnowledge->GetPopulationKnown() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::GetCriticalIntelligence
// Created: ABR 2011-03-16
// -----------------------------------------------------------------------------
std::string DEC_KnowledgePopulationFunctions::GetCriticalIntelligence( const MIL_AgentPion& caller, int knowledgeId )
{
    DEC_Knowledge_Population* pKnowledge = caller.GetKnowledgeGroup().GetKnowledge().GetKnowledgePopulationFromID( knowledgeId );
    if( !pKnowledge )
        return "";
    else
    {
        pKnowledge->SetCriticalIntelligenceFromPopulationKnown();
        return pKnowledge->GetCriticalIntelligence();
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::HasFlow
// Created: EVH 2011-04-07
// -----------------------------------------------------------------------------
bool DEC_KnowledgePopulationFunctions::HasFlow( const MIL_AgentPion& caller, int knowledgeId )
{
    DEC_Knowledge_Population* pKnowledge = caller.GetKnowledgeGroup().GetKnowledge().GetKnowledgePopulationFromID( knowledgeId );
    if( !pKnowledge )
        return false;
    return pKnowledge->GetPopulationKnown().HasFlow();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::ChangeArmedIndividuals
// Created: LMT 2011-04-01
// -----------------------------------------------------------------------------
void DEC_KnowledgePopulationFunctions::ChangeArmedIndividuals( const MIL_AgentPion& callerAgent, int knowledgeId, const double rArmedIndividuals)
{
    DEC_Knowledge_Population* pKnowledge = callerAgent.GetKnowledgeGroup().GetKnowledge().GetKnowledgePopulationFromID( knowledgeId );
    if( pKnowledge )
        pKnowledge->GetPopulationKnown().SetNewArmedIndividuals( rArmedIndividuals );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::GetCrowdAffinity
// Created: DDA 2011-05-02
// -----------------------------------------------------------------------------
float DEC_KnowledgePopulationFunctions::GetCrowdAffinity( const MIL_AgentPion& callerAgent, int knowledgeId )
{
    DEC_Knowledge_Population* pKnowledge = callerAgent.GetKnowledgeGroup().GetKnowledge().GetKnowledgePopulationFromID( knowledgeId );
    if( pKnowledge )
    {
        MIL_Army_ABC& army = callerAgent.GetArmy();
        return pKnowledge->GetPopulationKnown().GetAffinity( army.GetID() );
    }
    return 0;
}
