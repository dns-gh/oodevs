// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef shield_ClientTools_h
#define shield_ClientTools_h

#include "CommonTools.h"

namespace shield
{
    template< typename From, typename To >
    void ConvertProfile( const From& from, To* to )
    {
        ConvertProfileDescription( from, to );
        CONVERT_LIST( read_only_formations, elem, ConvertIdentifier );
        CONVERT_LIST( read_write_formations, elem, ConvertIdentifier );
        CONVERT_LIST( read_only_automates, elem, ConvertIdentifier );
        CONVERT_LIST( read_write_automates, elem, ConvertIdentifier );
        CONVERT_LIST_TO( read_only_camps, read_only_parties, elem, ConvertIdentifier );
        CONVERT_LIST_TO( read_write_camps, read_write_parties, elem, ConvertIdentifier );
        CONVERT_LIST( read_only_crowds, elem, ConvertIdentifier );
        CONVERT_LIST( read_write_crowds, elem, ConvertIdentifier );
    }
}

#endif // shield_ClientTools_h
