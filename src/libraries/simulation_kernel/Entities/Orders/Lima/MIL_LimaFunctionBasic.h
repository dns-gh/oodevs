//*****************************************************************************
//
// $Created: NLD 2002-08-22 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Orders/Lima/MIL_LimaFunctionBasic.h $
// $Author: Nld $
// $Modtime: 16/06/04 10:14 $
// $Revision: 1 $
// $Workfile: MIL_LimaFunctionBasic.h $
//
//*****************************************************************************

#ifndef __MIL_LimaFunctionBasic_h_
#define __MIL_LimaFunctionBasic_h_

#include "MIL.h"
#include "MIL_LimaFunction_ABC.h"

//=============================================================================
// Created: NLD 2002-08-22
//=============================================================================
class MIL_LimaFunctionBasic : public MIL_LimaFunction_ABC
{
    MT_COPYNOTALLOWED( MIL_LimaFunctionBasic );

public:
    MIL_LimaFunctionBasic( MIL_Lima::E_LimaFunctions nType, DIN::DIN_Input& input );
    virtual ~MIL_LimaFunctionBasic();
};

#   include "MIL_LimaFunctionBasic.inl"
#endif // __MIL_LimaFunctionBasic_h_
