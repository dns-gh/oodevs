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

// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_KnowledgePopulationFunctions::IsRecon
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
template< typename T > 
void DEC_KnowledgePopulationFunctions::IsRecon( DIA_Call_ABC& call, const T& caller )
{
    DEC_Knowledge_Population* pKnowledge = DEC_FunctionsTools::GetKnowledgePopulationFromDia( call.GetParameter( 0 ), caller.GetKnowledgeGroup() );
    if( !pKnowledge )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        call.GetResult().SetValue( false );
        return;
    }
    call.GetParameter( 1 ).SetValue( eQueryValid );
    call.GetResult().SetValue( pKnowledge->IsRecon() );
}

