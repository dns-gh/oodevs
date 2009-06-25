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
void DEC_KnowledgeObjectFunctions::Recon( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    DEC_Knowledge_Object* pKnowledge = DEC_FunctionsTools::GetKnowledgeObjectFromDia( call.GetParameter( 0 ), callerAgent.GetArmy() );
    if( pKnowledge )
        pKnowledge->Recon( callerAgent );
}

namespace 
{
    template< typename Capacity >
    Capacity* IsValidObjectCapacity( DEC_Knowledge_Object* pKnowledge )
    {
        if ( !pKnowledge )
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
void DEC_KnowledgeObjectFunctions::QueueForDecontamination( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    DEC_Knowledge_Object* pKnowledge = DEC_FunctionsTools::GetKnowledgeObjectFromDia( call.GetParameter( 0 ), callerAgent.GetArmy() );
    DecontaminationCapacity* pCapacity = IsValidObjectCapacity< DecontaminationCapacity >( pKnowledge );
    if( !pCapacity )
    {
        call.GetResult().SetValue( eQueryInvalid );
        return;    
    }
    call.GetResult().SetValue( eQueryValid );
    pCapacity->QueueForDecontamination( callerAgent );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::CanBeAnimated
// Created: NLD 2004-11-03
// -----------------------------------------------------------------------------
void DEC_KnowledgeObjectFunctions::CanBeAnimated( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    DEC_Knowledge_Object* pKnowledge = DEC_FunctionsTools::GetKnowledgeObjectFromDia( call.GetParameter( 0 ), callerAgent.GetArmy() );
    if( !( pKnowledge && pKnowledge->GetObjectKnown() ) )
    {
        call.GetResult().SetValue( false );
        return;    
    }

    MIL_Object_ABC& object = *pKnowledge->GetObjectKnown();
    call.GetResult().SetValue( object().CanBeAnimatedBy( callerAgent ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::DecontaminateZone
// Created: NLD 2005-03-22
// -----------------------------------------------------------------------------
void DEC_KnowledgeObjectFunctions::DecontaminateZone( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypeLocalisation( call.GetParameter( 0 ) ) );
    const TER_Localisation* pZoneDecontaminated = call.GetParameter( 0 ).ToUserPtr( pZoneDecontaminated );

    MIL_ObjectFilter filter;
    filter.Set( "zone nbc" ); // $$$$ TODO JCR ?? 
    filter.Set( "nuage nbc" ); // $$$$ TODO JCR ?? 

    T_KnowledgeObjectVector knowledges;
    callerAgent.GetArmy().GetKnowledge().GetObjects( knowledges, filter );
    for( CIT_KnowledgeObjectVector it = knowledges.begin(); it != knowledges.end(); ++it )
    {
        if ( pZoneDecontaminated->IsIntersecting( (*it)->GetLocalisation() ) )
        {
            ContaminationCapacity* pCapacity = IsValidObjectCapacity< ContaminationCapacity >( *it );
            if( !pCapacity )
                continue;
            pCapacity->DecontaminateZone( *pZoneDecontaminated );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::DamageObject
// Created: SBO 2006-01-23
// -----------------------------------------------------------------------------
void DEC_KnowledgeObjectFunctions::DamageObject( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    DEC_Knowledge_Object* pKnowledge = DEC_FunctionsTools::GetKnowledgeObjectFromDia( call.GetParameter( 0 ), callerAgent.GetArmy() );
    if( ! pKnowledge )
        call.GetResult().SetValue( eQueryInvalid );
    else
    {
        MIL_Object_ABC* pObject = pKnowledge->GetObjectKnown();
        if ( pObject && (*pObject)().CanBePerceived() )
        {
            call.GetResult().SetValue( eQueryValid );
            float rDamageFactor = call.GetParameter( 1 ).ToFloat();
            (*pObject)().Destroy( rDamageFactor );
        }
        else
            call.GetResult().SetValue( eQueryInvalid );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::CanBeOccupied
// Created: NLD 2004-11-26
// -----------------------------------------------------------------------------
void DEC_KnowledgeObjectFunctions::CanBeOccupied( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    MIL_Object_ABC*         pObject;
    DEC_Knowledge_Object*   pKnowledge = DEC_FunctionsTools::GetKnowledgeObjectFromDia( call.GetParameter( 0 ), callerAgent.GetArmy() );
    if( pKnowledge && ( pObject = pKnowledge->GetObjectKnown() ) )
    {
        const OccupantAttribute* pAttribute = pObject->RetrieveAttribute< OccupantAttribute >();
        if( pAttribute )
        {
            const MIL_Agent_ABC* occupant = pAttribute->GetOccupant();
            call.GetResult().SetValue( !occupant || occupant == &callerAgent );
            return;
        }
    }
    call.GetResult().SetValue( false );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::CanBeBypassed
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
void DEC_KnowledgeObjectFunctions::CanBeBypassed( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    DEC_Knowledge_Object* pKnowledge = DEC_FunctionsTools::GetKnowledgeObjectFromDia( call.GetParameter( 0 ), callerAgent.GetArmy() );
    
    if( pKnowledge )
    {
        MIL_Object_ABC* pObject = pKnowledge->GetObjectKnown();
        if( pObject )
            return call.GetResult().SetValue( (*pObject)().CanBeBypassed() );
    }
    call.GetResult().SetValue( (int)0 );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::CanBeMined
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
void DEC_KnowledgeObjectFunctions::CanBeMined( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    DEC_Knowledge_Object* pKnowledge = DEC_FunctionsTools::GetKnowledgeObjectFromDia( call.GetParameter( 0 ), callerAgent.GetArmy() );
    if( !( pKnowledge && pKnowledge->GetObjectKnown() ) )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        call.GetResult().SetValue( (int)0 );
        return;
    }
    MIL_Object_ABC& object = *pKnowledge->GetObjectKnown();
    call.GetParameter( 1 ).SetValue( eQueryValid );
    call.GetResult().SetValue( object().CanBeMined() );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::CanBeActivated
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
void DEC_KnowledgeObjectFunctions::CanBeActivated( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    DEC_Knowledge_Object* pKnowledge = DEC_FunctionsTools::GetKnowledgeObjectFromDia( call.GetParameter( 0 ), callerAgent.GetArmy() );
    
    if( !( pKnowledge && pKnowledge->GetObjectKnown() ) )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        call.GetResult().SetValue( (int)0 );
        return;
    }
    MIL_Object_ABC& object = *pKnowledge->GetObjectKnown();
    call.GetParameter( 1 ).SetValue( eQueryValid );
    call.GetResult().SetValue( object().CanBeActivated() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::EquipLogisticRoute
// Created: NLD 2005-02-18
// -----------------------------------------------------------------------------
void DEC_KnowledgeObjectFunctions::EquipLogisticRoute( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
	DEC_Knowledge_Object* pKnowledge = DEC_FunctionsTools::GetKnowledgeObjectFromDia( call.GetParameter( 0 ), callerAgent.GetArmy() );

    MIL_Object_ABC* pObject = 0;
    if( pKnowledge && ( pObject = pKnowledge->GetObjectKnown() ) )
    {
        SupplyRouteAttribute* pAttribute = pObject->RetrieveAttribute< SupplyRouteAttribute >();
        if( pAttribute )
        {
            call.GetResult().SetValue( eQueryValid );
            pAttribute->Equip();
            return;
        }
    }
    call.GetResult().SetValue( eQueryInvalid );
}


// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::SetExitingPopulationDensity
// Created: NLD 2006-03-08
// -----------------------------------------------------------------------------
void DEC_KnowledgeObjectFunctions::SetExitingPopulationDensity( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    DEC_Knowledge_Object* pKnowledge = DEC_FunctionsTools::GetKnowledgeObjectFromDia( call.GetParameter( 0 ), callerAgent.GetArmy() );
    MIL_Object_ABC* pObject;
    if( pKnowledge && ( pObject = pKnowledge->GetObjectKnown() ) )
    {
        PopulationAttribute* pAttribute = pObject->RetrieveAttribute< PopulationAttribute >();
        if( pAttribute )
            pAttribute->SetDensity( call.GetParameter( 1 ).ToFloat() );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::ResetExitingPopulationDensity
// Created: NLD 2006-03-08
// -----------------------------------------------------------------------------
void DEC_KnowledgeObjectFunctions::ResetExitingPopulationDensity( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    DEC_Knowledge_Object* pKnowledge = DEC_FunctionsTools::GetKnowledgeObjectFromDia( call.GetParameter( 0 ), callerAgent.GetArmy() );
    MIL_Object_ABC* pObject;
    if( pKnowledge && ( pObject = pKnowledge->GetObjectKnown() ) )
    {
        PopulationAttribute* pAttribute = pObject->RetrieveAttribute< PopulationAttribute >();
        if( pAttribute )
            pAttribute->Reset();
    }
}
