// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Decision/Functions/DEC_KnowledgeObjectFunctions.inl $
// $Author: Nld $
// $Modtime: 16/03/05 10:58 $
// $Revision: 9 $
// $Workfile: DEC_KnowledgeObjectFunctions.inl $
//
// *****************************************************************************

#include "DEC_FunctionsTools.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/DEC_Knowledge_ObjectAttributeCrossingSite.h"
#include "Knowledge/DEC_Knowledge_ObjectAttributeSupplyRoute.h"


// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_KnowledgeObjectFunctions::IsRecon
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
template< typename T > 
void DEC_KnowledgeObjectFunctions::IsRecon( DIA_Call_ABC& call, const T& caller )
{
    DEC_Knowledge_Object* pKnowledge = DEC_FunctionsTools::GetKnowledgeObjectFromDia( call.GetParameter( 0 ), caller.GetArmy() );
    if( !pKnowledge )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        call.GetResult().SetValue( false );
        return;
    }
    call.GetParameter( 1 ).SetValue( eQueryValid );
    call.GetResult().SetValue( pKnowledge->IsRecon() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::GetSiteFranchissementWidth
// Created: NLD 2004-05-13
// Modified: JVT 2004-12-17
// -----------------------------------------------------------------------------
template< typename T >
void DEC_KnowledgeObjectFunctions::GetSiteFranchissementWidth( DIA_Call_ABC& call, const T& caller )
{
    DEC_Knowledge_Object* pKnowledge = DEC_FunctionsTools::GetKnowledgeObjectFromDia( call.GetParameter( 0 ), caller.GetArmy() );
    const DEC_Knowledge_ObjectAttributeCrossingSite* attribute = pKnowledge->Retrieve< DEC_Knowledge_ObjectAttributeCrossingSite >();
    if( !(pKnowledge && attribute != 0 ) )
        call.GetResult().SetValue( (float)0. );
    else
        call.GetResult().SetValue( attribute->GetWidth() );
}
// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_KnowledgeObjectFunctions::GetLocalisation
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
template< typename T > 
void DEC_KnowledgeObjectFunctions::GetLocalisation( DIA_Call_ABC& call, const T& caller )
{
    DEC_Knowledge_Object* pKnowledge = DEC_FunctionsTools::GetKnowledgeObjectFromDia( call.GetParameter( 0 ), caller.GetArmy() );
    if( !pKnowledge )
        call.GetResult().SetValue( (void*)0, &DEC_Tools::GetTypeLocalisation() );
    else
        call.GetResult().SetValue( (void*)&pKnowledge->GetLocalisation(), &DEC_Tools::GetTypeLocalisation(), 1 );
}

// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_KnowledgeObjectFunctions::IsKnowledgeValid
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
template< typename T > 
static void DEC_KnowledgeObjectFunctions::IsKnowledgeValid( DIA_Call_ABC& call, const T& caller )
{
    DEC_Knowledge_Object* pKnowledge = DEC_FunctionsTools::GetKnowledgeObjectFromDia( call.GetParameter( 0 ), caller.GetArmy() );
    call.GetResult().SetValue( pKnowledge != 0 );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::IsBypassed
// Created: NLD 2004-04-09
// Modified: JVT 2004-12-17
// -----------------------------------------------------------------------------
template< typename T >
void DEC_KnowledgeObjectFunctions::IsBypassed( DIA_Call_ABC& call, const T& caller )
{
    DEC_Knowledge_Object* pKnowledge = DEC_FunctionsTools::GetKnowledgeObjectFromDia( call.GetParameter( 0 ), caller.GetArmy() );
    if( !pKnowledge )
        call.GetResult().SetValue( eTristate_DontKnow );
    else
        call.GetResult().SetValue( pKnowledge->IsBypassed() ? eTristate_True : eTristate_False );
}

// -----------------------------------------------------------------------------
// Name: template< typename T >  void DEC_KnowledgeObjectFunctions::IsConstructed                    
// Created: NLD 2006-10-27
// -----------------------------------------------------------------------------
template< typename T > 
void DEC_KnowledgeObjectFunctions::IsConstructed( DIA_Call_ABC& call, const T& caller )
{
    DEC_Knowledge_Object* pKnowledge = DEC_FunctionsTools::GetKnowledgeObjectFromDia( call.GetParameter( 0 ), caller.GetArmy() );
    if( !pKnowledge )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        call.GetResult().SetValue( false );
        return;
    }
    call.GetParameter( 1 ).SetValue( eQueryValid );
    call.GetResult().SetValue( pKnowledge->IsConstructed() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::IsReservedObstacle
// Created: NLD 2004-04-09
// Modified: JVT 2004-12-17
// -----------------------------------------------------------------------------
template< typename T >
void DEC_KnowledgeObjectFunctions::IsReservedObstacle( DIA_Call_ABC& call, const T& caller )
{
    DEC_Knowledge_Object* pKnowledge = DEC_FunctionsTools::GetKnowledgeObjectFromDia( call.GetParameter( 0 ), caller.GetArmy() );
    if( !pKnowledge )
        call.GetResult().SetValue( false );
    else
        call.GetResult().SetValue( pKnowledge->IsReservedObstacle() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::IsReservedObstacleActivated
// Created: NLD 2004-04-09
// Modified: JVT 2004-12-17
// -----------------------------------------------------------------------------
template< typename T >
void DEC_KnowledgeObjectFunctions::IsReservedObstacleActivated( DIA_Call_ABC& call, const T& caller )
{
    DEC_Knowledge_Object* pKnowledge = DEC_FunctionsTools::GetKnowledgeObjectFromDia( call.GetParameter( 0 ), caller.GetArmy() );
    if( !pKnowledge )
        call.GetResult().SetValue( true );
    else
        call.GetResult().SetValue( pKnowledge->IsReservedObstacleActivated() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::GetType
// Created: NLD 2004-04-09
// Modified: JVT 2004-12-17
// -----------------------------------------------------------------------------
template< typename T >
void DEC_KnowledgeObjectFunctions::GetType( DIA_Call_ABC& call, const T& caller )
{
    DEC_Knowledge_Object* pKnowledge = DEC_FunctionsTools::GetKnowledgeObjectFromDia( call.GetParameter( 0 ), caller.GetArmy() );
    if( !pKnowledge )
        call.GetResult().SetValue( (int)0 );
    else
        call.GetResult().SetValue( pKnowledge->GetType().GetName() );
}