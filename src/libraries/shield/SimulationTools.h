// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef shield_SimulationTools_h
#define shield_SimulationTools_h

#define SHIELD_SIMULATION
#include "CommonTools.h"

#define CONVERT_VERSION( field ) \
    to->mutable_####field()->set_value( Version::ProtocolVersion().value() )

namespace shield
{
    template< typename From, typename To >
    void ConvertProfile( const From& from, To* to )
    {
        ConvertProfileDescription( from, to );
        CONVERT( time_control );
        CONVERT_LIST( read_only_formations, elem, ConvertIdentifier );
        CONVERT_LIST( read_write_formations, elem, ConvertIdentifier );
        CONVERT_LIST( read_only_automates, elem, ConvertIdentifier );
        CONVERT_LIST( read_write_automates, elem, ConvertIdentifier );
        CONVERT_LIST_TO( read_only_parties, read_only_camps, elem, ConvertIdentifier );
        CONVERT_LIST_TO( read_write_parties, read_write_camps, elem, ConvertIdentifier );
        CONVERT_LIST( read_only_crowds, elem, ConvertIdentifier );
        CONVERT_LIST( read_write_crowds, elem, ConvertIdentifier );
    }
}

#endif // shield_SimulationTools_h
