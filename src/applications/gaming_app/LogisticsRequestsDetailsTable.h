// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __LogisticsRequestsDetailsTable_h_
#define __LogisticsRequestsDetailsTable_h_

#include "clients_kernel/Tools.h"
#include "clients_gui/CommonDelegate.h"
#include "clients_gui/RichWidget.h"
#include "QTableView.h"

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
/** @class  LogisticsRequestsDetailsTable
    @brief  LogisticsRequestsDetailsTable
*/
// Created: MMC 2013-09-11
// =============================================================================
class LogisticsRequestsDetailsTable : public gui::RichWidget< QTableView >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             LogisticsRequestsDetailsTable( const QString& objectName, QWidget* parent = 0 );
    virtual ~LogisticsRequestsDetailsTable();
    //@}

public:
    //! @name Operations
    //@{
    virtual void Purge();
    //@}

    //! @name Accessors
    const gui::LinkItemDelegate* GetLinkItemDelegate() const;
    void Add( const QString& title, const QString& value );
    void Set( const QString& title, const QString& value );
    //@}

protected:
    //! @name Data Members
    //@{
    QStandardItemModel*     dataModel_;
    QSortFilterProxyModel*  proxyModel_;
    gui::CommonDelegate*    delegate_;
    gui::LinkItemDelegate*  linkItemDelegate_;
    int                     detailIndex_;
    //@}
};

#endif // __LogisticsRequestsDetailsTable_h_
