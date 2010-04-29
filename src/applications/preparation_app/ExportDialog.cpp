// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ExportDialog.h"
#include "moc_ExportDialog.cpp"
#include "ExportFilter.h"
#include "tools/ExerciseConfig.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: ExportDialog constructor
// Created: SBO 2010-04-22
// -----------------------------------------------------------------------------
ExportDialog::ExportDialog( QWidget* parent, const tools::ExerciseConfig& config )
    : QDialog( parent )
    , config_( config )
{
    setCaption( tr( "Export exercise data" ) );
    QGridLayout* grid = new QGridLayout( this, 4, 2, 5 );
    grid->setRowStretch( 0, 3 );
    grid->setRowStretch( 1, 1 );
    grid->setRowStretch( 2, 1 );
    grid->setRowStretch( 3, 1 );
    {
        QGroupBox* box = new QGroupBox( 1, Qt::Horizontal, tr( "Select export filter:" ), this );
        list_ = new QListBox( box );
        grid->addMultiCellWidget( box, 0, 0, 0, 1 );
        connect( list_, SIGNAL( highlighted( int ) ), SLOT( OnSelectFilter( int ) ) );
    }
    {
        QGroupBox* box = new QGroupBox( 1, Qt::Horizontal, tr( "Description:" ), this );
        box->setMinimumHeight( 80 );
        description_ = new QLabel( box );
        grid->addMultiCellWidget( box, 1, 1, 0, 1 );
    }
    {
        QGroupBox* box = new QGroupBox( 3, Qt::Horizontal, tr( "Select output directory:" ), this );
        new QLabel( tr( "Output to: " ), box );
        output_ = new QLineEdit( box );
        QPushButton* browseBtn = new QPushButton( tr( "Browse..." ), box );
        connect( browseBtn, SIGNAL( clicked() ), SLOT( OnBrowse() ) );
        grid->addMultiCellWidget( box, 2, 2, 0, 1 );
    }
    {
        QHBox* box = new QHBox( this );
        box->setMargin( 5 );
        box->setMaximumHeight( 40 );
        okButton_ = new QPushButton( tr( "Ok" ), box );
        okButton_->setDefault( true );
        okButton_->setDisabled( true );
        QButton* cancelBtn = new QPushButton( tr( "Cancel" ), box );
        grid->addWidget( box, 3, 1, Qt::AlignRight );
        connect( okButton_, SIGNAL( clicked() ), SLOT( OnAccept() ) );
        connect( cancelBtn, SIGNAL( clicked() ), SLOT( OnReject() ) );
        grid->addWidget( box, 3, 1, Qt::AlignRight );
    }
    LoadFilters( tools::GeneralConfig::BuildResourceChildFile( "export/filters.xml" ) );
    hide();
}

// -----------------------------------------------------------------------------
// Name: ExportDialog destructor
// Created: SBO 2010-04-22
// -----------------------------------------------------------------------------
ExportDialog::~ExportDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExportDialog::OnSelectFilter
// Created: SBO 2010-04-26
// -----------------------------------------------------------------------------
void ExportDialog::OnSelectFilter( int index )
{
    const ExportFilter& filter = filters_.at( index );
    description_->setText( filter.GetDescription() );
}

// -----------------------------------------------------------------------------
// Name: ExportDialog::OnAccept
// Created: SBO 2010-04-22
// -----------------------------------------------------------------------------
void ExportDialog::OnAccept()
{
    if( !output_->text().isEmpty() )
    {
        const ExportFilter& filter = filters_.at( list_->currentItem() );
        filter.Execute( output_->text().ascii() );
        accept();
    }
}

// -----------------------------------------------------------------------------
// Name: ExportDialog::OnReject
// Created: SBO 2010-04-22
// -----------------------------------------------------------------------------
void ExportDialog::OnReject()
{
    reject();
}

// -----------------------------------------------------------------------------
// Name: ExportDialog::OnBrowse
// Created: SBO 2010-04-22
// -----------------------------------------------------------------------------
void ExportDialog::OnBrowse()
{
    QString directory = QFileDialog::getExistingDirectory( config_.GetExerciseFile().c_str(), this, 0, tr( "Select output directory" ) );
    if( directory.startsWith( "//" ) )
        directory.replace( "/", "\\" );
    output_->setText( directory );
    okButton_->setEnabled( !output_->text().isEmpty() );
}

// -----------------------------------------------------------------------------
// Name: ExportDialog::LoadFilters
// Created: SBO 2010-04-22
// -----------------------------------------------------------------------------
void ExportDialog::LoadFilters( const std::string& filename )
{
    xml::xifstream xis( filename );
    xis >> xml::start( "filters" )
            >> xml::list( "filter", *this, &ExportDialog::ReadFilter );
}

// -----------------------------------------------------------------------------
// Name: ExportDialog::ReadFilter
// Created: SBO 2010-04-22
// -----------------------------------------------------------------------------
void ExportDialog::ReadFilter( xml::xistream& xis )
{
    ExportFilter* filter = new ExportFilter( xis, config_ );
    filters_.push_back( filter );
    list_->insertItem( filter->GetName() );
}

// -----------------------------------------------------------------------------
// Name: ExportDialog::sizeHint
// Created: SBO 2010-04-23
// -----------------------------------------------------------------------------
QSize ExportDialog::sizeHint() const
{
    return QSize( 400, 300 );
}
