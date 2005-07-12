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

#include "MIL_pch.h"
#include "DEC_ObjectFunctions.h"

#include "Entities/Agents/Actions/Objects/PHY_RoleAction_Objects.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Objects/MIL_CampPrisonniers.h"
#include "Entities/Objects/MIL_CampRefugies.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: DEC_ObjectFunctions::ActivateObject
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
void DEC_ObjectFunctions::ActivateObject( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypeConnaissanceObjet( call.GetParameter( 0 ) ) );

    int nReturnCode = callerAgent.GetRole< PHY_RoleAction_Objects >().Activate( (uint)call.GetParameter( 0 ).ToPtr() );
    call.GetResult().SetValue( nReturnCode );
}

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
