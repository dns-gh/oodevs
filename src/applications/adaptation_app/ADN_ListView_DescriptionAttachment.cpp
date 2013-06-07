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
#include "clients_gui/FileDialog.h"
#include "ADN_Project_Data.h"
#include "ADN_Missions_ABC.h"
#include "ADN_Missions_Data.h"
#include <boost/lexical_cast.hpp>

// -----------------------------------------------------------------------------
// Name: ADN_ListView_DescriptionAttachment constructor
// Created: NPT 2013-01-16
// -----------------------------------------------------------------------------
ADN_ListView_DescriptionAttachment::ADN_ListView_DescriptionAttachment( E_MissionType missionType, ADN_EditLine_ABC* missionName )
    : ADN_ListView( 0, "ADN_ListView_DescriptionAttachment", tr( "Attachments" ) )
    , missionType_( missionType )
    , missionName_( missionName )
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
    tools::Path fileName = gui::FileDialog::getOpenFileName( this
        , tr( "Select an image" )
        , "."
        , tr( "All supported formats (*.bmp *.jpg *.jpeg *.gif *.png)\n"
        "Bitmap Files (*.bmp)\n"
        "JPEG (*.jpg *jpeg)\n"
        "GIF (*.gif)\n"
        "PNG (*.png)\n" )
        );
    if( fileName.IsEmpty() )
        return;
    tools::Path namePath = tools::Path::FromUnicode( missionName_->text().replace( "\'", " ").toStdWString() );
    tools::Path imageDir = ( tools::Path::TemporaryPath() / ADN_Missions_Data::imageTemporaryPath_ + boost::lexical_cast< std::string >( missionType_ ).c_str() ) / namePath;
    if( !imageDir.IsDirectory() )
        imageDir.CreateDirectories();
    tools::Path newFileName = imageDir / fileName.FileName();
    if( newFileName.Exists() )
    {
        QMessageBox* box = new QMessageBox();
        box->setWindowTitle(  tr( "Existing file found" ) );
        box->setText( tr( "The file you want to copy already exists in the base, your file will be renamed" )  );
        box->setModal( true );
        box->show();
        tools::Path tempFileName = newFileName;
        for( int i = 1; tempFileName.Exists(); ++i )
        {
            tempFileName = imageDir / newFileName.BaseName() + boost::lexical_cast< std::string >( i ).c_str() + newFileName.Extension();
            i++;
        }
        newFileName = tempFileName;
    }
    fileName.Copy( newFileName, tools::Path::OverwriteIfExists );
    ADN_Connector_Vector_ABC* connector = static_cast< ADN_Connector_Vector_ABC* >( pConnector_ );
    if( connector )
        connector->AddItem( new ADN_Missions_ABC::ADN_Missions_Attachment( newFileName.FileName() ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_DescriptionAttachment::CopyName
// Created: NPT 2013-04-30
// -----------------------------------------------------------------------------
void ADN_ListView_DescriptionAttachment::CopyName()
{
    QModelIndex index = selectionModel()->currentIndex();
    if( index.isValid() )
    {
        QStandardItem* item = dataModel_.GetItemFromIndex( index );
        if( item )
            QApplication::clipboard()->setText( item->text() );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_DescriptionAttachment::RemoveFile
// Created: NPT 2013-01-16
// -----------------------------------------------------------------------------
void ADN_ListView_DescriptionAttachment::RemoveFile()
{
    ADN_Connector_Vector_ABC* connector = static_cast< ADN_Connector_Vector_ABC* >( pConnector_ );
    tools::Path namePath = tools::Path::FromUnicode( missionName_->text().replace( "\'", " ").toStdWString() );
    tools::Path imageDir = ( tools::Path::TemporaryPath() / ADN_Missions_Data::imageTemporaryPath_ + boost::lexical_cast< std::string >( missionType_ ).c_str() ) / namePath;
    imageDir /= tools::Path::FromUnicode( GetModel().item( currentIndex().row() )->text().toStdWString() );
    if( imageDir.Exists() )
        imageDir.Remove();
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
    {
        menu->addAction( tr( "Copy name" ), this, SLOT( CopyName() ) );
        menu->addAction( tr( "Remove File" ), this, SLOT( RemoveFile() ) );
    }
    menu->exec( event->globalPos() );
}


bool ADN_ListView_DescriptionAttachment::IsFileInList( const QString& fileName )
{
    return dataModel_.FindTextItem( fileName, dataModel_.invisibleRootItem() ) != 0;
}
