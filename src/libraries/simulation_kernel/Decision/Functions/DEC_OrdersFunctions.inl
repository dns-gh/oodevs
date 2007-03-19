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

#include "Entities/Orders/MIL_LimaOrder.h"
#include "Entities/Orders/MIL_LimaFunction.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Decision/DEC_Tools.h"

// =============================================================================
// LIMAS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::AutomateSetMissionLimaFlag
// Created: NLD 2005-09-23
// -----------------------------------------------------------------------------
inline
void DEC_OrdersFunctions::AutomateSetMissionLimaFlag( DIA_Call_ABC& call, MIL_AgentPion& caller )
{
    assert( DEC_Tools::CheckTypeLima( call.GetParameter( 0 ) ) );
    MIL_LimaOrder* pLima = caller.GetAutomate().FindLima( (uint)call.GetParameter( 0 ).ToPtr() );
    if( !pLima )
    {
        call.GetResult().SetValue( false );
        return;
    }
    pLima->Flag( call.GetParameter( 1 ).ToBool() );
    call.GetResult().SetValue( false );
}

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::AutomateGetMissionLimaFlag
// Created: NLD 2007-03-19
// -----------------------------------------------------------------------------
inline
void DEC_OrdersFunctions::AutomateGetMissionLimaFlag( DIA_Call_ABC& call, MIL_AgentPion& caller )
{
    assert( DEC_Tools::CheckTypeLima( call.GetParameter( 0 ) ) );
    MIL_LimaOrder* pLima = caller.GetAutomate().FindLima( (uint)call.GetParameter( 0 ).ToPtr() );
    if( !pLima )
    {
        call.GetResult().SetValue( false );
        return;
    }
    call.GetResult().SetValue( pLima->IsFlagged() );
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_ABC::SetMissionLimaFlag
// Created: NLD 2003-12-23
// -----------------------------------------------------------------------------
template< typename T >
void DEC_OrdersFunctions::SetMissionLimaFlag( DIA_Call_ABC& call, T& caller )
{
    assert( DEC_Tools::CheckTypeLima( call.GetParameter( 0 ) ) );
    MIL_LimaOrder* pLima = caller.FindLima( (uint)call.GetParameter( 0 ).ToPtr() );
    if( !pLima )
    {
        call.GetResult().SetValue( false );
        return;
    }
    pLima->Flag( call.GetParameter( 1 ).ToBool() );
    call.GetResult().SetValue( false );
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_ABC::GetMissionLimaFlag
// Created: NLD 2003-12-23
// -----------------------------------------------------------------------------
template< typename T >
void DEC_OrdersFunctions::GetMissionLimaFlag( DIA_Call_ABC& call, const T& caller )
{
    assert( DEC_Tools::CheckTypeLima( call.GetParameter( 0 ) ) );
    MIL_LimaOrder* pLima = caller.FindLima( (uint)call.GetParameter( 0 ).ToPtr() );
    if( !pLima )
    {
        call.GetResult().SetValue( false );
        return;
    }
    call.GetResult().SetValue( pLima->IsFlagged() );
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_ABC::GetLima
// Created: NLD 2004-05-21
// -----------------------------------------------------------------------------
template< typename T >
void DEC_OrdersFunctions::GetLima( DIA_Call_ABC& call, const T& caller )
{
    const MIL_LimaFunction* pFunction = MIL_LimaFunction::Find( call.GetParameter( 0 ).ToId() );
    if( !pFunction )
    {
        call.GetResult().SetValue( (void*)0, &DEC_Tools::GetTypeLima() );
        return;
    }
    MIL_LimaOrder* pLima = caller.FindLima( *pFunction );
    if( !pLima )
    {
        call.GetResult().SetValue( (void*)0, &DEC_Tools::GetTypeLima() );
        return;
    }
    call.GetResult().SetValue( (void*)pLima->GetID(), &DEC_Tools::GetTypeLima() );
}

// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_OrdersFunctions::FinishMission
// Created: NLD 2005-09-13
// -----------------------------------------------------------------------------
template< typename T >
void DEC_OrdersFunctions::FinishMission( DIA_Call_ABC& /*call*/, T& caller )
{
    caller.GetOrderManager().ReplaceMission( 0 );
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
