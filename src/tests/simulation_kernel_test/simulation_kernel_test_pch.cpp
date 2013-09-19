// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"
#pragma warning( push, 0 )
#include <google/protobuf/message.h>
#pragma warning( pop )
#include <string>

tools::TestOptions testOptions;

::boost::unit_test::test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    testOptions = tools::ParseBoostTestOptions( argc, argv );
    return 0;
}

namespace
{
    struct ProtobufConfig
    {
        ProtobufConfig()
        {
            GOOGLE_PROTOBUF_VERIFY_VERSION;
        }
        ~ProtobufConfig()
        {
            google::protobuf::ShutdownProtobufLibrary();
        }
    };
    BOOST_GLOBAL_FIXTURE( ProtobufConfig );
}
