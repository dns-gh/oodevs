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
bool DEC_OrdersFunctions::GetMissionLimaFlag( const T& caller, unsigned int limaID)
{
    MIL_LimaOrder* pLima = caller.GetOrderManager().FindLima( limaID );
    if( !pLima )
    {
        return false;
    }
    return pLima->IsFlagged();
}

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::GetLima
// Created: NLD 2004-05-21
// -----------------------------------------------------------------------------
template< typename T >
unsigned int DEC_OrdersFunctions::GetLima( const T& caller, unsigned int limaId )
{
    const MIL_LimaFunction* pFunction = MIL_LimaFunction::Find( limaId );
    if( !pFunction )
        return 0;
    MIL_LimaOrder* pLima = caller.GetOrderManager().FindLima( *pFunction );
    if( !pLima )
        return 0;
    return pLima->GetID();
}

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::GetNextScheduledLima
// Created: NLD 2007-04-30
// -----------------------------------------------------------------------------
template< typename T >
unsigned int DEC_OrdersFunctions::GetNextScheduledLima( const T& caller )
{
    MIL_LimaOrder* pLima = caller.GetOrderManager().FindNextScheduledLima();
    if( !pLima )
    {
        return 0;
    }
    return pLima->GetID() ;
}

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::GetFuseau
// Created: NLD 2007-04-11
// -----------------------------------------------------------------------------
template< typename T >
static const MIL_Fuseau& DEC_OrdersFunctions::GetFuseau( const T& caller )
{
    return caller.GetOrderManager().GetFuseau();
}

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::FinishMission
// Created: NLD 2005-09-13
// -----------------------------------------------------------------------------
template< typename T >
void DEC_OrdersFunctions::FinishMission( T& caller )
{
    caller.GetOrderManager().ReplaceMission( 0 );
}

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::IsNewMissionStarted
// Created: NLD 2005-09-19
// -----------------------------------------------------------------------------
template< typename T > 
bool DEC_OrdersFunctions::IsNewMissionStarted( T& caller )
{
    return caller.GetOrderManager().IsNewMissionStarted();
}
