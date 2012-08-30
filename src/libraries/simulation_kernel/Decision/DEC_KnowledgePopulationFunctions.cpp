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
#include "DEC_Decision_ABC.h"
#include "Entities/MIL_Army.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Dotations/PHY_RoleInterface_Dotations.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Populations/MIL_PopulationConcentration.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/QueryValidity.h"

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::Recon
// Created: NLD 2004-10-29
// -----------------------------------------------------------------------------
void DEC_KnowledgePopulationFunctions::Recon( const MIL_AgentPion& callerAgent, int knowledgeId )
{
    boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = callerAgent.GetKnowledgeGroup()->GetKnowledge().GetKnowledgePopulationFromID( knowledgeId );
    if( pKnowledge )
        pKnowledge->Recon();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::GetDangerosity
// Created: NLD 2005-11-10
// -----------------------------------------------------------------------------
float DEC_KnowledgePopulationFunctions::GetDangerosity( const MIL_AgentPion& caller, int knowledgeId )
{
    boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = caller.GetKnowledgeGroup()->GetKnowledge().GetKnowledgePopulationFromID( knowledgeId );
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
    boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = caller.GetKnowledgeGroup()->GetKnowledge().GetKnowledgePopulationFromID( knowledgeId );
    if( pKnowledge )
        pKnowledge->Secure( caller );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::SecuringPoint
// Created: SBO 2005-12-16
// -----------------------------------------------------------------------------
boost::shared_ptr<MT_Vector2D> DEC_KnowledgePopulationFunctions::SecuringPoint( const MIL_AgentPion& caller, int knowledgeId )
{
    boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = caller.GetKnowledgeGroup()->GetKnowledge().GetKnowledgePopulationFromID( knowledgeId );
    if( !pKnowledge )
        return boost::shared_ptr< MT_Vector2D >();
    else
    {
        MT_Vector2D* pResult = new MT_Vector2D( pKnowledge->GetSecuringPoint( caller ) );
        return boost::shared_ptr< MT_Vector2D >( pResult );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::GetConcentrationPosition
// Created: JSR 2011-08-09
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_KnowledgePopulationFunctions::GetConcentrationPosition( const DEC_Decision_ABC& callerAgent, int knowledgeId, unsigned int concentrationId )
{
    boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = callerAgent.GetKnowledgeGroup()->GetKnowledge().GetKnowledgePopulationFromID( knowledgeId );
    if( !pKnowledge )
        return boost::shared_ptr< MT_Vector2D >();
    else
        return pKnowledge->GetPopulationKnown().GetConcentrationPosition( concentrationId );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::GetAllHumansInConcentration
// Created: JSR 2011-08-09
// -----------------------------------------------------------------------------
unsigned int DEC_KnowledgePopulationFunctions::GetAllHumansInConcentration( const DEC_Decision_ABC& callerAgent, int knowledgeId, unsigned int concentrationId )
{
    boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = callerAgent.GetKnowledgeGroup()->GetKnowledge().GetKnowledgePopulationFromID( knowledgeId );
    if( !pKnowledge )
        return 0;
    else
        return pKnowledge->GetPopulationKnown().GetAllHumansInConcentration( concentrationId );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::ClosestPoint
// Created: NLD 2005-12-05
// -----------------------------------------------------------------------------
boost::shared_ptr<MT_Vector2D> DEC_KnowledgePopulationFunctions::ClosestPoint( const MIL_AgentPion& caller, int knowledgeId )
{
    boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = caller.GetKnowledgeGroup()->GetKnowledge().GetKnowledgePopulationFromID( knowledgeId );
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
    boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = caller.GetKnowledgeGroup()->GetKnowledge().GetKnowledgePopulationFromID( knowledgeId );
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
    boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = caller.GetKnowledgeGroup()->GetKnowledge().GetKnowledgePopulationFromID( knowledgeId );
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
    boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = caller.GetKnowledgeGroup()->GetKnowledge().GetKnowledgePopulationFromID( knowledgeId );
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
    boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = caller.GetKnowledgeGroup()->GetKnowledge().GetKnowledgePopulationFromID( knowledgeId );
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
    boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = caller.GetKnowledgeGroup()->GetKnowledge().GetKnowledgePopulationFromID( knowledgeId );
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
    boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = callerAgent.GetKnowledgeGroup()->GetKnowledge().GetKnowledgePopulationFromID( knowledgeId );
    if( pKnowledge )
        pKnowledge->GetPopulationKnown().SetNewArmedIndividuals( rArmedIndividuals );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::GetCrowdAffinity
// Created: DDA 2011-05-02
// -----------------------------------------------------------------------------
float DEC_KnowledgePopulationFunctions::GetCrowdAffinity( const MIL_AgentPion& callerAgent, int knowledgeId )
{
    boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = callerAgent.GetKnowledgeGroup()->GetKnowledge().GetKnowledgePopulationFromID( knowledgeId );
    if( pKnowledge )
    {
        MIL_Army_ABC& army = callerAgent.GetArmy();
        return pKnowledge->GetPopulationKnown().GetAffinity( army.GetID() );
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::GetCrowdAttitude
// Created: DDA 2011-05-17
// -----------------------------------------------------------------------------
int DEC_KnowledgePopulationFunctions::GetCrowdAttitude( const MIL_AgentPion& callerAgent, int knowledgeId )
{
    boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = callerAgent.GetKnowledgeGroup()->GetKnowledge().GetKnowledgePopulationFromID( knowledgeId );
    if( pKnowledge )
    {
        return pKnowledge->GetPopulationKnown().GetAttitude().GetID();
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr<MT_Vector2D> DEC_KnowledgePopulationFunctions::GetFlowHead
// Created: EVH 2011-05-10
// -----------------------------------------------------------------------------
boost::shared_ptr<MT_Vector2D> DEC_KnowledgePopulationFunctions::GetFlowHead( const DEC_Decision_ABC& callerAgent, int knowledgeId )
{
    boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = callerAgent.GetKnowledgeGroup()->GetKnowledge().GetKnowledgePopulationFromID( knowledgeId );
    if( pKnowledge )
    {
        MT_Vector2D* pResult = new MT_Vector2D( pKnowledge->GetPopulationKnown().GetFlowHeadPosition() );
        return  boost::shared_ptr< MT_Vector2D >( pResult );
    }
    return boost::shared_ptr<MT_Vector2D>();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::GetClosestConcentration
// Created: JSR 2011-08-09
// -----------------------------------------------------------------------------
int DEC_KnowledgePopulationFunctions::GetClosestConcentration( const DEC_Decision_ABC& callerAgent, int knowledgeId, const MT_Vector2D& point, int refDistance )
{
    boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = callerAgent.GetKnowledgeGroup()->GetKnowledge().GetKnowledgePopulationFromID( knowledgeId );
    if( pKnowledge )
    {
        MIL_PopulationConcentration* concentration = pKnowledge->GetPopulationKnown().GetClosestConcentration( point, refDistance );
        if( concentration )
            return concentration->GetID();
    }
    return 0;
}
