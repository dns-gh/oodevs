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

#include "MIL_pch.h"
#include "DEC_PopulationFunctions.h"

#include "Entities/Populations/MIL_Population.h"
#include "Entities/Populations/MIL_PopulationAttitude.h"
#include "Entities/Populations/DEC_PopulationKnowledge.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Agents/Units/Categories/PHY_RoePopulation.h"
#include "Entities/Objects/MIL_RealObject_ABC.h"
#include "Entities/Objects/MIL_RealObjectTypeFilter.h"
#include "Tools/MIL_Tools.h"
#include "Decision/DEC_Tools.h"
#include "DEC_FunctionsTools.h"

//-----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::Debug
// Created: NLD 2002-12-17
//-----------------------------------------------------------------------------
void DEC_PopulationFunctions::Debug( DIA_Call_ABC& call, const MIL_Population& callerPopulation )
{
    if( !MIL_AgentServer::GetWorkspace().GetConfig().UseDecDebug() )
        return;

    std::string msg( call.GetParameter( 0 ).ToString() );
    MT_LOG_INFO_MSG( MT_FormatString( "Population %d says : [%s]", callerPopulation.GetID(), msg.c_str() ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::SetPionMaxSpeed
// Created: NLD 2005-10-24
// -----------------------------------------------------------------------------
void DEC_PopulationFunctions::SetPionMaxSpeed( DIA_Call_ABC& call, MIL_Population& callerPopulation )
{
    const MT_Float rSpeed = call.GetParameter( 0 ).ToFloat();
    assert( rSpeed >= 0. );
    callerPopulation.SetPionMaxSpeed( MIL_Tools::ConvertSpeedMosToSim( rSpeed ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::ResetPionMaxSpeed
// Created: NLD 2005-10-24
// -----------------------------------------------------------------------------
void DEC_PopulationFunctions::ResetPionMaxSpeed( DIA_Call_ABC& /*call*/, MIL_Population& callerPopulation )
{
    callerPopulation.ResetPionMaxSpeed();
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::SetAttitude
// Created: SBO 2005-11-23
// -----------------------------------------------------------------------------
void DEC_PopulationFunctions::SetAttitude( DIA_Call_ABC& call, MIL_Population& callerPopulation )
{
    const MIL_PopulationAttitude* pAttitude = MIL_PopulationAttitude::Find( call.GetParameter( 0 ).ToId() );
    assert( pAttitude );
    callerPopulation.SetAttitude( *pAttitude );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::GetAttitude
// Created: NLD 2005-12-02
// -----------------------------------------------------------------------------
void DEC_PopulationFunctions::GetAttitude( DIA_Call_ABC& call, MIL_Population& callerPopulation )
{
    call.GetResult().SetValue( (int)callerPopulation.GetAttitude().GetID() );
}

// =============================================================================
// KNOWLEDGE AGENTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::GetKnowledgeAgentRoePopulation
// Created: NLD 2005-12-01
// -----------------------------------------------------------------------------
void DEC_PopulationFunctions::GetKnowledgeAgentRoePopulation( DIA_Call_ABC& call, const MIL_Population& /*callerPopulation*/ )
{
    const MIL_AgentPion* pAgent = DEC_FunctionsTools::GetPopulationKnowledgeAgentFromDia( call.GetParameter( 0 ) );
    assert( pAgent  );
    call.GetResult().SetValue( (int)pAgent->GetRole< DEC_RolePion_Decision >().GetRoePopulation().GetID() );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::GetPionsAttacking
// Created: NLD 2005-11-10
// -----------------------------------------------------------------------------
void DEC_PopulationFunctions::GetPionsAttacking( DIA_Call_ABC& call, const MIL_Population& callerPopulation )
{
    T_PopulationKnowledgeAgentDiaIDVector attackers;
    callerPopulation.GetKnowledge().GetPionsAttacking( attackers );

    DIA_Variable_ObjectList& diaObjectList = static_cast< DIA_Variable_ObjectList& >( call.GetResult() );
    diaObjectList.SetValueUserType( attackers, DEC_Tools::GetTypePopulationConnaissanceAgent() );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::GetPionsSecuring
// Created: NLD 2005-12-02
// -----------------------------------------------------------------------------
void DEC_PopulationFunctions::GetPionsSecuring( DIA_Call_ABC& call, const MIL_Population& callerPopulation )
{
    T_PopulationKnowledgeAgentDiaIDVector securers;
    callerPopulation.GetKnowledge().GetPionsSecuring( securers );

    DIA_Variable_ObjectList& diaObjectList = static_cast< DIA_Variable_ObjectList& >( call.GetResult() );
    diaObjectList.SetValueUserType( securers, DEC_Tools::GetTypePopulationConnaissanceAgent() );
}

// =============================================================================
// KNOWLEDGE OBJECTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::GetObjectsInZone
// Created: NLD 2005-12-05
// -----------------------------------------------------------------------------
void DEC_PopulationFunctions::GetObjectsInZone( DIA_Call_ABC& call, const MIL_Population& /*callerPopulation*/ )
{
    assert( DEC_Tools::CheckTypeLocalisation( call.GetParameter( 0 ) ) );

    MIL_RealObjectTypeFilter objectsFilter( call.GetParameters(), 1 );

    const TER_Localisation* pZone = call.GetParameter( 0 ).ToUserPtr( pZone );
    assert( pZone );

    T_PopulationKnowledgeObjectDiaIDVector knowledges;

    TER_Object_ABC::T_ObjectVector objects;
    TER_World::GetWorld().GetObjectManager().GetListWithinLocalisation( *pZone, objects );
    for( TER_Object_ABC::CIT_ObjectVector it = objects.begin(); it != objects.end(); ++it )
    {
        MIL_Object_ABC& object = static_cast< MIL_Object_ABC& >( **it );
        if( !object.IsReal() )
            continue;
        MIL_RealObject_ABC& realObject = static_cast< MIL_RealObject_ABC& >( object );
        if( !realObject.CanBePerceived() || !objectsFilter.Test( realObject.GetType() ) )
            continue;

        knowledges.push_back( (void*)realObject.GetID() );
    }

    DIA_Variable_ObjectList& diaObjectList = static_cast< DIA_Variable_ObjectList& >( call.GetResult() );
    diaObjectList.SetValueUserType( knowledges, DEC_Tools::GetTypePopulationConnaissanceObjet() );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::GetKnowledgeObjectLocalisation
// Created: NLD 2005-12-05
// -----------------------------------------------------------------------------
void DEC_PopulationFunctions::GetKnowledgeObjectLocalisation( DIA_Call_ABC& call, const MIL_Population& /*callerPopulation*/ )
{
    const MIL_RealObject_ABC* pObject = DEC_FunctionsTools::GetPopulationKnowledgeObjectFromDia( call.GetParameter( 0 ) );
    if( !pObject || !pObject->CanBePerceived() )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        call.GetResult().SetValue( (int)0 );
        return;
    }

    call.GetParameter( 1 ).SetValue( eQueryValid );
    call.GetResult().SetValue( (void*)&pObject->GetLocalisation(), &DEC_Tools::GetTypeLocalisation() );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::IsKnowledgeObjectValid
// Created: NLD 2005-12-05
// -----------------------------------------------------------------------------
void DEC_PopulationFunctions::IsKnowledgeObjectValid( DIA_Call_ABC& call, const MIL_Population& /*callerPopulation*/ )
{
    const MIL_RealObject_ABC* pObject = DEC_FunctionsTools::GetPopulationKnowledgeObjectFromDia( call.GetParameter( 0 ) );
    if( !pObject || !pObject->CanBePerceived() )
        call.GetResult().SetValue( false );
    else
        call.GetResult().SetValue( true );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::DamageObject
// Created: NLD 2005-12-05
// -----------------------------------------------------------------------------
void DEC_PopulationFunctions::DamageObject( DIA_Call_ABC& call, const MIL_Population& callerPopulation )
{
    MIL_RealObject_ABC* pObject = DEC_FunctionsTools::GetPopulationKnowledgeObjectFromDia( call.GetParameter( 0 ) );
    if( !pObject || !pObject->CanBePerceived() )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        call.GetResult().SetValue( (int)0 );
        return;
    }

    call.GetParameter( 1 ).SetValue( eQueryValid );

    float rDamageFactor = call.GetParameter( 2 ).ToFloat();
    pObject->Destroy( rDamageFactor );
}
