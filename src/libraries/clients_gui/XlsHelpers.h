// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef ___gui_XlsHelpers_h_
#define ___gui_XlsHelpers_h_

namespace tools
{
    class Path;
}

namespace gui
{
    // Serialize a list of tableviews into a list of xsl sheets in an xsl file (output).
    // Each view must owns a QStandardItemModel or a QAbstractProxyModel linked to a
    // QStandardItemModel. An exception will be raised otherwise.
    // If one tableview's row count exceed numeric_limits< USHORT >::max(), an exception is raised.
    // If sheetNames are provided, they are used for each xsl sheet.
    // If headerColor is provided and valid, the tableview's horizontal header is written on the first row,
    // with bold border and with the given color as background.
    // If colors are provided, they override the excel default palette. This palette cannot
    // contains more than 55 colors, according to our actual excel library. If more are provided
    // they'll be ignored.
    // Each QStandardItem from the qviews can store in a dedicated role (gui::Roles::ExcelColorRole)
    // the index of the it's color in the palette. If this index is out of range, an exception is raised.
    void QTablesToXls( const tools::Path& output,
                       const QList< QTableView* >& views,
                       const QStringList& sheetNames = QStringList(),
                       const QColor& headerColor = QColor(),
                       const QList< QColor >& colors = QList< QColor >() );
}

#endif // ___gui_XlsHelpers_h_
