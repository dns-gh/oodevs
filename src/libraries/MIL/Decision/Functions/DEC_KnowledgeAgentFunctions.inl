// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_KnowledgeAgentFunctions.inl $
// $Author: Jvt $
// $Modtime: 23/05/05 16:33 $
// $Revision: 7 $
// $Workfile: DEC_KnowledgeAgentFunctions.inl $
//
// *****************************************************************************

#include "DEC_FunctionsTools.h"
#include "Knowledge/DEC_Knowledge_Agent.h"

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::GetOperationalState
// Created: NLD 2004-04-14
// -----------------------------------------------------------------------------
template< typename T >
void DEC_KnowledgeAgentFunctions::GetOperationalState( DIA_Call_ABC& call, const T& caller )
{
    DEC_Knowledge_Agent* pKnowledge = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), caller.GetKnowledgeGroup() );
    if( !pKnowledge )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        call.GetResult().SetValue( (float)0. );
        return;
    }

    call.GetParameter( 1 ).SetValue( eQueryValid );
    call.GetResult().SetValue( (float)pKnowledge->GetOperationalState() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::GetMajorOperationalState
// Created: NLD 2004-04-14
// -----------------------------------------------------------------------------
template< typename T >
void DEC_KnowledgeAgentFunctions::GetMajorOperationalState( DIA_Call_ABC& call, const T& caller )
{
    DEC_Knowledge_Agent* pKnowledge = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), caller.GetKnowledgeGroup() );
    if( !pKnowledge )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        call.GetResult().SetValue( (float)0. );
        return;
    }

    call.GetParameter( 1 ).SetValue( eQueryValid );
    call.GetResult().SetValue( (float)pKnowledge->GetMajorOperationalState() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::GetSpeed
// Created: NLD 2004-03-31
// -----------------------------------------------------------------------------
template< typename T >
void DEC_KnowledgeAgentFunctions::GetSpeed( DIA_Call_ABC& call, const T& caller )
{
    DEC_Knowledge_Agent* pKnowledge = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), caller.GetKnowledgeGroup() );
    if( !pKnowledge )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        call.GetResult().SetValue( (float)0. );
        return;
    }
    call.GetParameter( 1 ).SetValue( eQueryValid );
    call.GetResult().SetValue( (float)MIL_Tools::ConvertSpeedMosToSim( pKnowledge->GetSpeed() ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::GetPosition
// Created: NLD 2004-03-31
// -----------------------------------------------------------------------------
template< typename T >
void DEC_KnowledgeAgentFunctions::GetPosition( DIA_Call_ABC& call, const T& caller )
{
    DEC_Knowledge_Agent* pKnowledge = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), caller.GetKnowledgeGroup() );
    if( !pKnowledge )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        call.GetResult().SetValue( (void*)0, &DEC_Tools::GetTypePoint() );
        return;
    }
    call.GetParameter( 1 ).SetValue( eQueryValid );
    call.GetResult().SetValue( (void*)&pKnowledge->GetPosition(), &DEC_Tools::GetTypePoint(), 1 );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::IsFlying
// Created: JVT 2005-08-23
// -----------------------------------------------------------------------------
template< typename T >
void DEC_KnowledgeAgentFunctions::IsFlying( DIA_Call_ABC& call, const T& caller )
{
    DEC_Knowledge_Agent* pKnowledge = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), caller.GetKnowledgeGroup() );
    
    if ( !pKnowledge )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        call.GetResult().SetValue( false );
        return;
    }
    call.GetParameter( 1 ).SetValue( eQueryValid );
    call.GetResult().SetValue( pKnowledge->GetAltitude() > MIL_Tools::GetAltitude( pKnowledge->GetPosition() ) );
}


// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::IsKnowledgeValid
// Created: NLD 2004-04-02
// -----------------------------------------------------------------------------
template< typename T >
void DEC_KnowledgeAgentFunctions::IsKnowledgeValid( DIA_Call_ABC& call, const T& caller )
{
    DEC_Knowledge_Agent* pKnowledge = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), caller.GetKnowledgeGroup() );
    call.GetResult().SetValue( pKnowledge != 0 );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::IsSurrendered
// Created: NLD 2005-03-04
// -----------------------------------------------------------------------------
template< typename T >
void DEC_KnowledgeAgentFunctions::IsSurrendered( DIA_Call_ABC& call, const T& caller )
{
    DEC_Knowledge_Agent* pKnowledge = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), caller.GetKnowledgeGroup() );
    if( !pKnowledge )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        call.GetResult().SetValue( false );
        return;
    }
    
    call.GetParameter( 1 ).SetValue( eQueryValid );
    call.GetResult().SetValue( pKnowledge->IsSurrendered() );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::IsPrisoner
// Created: NLD 2005-03-04
// -----------------------------------------------------------------------------
template< typename T >
void DEC_KnowledgeAgentFunctions::IsPrisoner( DIA_Call_ABC& call, const T& caller )
{
    DEC_Knowledge_Agent* pKnowledge = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), caller.GetKnowledgeGroup() );
    if( !pKnowledge )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        call.GetResult().SetValue( false );
        return;
    }
    
    call.GetParameter( 1 ).SetValue( eQueryValid );
    call.GetResult().SetValue( pKnowledge->IsPrisoner() );
}

// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_KnowledgeAgentFunctions::IsRefugee
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
template< typename T > 
void DEC_KnowledgeAgentFunctions::IsRefugee( DIA_Call_ABC& call, const T& caller )
{
    DEC_Knowledge_Agent* pKnowledge = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), caller.GetKnowledgeGroup() );
    if( !pKnowledge )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        call.GetResult().SetValue( false );
        return;
    }
    
    call.GetParameter( 1 ).SetValue( eQueryValid );
    call.GetResult().SetValue( pKnowledge->IsRefugee() );
}

// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_KnowledgeAgentFunctions::IsRefugeeManaged
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
template< typename T > 
void DEC_KnowledgeAgentFunctions::IsRefugeeManaged( DIA_Call_ABC& call, const T& caller )
{
    DEC_Knowledge_Agent* pKnowledge = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), caller.GetKnowledgeGroup() );
    if( !pKnowledge )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        call.GetResult().SetValue( false );
        return;
    }
    
    call.GetParameter( 1 ).SetValue( eQueryValid );
    call.GetResult().SetValue( pKnowledge->IsRefugeeManaged() );
}

// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_KnowledgeAgentFunctions::IsMilita
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
template< typename T > 
void DEC_KnowledgeAgentFunctions::IsMilita( DIA_Call_ABC& call, const T& caller )
{
    DEC_Knowledge_Agent* pKnowledge = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), caller.GetKnowledgeGroup() );
    if( !pKnowledge )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        call.GetResult().SetValue( false );
        return;
    }
    
    call.GetParameter( 1 ).SetValue( eQueryValid );
    call.GetResult().SetValue( pKnowledge->IsMilitia() );
}

// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_KnowledgeAgentFunctions::IsDead
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
template< typename T > 
void DEC_KnowledgeAgentFunctions::IsDead( DIA_Call_ABC& call, const T& caller )
{
    DEC_Knowledge_Agent* pKnowledge = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), caller.GetKnowledgeGroup() );
    if( !pKnowledge )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        call.GetResult().SetValue( false );
        return;
    }
    
    call.GetParameter( 1 ).SetValue( eQueryValid );
    call.GetResult().SetValue( pKnowledge->IsDead() );
}

// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_KnowledgeAgentFunctions::Lock
// Created: NLD 2005-04-21
// -----------------------------------------------------------------------------
template< typename T >
void DEC_KnowledgeAgentFunctions::Lock( DIA_Call_ABC& call, const T& caller )
{
    DEC_Knowledge_Agent* pKnowledge = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), caller.GetKnowledgeGroup() );
    if( !pKnowledge )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        return;
    }
    
    call.GetParameter( 1 ).SetValue( eQueryValid );
    pKnowledge->Lock();
}
    
// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_KnowledgeAgentFunctions::Unlock
// Created: NLD 2005-04-21
// -----------------------------------------------------------------------------
template< typename T >
void DEC_KnowledgeAgentFunctions::Unlock( DIA_Call_ABC& call, const T& caller )
{
    DEC_Knowledge_Agent* pKnowledge = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), caller.GetKnowledgeGroup() );
    if( !pKnowledge )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        return;
    }
    
    call.GetParameter( 1 ).SetValue( eQueryValid );
    pKnowledge->Unlock();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::GetDangerosityOnKnowledge
// Created: NLD 2005-05-17
// -----------------------------------------------------------------------------
template< typename T >
void DEC_KnowledgeAgentFunctions::GetDangerosityOnKnowledge( DIA_Call_ABC& call, const T& caller )
{
    DEC_Knowledge_Agent* pSource = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), caller.GetKnowledgeGroup() );
    DEC_Knowledge_Agent* pTarget = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 1 ), caller.GetKnowledgeGroup() );

    if( !pSource || !pTarget )
    {
        call.GetParameter( 2 ).SetValue( eQueryInvalid );
        call.GetResult().SetValue( (float)0. );
        return;
    }
    call.GetParameter( 2 ).SetValue( eQueryValid );

    // For DIA, the dangerosity value is 1 <= dangerosity <= 2
    const MT_Float rDangerosity = pSource->GetDangerosity( *pTarget );
    call.GetResult().SetValue( (float)( rDangerosity + 1. ) );
}


// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::GetDangerosityOnPion
// Created: NLD 2004-04-02
// -----------------------------------------------------------------------------
template< typename T >
void DEC_KnowledgeAgentFunctions::GetDangerosityOnPion( DIA_Call_ABC& call, const T& caller )
{
    DEC_Knowledge_Agent* pKnowledge = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), caller.GetKnowledgeGroup() );
    if( !pKnowledge )
    {
        call.GetParameter( 2 ).SetValue( eQueryInvalid );
        call.GetResult().SetValue( (float)0. );
        return;
    }
    call.GetParameter( 2 ).SetValue( eQueryValid );

    DEC_RolePion_Decision* pTarget = call.GetParameter( 1 ).ToUserObject( pTarget );
    assert( pTarget );

    // For DIA, the dangerosity value is 1 <= dangerosity <= 2
    const MT_Float rDangerosity = pKnowledge->GetDangerosity( pTarget->GetPion() );
    call.GetResult().SetValue( (float)( rDangerosity + 1. ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::GetMaxPerceptionLevelForKnowledgeGroup
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
template< typename T >
void DEC_KnowledgeAgentFunctions::GetMaxPerceptionLevelForKnowledgeGroup( DIA_Call_ABC& call, const T& caller )
{
    DEC_Knowledge_Agent* pKnowledge = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), caller.GetKnowledgeGroup() );
     if( !pKnowledge )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        call.GetResult().SetValue( (int)0 );
        return;
    }

    call.GetParameter( 1 ).SetValue( eQueryValid );
    call.GetResult().SetValue( (int)pKnowledge->GetMaxPerceptionLevel().GetID() );
}

