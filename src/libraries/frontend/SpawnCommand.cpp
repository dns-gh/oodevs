// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_pch.h"
#include "SpawnCommand.h"

#include "clients_kernel/tools.h"
#include "tools/GeneralConfig.h"
#include "tools/Path.h"

#pragma warning( push, 0 )
#include <QProcess>
#pragma warning( pop )
#include <windows.h>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/make_shared.hpp>

using namespace frontend;

tools::Path frontend::MakeBinaryName( const tools::Path& prefix )
{
#ifdef _DEBUG
    return prefix + tools::Path::FromUTF8( "_d.exe" );
#else
    return prefix + tools::Path::FromUTF8( ".exe" );
#endif
}

struct SpawnCommand::Private : public boost::noncopyable
{
    Private( const tools::Path& exe )
        : exe( exe )
    {
        // NOTHING
    }

    const tools::Path exe;
    tools::Path working;
    QStringList arguments;
    boost::shared_ptr< QProcess > process;
    boost::shared_ptr< Process_ABC > attached;
};

// -----------------------------------------------------------------------------
// Name: SpawnCommand constructor
// Created: AGE 2007-10-04
// -----------------------------------------------------------------------------
SpawnCommand::SpawnCommand( const tools::GeneralConfig& config,
                            const tools::Path& exe,
                            const std::string& name )
    : config_ ( config )
    , name_   ( name )
    , private_( new Private( exe ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SpawnCommand destructor
// Created: AGE 2007-10-04
// -----------------------------------------------------------------------------
SpawnCommand::~SpawnCommand()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SpawnCommand::AddArgument
// Created: BAX 2013-09-11
// -----------------------------------------------------------------------------
void SpawnCommand::AddArgument( const std::string& arg )
{
    private_->arguments << QString::fromStdString( arg );
}

// -----------------------------------------------------------------------------
// Name: SpawnCommand::AddArgument
// Created: BAX 2013-09-11
// -----------------------------------------------------------------------------
void SpawnCommand::AddArgument( const std::string& key, const std::string& value )
{
    AddArgument( "--" + key );
    AddArgument( value );
}

// -----------------------------------------------------------------------------
// Name: SpawnCommand::AddRootDirArgument
// Created: AGE 2007-10-04
// -----------------------------------------------------------------------------
void SpawnCommand::AddRootArgument()
{
    AddArgument( "root-dir", config_.GetRootDir().ToUTF8() );
}

// -----------------------------------------------------------------------------
// Name: SpawnCommand::AddExerciseArgument
// Created: AGE 2007-10-04
// -----------------------------------------------------------------------------
void SpawnCommand::AddExerciseArgument( const tools::Path& exercise )
{
    AddArgument( "exercise", exercise.ToUTF8() );
}

// -----------------------------------------------------------------------------
// Name: SpawnCommand::AddSessionArgument
// Created: AGE 2008-01-04
// -----------------------------------------------------------------------------
void SpawnCommand::AddSessionArgument( const tools::Path& session )
{
    AddArgument( "session", session.ToUTF8() );
}

// -----------------------------------------------------------------------------
// Name: SpawnCommand::Start
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
void SpawnCommand::Start()
{
    private_->process = boost::make_shared< QProcess >();
    if( !private_->working.IsEmpty() )
        private_->process->setWorkingDirectory( QString::fromStdWString( private_->working.ToUnicode() ) );
    private_->process->start( QString::fromStdWString( private_->exe.ToUnicode() ), private_->arguments );
    const bool done = private_->process->waitForStarted();
    if( done )
        return;
    const int exit = private_->process->exitCode();
    private_->process.reset();
    throw MASA_EXCEPTION( tools::translate( "SpawnCommand", "Could not start process: %1, error: %2" )
            .arg( QString::fromStdWString( private_->exe.ToUnicode() ) + " " + private_->arguments.join( " " ) )
            .arg( exit ).toUtf8().constData() );
}

// -----------------------------------------------------------------------------
// Name: SpawnCommand::Attach
// Created: SBO 2010-11-05
// -----------------------------------------------------------------------------
void SpawnCommand::Attach( const boost::shared_ptr< Process_ABC >& process )
{
    private_->attached = process;
}

// -----------------------------------------------------------------------------
// Name: SpawnCommand::Stop
// Created: RDS 2008-08-19
// -----------------------------------------------------------------------------
void SpawnCommand::Stop()
{
    if( private_->process )
        private_->process->kill();
}

// -----------------------------------------------------------------------------
// Name: SpawnCommand::GetPid
// Created: BAX 2013-01-09
// -----------------------------------------------------------------------------
int SpawnCommand::GetPid() const
{
    if( private_->process )
        if( auto info = private_->process->pid() )
            return info->dwProcessId;
    return -1; // -1 is the only invalid process id under windows
}

// -----------------------------------------------------------------------------
// Name: SpawnCommand::Wait
// Created: BAX 2013-01-09
// -----------------------------------------------------------------------------
bool SpawnCommand::Wait( const boost::posix_time::time_duration& duration )
{
    if( !private_->process )
        return true;
    return private_->process->waitForFinished( boost::numeric_cast< int >( duration.total_milliseconds() ) );
}

// -----------------------------------------------------------------------------
// Name: SpawnCommand::GetPercentage
// Created: RDS 2008-09-10
// -----------------------------------------------------------------------------
unsigned int SpawnCommand::GetPercentage() const
{
    return private_->process && private_->process->state() == QProcess::Running ? 100 : 0;
}

// -----------------------------------------------------------------------------
// Name: SpawnCommand::GetStatus
// Created: SBO 2008-10-14
// -----------------------------------------------------------------------------
QString SpawnCommand::GetStatus() const
{
    auto state = private_->process ? private_->process->state() : QProcess::NotRunning;
    switch( state )
    {
        case QProcess::Starting:   return tools::translate( "SpawnCommand", "Starting..." );
        case QProcess::Running:    return tools::translate( "SpawnCommand", "Started" );
        default:
        case QProcess::NotRunning: return tools::translate( "SpawnCommand", "Stopped" );
    }
}

// -----------------------------------------------------------------------------
// Name: SpawnCommand::GetStartedExercise
// Created: LDC 2008-10-23
// -----------------------------------------------------------------------------
tools::Path SpawnCommand::GetStartedExercise() const
{
    return tools::Path();
}

// -----------------------------------------------------------------------------
// Name: SpawnCommand::GetExercise
// Created: RPD 2011-09-12
// -----------------------------------------------------------------------------
tools::Path SpawnCommand::GetExercise() const
{
    return tools::Path();
}

// -----------------------------------------------------------------------------
// Name: SpawnCommand::GetSession
// Created: RPD 2011-09-12
// -----------------------------------------------------------------------------
tools::Path SpawnCommand::GetSession() const
{
    return tools::Path();
}

// -----------------------------------------------------------------------------
// Name: SpawnCommand::SetWorkingDirectory
// Created: SBO 2009-06-12
// -----------------------------------------------------------------------------
void SpawnCommand::SetWorkingDirectory( const tools::Path& directory )
{
    private_->working = directory;
}

// -----------------------------------------------------------------------------
// Name: SpawnCommand::GetName
// Created: RPD 2011-09-12
// -----------------------------------------------------------------------------
const std::string& SpawnCommand::GetName() const
{
    return name_;
}