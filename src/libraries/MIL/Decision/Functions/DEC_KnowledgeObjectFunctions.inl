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
#include "Knowledge/DEC_Knowledge_ObjectSiteFranchissement.h"
#include "Knowledge/DEC_Knowledge_ObjectItineraireLogistique.h"

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
    if( !pKnowledge || pKnowledge->GetType() != MIL_RealObjectType::siteFranchissement_ )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        call.GetResult().SetValue( (float)0. );
        return;
    }
    DEC_Knowledge_ObjectSiteFranchissement* pKnowledgeSiteFranchissement = static_cast< DEC_Knowledge_ObjectSiteFranchissement* >( pKnowledge );
    call.GetParameter( 1 ).SetValue( eQueryValid );
    call.GetResult().SetValue( (float)pKnowledgeSiteFranchissement->GetWidth() );
}
// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::IsSiteFranchissementBanksToFitOut
// Created: PBT 2005-03-10
// -----------------------------------------------------------------------------
template< typename T >
void DEC_KnowledgeObjectFunctions::IsSiteFranchissementBanksToFitOut( DIA_Call_ABC& call, const T& caller )
{
    DEC_Knowledge_Object* pKnowledge = DEC_FunctionsTools::GetKnowledgeObjectFromDia( call.GetParameter( 0 ), caller.GetArmy() );
    if( !pKnowledge || pKnowledge->GetType() != MIL_RealObjectType::siteFranchissement_ )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        call.GetResult().SetValue( false );
        return;
    }
    DEC_Knowledge_ObjectSiteFranchissement* pKnowledgeSiteFranchissement = static_cast< DEC_Knowledge_ObjectSiteFranchissement* >( pKnowledge );
    call.GetParameter( 1 ).SetValue( eQueryValid );
    call.GetResult().SetValue( pKnowledgeSiteFranchissement->IsBanksToFitOut() );
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
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        call.GetResult().SetValue( (void*)0, &DEC_Tools::GetTypeLocalisation() );
        return;
    }

    call.GetParameter( 1 ).SetValue( eQueryValid );
    call.GetResult().SetValue( (void*)&pKnowledge->GetLocalisation(), &DEC_Tools::GetTypeLocalisation() );
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
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        call.GetResult().SetValue( false );
        return;
    }
    call.GetParameter( 1 ).SetValue( eQueryValid );
    call.GetResult().SetValue( pKnowledge->IsBypassed() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::IsPrepared
// Created: NLD 2004-04-09
// Modified: JVT 2004-12-17
// -----------------------------------------------------------------------------
template< typename T >
void DEC_KnowledgeObjectFunctions::IsPrepared( DIA_Call_ABC& call, const T& caller )
{
    DEC_Knowledge_Object* pKnowledge = DEC_FunctionsTools::GetKnowledgeObjectFromDia( call.GetParameter( 0 ), caller.GetArmy() );
    if( !pKnowledge )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        call.GetResult().SetValue( false );
        return;
    }
    call.GetParameter( 1 ).SetValue( eQueryValid );
    call.GetResult().SetValue( pKnowledge->IsPrepared() );
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
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        call.GetResult().SetValue( (int)0 );
        return;
    }
    call.GetParameter( 1 ).SetValue( eQueryValid );
    call.GetResult().SetValue( (int)pKnowledge->GetType().GetID() );
}

// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_KnowledgeObjectFunctions::IsLogisticRouteEquipped
// Created: NLD 2005-02-18
// -----------------------------------------------------------------------------
template< typename T > 
void DEC_KnowledgeObjectFunctions::IsLogisticRouteEquipped( DIA_Call_ABC& call, const T& caller )
{
    DEC_Knowledge_Object* pKnowledge = DEC_FunctionsTools::GetKnowledgeObjectFromDia( call.GetParameter( 0 ), caller.GetArmy() );
    if( !pKnowledge || pKnowledge->GetType() != MIL_RealObjectType::itineraireLogistique_ )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        call.GetResult().SetValue( false );
        return;
    }
    DEC_Knowledge_ObjectItineraireLogistique* pKnowledgeItiLog = static_cast< DEC_Knowledge_ObjectItineraireLogistique* >( pKnowledge );
    call.GetParameter( 1 ).SetValue( eQueryValid );
    call.GetResult().SetValue( pKnowledgeItiLog->IsEquipped() );
}
