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
#include "clients_kernel/Tools.h"
#include "clients_kernel/XmlDescription.h"

// -----------------------------------------------------------------------------
// Name: FilterInputArgument constructor
// Created: ABR 2011-09-28
// -----------------------------------------------------------------------------
FilterInputArgument::FilterInputArgument( const std::string& argumentValue, const kernel::XmlDescription& description, const std::string exerciseDir /*= ""*/ )
    : exerciseDir_( exerciseDir )
    , description_( description.GetName() )
{
    if( argumentValue == "$input$" )
        type_ = eInput;
    else if( argumentValue == "$input_file$" )
        type_ = eFile;
    else if( argumentValue == "$input_dir$" )
        type_ = eDirectory;
    else
        throw std::runtime_error( __FUNCTION__ "Error, invalide parameter given to FilterInputArgument constructor. Must be $input$, $input_file$ or $input_dir$.");
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
// Name: FilterInputArgument::CreateWidget
// Created: ABR 2011-09-28
// -----------------------------------------------------------------------------
QWidget* FilterInputArgument::CreateWidget( QWidget* parent )
{
    QWidget* result = 0;
    if( type_ == eInput )
    {
        line_ = new QLineEdit( parent, "FilterInputArgument_EditLine" );
        result = line_;
    }
    else
    {
        result = new Q3HBox( parent, "FilterInputArgument_HBox" );
        static_cast< Q3HBox* >( result )->setSpacing( 5 );
        line_ = new QLineEdit( result, "FilterInputArgument_EditLine" );
        QPushButton* browseBtn = new QPushButton( tools::translate( "FilterInputArgument", "Browse..." ), result, "FilterInputArgument_BrowseButton" );
        connect( browseBtn, SIGNAL( clicked() ), SLOT( OnBrowse() ) );
    }
    line_->setText( description_.c_str() );
    connect( line_, SIGNAL( textChanged( const QString& ) ), this, SLOT( OnTextChanged( const QString& ) ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: FilterInputArgument::OnBrowse
// Created: ABR 2011-09-28
// -----------------------------------------------------------------------------
void FilterInputArgument::OnBrowse()
{
    QString path = ( type_ == eFile )
        ? QFileDialog::getOpenFileName( exerciseDir_.c_str(), QString(), QApplication::activeModalWidget(), "FilterInputArgument_FileDialog", tools::translate( "FilterInputArgument", "Select a file" ) )
        : QFileDialog::getExistingDirectory( exerciseDir_.c_str(), QApplication::activeModalWidget(), "FilterInputArgument_DirectoryDialog", tools::translate( "FilterInputArgument", "Select a directory" ) );
    path.replace( "/", "\\" );
    line_->setText( path );
}

// -----------------------------------------------------------------------------
// Name: FilterInputArgument::OnTextChanged
// Created: ABR 2011-09-28
// -----------------------------------------------------------------------------
void FilterInputArgument::OnTextChanged( const QString& /*text*/ )
{
    emit ValueChanged();
}

// -----------------------------------------------------------------------------
// Name: FilterInputArgument::GetText
// Created: ABR 2011-09-28
// -----------------------------------------------------------------------------
QString FilterInputArgument::GetText() const
{
    return QString( "\"%1\"" ).arg( ( line_ ) ? line_->text() : QString() );
}
