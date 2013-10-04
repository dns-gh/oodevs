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
#include "ADN_Data_ABC.h"
#include "ADN_GeneralConfig.h"
#include "ADN_MainWindow.h"
#include "ADN_Tr.h"
#include "ADN_Workspace.h"
#include "ENT/ENT_Tr.h"
#pragma warning( push, 1 )
#pragma warning( disable: 4127 4512 4511 )
#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>
#pragma warning( pop )
#include <shlobj.h>

namespace po = boost::program_options;

namespace
{
    tools::Path ReadDataDirectory()
    {
        QSettings settings( "MASA Group", "SWORD" );
        return tools::Path::FromUnicode( settings.readEntry( "/Common/DataDirectory", "" ).toStdWString() );
    }
    tools::Path GetDefaultRoot( const std::string& appName )
    {
        const tools::Path regDir = ReadDataDirectory();
        if( !regDir.IsEmpty() )
            return regDir;
        char myDocuments[ MAX_PATH ];
        SHGetSpecialFolderPath( 0, myDocuments, CSIDL_PERSONAL, 0 );
        return tools::Path( myDocuments ) / tools::Path::FromUTF8( appName );
    }
    bool IsAlreadyWrapped( const std::string& content )
    {
        return content.find( "WARNING" ) != std::string::npos || content.find( "ERROR" ) != std::string::npos || content.find( "INFO" ) != std::string::npos;
    }
    std::string Wrap( const std::string& content, const std::string& prefix )
    {
        std::string result;
        std::stringstream input( content );
        std::string line;
        bool bFirst = true;
        while( std::getline( input, line ) )
        {
            if( !bFirst )
                result += '\n';
            else
                bFirst = false;
            if( !IsAlreadyWrapped( line ) )
                result += prefix;
            result += line;
        }
        return result;
    }
}

//-----------------------------------------------------------------------------
// Name: ADN_App constructor
// Created: JDY 03-06-19
//-----------------------------------------------------------------------------
ADN_App::ADN_App( gui::ApplicationMonitor& monitor, int argc, char** argv )
    : gui::Application_ABC( monitor )
    , config_( new ADN_GeneralConfig( argc, argv, GetDefaultRoot( qApp->translate( "Application", "SWORD" ).toStdString() ) ) )
    , mainWindow_( *config_ )
{
    // License
    CheckLicense( "sword-authoring" );
    if( IsInvalidLicense() )
        return;

    // Application_ABC initialization
    Initialize();

    // Workspace creation
    ADN_Workspace::CreateWorkspace( mainWindow_, *config_ );

    // Make sure that once the last window is closed, the application quits.
    qApp->connect( qApp, SIGNAL( lastWindowClosed() ), SLOT( quit() ) );
}

//-----------------------------------------------------------------------------
// Name: ADN_App destructor
// Created: JDY 03-06-19
//-----------------------------------------------------------------------------
ADN_App::~ADN_App()
{
    ADN_Workspace::DeleteWorkspace();
}

// -----------------------------------------------------------------------------
// Name: ADN_App::CreateTranslators
// Created: ABR 2012-07-13
// -----------------------------------------------------------------------------
void ADN_App::CreateTranslators()
{
    AddTranslator( "qt" );
    AddTranslator( "adaptation_app" );
    AddTranslator( "ENT" );
    AddTranslator( "clients_gui" );
    AddTranslator( "clients_gui_app6" );
    ADN_Tr::InitTranslations();
    ENT_Tr::InitTranslations();
    ADN_Data_ABC::InitQtTranslations();
}

// -----------------------------------------------------------------------------
// Name: ADN_App::Run
// Created: ABR 2012-07-12
// -----------------------------------------------------------------------------
int ADN_App::Run()
{
    const tools::Path& newFile = config_->GetNewFile();
    const tools::Path& inputFile = config_->GetInputFile();
    const tools::Path& outputFile = config_->GetOutputFile();
    try
    {
        if( IsInvalidLicense() )
            return EXIT_FAILURE;

        if( !newFile.IsEmpty() && ( !inputFile.IsEmpty() || !outputFile.IsEmpty() ) )
            throw MASA_EXCEPTION( tools::translate( "ADN_App" , "New base creation and input/output file options cannot be used together." ).toStdString() );
        if( !newFile.IsEmpty() )
        {
            if( ADN_Workspace::GetWorkspace().IsNewBaseReadOnly( newFile ) )
                return EXIT_FAILURE;
            ADN_Workspace::GetWorkspace().New( newFile, false );
            ADN_Workspace::GetWorkspace().SaveAs( newFile );
            return EXIT_SUCCESS;
        }
        if( !inputFile.IsEmpty() )
        {
            if( outputFile.IsEmpty() )
                mainWindow_.showMaximized();
            ADN_Workspace::GetWorkspace().Load( inputFile, outputFile.IsEmpty() );
        }
        if( !outputFile.IsEmpty() )
        {
            if( ADN_Workspace::GetWorkspace().IsNewBaseReadOnly( outputFile ) )
                return EXIT_FAILURE;
            ADN_Workspace::GetWorkspace().SaveAs( outputFile );
            return EXIT_SUCCESS;
        }
    }
    catch( const std::exception& e )
    {
        if( outputFile.IsEmpty() && newFile.IsEmpty() )
            QMessageBox::critical( &mainWindow_, tr( "Error" ), tools::GetExceptionMsg( e ).c_str() );
        else
        {
            std::stringstream ss;
            ss << tools::GetExceptionMsg( e ).c_str() << std::endl;
            MT_LOG_ERROR_MSG( Wrap( ss.str(), "ERROR: " ) );
        }
        return EXIT_FAILURE;
    }
    mainWindow_.showMaximized();
    return qApp->exec();
}
