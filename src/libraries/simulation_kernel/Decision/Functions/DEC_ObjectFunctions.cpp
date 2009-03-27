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

#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Objects/MIL_ObjectManipulator_ABC.h"

#include "Decision/DEC_Tools.h"
#include "Decision/DEC_Gen_Object.h"
#include "Decision/DEC_Objective.h"

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
    call.GetResult().SetValue( pTmp->GetType().GetName() );
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
// Name: DEC_ObjectFunctions::GetGenObjectReservedObstacle
// Created: NLD 2006-10-26
// -----------------------------------------------------------------------------
void DEC_ObjectFunctions::GetGenObjectReservedObstacle( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypeGenObjet( call.GetParameter( 0 ) ) );    
    DEC_Gen_Object* pTmp = call.GetParameter( 0 ).ToUserPtr( pTmp );    
    call.GetResult().SetValue( pTmp->GetObstacleType() == EnumDemolitionTargetType::reserved );
}

// -----------------------------------------------------------------------------
// Name: DEC_ObjectFunctions::GetGenObjectTC2
// Created: NLD 2006-10-26
// -----------------------------------------------------------------------------
void DEC_ObjectFunctions::GetGenObjectTC2( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypeGenObjet( call.GetParameter( 0 ) ) );    
    DEC_Gen_Object* pTmp = call.GetParameter( 0 ).ToUserPtr( pTmp );
    DEC_AutomateDecision* dec = pTmp->GetTC2() ? const_cast< DEC_AutomateDecision* >( &pTmp->GetTC2()->GetDecision() ) : (DEC_AutomateDecision*)0;
    call.GetResult().SetValue( *dec );
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

// -----------------------------------------------------------------------------
// Name: DEC_ObjectFunctions::GetObjectiveLocalisation
// Created: NLD 2007-05-14
// -----------------------------------------------------------------------------
void DEC_ObjectFunctions::GetObjectiveLocalisation( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypeObjectif( call.GetParameter( 0 ) ) );
    DEC_Objective* pTmp = call.GetParameter( 0 ).ToUserPtr( pTmp );
    assert( pTmp );
    call.GetResult().SetValue( (void*)&pTmp->GetLocalisation(), &DEC_Tools::GetTypeLocalisation(), 1 );
}

// -----------------------------------------------------------------------------
// Name: DEC_ObjectFunctions::IsObjectiveFlagged
// Created: NLD 2007-05-15
// -----------------------------------------------------------------------------
void DEC_ObjectFunctions::IsObjectiveFlagged( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypeObjectif( call.GetParameter( 0 ) ) );
    DEC_Objective* pTmp = call.GetParameter( 0 ).ToUserPtr( pTmp );
    assert( pTmp );
    call.GetResult().SetValue( pTmp->IsFlagged() );
}

// -----------------------------------------------------------------------------
// Name: DEC_ObjectFunctions::SetObjectiveFlag
// Created: NLD 2007-05-15
// -----------------------------------------------------------------------------
void DEC_ObjectFunctions::SetObjectiveFlag( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypeObjectif( call.GetParameter( 0 ) ) );
    DEC_Objective* pTmp = call.GetParameter( 0 ).ToUserPtr( pTmp );
    assert( pTmp );
    pTmp->Flag( call.GetParameter( 1 ).ToBool() );
}
