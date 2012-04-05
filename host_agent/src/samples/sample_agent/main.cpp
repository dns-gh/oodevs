// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include <runtime/Factory.h>
#include <host/Agent.h>
#include <host/UuidFactory.h>
#include <host/FileSystem.h>
#include <host/PortFactory.h>
#include <host/NodeFactory.h>
#include <host/SessionFactory.h>
#include <web/Controller.h>
#include <web/MongooseServer.h>

#define  CPPLOG_THREADING
#include <cpplog/cpplog.hpp>

namespace
{
    void Start( cpplog::BaseLogger& log )
    {
        runtime::Factory runtime( log );
        host::UuidFactory uuids;
        host::FileSystem system;
        host::PortFactory ports( 40, 50000, 60000 );
        int host = 15000;
        host::NodeFactory nodes( log, runtime.GetRuntime(), uuids, system, ports,
            L"C:/Program Files/Java/jdk1.6.0_31/bin/java.exe", L"e:/cloud_hg/node/out/jar/node.jar", L"e:/cloud_hg/node/www", host );
        host::SessionFactory sessions( log, runtime.GetRuntime(), uuids, system, ports, L"d:/apps/sword_434_data", L"d:/apps/sword_434/applications" );
        host::Agent agent( log, nodes, sessions );
        web::Controller controller( log, agent );
        web::MongooseServer server( controller, host );
        getc( stdin );
    }
}

int main( int /*argc*/, const char* /*argv*/[] )
{
    cpplog::StdErrLogger base;
    std::auto_ptr< cpplog::BackgroundLogger > log( new cpplog::BackgroundLogger( &base ) );
    LOG_INFO( *log ) << "Host Agent - (c) copyright MASA Group 2012";
    Start( *log );
    LOG_INFO( *log ) << "Host Agent - Exit";
    log.release(); // log must be explicitely released to join its thread
}
