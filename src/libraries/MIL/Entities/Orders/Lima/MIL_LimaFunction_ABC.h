
//*****************************************************************************
//
// $Created: NLD 2002-08-22 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Orders/Lima/MIL_LimaFunction_ABC.h $
// $Author: Nld $
// $Modtime: 16/06/04 10:14 $
// $Revision: 1 $
// $Workfile: MIL_LimaFunction_ABC.h $
//
//*****************************************************************************

#ifndef __MIL_LimaFunction_ABC_h_
#define __MIL_LimaFunction_ABC_h_

#include "MIL.h"
#include "MIL_Lima.h"

//=============================================================================
// Created: NLD 2002-08-22
// Last modified: JVT 02-10-02
//=============================================================================
class MIL_LimaFunction_ABC
{
public:
    MIL_LimaFunction_ABC( MIL_Lima::E_LimaFunctions, DIN::DIN_Input& input );
    virtual ~MIL_LimaFunction_ABC();

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    MIL_Lima::E_LimaFunctions GetType() const;
    //@}

private:
    MIL_Lima::E_LimaFunctions   nType_;
};

#   include "MIL_LimaFunction_ABC.inl"
#endif // __MIL_LimaFunction_ABC_h_
