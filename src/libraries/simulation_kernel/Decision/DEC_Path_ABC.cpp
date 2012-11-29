//*****************************************************************************
//
// $Created: AGN 02-11-25 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_Path_ABC.cpp $
// $Author: Age $
// $Modtime: 4/03/05 13:48 $
// $Revision: 3 $
// $Workfile: DEC_Path_ABC.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Path_ABC.h"
#include "MIL_AgentServer.h"
#include "DEC_PathFind_Manager.h"

unsigned int DEC_Path_ABC::nIDIdx_ = 0;

// -----------------------------------------------------------------------------
// Name: DEC_Path_ABC constructor
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
DEC_Path_ABC::DEC_Path_ABC()
    : nID_( ++ nIDIdx_ )
    , nNbrRefs_    ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_ABC destructor
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
DEC_Path_ABC::~DEC_Path_ABC()
{
    // NOTHING
}
// -----------------------------------------------------------------------------
// Name: DEC_Path_ABC::AddRef
// Created: LDC 2012-05-15
// -----------------------------------------------------------------------------
void DEC_Path_ABC::AddRef()
{
    ++nNbrRefs_;
}
    
// -----------------------------------------------------------------------------
// Name: DEC_Path_ABC::DecRef
// Created: LDC 2012-05-15
// -----------------------------------------------------------------------------
void DEC_Path_ABC::DecRef()
{
    if( --nNbrRefs_ <= 0 )
        MIL_AgentServer::GetWorkspace().GetPathFindManager().CancelJob( this );
}

