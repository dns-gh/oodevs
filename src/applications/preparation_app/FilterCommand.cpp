// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "FilterCommand.h"

#include "clients_kernel/Tools.h"
#include "frontend/SpawnCommand.h"
#include "frontend/ProcessWrapper.h"
#include "tools/ExerciseConfig.h"
#pragma warning( push, 0 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )
#include <boost/filesystem/operations.hpp>
#include <xeumeuleu/xml.hpp>

namespace bfs = boost::filesystem;

namespace
{
    std::string ConvertArgumentVariable( std::string value, const tools::ExerciseConfig& config )
    {
        std::string result = value;
        if( value == "$rootdir$" )
            result = config.GetRootDir();
        else if( value == "$exercice$" )
            result = config.GetExerciseName();
        return "\"" + result + "\"";
    }
}

// -----------------------------------------------------------------------------
// Name: FilterCommand constructor
// Created: ABR 2011-06-17
// -----------------------------------------------------------------------------
FilterCommand::FilterCommand( xml::xistream& xis, const tools::ExerciseConfig& config )
    : Filter()
    , config_        ( config )
    , command_       ( xis.attribute< std::string >( "command" ) )
    , reloadExercise_( xis.attribute< bool >( "reload-exercise", false ) )
    , path_          ( xis.attribute< std::string >( "directory", "." ) )
{
    assert( !command_.empty() );
    ReadDescriptions( xis );
    ReadArguments( xis );
    for( IT_Arguments it = arguments_.begin(); it != arguments_.end(); ++it )
        argumentsLine_ += ( it->second.empty() ) ? " " + it->first : " " + it->first + "=" + ConvertArgumentVariable( it->second, config_ );
    ComputePath();
}

// -----------------------------------------------------------------------------
// Name: FilterCommand destructor
// Created: ABR 2011-06-17
// -----------------------------------------------------------------------------
FilterCommand::~FilterCommand()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FilterCommand::GetName
// Created: ABR 2011-06-17
// -----------------------------------------------------------------------------
const std::string FilterCommand::GetName() const
{
    const std::string result = Filter::GetName();
    return ( result.empty() ) ? command_ : result;
}

// -----------------------------------------------------------------------------
// Name: FilterCommand::IsValid
// Created: ABR 2011-06-22
// -----------------------------------------------------------------------------
bool FilterCommand::IsValid() const
{
    return !command_.empty() && !path_.empty();
}

// -----------------------------------------------------------------------------
// Name: FilterCommand::NeedToReloadExercise
// Created: ABR 2011-06-24
// -----------------------------------------------------------------------------
bool FilterCommand::NeedToReloadExercise() const
{
    return reloadExercise_;
}

// -----------------------------------------------------------------------------
// Name: FilterCommand::ReadArguments
// Created: ABR 2011-06-21
// -----------------------------------------------------------------------------
void FilterCommand::ReadArguments( xml::xistream& xis )
{
    xis >> xml::start( "arguments" )
            >> xml::list( "argument", *this, &FilterCommand::ReadArgument )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: FilterCommand::ReadArgument
// Created: ABR 2011-06-17
// -----------------------------------------------------------------------------
void FilterCommand::ReadArgument( xml::xistream& xis )
{
    assert( xis.has_attribute( "name" ) );
    const std::string name = xis.attribute< std::string >( "name" );
    const std::string value = xis.attribute< std::string >( "value", "" );
    arguments_.push_back( std::pair< std::string, std::string >( name, value ) );
}

// -----------------------------------------------------------------------------
// Name: FilterCommand::ComputePath
// Created: ABR 2011-06-23
// -----------------------------------------------------------------------------
void FilterCommand::ComputePath()
{
    std::string path = getenv( "PATH" );
    std::vector< std::string > valuesVector;
    boost::split( valuesVector, path, boost::algorithm::is_any_of( ";" ) );
    valuesVector.insert( valuesVector.begin(), config_.BuildPhysicalChildFile( "Filters/" ) );
    if( path_.size() > 0 && path_[ 0 ] == '.' ) // $$$$ ABR 2011-08-01: If directory begin with '.', concat to current directory
    {
        TCHAR current[ 512 ];
        int done = GetCurrentDirectory( 512, current );
        if( done < 512 )
            path_ = ( bfs::path( current ) / bfs::path( path_ ) ).string();
    }
    valuesVector.insert( valuesVector.begin(), path_ );
    path_.clear();

    bool founded = false;
    for( std::vector< std::string >::iterator it = valuesVector.begin(); it != valuesVector.end() && !founded; ++it )
    {
        if( it->find( "%" ) != std::string::npos ) // $$$$ ABR 2011-08-01: If an environment variable is given, expand it.
        {
            TCHAR result[ 512 ];
            int done = ExpandEnvironmentStrings( it->c_str(), result, 512 );
            if( done < 512 )
                *it = result;
        }
        if( bfs::exists( *it ) && bfs::is_directory( *it ) )
        {
            bfs::directory_iterator end_itr;
            for( bfs::directory_iterator dir_it( *it ); dir_it != end_itr && !founded; ++dir_it )
                if( !bfs::is_directory( dir_it->path() ) )
                {
                    std::string file = dir_it->path().leaf();
                    size_t pos = file.rfind( ".exe" );
                    if( pos != std::string::npos && pos == file.size() - 4 && file == command_ )
                    {
                        path_ = *it;
                        founded = true;
                    }
                }
        }
    }
    emit statusChanged( IsValid() );
}

// -----------------------------------------------------------------------------
// Name: FilterCommand::CreateParametersWidget
// Created: ABR 2011-06-21
// -----------------------------------------------------------------------------
QWidget* FilterCommand::CreateParametersWidget( QWidget* parent )
{
    Q3GroupBox* parametersWidget = new Q3GroupBox( 1, Qt::Horizontal, tools::translate( "FilterCommand", "Command overview" ), parent, "FilterCommand_ParameterGroupBox" );
    QWidget* widget = new QWidget( parametersWidget, "FilterCommand_BaseWidget" );
    QGridLayout* grid = new QGridLayout( widget, 3, 2, 0, 5, "FilterCommand_GridLayout" );
    grid->setMargin( 5 );
    grid->setColStretch( 1, 1 );

    grid->addWidget( new QLabel( tools::translate( "FilterCommand", "Command:" ), widget, "FilterCommand_CommandTitle" ), 0, 0 );
    grid->addWidget( new QLabel( ( command_ + argumentsLine_ ).c_str(), widget, "FilterCommand_CommandLabel" ), 0, 1 );

    grid->addWidget( new QLabel( tools::translate( "FilterCommand", "Path:" ), widget, "FilterCommand_PathTitle" ), 1, 0 );
    grid->addWidget( ( path_.empty() )
        ? new QLabel( "<font color=\"#FF0000\">" + tools::translate( "FilterCommand", "File not found, move your binary to the filters directory, or update the $PATH environment variable." ) + "</font>", widget, "FilterCommand_PathLabel" )
        : new QLabel( path_.c_str(), widget, "FilterCommand_PathLabel" ), 1, 1 );

    grid->addWidget( new QLabel( tools::translate( "FilterCommand", "Reload exercise:" ), widget, "FilterCommand_ReloadTitle" ), 2, 0 );
    QCheckBox* checkBox = new QCheckBox( widget, "FilterCommand_ReloadCheckBox" );
    checkBox->setChecked( reloadExercise_ );
    checkBox->setEnabled( false );
    grid->addWidget( checkBox, 2, 1 );

    return parametersWidget;
}

// -----------------------------------------------------------------------------
// Name: FilterCommand::Execute
// Created: ABR 2011-06-17
// -----------------------------------------------------------------------------
void FilterCommand::Execute()
{
    assert( !path_.empty() );
    boost::shared_ptr< frontend::SpawnCommand > command(
        new frontend::SpawnCommand( config_, ( bfs::path( bfs::path( path_, bfs::native ) / bfs::path( command_, bfs::native ) ).string() + argumentsLine_ ).c_str(), true ) );
    command->SetWorkingDirectory( path_.c_str() );
    boost::shared_ptr< frontend::ProcessWrapper > process(
        new frontend::ProcessWrapper( *this, command ) );
    process->StartAndBlockMainThread();
}

// -----------------------------------------------------------------------------
// Name: FilterCommand::NotifyStopped
// Created: ABR 2011-06-22
// -----------------------------------------------------------------------------
void FilterCommand::NotifyStopped()
{
     // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FilterCommand::NotifyError
// Created: ABR 2011-06-22
// -----------------------------------------------------------------------------
void FilterCommand::NotifyError( const std::string& error, std::string /*commanderEndpoint*/  )
{
    reloadExercise_ = false;
    QMessageBox::critical( QApplication::activeModalWidget(), tools::translate( "FilterDialog", "Error on filter execution" ), error.c_str() );
}
