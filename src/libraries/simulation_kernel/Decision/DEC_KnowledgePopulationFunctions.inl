// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Decision/Functions/DEC_KnowledgePopulationFunctions.inl $
// $Author: Nld $
// $Modtime: 16/03/05 10:58 $
// $Revision: 9 $
// $Workfile: DEC_KnowledgePopulationFunctions.inl $
//
// *****************************************************************************

#include "DEC_FunctionsTools.h"
#include "Knowledge/DEC_Knowledge_Population.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Populations/MIL_PopulationType.h"
#include "Entities/Populations/MIL_PopulationAttitude.h"

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::GetDominationState
// Created: NLD 2006-02-22
// -----------------------------------------------------------------------------
template< typename T > 
std::pair< float, int > DEC_KnowledgePopulationFunctions::GetDominationState( const T& caller, unsigned int knowledgeId )
{
    auto bbKg = caller.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
    {
        boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = bbKg->GetKnowledgePopulationFromID( knowledgeId );
        if( pKnowledge )
            return std::pair< float, int >( float( pKnowledge->GetDominationState() ), knowledgeId );
    }
    return std::pair< float, int >( 0.f, knowledgeId );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::GetBarycenter
// Created: JSR 2013-07-25
// -----------------------------------------------------------------------------
template< typename T >
boost::shared_ptr< MT_Vector2D > DEC_KnowledgePopulationFunctions::GetBarycenter( const T& caller, unsigned int knowledgeId )
{
    auto bbKg = caller.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
    {
        boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = bbKg->GetKnowledgePopulationFromID( knowledgeId );
        if( pKnowledge )
            return pKnowledge->GetPopulationKnown().GetBarycenter();
    }
    return boost::shared_ptr< MT_Vector2D >();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::GetAttitude
// Created: SBO 2006-04-05
// -----------------------------------------------------------------------------
template< typename T >
unsigned int DEC_KnowledgePopulationFunctions::GetAttitude( const T& caller, unsigned int knowledgeId )
{
    auto bbKg = caller.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
    {
        boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = bbKg->GetKnowledgePopulationFromID( knowledgeId );
        if( pKnowledge )
            return pKnowledge->GetAttitude().GetID();
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::IsRecon
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
template< typename T > 
bool DEC_KnowledgePopulationFunctions::IsRecon( const T& caller, unsigned int knowledgeId )
{
    auto bbKg = caller.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
    {
        boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = bbKg->GetKnowledgePopulationFromID( knowledgeId );
        if( pKnowledge )
            return pKnowledge->IsRecon();
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::IsInZone
// Created: NLD 2005-12-02
// -----------------------------------------------------------------------------
template< typename T > 
bool DEC_KnowledgePopulationFunctions::IsInZone( const T& caller, unsigned int knowledgeId, TER_Localisation* pLoc )
{
    auto bbKg = caller.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
    {
        boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = bbKg->GetKnowledgePopulationFromID( knowledgeId );
        if( pKnowledge )
            return  pKnowledge->IsInZone( *pLoc );
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::ExtractDeadFromCrowd
// Created: NMI 2013-07-25
// -----------------------------------------------------------------------------
template< typename T >
bool DEC_KnowledgePopulationFunctions::ExtractDeadFromCrowd( const T& caller, unsigned int knowledgeId, const MT_Vector2D* position )
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
// Name: DEC_KnowledgePopulationFunctions::HasWoundedHumans
// Created: MIA 2011-01-20
// Modified: NMI 2013-07-29
// -----------------------------------------------------------------------------
template< typename T >
bool DEC_KnowledgePopulationFunctions::HasWoundedHumans( const T& caller, int knowledgeId )
{
    auto bbKg = caller.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
    {
        boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = bbKg->GetKnowledgePopulationFromID( knowledgeId );
        if( pKnowledge )
        {
            MIL_Population& population = pKnowledge->GetPopulationKnown();
            return population.GetWoundedHumans() > 0;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::HasDeadHumans
// Created: NMI 2013-07-29
// -----------------------------------------------------------------------------
template< typename T >
bool DEC_KnowledgePopulationFunctions::HasDeadHumans( const T& caller, int knowledgeId )
{
    auto bbKg = caller.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
    {
        boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = bbKg->GetKnowledgePopulationFromID( knowledgeId );
        if( pKnowledge )
        {
            MIL_Population& population = pKnowledge->GetPopulationKnown();
            return population.GetDeadHumans() > 0;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::GetHumansFromAllTypes
// Created: NMI 2013-08-01
// -----------------------------------------------------------------------------
template< typename T >
std::vector< unsigned int > DEC_OrdersFunctions::GetHumansFromAllTypes( const T& caller, int knowledgeId )
{
    std::vector< unsigned int > vecHumans;
    auto bbKg = caller.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
    {
        boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = bbKg->GetKnowledgePopulationFromID( knowledgeId );
        if( pKnowledge )
        {
            MIL_Population& population = pKnowledge->GetPopulationKnown();
            vecHumans.push_back( population.GetHealthyHumans() );
            vecHumans.push_back( population.GetWoundedHumans() );
            vecHumans.push_back( population.GetContaminatedHumans() );
            vecHumans.push_back( population.GetDeadHumans() );
            return vecHumans;
        }
    }
    return false;
}