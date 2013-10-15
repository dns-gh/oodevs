// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __FileList_h_
#define __FileList_h_

#include <boost/noncopyable.hpp>
#include "clients_gui/WidgetLanguageObserver_ABC.h"
#include <QtGui/QGroupBox>
#include <vector>
#include <string>


class QListWidget;
class QPushButton;

namespace frontend
{

class FileList : public gui::WidgetLanguageObserver_ABC< QGroupBox >
                   , private boost::noncopyable
{
    Q_OBJECT;
public:
    typedef gui::WidgetLanguageObserver_ABC< QGroupBox > BaseClass;
    typedef std::vector<std::string> FileVec_T;

            FileList(const QString& title, QWidget* parent, const QString& caption, const QString& fileFilter);
    virtual ~FileList();

    //! @name Operations
    //@{
    void GetFiles( FileVec_T& filesToFill ) const;
    void SetFiles( const FileVec_T& files );
    void SetFilesDelimited( const std::string& files, const std::string& delimiters = "," );
    std::string GetFilesDelimited( const std::string& delimiter = "," ) const;
    virtual void OnLanguageChanged();
    //@}

protected slots:

    //! @name Operations
    //@{
    void SelectionChanged();
    void OnAddClicked();
    void OnRemoveClicked();
    //@}

private:
    QListWidget* listWidget_;
    QPushButton* addButton_;
    QPushButton* removeButton_;
    tools::Path lastDir_;
    QString caption_;
    QString fileFilter_;
};
}
#endif