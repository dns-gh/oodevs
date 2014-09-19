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

namespace kernel
{
    class Entity_ABC;
}

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
    void AddRequest( const QString& state,
                     const QString& started,
                     const QString& ended,
                     const QString& handlerName,
                     const kernel::Entity_ABC* handler );
    void UpdateHandler( const kernel::Entity_ABC& entity, const QString& name );
    //@}

    //! @name Accessors
    const gui::LinkItemDelegate* GetLinkItemDelegate() const;
    //@}

private:
    //! @name Helpers
    //@{
    QStandardItem* AddItem( int row, int col, const QString& text );
    bool ItemHasText( int row, int col, const QString& text ) const;
    //@}

private:
    //! @name Data Members
    //@{
    gui::LinkItemDelegate* linkItemDelegate_;
    //@}
};

#endif // __LogisticsRequestsHistoryTable_h_
