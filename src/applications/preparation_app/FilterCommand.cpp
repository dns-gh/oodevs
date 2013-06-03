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
#include "moc_FilterCommand.cpp"
#include "ModelConsistencyDialog.h"
#include "clients_kernel/Tools.h"
#include "clients_gui/ConsistencyDialog_ABC.h"
#include "frontend/SpawnCommand.h"
#include "frontend/ProcessWrapper.h"
#include "preparation/Model.h"
#include "preparation/Exercise.h"
#include "tools/ExerciseConfig.h"
#pragma warning( push, 0 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )
#include <boost/filesystem/operations.hpp>
#include <boost/make_shared.hpp>
#include <boost/assign.hpp>
#include <xeumeuleu/xml.hpp>

namespace bfs = boost::filesystem;

// -----------------------------------------------------------------------------
// Name: FilterCommand constructor
// Created: ABR 2011-06-17
// -----------------------------------------------------------------------------
FilterCommand::FilterCommand( xml::xistream& xis, const tools::ExerciseConfig& config, Model& model, gui::ConsistencyDialog_ABC& consistency )
    : Filter( xis )
    , config_        ( config )
    , model_         ( model )
    , consistency_   ( consistency )
    , command_       ( xis.attribute< std::string >( "command" ) )
    , reloadExercise_( xis.attribute< bool >( "reload-exercise", false ) )
    , minimalDisplay_( xis.attribute< bool >( "minimal-display", false ) )
    , nonBlocking_   ( xis.attribute< bool >( "non-blocking", false) )
    , path_          ( xis.attribute< std::string >( "directory", "." ) )
    , commandLabel_  ( 0 )
{
    assert( !command_.empty() );
    ReadArguments( xis );
    ComputeArgument();
    ComputePath();
    connect( this, SIGNAL( Finished() ), SLOT( OnFinished() ) );
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
    return result.empty() ? command_ : result;
}

// -----------------------------------------------------------------------------
// Name: FilterCommand::IsValid
// Created: ABR 2011-06-22
// -----------------------------------------------------------------------------
bool FilterCommand::IsValid() const
{
    for( auto it = arguments_.begin(); it != arguments_.end(); ++it )
        if( ! (*it)->IsValid() )
            return false;
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
// Name: FilterCommand::ConvertArgumentVariable
// Created: ABR 2011-09-29
// -----------------------------------------------------------------------------
std::string FilterCommand::ConvertArgumentVariable( const std::string& value ) const
{
     if( value.empty() )
        return value;
    std::string result = value;
    if( value == "$rootdir$" )
        result = config_.GetRootDir();
    else if( value == "$exercise$" )
        result = model_.IsLoaded() && !model_.exercise_.GetName().isEmpty() ? model_.exercise_.GetName().toAscii().constData() : config_.GetExerciseName();
    else if( value == "$exercise_dir$" )
        result = config_.GetExerciseDir( config_.GetExerciseName() );
    else if( value == "$orbat_file$" )
        result = config_.GetOrbatFile();
    else if( value == "$language$" )
        result = description_.GetCurrentLanguage();
    return "\"" + result + "\"";
}

// -----------------------------------------------------------------------------
// Name: FilterCommand::ReadArguments
// Created: ABR 2011-06-21
// -----------------------------------------------------------------------------
void FilterCommand::ReadArguments( xml::xistream& xis )
{
    xis >> xml::start( "arguments" )
            >> xml::list( "argument", *this, &FilterCommand::ReadArgument )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: FilterCommand::ReadArgument
// Created: ABR 2011-06-17
// -----------------------------------------------------------------------------
void FilterCommand::ReadArgument( xml::xistream& xis )
{
    const std::string value = xis.attribute< std::string >( "value", "" );
    arguments_.push_back(
        boost::make_shared< FilterArgument >(
            boost::ref( xis ), config_, ConvertArgumentVariable( value ) ) );
    connect( arguments_.back().get(), SIGNAL( ValueChanged( const QString& ) ), SLOT( OnValueChanged() ) );
    if( value == "$log_file$" )
        connect( arguments_.back().get(), SIGNAL( ValueChanged( const QString& ) ), SLOT( OnLogFileChanged( const QString& ) ) );
}

// -----------------------------------------------------------------------------
// Name: FilterCommand::ComputeArgument
// Created: ABR 2011-09-28
// -----------------------------------------------------------------------------
void FilterCommand::ComputeArgument()
{
    argumentsLine_.clear();
    for( auto it = arguments_.begin(); it != arguments_.end(); ++it )
        argumentsLine_ += (*it)->ToCommandLine();
    if( commandLabel_ )
        commandLabel_->setText( ( command_ + argumentsLine_ ).c_str() );
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
        try
        {
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
        catch ( std::exception& )
        {
            // NOTHING, here to prevent bfs::exists to throw an exception when tested path is on an unknown hard drive
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

    int row = ( !minimalDisplay_ ) ? 3 : ( path_.empty() ) ? 2 : 1;
    int rows = 0;
    for( auto it = arguments_.begin(); it != arguments_.end(); ++it )
        if( (*it)->IsInputArgument() )
            ++rows;
    QGridLayout* grid = new QGridLayout( widget, row + rows, 2, 0, 5, "FilterCommand_GridLayout" );
    grid->setColStretch( 1, 1 );

    // Command
    if( !minimalDisplay_ )
    {
        grid->addWidget( new QLabel( tools::translate( "FilterCommand", "Command:" ), widget, "FilterCommand_CommandTitle" ), row, 0 );
        commandLabel_ = new QLabel( ( command_ + argumentsLine_ ).c_str(), widget, "FilterCommand_CommandLabel" );
        commandLabel_->setWordWrap( true );
        grid->addWidget( commandLabel_, row++, 1 );
    }
    // Path
    if( !minimalDisplay_ || ( minimalDisplay_ && path_.empty() ) )
    {
        grid->addWidget( new QLabel( tools::translate( "FilterCommand", "Path:" ), widget, "FilterCommand_PathTitle" ), row, 0 );
        QLabel* pathLabel = ( path_.empty() )
            ? new QLabel( "<font color=\"#FF0000\">" + tools::translate( "FilterCommand", "File not found, move your binary to the filters directory, or update the $PATH environment variable." ) + "</font>", widget, "FilterCommand_PathLabel" )
            : new QLabel( path_.c_str(), widget, "FilterCommand_PathLabel" );
        pathLabel->setWordWrap( true );
        grid->addWidget( pathLabel, row++, 1 );
    }
    // Reload-Exercise
    {
        grid->addWidget( new QLabel( tools::translate( "FilterCommand", "Reload exercise:" ), widget, "FilterCommand_ReloadTitle" ), row, 0 );
        QCheckBox* checkBox = new QCheckBox( widget, "FilterCommand_ReloadCheckBox" );
        checkBox->setChecked( reloadExercise_ );
        checkBox->setEnabled( false );
        grid->addWidget( checkBox, row++, 1 );
    }
    // Non blocking
    {
        grid->addWidget( new QLabel( tools::translate( "FilterCommand", "Is blocking:" ), widget, "FilterCommand_ReloadTitle" ), row, 0 );
        QCheckBox* checkBox = new QCheckBox( widget, "FilterCommand_ReloadCheckBox" );
        checkBox->setChecked( !nonBlocking_ );
        checkBox->setEnabled( false );
        grid->addWidget( checkBox, row++, 1 );
    }
    // Input arguments
    for( auto it = arguments_.begin(); it != arguments_.end(); ++it, ++row )
        (*it)->AddWidget( widget, grid, row );
    return parametersWidget;
}

// -----------------------------------------------------------------------------
// Name: FilterCommand::Execute
// Created: ABR 2011-06-17
// -----------------------------------------------------------------------------
void FilterCommand::Execute()
{
    // $$$$ ABR 2011-10-18: Hack, should not be hard coded, add an attribute to Filters.xml instead (when the ICD will no longer be frozen)
    {
        std::string lowerCommand = command_;
        std::transform( lowerCommand.begin(), lowerCommand.end(), lowerCommand.begin(), tolower); 
        if( lowerCommand.find( "melmil.exe" ) != std::string::npos && reloadExercise_ )
            emit ForceSaveAndAddActionPlanning( "melmil.xml" );
    }
    assert( !path_.empty() );
    boost::shared_ptr< frontend::SpawnCommand > command(
        new frontend::SpawnCommand( config_, ( bfs::path( bfs::path( path_, bfs::native ) / bfs::path( command_, bfs::native ) ).string() + argumentsLine_ ).c_str(), true ) );
    command->SetWorkingDirectory( path_.c_str() );
    boost::shared_ptr< frontend::ProcessWrapper > process(
        new frontend::ProcessWrapper( *this, command ) );
    if( nonBlocking_ )
        process->Start();
    else
        process->StartAndBlockMainThread();
}

// -----------------------------------------------------------------------------
// Name: FilterCommand::NotifyStopped
// Created: ABR 2011-06-22
// -----------------------------------------------------------------------------
void FilterCommand::NotifyStopped()
{
    emit Finished();
}

// -----------------------------------------------------------------------------
// Name: FilterCommand::OnFinished
// Created: MCO 2013-04-30
// -----------------------------------------------------------------------------
void FilterCommand::OnFinished()
{
    if( logFile_.empty() )
        return;
    model_.ClearExternalErrors();
    xml::xifstream xis( logFile_ );
    xis >> xml::start( "errors" )
            >> xml::list( "error", *this, &FilterCommand::ReadError );
    if( xis.has_child( "error" ) )
        consistency_.Display();
}

xml::xistream& operator>>( xml::xistream& xis, E_ConsistencyCheck& type )
{
    static const std::map< std::string, E_ConsistencyCheck > types =
        boost::assign::map_list_of( "uniqueness", eUniquenessMask )
                                  ( "logistic", eLogisticMask )
                                  ( "profile", eProfileMask )
                                  ( "ghost", eGhostMask )
                                  ( "headquarters", eCommandPostMask )
                                  ( "other", eOthersMask );
    auto it = types.find( xis.value< std::string >() );
    if( it == types.end() )
        throw xml::exception( xis.context() + "invalid error type" );
    type = it->second;
    return xis;
}

// -----------------------------------------------------------------------------
// Name: FilterCommand::ReadError
// Created: MCO 2013-04-30
// -----------------------------------------------------------------------------
void FilterCommand::ReadError( xml::xistream& xis )
{
    E_ConsistencyCheck type;
    std::string description, criticity;
    unsigned int unitId = 0;
    xis >> xml::attribute( "type", type )
        >> xml::attribute( "description", description )
        >> xml::attribute( "criticity", criticity )
        >> xml::optional >> xml::attribute( "unit-id", unitId );
    model_.AppendExternalError( type, description, criticity == "error", model_.FindEntity( unitId ) );
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

// -----------------------------------------------------------------------------
// Name: FilterCommand::OnValueChanged
// Created: ABR 2011-09-28
// -----------------------------------------------------------------------------
void FilterCommand::OnValueChanged()
{
    ComputeArgument();
}

// -----------------------------------------------------------------------------
// Name: FilterCommand::OnLogFileChanged
// Created: MCO 2013-04-30
// -----------------------------------------------------------------------------
void FilterCommand::OnLogFileChanged( const QString& text )
{
    logFile_ = text.toAscii().constData();
}

// -----------------------------------------------------------------------------
// Name: FilterCommand::Update
// Created: ABR 2012-05-30
// -----------------------------------------------------------------------------
void FilterCommand::Update()
{
    for( auto it = arguments_.begin(); it != arguments_.end(); ++it )
        (*it)->Update();
    ComputeArgument();
}
