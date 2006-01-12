// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_FireFunctions.cpp $
// $Author: Jvt $
// $Modtime: 2/05/05 16:22 $
// $Revision: 9 $
// $Workfile: DEC_FireFunctions.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "DEC_FireFunctions.h"

#include "DEC_FunctionsTools.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Actions/Firing/IndirectFiring/PHY_RoleAction_IndirectFiring.h"
#include "Entities/Agents/Units/Dotations/PHY_IndirectFireDotationClass.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Tools/MIL_Tools.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::GetMaxRangeToFireOnEnemy
// Created: NLD 2004-04-02
// -----------------------------------------------------------------------------
void DEC_FireFunctions::GetMaxRangeToFireOnEnemy( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypeConnaissanceAgent( call.GetParameter( 0 ) ) );
    DEC_Knowledge_Agent* pKnowledge = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), callerAgent.GetKnowledgeGroup() );
    if( !pKnowledge )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        call.GetResult().SetValue( (float)-1. );
        return;
    }

    call.GetParameter( 1 ).SetValue( eQueryValid );
    float rWantedPH = 0.8f;
    if( call.GetParameters().GetParameters().size() >= 3 )
        rWantedPH = call.GetParameter( 2 ).ToFloat();
    
    const float rRange = MIL_Tools::ConvertSimToMeter( callerAgent.GetRole< PHY_RolePion_Composantes >().GetMaxRangeToFireOn( *pKnowledge, rWantedPH ) );
    call.GetResult().SetValue( rRange );
}

// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::GetMinRangeToFireOnEnemy
// Created: JVT 2004-12-17
// -----------------------------------------------------------------------------
void DEC_FireFunctions::GetMinRangeToFireOnEnemy( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypeConnaissanceAgent( call.GetParameter( 0 ) ) );
    DEC_Knowledge_Agent* pKnowledge = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), callerAgent.GetKnowledgeGroup() );
    if ( !pKnowledge )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        call.GetResult().SetValue( (float)-1. );
        return;
    }
    
    call.GetParameter( 1 ).SetValue( eQueryValid );
    float rWantedPH = 0.8f;
    if ( call.GetParameters().GetParameters().size() >= 3 )
        rWantedPH = call.GetParameter( 2 ).ToFloat();

    const MT_Float rRange = callerAgent.GetRole< PHY_RolePion_Composantes >().GetMinRangeToFireOn( *pKnowledge, rWantedPH );
    if( rRange == std::numeric_limits< MT_Float >::max() )
        call.GetResult().SetValue( (float)-1. );
    else
        call.GetResult().SetValue( MIL_Tools::ConvertSimToMeter( (float)rRange ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::GetMaxRangeToFireOnEnemyActualPosture
// Created: SBO 2006-01-10
// -----------------------------------------------------------------------------
void DEC_FireFunctions::GetMaxRangeToFireOnEnemyActualPosture( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypeConnaissanceAgent( call.GetParameter( 0 ) ) );
    DEC_Knowledge_Agent* pKnowledge = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), callerAgent.GetKnowledgeGroup() );
    if( !pKnowledge )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        call.GetResult().SetValue( (float)-1. );
        return;
    }

    call.GetParameter( 1 ).SetValue( eQueryValid );
    float rWantedPH = 0.8f;
    if( call.GetParameters().GetParameters().size() >= 3 )
        rWantedPH = call.GetParameter( 2 ).ToFloat();
    
    const float rRange = MIL_Tools::ConvertSimToMeter( callerAgent.GetRole< PHY_RolePion_Composantes >().GetMaxRangeToFireOnActualPosture( *pKnowledge, rWantedPH ) );
    call.GetResult().SetValue( rRange );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::GetMinRangeToFireOnEnemyActualPosture
// Created: SBO 2006-01-10
// -----------------------------------------------------------------------------
void DEC_FireFunctions::GetMinRangeToFireOnEnemyActualPosture( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypeConnaissanceAgent( call.GetParameter( 0 ) ) );
    DEC_Knowledge_Agent* pKnowledge = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), callerAgent.GetKnowledgeGroup() );
    if ( !pKnowledge )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        call.GetResult().SetValue( (float)-1. );
        return;
    }
    
    call.GetParameter( 1 ).SetValue( eQueryValid );
    float rWantedPH = 0.8f;
    if ( call.GetParameters().GetParameters().size() >= 3 )
        rWantedPH = call.GetParameter( 2 ).ToFloat();

    const MT_Float rRange = callerAgent.GetRole< PHY_RolePion_Composantes >().GetMinRangeToFireOnActualPosture( *pKnowledge, rWantedPH );
    if( rRange == std::numeric_limits< MT_Float >::max() )
        call.GetResult().SetValue( (float)-1. );
    else
        call.GetResult().SetValue( MIL_Tools::ConvertSimToMeter( (float)rRange ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::GetMaxRangeToFireOnEnemyWhenUnloaded
// Created: NLD 2004-04-02
// -----------------------------------------------------------------------------
void DEC_FireFunctions::GetMaxRangeToFireOnEnemyWhenUnloaded( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypeConnaissanceAgent( call.GetParameter( 0 ) ) );
    DEC_Knowledge_Agent* pKnowledge = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), callerAgent.GetKnowledgeGroup() );
    if( !pKnowledge )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        call.GetResult().SetValue( (float)-1. );
        return;
    }

    call.GetParameter( 1 ).SetValue( eQueryValid );
    float rWantedPH = 0.8f;
    if( call.GetParameters().GetParameters().size() >= 3 )
        rWantedPH = call.GetParameter( 2 ).ToFloat();

    float rRange = MIL_Tools::ConvertSimToMeter( callerAgent.GetRole< PHY_RolePion_Composantes >().GetOnlyLoadableMaxRangeToFireOn( *pKnowledge, rWantedPH ) );
    call.GetResult().SetValue( rRange );
}

// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::GetMaxRangeToBeFiredByEnemy
// Created: NLD 2004-04-14
// -----------------------------------------------------------------------------
void DEC_FireFunctions::GetMaxRangeToBeFiredByEnemy( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypeConnaissanceAgent( call.GetParameter( 0 ) ) );
    DEC_Knowledge_Agent* pKnowledge = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), callerAgent.GetKnowledgeGroup() );
    if( !pKnowledge )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        call.GetResult().SetValue( (float)-1. );
        return;
    }

    call.GetParameter( 1 ).SetValue( eQueryValid );
    float rWantedPH = 0.8f;
    if( call.GetParameters().GetParameters().size() >= 3 )
        rWantedPH = call.GetParameter( 2 ).ToFloat();
    
    float rRange = MIL_Tools::ConvertSimToMeter( pKnowledge->GetMaxRangeToFireOn( callerAgent, rWantedPH ) );
    call.GetResult().SetValue( rRange );
}

// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::GetMaxRangeToIndirectFire
// Created: JVT 2005-05-02
// -----------------------------------------------------------------------------
void DEC_FireFunctions::GetMaxRangeToIndirectFire( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    const PHY_IndirectFireDotationClass* pClass = PHY_IndirectFireDotationClass::Find( call.GetParameter( 0 ).ToId() );
    
    if ( pClass )
    {
        const MT_Float rRange = callerAgent.GetRole< PHY_RolePion_Composantes >().GetMaxRangeToIndirectFire( *pClass, true );
        
        if ( rRange < 0. ) // Pas de possibilité de tir
            call.GetResult().SetValue( -1.f );
        else
            call.GetResult().SetValue( (float)MIL_Tools::ConvertSimToMeter( rRange ) );
    }
    else
        call.GetResult().SetValue( -1.f );
}

// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::GetMinRangeToIndirectFire
// Created: JVT 2005-05-02
// -----------------------------------------------------------------------------
void DEC_FireFunctions::GetMinRangeToIndirectFire( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    const PHY_IndirectFireDotationClass* pClass = PHY_IndirectFireDotationClass::Find( call.GetParameter( 0 ).ToId() );
    
    if ( pClass )
    {
        const MT_Float rRange = callerAgent.GetRole< PHY_RolePion_Composantes >().GetMinRangeToIndirectFire( *pClass, true );

        if ( rRange == std::numeric_limits< MT_Float >::max() ) // Pas de possibilité de tir
            call.GetResult().SetValue( -1.f );
        else
            call.GetResult().SetValue( (float)MIL_Tools::ConvertSimToMeter( rRange ) );
    }
    else
        call.GetResult().SetValue( -1.f );
}

// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::GetMaxRangeToIndirectFireWithoutAmmoCheck
// Created: JVT 2005-05-02
// -----------------------------------------------------------------------------
void DEC_FireFunctions::GetMaxRangeToIndirectFireWithoutAmmoCheck( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    const PHY_IndirectFireDotationClass* pClass = PHY_IndirectFireDotationClass::Find( call.GetParameter( 0 ).ToId() );
    
    if ( pClass )
    {
        const MT_Float rRange = callerAgent.GetRole< PHY_RolePion_Composantes >().GetMaxRangeToIndirectFire( *pClass, false );
        
        if ( rRange < 0. ) // Pas de possibilité de tir
            call.GetResult().SetValue( -1.f );
        else
            call.GetResult().SetValue( (float)MIL_Tools::ConvertSimToMeter( rRange ) );
    }
    else
        call.GetResult().SetValue( -1.f );
}

// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::GetMinRangeToIndirectFireWithoutAmmoCheck
// Created: JVT 2005-05-02
// -----------------------------------------------------------------------------
void DEC_FireFunctions::GetMinRangeToIndirectFireWithoutAmmoCheck( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    const PHY_IndirectFireDotationClass* pClass = PHY_IndirectFireDotationClass::Find( call.GetParameter( 0 ).ToId() );
    
    if ( pClass )
    {
        const MT_Float rRange = callerAgent.GetRole< PHY_RolePion_Composantes >().GetMinRangeToIndirectFire( *pClass, false );

        if ( rRange == std::numeric_limits< MT_Float >::max() ) // Pas de possibilité de tir
            call.GetResult().SetValue( -1.f );
        else
            call.GetResult().SetValue( (float)MIL_Tools::ConvertSimToMeter( rRange ) );
    }
    else
        call.GetResult().SetValue( -1.f );
}

// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::ThrowSmokeOnKnowledgeAgent
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
void DEC_FireFunctions::ThrowSmokeOnKnowledgeAgent( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypeConnaissanceAgent( call.GetParameter( 0 ) ) );
    DEC_Knowledge_Agent* pTarget = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), callerAgent.GetKnowledgeGroup() );
    if( pTarget )
        callerAgent.GetRole< PHY_RoleAction_IndirectFiring >().ThrowSmoke( pTarget->GetPosition(), 2 ); //$$$ 
}
