// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "FileDialog.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: FileDialog::getExistingDirectory
// Created: ABR 2013-03-04
// -----------------------------------------------------------------------------
tools::Path FileDialog::getExistingDirectory( QWidget* parent /*= 0*/, const QString& caption /*= QString()*/,
                                              const tools::Path& dir /*= tools::Path()*/, QFileDialog::Options options /*= QFileDialog::ShowDirsOnly*/,
                                              QFileDialog::FileMode fileMode /* = QFileDialog::AnyFile */ )
{
    QFileDialog dialog( parent );
    dialog.setFileMode( fileMode );
    QString result = dialog.getExistingDirectory( parent, caption, QString::fromStdWString( dir.ToUnicode() ), options );
    return tools::Path::FromUnicode( result.toStdWString() );
}

// -----------------------------------------------------------------------------
// Name: FileDialog::getOpenFileName
// Created: ABR 2013-03-04
// -----------------------------------------------------------------------------
tools::Path FileDialog::getOpenFileName( QWidget* parent /*= 0*/, const QString& caption /*= QString()*/,
                                         const tools::Path& dir /*= tools::Path()*/, const QString& filter /*= QString()*/,
                                         QString* selectedFilter /*= 0*/, QFileDialog::Options options /*= 0*/,
                                         QFileDialog::FileMode fileMode /* = QFileDialog::AnyFile */ )
{
    QFileDialog dialog( parent );
    dialog.setFileMode( fileMode );
    QString result = dialog.getOpenFileName( parent, caption, QString::fromStdWString( dir.ToUnicode() ), filter, selectedFilter, options );
    return tools::Path::FromUnicode( result.toStdWString() );
}

// -----------------------------------------------------------------------------
// Name: FileDialog::getOpenFileNames
// Created: ABR 2013-03-04
// -----------------------------------------------------------------------------
tools::Path::T_Paths FileDialog::getOpenFileNames( QWidget* parent /*= 0*/, const QString& caption /*= QString()*/,
                                                   const tools::Path& dir /*= tools::Path()*/, const QString& filter /*= QString()*/,
                                                   QString* selectedFilter /*= 0*/, QFileDialog::Options options /*= 0*/,
                                                   QFileDialog::FileMode fileMode /* = QFileDialog::AnyFile */ )
{
    QFileDialog dialog( parent );
    dialog.setFileMode( fileMode );
    QStringList result = dialog.getOpenFileNames( parent, caption, QString::fromStdWString( dir.ToUnicode() ), filter, selectedFilter, options );
    tools::Path::T_Paths pathList;
    for( auto it = result.begin(); it != result.end(); ++it )
        pathList.push_back( tools::Path::FromUnicode( it->toStdWString() ) );
    return pathList;
}

// -----------------------------------------------------------------------------
// Name: FileDialog::getSaveFileName
// Created: ABR 2013-03-04
// -----------------------------------------------------------------------------
tools::Path FileDialog::getSaveFileName( QWidget* parent /*= 0*/, const QString& caption /*= QString()*/,
                                         const tools::Path& dir /*= tools::Path()*/, const QString& filter /*= QString()*/,
                                         QString* selectedFilter /*= 0*/, QFileDialog::Options options /*= 0*/,
                                         QFileDialog::FileMode fileMode /* = QFileDialog::AnyFile */ )
{
    QFileDialog dialog( parent );
    dialog.setFileMode( fileMode );
    QString result = dialog.getSaveFileName( parent, caption, QString::fromStdWString( dir.ToUnicode() ), filter, selectedFilter, options );
    return tools::Path::FromUnicode( result.toStdWString() );
}
