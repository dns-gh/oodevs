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
#include "clients_gui/RichCheckBox.h"
#include "clients_gui/RichGroupBox.h"
#include "frontend/SpawnCommand.h"
#include "frontend/ProcessWrapper.h"
#include "tools/ExerciseConfig.h"
#include "tools/EnvHelpers.h"
#include "tools/Language.h"
#include <boost/bind.hpp>
#include <boost/make_shared.hpp>
#include <xeumeuleu/xml.hpp>

namespace
{
    bool IsInputArgument( const std::string& argument )
    {
        return argument == "$input_file$" || argument == "$input_dir$" || argument == "$input$" || argument == "$input_team_list$";
    }
}

// -----------------------------------------------------------------------------
// Name: FilterCommand constructor
// Created: ABR 2011-06-17
// -----------------------------------------------------------------------------
FilterCommand::FilterCommand( xml::xistream& xis, const tools::ExerciseConfig& config )
    : Filter( xis )
    , config_        ( config )
    , command_       ( xis.attribute< tools::Path >( "command" ) )
    , reloadExercise_( xis.attribute< bool >( "reload-exercise", false ) )
    , minimalDisplay_( xis.attribute< bool >( "minimal-display", false ) )
    , nonBlocking_    ( xis.attribute< bool >( "non-blocking", false) )
    , path_          ( xis.attribute< tools::Path >( "directory", "." ) )
    , commandLabel_  ( 0 )
{
    assert( !command_.IsEmpty() );
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
    return ( result.empty() ) ? command_.ToUTF8() : result;
}

// -----------------------------------------------------------------------------
// Name: FilterCommand::IsValid
// Created: ABR 2011-06-22
// -----------------------------------------------------------------------------
bool FilterCommand::IsValid() const
{
    bool valid = true;
    for( auto it = inputArguments_.begin(); it != inputArguments_.end(); ++it )
        valid = valid && it->second->IsValid();
    return !command_.IsEmpty() && !path_.IsEmpty() && valid;
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
        result = config_.GetRootDir().ToUTF8();
    else if( value == "$exercise$" )
        result = config_.GetExerciseName().ToUTF8();
    else if( value == "$exercise_dir$" )
        result = config_.GetExerciseDir( config_.GetExerciseName() ).ToUTF8();
    else if( value == "$orbat_file$" )
        result = config_.GetOrbatFile().ToUTF8();
    else if( value == "$language$" )
        result = description_.GetCurrentLanguage();
    else if( value == "$input$" || value == "$input_file$" || value == "$input_dir$" || value == "$input_team_list$" || value.empty() ) // $$$$ ABR 2011-09-28: Cf FilterInputArgument
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
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: FilterCommand::ReadArgument
// Created: ABR 2011-06-17
// -----------------------------------------------------------------------------
void FilterCommand::ReadArgument( xml::xistream& xis )
{
    assert( xis.has_attribute( "name" ) );
    FilterArgument argument;
    argument.name_ = xis.attribute< std::string >( "name" );
    argument.displayName_ = xis.attribute< std::string >( "display-name", "" );
    argument.value_ = xis.attribute< std::string >( "value", "" );
    argument.value_ = ConvertArgumentVariable( argument.value_ );
    if( ::IsInputArgument( argument.value_ ) )
    {
        kernel::XmlDescription description( xis, tools::Language::Current() );
        FilterInputArgument* inputArgument = new FilterInputArgument( config_, argument.value_, description, config_.GetExerciseDir( config_.GetExerciseName() ) );
        inputArguments_[ arguments_.size() ] = inputArgument;
        connect( inputArgument, SIGNAL( ValueChanged() ), this, SLOT( OnValueChanged() ) );
        argument.value_ = "";
    }
    arguments_.push_back( argument );
}

// -----------------------------------------------------------------------------
// Name: FilterCommand::IsInputArgument
// Created: ABR 2012-05-29
// -----------------------------------------------------------------------------
bool FilterCommand::IsInputArgument( size_t index ) const
{
    for( auto it = inputArguments_.begin(); it != inputArguments_.end(); ++it )
        if( it->first == index )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: FilterCommand::ComputeArgument
// Created: ABR 2011-09-28
// -----------------------------------------------------------------------------
void FilterCommand::ComputeArgument()
{
    argumentsLine_.clear();
    for( size_t i = 0; i < arguments_.size(); ++i )
    {
        if( IsInputArgument( i ) )
        {
            if( !arguments_[ i ].value_.empty() )
                argumentsLine_ += " " + arguments_[ i ].name_ + "=\"" + arguments_[ i ].value_ + "\"";
        }
        else
            argumentsLine_ += ( arguments_[ i ].value_.empty() ) ? " " + arguments_[ i ].name_ : " " + arguments_[ i ].name_+ "=" + arguments_[ i ].value_;
    }
    if( commandLabel_ )
        commandLabel_->setText( ( command_.ToUTF8() + argumentsLine_ ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: FilterCommand::SearchCommand
// Created: ABR 2013-03-07
// -----------------------------------------------------------------------------
bool FilterCommand::SearchCommand( const tools::Path& path ) const
{
    return !path.IsDirectory() && path.HasExtension() && path.Extension() == ".exe" && path.FileName() == command_;
}

// -----------------------------------------------------------------------------
// Name: FilterCommand::ComputePath
// Created: ABR 2011-06-23
// -----------------------------------------------------------------------------
void FilterCommand::ComputePath()
{
    auto dirs = tools::ExpandEnvPath( true );
    dirs.insert( dirs.begin(), config_.BuildPhysicalChildFile( "Filters/" ) );
    if( path_.IsRelative() )
        path_ = path_.Absolute();
    dirs.insert( dirs.begin(), path_ );
    path_.Clear();

    for( auto it = dirs.begin(); it != dirs.end(); ++it )
    {
        if( it->Apply( boost::bind( &FilterCommand::SearchCommand, this, _1 ), false ) )
        {
            path_ = *it;
            break;
        }
    }
    emit statusChanged( IsValid() );
}

// -----------------------------------------------------------------------------
// Name: FilterCommand::CreateParametersWidget
// Created: ABR 2011-06-21
// -----------------------------------------------------------------------------
QWidget* FilterCommand::CreateParametersWidget( const QString& objectName, QWidget* parent )
{
    gui::SubObjectName subObject( objectName );
    QWidget* widget = new QWidget();

    gui::RichGroupBox* parametersWidget = new gui::RichGroupBox( "ParameterGroupBox", tools::translate( "FilterCommand", "Command overview" ), parent );
    parametersWidget->setLayout( new QHBoxLayout() );
    parametersWidget->layout()->addWidget( widget );

    int row = ( !minimalDisplay_ ) ? 3 : ( path_.IsEmpty() ) ? 2 : 1;
    QGridLayout* grid = new QGridLayout( widget, ( inputArguments_.empty() ) ? row : row + static_cast< int >( inputArguments_.size() ), 2, 0, 5, "FilterCommand_GridLayout" );
    grid->setColStretch( 1, 1 );

    // Command
    if( !minimalDisplay_ )
    {
        grid->addWidget( new QLabel( tools::translate( "FilterCommand", "Command:" ), widget, "FilterCommand_CommandTitle" ), row, 0 );
        commandLabel_ = new QLabel( ( command_.ToUTF8() + argumentsLine_ ).c_str(), widget, "FilterCommand_CommandLabel" );
        commandLabel_->setWordWrap( true );
        grid->addWidget( commandLabel_, row++, 1 );
    }
    // Path
    if( !minimalDisplay_ || ( minimalDisplay_ && path_.IsEmpty() ) )
    {
        grid->addWidget( new QLabel( tools::translate( "FilterCommand", "Path:" ), widget, "FilterCommand_PathTitle" ), row, 0 );
        QLabel* pathLabel = ( path_.IsEmpty() )
            ? new QLabel( "<font color=\"#FF0000\">" + tools::translate( "FilterCommand", "File not found, move your binary to the filters directory, or update the $PATH environment variable." ) + "</font>", widget, "FilterCommand_PathLabel" )
            : new QLabel( path_.ToUTF8().c_str(), widget, "FilterCommand_PathLabel" );
        pathLabel->setWordWrap( true );
        grid->addWidget( pathLabel, row++, 1 );
    }
    // Reload-Exercise
    {
        grid->addWidget( new QLabel( tools::translate( "FilterCommand", "Reload exercise:" ), widget, "FilterCommand_ReloadTitle" ), row, 0 );
        gui::RichCheckBox* checkBox = new gui::RichCheckBox( "ReloadCheckBox", widget );
        checkBox->setChecked( reloadExercise_ );
        checkBox->setEnabled( false );
        grid->addWidget( checkBox, row++, 1 );
    }
    // Non blocking
    {
        grid->addWidget( new QLabel( tools::translate( "FilterCommand", "Is blocking:" ), widget, "FilterCommand_ReloadTitle" ), row, 0 );
        gui::RichCheckBox* checkBox = new gui::RichCheckBox( "ReloadCheckBox", widget );
        checkBox->setChecked( !nonBlocking_ );
        checkBox->setEnabled( false );
        grid->addWidget( checkBox, row++, 1 );
    }
    // Input arguments
    for( auto it = inputArguments_.begin(); it != inputArguments_.end(); ++it, ++row )
    {
        if( arguments_[ it->first ].displayName_.empty() )
            grid->addWidget( new QLabel( tools::translate( "FilterCommand", "Argument '%1':" ).arg( arguments_[ it->first ].name_.c_str() ), widget, "FilterCommand_CommandTitle" ), row, 0 );
        else
            grid->addWidget( new QLabel( QString( "%1:" ).arg( arguments_[ it->first ].displayName_.c_str() ), widget, "FilterCommand_CommandTitle" ), row, 0 );
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
        std::wstring lowerCommand = command_.ToUnicode();
        std::transform( lowerCommand.begin(), lowerCommand.end(), lowerCommand.begin(), tolower);
        if( lowerCommand.find( L"melmil.exe" ) != std::wstring::npos && reloadExercise_ )
            emit ForceSaveAndAddActionPlanning( "melmil.xml" );
    }
    assert( !path_.IsEmpty() );
    auto process = boost::make_shared< frontend::ProcessWrapper >( *this );
    auto command = boost::make_shared< frontend::SpawnCommand >( config_, path_ / command_ + tools::Path::FromUTF8( argumentsLine_ ), "" );
    process->Add( command );
    command->SetWorkingDirectory( path_ );
    frontend::ProcessWrapper::Start( process );
    if( !nonBlocking_ )
        process->Join();
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
void FilterCommand::NotifyError( const std::string& error, const std::string& /*commanderEndpoint*/  )
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
    for( auto it = inputArguments_.begin(); it != inputArguments_.end(); ++it )
        arguments_[ it->first ].value_ = it->second->GetText().toStdString();
    ComputeArgument();
}

// -----------------------------------------------------------------------------
// Name: FilterCommand::Update
// Created: ABR 2012-05-30
// -----------------------------------------------------------------------------
void FilterCommand::Update()
{
    for( auto it = inputArguments_.begin(); it != inputArguments_.end(); ++it )
        it->second->Update();
    for( size_t i = 0; i < arguments_.size(); ++i )
        if( IsInputArgument( i ) )
            arguments_[ i ].value_.clear();
    ComputeArgument();
}
