// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef MOCK_NODE_H
#define MOCK_NODE_H

#include "host/Node_ABC.h"

namespace mocks
{
    MOCK_BASE_CLASS( MockNode, host::Node_ABC )
    {
        MockNode( const host::Uuid& id, const host::Tree& tree )
        {
            MOCK_EXPECT( GetId ).returns( id );
            MOCK_EXPECT( GetRoot ).returns( "" );
            MOCK_EXPECT( GetName ).returns( tree.get< std::string >( "name" ) );
            MOCK_EXPECT( GetPort ).returns( tree.get< int >( "port" ) );
            MOCK_EXPECT( Save ).returns( tree );
        }
        MOCK_METHOD( GetId, 0 );
        MOCK_METHOD( GetRoot, 0 );
        MOCK_METHOD( GetName, 0 );
        MOCK_METHOD( GetPort, 0 );
        MOCK_METHOD( GetProperties, 0 );
        MOCK_METHOD( Save, 0 );
        MOCK_METHOD( Start, 6 );
        MOCK_METHOD( Stop, 1 );
        MOCK_METHOD( Remove, 2 );
        MOCK_METHOD( GetInstall, 0 );
        MOCK_METHOD( DeleteInstall, 1 );
        MOCK_METHOD( UploadCache, 1 );
        MOCK_METHOD( GetCache, 0 );
        MOCK_METHOD( DeleteCache, 0 );
        MOCK_METHOD( InstallFromCache, 1 );
        MOCK_METHOD( GetExercises, 2 );
        MOCK_METHOD( CountExercises, 0 );
        MOCK_METHOD_EXT( LinkExercise, 1, host::Tree( const std::string& ), LinkExerciseName );
        MOCK_METHOD_EXT( LinkExercise, 1, host::Tree( const host::Tree& ), LinkExerciseTree );
        MOCK_METHOD( UnlinkExercise, 1 );
    };

    MOCK_BASE_CLASS( MockNodeFactory, host::NodeFactory_ABC )
    {
        MOCK_METHOD_EXT( Make, 1, host::NodeFactory_ABC::Ptr( const host::Path& tag ), Make1 );
        MOCK_METHOD_EXT( Make, 2, host::NodeFactory_ABC::Ptr( const host::Path& root, const std::string& name ), Make2 );
    };
};

#endif // MOCK_NODE_H
