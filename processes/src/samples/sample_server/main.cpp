// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************
#include <runtime/Factory.h>
#include <web/Controller.h>
#include <web/MongooseServer.h>

int main( int /*argc*/, const char* /*argv*/[] )
{

    process::Factory factory;
    web::Controller controller( factory.GetRuntime() );
    web::MongooseServer server( controller );
    getc( stdin );
}
