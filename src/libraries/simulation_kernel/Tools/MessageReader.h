// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MessageReader_h_
#define __MessageReader_h_

#include "tools/NET_AsnException.h"

#define MASA_BADPARAM_MAGICACTION( name ) MASA_BADPARAM_ASN( sword::MagicActionAck_ErrorCode, sword::MagicActionAck::error_invalid_parameter, name )
#define MASA_BADPARAM_UNIT( name ) MASA_BADPARAM_ASN( sword::UnitActionAck_ErrorCode, sword::UnitActionAck::error_invalid_parameter, name )


#endif // __MessageReader_h_
