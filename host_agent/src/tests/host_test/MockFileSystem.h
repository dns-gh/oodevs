// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef MOCK_FILE_SYSTEM_H
#define MOCK_FILE_SYSTEM_H

#include "runtime/FileSystem_ABC.h"
#include <boost/foreach.hpp>

namespace mocks
{
    MOCK_BASE_CLASS( MockFileSystem, runtime::FileSystem_ABC )
    {
        MOCK_METHOD( IsFile, 1 );
        MOCK_METHOD( IsDirectory, 1 );
        MOCK_METHOD( Exists, 1 );
        MOCK_METHOD( CopyDirectory, 2 );
        MOCK_METHOD( CopyFile, 2 );
        MOCK_METHOD( MakePaths, 1 );
        MOCK_METHOD( MakePath, 1 );
        MOCK_METHOD( Remove, 1 );
        MOCK_METHOD( Rename, 2 );
        MOCK_METHOD( WriteFile, 2 );
        MOCK_METHOD( ReadFile, 1 );
        MOCK_METHOD( Walk, 3 );
        MOCK_METHOD( Unpack, 3 );
        MOCK_METHOD( Pack, 2 );
        MOCK_METHOD( Checksum, 3 );
        MOCK_METHOD( MakeAnyPath, 1 );
        MOCK_METHOD( GetLastWrite, 1 );
        MOCK_METHOD( GetDirectorySize, 1 );

        bool Apply( const T_Predicate& operand, const std::vector< boost::filesystem::path >& list )
        {
            BOOST_FOREACH( const boost::filesystem::path& it, list )
                operand( it );
            return true;
        }
    };

    MOCK_BASE_CLASS( MockUnpack, runtime::Unpacker_ABC )
    {
        MOCK_METHOD( Unpack, 0 );
    };
};

#endif // MOCK_FILE_SYSTEM_H
