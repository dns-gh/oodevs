// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Gaming_Tools_h_
#define __Gaming_Tools_h_

#include "clients_kernel/Tools.h"
#include "Types.h"
#include "ASN_Types.h"

namespace gaming
{
    namespace tools
    {
        const char* ToString( E_FuncLimaType nType );
        const char* ToString( ASN1T_EnumTypeLocalisation nType );
    };
}

#endif // __Gaming_Tools_h_
