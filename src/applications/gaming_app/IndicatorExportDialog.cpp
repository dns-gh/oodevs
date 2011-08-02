// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "IndicatorExportDialog.h"
#include "moc_IndicatorExportDialog.cpp"
#include "gaming/IndicatorRequest.h"
#include "gaming/Tools.h"
#include <boost/foreach.hpp>
#include <fstream>

// -----------------------------------------------------------------------------
// Name: IndicatorExportDialog constructor
// Created: SBO 2009-04-30
// -----------------------------------------------------------------------------
IndicatorExportDialog::IndicatorExportDialog( QWidget* parent )
    : QDialog( parent, "IndicatorExportDialog" )
{
    setCaption( tools::translate( "Scores", "Export data" ) );
    Q3GridLayout* grid = new Q3GridLayout( this, 2, 2, 0, 5 );
    grid->setMargin( 5 );
    grid->setRowStretch( 0, 4 );
    {
        Q3GroupBox* box = new Q3GroupBox( 3, Qt::Horizontal, tools::translate( "Scores", "Output" ), this );
        new QLabel( tools::translate( "Scores", "File: " ), box );
        file_ = new QLineEdit( box );
        QPushButton* browse = new QPushButton( tools::translate( "Scores", "Browse..." ), box );
        new QLabel( tools::translate( "Scores", "Separator: " ), box );
        separator_ = new QLineEdit( ";", box );
        header_ = new QCheckBox( tools::translate( "Scores", "Headers" ), box );
        header_->setChecked( true );
        connect( browse, SIGNAL( clicked() ), SLOT( OnBrowse() ) );
        connect( file_, SIGNAL( textChanged( const QString& ) ), SLOT( OnFileChanged( const QString& ) ) );
        grid->addMultiCellWidget( box, 0, 0, 0, 1 );
    }
    {
        Q3HBox* box = new Q3HBox( this );
        ok_ = new QPushButton( tools::translate( "Scores", "Ok" ), box );
        ok_->setEnabled( false );
        QPushButton* cancel = new QPushButton( tools::translate( "Scores", "Cancel" ), box );
        grid->addWidget( box, 1, 1 );
        connect( ok_, SIGNAL( clicked() ), SLOT( OnAccept() ) );
        connect( cancel, SIGNAL( clicked() ), SLOT( reject() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: IndicatorExportDialog destructor
// Created: SBO 2009-04-30
// -----------------------------------------------------------------------------
IndicatorExportDialog::~IndicatorExportDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IndicatorExportDialog::Add
// Created: SBO 2009-04-30
// -----------------------------------------------------------------------------
void IndicatorExportDialog::Add( const IndicatorRequest& request )
{
    requests_.push_back( &request );
}

// -----------------------------------------------------------------------------
// Name: IndicatorExportDialog::Export
// Created: SBO 2009-05-04
// -----------------------------------------------------------------------------
void IndicatorExportDialog::Export()
{
    if( ! requests_.empty() )
        show();
}

// -----------------------------------------------------------------------------
// Name: IndicatorExportDialog::OnBrowse
// Created: SBO 2009-04-30
// -----------------------------------------------------------------------------
void IndicatorExportDialog::OnBrowse()
{
    QString filename = Q3FileDialog::getSaveFileName( 0, tools::translate( "Scores", "CSV (*.csv)" ), topLevelWidget(), 0, tools::translate( "Scores", "Export data" ) );
    if( filename == QString::null )
        return;
    if( !filename.endsWith( ".csv" ) )
        filename += ".csv";
    file_->setText( filename );
}

// -----------------------------------------------------------------------------
// Name: IndicatorExportDialog::OnFileChanged
// Created: SBO 2009-04-30
// -----------------------------------------------------------------------------
void IndicatorExportDialog::OnFileChanged( const QString& text )
{
    ok_->setEnabled( !text.isEmpty() );
}

// -----------------------------------------------------------------------------
// Name: IndicatorExportDialog::OnAccept
// Created: SBO 2009-04-30
// -----------------------------------------------------------------------------
void IndicatorExportDialog::OnAccept()
{
    try
    {
        std::ofstream file( file_->text().ascii() );
        if( !file )
            throw std::exception(tools::translate( "IndicatorExportDialog", "Impossible to create file in specified directory" ) );
        const std::string sep = separator_->text().ascii();
        if( header_->isChecked() )
        {
            file << tools::translate( "Indicators", "Time" );
            BOOST_FOREACH( const T_Requests::value_type& request, requests_ )
                file << sep << request->GetName();
            file << std::endl;
        }
        std::size_t hasData = requests_.size();
        std::size_t index = 0;
        while( hasData )
        {
            file << index;
            BOOST_FOREACH( const T_Requests::value_type& request, requests_ )
            {
                const std::size_t size = request->Result().size();
                file << sep;
                if( index < size )
                    file << request->Result()[ index ];
                else if( index == size )
                    --hasData;
            }
            file << std::endl;
            ++index;
        }
        file.close();
        requests_.clear();
        accept();
    }
    catch ( std::exception& e )
    {
        QMessageBox::critical( this, tr( "Can not save indicator file :" ), e.what() );
    }
}
