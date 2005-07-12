//*****************************************************************************
//
// $Basiceated: NLD 2002-08-22 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Orders/Lima/MIL_LimaFunctionBasic.cpp $
// $Author: Nld $
// $Modtime: 16/06/04 10:14 $
// $Revision: 1 $
// $Workfile: MIL_LimaFunctionBasic.cpp $
//
//*****************************************************************************

#include "MIL_Pch.h"
#include "MIL_LimaFunctionBasic.h"

using namespace DIN;

//-----------------------------------------------------------------------------
// Name: MIL_LimaFunctionBasic constructor
// Basiceated: NLD 2002-08-22
// Last modified: JVT 02-10-02
//-----------------------------------------------------------------------------
MIL_LimaFunctionBasic::MIL_LimaFunctionBasic( MIL_Lima::E_LimaFunctions nType, DIN_Input& input )
: MIL_LimaFunction_ABC( nType, input )
{
    
}


//-----------------------------------------------------------------------------
// Name: MIL_LimaFunctionBasic destructor
// Basiceated: NLD 2002-08-22
//-----------------------------------------------------------------------------
MIL_LimaFunctionBasic::~MIL_LimaFunctionBasic()
{
    
}
