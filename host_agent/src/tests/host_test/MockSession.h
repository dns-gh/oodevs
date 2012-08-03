// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Math�matiques Appliqu�es SA (MASA)
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
        }
        MOCK_METHOD( GetId, 0 );
        MOCK_METHOD( GetRoot, 0 );
        MOCK_METHOD( GetNode, 0 );
        MOCK_METHOD( GetExercise, 0 );
        MOCK_METHOD( GetName, 0 );
        MOCK_METHOD( GetPort, 0 );
        MOCK_METHOD( GetProperties, 0 );
        MOCK_METHOD( Save, 0 );
        MOCK_METHOD( Start, 2 );
        MOCK_METHOD( Stop, 0 );
        MOCK_METHOD( Refresh, 0 );
        MOCK_METHOD( RefreshSize, 0 );
        MOCK_METHOD( Poll, 0 );
        MOCK_METHOD( Pause, 0 );
        MOCK_METHOD( Remove, 0 );
    };

    MOCK_BASE_CLASS( MockSessionFactory, host::SessionFactory_ABC )
    {
        MOCK_METHOD_EXT( Make, 1, host::SessionFactory_ABC::Ptr( const host::Path& tag ), Make1 );
        MOCK_METHOD_EXT( Make, 4, host::SessionFactory_ABC::Ptr( const host::Path& root, const host::Uuid& node, const web::session::Config& cfg, const std::string& exercise ), Make4 );
    };
};

#endif // MOCK_SESSION_H
