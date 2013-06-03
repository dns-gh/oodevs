// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "FilterInputArgument.h"
#include "moc_FilterInputArgument.cpp"
#include "FilterPartiesListView.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/XmlDescription.h"
#include "tools/ExerciseConfig.h"

bool FilterInputArgument::IsInputArgument( const std::string& argument )
{
    return argument == "$input$"
        || argument == "$input_file$"
        || argument == "$input_dir$"
        || argument == "$log_file$"
        || argument == "$input_team_list$";
}

// -----------------------------------------------------------------------------
// Name: FilterInputArgument constructor
// Created: ABR 2011-09-28
// -----------------------------------------------------------------------------
FilterInputArgument::FilterInputArgument( const tools::ExerciseConfig& config, const std::string& argumentValue, const kernel::XmlDescription& description, const std::string& exerciseDir /* = "" */ )
    : exerciseDir_( exerciseDir )
    , description_( description.GetName() )
    , config_     ( config )
    , line_       ( 0 )
    , listView_   ( 0 )
    , errorLabel_( 0 )
{
    if( argumentValue == "$input$" )
        type_ = eInput;
    else if( argumentValue == "$input_file$" )
        type_ = eOpen;
    else if( argumentValue == "$input_dir$" )
        type_ = eDirectory;
    else if( argumentValue == "$input_team_list$" )
        type_ = eTeamList;
    else if( argumentValue == "$log_file$" )
        type_ = eSave;
    else
        throw std::runtime_error( __FUNCTION__ "Error, invalide parameter given to FilterInputArgument constructor. Must be $input$, $input_file$, $input_dir$, $log_file$ or $input_team_list$.");
}

// -----------------------------------------------------------------------------
// Name: FilterInputArgument destructor
// Created: ABR 2011-09-28
// -----------------------------------------------------------------------------
FilterInputArgument::~FilterInputArgument()
{
        // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FilterInputArgument::IsValid
// Created: ABR 2012-07-19
// -----------------------------------------------------------------------------
bool FilterInputArgument::IsValid()
{
    if( type_ == eTeamList && listView_ )
        return listView_->isEnabled();
    return true;
}

// -----------------------------------------------------------------------------
// Name: FilterInputArgument::CreateWidget
// Created: ABR 2011-09-28
// -----------------------------------------------------------------------------
QWidget* FilterInputArgument::CreateWidget( QWidget* parent )
{
    QWidget* result = 0;
    if( type_ == eInput )
    {
        line_ = new QLineEdit( parent );
        result = line_;
    }
    else if( type_ == eTeamList )
    {
        result = new Q3VBox( parent );
        static_cast< Q3VBox* >( result )->setSpacing( 5 );
        listView_ = new FilterPartiesListView( result, false );
        listView_->ParseOrbatFile( config_.GetOrbatFile() );
        connect( listView_, SIGNAL( ValueChanged() ), this, SLOT( OnTextChanged() ) );
        errorLabel_ = new QLabel( result );
        if( !listView_->isEnabled() )
            errorLabel_->setText( "<font color=\"#FF0000\">" + tools::translate( "FilterInputArgument", "Invalid or missing orbat file." ) + "</font><br/>" );
    }
    else
    {
        result = new Q3HBox( parent );
        static_cast< Q3HBox* >( result )->setSpacing( 5 );
        line_ = new QLineEdit( result );
        QPushButton* browseBtn = new QPushButton( tools::translate( "FilterInputArgument", "Browse..." ), result, "FilterInputArgument_BrowseButton" );
        connect( browseBtn, SIGNAL( clicked() ), SLOT( OnBrowse() ) );
    }
    if( line_ )
    {
        line_->setText( description_.c_str() );
        connect( line_, SIGNAL( textChanged( const QString& ) ), this, SLOT( OnTextChanged( const QString& ) ) );
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: FilterInputArgument::Update
// Created: ABR 2012-05-30
// -----------------------------------------------------------------------------
void FilterInputArgument::Update()
{
    if( type_ == eTeamList && listView_ )
    {
        listView_->ParseOrbatFile( config_.GetOrbatFile() );
        errorLabel_->setText( listView_->isEnabled() ? "" : "<font color=\"#FF0000\">" + tools::translate( "FilterInputArgument", "Invalid or missing orbat file." ) + "</font><br/>" );
    }
    if( type_ == eSave )
        line_->clear();
}

// -----------------------------------------------------------------------------
// Name: FilterInputArgument::OnBrowse
// Created: ABR 2011-09-28
// -----------------------------------------------------------------------------
void FilterInputArgument::OnBrowse()
{
    QString path = ( type_ == eOpen )
        ? QFileDialog::getOpenFileName( exerciseDir_.c_str(), QString(), QApplication::activeModalWidget(), "FilterInputArgument_FileDialog", tools::translate( "FilterInputArgument", "Select a file" ) )
        : ( type_ == eSave )
            ? QFileDialog::getSaveFileName( line_->text().isEmpty() ? ( exerciseDir_ + "/errorDQP.xml" ).c_str() : line_->text(), QString(), QApplication::activeModalWidget(), "FilterInputArgument_FileDialog", tools::translate( "FilterInputArgument", "Select a file" ) )
            : QFileDialog::getExistingDirectory( exerciseDir_.c_str(), QApplication::activeModalWidget(), "FilterInputArgument_DirectoryDialog", tools::translate( "FilterInputArgument", "Select a directory" ) );
    path.replace( "/", "\\" );
    line_->setText( path );
}

// -----------------------------------------------------------------------------
// Name: FilterInputArgument::OnTextChanged
// Created: ABR 2011-09-28
// -----------------------------------------------------------------------------
void FilterInputArgument::OnTextChanged( const QString& /*text*/ /* = "" */ )
{
    emit ValueChanged( GetText() );
}

// -----------------------------------------------------------------------------
// Name: FilterInputArgument::GetText
// Created: ABR 2011-09-28
// -----------------------------------------------------------------------------
QString FilterInputArgument::GetText() const
{
    if( type_ == eTeamList )
        return ( listView_ ) ? listView_->GetTeamList() : QString();
    return ( line_ ) ? line_->text() : QString();
}
