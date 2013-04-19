// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Units_Groups_GUI_h_
#define __ADN_Units_Groups_GUI_h_

#include "ADN_Types.h"

class GroupsDelegate;

class ADN_Units_Groups_GUI : public QTableView
{
    Q_OBJECT

public:
    explicit ADN_Units_Groups_GUI( const Q3Table& model, QWidget* parent = 0 );
    virtual ~ADN_Units_Groups_GUI();

public slots:
    void Commit() const;

private:
    virtual void contextMenuEvent( QContextMenuEvent* event );

    int AddGroup( const QString& name, int rows );
    void AddItem( int row, int column, const QString& text, void* data = 0 );

    QString InputName( const QString& label, const QString& previous );

private:
    QStandardItemModel dataModel_;
    QSortFilterProxyModel proxyModel_;
    std::auto_ptr< GroupsDelegate > delegate_;
};

#endif // __ADN_Units_Groups_GUI_h_
