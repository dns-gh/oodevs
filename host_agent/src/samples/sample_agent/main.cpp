// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifdef _MSC_VER
#   define _SCL_SECURE_NO_WARNINGS
#endif

#include <runtime/Factory.h>
#include <host/Agent.h>
#include <host/UuidFactory.h>
#include <host/FileSystem.h>
#include <host/PortFactory.h>
#include <host/NodeFactory.h>
#include <host/SessionFactory.h>
#include <web/Controller.h>
#include <web/MongooseServer.h>

int main( int /*argc*/, const char* /*argv*/[] )
{
    runtime::Factory runtime;
    host::UuidFactory uuids;
    host::FileSystem system;
    host::PortFactory ports( 40, 50000, 60000 );
    int host = 15000;
    host::NodeFactory nodes( runtime.GetRuntime(), uuids, system, ports,
        L"C:/Program Files/Java/jdk1.6.0_31/bin/java.exe", L"e:/cloud_hg/node/out/jar/node.jar", L"e:/cloud_hg/node/www", host );
    host::SessionFactory sessions( runtime.GetRuntime(), uuids, system, ports, L"d:/apps/sword_434_data", L"d:/apps/sword_434/applications" );
    host::Agent agent( nodes, sessions );
    web::Controller controller( agent );
    web::MongooseServer server( controller, host );
    getc( stdin );
}
