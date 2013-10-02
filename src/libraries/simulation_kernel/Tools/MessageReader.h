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

#define MASA_BADPARAM_MAGICACTION(reason)               \
    MASA_BADPARAM_ASN( sword::MagicActionAck::ErrorCode,\
        sword::MagicActionAck::error_invalid_parameter, \
        static_cast< std::stringstream& >( std::stringstream() << reason ).str().c_str() )

#define MASA_BADPARAM_UNIT(reason)                      \
    MASA_BADPARAM_ASN( sword::UnitActionAck::ErrorCode, \
        sword::UnitActionAck::error_invalid_parameter,  \
        static_cast< std::stringstream& >( std::stringstream() << reason ).str().c_str() )



#endif // __MessageReader_h_
