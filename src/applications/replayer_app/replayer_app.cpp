// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "App.h"
#include "MT_Tools/MT_ConsoleLogger.h"
#include "MT_Tools/MT_Logger.h"
#include "dispatcher/Config.h"
#include "tools/Codec.h"
#include "tools/Main.h"
#include "tools/WinArguments.h"
#include <tools/Exception.h>
#pragma warning( push, 0 )
#include <google/protobuf/message.h>
#pragma warning( pop )

namespace
{
    int Main( const tools::WinArguments& winArgs )
    {
        GOOGLE_PROTOBUF_VERIFY_VERSION;
        try
        {
            tools::SetCodec();
            App app( !winArgs.HasOption( "--no-log" ) );
            app.Execute();
        }
        catch( const std::exception& e )
        {
            MT_LOG_ERROR_MSG( tools::GetExceptionMsg( e ) );
            return EXIT_FAILURE;
        }
        google::protobuf::ShutdownProtobufLibrary();
        return EXIT_SUCCESS;
    }
}

int main()
{
    MT_ConsoleLogger consoleLogger;
    MT_LOG_REGISTER_LOGGER( consoleLogger );
    return tools::Main(
        tools::WinArguments( GetCommandLineW() ),
        MT_Logger_ABC::eReplayer, true, &Main );
}
