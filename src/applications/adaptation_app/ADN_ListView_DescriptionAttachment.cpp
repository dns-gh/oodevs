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
ADN_ListView_DescriptionAttachment::ADN_ListView_DescriptionAttachment( E_MissionType missionType )
    : ADN_ListView( 0, "ADN_ListView_DescriptionAttachment", tr( "Attachments" ) )
    , missionType_( missionType )
    , missionName_( 0 )
{
    pConnector_.reset( new ADN_Connector_ListView< ADN_Missions_ABC::ADN_Missions_Attachment >( *this ) );
    setHeaderHidden( true );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_DescriptionAttachment destructor
// Created: NPT 2013-01-16
// -----------------------------------------------------------------------------
ADN_ListView_DescriptionAttachment::~ADN_ListView_DescriptionAttachment()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_DescriptionAttachment::OnItemSelected
// Created: ABR 2013-09-20
// -----------------------------------------------------------------------------
void ADN_ListView_DescriptionAttachment::OnItemSelected( void* pData )
{
    if( !pData )
    {
        missionName_ = 0;
        return;
    }
    missionName_ = &static_cast< ADN_Missions_ABC* >( pData )->strName_;
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_DescriptionAttachment::GetImageDir
// Created: ABR 2013-10-01
// -----------------------------------------------------------------------------
tools::Path ADN_ListView_DescriptionAttachment::GetImageDir( std::string key ) const
{
    return ADN_Missions_Data::GetTemporaryImagesPath( missionType_ ) / tools::Path::FromUTF8( key );
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
    if( fileName.ToUTF8().find( '$' ) != std::string::npos )
    {
        QMessageBox::warning( this, tr( "Warning" ), tr( "'$' character if forbidden in image's path." ) );
        return;
    }
    if( fileName.IsEmpty() || !missionName_ )
        return;
    const tools::Path imageDir = GetImageDir( missionName_->GetKey() );
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
    ADN_Connector_Vector_ABC& connector = static_cast< ADN_Connector_Vector_ABC& >( *pConnector_ );
    connector.AddItem( new ADN_Missions_ABC::ADN_Missions_Attachment( newFileName.FileName() ) );
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
    if( !missionName_ )
        return;
    ADN_Connector_Vector_ABC& connector = static_cast< ADN_Connector_Vector_ABC& >( *pConnector_ );
    tools::Path imageDir = GetImageDir( missionName_->GetKey() );
    imageDir /= tools::Path::FromUnicode( GetModel().item( currentIndex().row() )->text().toStdWString() );
    if( imageDir.IsRegularFile() )
        imageDir.Remove();
    connector.RemItem( GetCurrentData() );
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
