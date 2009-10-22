// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_KnowledgeObjectFunctions.cpp $
// $Author: Nld $
// $Modtime: 23/03/05 13:57 $
// $Revision: 9 $
// $Workfile: DEC_KnowledgeObjectFunctions.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_KnowledgeObjectFunctions.h"

#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Objects/Object.h"
#include "Entities/Objects/MIL_ObjectManipulator_ABC.h"

#include "Entities/Objects/ContaminationCapacity.h"
#include "Entities/Objects/DecontaminationCapacity.h"
#include "Entities/Objects/OccupantAttribute.h"
#include "Entities/Objects/PopulationAttribute.h"
#include "Entities/Objects/SupplyRouteAttribute.h"

#include "Entities/Objects/MIL_ObjectFilter.h"

#include "Entities/MIL_Army.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::Recon
// Created: NLD 2004-10-29
// -----------------------------------------------------------------------------
void DEC_KnowledgeObjectFunctions::Recon( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid() )
        pKnowledge->Recon( callerAgent );
}

namespace 
{
    template< typename Capacity >
    Capacity* IsValidObjectCapacity( const boost::shared_ptr< DEC_Knowledge_Object >& pKnowledge )
    {
        if ( !pKnowledge || !pKnowledge->IsValid() )
            return 0;

        MIL_Object_ABC* pObject = pKnowledge->GetObjectKnown();
        if ( pObject )
            return pObject->Retrieve< Capacity >();
        return 0;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::QueueForDecontamination
// Created: NLD 2004-11-02
// -----------------------------------------------------------------------------
int DEC_KnowledgeObjectFunctions::QueueForDecontamination( MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( DecontaminationCapacity* pCapacity = IsValidObjectCapacity< DecontaminationCapacity >( pKnowledge ) )
    {
        pCapacity->QueueForDecontamination( callerAgent );
        return int( eQueryValid );
    }
    return int( eQueryInvalid );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::CanBeAnimated
// Created: NLD 2004-11-03
// -----------------------------------------------------------------------------
bool DEC_KnowledgeObjectFunctions::CanBeAnimated( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid() )
        if( const MIL_Object_ABC* object = pKnowledge->GetObjectKnown() )
            return (*object)().CanBeAnimatedBy( callerAgent );
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::DecontaminateZone
// Created: NLD 2005-03-22
// -----------------------------------------------------------------------------
void DEC_KnowledgeObjectFunctions::DecontaminateZone( const MIL_AgentPion& callerAgent, const TER_Localisation* location )
{
    assert( location );

    MIL_ObjectFilter filter;
    filter.Set( "zone nbc" ); // $$$$ TODO JCR ?? 
    filter.Set( "nuage nbc" ); // $$$$ TODO JCR ?? 

    T_KnowledgeObjectVector knownObjects;
    callerAgent.GetArmy().GetKnowledge().GetObjects( knownObjects, filter );
    for( CIT_KnowledgeObjectVector it = knownObjects.begin(); it != knownObjects.end(); ++it )
        if( location->IsIntersecting( (*it)->GetLocalisation() ) )
            if( ContaminationCapacity* pCapacity = IsValidObjectCapacity< ContaminationCapacity >( *it ) )
                pCapacity->DecontaminateZone( *location );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::DamageObject
// Created: SBO 2006-01-23
// -----------------------------------------------------------------------------
int DEC_KnowledgeObjectFunctions::DamageObject( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge, float factor )
{
    if( pKnowledge && pKnowledge->IsValid() )
        if( MIL_Object_ABC* pObject = pKnowledge->GetObjectKnown() )
            if( (*pObject)().CanBePerceived() )
            {
                (*pObject)().Destroy( factor );
                return int( eQueryValid );
            }
    return int( eQueryInvalid );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::CanBeOccupied
// Created: NLD 2004-11-26
// -----------------------------------------------------------------------------
bool DEC_KnowledgeObjectFunctions::CanBeOccupied( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid() )
        if( const OccupantAttribute* pAttribute = pKnowledge->GetObjectKnown()->RetrieveAttribute< OccupantAttribute >() )
        {
            const MIL_Agent_ABC* occupant = pAttribute->GetOccupant();
            return !occupant || occupant == &callerAgent;
        }
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::CanBeBypassed
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
bool DEC_KnowledgeObjectFunctions::CanBeBypassed( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid() )
        if( const MIL_Object_ABC* pObject = pKnowledge->GetObjectKnown() )
            return (*pObject)().CanBeBypassed();
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::EquipLogisticRoute
// Created: NLD 2005-02-18
// -----------------------------------------------------------------------------
int DEC_KnowledgeObjectFunctions::EquipLogisticRoute( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid() )
        if( MIL_Object_ABC* pObject = pKnowledge->GetObjectKnown() )
            if( SupplyRouteAttribute* pAttribute = pObject->RetrieveAttribute< SupplyRouteAttribute >() )
            {
                pAttribute->Equip();
                return int( eQueryValid );
            }
    return int( eQueryInvalid );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::SetExitingPopulationDensity
// Created: NLD 2006-03-08
// -----------------------------------------------------------------------------
void DEC_KnowledgeObjectFunctions::SetExitingPopulationDensity( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge, float density )
{
    if( pKnowledge && pKnowledge->IsValid() )
        if( MIL_Object_ABC* pObject = pKnowledge->GetObjectKnown() )
            if( PopulationAttribute* pAttribute = pObject->RetrieveAttribute< PopulationAttribute >() )
                pAttribute->SetDensity( density );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::ResetExitingPopulationDensity
// Created: NLD 2006-03-08
// -----------------------------------------------------------------------------
void DEC_KnowledgeObjectFunctions::ResetExitingPopulationDensity( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid() )
        if( MIL_Object_ABC* pObject = pKnowledge->GetObjectKnown() )
            if( PopulationAttribute* pAttribute = pObject->RetrieveAttribute< PopulationAttribute >() )
                pAttribute->Reset();
}
