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

unsigned int DEC_Path_ABC::nIDIdx_ = 0;

// -----------------------------------------------------------------------------
// Name: DEC_Path_ABC constructor
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
DEC_Path_ABC::DEC_Path_ABC()
    : nID_( ++ nIDIdx_ )
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
