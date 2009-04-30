// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ScoreExportDialog.h"
#include "moc_ScoreExportDialog.cpp"
#include "gaming/IndicatorRequest.h"
#include "gaming/Tools.h"
#include <fstream>
#include <string>

// -----------------------------------------------------------------------------
// Name: ScoreExportDialog constructor
// Created: SBO 2009-04-30
// -----------------------------------------------------------------------------
ScoreExportDialog::ScoreExportDialog( QWidget* parent )
    : QDialog( parent, "ScoreExportDialog" )
    , current_( 0 )
{
    setCaption( tools::translate( "Scores", "Export data" ) );
    QGridLayout* grid = new QGridLayout( this, 2, 2, 0, 5 );
    grid->setMargin( 5 );
    grid->setRowStretch( 0, 4 );
    {
        QGroupBox* box = new QGroupBox( 3, Qt::Horizontal, tools::translate( "Scores", "Output" ), this );
        new QLabel( tools::translate( "Scores", "File: " ), box );
        file_ = new QLineEdit( box );
        QButton* browse = new QPushButton( tools::translate( "Scores", "Browse..." ), box );
        new QLabel( tools::translate( "Scores", "Separator: " ), box );
        separator_ = new QLineEdit( ";", box );
        header_ = new QCheckBox( tools::translate( "Scores", "Headers" ), box );
        header_->setChecked( true );
        connect( browse, SIGNAL( clicked() ), SLOT( OnBrowse() ) );
        connect( file_, SIGNAL( textChanged( const QString& ) ), SLOT( OnFileChanged( const QString& ) ) );
        grid->addMultiCellWidget( box, 0, 0, 0, 1 );
    }
    {
        QHBox* box = new QHBox( this );
        ok_ = new QPushButton( tools::translate( "Scores", "Ok" ), box );
        ok_->setEnabled( false );
        QButton* cancel = new QPushButton( tools::translate( "Scores", "Cancel" ), box );
        grid->addWidget( box, 1, 1 );
        connect( ok_, SIGNAL( clicked() ), SLOT( OnAccept() ) );
        connect( cancel, SIGNAL( clicked() ), SLOT( reject() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ScoreExportDialog destructor
// Created: SBO 2009-04-30
// -----------------------------------------------------------------------------
ScoreExportDialog::~ScoreExportDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ScoreExportDialog::Export
// Created: SBO 2009-04-30
// -----------------------------------------------------------------------------
void ScoreExportDialog::Export( const IndicatorRequest& request )
{
    current_ = &request;
    show();
}

// -----------------------------------------------------------------------------
// Name: ScoreExportDialog::OnBrowse
// Created: SBO 2009-04-30
// -----------------------------------------------------------------------------
void ScoreExportDialog::OnBrowse()
{
    QString filename = QFileDialog::getSaveFileName( 0, tools::translate( "Scores", "CSV (*.csv)" ), topLevelWidget(), 0, tools::translate( "Scores", "Export data" ) );
    if( filename == QString::null )
        return;
    if( !filename.endsWith( ".csv" ) )
        filename += ".csv";
    file_->setText( filename );
}

// -----------------------------------------------------------------------------
// Name: ScoreExportDialog::OnFileChanged
// Created: SBO 2009-04-30
// -----------------------------------------------------------------------------
void ScoreExportDialog::OnFileChanged( const QString& text )
{
    ok_->setEnabled( !text.isEmpty() );
}

// -----------------------------------------------------------------------------
// Name: ScoreExportDialog::OnAccept
// Created: SBO 2009-04-30
// -----------------------------------------------------------------------------
void ScoreExportDialog::OnAccept()
{
    std::ofstream file( file_->text().ascii() );
    const std::string sep = separator_->text().ascii();
    if( header_->isChecked() )
        file << tools::translate( "Scores", "Time;%1" ).arg( current_->GetName() ) << std::endl;
    for( unsigned int i = 0; i < current_->Result().size(); ++i )
        file << i << sep << current_->Result()[i] << std::endl;
    file.close();
    accept();
}
