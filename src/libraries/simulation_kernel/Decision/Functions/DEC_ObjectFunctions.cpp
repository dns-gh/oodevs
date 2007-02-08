// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Decision/Functions/DEC_ObjectFunctions.cpp $
// $Author: Nld $
// $Modtime: 10/03/05 18:17 $
// $Revision: 3 $
// $Workfile: DEC_ObjectFunctions.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_ObjectFunctions.h"

#include "Entities/Agents/Actions/Objects/PHY_RoleAction_Objects.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/Objects/MIL_CampPrisonniers.h"
#include "Entities/Objects/MIL_CampRefugies.h"
#include "Entities/Objects/MIL_RealObjectType.h"
#include "Decision/DEC_Tools.h"
#include "Decision/Genie/DEC_Gen_Object.h"

// -----------------------------------------------------------------------------
// Name: DEC_ObjectFunctions::GetPrisonerCampPosition
// Created: NLD 2005-03-04
// -----------------------------------------------------------------------------
void DEC_ObjectFunctions::GetPrisonerCampPosition( DIA_Call_ABC& call, MIL_Automate& callerAutomate )
{
    const MIL_CampPrisonniers* pCamp = callerAutomate.GetPrisonerCamp();
    if( !pCamp )
    {
        call.GetResult().SetValue( (void*)0, &DEC_Tools::GetTypePoint() );
        return;
    }

    MT_Vector2D* pPos = new MT_Vector2D( pCamp->GetLocalisation().ComputeBarycenter() );
    call.GetResult().SetValue( pPos, &DEC_Tools::GetTypePoint() );
}

// -----------------------------------------------------------------------------
// Name: DEC_ObjectFunctions::GetRefugeeCampPosition
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
void DEC_ObjectFunctions::GetRefugeeCampPosition( DIA_Call_ABC& call, MIL_Automate& callerAutomate )
{
    const MIL_CampRefugies* pCamp = callerAutomate.GetRefugeeCamp();
    if( !pCamp )
    {
        call.GetResult().SetValue( (void*)0, &DEC_Tools::GetTypePoint() );
        return;
    }

    MT_Vector2D* pPos = new MT_Vector2D( pCamp->GetLocalisation().ComputeBarycenter() );
    call.GetResult().SetValue( pPos, &DEC_Tools::GetTypePoint() );
}

// -----------------------------------------------------------------------------
// Name: DEC_ObjectFunctions::CanObjectTypeBeBypassed
// Created: NLD 2005-09-21
// -----------------------------------------------------------------------------
void DEC_ObjectFunctions::CanObjectTypeBeBypassed( DIA_Call_ABC& call )
{
    const MIL_RealObjectType* pObjectType = MIL_RealObjectType::Find( call.GetParameter( 0 ).ToId() );
    assert( pObjectType );
    if( pObjectType )
        call.GetResult().SetValue( pObjectType->CanBeBypassed() );
    else
        call.GetResult().SetValue( false );
}

// -----------------------------------------------------------------------------
// Name: DEC_ObjectFunctions::CanObjectTypeBeMined
// Created: NLD 2005-09-21
// -----------------------------------------------------------------------------
void DEC_ObjectFunctions::CanObjectTypeBeMined( DIA_Call_ABC& call )
{
    const MIL_RealObjectType* pObjectType = MIL_RealObjectType::Find( call.GetParameter( 0 ).ToId() );
    assert( pObjectType );
    if( pObjectType )
        call.GetResult().SetValue( pObjectType->CanBeMined() );
    else
        call.GetResult().SetValue( false );
}

// -----------------------------------------------------------------------------
// Name: DEC_ObjectFunctions::CanObjectTypeBePrepared
// Created: NLD 2005-09-21
// -----------------------------------------------------------------------------
void DEC_ObjectFunctions::CanObjectTypeBePrepared( DIA_Call_ABC& call )
{
    const MIL_RealObjectType* pObjectType = MIL_RealObjectType::Find( call.GetParameter( 0 ).ToId() );
    assert( pObjectType );
    if( pObjectType )
        call.GetResult().SetValue( pObjectType->CanBePrepared() );
    else
        call.GetResult().SetValue( false );
}

// =============================================================================
// GEN OBJECT
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_ObjectFunctions::GetGenObjectType
// Created: NLD 2006-10-26
// -----------------------------------------------------------------------------
void DEC_ObjectFunctions::GetGenObjectType( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypeGenObjet( call.GetParameter( 0 ) ) );    
    DEC_Gen_Object* pTmp = call.GetParameter( 0 ).ToUserPtr( pTmp );    
    call.GetResult().SetValue( (int)pTmp->GetType().GetID() );
}

// -----------------------------------------------------------------------------
// Name: DEC_ObjectFunctions::GetGenObjectLocalisation
// Created: NLD 2006-10-26
// -----------------------------------------------------------------------------
void DEC_ObjectFunctions::GetGenObjectLocalisation( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypeGenObjet( call.GetParameter( 0 ) ) );    
    DEC_Gen_Object* pTmp = call.GetParameter( 0 ).ToUserPtr( pTmp );    
    call.GetResult().SetValue( (void*)&pTmp->GetLocalisation(), &DEC_Tools::GetTypeLocalisation(), 1 );
}

// -----------------------------------------------------------------------------
// Name: DEC_ObjectFunctions::GetGenObjectDensity
// Created: NLD 2006-10-26
// -----------------------------------------------------------------------------
void DEC_ObjectFunctions::GetGenObjectDensity( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypeGenObjet( call.GetParameter( 0 ) ) );    
    DEC_Gen_Object* pTmp = call.GetParameter( 0 ).ToUserPtr( pTmp );    
    call.GetResult().SetValue( (float)pTmp->GetDensity() );
}

// -----------------------------------------------------------------------------
// Name: DEC_ObjectFunctions::GetGenObjectPreliminaire
// Created: NLD 2006-10-26
// -----------------------------------------------------------------------------
void DEC_ObjectFunctions::GetGenObjectPreliminaire( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypeGenObjet( call.GetParameter( 0 ) ) );    
    DEC_Gen_Object* pTmp = call.GetParameter( 0 ).ToUserPtr( pTmp );    
    call.GetResult().SetValue( (int)pTmp->GetPreliminaire() );
}

// -----------------------------------------------------------------------------
// Name: DEC_ObjectFunctions::GetGenObjectTC2
// Created: NLD 2006-10-26
// -----------------------------------------------------------------------------
void DEC_ObjectFunctions::GetGenObjectTC2( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypeGenObjet( call.GetParameter( 0 ) ) );    
    DEC_Gen_Object* pTmp = call.GetParameter( 0 ).ToUserPtr( pTmp );
    call.GetResult().SetValue( pTmp->GetTC2() ? pTmp->GetTC2()->GetDecision() : *(DEC_AutomateDecision*)( 0 ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_ObjectFunctions::GetGenObjectMinesActivityTime
// Created: NLD 2006-10-26
// -----------------------------------------------------------------------------
void DEC_ObjectFunctions::GetGenObjectMinesActivityTime( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypeGenObjet( call.GetParameter( 0 ) ) );    
    DEC_Gen_Object* pTmp = call.GetParameter( 0 ).ToUserPtr( pTmp );    
    call.GetResult().SetValue( (float)pTmp->GetMinesActivityTime() );
}