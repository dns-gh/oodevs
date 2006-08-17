//*****************************************************************************
//
// $Created: NLD 2002-08-22 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Orders/Lima/MIL_LimaFunction_ABC.cpp $
// $Author: Nld $
// $Modtime: 16/06/04 10:14 $
// $Revision: 1 $
// $Workfile: MIL_LimaFunction_ABC.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_LimaFunction_ABC.h"

using namespace DIN;

//-----------------------------------------------------------------------------
// Name: MIL_LimaFunction_ABC constructor
// Created: NLD 2002-08-22
//-----------------------------------------------------------------------------
MIL_LimaFunction_ABC::MIL_LimaFunction_ABC( MIL_Lima::E_LimaFunctions nType, DIN_Input& )
    : nType_ ( nType )
{    
}


//-----------------------------------------------------------------------------
// Name: MIL_LimaFunction_ABC destructor
// Created: NLD 2002-08-22
//-----------------------------------------------------------------------------
MIL_LimaFunction_ABC::~MIL_LimaFunction_ABC()
{
    
}
