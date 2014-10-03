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
            MOCK_EXPECT( GetIdent ).returns( tree.get< std::string >( "ident" ) );
            MOCK_EXPECT( GetPort ).returns( tree.get< int >( "port" ) );
            MOCK_EXPECT( Save ).returns( tree );
            MOCK_EXPECT( GetExerciseProperties ).returns( host::Tree() );
        }
        MOCK_METHOD( GetId, 0 );
        MOCK_METHOD( GetRoot, 0 );
        MOCK_METHOD( GetIdent, 0 );
        MOCK_METHOD( GetPort, 0 );
        MOCK_METHOD( GetProperties, 0 );
        MOCK_METHOD( Save, 0 );
        MOCK_METHOD( Start, 6 );
        MOCK_METHOD( Stop, 1 );
        MOCK_METHOD( SoftKill, 0 );
        MOCK_METHOD( Remove, 1 );
        MOCK_METHOD( Update, 1 );
        MOCK_METHOD( GetInstall, 0 );
        MOCK_METHOD( DeleteInstall, 1 );
        MOCK_METHOD_EXT( DownloadInstall, 2, void( web::Chunker_ABC&, size_t ), DownloadInstallById );
        MOCK_METHOD_EXT( DownloadInstall, 4, void( web::Chunker_ABC&, const std::string&, const std::string&, const std::string& ), DownloadInstallByChecksum );
        MOCK_METHOD( UploadCache, 1 );
        MOCK_METHOD( GetCache, 0 );
        MOCK_METHOD( DeleteCache, 0 );
        MOCK_METHOD( InstallFromCache, 1 );
        MOCK_METHOD( GetExercises, 2 );
        MOCK_METHOD( CountExercises, 0 );
        MOCK_METHOD_EXT( LinkExercise, 1, host::Tree( const std::string& ), LinkExerciseName );
        MOCK_METHOD_EXT( LinkExercise, 1, host::Tree( const host::Tree& ), LinkExerciseTree );
        MOCK_METHOD( UnlinkExercise, 1 );
        MOCK_METHOD( StartSession, 2 );
        MOCK_METHOD( RemoveSession, 1 );
        MOCK_METHOD( UpdateSessionSize, 2 );
        MOCK_METHOD( FilterConfig, 1 );
        MOCK_METHOD( GetExerciseProperties, 1 );
    };

    MOCK_BASE_CLASS( MockNodeFactory, host::NodeFactory_ABC )
    {
        MOCK_METHOD_EXT( Make, 1, host::NodeFactory_ABC::Ptr( const host::Path& tag ), Make1 );
        MOCK_METHOD_EXT( Make, 3, host::NodeFactory_ABC::Ptr( const host::Path& root, const std::string& ident, const web::node::Config& ), Make3 );
    };
};

#endif // MOCK_NODE_H
