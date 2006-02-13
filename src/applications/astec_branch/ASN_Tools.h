//*****************************************************************************
//
// $Created: NLD 2003-02-28 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ASN_Tools.h $
// $Author: Ape $
// $Modtime: 18/02/04 16:08 $
// $Revision: 1 $
// $Workfile: ASN_Tools.h $
//
//*****************************************************************************

#ifndef __ASN_Tools_h_
#define __ASN_Tools_h_

#include "Types.h"

#include "ASN_Types.h"

//=============================================================================
// Created: NLD 2003-02-28
//=============================================================================
class ASN_Tools
{
public:
    static void BuildAsnGDH( ASN1T_GDH& asnGDH );
};

#   include "ASN_Tools.inl"

#endif // __ASN_Tools_h_
