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
#include "Decision/DEC_Tools.h"

// =============================================================================
// LIMAS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::GetMissionLimaFlag
// Created: NLD 2003-12-23
// -----------------------------------------------------------------------------
template< typename T >
void DEC_OrdersFunctions::GetMissionLimaFlag( DIA_Call_ABC& call, const T& caller )
{
    assert( DEC_Tools::CheckTypeLima( call.GetParameter( 0 ) ) );
    MIL_LimaOrder* pLima = caller.GetOrderManager().FindLima( (uint)call.GetParameter( 0 ).ToPtr() );
    if( !pLima )
    {
        call.GetResult().SetValue( false );
        return;
    }
    call.GetResult().SetValue( pLima->IsFlagged() );
}

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::GetLima
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
    MIL_LimaOrder* pLima = caller.GetOrderManager().FindLima( *pFunction );
    if( !pLima )
    {
        call.GetResult().SetValue( (void*)0, &DEC_Tools::GetTypeLima() );
        return;
    }
    call.GetResult().SetValue( (void*)pLima->GetID(), &DEC_Tools::GetTypeLima() );
}

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::GetNextScheduledLima
// Created: NLD 2007-04-30
// -----------------------------------------------------------------------------
template< typename T >
void DEC_OrdersFunctions::GetNextScheduledLima( DIA_Call_ABC& call, const T& caller )
{
    MIL_LimaOrder* pLima = caller.GetOrderManager().FindNextScheduledLima();
    if( !pLima )
    {
        call.GetResult().SetValue( (void*)0, &DEC_Tools::GetTypeLima() );
        return;
    }
    call.GetResult().SetValue( (void*)pLima->GetID(), &DEC_Tools::GetTypeLima() );
}

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::GetFuseau
// Created: NLD 2007-04-11
// -----------------------------------------------------------------------------
template< typename T >
static void DEC_OrdersFunctions::GetFuseau( DIA_Call_ABC& call, const T& caller )
{
    call.GetResult().SetValue( (void*)&caller.GetOrderManager().GetFuseau(), &DEC_Tools::GetTypeFuseau(), 1 );
}

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::FinishMission
// Created: NLD 2005-09-13
// -----------------------------------------------------------------------------
template< typename T >
void DEC_OrdersFunctions::FinishMission( DIA_Call_ABC& /*call*/, T& caller )
{
    caller.GetOrderManager().ReplaceMission( 0 );
}

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::IsNewMissionStarted
// Created: NLD 2005-09-19
// -----------------------------------------------------------------------------
template< typename T > 
void DEC_OrdersFunctions::IsNewMissionStarted( DIA_Call_ABC& call, T& caller )
{
    call.GetResult().SetValue( caller.GetOrderManager().IsNewMissionStarted() );
}
