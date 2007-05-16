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
#include "game_asn/Asn.h"

namespace tools
{
    QString ToString( E_FuncLimaType nType );
    QString ToString( ASN1T_EnumTypeLocalisation nType );
    E_FuncLimaType FromString( const QString& type );
    E_TypeLocalisation LocationFromString( const QString& type );
};

#endif // __Gaming_Tools_h_
