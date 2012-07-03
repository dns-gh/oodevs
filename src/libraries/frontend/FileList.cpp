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

#include <QtGui/QPushButton>
#include <QtGui/QListWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtCore/QStringList>
#include <QtGui/QFileDialog>

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
    listWidget_->setSelectionMode(QAbstractItemView::ExtendedSelection);
    addButton_ = new QPushButton(tr("Add"), this, "add");
    removeButton_ = new QPushButton(tr("Remove"), this, "remove");

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
    QFileDialog* fdialog = new QFileDialog(this, caption_, lastDir_, fileFilter_);
    fdialog->setMode(QFileDialog::ExistingFiles);

    QStringList files = fdialog->getOpenFileNames(); 
    if (!files.isEmpty())
    {
        lastDir_ = fdialog->directory().absolutePath();

        for (int i = 0; i < listWidget_->count(); ++i)
        {
            files.append(listWidget_->item(i)->text());
        }

        files.sort();
        files.removeDuplicates();

        listWidget_->clear();
        listWidget_->addItems(files);
    }

    delete fdialog;
}

void FileList::OnRemoveClicked()
{
    QList<QListWidgetItem*> selectedItems = listWidget_->selectedItems();
    for (int i = 0; i < selectedItems.size(); ++i)
    {
        delete listWidget_->takeItem(listWidget_->row(selectedItems.at(i)));
    }
}

void FileList::GetFiles(FileVec_T& filesToFill) const
{
    filesToFill.reserve(listWidget_->count());
    for (int i = 0; i < listWidget_->count(); ++i)
    {
        filesToFill.push_back(listWidget_->item(i)->text().toStdString());
    }
}

void FileList::SetFiles(const FileVec_T& files)
{
    listWidget_->clear();
    QStringList itemsToAdd;

    itemsToAdd.reserve(files.size());
    for (unsigned i = 0; i < files.size(); ++i)
    {
        itemsToAdd.append(tr(files[i].c_str()));
    }

    listWidget_->addItems(itemsToAdd);
}

void FileList::SetFilesDelimited(const std::string& files, const std::string& delimiters)
{
    FileVec_T fileV;
    boost::split(fileV, files, boost::is_any_of(delimiters));
    SetFiles(fileV);
}

std::string FileList::GetFilesDelimited(const std::string& delimiter) const
{
    FileVec_T fileV;
    GetFiles(fileV);
    std::string result;
    // reserve with a guess of 15 characters per file.
    result.reserve(fileV.size() * (15 + delimiter.length()));
    FileVec_T::iterator i, iend;
    i = fileV.begin();
    iend = fileV.end();

    for (; i != iend; ++i)
    {
        if (!result.empty()) 
            result += delimiter;

        result += *i;        
    }
    return result;
}

void FileList::OnLanguageChanged()
{
}


}
