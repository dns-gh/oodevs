// *****************************************************************************
//
// $Created: NLD 2004-07-08 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Decision/Functions/DEC_MiscFunctions.inl $
// $Author: Nld $
// $Modtime: 9/03/05 16:17 $
// $Revision: 2 $
// $Workfile: DEC_MiscFunctions.inl $
//
// *****************************************************************************

#include "Entities/Orders/MIL_Report.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/MIL_Automate.h"

//-----------------------------------------------------------------------------
// Name: DEC_Agent_ABC::RC_Operational
// Created: NLD 2003-01-07
//-----------------------------------------------------------------------------
template< typename T > 
void DEC_MiscFunctions::RC_Operational( DIA_Call_ABC& call, T& caller )
{
    const MIL_Report* pReport = MIL_Report::Find( call.GetParameter( 0 ).ToId() );
    if( pReport )
        pReport->Send( caller, MIL_Report::eRcTypeOperational, call.GetParameters() );
}

//-----------------------------------------------------------------------------
// Name: DEC_Agent_ABC::RC_Message
// Created: NLD 2003-01-07
//-----------------------------------------------------------------------------
template< typename T > 
void DEC_MiscFunctions::RC_Message( DIA_Call_ABC& call, T& caller )
{
    const MIL_Report* pReport = MIL_Report::Find( call.GetParameter( 0 ).ToId() );
    if( pReport )
        pReport->Send( caller, MIL_Report::eRcTypeMessage, call.GetParameters() );
}

//-----------------------------------------------------------------------------
// Name: DEC_Agent_ABC::RC_Warning
// Created: NLD 2003-01-07
//-----------------------------------------------------------------------------
template< typename T > 
void DEC_MiscFunctions::RC_Warning( DIA_Call_ABC& call, T& caller )
{
    const MIL_Report* pReport = MIL_Report::Find( call.GetParameter( 0 ).ToId() );
    if( pReport )
        pReport->Send( caller, MIL_Report::eRcTypeWarning, call.GetParameters() );
}