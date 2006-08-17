// *****************************************************************************
//
// $Created: NLD 2004-07-08 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Decision/Functions/DEC_OrdersFunctions.inl $
// $Author: Nld $
// $Modtime: 21/10/04 10:17 $
// $Revision: 2 $
// $Workfile: DEC_OrdersFunctions.inl $
//
// *****************************************************************************

#include "Entities/Orders/Lima/MIL_Lima.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/MIL_Automate.h"

// =============================================================================
// LIMAS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Agent_ABC::SetMissionLimaFlag
// Created: NLD 2003-12-23
// -----------------------------------------------------------------------------
template< typename T >
void DEC_OrdersFunctions::SetMissionLimaFlag( DIA_Call_ABC& call, T& caller )
{
    const MIL_Lima* pLima = call.GetParameter( 0 ).ToUserPtr( pLima );
    assert( pLima );
    const bool bOut = caller.GetOrderManager().SetMissionLimaFlag( *pLima, call.GetParameter( 1 ).ToBool() );
    call.GetResult().SetValue( bOut );
}

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::AutomateSetMissionLimaFlag
// Created: NLD 2005-09-23
// -----------------------------------------------------------------------------
inline
void DEC_OrdersFunctions::AutomateSetMissionLimaFlag( DIA_Call_ABC& call, MIL_AgentPion& caller )
{
    const MIL_Lima* pLima = call.GetParameter( 0 ).ToUserPtr( pLima );
    assert( pLima );
    const bool bOut = caller.GetAutomate().GetOrderManager().SetMissionLimaFlag( *pLima, call.GetParameter( 1 ).ToBool() );
    call.GetResult().SetValue( bOut );
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_ABC::GetMissionLimaFlag
// Created: NLD 2003-12-23
// -----------------------------------------------------------------------------
template< typename T >
void DEC_OrdersFunctions::GetMissionLimaFlag( DIA_Call_ABC& call, const T& caller )
{
    const MIL_Lima* pLima = call.GetParameter( 0 ).ToUserPtr( pLima );
    assert( pLima );
    const bool bOut = caller.GetOrderManager().GetMissionLimaFlag( *pLima );
    call.GetResult().SetValue( bOut );
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_ABC::GetLima
// Created: NLD 2004-05-21
// -----------------------------------------------------------------------------
template< typename T >
void DEC_OrdersFunctions::GetLima( DIA_Call_ABC& call, const T& caller )
{
    MIL_Lima::E_LimaFunctions nLimaType = (MIL_Lima::E_LimaFunctions)call.GetParameter( 0 ).ToId();
    const T_LimaFlagedPtrMap& limas = caller.GetLimas();
    for( CIT_LimaFlagedPtrMap itLima = limas.begin(); itLima != limas.end(); ++itLima )
    {
        if( itLima->first->GetLimaFunction() == nLimaType )
        {
            call.GetResult().SetValue( (void*)itLima->first, &DEC_Tools::GetTypeLima(), 1 );
            return;
        }
    }
    call.GetResult().SetValue( (void*)0, &DEC_Tools::GetTypeLima() );
}

// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_OrdersFunctions::FinishMission
// Created: NLD 2005-09-13
// -----------------------------------------------------------------------------
template< typename T >
void DEC_OrdersFunctions::FinishMission( DIA_Call_ABC& /*call*/, T& caller )
{
    caller.GetOrderManager().OnReceiveRCMissionFinished();
}

// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_OrdersFunctions::IsNewMissionStarted
// Created: NLD 2005-09-19
// -----------------------------------------------------------------------------
template< typename T > 
void DEC_OrdersFunctions::IsNewMissionStarted( DIA_Call_ABC& call, T& caller )
{
    call.GetResult().SetValue( caller.GetOrderManager().IsNewMissionStarted() );
}
