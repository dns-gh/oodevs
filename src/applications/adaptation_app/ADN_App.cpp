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
#include "ADN_Languages_Data.h"
#include "ADN_MainWindow.h"
#include "ADN_Tr.h"
#include "ADN_Workspace.h"
#include "ADN_WorkspaceElement.h"
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
        return result + '\n';
    }
}

//-----------------------------------------------------------------------------
// Name: ADN_App constructor
// Created: JDY 03-06-19
//-----------------------------------------------------------------------------
ADN_App::ADN_App( gui::ApplicationMonitor& monitor, int argc, char** argv )
    : gui::Application_ABC( monitor )
{
    // License
    CheckLicense( "sword-authoring" );
    if( IsInvalidLicense() )
        return;

    // Application_ABC initialization
    Initialize();

    // ADN initialization
    tools::Language::SetCanUseInvalidCurrent( true );
    config_.reset( new ADN_GeneralConfig( argc, argv, GetDefaultRoot( qApp->translate( "Application", "SWORD" ).toStdString() ) ) );
    LoadCommandLineLanguage( config_->GetLanguages(), config_->GetCommandLineLanguage() );
    mainWindow_.reset( new ADN_MainWindow( *config_ ) );
    ADN_Workspace::CreateWorkspace( *mainWindow_, *config_, *this );

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
    mainWindow_.reset();
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
    const std::string& swapLanguage = config_->GetSwapLanguage();
    const tools::Path& qtNamePath = config_->GetQtNamesPath();
    try
    {
        if( IsInvalidLicense() )
            return EXIT_FAILURE;

        if( !newFile.IsEmpty() && ( !inputFile.IsEmpty() || !outputFile.IsEmpty() ) )
            throw MASA_EXCEPTION( tools::translate( "ADN_App" , "New base creation and input/output file options cannot be used together." ).toStdString() );
        if( !swapLanguage.empty() && ( inputFile.IsEmpty() || outputFile.IsEmpty() ) )
            throw MASA_EXCEPTION( tools::translate( "ADN_App" , "Swap language option needs both the input and the output file options." ).toStdString() );
        if( !qtNamePath.IsEmpty() && inputFile.IsEmpty() )
            throw MASA_EXCEPTION( tools::translate( "ADN_App" , "Debug qt name path option needs the input file options." ).toStdString() );
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
            if( outputFile.IsEmpty() && qtNamePath.IsEmpty() )
                mainWindow_->showMaximized();
            ADN_Workspace::GetWorkspace().Load( inputFile, outputFile.IsEmpty() );
            if( !qtNamePath.IsEmpty() )
            {
                CheckInterfaceComponentNaming( mainWindow_.get(), qtNamePath );
                return EXIT_SUCCESS;
            }
        }
        if( !outputFile.IsEmpty() )
        {
            if( !swapLanguage.empty() )
            {
                ADN_Workspace::GetWorkspace().GetLanguages().GetData().ChangeLanguage( swapLanguage );
                ADN_Workspace::GetWorkspace().GetLanguages().GetData().SwapMaster();
            }
            if( ADN_Workspace::GetWorkspace().IsNewBaseReadOnly( outputFile ) )
                return EXIT_FAILURE;
            ADN_Workspace::GetWorkspace().SaveAs( outputFile );
            return EXIT_SUCCESS;
        }
    }
    catch( const std::exception& e )
    {
        if( outputFile.IsEmpty() && newFile.IsEmpty() )
            QMessageBox::critical( mainWindow_.get(), tools::translate( "ADN_App", "Error" ), tools::GetExceptionMsg( e ).c_str() );
        else
            MT_LOG_FATAL_ERROR_MSG( Wrap( tools::GetStackTraceAndMessage( e ), "ERROR: " ) );
        return EXIT_FAILURE;
    }
    mainWindow_->showMaximized();
    return qApp->exec();
}
