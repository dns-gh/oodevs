// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PathTools_h_
#define __PathTools_h_

namespace path_tools
{
    std::string BuildChildPath( const std::string& parent, const std::string& child );
    std::string BuildWorkingDirectoryPath( const std::string& file );
}

#endif // __PathTools_h_
