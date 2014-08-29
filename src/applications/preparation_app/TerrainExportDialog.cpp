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
#include "preparation/UrbanFileExporter.h"
#include "clients_gui/FileDialog.h"
#include "clients_gui/resources.h"
#include "clients_gui/RichPushButton.h"
#include "clients_gui/RichGroupBox.h"
#include "clients_gui/RichLineEdit.h"
#include "clients_gui/RichCheckBox.h"
#include "tools/ExerciseConfig.h"
#include <extractor/TerrainExportManager.h>
#include <terrain/PlanarCartesianProjector.h>
#include <terrain/Translator.h>

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
    gui::SubObjectName subObject( "TerrainExportDialog" );

    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->setMargin( 5 );

    gui::RichGroupBox* exportGroupBox = new gui::RichGroupBox( "export" );
    QHBoxLayout* layoutExportFolder = new QHBoxLayout();
    QLabel* labelshp = new QLabel( tr( "Export directory:"), this );
    exportPathEditor_ = new gui::RichLineEdit( "exportPathEditor" );
    exportPathEditor_->setReadOnly( true );
    exportPathEditor_->setMinimumWidth( 250 );

    gui::RichPushButton* pBrowseExport = new gui::RichPushButton( "browseExport", tr( "..." ) );
    pBrowseExport->setFixedWidth( 20 );
    pBrowseExport->setFixedHeight( 20 );

    layoutExportFolder->addWidget( labelshp );
    layoutExportFolder->addWidget( exportPathEditor_ );
    layoutExportFolder->addWidget( pBrowseExport );
    exportGroupBox->setLayout( layoutExportFolder );

    QHBoxLayout* layoutButton = new QHBoxLayout();
    okButton_ = new gui::RichPushButton( "ok", tr( "Ok" ) );
    okButton_->adjustSize();
    gui::RichPushButton* pCancel = new gui::RichPushButton( "cancel", tr( "Cancel" ) );
    layoutButton->addWidget( okButton_ );
    layoutButton->addWidget( pCancel );
    layoutButton->setAlignment( Qt::AlignRight );

    shapeCheck_ = new gui::RichCheckBox( "shapeCheck", tr( "Export vector data (Shapefile)" ) );
    connect( shapeCheck_, SIGNAL( clicked( bool ) ), SLOT( CheckExportReady() ) );
    shapeCheck_->setChecked( true );
    rasterCheck_ = new gui::RichCheckBox( "rasterCheck", tr( "Export raster data (GeoTIFF)" ) );
    connect( rasterCheck_, SIGNAL( clicked( bool ) ), SLOT( CheckExportReady() ) );
    rasterCheck_->setChecked( true );
    elevationCheck_ = new gui::RichCheckBox( "elevationCheck", tr( "Export elevation data (ESRI Ascii GRID)" ) );
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
    const tools::Path path = tools::Path::FromUnicode( exportPathEditor_->text().toStdWString() );
    assert( !path.IsEmpty() && path.IsDirectory() );
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
                ExportUrbanFiles( path, "urbanblock", translator, urbanModel_ );
            }
        }
        SetProgression( progressDialog, 50, tr( "Exporting raster data..." ) );
        extractor::TerrainExtractionManager extractor( config_.GetTerrainFile().Parent() );
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
    tools::Path newDirectory = gui::FileDialog::getExistingDirectory( this, tr( "Select export directory" ), tools::Path::FromUnicode( exportPathEditor_->text().toStdWString() ) );
    exportPathEditor_->setText( QString::fromStdWString( newDirectory.ToUnicode() ) );
    CheckExportReady();
}

// -----------------------------------------------------------------------------
// Name: TerrainExportDialog::OnBrowseExport
// Created: AME 2010-10-15
// -----------------------------------------------------------------------------
void TerrainExportDialog::CheckExportReady()
{
    const tools::Path newDirectory = tools::Path::FromUnicode( exportPathEditor_->text().toStdWString() );
    const bool isExportEnabled = shapeCheck_->isChecked() || rasterCheck_->isChecked() || elevationCheck_->isChecked();
    okButton_->setEnabled( isExportEnabled && !newDirectory.IsEmpty() && newDirectory.IsDirectory() );
}
