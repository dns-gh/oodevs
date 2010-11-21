// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_AutomateFunctions.cpp $
// $Author: Jvt $
// $Modtime: 11/05/05 11:03 $
// $Revision: 20 $
// $Workfile: DEC_AutomateFunctions.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_PopulationFunctions.h"
#include "DEC_FunctionsTools.h"
#include "Entities/MIL_Army.h"
#include "Entities/Populations/MIL_PopulationElement_ABC.h"
#include "Entities/MIL_EntityVisitor_ABC.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Agents/Units/Categories/PHY_RoePopulation.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Objects/MIL_ObjectFilter.h"
#include "Entities/Objects/MIL_ObjectManipulator_ABC.h"
#include "Entities/Populations/MIL_PopulationAttitude.h"
#include "Network/NET_Publisher_ABC.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "Knowledge/QueryValidity.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "protocol/ClientSenders.h"
#include "simulation_terrain/TER_ObjectManager.h"
#include "simulation_terrain/TER_World.h"

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::DecisionalState
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
void DEC_PopulationFunctions::DecisionalState( const MIL_Population& callerPopulation, const std::string& key, const std::string& value )
{
    client::DecisionalState msg;
    msg().mutable_source()->mutable_crowd()->set_id( callerPopulation.GetID() );
    msg().set_key  ( key.c_str() );
    msg().set_value( value.c_str() );
    msg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::SetAttitude
// Created: SBO 2005-11-23
// -----------------------------------------------------------------------------
void DEC_PopulationFunctions::SetAttitude( MIL_Population& callerPopulation, unsigned int attitudeId )
{
    const MIL_PopulationAttitude* pAttitude = MIL_PopulationAttitude::Find( attitudeId );
    assert( pAttitude );
    callerPopulation.SetAttitude( *pAttitude );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::GetAttitude
// Created: NLD 2005-12-02
// -----------------------------------------------------------------------------
unsigned int DEC_PopulationFunctions::GetAttitude( const MIL_Population& callerPopulation )
{
    return callerPopulation.GetAttitude().GetID() ;
}

namespace 
{
    class PopulationVisitor : public MIL_EntityVisitor_ABC< MIL_PopulationElement_ABC >
    {
    public:
        explicit PopulationVisitor( std::vector< boost::shared_ptr< TER_Localisation > >& locations )
            : locations_ ( locations ) 
        {
            // NOTHING
        }

        void Visit( const MIL_PopulationElement_ABC& element )
        {
            boost::shared_ptr< TER_Localisation > location( new TER_Localisation( element.GetLocation() ) );
            locations_.push_back( location );
        }
    private:
        std::vector< boost::shared_ptr< TER_Localisation > >& locations_;
    };
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::GetCurrentLocations
// Created: JCR 2010-09-15
// -----------------------------------------------------------------------------
std::vector< boost::shared_ptr< TER_Localisation > > DEC_PopulationFunctions::GetCurrentLocations( const MIL_Population& callerPopulation )
{
    std::vector< boost::shared_ptr< TER_Localisation > > locations;
    PopulationVisitor       visitor( locations );

    callerPopulation.Apply( visitor );
    return locations;
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::GetKnowledgeAgentRoePopulation
// Created: NLD 2005-12-01
// -----------------------------------------------------------------------------
int DEC_PopulationFunctions::GetKnowledgeAgentRoePopulation( unsigned int agentId )
{
    const MIL_AgentPion* pAgent = MIL_AgentServer::GetWorkspace().GetEntityManager().FindAgentPion( agentId );
    assert( pAgent  );
    return pAgent->GetRole< DEC_RolePion_Decision >().GetRoePopulation().GetID();
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::GetObjectsInZone
// Created: NLD 2005-12-05
// -----------------------------------------------------------------------------
std::vector< boost::shared_ptr< DEC_Knowledge_Object > > DEC_PopulationFunctions::GetObjectsInZone( const MIL_Population& caller, const TER_Localisation* pZone, const std::vector< std::string >& parameters )
{
    typedef std::vector< boost::shared_ptr< DEC_Knowledge_Object > >::iterator IT_KnowledgeObject;

    assert( pZone );
    MIL_ObjectFilter filter( parameters );
    std::vector< boost::shared_ptr< DEC_Knowledge_Object > > knowledges; //T_KnowledgeObjectDiaIDVector
    caller.GetArmy().GetKnowledge().GetObjectsInZone( knowledges, filter, *pZone );
    for ( IT_KnowledgeObject it = knowledges.begin(); it != knowledges.end(); )
    {
        if( ! IsKnowledgeObjectValid( *it ) )
            it = knowledges.erase( it );
        else
            ++it;
    }
    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::GetKnowledgeObjectLocalisation
// Created: NLD 2005-12-05
// -----------------------------------------------------------------------------
const TER_Localisation* DEC_PopulationFunctions::GetKnowledgeObjectLocalisation( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid() )
        return &pKnowledge->GetLocalisation();
    return 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::IsKnowledgeObjectValid
// Created: NLD 2005-12-05
// -----------------------------------------------------------------------------
bool DEC_PopulationFunctions::IsKnowledgeObjectValid( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    bool isValid = pKnowledge && pKnowledge->IsValid();
    const MIL_Object_ABC* pObject = pKnowledge->GetObjectKnown();
    return isValid && pObject && (*pObject)().CanBePerceived();
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::DamageObject
// Created: NLD 2005-12-05
// -----------------------------------------------------------------------------
int DEC_PopulationFunctions::DamageObject( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge, double damageFactor )
{
    if( !( pKnowledge && pKnowledge->IsValid() ) )
        return  (int)eQueryInvalid;
    MIL_Object_ABC* pObject = pKnowledge->GetObjectKnown();
    if( !pObject )
        return  (int)eQueryInvalid;
    (*pObject)().Destroy( damageFactor );
    return (int)eQueryValid;
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::GetKnowledgeObjectDistance
// Created: SBO 2005-12-13
// -----------------------------------------------------------------------------
float DEC_PopulationFunctions::GetKnowledgeObjectDistance( const MIL_Population& callerPopulation, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( !( pKnowledge && pKnowledge->IsValid() ) )
        return std::numeric_limits< float >::max();
    return (float)callerPopulation.GetDistanceTo( pKnowledge->GetLocalisation() ) ;
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::GetKnowledgeObjectClosestPoint
// Created: SBO 2005-12-13
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_PopulationFunctions::GetKnowledgeObjectClosestPoint( const MIL_Population& callerPopulation, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( !( pKnowledge && pKnowledge->IsValid() ) )
        return boost::shared_ptr<MT_Vector2D>();
    else
    {
        MT_Vector2D* pPoint = new MT_Vector2D( callerPopulation.GetClosestPoint( pKnowledge->GetLocalisation() ) );
        return boost::shared_ptr<MT_Vector2D>( pPoint );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::IsEnemy
// Created: HME 2005-12-29
// -----------------------------------------------------------------------------
int DEC_PopulationFunctions::IsEnemy( const MIL_Population& callerPopulation, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( !( pKnowledge && pKnowledge->IsValid() ) )
        return int( eTristate_DontKnow );
    return int( pKnowledge->IsAnEnemy( callerPopulation.GetArmy() ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::NotifyDominationStateChanged
// Created: NLD 2006-02-22
// -----------------------------------------------------------------------------
void DEC_PopulationFunctions::NotifyDominationStateChanged( MIL_Population& callerPopulation, double dominationState )
{
    callerPopulation.GetDecision().NotifyDominationStateChanged( dominationState );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::GetSzName
// Created: LDC 2009-04-09
// -----------------------------------------------------------------------------
std::string DEC_PopulationFunctions::GetSzName( MIL_Population& callerPopulation )
{
    return callerPopulation.GetDecision().GetName();
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::GetMission
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
boost::shared_ptr< MIL_Mission_ABC > DEC_PopulationFunctions::GetMission( DEC_Decision_ABC* pAgent )
{
    return pAgent->GetMission();
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::SetMission
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_PopulationFunctions::SetMission( DEC_Decision_ABC* object, boost::shared_ptr< MIL_Mission_ABC > mission )
{
    object->SetMission( mission );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::GetBarycenter
// Created: MGD 2010-09-28
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_PopulationFunctions::GetBarycenter( const MIL_Population& callerPopulation )
{
    return callerPopulation.GetBarycenter();
}
