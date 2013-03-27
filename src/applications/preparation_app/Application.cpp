// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "Application.h"
#include "Config.h"
#include "FileLoaderObserver.h"
#include "MainWindow.h"
#include "clients_kernel/Controllers.h"
#include "preparation/Model.h"
#include "preparation/StaticModel.h"
#include "ENT/ENT_Tr.h"
#pragma warning( push, 1 )
#pragma warning( disable : 4512 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )

// -----------------------------------------------------------------------------
// Name: Application::Application
// Created: SBO 2006-07-05
// -----------------------------------------------------------------------------
Application::Application( gui::ApplicationMonitor& monitor, int& argc, char** argv )
    : Application_ABC( monitor )
{
    // License
    CheckLicense( "sword-preparation" );
    if( IsInvalidLicense() )
        return;

    // Application_ABC initialization
    Initialize();

    // Data
    observer_.reset( new FileLoaderObserver() );
    config_.reset( new Config( argc, argv, *observer_ ) );
    controllers_.reset( new kernel::Controllers() );
    staticModel_.reset( new StaticModel( *controllers_ ) );
    model_.reset( new Model( *controllers_, *staticModel_ ) );

    // GUI
    mainWindow_ = new MainWindow( *controllers_, *staticModel_, *model_, *config_, GetExpiration() );
    qApp->connect( qApp, SIGNAL( lastWindowClosed() ), SLOT( quit() ) ); // Make sure that once the last window is closed, the application quits.
}

// -----------------------------------------------------------------------------
// Name: Application destructor
// Created: SBO 2006-07-05
// -----------------------------------------------------------------------------
Application::~Application()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Application::CreateTranslators
// Created: ABR 2012-07-12
// -----------------------------------------------------------------------------
void Application::CreateTranslators()
{
    AddTranslator( "qt" );
    AddTranslator( "ENT" );
    AddTranslator( "actions" );
    AddTranslator( "actions_gui" );
    AddTranslator( "clients_kernel" );
    AddTranslator( "clients_gui" );
    AddTranslator( "clients_gui_app6" );
    AddTranslator( "indicators" );
    AddTranslator( "preparation" );
    AddTranslator( "preparation_app" );
    AddTranslator( "reports" );
    AddTranslator( "resources_gradients" );
    ENT_Tr::InitTranslations();
}

// -----------------------------------------------------------------------------
// Name: Application::Run
// Created: ABR 2012-07-11
// -----------------------------------------------------------------------------
int Application::Run()
{
    if( IsInvalidLicense() )
        return EXIT_FAILURE;

    if( config_->HasGenerateScores() || !config_->GetFolderToMigrate().IsEmpty() )
        return EXIT_SUCCESS;

    mainWindow_->show();
    observer_->DisplayErrors();
    return qApp->exec();
}

namespace
{
#define INHERIT( name ) \
    if( o->inherits( name ) ) \
    {\
        className = name;\
        return true;\
    }

    bool GetClassName( std::string& className, QObject* o )
    {
        INHERIT( "QSpinBox" )
        INHERIT( "QLineEdit" )
        INHERIT( "QTextEdit" )
        INHERIT( "QComboBox" )
        INHERIT( "QCheckBox" )
        INHERIT( "QTabWidget" )
        INHERIT( "QTableWidget" )
        INHERIT( "QTableView" )
        INHERIT( "QListWidget" )
        INHERIT( "QListView" )
        INHERIT( "QToolButton" )
        INHERIT( "QPushButton" )
        INHERIT( "QRadioButton" )
        INHERIT( "QdateTimeEdit" )
        INHERIT( "QTimeEdit" )
        INHERIT( "QTreeWidget" )
        INHERIT( "Q3GroupBox" )
        INHERIT( "QGroupBox" )
        return false;
    }

    void CheckNamingHierarchy( QObject* parent, std::map< std::string, std::pair< unsigned int, unsigned int > >& map )
    {
        if( !parent )
            return;

        for( auto it  = parent->children().begin(); it != parent->children().end(); ++it )
        {
            std::string className;
            if( GetClassName( className, *it ) )
                if( !( *it )->objectName().isEmpty() )
                    ++map[ className ].first;
                else
                    ++map[ className ].second;
            CheckNamingHierarchy( *it, map );
        }
    }

    void DisplayList( std::map< std::string, std::pair< unsigned int, unsigned int > > map )
    {
        for( auto it = map.begin(); it != map.end(); ++it )
            std::cout << it->first << " - named : " << it->second.first << " / unnamed : " << it->second.second << std::endl;
    }
}

// -----------------------------------------------------------------------------
// Name: Application::CheckInterfaceComponentNaming
// Created: NPT 2013-03-21
// -----------------------------------------------------------------------------
void Application::CheckInterfaceComponentNaming()
{
    std::map< std::string, std::pair< unsigned int, unsigned int > > map;
    CheckNamingHierarchy( mainWindow_, map );
    DisplayList( map );
}
