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
#include "clients_gui/RichTableView.h"

namespace actions
{
    class ActionsModel;
}

namespace kernel
{
    class Entity_ABC;
    class Controllers;
    class Profile_ABC;
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
class LogisticsRequestsTable : public gui::RichTableView
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             LogisticsRequestsTable( const QString& objectName,
                                     QWidget* parent,
                                     const QStringList& horizontalHeaders,
                                     const kernel::Controllers& controllers,
                                     const kernel::Profile_ABC& profile );
    virtual ~LogisticsRequestsTable();
    //@}

public:
    //! @name Operations
    //@{
    virtual void Purge();
    virtual void AddRequest( const LogisticsConsign_ABC& consign, unsigned int id,
                             const QString& requester, const QString& handler, const QString& state );
    virtual void SelectRequest( unsigned int id );
    virtual const LogisticsConsign_ABC* GetRequest( const QModelIndex& index ) const;
    const LogisticsConsign_ABC* GetCurrentRequest() const;
    void FindRequestsIds( std::set< unsigned int >& requests );
    void SetData( int col, QString text, const LogisticsConsign_ABC& consign );
    //@}

protected:
    //! @name Helpers
    //@{
     int GetRequestRow( const LogisticsConsign_ABC& consign );
     void SetData( int row, int col, QString displayText,
                   QVariant sortText, const LogisticsConsign_ABC& consign );
    //@}

signals:
    //! @name Signals
    //@{
     void RequestSelectionDialog( const LogisticsConsign_ABC& consign );
    //@}

public slots:
    //! @name Slots
    //@{
    void OnLinkClicked( const QString& url, const QModelIndex& index );
    //@}

protected:
    //! @name Data Members
    //@{
    QStandardItemModel         dataModel_;
    QSortFilterProxyModel*     proxyModel_;
    gui::CommonDelegate        delegate_;
    gui::LinkItemDelegate*     linkItemDelegate_;
    QStringList                horizontalHeaders_;
    const kernel::Controllers& controllers_;
    const kernel::Profile_ABC& profile_;
    //@}
};

#endif // __LogisticsRequestsTable_h_
