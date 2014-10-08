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
#include "clients_gui/FileDialog.h"
#include "clients_kernel/Tools.h"
#include <boost/foreach.hpp>

// -----------------------------------------------------------------------------
// Name: IndicatorExportDialog constructor
// Created: SBO 2009-04-30
// -----------------------------------------------------------------------------
IndicatorExportDialog::IndicatorExportDialog( QWidget* parent )
    : QDialog( parent, "IndicatorExportDialog" )
    , request_( 0 )
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
    request_ = &request;
}

// -----------------------------------------------------------------------------
// Name: IndicatorExportDialog::Export
// Created: SBO 2009-05-04
// -----------------------------------------------------------------------------
void IndicatorExportDialog::Export()
{
    if( request_ )
        show();
}

// -----------------------------------------------------------------------------
// Name: IndicatorExportDialog::OnBrowse
// Created: SBO 2009-04-30
// -----------------------------------------------------------------------------
void IndicatorExportDialog::OnBrowse()
{
    tools::Path filename = gui::FileDialog::getSaveFileName( topLevelWidget(), tools::translate( "Scores", "Export data" ), "", tools::translate( "Scores", "CSV (*.csv)" ), 0, QFileDialog::DontConfirmOverwrite );
    if( filename.IsEmpty() )
        return;
    if( filename.Extension() != ".csv" )
        filename.ReplaceExtension( ".csv" );
    file_->setText( QString::fromStdWString( filename.ToUnicode() ) );
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
        if( !request_ )
            return;
        const tools::Path filepath( file_->text().toStdString().c_str() );
        if( filepath.Exists() )
            if( QMessageBox::warning( QApplication::activeWindow(), tools::translate( "IndicatorExportDialog", "Confirm file replace" ),
                tools::translate( "IndicatorExportDialog", "%1 already exists.\nDo you want to replace it?" ).arg( filepath.FileName().ToUTF8().c_str() ),
                QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes ) == QMessageBox::No )
                return;
        tools::Ofstream file( filepath );
        if( !file )
            throw MASA_EXCEPTION( tools::translate( "IndicatorExportDialog", "Impossible to create file in specified directory" ).toStdString() );
        const std::string sep = separator_->text().toStdString();
        if( header_->isChecked() )
        {
            file << tools::translate( "Indicators", "Time" );
            file << sep << request_->GetDisplayName();
            file << std::endl;
        }
        std::size_t index = request_->GetFirstTick();
        auto requestResult = request_->Result();
        for( auto it = requestResult.begin(); it != requestResult.end(); ++it )
        {
            file << index;
            file << sep;
            file << *it;
            file << std::endl;
            ++index;
        }
        file.close();
        request_ = 0;
        accept();
    }
    catch( const std::exception& e )
    {
        QMessageBox::critical( this, tr( "Can not save indicator file :" ), tools::GetExceptionMsg( e ).c_str() );
    }
}
