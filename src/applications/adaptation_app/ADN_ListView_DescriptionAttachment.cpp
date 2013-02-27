// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_ListView_DescriptionAttachment.h"
#include "moc_ADN_ListView_DescriptionAttachment.cpp"
#include "ADN_Connector_ListView.h"
#include "ADN_Missions_Parameter.h"
#include "clients_kernel/ContextMenu.h"
#include "boost/filesystem.hpp"
#include "ADN_Project_Data.h"
#include "ADN_Missions_ABC.h"

namespace bfs = boost::filesystem;

// -----------------------------------------------------------------------------
// Name: ADN_ListView_DescriptionAttachment constructor
// Created: NPT 2013-01-16
// -----------------------------------------------------------------------------
ADN_ListView_DescriptionAttachment::ADN_ListView_DescriptionAttachment( E_MissionType missionType )
    : ADN_ListView( 0, "ADN_ListView_DescriptionAttachment", tr( "Attachments" ) )
    , missionType_( missionType )
{
    pConnector_ = new ADN_Connector_ListView< ADN_Missions_ABC::ADN_Missions_Attachment >( *this );
    setHeaderHidden( true );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_DescriptionAttachment destructor
// Created: NPT 2013-01-16
// -----------------------------------------------------------------------------
ADN_ListView_DescriptionAttachment::~ADN_ListView_DescriptionAttachment()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_DescriptionAttachment::AddFile
// Created: NPT 2013-01-16
// -----------------------------------------------------------------------------
void ADN_ListView_DescriptionAttachment::AddFile()
{
    QString fileName = QFileDialog::getOpenFileName( this
        , tr( "Select an image" )
        , "."
        , tr( "All supported formats (*.bmp *.jpg *.jpeg *.gif *.png)\n"
        "Bitmap Files (*.bmp)\n"
        "JPEG (*.jpg *jpeg)\n"
        "GIF (*.gif)\n"
        "PNG (*.png)\n" )
        );
    if( fileName.isEmpty() )
        return;
    std::string imageDir = QDir::tempPath().toStdString() + "_MissionSheets/ImagesTemp" + QString::number( missionType_ ).toStdString() + "/";
    if( !bfs::is_directory( imageDir ) )
        bfs::create_directory( imageDir );
    std::string newFileName = imageDir + QFileInfo( fileName ).fileName().toStdString(); 
    if( fileName != QString( newFileName.c_str() ) )
    {
        if( bfs::exists( newFileName ) )
        {
            QMessageBox* box = new QMessageBox();
            box->setWindowTitle(  tr( "Existing file found" ) );
            box->setText( tr( "The file you want to copy already exists in the base, your file will be renamed" )  );
            box->setModal( true );
            box->show();
            std::string tempFileName = newFileName;
            int i = 1;
            while( bfs::exists( tempFileName ) )
            {
                tempFileName = imageDir + QString( QFileInfo( newFileName.c_str() ).completeBaseName() + QString::number( i ) + "." + QFileInfo( newFileName.c_str() ).suffix() ).toStdString();
                i++;
            }
            newFileName = tempFileName;
        }
        bfs::copy_file( fileName.toStdString(), newFileName, bfs::copy_option::overwrite_if_exists );
    }
    ADN_Connector_Vector_ABC* connector = static_cast< ADN_Connector_Vector_ABC* >( pConnector_ );
    if( connector )
        connector->AddItem( new ADN_Missions_ABC::ADN_Missions_Attachment( QFileInfo( newFileName.c_str() ).fileName().toStdString() ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_DescriptionAttachment::RemoveFile
// Created: NPT 2013-01-16
// -----------------------------------------------------------------------------
void ADN_ListView_DescriptionAttachment::RemoveFile()
{
    ADN_Connector_Vector_ABC* connector = static_cast< ADN_Connector_Vector_ABC* >( pConnector_ );
    std::string imageDir = QDir::tempPath().toStdString() + "_MissionSheets/ImagesTemp" + QString::number( missionType_ ).toStdString() + "/";
    bfs::path imagePath = imageDir + GetModel().item( currentIndex().row() )->text().toStdString();
    if( bfs::exists( imagePath ) )
        bfs::remove( imagePath );
    if( connector )
        connector->RemItem( GetCurrentData() );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_DescriptionAttachment::contextMenuEvent
// Created: NPT 2013-01-17
// -----------------------------------------------------------------------------
void ADN_ListView_DescriptionAttachment::contextMenuEvent( QContextMenuEvent* event )
{
    QMenu* menu = new QMenu( this );
    menu->addAction( tr( "Add File" ), this, SLOT( AddFile() ) );
    QModelIndex index = selectionModel()->currentIndex();
    if( index.isValid() )
        menu->addAction( tr( "Remove File" ),  this, SLOT( RemoveFile() ) );
    menu->exec( event->globalPos() );
}


bool ADN_ListView_DescriptionAttachment::IsFileInList( const QString& fileName )
{
    return dataModel_.FindTextItem( fileName, dataModel_.invisibleRootItem() ) != 0;
}
