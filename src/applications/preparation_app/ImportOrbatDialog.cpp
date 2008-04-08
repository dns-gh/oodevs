// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ImportOrbatDialog.h"
#include "moc_ImportOrbatDialog.cpp"
#include "preparation/Model.h"
#include "preparation/OrbatImportFilter.h"
#include "tools/ExerciseConfig.h"
#include <xeumeuleu/xml.h>
#include <qlistview.h>

// -----------------------------------------------------------------------------
// Name: ImportOrbatDialog constructor
// Created: SBO 2008-04-07
// -----------------------------------------------------------------------------
ImportOrbatDialog::ImportOrbatDialog( QWidget* parent, const tools::ExerciseConfig& config, Model& model )
    : QDialog( parent )
    , config_( config )
    , model_( model )
    , importObjects_( true )
    , importPopulations_( true )
{
    setCaption( tr( "Import order of battle" ) );
    QGridLayout* grid = new QGridLayout( this, 3, 2, 5 );
    grid->setRowStretch( 0, 1 );
    grid->setRowStretch( 1, 3 );
    grid->setRowStretch( 2, 1 );
    {
        QGroupBox* box = new QGroupBox( 3, Qt::Horizontal, tr( "Select file to import:" ), this );
        new QLabel( tr( "Order of battle: " ), box );
        filename_ = new QLineEdit( box );
        QPushButton* browseBtn = new QPushButton( tr( "Browse..." ), box );
        connect( browseBtn, SIGNAL( clicked() ), SLOT( OnBrowse() ) );
        grid->addMultiCellWidget( box, 0, 0, 0, 1 );
    }
    {
        QGroupBox* box = new QGroupBox( 1, Qt::Horizontal, tr( "Sides:" ), this );
        preview_ = new QListView( box );
        preview_->setMinimumSize( 100, 150 );
        preview_->setDisabled( true );
        preview_->addColumn( "" );
        preview_->setResizeMode( QListView::LastColumn );
        preview_->header()->hide();
        grid->addWidget( box, 1, 0 );
    }
    {
        QGroupBox* box = new QGroupBox( 1, Qt::Horizontal, tr( "Import options:" ), this );
        QCheckBox* objectCheckBox = new QCheckBox( tr( "Objects" ), box );
        objectCheckBox->setChecked( importObjects_ );
        QCheckBox* populationCheckBox = new QCheckBox( tr( "Populations" ), box );
        populationCheckBox->setChecked( importPopulations_ );
        connect( objectCheckBox    , SIGNAL( toggled( bool ) ), SLOT( ToggleObjects( bool ) ) );
        connect( populationCheckBox, SIGNAL( toggled( bool ) ), SLOT( TogglePopulations( bool ) ) );
        grid->addWidget( box, 1, 1 );
    }
    {
        QHBox* box = new QHBox( this );
        box->setMargin( 5 );
        box->setMaximumHeight( 40 );
        okButton_ = new QPushButton( tr( "Ok" ), box );
        okButton_->setDefault( true );
        okButton_->setDisabled( true );
        QButton* cancelBtn = new QPushButton( tr( "Cancel" ), box );
        grid->addWidget( box, 2, 1, Qt::AlignRight );
        connect( okButton_, SIGNAL( clicked() ), SLOT( OnAccept() ) );
        connect( cancelBtn, SIGNAL( clicked() ), SLOT( OnReject() ) );
        grid->addWidget( box, 2, 1, Qt::AlignRight );
    }
    hide();
}

// -----------------------------------------------------------------------------
// Name: ImportOrbatDialog destructor
// Created: SBO 2008-04-07
// -----------------------------------------------------------------------------
ImportOrbatDialog::~ImportOrbatDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ImportOrbatDialog::OnAccept
// Created: SBO 2008-04-07
// -----------------------------------------------------------------------------
void ImportOrbatDialog::OnAccept()
{
    if( !filename_->text().isEmpty() )
    {
        model_.Import( filename_->text().ascii(), OrbatImportFilter( importObjects_, importPopulations_ ) );
        accept();
    }
}

// -----------------------------------------------------------------------------
// Name: ImportOrbatDialog::OnReject
// Created: SBO 2008-04-07
// -----------------------------------------------------------------------------
void ImportOrbatDialog::OnReject()
{
    reject();
}

// -----------------------------------------------------------------------------
// Name: ImportOrbatDialog::OnBrowse
// Created: SBO 2008-04-07
// -----------------------------------------------------------------------------
void ImportOrbatDialog::OnBrowse()
{
    QString filename = QFileDialog::getOpenFileName( config_.GetOrbatFile().c_str(), "Orbat (*.xml)", this, 0, tr( "Load orbat file" ) );
    if( filename.startsWith( "//" ) )
        filename.replace( "/", "\\" );
    filename_->setText( filename );
    LoadPreview();
}

// -----------------------------------------------------------------------------
// Name: ImportOrbatDialog::LoadPreview
// Created: SBO 2008-04-08
// -----------------------------------------------------------------------------
void ImportOrbatDialog::LoadPreview()
{
    preview_->clear();
    if( !filename_->text().isEmpty() )
    {
        try
        {
            xml::xifstream xis( filename_->text().ascii() );
            xis >> xml::start( "orbat" )
                    >> xml::start( "sides" )
                    >> xml::list( "side", *this, &ImportOrbatDialog::ReadTeam );                
            okButton_->setDisabled( false );
        }
        catch( ... )
        {
            QMessageBox::critical( this, tr( "Error loading file" ), tr( "File does not appear to be a valid orbat file.\nPlease select an other orbat file to import." ) );
            okButton_->setDisabled( true );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ImportOrbatDialog::ReadTeam
// Created: SBO 2008-04-08
// -----------------------------------------------------------------------------
void ImportOrbatDialog::ReadTeam( xml::xistream& xis )
{
    const std::string name = xml::attribute< std::string >( xis, "name" );
    QListViewItem* item = new QListViewItem( preview_ );
    item->setText( 0, name.c_str() );
}

// -----------------------------------------------------------------------------
// Name: ImportOrbatDialog::ToggleObjects
// Created: SBO 2008-04-08
// -----------------------------------------------------------------------------
void ImportOrbatDialog::ToggleObjects( bool toggle )
{
    importObjects_ = toggle;
}

// -----------------------------------------------------------------------------
// Name: ImportOrbatDialog::TogglePopulations
// Created: SBO 2008-04-08
// -----------------------------------------------------------------------------
void ImportOrbatDialog::TogglePopulations( bool toggle )
{
    importPopulations_ = toggle;
}
