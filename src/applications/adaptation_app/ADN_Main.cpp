// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_App.h"
#include "ADN_Exception_ABC.h"
#include "MT_Tools/MT_ConsoleLogger.h"
#include "MT_Tools/MT_Version.h"
#include "tools/Version.h"

static const std::string szADN_Version   = "ADN - " + std::string( tools::AppProjectVersion() ) + " - " MT_COMPILE_TYPE " - " __TIMESTAMP__;

namespace
{
    int RunAdn( int argc, char** argv )
    {
        try
        {
            ADN_App app( argc, argv );
            return app.Run();
        }
        catch( ADN_Exception_ABC& e )
        {
            QMessageBox::critical( 0, e.GetExceptionTitle().c_str(), e.GetExceptionMessage().c_str() );
        }
        catch( std::exception& exception )
        {
            MT_LOG_ERROR_MSG( exception.what() );
            QMessageBox::critical( 0, "Critical", exception.what() );
        }
        catch( ... )
        {
            QMessageBox::critical( 0, "Critical", "Unknown error !" );
        }
        return EXIT_FAILURE;
    }
}

int main( int argc, char** argv )
{
    QApplication qapp( argc, argv );
    SetConsoleTitle( szADN_Version.c_str() );
    MT_ConsoleLogger        consoleLogger;
    MT_LOG_REGISTER_LOGGER( consoleLogger );
    const int nResultCode = RunAdn( argc, argv );
    MT_LOG_UNREGISTER_LOGGER( consoleLogger );
    return nResultCode;
}
