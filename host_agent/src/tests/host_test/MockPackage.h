// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef MOCK_PACKAGE_H
#define MOCK_PACKAGE_H

#include "host/Package_ABC.h"

namespace mocks
{
    MOCK_BASE_CLASS( MockPackage, host::Package_ABC )
    {
        MOCK_METHOD( GetProperties, 0 );
        MOCK_METHOD( GetPath, 0 );
        MOCK_METHOD( Parse, 0 );
        MOCK_METHOD( Identify, 1 );
        MOCK_METHOD( Install, 4 );
        MOCK_METHOD( Uninstall, 3 );
        MOCK_METHOD_EXT( Find, 2, host::Package_ABC::T_Item( size_t, bool ), FindId );
        MOCK_METHOD_EXT( Find, 2, host::Package_ABC::T_Item( const host::Package_ABC::Item_ABC&, bool ), FindItem );
        MOCK_METHOD( GetExercises, 2 );
        MOCK_METHOD( CountExercises, 0 );
        MOCK_METHOD( LinkExercise, 1 );
        MOCK_METHOD( LinkItem, 1 );
        MOCK_METHOD( UnlinkItem, 2 );
    };

    MOCK_BASE_CLASS( MockPackageFactory, host::PackageFactory_ABC )
    {
        MOCK_METHOD( Make, 2 );
    };
};

#endif // MOCK_PACKAGE_H
