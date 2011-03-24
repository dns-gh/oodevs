// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef protocol_Version_h
#define protocol_Version_h

#pragma warning( push, 0 )
#include "protocol/proto/version.pb.h"
#pragma warning( pop )

namespace sword
{
    bool CheckCompatibility( const sword::ProtocolVersion& version );
    bool CheckCompatibility( const std::string& version1, const std::string& version2 );
}

#endif // protocol_Version_h
