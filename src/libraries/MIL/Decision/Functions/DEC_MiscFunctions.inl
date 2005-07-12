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

#include "Entities/RC/MIL_RC.h"

//-----------------------------------------------------------------------------
// Name: DEC_Agent_ABC::RC_Operational
// Created: NLD 2003-01-07
//-----------------------------------------------------------------------------
template< typename T > 
void DEC_MiscFunctions::RC_Operational( DIA_Call_ABC& call, T& caller )
{
    const MIL_RC* pRC = MIL_RC::Find( call.GetParameter( 0 ).ToId() );
    assert( pRC );
    pRC->Send( caller, MIL_RC::eRcTypeOperational, call.GetParameters() );
}


//-----------------------------------------------------------------------------
// Name: DEC_Agent_ABC::RC_Message
// Created: NLD 2003-01-07
//-----------------------------------------------------------------------------
template< typename T > 
void DEC_MiscFunctions::RC_Message( DIA_Call_ABC& call, T& caller )
{
    const MIL_RC* pRC = MIL_RC::Find( call.GetParameter( 0 ).ToId() );
    assert( pRC );
    pRC->Send( caller, MIL_RC::eRcTypeMessage, call.GetParameters() );
}
