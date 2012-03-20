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
#include <host/SessionFactory.h>
#include <web/Controller.h>
#include <web/MongooseServer.h>

int main( int /*argc*/, const char* /*argv*/[] )
{

    runtime::Factory runtime;
    host::UuidFactory uuids;
    host::FileSystem system;
    host::SessionFactory factory( runtime.GetRuntime(), uuids, system,
        L"d:/apps/sword_434_data", L"d:/apps/sword_434/applications" );
    host::Agent agent( factory );
    web::Controller controller( agent );
    web::MongooseServer server( controller );
    getc( stdin );
}
