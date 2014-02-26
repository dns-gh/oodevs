// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __LogisticsRequestsHistoryTable_h_
#define __LogisticsRequestsHistoryTable_h_

#include "clients_gui/RichTableView.h"

namespace gui
{
    class LinkItemDelegate;
}

// =============================================================================
/** @class  LogisticsRequestsHistoryTable
    @brief  LogisticsRequestsHistoryTable
*/
// Created: MMC 2013-09-11
// =============================================================================
class LogisticsRequestsHistoryTable : public gui::RichTableView
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit LogisticsRequestsHistoryTable( const QString& objectName, QWidget* parent = 0 );
    virtual ~LogisticsRequestsHistoryTable();
    //@}

public:
    //! @name Operations
    //@{
    void Purge();
    void AddRequest( const QString& state, const QString& started, const QString& ended, const QString& handler );
    //@}

    //! @name Accessors
    const gui::LinkItemDelegate* GetLinkItemDelegate() const;
    //@}

private:
    //! @name Helpers
    //@{
    void AddItem( int row, int col, const QString& text );
    bool ItemHasText( int row, int col, const QString& text ) const;
    //@}

private:
    //! @name Data Members
    //@{
    QStandardItemModel* dataModel_;
    gui::LinkItemDelegate* linkItemDelegate_;
    //@}
};

#endif // __LogisticsRequestsHistoryTable_h_
