// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef MOCKS_H
#define MOCKS_H

#include <host/FileSystem_ABC.h>
#include <host/Node_ABC.h>
#include <host/NodeController_ABC.h>
#include <host/Pool_ABC.h>
#include <host/PortFactory_ABC.h>
#include <host/Proxy_ABC.h>
#include <host/Session_ABC.h>
#include <host/SessionController_ABC.h>
#include <host/UuidFactory_ABC.h>
#include <runtime/Process_ABC.h>
#include <runtime/Runtime_ABC.h>
#include <web/Client_ABC.h>

#include <boost/bind/apply.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/filesystem/path.hpp>
#include <cpplog/cpplog.hpp>

namespace mocks
{
    MOCK_BASE_CLASS( MockLog, cpplog::BaseLogger )
    {
        MOCK_METHOD( sendLogMessage, 1 );
        MockLog()
        {
            MOCK_EXPECT( sendLogMessage ).returns( true );
        }
    };

    MOCK_BASE_CLASS( MockNodeController, host::NodeController_ABC )
    {
        MockNodeController()
        {
            MOCK_EXPECT( Reload ).once();
        }
        MOCK_METHOD( Reload, 0 );
        MOCK_METHOD( List, 2 );
        MOCK_METHOD( Count, 0 );
        MOCK_METHOD( Has, 1 );
        MOCK_METHOD( Get, 1 );
        MOCK_METHOD( Create, 1 );
        MOCK_METHOD( Delete, 1 );
        MOCK_METHOD( Start, 1 );
        MOCK_METHOD( Stop, 1 );
    };

    MOCK_BASE_CLASS( MockNode, host::Node_ABC )
    {
        MOCK_METHOD( GetId, 0 );
        MOCK_METHOD( GetProperties, 0 );
    };

    MOCK_BASE_CLASS( MockSessionController, host::SessionController_ABC )
    {
        MockSessionController()
        {
            MOCK_EXPECT( Reload ).once();
        }
        MOCK_METHOD( Reload, 1 );
        MOCK_METHOD( List, 3 );
        MOCK_METHOD( Count, 1 );
        MOCK_METHOD( Has, 1 );
        MOCK_METHOD( Get, 1 );
        MOCK_METHOD( Create, 3 );
        MOCK_METHOD( Delete, 1 );
        MOCK_METHOD( Start, 1 );
        MOCK_METHOD( Stop, 1 );
        MOCK_METHOD( GetExercises, 0 );
    };

    MOCK_BASE_CLASS( MockSession, host::Session_ABC )
    {
        MOCK_METHOD( GetId, 0 );
        MOCK_METHOD( GetNode, 0 );
        MOCK_METHOD( GetProperties, 0 );
    };

    MOCK_BASE_CLASS( MockPort, host::Port_ABC )
    {
        MOCK_METHOD( Get, 0 );
        MockPort( int port )
        {
            MOCK_EXPECT( Get ).returns( port );
        }
    };

    MOCK_BASE_CLASS( MockPortFactory, host::PortFactory_ABC )
    {
        MOCK_METHOD_EXT( Create, 0, std::auto_ptr< host::Port_ABC >(), Create0 );
        MOCK_METHOD_EXT( Create, 1, std::auto_ptr< host::Port_ABC >( int ), Create1 );
    };

    MOCK_BASE_CLASS( MockProcess, runtime::Process_ABC )
    {
        MockProcess( int pid, std::string name )
        {
            MOCK_EXPECT( GetPid ).returns( pid );
            MOCK_EXPECT( GetName ).returns( name );
        }
        MOCK_METHOD( GetPid, 0 );
        MOCK_METHOD( GetName, 0 );
        MOCK_METHOD( Join, 1 );
        MOCK_METHOD( Kill, 1 );
    };

    MOCK_BASE_CLASS( MockRuntime, runtime::Runtime_ABC )
    {
        MOCK_METHOD( GetProcesses, 0 );
        MOCK_METHOD( GetProcess, 1 );
        MOCK_METHOD( Start, 3 );
        MOCK_METHOD( GetModuleFilename, 0 );
    };

    MOCK_BASE_CLASS( MockFileSystem, host::FileSystem_ABC )
    {
        MOCK_METHOD( IsFile, 1 );
        MOCK_METHOD( IsDirectory, 1 );
        MOCK_METHOD( Exists, 1 );
        MOCK_METHOD( CopyDirectory, 2 );
        MOCK_METHOD( CopyFile, 2 );
        MOCK_METHOD( MakeDirectory, 1 );
        MOCK_METHOD( Remove, 1 );
        MOCK_METHOD( WriteFile, 2 );
        MOCK_METHOD( ReadFile, 1 );
        MOCK_METHOD( Glob, 2 );
    };

    MOCK_BASE_CLASS( MockClient, web::Client_ABC )
    {
        MOCK_METHOD( Get, 4 );
    };

    MOCK_BASE_CLASS( MockPool, host::Pool_ABC )
    {
        MockPool()
        {
            MOCK_EXPECT( Post ).calls( boost::bind( boost::apply< void >(), _1 ) );
        }
        MOCK_METHOD( Post, 1 );
        MOCK_METHOD( Stop, 0 );
    };

    MOCK_BASE_CLASS( MockUuidFactory, host::UuidFactory_ABC )
    {
        MOCK_METHOD( Create, 0 );
    };

    MOCK_BASE_CLASS( MockProxy, host::Proxy_ABC )
    {
        MOCK_METHOD( GetPort, 0 );
        MOCK_METHOD( Register, 3 );
        MOCK_METHOD( Unregister, 1 );
    };
};

#endif // MOCKS_H
