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
// Name: DEC_OrdersFunctions::GetMissionLimaFlag
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
// Name: DEC_OrdersFunctions::GetMissionLimaScheduleFlag
// Created: NLD 2003-12-23
// -----------------------------------------------------------------------------
template< typename T >
void DEC_OrdersFunctions::GetMissionLimaScheduleFlag( DIA_Call_ABC& call, const T& caller )
{
    assert( DEC_Tools::CheckTypeLima( call.GetParameter( 0 ) ) );
    MIL_LimaOrder* pLima = caller.FindLima( (uint)call.GetParameter( 0 ).ToPtr() );
    if( !pLima )
    {
        call.GetResult().SetValue( false );
        return;
    }
    call.GetResult().SetValue( pLima->IsScheduleFlagged() );
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
    MIL_LimaOrder* pLima = caller.FindLima( *pFunction );
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
    MIL_LimaOrder* pLima = caller.FindNextScheduledLima();
    if( !pLima )
    {
        call.GetResult().SetValue( (void*)0, &DEC_Tools::GetTypeLima() );
        return;
    }
    call.GetResult().SetValue( (void*)pLima->GetID(), &DEC_Tools::GetTypeLima() );
}

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::GetNextScheduledElement
// Created: NLD 2007-05-14
// -----------------------------------------------------------------------------
template< typename T > 
void DEC_OrdersFunctions::GetNextScheduledElement( DIA_Call_ABC& call, const T& caller )
{
    //
    typedef std::vector< DEC_Objective* >     T_ObjectiveVector;
    typedef T_ObjectiveVector::iterator       IT_ObjectiveVector;
    typedef T_ObjectiveVector::const_iterator CIT_ObjectiveVector;
    //

    assert( DEC_Tools::CheckTypeListeObjectifs( call.GetParameter( 0 ) ) );

    T_ObjectVariableVector& objectives = const_cast< T_ObjectVariableVector& >( static_cast< DIA_Variable_ObjectList& >( call.GetParameter( 0 ) ).GetContainer() );

    const MIL_LimaOrder* pNextLima = caller.FindNextScheduledLima();

    const DIA_Variable_ABC* pNextObjective = 0;
    for( CIT_ObjectVariableVector it = objectives.begin(); it != objectives.end(); ++it )
    {
        const DEC_Objective* pObjective = (**it).ToUserPtr( pObjective );
        if( pObjective->GetSchedule() == 0 || pObjective->IsFlagged() )
            continue;

        if( ( pObjective->GetSchedule() != 0 ) && ( !pNextObjective || pObjective->GetSchedule() < static_cast< DEC_Objective* >( pNextObjective->ToPtr() )->GetSchedule() )
                                               && ( !pNextLima      || pObjective->GetSchedule() < pNextLima                                               ->GetSchedule() ) )
            pNextObjective = *it;
    }

    if( pNextObjective )
        call.GetResult() = *pNextObjective;
    else if( pNextLima )
        call.GetResult().SetValue( (void*)pNextLima->GetID(), &DEC_Tools::GetTypeLima() );
    else 
        call.GetResult().SetValue( (void*)0, &DEC_Tools::GetTypeLima() ); ///
}

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::GetFuseau
// Created: NLD 2007-04-11
// -----------------------------------------------------------------------------
template< typename T >
static void DEC_OrdersFunctions::GetFuseau( DIA_Call_ABC& call, const T& caller )
{
    call.GetResult().SetValue( (void*)&caller.GetFuseau(), &DEC_Tools::GetTypeFuseau(), 1 );
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
