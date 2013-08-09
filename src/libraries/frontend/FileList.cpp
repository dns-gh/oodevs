// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "frontend_pch.h"
#include "FileList.h"
#include "moc_FileList.cpp"

#include "clients_gui/FileDialog.h"

#pragma warning( push, 0 )
#include <QtGui/QPushButton>
#include <QtGui/QListWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtCore/QStringList>
#pragma warning( pop )

#include <boost/algorithm/string.hpp>

namespace frontend
{

FileList::FileList(const QString& title, QWidget* parent, const QString& caption, const QString& fileFilter)
    : BaseClass(title, parent)
    , lastDir_(".")
    , caption_(caption)
    , fileFilter_(fileFilter)
{
    listWidget_ = new QListWidget(this);
    listWidget_->setFont( QFont( "Calibri", 12, QFont::Bold ) );
    listWidget_->setSelectionMode(QAbstractItemView::ExtendedSelection);
    addButton_ = new QPushButton(tr("Add"), this );
    removeButton_ = new QPushButton(tr("Remove"), this );

    QVBoxLayout* layoutV = new QVBoxLayout;
    layoutV->addWidget(listWidget_);
    QHBoxLayout* layoutH = new QHBoxLayout;
    layoutH->addWidget(addButton_);
    layoutH->addWidget(removeButton_);

    layoutV->addLayout(layoutH);

    this->setLayout(layoutV);

    connect(listWidget_, SIGNAL(itemSelectionChanged()), SLOT(SelectionChanged()));
    connect(addButton_, SIGNAL(clicked()), SLOT(OnAddClicked()));
    connect(removeButton_, SIGNAL(clicked()), SLOT(OnRemoveClicked()));

    SelectionChanged();
}

FileList::~FileList()
{
}

void FileList::SelectionChanged()
{
    removeButton_->setEnabled(listWidget_->currentItem() != NULL);
}

void FileList::OnAddClicked()
{
    tools::Path::T_Paths files = gui::FileDialog::getOpenFileNames( this, caption_, lastDir_, fileFilter_, 0, 0, QFileDialog::ExistingFiles );
    if ( !files.empty() )
    {
        lastDir_ = files.back().Parent();

        for( int i = 0; i < listWidget_->count(); ++i )
            files.push_back( tools::Path::FromUnicode( listWidget_->item(i)->text().toStdWString() ) );

        std::sort( files.begin(), files.end() );
        files.erase( std::unique( files.begin(), files.end() ), files.end() );

        listWidget_->clear();
        for( auto it = files.begin(); it != files.end(); ++it )
            listWidget_->addItem( QString::fromStdWString( it->ToUnicode() ) );
    }
}

void FileList::OnRemoveClicked()
{
    QList< QListWidgetItem* > selectedItems = listWidget_->selectedItems();
    for( int i = 0; i < selectedItems.size(); ++i )
        delete listWidget_->takeItem( listWidget_->row( selectedItems.at( i ) ) );
}

void FileList::GetFiles( FileVec_T& filesToFill ) const
{
    filesToFill.reserve( listWidget_->count() );
    for( int i = 0; i < listWidget_->count(); ++i)
        filesToFill.push_back( listWidget_->item( i )->text().toStdString() );
}

void FileList::SetFiles( const FileVec_T& files )
{
    listWidget_->clear();
    QStringList itemsToAdd;

    itemsToAdd.reserve( static_cast< int >( files.size() ) );
    for( unsigned int i = 0; i < files.size(); ++i )
    {
        itemsToAdd.append( tr( files[ i ].c_str() ) );
    }

    listWidget_->addItems( itemsToAdd );
}

void FileList::SetFilesDelimited( const std::string& files, const std::string& delimiters )
{
    FileVec_T fileV;
    boost::split( fileV, files, boost::is_any_of( delimiters ) );
    SetFiles( fileV );
}

std::string FileList::GetFilesDelimited( const std::string& delimiter ) const
{ 
    FileVec_T fileV;
    GetFiles( fileV );
    std::string result;
    // reserve with a guess of 15 characters per file.
    result.reserve( fileV.size() * (15 + delimiter.length() ) );
    for( FileVec_T::iterator i = fileV.begin(); i != fileV.end(); ++i )
    {
        if ( !result.empty() ) 
            result += delimiter;
        result += *i;
    }
    return result;
}

void FileList::OnLanguageChanged()
{
    //NOTHING
}
}
