// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "Application.h"
#include "clients_gui/ApplicationMonitor.h"
#include "clients_kernel/Tools.h"
#include "tools/WinArguments.h"
#include "tools/Main.h"
#include "MT_Tools/MT_Logger.h"
#include <boost/filesystem.hpp>

namespace
{
    int Main( const tools::WinArguments& winArgs )
    {
        int argc = winArgs.Argc();
        auto argv = winArgs.Argv();
        gui::ApplicationMonitor monitor( argc, argv );
        try
        {
            Application app( monitor, argc, argv );
            return app.Run();
        }
        catch( const std::exception& e )
        {
            QMessageBox::critical( 0, tools::translate( "Application", "Error" ), tools::GetExceptionMsg( e ).c_str() );
        }
        catch( ... )
        {
            QMessageBox::critical( 0, tools::translate( "Application", "Error" ), tools::translate( "Application", "Unhandled error" ) );
        }
        return EXIT_FAILURE;
    }
}

int main()
{
    const tools::WinArguments winArgs( GetCommandLineW() ) ;
    // Change the current working directory, useful when combined with
    // --install and called by a registry key file type bindings. For deployed
    // applications, the expected cwd is almost always the parent applications
    // directory, but the option can also be used with development versions
    // from shell-impaired operating systems.
    const tools::Path cwd = tools::Path::FromUTF8( winArgs.GetOption( "--cwd" ));
    if( !cwd.IsEmpty() && cwd.Exists() )
        boost::filesystem::current_path( cwd.ToBoost() );
    return tools::Main( winArgs, MT_Logger_ABC::eFrontend, false, &Main );
}
