// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_AutomateFunctions.cpp $
// $Author: Jvt $
// $Modtime: 11/05/05 11:03 $
// $Revision: 20 $
// $Workfile: DEC_AutomateFunctions.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "DEC_PopulationFunctions.h"

#include "Entities/Populations/MIL_Population.h"
#include "MIL_AgentServer.h"

//-----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::Debug
// Created: NLD 2002-12-17
//-----------------------------------------------------------------------------
void DEC_PopulationFunctions::Debug( DIA_Call_ABC& call, const MIL_Population& callerPopulation )
{
    if( !MIL_AgentServer::GetWorkspace().GetConfig().UseDecDebug() )
        return;

    std::string msg( call.GetParameter( 0 ).ToString() );
    MT_LOG_INFO_MSG( MT_FormatString( "Population %d says : [%s]", callerPopulation.GetID(), msg.c_str() ).c_str() );
}
