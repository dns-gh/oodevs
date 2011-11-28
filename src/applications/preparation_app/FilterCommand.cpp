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
#include "FilterInputArgument.h"
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
    std::string ReadLang()
    {
        QSettings settings;
        settings.setPath( "MASA Group", tools::translate( "Application", "SWORD" ) );
        return settings.readEntry( "/Common/Language", QTextCodec::locale() ).ascii();
    }

    bool IsInputArgument( const std::string& argument )
    {
        return argument == "$input_file$" || argument == "$input_dir$" || argument == "$input$";
    }
}

// -----------------------------------------------------------------------------
// Name: FilterCommand constructor
// Created: ABR 2011-06-17
// -----------------------------------------------------------------------------
FilterCommand::FilterCommand( xml::xistream& xis, const tools::ExerciseConfig& config )
    : Filter( xis )
    , config_        ( config )
    , command_       ( xis.attribute< std::string >( "command" ) )
    , reloadExercise_( xis.attribute< bool >( "reload-exercise", false ) )
    , minimalDisplay_( xis.attribute< bool >( "minimal-display", false ) )
    , path_          ( xis.attribute< std::string >( "directory", "." ) )
    , commandLabel_  ( 0 )
{
    assert( !command_.empty() );
    ReadArguments( xis );
    ComputeArgument();
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
// Name: FilterCommand::ConvertArgumentVariable
// Created: ABR 2011-09-29
// -----------------------------------------------------------------------------
std::string FilterCommand::ConvertArgumentVariable( const std::string& value ) const
{
    std::string result = value;
    if( value == "$rootdir$" )
        result = config_.GetRootDir();
    else if( value == "$exercise$" )
        result = config_.GetExerciseName();
    else if( value == "$exercise_dir$" )
        result = config_.GetExerciseDir( config_.GetExerciseName() );
    else if( value == "$orbat_file$" )
        result = config_.GetOrbatFile();
    else if( value == "$language$" )
        result = description_.GetCurrentLanguage();
    else if( value == "$input$" || value == "$input_file$" || value == "$input_dir$" || value.empty() ) // $$$$ ABR 2011-09-28: Cf FilterInputArgument
        return value;
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
    arguments_.push_back( std::pair< std::string, std::string >( name, ConvertArgumentVariable( value ) ) );
    if( IsInputArgument( value ) )
    {
        kernel::XmlDescription description( xis, ReadLang() );
        FilterInputArgument* inputArgument = new FilterInputArgument( value, description, config_.GetExerciseDir( config_.GetExerciseName() ) );
        inputArguments_[ arguments_.size() - 1 ] = inputArgument;
        connect( inputArgument, SIGNAL( ValueChanged() ), this, SLOT( OnValueChanged() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: FilterCommand::ComputeArgument
// Created: ABR 2011-09-28
// -----------------------------------------------------------------------------
void FilterCommand::ComputeArgument()
{
    argumentsLine_.clear();
    for( IT_Arguments it = arguments_.begin(); it != arguments_.end(); ++it )
    {
        if( IsInputArgument(  it->second ) )
            argumentsLine_ += " " + it->first + "=\"\"";
        else
            argumentsLine_ += ( it->second.empty() ) ? " " + it->first : " " + it->first + "=" + it->second;
    }
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
    QGridLayout* grid = new QGridLayout( widget, ( inputArguments_.empty() ) ? row : row + static_cast< int >( inputArguments_.size() ), 2, 0, 5, "FilterCommand_GridLayout" );
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
    grid->addWidget( new QLabel( tools::translate( "FilterCommand", "Reload exercise:" ), widget, "FilterCommand_ReloadTitle" ), row, 0 );
    QCheckBox* checkBox = new QCheckBox( widget, "FilterCommand_ReloadCheckBox" );
    checkBox->setChecked( reloadExercise_ );
    checkBox->setEnabled( false );
    grid->addWidget( checkBox, row++, 1 );
    // Input arguments
    for( CIT_InputArguments it = inputArguments_.begin(); it != inputArguments_.end(); ++it, ++row )
    {
        grid->addWidget( new QLabel( tools::translate( "FilterCommand", "Argument '%1':" ).arg( arguments_[ it->first ].first.c_str() ), widget, "FilterCommand_CommandTitle" ), row, 0 );
        grid->addWidget( it->second->CreateWidget( widget ), row, 1 );
    }
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

// -----------------------------------------------------------------------------
// Name: FilterCommand::OnValueChanged
// Created: ABR 2011-09-28
// -----------------------------------------------------------------------------
void FilterCommand::OnValueChanged()
{
    for( CIT_InputArguments it = inputArguments_.begin(); it != inputArguments_.end(); ++it )
        arguments_[ it->first ].second = inputArguments_[ it->first ]->GetText().ascii();
    ComputeArgument();
}