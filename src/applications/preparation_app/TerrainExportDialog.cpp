// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "TerrainExportDialog.h"
#include "moc_TerrainExportDialog.cpp"
#include "preparation/UrbanModel.h"
#include "clients_gui/resources.h"
#include <boost/filesystem.hpp>

namespace bfs = boost::filesystem;

// -----------------------------------------------------------------------------
// Name: TerrainExportDialog constructor
// Created: ABR 2012-06-13
// -----------------------------------------------------------------------------
TerrainExportDialog::TerrainExportDialog( QWidget* parent, const tools::ExerciseConfig& config, const UrbanModel& urbanModel )
    : ModalDialog( parent, "TerrainExportDialog" )
    , urbanModel_( urbanModel )
    , config_    ( config )
{
    setCaption( tr( "Terrain Export" ) );

    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->setMargin( 5 );

    QGroupBox* exportGroupBox = new QGroupBox();
    QHBoxLayout* layoutShpFolder = new QHBoxLayout();
    QLabel* labelshp = new QLabel( tr( "Export shapefiles directory:"), this );
    pathEditor_ = new QLineEdit( );
    pathEditor_->setReadOnly( true );
    pathEditor_->setMinimumWidth( 250 );
    QPushButton* pBrowseExport = new QPushButton( tr( "..." ) );
    pBrowseExport->setFixedWidth( 20 );
    pBrowseExport->setFixedHeight( 20 );
    layoutShpFolder->addWidget( labelshp );
    layoutShpFolder->addWidget( pathEditor_ );
    layoutShpFolder->addWidget( pBrowseExport );
    exportGroupBox->setLayout( layoutShpFolder );

    QHBoxLayout* layoutButton = new QHBoxLayout();
    okButton_ = new QPushButton( tr( "Ok" ) );
    okButton_->adjustSize();
    QPushButton* pCancel = new QPushButton( tr( "Cancel" ) );
    layoutButton->addWidget( okButton_ );
    layoutButton->addWidget( pCancel );
    layoutButton->setAlignment( Qt::AlignRight );

    mainLayout->addWidget( exportGroupBox );
    mainLayout->addLayout( layoutButton );

    setLayout( mainLayout );

    connect( pBrowseExport, SIGNAL( clicked() ), this, SLOT( OnBrowseExportShp() ) );
    connect( okButton_, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( pCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
}

// -----------------------------------------------------------------------------
// Name: TerrainExportDialog destructor
// Created: ABR 2012-06-13
// -----------------------------------------------------------------------------
TerrainExportDialog::~TerrainExportDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TerrainExportDialog::showEvent
// Created: ABR 2012-06-13
// -----------------------------------------------------------------------------
void TerrainExportDialog::showEvent( QShowEvent* event )
{
    pathEditor_->clear();
    okButton_->setEnabled( false );
    QDialog::showEvent( event );
}

namespace
{
    void SetProgression( QProgressDialog& progressDialog, int value, const QString& text )
    {
        if( !value )
            progressDialog.show();
        progressDialog.setLabelText( text );
        progressDialog.setValue( value );
        qApp->processEvents();
    }
}

// -----------------------------------------------------------------------------
// Name: TerrainExportDialog::OnOk
// Created: AME 2010-10-15
// -----------------------------------------------------------------------------
void TerrainExportDialog::accept()
{
    QDialog::accept();
    const std::string path = pathEditor_->text().toAscii().constData();
    assert( !path.empty() && bfs::exists( path ) && bfs::is_directory( path ) );

    QProgressDialog progressDialog( "", "", 0, 100, this, Qt::SplashScreen );
    progressDialog.setAutoClose( true );
    progressDialog.setContentsMargins( 5, 5, 5, 5 );
    progressDialog.setCancelButton( 0 );

    try
    {
        urbanModel_.ExportShapeFile( path, config_, progressDialog );
        QMessageBox::information( this, tr( "Terrain export" ),tr( "Export successfull." ) );
    }
    catch( std::exception& e )
    {
        SetProgression( progressDialog, 100, "" );
        QMessageBox::critical( this, tr( "Error during export process" ), e.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: TerrainExportDialog::OnBrowseExportShp
// Created: AME 2010-10-15
// -----------------------------------------------------------------------------
void TerrainExportDialog::OnBrowseExportShp()
{
    QString newDirectory = QFileDialog::getExistingDirectory( this, tr( "Select export directory" ), pathEditor_->text() );
    pathEditor_->setText( newDirectory );
    okButton_->setEnabled( !newDirectory.isEmpty() && bfs::exists( newDirectory.toAscii().constData() ) && bfs::is_directory( newDirectory.toAscii().constData() ) );
}
