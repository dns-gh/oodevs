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
#include "preparation/UrbanExportManager.h"
#include "clients_gui/resources.h"
#include "tools/ExerciseConfig.h"
#include <terrain/PlanarCartesianProjector.h>
#include <terrain/TerrainExportManager.h>
#include <terrain/Translator.h>
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
    QHBoxLayout* layoutExportFolder = new QHBoxLayout();
    QLabel* labelshp = new QLabel( tr( "Export directory:"), this );
    exportPathEditor_ = new QLineEdit( );
    exportPathEditor_->setReadOnly( true );
    exportPathEditor_->setMinimumWidth( 250 );

    QPushButton* pBrowseExport = new QPushButton( tr( "..." ) );
    pBrowseExport->setFixedWidth( 20 );
    pBrowseExport->setFixedHeight( 20 );

    layoutExportFolder->addWidget( labelshp );
    layoutExportFolder->addWidget( exportPathEditor_ );
    layoutExportFolder->addWidget( pBrowseExport );
    exportGroupBox->setLayout( layoutExportFolder );

    QHBoxLayout* layoutButton = new QHBoxLayout();
    okButton_ = new QPushButton( tr( "Ok" ) );
    okButton_->adjustSize();
    QPushButton* pCancel = new QPushButton( tr( "Cancel" ) );
    layoutButton->addWidget( okButton_ );
    layoutButton->addWidget( pCancel );
    layoutButton->setAlignment( Qt::AlignRight );

    shapeCheck_ = new QCheckBox( tr( "Export vector data (Shapefile)" ) );
    connect( shapeCheck_, SIGNAL( clicked( bool ) ), SLOT( CheckExportReady() ) );
    shapeCheck_->setChecked( true );
    rasterCheck_ = new QCheckBox( tr( "Export raster data (GeoTIFF)" ) );
    connect( rasterCheck_, SIGNAL( clicked( bool ) ), SLOT( CheckExportReady() ) );
    rasterCheck_->setChecked( true );
    elevationCheck_ = new QCheckBox( tr( "Export elevation data (ESRI Ascii GRID)" ) );
    connect( elevationCheck_, SIGNAL( clicked( bool ) ), SLOT( CheckExportReady() ) );
    elevationCheck_->setChecked( true );

    mainLayout->addWidget( shapeCheck_ );
    mainLayout->addWidget( rasterCheck_ );
    mainLayout->addWidget( elevationCheck_ );
    mainLayout->addWidget( exportGroupBox );
    mainLayout->addLayout( layoutButton );

    setLayout( mainLayout );

    connect( pBrowseExport, SIGNAL( clicked() ), this, SLOT( OnBrowseExport() ) );
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
    exportPathEditor_->clear();
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
// Name: TerrainExportDialog::accept
// Created: AME 2010-10-15
// -----------------------------------------------------------------------------
void TerrainExportDialog::accept()
{
    QDialog::accept();
    const std::string path = exportPathEditor_->text().toStdString();
    assert( !path.empty() && bfs::exists( path ) && bfs::is_directory( path ) );
    QProgressDialog progressDialog( "", "", 0, 100, this, Qt::SplashScreen );
    progressDialog.setAutoClose( true );
    progressDialog.setContentsMargins( 5, 5, 5, 5 );
    progressDialog.setCancelButton( 0 );
    try
    {
        SetProgression( progressDialog, 0, tr( "Exporting terrain data..." ) );
        if( shapeCheck_->isChecked() )
        {
            PlanarCartesianProjector projector( config_.GetTerrainLatitude(), config_.GetTerrainLongitude() );
            Translator translator( projector, geometry::Vector2d( config_.GetTerrainWidth() / 2.f, config_.GetTerrainHeight() / 2.f ) );
            {
                TerrainExportManager manager( config_.GetTerrainDir( config_.GetTerrainName() ), translator );
                manager.Run( path );
            }
            SetProgression( progressDialog, 25, tr( "Exporting urban data..." ) );
            {
                UrbanExportManager manager( path, translator, urbanModel_ );
                manager.Run();
            }
        }
        SetProgression( progressDialog, 50, tr( "Exporting raster data..." ) );
        extractor::TerrainExtractionManager extractor( bfs::path( config_.GetTerrainFile() ).parent_path() );
        if( rasterCheck_->isChecked() )
            extractor.ExportRaster( path );
        SetProgression( progressDialog, 75, tr( "Exporting elevation data..." ) );
        if( elevationCheck_->isChecked() )
            extractor.ExportElevation( path );
        SetProgression( progressDialog, 100, "" );
        QMessageBox::information( this, tr( "Terrain export" ), tr( "Export successful." ) );
    }
    catch( const std::exception& e )
    {
        SetProgression( progressDialog, 100, "" );
        QMessageBox::critical( this, tr( "Error during export process" ), tools::GetExceptionMsg( e ).c_str() );
    }
}

// -----------------------------------------------------------------------------
// Name: TerrainExportDialog::OnBrowseExport
// Created: AME 2010-10-15
// -----------------------------------------------------------------------------
void TerrainExportDialog::OnBrowseExport()
{
    QString newDirectory = QFileDialog::getExistingDirectory( this, tr( "Select export directory" ), exportPathEditor_->text() );
    exportPathEditor_->setText( newDirectory );
    CheckExportReady();
}

// -----------------------------------------------------------------------------
// Name: TerrainExportDialog::OnBrowseExport
// Created: AME 2010-10-15
// -----------------------------------------------------------------------------
void TerrainExportDialog::CheckExportReady()
{
    const QString newDirectory = exportPathEditor_->text();
    const bool isExportEnabled = shapeCheck_->isChecked() || rasterCheck_->isChecked() || elevationCheck_->isChecked();
    okButton_->setEnabled( isExportEnabled && !newDirectory.isEmpty() && bfs::exists( newDirectory.toStdString() ) && bfs::is_directory( newDirectory.toStdString() ) );
}
