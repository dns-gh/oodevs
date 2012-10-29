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

#include "ADN_MainWindow.h"
#include "ADN_Tr.h"
#include "ENT/ENT_Tr.h"
#include "ADN_Exception_ABC.h"
#include "ADN_Config.h"
#pragma warning( push, 1 )
#pragma warning( disable: 4127 4512 4511 )
#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>
#pragma warning( pop )

namespace po = boost::program_options;

namespace
{
    const char mainWindowProperty[] = "ADN_App_MainWindow";
}

//-----------------------------------------------------------------------------
// Name: ADN_App constructor
// Created: JDY 03-06-19
//-----------------------------------------------------------------------------
ADN_App::ADN_App( int argc, char** argv )
    : gui::Application_ABC()
{
    // License
    CheckLicense( "sword-authoring" );
    if( IsInvalidLicense() )
        return;

    // Application_ABC initialization
    Initialize();

    // Data
    config_.reset( new ADN_Config() );

    // GUI
    mainWindow_ = new ADN_MainWindow( *config_, argc , argv );
    qApp->connect( qApp, SIGNAL( lastWindowClosed() ), SLOT( quit() ) ); // Make sure that once the last window is closed, the application quits.
    qApp->setProperty( mainWindowProperty, QVariant::fromValue< QWidget* >( mainWindow_ ) );

    // Command line options
    po::options_description desc( "Allowed options" );
    desc.add_options()
        ( "input,i" , po::value< std::string >( &inputFile_  )->default_value( "" ), "specify root input file (physical.xml)" )
        ( "output,o", po::value< std::string >( &outputFile_ )->default_value( "" ), "specify output file (physical.xml) (open/save-mode: input must be specified)" )
        ( "silent", "silent mode" )
        ( "nosymbols,n", "turn off unit symbols view" )
        ( "noreadonly", "disable read-only protection" )
        ;
    po::variables_map vm;
    po::store( po::parse_command_line( argc, argv, desc ), vm );
    po::notify( vm );

    // Initialize
    ADN_Workspace::GetWorkspace().SetOptions( vm.count( "nosymbols" ) == 0, vm.count( "noreadonly" ) != 0 );
    mainWindow_->Build();
}

//-----------------------------------------------------------------------------
// Name: ADN_App destructor
// Created: JDY 03-06-19
//-----------------------------------------------------------------------------
ADN_App::~ADN_App()
{
    // NOTHING
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
}

namespace
{
    //$$$$ C DEGUEU !
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

// -----------------------------------------------------------------------------
// Name: ADN_App::Run
// Created: ABR 2012-07-12
// -----------------------------------------------------------------------------
int ADN_App::Run()
{
    try
    {
        if( IsInvalidLicense() )
            return EXIT_FAILURE;

        if( !inputFile_.empty() )
            mainWindow_->OpenProject( inputFile_, true );
        if( !outputFile_.empty() )
        {
            mainWindow_->SaveProjectAs( outputFile_ );
            return EXIT_SUCCESS;
        }
    }
    catch( ADN_Exception_ABC& e )
    {
        if( outputFile_.empty() )
            QMessageBox::critical( mainWindow_, e.GetExceptionTitle().c_str(), e.GetExceptionMessage().c_str() );
        else
        {
            std::stringstream ss;
            ss << e.GetExceptionTitle().c_str() << std::endl << e.GetExceptionMessage().c_str() << std::endl;
            MT_LOG_ERROR_MSG( Wrap( ss.str(), "ERROR: " ) );
        }
        return EXIT_FAILURE;
    }
    mainWindow_->showMaximized();
    return qApp->exec();
}

// -----------------------------------------------------------------------------
// Name: ADN_App::GetMainWindow
// Created: ABR 2012-10-28
// -----------------------------------------------------------------------------
QMainWindow* ADN_App::GetMainWindow()
{
    return static_cast< QMainWindow* >( qApp->property( mainWindowProperty ).value< QWidget* >() );
}