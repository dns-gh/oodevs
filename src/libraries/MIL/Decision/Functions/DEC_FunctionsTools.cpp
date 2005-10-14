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
#include "Decision/DEC_Tools.h"

// =============================================================================
// TOOLS
// =============================================================================

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

