// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __LogisticsRequestsHistoryTable_h_
#define __LogisticsRequestsHistoryTable_h_

#include "clients_kernel/Tools.h"
#include "clients_gui/CommonDelegate.h"
#include "clients_gui/RichTableView.h"

namespace kernel
{
    class Entity_ABC;
}

namespace gui
{
    class LinkItemDelegate;
}

class LogisticsConsign_ABC;

// =============================================================================
/** @class  LogisticsRequestsHistoryTable
    @brief  LogisticsRequestsHistoryTable
*/
// Created: MMC 2013-09-11
// =============================================================================
class LogisticsRequestsHistoryTable : public gui::RichTableView
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             LogisticsRequestsHistoryTable( const QString& objectName, QWidget* parent = 0 );
    virtual ~LogisticsRequestsHistoryTable();
    //@}

public:
    //! @name Operations
    //@{
    virtual void Purge();
    virtual void AddRequest( const QString& state, const QString& started, const QString& ended, const QString& handler );
    //@}

    //! @name Accessors
    const gui::LinkItemDelegate* GetLinkItemDelegate() const;
    //@}

protected:
    //! @name Helpers
    //@{
    void AddItem( int row, int col, QString text );
    //@}

protected:
    //! @name Data Members
    //@{
    QStandardItemModel*     dataModel_;
    QSortFilterProxyModel*  proxyModel_;
    gui::CommonDelegate*    delegate_;
    gui::LinkItemDelegate*  linkItemDelegate_;
    QStringList             horizontalHeaders_;
    //@}
};

#endif // __LogisticsRequestsHistoryTable_h_
