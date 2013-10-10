// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef MIL_MessageParameters_h_
#define MIL_MessageParameters_h_

#include "protocol/MessageParameters.h"

// BAX migrate everybody to parameters::Check
// and remove this include and the following macros
#include "tools/NET_AsnException.h"

#define MASA_BADPARAM_MAGICACTION( reason )\
    MASA_BADPARAM_ASN( sword::MagicActionAck::ErrorCode,\
        sword::MagicActionAck::error_invalid_parameter,\
        STR( reason ) )

#define MASA_BADPARAM_UNIT( reason )\
    MASA_BADPARAM_ASN( sword::UnitActionAck::ErrorCode,\
        sword::UnitActionAck::error_invalid_parameter,\
        STR( reason ) )

namespace boost
{
    template< typename T > class function;
}

class DEC_Model_ABC;

namespace parameters
{
    typedef boost::function< const DEC_Model_ABC*( const std::string& model ) > ModelFinder;
    const DEC_Model_ABC* GetModel( const sword::MissionParameters& params, const ModelFinder& finder );
}

#endif // MIL_MessageParameters_h_
