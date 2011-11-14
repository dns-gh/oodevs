// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __reports_Tools_h_
#define __reports_Tools_h_

#include "clients_kernel/Tools.h"
#include "clients_kernel/Types.h"
#include "protocol/Simulation.h"

namespace tools
{
    QString ToString(  sword::Location::Geometry nType );

    E_LocationType     LocationFromString        ( const QString& type );
    E_DotationFamily   DotationFamilyFromString  ( const QString& type );
};

#endif // __reports_Tools_h_
