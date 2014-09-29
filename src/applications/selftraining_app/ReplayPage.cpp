// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "ReplayPage.h"
#include "moc_ReplayPage.cpp"

#include "Config.h"
#include "DebugConfigPanel.h"
#include "ExerciseContainer.h"
#include "ExerciseList.h"
#include "MainWindow.h"
#include "ProcessDialogs.h"
#include "ProgressPage.h"
#include "SessionList.h"

#include "frontend/CreateSession.h"
#include "frontend/DebugConfig.h"
#include "frontend/Exercise_ABC.h"
#include "frontend/StartReplay.h"
#include "frontend/StartTimeline.h"
#include "frontend/JoinExercise.h"
#include "frontend/ProcessWrapper.h"

#include "clients_kernel/Tools.h"
#include "clients_kernel/Controllers.h"

#include "tools/Loader_ABC.h"
#include "tools/StdFileWrapper.h"
#include "tools/Version.h"

#include <QtGui/QProgressDialog>
#include <boost/filesystem.hpp>
#include <boost/make_shared.hpp>
#include <boost/thread.hpp>
#include <xeumeuleu/xml.hpp>
#include <windows.h>

namespace bfs = boost::filesystem;

// -----------------------------------------------------------------------------
// Name: ReplayPage constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
ReplayPage::ReplayPage( Application& app, QStackedWidget* pages, Page_ABC& previous, const Config& config, const tools::Loader_ABC& fileLoader, kernel::Controllers& controllers, ExerciseContainer& exercises, const frontend::DebugConfig& debug )
    : ContentPage( pages, previous, eButtonBack | eButtonStart | eButtonExport )
    , config_( config )
    , debug_( debug )
    , fileLoader_( fileLoader )
    , controllers_( controllers )
    , exerciseContainer_( exercises )
    , progressPage_( new ProgressPage( app, pages, *this ) )
{
    //Main Window
    setWindowTitle( "ReplayPage" );
    QWidget* mainBox = new QWidget( this );
    QHBoxLayout* mainBoxLayout = new QHBoxLayout( mainBox );
    mainBoxLayout->setMargin( 10 );
    mainBoxLayout->setSpacing( 10 );

    //exercise list
    exercises_ = new ExerciseList( app, mainBox, config, fileLoader_, controllers, false, true, false );
    connect( exercises_, SIGNAL( Select( const frontend::Exercise_ABC&, const frontend::Profile& ) ), SLOT( OnSelectExercise( const frontend::Exercise_ABC&, const frontend::Profile& ) ) );
    connect( exercises_, SIGNAL( ClearSelection() ), SLOT( ClearSelection() ) );
    mainBoxLayout->addWidget( exercises_ );

    //session List
    sessions_ = new SessionList( mainBox, config, fileLoader_ );
    connect( sessions_, SIGNAL( Select( const tools::Path& ) ), SLOT( OnSelectSession( const tools::Path& ) ) );

    EnableButton( eButtonStart, false );
    EnableButton( eButtonExport, false );
    AddContent( mainBox );
}

// -----------------------------------------------------------------------------
// Name: ReplayPage destructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
ReplayPage::~ReplayPage()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ReplayPage::OnLanguageChanged
// Created: ABR 2011-11-09
// -----------------------------------------------------------------------------
void ReplayPage::OnLanguageChanged()
{
    SetTitle( tools::translate( "ReplayPage", "Replay" ) );
    progressPage_->SetTitle( tools::translate( "ReplayPage", "Starting replay session" ) );
    ContentPage::OnLanguageChanged();
}

// -----------------------------------------------------------------------------
// Name: ReplayPage::Update
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
void ReplayPage::Update()
{
    exercises_->Clear();
    exerciseContainer_.Refresh();
}

// -----------------------------------------------------------------------------
// Name: ReplayPage::StartExercise
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
void ReplayPage::StartExercise()
{
    if( !exercise_ || session_.IsEmpty() || !profile_.IsValid() || ! dialogs::KillRunningProcesses( parentWidget()->parentWidget() ) )
        return;
    const tools::Path exerciseName = exercise_->GetName();
    const auto sessionDir = ConfigureSession( exerciseName, session_ );
    auto process = boost::make_shared< frontend::ProcessWrapper >( *progressPage_ );
    process->Add( boost::make_shared< frontend::StartReplay >( config_, exerciseName, session_, "" ) );
    process->Add( boost::make_shared< frontend::StartTimeline >( config_, exerciseName, session_, debug_, false ) );
    const auto profile = profile_.GetLogin();
    process->Add( boost::make_shared< frontend::JoinExercise >( config_,
            exerciseName, session_, &profile, sessionDir, debug_ ));
    progressPage_->Attach( process );
    frontend::ProcessWrapper::Start( process );
    progressPage_->show();
}

// -----------------------------------------------------------------------------
// Name: TutorialPage::OnStart
// Created: RDS 2008-09-01
// -----------------------------------------------------------------------------
void ReplayPage::OnStart()
{
    StartExercise();
}

// -----------------------------------------------------------------------------
// Name: ReplayPage::OnSelectExercise
// Created: RDS 2008-09-02
// -----------------------------------------------------------------------------
void ReplayPage::OnSelectExercise( const frontend::Exercise_ABC& exercise, const frontend::Profile& profile )
{
    if( exercise_ != &exercise )
    {
        session_ = "";
        sessions_->Update( exercise.GetName() );
    }
    exercise_ = &exercise;
    profile_ = profile;
    const bool enabled = !session_.IsEmpty();
    EnableButton( eButtonStart, enabled && profile_.IsValid() );
    EnableButton( eButtonExport, enabled );
}

// -----------------------------------------------------------------------------
// Name: ReplayPage::ClearSelection
// Created: SBO 2010-10-28
// -----------------------------------------------------------------------------
void ReplayPage::ClearSelection()
{
    exercise_ = 0;
    profile_ = frontend::Profile::Invalid;
    session_ = "";
    sessions_->Update( "" );
    EnableButton( eButtonStart, false );
    EnableButton( eButtonExport, false );
}

// -----------------------------------------------------------------------------
// Name: ReplayPage::OnSelectSession
// Created: SBO 2009-12-13
// -----------------------------------------------------------------------------
void ReplayPage::OnSelectSession( const tools::Path& session )
{
    session_ = session;
    const bool enabled = exercise_ && !session_.IsEmpty();
    EnableButton( eButtonStart, enabled  && profile_.IsValid() );
    EnableButton( eButtonExport, enabled );
}

// -----------------------------------------------------------------------------
// Name: ReplayPage::ConfigureSession
// Created: SBO 2008-02-27
// -----------------------------------------------------------------------------
tools::Path ReplayPage::ConfigureSession( const tools::Path& exercise, const tools::Path& session )
{
    frontend::CreateSession action( config_, exercise, session );
    tools::Path sessionDir = action.GetPath().Parent();
    action.SetDefaultValues(); // reset specific parameters
    action.Commit();
    return sessionDir;
}

void ReplayPage::OnExport()
{
    const auto dir = QFileDialog::getExistingDirectory( this,
        tr( "Export Replay" ) );
    if( dir.isEmpty() )
        return;
    const auto error = ExportReplay( this, fileLoader_, config_, exercise_->GetName(), session_,
        tools::Path::FromUnicode( dir.toStdWString() ) );
    if( !error.isEmpty() )
        QMessageBox::critical( this, tr( "Error" ), tr( "Unable to export replay" ) + "\n" + error );
}

namespace
{
    struct Properties
    {
        tools::Path model;
        tools::Path physical;
        tools::Path terrain;
    };

    Properties GetProperties( const frontend::Config& config,
                              const tools::Loader_ABC& loader,
                              const tools::Path& exercise )
    {
        Properties props;
        auto xis = loader.LoadFile( config.GetExerciseFile( exercise ) );
        *xis >> xml::start( "exercise" )
                >> xml::start( "terrain" )
                    >> xml::attribute( "name", props.terrain )
                >> xml::end
                >> xml::start( "model" )
                    >> xml::attribute( "dataset", props.model )
                    >> xml::attribute( "physical", props.physical )
                >> xml::end;
        return props;
    }

    tools::Path MovePath( bfs::path dst, const bfs::path& prefix, const bfs::path& src )
    {
        auto cursor = src.begin();
        for( auto it = prefix.begin(); it != prefix.end(); ++it, ++cursor )
            if( *it != *cursor )
                throw std::runtime_error( "error: " + prefix.string() + " is not a prefix of " + src.string() );
        for( ; cursor != src.end(); ++cursor )
            dst /= *cursor;
        return tools::Path::FromUnicode( dst.wstring() );
    }

    bool BeginsWith( const bfs::path& path, const bfs::path& prefix )
    {
        for( auto a = prefix.begin(), b = path.begin(); a != prefix.end(); ++a, ++b )
            if( b == path.end() )
                return false;
            else if( *a != *b )
                return false;
        return true;
    }

    tools::Path GetExportDirectory( const tools::Path& exercise, const tools::Path& session )
    {
        std::string name = exercise.ToUTF8() + "-" + session.ToUTF8();
        std::replace( name.begin(), name.end(), '\\', '-');
        std::replace( name.begin(), name.end(), '/', '-');
        return tools::Path::FromUTF8( name );
    }

    #define COUNT_OF( X ) ( sizeof( X ) / sizeof *( X ) )

    static const tools::Path invalidExtensions[] =
    {
        ".dat",
        ".lic",
        ".pdb",
    };

    static const tools::Path invalidFilenames[] =
    {
        "adaptation_app.exe",
        "preparation_app.exe",
        "selftraining_app.exe",
        "simulation_app.exe",
    };
}

QString ReplayPage::ExportReplay( QWidget* parent,
                                  const tools::Loader_ABC& loader,
                                  const Config& config,
                                  const tools::Path& exercise,
                                  const tools::Path& session,
                                  const tools::Path& output )
{
    const auto props = GetProperties( config, loader, exercise );
    QProgressDialog progress( tr( "Exporting replay data" ), QString(), 0, 7, parent, Qt::Dialog |
        Qt::CustomizeWindowHint );
    MainWindow::SetStyle( &progress );
    progress.setWindowModality( Qt::WindowModal );
    progress.show();

    QString error;
    int idx = 0;
    const auto MakeProgress = [&]()
    {
        ++idx;
        QMetaObject::invokeMethod( &progress, "setValue", Q_ARG( int, idx ) );
    };
    boost::thread background( [&]()
    {
        try
        {
            const auto prefix = config.GetRootDir();
            const auto dstRoot = output / GetExportDirectory( exercise, session );
            const auto CopyFrom = [&]( const tools::Path& src, const tools::Path::T_Functor& predicate )
            {
                const auto dst = MovePath( dstRoot.ToBoost(), prefix.ToBoost(), src.ToBoost() );
                src.Copy( dst, tools::Path::FailIfExists, predicate );
                MakeProgress();
            };
            CopyFrom( config.GetPhysicalsDir( props.model, props.physical ), tools::Path::T_Functor() );
            CopyFrom( config.GetTerrainDir( props.terrain ), tools::Path::T_Functor() );
            const auto srcExo = config.GetExerciseDir( exercise );
            const auto exoPrefix = ( srcExo / "sessions" ).ToBoost();
            CopyFrom( srcExo, [&]( const tools::Path& path )
            {
                return !BeginsWith( path.ToBoost(), exoPrefix );
            });
            CopyFrom( config.BuildSessionDir( exercise, session ),  tools::Path::T_Functor() );
            const auto srcRun = tools::Path::FromUnicode( tools::GetModuleFilename() ).Parent();
            srcRun.Copy( dstRoot / "bin", tools::Path::FailIfExists, [&]( const tools::Path& path ) -> bool
            {
                const auto ext = path.Extension();
                for( size_t i = 0; i < COUNT_OF( invalidExtensions ); ++i )
                    if( ext == invalidExtensions[i] )
                        return false;
                const auto name = path.FileName();
                for( size_t i = 0; i < COUNT_OF( invalidFilenames ); ++i )
                    if( name == invalidFilenames[i] )
                        return false;
                return true;
            });
            MakeProgress();
            tools::WriteFile( dstRoot / "replay.cmd",
                "@echo off\n"
                "set CUR=%~dp0\n"
                "set CUR=%CUR:\\=/%\n"
                "\"%CUR%/bin/replay.exe\" \"%CUR%\" \"" + exercise.ToUTF8() + "\" \"" + session.ToUTF8() + "\"\n" );
            MakeProgress();
            tools::WriteFile( dstRoot / "replay.vbs",
                "DIM objShell\n"
                "set objShell=wscript.createObject(\"wscript.shell\")\n"
                "iReturn=objShell.Run(\"replay.cmd\", 0, TRUE)\n" );
            MakeProgress();
        }
        catch( const std::exception& err )
        {
            error = err.what();
        }
    } );
    while( !background.timed_join( boost::posix_time::milliseconds( 100 ) ) )
        qApp->processEvents();
    return error;
}
