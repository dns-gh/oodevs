// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __FileDialog_h_
#define __FileDialog_h_

#include <boost/noncopyable.hpp>

namespace gui
{
    namespace FileDialog
    {
        tools::Path getExistingDirectory( QWidget* parent = 0, const QString& caption = QString(),
                                          const tools::Path& dir = tools::Path(), QFileDialog::Options options = QFileDialog::ShowDirsOnly,
                                          QFileDialog::FileMode fileMode = QFileDialog::AnyFile );
        tools::Path getOpenFileName( QWidget* parent = 0, const QString& caption = QString(),
                                     const tools::Path& dir = tools::Path(), const QString& filter = QString(),
                                     QString* selectedFilter = 0, QFileDialog::Options options = 0,
                                     QFileDialog::FileMode fileMode = QFileDialog::AnyFile );
        tools::Path::T_Paths getOpenFileNames( QWidget* parent = 0, const QString& caption = QString(),
                                               const tools::Path& dir = tools::Path(), const QString& filter = QString(),
                                               QString* selectedFilter = 0, QFileDialog::Options options = 0,
                                               QFileDialog::FileMode fileMode = QFileDialog::AnyFile );
        tools::Path getSaveFileName( QWidget* parent = 0, const QString& caption = QString(),
                                     const tools::Path& dir = tools::Path(), const QString& filter = QString(),
                                     QString* selectedFilter = 0, QFileDialog::Options options = 0,
                                     QFileDialog::FileMode fileMode = QFileDialog::AnyFile );
    }
}

#endif // __FileDialog_h_
