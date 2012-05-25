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
#include <host/Package_ABC.h>
#include <host/Pool_ABC.h>
#include <host/PortFactory_ABC.h>
#include <host/Proxy_ABC.h>
#include <host/Session_ABC.h>
#include <host/SessionController_ABC.h>
#include <host/UuidFactory_ABC.h>
#include <runtime/Process_ABC.h>
#include <runtime/Runtime_ABC.h>
#include <web/Client_ABC.h>

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
        MOCK_METHOD( GetCache, 1 );
        MOCK_METHOD( UploadCache, 2 );
        MOCK_METHOD( DeleteCache, 1 );
        MOCK_METHOD( InstallFromCache, 2 );
    };

    MOCK_BASE_CLASS( MockNode, host::Node_ABC )
    {
        MockNode( const host::Uuid& id, const host::Tree& tree )
        {
            MOCK_EXPECT( GetId ).returns( id );
            MOCK_EXPECT( GetName ).returns( tree.get< std::string >( "name" ) );
            MOCK_EXPECT( GetPort ).returns( tree.get< int >( "port" ) );
            MOCK_EXPECT( Save ).returns( tree );
        }
        MOCK_METHOD( GetId, 0 );
        MOCK_METHOD( GetName, 0 );
        MOCK_METHOD( GetPort, 0 );
        MOCK_METHOD( GetProperties, 0 );
        MOCK_METHOD( GetCachePath, 0 );
        MOCK_METHOD( GetInstallPath, 0 );
        MOCK_METHOD( Save, 0 );
        MOCK_METHOD( Start, 1 );
        MOCK_METHOD( Stop, 0 );
        MOCK_METHOD( UploadCache, 1 );
        MOCK_METHOD( GetCache, 0 );
        MOCK_METHOD( DeleteCache, 0 );
        MOCK_METHOD( InstallFromCache, 1 );
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
        MockSession()
        {
            // NOTHING
        }
        MockSession( const host::Uuid& id, const host::Uuid& node, const host::Tree& tree )
        {
            MOCK_EXPECT( GetId ).returns( id );
            MOCK_EXPECT( GetExercise ).returns( tree.get< std::string >( "exercise" ) );
            MOCK_EXPECT( GetName ).returns( tree.get< std::string >( "name" ) );
            MOCK_EXPECT( GetNode ).returns( node );
            MOCK_EXPECT( GetPort ).returns( tree.get< int >( "port" ) );
            MOCK_EXPECT( GetConfiguration ).returns( "some config" );
            MOCK_EXPECT( Save ).returns( tree );
        }
        MOCK_METHOD( GetId, 0 );
        MOCK_METHOD( GetNode, 0 );
        MOCK_METHOD( GetExercise, 0 );
        MOCK_METHOD( GetName, 0 );
        MOCK_METHOD( GetPort, 0 );
        MOCK_METHOD( GetConfiguration, 0 );
        MOCK_METHOD( GetProperties, 0 );
        MOCK_METHOD( Save, 0 );
        MOCK_METHOD( Start, 1 );
        MOCK_METHOD( Stop, 0 );
    };

    MOCK_BASE_CLASS( MockSessionFactory, host::SessionFactory_ABC )
    {
        MOCK_METHOD_EXT( Make, 1, host::SessionFactory_ABC::Ptr( const host::Tree& tree ), Make1 );
        MOCK_METHOD_EXT( Make, 3, host::SessionFactory_ABC::Ptr( const host::Uuid& node, const std::string& name, const std::string& exercise ), Make3 );
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
        MOCK_METHOD( Start, 4 );
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
        MOCK_METHOD( Walk, 2 );
        MOCK_METHOD( Unpack, 2 );
        MOCK_METHOD( Checksum, 2 );
    };

    MOCK_BASE_CLASS( MockUnpack, host::Unpacker_ABC )
    {
        MOCK_METHOD( Unpack, 0 );
    };

    MOCK_BASE_CLASS( MockClient, web::Client_ABC )
    {
        MOCK_METHOD( Get, 4 );
    };

    MOCK_BASE_CLASS( MockPool, host::Pool_ABC )
    {
        MockPool()
        {
            MOCK_EXPECT( Post ).calls( &MockPool::Call );
        }
        static Future Call( Task task )
        {
            boost::promise< void > promise;
            task();
            promise.set_value();
            return promise.get_future();
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

    MOCK_BASE_CLASS( MockPackage, host::Package_ABC )
    {
        MOCK_METHOD( GetProperties, 0 );
        MOCK_METHOD( Parse, 0 );
        MOCK_METHOD( Identify, 1 );
        MOCK_METHOD( Install, 2 );
        MOCK_METHOD_EXT( Find, 1, host::Package_ABC::T_Item( size_t ), FindId );
        MOCK_METHOD_EXT( Find, 1, host::Package_ABC::T_Item( const host::Package_ABC::Item_ABC& ), FindItem );
        MOCK_METHOD( Remove, 1 );
    };

    MOCK_BASE_CLASS( MockPackageFactory, host::PackageFactory_ABC )
    {
        MOCK_METHOD( Make, 2 );
    };

    MOCK_BASE_CLASS( MockNodeFactory, host::NodeFactory_ABC )
    {
        MOCK_METHOD_EXT( Make, 1, host::NodeFactory_ABC::Ptr( const host::Tree& tree ), Make1 );
        MOCK_METHOD_EXT( Make, 2, host::NodeFactory_ABC::Ptr( const host::Path& root, const std::string& name ), Make2 );
    };
};

#endif // MOCKS_H
