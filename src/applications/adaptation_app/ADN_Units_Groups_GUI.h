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
#include "clients_gui/CommonDelegate.h"
#include <boost/shared_ptr.hpp>
#include <string>

class GroupsHeader;
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

    int AddGroup( const std::string& name, int rows );
    void RemoveGroup( int column );

    void AddItem( int row, int column, const QString& text, void* data = 0 );

private:
    GroupsHeader* header_;
    QStandardItemModel dataModel_;
    QSortFilterProxyModel proxyModel_;
    std::auto_ptr< GroupsDelegate > delegate_;
};

#endif // __ADN_Units_Groups_GUI_h_
