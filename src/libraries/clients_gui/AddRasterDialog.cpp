// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "AddRasterDialog.h"
#include "moc_AddRasterDialog.cpp"
#include "SpinBoxAndSlider.h"
#include "FileDialog.h"
#include "RichLineEdit.h"
#include "RichPushButton.h"
#include "RichWidget.h"
#include "SubObjectName.h"
#include <boost/algorithm/string.hpp>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: AddRasterDialog constructor
// Created: ABR 2012-06-12
// -----------------------------------------------------------------------------
AddRasterDialog::AddRasterDialog( QWidget* parent )
    : ModalDialog( parent, "AddRasterDialog" )
    , pixelSize_( 16 )
{
    SubObjectName subObject( "AddRasterDialog" );
    setCaption( tr( "Add a raster layer" ) );
    setMinimumSize( 300, 200 );
    tabWidget_ = new RichWidget< QTabWidget >( "tabWidget" );

    // General tab
    QWidget* generalTab = new QWidget( tabWidget_ );
    QVBoxLayout* generalLayout = new QVBoxLayout( generalTab );
    tabWidget_->addTab( generalTab, tr( "General" ) );
    QGridLayout* gridLayout = new QGridLayout();
    nameEditor_ = new RichLineEdit( "nameEditor" );
    nameEditor_->setPlaceholderText( tr( "Enter layer's name" ) );
    connect( nameEditor_, SIGNAL( textChanged( const QString& ) ), SLOT( OnValueChanged() ) );
    QHBoxLayout* fileLayout = new QHBoxLayout();
    fileEditor_ = new RichLineEdit( "fileEditor" );
    fileEditor_->setPlaceholderText( tr( "Enter files to import" ) );
    connect( fileEditor_, SIGNAL( textChanged( const QString& ) ), SLOT( OnValueChanged() ) );
    RichPushButton* browseButton = new RichPushButton( "browse", tr( "Browse..." ) );
    connect( browseButton, SIGNAL( pressed() ), SLOT( OnBrowse() ) );
    fileLayout->addWidget( fileEditor_ );
    fileLayout->addWidget( browseButton );
    gridLayout->addWidget( new QLabel( tr( "Name:" ) ), 0, 0 );
    gridLayout->addWidget( nameEditor_, 0, 1 );
    gridLayout->addWidget( new QLabel( tr( "Files:" ) ), 1, 0 );
    gridLayout->addLayout( fileLayout, 1, 1 );
    generalLayout->addLayout( gridLayout );
    // Warning
    warningLabel_ = new QLabel();
    generalLayout->addWidget( warningLabel_ );
    generalLayout->addStretch();

    // Advanced tab
    QWidget* advancedTab = new QWidget( tabWidget_ );
    tabWidget_->addTab( advancedTab, tr( "Advanced" ) );
    QVBoxLayout* advancedLayout = new QVBoxLayout( advancedTab );
    pixelSizeEditor_ = new SpinBoxAndSlider( 0, pixelSize_, 1, 20, 1 );
    pixelSizeEditor_->setSpacing( 5 );
    advancedLayout->addWidget( new QLabel( tr( "Pixel size:" ) ) );
    advancedLayout->addWidget( pixelSizeEditor_ );
    advancedLayout->addStretch();

    // Buttons
    okButton_ = new RichPushButton( "ok", tr( "Ok" ) );
    connect( okButton_, SIGNAL( pressed() ), SLOT( accept() ) );
    RichPushButton* cancelButton = new RichPushButton( "cancel", tr( "Cancel" ) );
    connect( cancelButton, SIGNAL( pressed() ), SLOT( reject() ) );
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget( okButton_ );
    buttonLayout->addWidget( cancelButton );

    // Layout
    QVBoxLayout* mainLayout = new QVBoxLayout( this );
    mainLayout->addWidget( tabWidget_ );
    mainLayout->addLayout( buttonLayout );
    hide();
}

// -----------------------------------------------------------------------------
// Name: AddRasterDialog destructor
// Created: ABR 2012-06-12
// -----------------------------------------------------------------------------
AddRasterDialog::~AddRasterDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AddRasterDialog::OnValueChanged
// Created: ABR 2012-06-12
// -----------------------------------------------------------------------------
void AddRasterDialog::OnValueChanged()
{
    QString warning = "";
    if( nameEditor_->text().isEmpty() )
        warning += "<font color=\"#FF0000\">" + tr( "No name defined." ) + "</font><br/>";
    if( fileEditor_->text().isEmpty() )
        warning += "<font color=\"#FF0000\">" + tr( "No files defined." ) + "</font><br/>";
    else
    {
        QStringList list = QStringList::split( ";", fileEditor_->text() );
        for( QStringList::const_iterator it = list.constBegin(); it != list.constEnd(); ++it )
            if( !tools::Path::FromUnicode( it->toStdWString() ).Exists() )
                warning += "<font color=\"#FF0000\">" + tr( "File '%1' doesn't exist." ).arg( *it ) + "</font><br/>";
    }
    warningLabel_->setText( warning );
    warningLabel_->setVisible( !warning.isEmpty() );
    okButton_->setEnabled( warning.isEmpty() );
}

// -----------------------------------------------------------------------------
// Name: AddRasterDialog::OnBrowse
// Created: ABR 2012-06-12
// -----------------------------------------------------------------------------
void AddRasterDialog::OnBrowse()
{
    tools::Path::T_Paths strRasterPathFiles = gui::FileDialog::getOpenFileNames( this,
            tr( "Select USRP header file or GeoTIFF File" ), "",
            tr( "Images (*.tif *.bmp *.png *.jpg *.jp2);;"
                "USRP database (TRANSH01.THF);;"
                "All Files (*.*)" ) );
    if( strRasterPathFiles.empty() )
        return;

    std::wstring files;
    for( auto it = strRasterPathFiles.begin(); it != strRasterPathFiles.end(); ++it )
        files += it->Normalize().ToUnicode() + L";";
    if( files.empty() )
        return;
    files.erase( files.size() - 1 );
    boost::to_lower( files );
    fileEditor_->setText( QString::fromStdWString( files ) );
}

// -----------------------------------------------------------------------------
// Name: AddRasterDialog::showEvent
// Created: ABR 2012-06-12
// -----------------------------------------------------------------------------
void AddRasterDialog::showEvent( QShowEvent* event )
{
    tabWidget_->setCurrentIndex( 0 );
    pixelSizeEditor_->setValue( 16 );
    nameEditor_->clear();
    fileEditor_->clear();
    OnValueChanged();
    QDialog::showEvent( event );
}

// -----------------------------------------------------------------------------
// Name: AddRasterDialog::GetName
// Created: ABR 2012-06-12
// -----------------------------------------------------------------------------
const QString AddRasterDialog::GetName() const
{
    return nameEditor_->text();
}

// -----------------------------------------------------------------------------
// Name: AddRasterDialog::GetFiles
// Created: ABR 2012-06-12
// -----------------------------------------------------------------------------
const QString AddRasterDialog::GetFiles() const
{
    return fileEditor_->text();
}

// -----------------------------------------------------------------------------
// Name: AddRasterDialog::GetPixelSize
// Created: ABR 2012-06-12
// -----------------------------------------------------------------------------
int AddRasterDialog::GetPixelSize() const
{
    return pixelSize_;
}
