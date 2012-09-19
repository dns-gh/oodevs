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
        MOCK_METHOD( GetSize, 0 );
        MOCK_METHOD( GetPath, 0 );
        MOCK_METHOD( Parse, 0 );
        MOCK_METHOD( Identify, 1 );
        MOCK_METHOD( Install, 4 );
        MOCK_METHOD( Uninstall, 3 );
        MOCK_METHOD_EXT( Find, 2, host::Package_ABC::T_Item( size_t, bool ), FindById );
        MOCK_METHOD_EXT( Find, 2, host::Package_ABC::T_Item( const host::Package_ABC::Item_ABC&, bool ), FindByItem );
        MOCK_METHOD_EXT( Find, 3, host::Package_ABC::T_Item( const std::string&, const std::string&, const std::string& ), FindByChecksum );
        MOCK_METHOD( GetExercises, 2 );
        MOCK_METHOD( CountExercises, 0 );
        MOCK_METHOD( LinkExercise, 1 );
        MOCK_METHOD_EXT( LinkItem, 1, host::Tree( host::Package_ABC::Item_ABC& ), LinkItemByRef );
        MOCK_METHOD_EXT( LinkItem, 1, host::Tree( const host::Tree& ), LinkItemByTree );
        MOCK_METHOD( UnlinkItem, 2 );
        MOCK_METHOD( Download, 2 );
    };

    MOCK_BASE_CLASS( MockPackageFactory, host::PackageFactory_ABC )
    {
        MOCK_METHOD( Make, 2 );
    };
};

#endif // MOCK_PACKAGE_H
