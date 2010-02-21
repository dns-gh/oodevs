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

#include "Types.h"
#include "clients_kernel/Tools.h"
#include "protocol/Protocol.h"

using namespace Common;


namespace tools
{
    QString ToString( Common::MsgLocation::Geometry nType );

    E_LocationType     LocationFromString        ( const QString& type );
    E_AmmunitionFamily AmmunitionFamilyFromString( const QString& type );
    E_DotationFamily   DotationFamilyFromString  ( const QString& type );
};

#endif // __Gaming_Tools_h_
