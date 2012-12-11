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
        type_ = eFile;
    else if( argumentValue == "$input_dir$" )
        type_ = eDirectory;
    else if( argumentValue == "$input_team_list$" )
        type_ = eTeamList;
    else
        throw MASA_EXCEPTION( "Error, invalide parameter. Must be $input$, $input_file$, $input_dir$ or $input_team_list$.");
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
}

// -----------------------------------------------------------------------------
// Name: FilterInputArgument::OnBrowse
// Created: ABR 2011-09-28
// -----------------------------------------------------------------------------
void FilterInputArgument::OnBrowse()
{
    QString path = ( type_ == eFile )
        ? QFileDialog::getOpenFileName( QApplication::activeModalWidget(), tools::translate( "FilterInputArgument", "Select a file" ), exerciseDir_.c_str() )
        : QFileDialog::getExistingDirectory( QApplication::activeModalWidget(), tools::translate( "FilterInputArgument", "Select a directory" ), exerciseDir_.c_str() );
    path.replace( "/", "\\" );
    line_->setText( path );
}

// -----------------------------------------------------------------------------
// Name: FilterInputArgument::OnTextChanged
// Created: ABR 2011-09-28
// -----------------------------------------------------------------------------
void FilterInputArgument::OnTextChanged( const QString& /* text */ /* = "" */ )
{
    emit ValueChanged();
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
