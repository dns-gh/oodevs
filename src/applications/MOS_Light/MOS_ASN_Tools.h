//*****************************************************************************
//
// $Created: NLD 2003-02-28 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_ASN_Tools.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 2 $
// $Workfile: MOS_ASN_Tools.h $
//
//*****************************************************************************

#ifndef __MOS_ASN_Tools_h_
#define __MOS_ASN_Tools_h_

#include "MOS_Types.h"

#include "MOS_ASN_Types.h"

//=============================================================================
// Created: NLD 2003-02-28
//=============================================================================
class MOS_ASN_Tools
{
public:
    static void BuildAsnGDH( ASN1T_GDH& asnGDH );
};

#ifdef MOS_USE_INLINE
#   include "MOS_ASN_Tools.inl"
#endif


#endif // __MOS_ASN_Tools_h_
