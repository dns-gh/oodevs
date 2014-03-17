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
#include "MIL_AgentServer.h"
#include "Entities/MIL_Army.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Composantes//PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Dotations/PHY_RoleInterface_Dotations.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Orders/MIL_PopulationOrderManager.h"
#include "Entities/Populations/DEC_PopulationDecision.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Populations/MIL_PopulationType.h"
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
    auto bbKg = callerAgent.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
    {
        boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = bbKg->GetKnowledgePopulationFromID( knowledgeId );
        if( pKnowledge )
            pKnowledge->Recon();
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::GetDangerosity
// Created: NLD 2005-11-10
// -----------------------------------------------------------------------------
float DEC_KnowledgePopulationFunctions::GetDangerosity( const MIL_AgentPion& caller, int knowledgeId )
{
    auto bbKg = caller.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
    {
        boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = bbKg->GetKnowledgePopulationFromID( knowledgeId );
        if( pKnowledge )
        {
            // For DIA, the dangerosity value is 1 <= dangerosity <= 2
            const double rDangerosity = pKnowledge->GetDangerosity( caller );
            return (float)( rDangerosity + 1. ) ;
        }
    }
    return 0.f;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::Secure
// Created: NLD 2005-12-02
// -----------------------------------------------------------------------------
void DEC_KnowledgePopulationFunctions::Secure( const MIL_AgentPion& caller, int knowledgeId )
{
    auto bbKg = caller.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
    {
        boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = bbKg->GetKnowledgePopulationFromID( knowledgeId );
        if( pKnowledge )
            pKnowledge->Secure( caller );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::SecuringPoint
// Created: SBO 2005-12-16
// -----------------------------------------------------------------------------
boost::shared_ptr<MT_Vector2D> DEC_KnowledgePopulationFunctions::SecuringPoint( const MIL_AgentPion& caller, int knowledgeId )
{
    auto bbKg = caller.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
    {
        boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = bbKg->GetKnowledgePopulationFromID( knowledgeId );
        if( pKnowledge )
            return pKnowledge->GetSecuringPoint( caller );
    }
    return boost::shared_ptr< MT_Vector2D >();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::GetConcentrationPosition
// Created: JSR 2011-08-09
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_KnowledgePopulationFunctions::GetConcentrationPosition( const DEC_Decision_ABC& callerAgent, int knowledgeId, unsigned int concentrationId )
{
    auto bbKg = callerAgent.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
    {
        boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = bbKg->GetKnowledgePopulationFromID( knowledgeId );
        if( pKnowledge )
           return pKnowledge->GetPopulationKnown().GetConcentrationPosition( concentrationId );
    }
    return boost::shared_ptr< MT_Vector2D >();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::GetAllHumansInConcentration
// Created: JSR 2011-08-09
// -----------------------------------------------------------------------------
unsigned int DEC_KnowledgePopulationFunctions::GetAllHumansInConcentration( const DEC_Decision_ABC& callerAgent, int knowledgeId, unsigned int concentrationId )
{
    auto bbKg = callerAgent.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
    {
        boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = bbKg->GetKnowledgePopulationFromID( knowledgeId );
        if( pKnowledge )
            return pKnowledge->GetPopulationKnown().GetAllHumansInConcentration( concentrationId );
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::GetAllHumans
// Created: GGE 2013-04-04
// -----------------------------------------------------------------------------
unsigned int DEC_KnowledgePopulationFunctions::GetAllHumans( const DEC_Decision_ABC& callerAgent, int knowledgeId )
{
    auto bbKg = callerAgent.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
    {
        boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = bbKg->GetKnowledgePopulationFromID( knowledgeId );
        if( pKnowledge )
            return pKnowledge->GetPopulationKnown().GetAllHumans();
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::ClosestPoint
// Created: NLD 2005-12-05
// -----------------------------------------------------------------------------
boost::shared_ptr<MT_Vector2D> DEC_KnowledgePopulationFunctions::ClosestPoint( const MIL_AgentPion& caller, int knowledgeId )
{
    auto bbKg = caller.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
    {
        boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = bbKg->GetKnowledgePopulationFromID( knowledgeId );
        if( pKnowledge )
        {
            MT_Vector2D* pResult = new MT_Vector2D( pKnowledge->GetClosestPoint( caller.GetRole< PHY_RoleInterface_Location >().GetPosition() ) ); //$$$ RAM
            return boost::shared_ptr< MT_Vector2D >( pResult );
        }
    }
    return boost::shared_ptr< MT_Vector2D >();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::IsEnemy
// Created: HME 2005-12-29
// -----------------------------------------------------------------------------
bool DEC_KnowledgePopulationFunctions::IsEnemy( const MIL_AgentPion& caller, int knowledgeId )
{
    auto bbKg = caller.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
    {
        boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = bbKg->GetKnowledgePopulationFromID( knowledgeId );
        if( pKnowledge )
            return eTristate_True == caller.GetArmy().IsAnEnemy( pKnowledge->GetArmy() );
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::Exterminate
// Created: SBO 2005-12-22
// -----------------------------------------------------------------------------
int DEC_KnowledgePopulationFunctions::Exterminate( MIL_AgentPion& caller, int knowledgeId, float surface, const PHY_DotationCategory* dotation )
{
    auto bbKg = caller.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
    {
        boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = bbKg->GetKnowledgePopulationFromID( knowledgeId );
        if( pKnowledge )
        {
            pKnowledge->Exterminate( caller, surface );
            caller.Get< dotation::PHY_RoleInterface_Dotations >().AddFireReservation( *dotation, 1 );
            return eQueryValid;
        }
    }
    return eQueryInvalid;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::IsPerceivedByAgent
// Created: SBO 2006-01-18
// -----------------------------------------------------------------------------
bool DEC_KnowledgePopulationFunctions::IsPerceivedByAgent( const MIL_AgentPion& caller, int knowledgeId )
{
    auto bbKg = caller.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
    {
        boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = bbKg->GetKnowledgePopulationFromID( knowledgeId );
        if( pKnowledge )
            return caller.GetKnowledge().IsPerceived( pKnowledge->GetPopulationKnown() );
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::GetCriticalIntelligence
// Created: ABR 2011-03-16
// -----------------------------------------------------------------------------
std::string DEC_KnowledgePopulationFunctions::GetCriticalIntelligence( const MIL_AgentPion& caller, int knowledgeId )
{
    auto bbKg = caller.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
    {
        boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = bbKg->GetKnowledgePopulationFromID( knowledgeId );
        if( pKnowledge )
        {
            pKnowledge->SetCriticalIntelligenceFromPopulationKnown();
            return pKnowledge->GetCriticalIntelligence();
        }
    }
    return "";
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::ChangeArmedIndividuals
// Created: LMT 2011-04-01
// -----------------------------------------------------------------------------
void DEC_KnowledgePopulationFunctions::ChangeArmedIndividuals( const MIL_AgentPion& callerAgent, int knowledgeId, const double rArmedIndividuals )
{
    auto bbKg = callerAgent.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
    {
        boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = bbKg->GetKnowledgePopulationFromID( knowledgeId );
        if( pKnowledge )
            pKnowledge->GetPopulationKnown().SetNewArmedIndividuals( rArmedIndividuals );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::GetCrowdAffinity
// Created: DDA 2011-05-02
// -----------------------------------------------------------------------------
float DEC_KnowledgePopulationFunctions::GetCrowdAffinity( const MIL_AgentPion& callerAgent, int knowledgeId )
{
    auto bbKg = callerAgent.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
    {
        boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = bbKg->GetKnowledgePopulationFromID( knowledgeId );
        if( pKnowledge )
        {
            MIL_Army_ABC& army = callerAgent.GetArmy();
            return pKnowledge->GetPopulationKnown().GetAffinity( army.GetID() );
        }
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::GetCrowdAttitude
// Created: DDA 2011-05-17
// -----------------------------------------------------------------------------
int DEC_KnowledgePopulationFunctions::GetCrowdAttitude( const MIL_AgentPion& callerAgent, int knowledgeId )
{
    auto bbKg = callerAgent.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
    {
        boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = bbKg->GetKnowledgePopulationFromID( knowledgeId );
        if( pKnowledge )
            return pKnowledge->GetPopulationKnown().GetAttitude().GetID();
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::StartHidingInCrowd
// Created: MCO 2013-05-07
// -----------------------------------------------------------------------------
void DEC_KnowledgePopulationFunctions::StartHidingInCrowd( MIL_AgentPion& callerAgent, int knowledgeId )
{
    auto bbKg = callerAgent.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
    {
        boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = bbKg->GetKnowledgePopulationFromID( knowledgeId );
        if( pKnowledge )
            pKnowledge->GetPopulationKnown().AddHidden( callerAgent );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::StopHidingInCrowd
// Created: MCO 2013-05-07
// -----------------------------------------------------------------------------
void DEC_KnowledgePopulationFunctions::StopHidingInCrowd( MIL_AgentPion& callerAgent, int knowledgeId )
{
    auto bbKg = callerAgent.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
    {
        boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = bbKg->GetKnowledgePopulationFromID( knowledgeId );
        if( pKnowledge )
            pKnowledge->GetPopulationKnown().RemoveHidden( callerAgent );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::GetAgentsHiddenInCrowd
// Created: MCO 2013-05-23
// -----------------------------------------------------------------------------
std::vector< DEC_Decision_ABC* > DEC_KnowledgePopulationFunctions::GetAgentsHiddenInCrowd( MIL_AgentPion& callerAgent, int knowledgeId )
{
    std::vector< DEC_Decision_ABC* > result;
    auto bbKg = callerAgent.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
    {
        boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = bbKg->GetKnowledgePopulationFromID( knowledgeId );
        if( pKnowledge )
        {
            const auto& hidden = pKnowledge->GetPopulationKnown().GetHidden();
            for( auto it = hidden.begin(); it != hidden.end(); ++it )
                result.push_back( &(*it)->GetRole< DEC_Decision_ABC >() );
        }
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::GetFlowHead
// Created: EVH 2011-05-10
// -----------------------------------------------------------------------------
boost::shared_ptr<MT_Vector2D> DEC_KnowledgePopulationFunctions::GetFlowHead( const DEC_Decision_ABC& callerAgent, int knowledgeId )
{
    auto bbKg = callerAgent.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
    {
        boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = bbKg->GetKnowledgePopulationFromID( knowledgeId );
        if( pKnowledge )
        {
            MT_Vector2D* pResult = new MT_Vector2D( pKnowledge->GetPopulationKnown().GetFlowHeadPosition() );
            return  boost::shared_ptr< MT_Vector2D >( pResult );
        }
    }
    return boost::shared_ptr<MT_Vector2D>();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::GetClosestConcentration
// Created: JSR 2011-08-09
// -----------------------------------------------------------------------------
int DEC_KnowledgePopulationFunctions::GetClosestConcentration( const DEC_Decision_ABC& callerAgent, int knowledgeId, const MT_Vector2D& point, int refDistance )
{
    auto bbKg = callerAgent.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
    {
        boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = bbKg->GetKnowledgePopulationFromID( knowledgeId );
        if( pKnowledge )
        {
            MIL_PopulationConcentration* concentration = pKnowledge->GetPopulationKnown().GetClosestConcentration( point, refDistance );
            if( concentration )
                return concentration->GetID();
        }
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::Lock
// Created: LDC 2013-05-22
// -----------------------------------------------------------------------------
void DEC_KnowledgePopulationFunctions::Lock( const MIL_AgentPion& callerAgent, int knowledgeId )
{
    auto bbKg = callerAgent.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
    {
        boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = bbKg->GetKnowledgePopulationFromID( knowledgeId );
        if( pKnowledge )
            pKnowledge->Lock();
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::Unlock
// Created: LDC 2013-05-22
// -----------------------------------------------------------------------------
void DEC_KnowledgePopulationFunctions::Unlock( const MIL_AgentPion& callerAgent, int knowledgeId )
{
    auto bbKg = callerAgent.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
    {
        boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = bbKg->GetKnowledgePopulationFromID( knowledgeId );
        if( pKnowledge )
            pKnowledge->Unlock();
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::ExtractWoundedFromCrowd
// Created: JSR 2012-01-18
// -----------------------------------------------------------------------------
bool DEC_KnowledgePopulationFunctions::ExtractWoundedFromCrowd( const MIL_AgentPion& callerAgent, unsigned int knowledgeId, const MT_Vector2D* position )
{
    if( !position )
        throw MASA_EXCEPTION( "invalid parameter." );
    auto bbKg = callerAgent.GetKnowledgeGroup()->GetKnowledge();
    if( !bbKg )
        return false;
    boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = bbKg->GetKnowledgePopulationFromID( knowledgeId );
    if( !pKnowledge )
        return false;

    MIL_Population& population = pKnowledge->GetPopulationKnown();
    unsigned int wounded = population.GetWoundedHumans();
    if( wounded > 0 )
    {
        population.ChangeComposition( population.GetHealthyHumans(), 0,  population.GetContaminatedHumans(), population.GetDeadHumans() );
        MIL_Population* newPopulation = MIL_AgentServer::GetWorkspace().GetEntityManager().CreateCrowd( population.GetType().GetName(), *position, wounded, population.GetName() + " - wounded", population.GetArmy() );
        if( newPopulation )
        {
            newPopulation->ChangeComposition( 0, wounded, 0, 0 );
            return true;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::HealWoundedHumans
// Created: MIA 2011-02-15
// -----------------------------------------------------------------------------
bool DEC_KnowledgePopulationFunctions::HealWoundedHumans( const MIL_AgentPion& caller, int knowledgeId )
{
    auto bbKg = caller.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
    {
        boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = bbKg->GetKnowledgePopulationFromID( knowledgeId );
        if( pKnowledge )
        {
            MIL_Population& population = pKnowledge->GetPopulationKnown();
            population.HealWounded();
            return true;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::CanLoadCrowdConcentration
// Created: JSR 2012-04-18
// -----------------------------------------------------------------------------
bool DEC_KnowledgePopulationFunctions::CanLoadCrowdConcentration( const DEC_Decision_ABC& callerAgent, int knowledgeId, unsigned int concentrationId )
{
    auto bbKg = callerAgent.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
    {
        boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = bbKg->GetKnowledgePopulationFromID( knowledgeId );
        if( pKnowledge )
        {
            unsigned int humans = pKnowledge->GetPopulationKnown().GetAllHumansInConcentration( concentrationId );
            return humans <= callerAgent.GetPion().GetRole< PHY_RolePion_Composantes >().GetCrowdTransporterCapacity();
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::IsValid
// Created: MIA 2013-05-17
// -----------------------------------------------------------------------------
bool DEC_KnowledgePopulationFunctions::IsValid( const DEC_Decision_ABC& callerAgent, int knowledgeId )
{
    auto bbKg = callerAgent.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
    {
        boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = bbKg->GetKnowledgePopulationFromID( knowledgeId );
        if( pKnowledge )
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::IsFlowKnown
// Created: EVH 2011-04-07
// Modified: NMI 2013-08-09
// -----------------------------------------------------------------------------
bool DEC_KnowledgePopulationFunctions::IsFlowKnown( const DEC_Decision_ABC& caller, int knowledgeId )
{
    auto bbKg = caller.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
    {
        boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = bbKg->GetKnowledgePopulationFromID( knowledgeId );
        if( pKnowledge )
            return pKnowledge->GetPopulationKnown().HasFlow();
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::ExtractDeadFromCrowd
// Created: NMI 2013-07-25
// -----------------------------------------------------------------------------
bool DEC_KnowledgePopulationFunctions::ExtractDeadFromCrowd( const MIL_AgentPion& caller,
        unsigned int knowledgeId, const MT_Vector2D* position )
{
    if( !position )
        throw MASA_EXCEPTION( "invalid parameter." );
    auto bbKg = caller.GetKnowledgeGroup()->GetKnowledge();
    if( !bbKg )
        return false;
    boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = bbKg->GetKnowledgePopulationFromID( knowledgeId );
    if( !pKnowledge )
        return false;

    MIL_Population& population = pKnowledge->GetPopulationKnown();
    unsigned int dead = population.GetDeadHumans();
    if( dead > 0 )
    {
        population.ChangeComposition( population.GetHealthyHumans(), population.GetWoundedHumans(),  population.GetContaminatedHumans(), 0 );
        MIL_Population* newPopulation = MIL_AgentServer::GetWorkspace().GetEntityManager().CreateCrowd( population.GetType().GetName(), *position, dead, population.GetName() + " - dead", population.GetArmy() );
        if( newPopulation )
        {
            newPopulation->ChangeComposition( 0, 0, 0, dead );
            return true;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::GetBrain
// Created: LDC 2013-11-22
// -----------------------------------------------------------------------------
DEC_Decision_ABC* DEC_KnowledgePopulationFunctions::GetBrain( const DEC_Decision_ABC& callerAgent, int knowledgeId )
{
    auto bbKg = callerAgent.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
    {
        boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = bbKg->GetKnowledgePopulationFromID( knowledgeId );
        if( pKnowledge )
            return &pKnowledge->GetPopulationKnown().GetDecision();
    }
    return 0;
}
