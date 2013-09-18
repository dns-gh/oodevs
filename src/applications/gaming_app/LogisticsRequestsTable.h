// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __LogisticsRequestsTable_h_
#define __LogisticsRequestsTable_h_

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
/** @class  LogisticsRequestsTable
    @brief  LogisticsRequestsTable
*/
// Created: MMC 2013-09-11
// =============================================================================
class LogisticsRequestsTable : public gui::RichWidget< QTableView >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             LogisticsRequestsTable( const QString& objectName, QWidget* parent, const QStringList& horizontalHeaders, const QString filter );
    virtual ~LogisticsRequestsTable();
    //@}

public:
    //! @name Operations
    //@{
    virtual void Purge();
    virtual void AddRequest( const LogisticsConsign_ABC& consign, const QString& id, 
                             const QString& requester, const QString& handler, const QString& state );
    virtual void SelectRequest( const LogisticsConsign_ABC* consign = 0 );
    virtual const LogisticsConsign_ABC* GetRequest( const QModelIndex& index ) const;
    const LogisticsConsign_ABC* GetCurrentRequest() const;
    void SetFilterActivated( bool activated );
    void FindRequestsIds( std::set< unsigned int >& requests );
    //@}

    //! @name Accessors
    const gui::LinkItemDelegate* GetLinkItemDelegate() const;
    //@}

protected:
    //! @name Helpers
    //@{
     int GetRequestRow( const LogisticsConsign_ABC& consign );
     void SetData( int row, int col, QString text, const LogisticsConsign_ABC& consign );
    //@}

protected:
    //! @name Data Members
    //@{
    QStandardItemModel      dataModel_;
    QSortFilterProxyModel*  proxyModel_;
    gui::CommonDelegate     delegate_;
    gui::LinkItemDelegate*  linkItemDelegate_;
    QStringList             horizontalHeaders_;
    //@}
};

#endif // __LogisticsRequestsTable_h_
