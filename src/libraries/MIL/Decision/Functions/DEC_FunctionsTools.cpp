// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_FunctionsTools.cpp $
// $Author: Jvt $
// $Modtime: 17/03/05 17:03 $
// $Revision: 5 $
// $Workfile: DEC_FunctionsTools.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "DEC_FunctionsTools.h"

#include "Knowledge/DEC_KS_KnowledgeGroupQuerier.h"
#include "Knowledge/DEC_KS_AgentQuerier.h"
#include "Knowledge/DEC_KS_ArmyQuerier.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Entities/MIL_Army.h"
#include "Entities/MIL_EntityManager.h"
#include "Decision/DEC_Tools.h"
#include "MIL_AgentServer.h"

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_FunctionsTools::GetKnowledgePopulationFromDia
// Created: NLD 2005-11-03
// -----------------------------------------------------------------------------
DEC_Knowledge_Population* DEC_FunctionsTools::GetKnowledgePopulationFromDia( const DIA_Variable_ABC& diaVar, const MIL_KnowledgeGroup& caller )
{
    assert( DEC_Tools::CheckTypeConnaissancePopulation( diaVar ) );

    uint nKnowledgeID_ = (uint)diaVar.ToPtr();
    return caller.GetKSQuerier().GetKnowledgePopulationFromID( nKnowledgeID_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_FunctionsTools::GetKnowledgeAgentFromDia
// Created: NLD 2004-03-31
// -----------------------------------------------------------------------------
DEC_Knowledge_Agent* DEC_FunctionsTools::GetKnowledgeAgentFromDia( const DIA_Variable_ABC& diaVar, const MIL_KnowledgeGroup& caller )
{
    assert( DEC_Tools::CheckTypeConnaissanceAgent( diaVar ) );

    uint nKnowledgeID_ = (uint)diaVar.ToPtr();
    return caller.GetKSQuerier().GetKnowledgeAgentFromID( nKnowledgeID_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_FunctionsTools::GetKnowledgeObjectFromDia
// Created: NLD 2004-03-31
// -----------------------------------------------------------------------------
DEC_Knowledge_Object* DEC_FunctionsTools::GetKnowledgeObjectFromDia( const DIA_Variable_ABC& diaVar, const MIL_Army& caller )
{
    assert( DEC_Tools::CheckTypeConnaissanceObjet( diaVar ) );

    uint nKnowledgeID_ = (uint)diaVar.ToPtr();
    return caller.GetKSQuerier().GetKnowledgeObjectFromID( nKnowledgeID_ ); 
}


// =============================================================================
// POPULATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_FunctionsTools::GetPopulationKnowledgeAgentFromDia
// Created: NLD 2005-12-05
// -----------------------------------------------------------------------------
MIL_AgentPion* DEC_FunctionsTools::GetPopulationKnowledgeAgentFromDia( const DIA_Variable_ABC& diaVar )
{
    assert( DEC_Tools::CheckTypePopulationConnaissanceAgent( diaVar ) );
    uint nID = ( uint )diaVar.ToPtr();
    return MIL_AgentServer::GetWorkspace().GetEntityManager().FindAgentPion( nID ); // $$$$ HLA
}

// -----------------------------------------------------------------------------
// Name: DEC_FunctionsTools::GetPopulationKnowledgeObjectFromDia
// Created: NLD 2005-12-05
// -----------------------------------------------------------------------------
MIL_RealObject_ABC* DEC_FunctionsTools::GetPopulationKnowledgeObjectFromDia( const DIA_Variable_ABC& diaVar )
{
    assert( DEC_Tools::CheckTypePopulationConnaissanceObjet( diaVar ) );
    uint nID = ( uint )diaVar.ToPtr();
    return MIL_AgentServer::GetWorkspace().GetEntityManager().FindRealObject( nID );
}
