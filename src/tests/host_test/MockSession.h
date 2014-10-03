// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef MOCK_SESSION_H
#define MOCK_SESSION_H

#include "host/Session_ABC.h"

namespace mocks
{
    MOCK_BASE_CLASS( MockSession, host::Session_ABC )
    {
        MockSession()
        {
            // NOTHING
        }
        MockSession( const host::Uuid& id, const host::Uuid& node, const host::Tree& tree )
        {
            MOCK_EXPECT( GetId ).returns( id );
            MOCK_EXPECT( GetRoot ).returns( "" );
            MOCK_EXPECT( GetNode ).returns( node );
            MOCK_EXPECT( GetExercise ).returns( tree.get< std::string >( "exercise" ) );
            MOCK_EXPECT( GetName ).returns( tree.get< std::string >( "name" ) );
            MOCK_EXPECT( GetPort ).returns( tree.get< int >( "port" ) );
            MOCK_EXPECT( Save ).returns( tree );
            MOCK_EXPECT( IsReplay ).returns( false );
            MOCK_EXPECT( HasReplays ).returns( false );
        }
        MOCK_METHOD( GetId, 0 );
        MOCK_METHOD( GetRoot, 0 );
        MOCK_METHOD( GetNode, 0 );
        MOCK_METHOD( GetExercise, 0 );
        MOCK_METHOD( GetName, 0 );
        MOCK_METHOD( GetPort, 0 );
        MOCK_METHOD( GetProperties, 0 );
        MOCK_METHOD( Save, 0 );
        MOCK_METHOD( Start, 3 );
        MOCK_METHOD( Stop, 0 );
        MOCK_METHOD( Refresh, 0 );
        MOCK_METHOD( RefreshSize, 0 );
        MOCK_METHOD( Poll, 0 );
        MOCK_METHOD( Pause, 0 );
        MOCK_METHOD( Remove, 0 );
        MOCK_METHOD( Update, 1 );
        MOCK_METHOD( Archive, 0 );
        MOCK_METHOD( Restore, 0 );
        MOCK_METHOD( Download, 1 );
        MOCK_METHOD( Replay, 1 );
        MOCK_METHOD( IsReplay, 0 );
        MOCK_METHOD( GetReplayId, 0 );
        MOCK_METHOD( HasReplays, 0 );
        MOCK_METHOD( AttachReplay, 1 );
        MOCK_METHOD( DetachReplay, 1 );
        MOCK_METHOD( NotifyNode, 0 );
        MOCK_METHOD( AvailableLogs, 0 );
        MOCK_METHOD( DownloadLog, 4 );
        MOCK_METHOD( IsAuthorized, 1 );
        MOCK_METHOD( DeleteUser, 2 );
    };

    MOCK_BASE_CLASS( MockSessionFactory, host::SessionFactory_ABC )
    {
        MOCK_METHOD_EXT( Make, 2, host::Session_ABC::T_Ptr( const host::Path& tag,  const host::Path& trash ), Make2 );
        MOCK_METHOD_EXT( Make, 6, host::Session_ABC::T_Ptr( const host::Path& root, const host::Path& trash,
                         const host::Uuid& node, const web::session::Config& cfg, const std::string& exercise, const web::User& owner ), Make5 );
    };
};

#endif // MOCK_SESSION_H
