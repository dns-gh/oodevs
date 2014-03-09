// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_KnowledgePopulationFunctions.h $
// $Author: Nld $
// $Modtime: 23/03/05 11:38 $
// $Revision: 11 $
// $Workfile: DEC_KnowledgePopulationFunctions.h $
//
// *****************************************************************************

#ifndef __DEC_KnowledgePopulationFunctions_h_
#define __DEC_KnowledgePopulationFunctions_h_

#include "Entities/Populations/MIL_Population.h"
#include "Entities/Populations/MIL_PopulationAttitude.h"
#include "Knowledge/DEC_Knowledge_Population.h"
#include "Knowledge/MIL_KnowledgeGroup.h"

class MIL_AgentPion;
class DEC_Decision_ABC;
class PHY_DotationCategory;
class TER_Localisation;
class MT_Vector2D;
class DEC_Knowledge_Population;

// =============================================================================
// Created: NLD 2004-03-31
// =============================================================================
class DEC_KnowledgePopulationFunctions
{
public:
    //! @name Functions
    //@{
    template< typename T > static std::pair< float, int > GetDominationState( const T& caller, unsigned int knowledgeId );
    template< typename T > static boost::shared_ptr< MT_Vector2D > GetBarycenter( const T& caller, unsigned int knowledgeId );
    template< typename T > static unsigned int GetAttitude( const T& caller, unsigned int knowledgeId );
                           static float GetDangerosity( const MIL_AgentPion& caller, int knowledgeId );
                           static void Recon( const MIL_AgentPion& caller, int knowledgeId );
    template< typename T > static bool IsRecon( const T& caller, unsigned int knowledgeId );
    template< typename T > static bool IsInZone( const T& caller, unsigned int knowledgeId, TER_Localisation* pLoc );
                           static bool IsPerceivedByAgent( const MIL_AgentPion& caller, int knowledgeId );
                           static boost::shared_ptr< MT_Vector2D > GetConcentrationPosition( const DEC_Decision_ABC& callerAgent, int knowledgeId, unsigned int concentrationId );
                           static unsigned int GetAllHumansInConcentration( const DEC_Decision_ABC& callerAgent, int knowledgeId, unsigned int concentrationId );
                           static unsigned int GetAllHumans( const DEC_Decision_ABC& callerAgent, int knowledgeId );
                           static boost::shared_ptr< MT_Vector2D > ClosestPoint( const MIL_AgentPion& caller, int knowledgeId );
                           static void Secure( const MIL_AgentPion& caller, int knowledgeId );
                           static boost::shared_ptr< MT_Vector2D > SecuringPoint ( const MIL_AgentPion& caller, int knowledgeId );
                           static bool IsEnemy( const MIL_AgentPion& caller, int knowledgeId );
                           static int Exterminate( MIL_AgentPion& caller, int knowledgeId, float surface, const PHY_DotationCategory* dotation );
                           static std::string GetCriticalIntelligence( const MIL_AgentPion& caller, int knowledgeId );
                           static bool IsFlowKnown( const DEC_Decision_ABC& caller, int knowledgeId );;
                           static void ChangeArmedIndividuals( const MIL_AgentPion& callerAgent, int knowledgeId, const double rArmedIndividuals );
                           static float GetCrowdAffinity( const MIL_AgentPion& callerAgent, int knowledgeId );
                           static int GetCrowdAttitude( const MIL_AgentPion& callerAgent, int knowledgeId );
                           static void StartHidingInCrowd( MIL_AgentPion& callerAgent, int knowledgeId );
                           static void StopHidingInCrowd( MIL_AgentPion& callerAgent, int knowledgeId );
                           static std::vector< DEC_Decision_ABC* > GetAgentsHiddenInCrowd( MIL_AgentPion& callerAgent, int knowledgeId );
                           static boost::shared_ptr< MT_Vector2D > GetFlowHead( const DEC_Decision_ABC& callerAgent, int knowledgeId );
                           static int GetClosestConcentration( const DEC_Decision_ABC& callerAgent, int knowledgeId, const MT_Vector2D& point, int refDistance );
                           static void Lock( const MIL_AgentPion& callerAgent, int knowledgeId );
                           static void Unlock( const MIL_AgentPion& callerAgent, int knowledgeId );
                           static bool ExtractWoundedFromCrowd( const MIL_AgentPion& callerPion, unsigned int knowledgeId, const MT_Vector2D* position );
                           static bool ExtractDeadFromCrowd( const MIL_AgentPion& caller, unsigned int knowledgeId, const MT_Vector2D* position );
    template< typename T > static std::vector< unsigned int > GetHumansFromAllTypes( const T& caller, int knowledgeId );
                           static bool HealWoundedHumans( const MIL_AgentPion& caller, int knowledgeId );
                           static bool CanLoadCrowdConcentration( const DEC_Decision_ABC& callerAgent, int knowledgeId, unsigned int concentrationId );
                           static bool IsValid( const DEC_Decision_ABC& callerAgent, int knowledgeId );
                           static DEC_Decision_ABC* GetBrain( const DEC_Decision_ABC& callerAgent, int knowledgeId );
    //@}
};

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
// Name: DEC_KnowledgePopulationFunctions::GetHumansFromAllTypes
// Created: NMI 2013-08-01
// -----------------------------------------------------------------------------
template< typename T >
std::vector< unsigned int > DEC_KnowledgePopulationFunctions::GetHumansFromAllTypes( const T& caller, int knowledgeId )
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
        }
    }
    return vecHumans;
}

#endif // __DEC_KnowledgePopulationFunctions_h_
