// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ReportListView_h_
#define __ReportListView_h_

#include "tools/Observer_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "tools/ElementObserver_ABC.h"
#include "gaming/AgentSelectionObserver.h"
#include "reports/Report.h"

namespace gui
{
    class DisplayExtractor;
    class LinkItemDelegate;
}

namespace kernel
{
    class ContextMenu;
    class Controllers;
}

class Reports;

// =============================================================================
/** @class  ReportListView
    @brief  Displays the unit reports and debug traces in a sorted listview.
*/
// Created: APE 2004-03-10
// =============================================================================
class ReportListView : public QTreeView
                     , public tools::Observer_ABC
                     , public AgentSelectionObserver
                     , public tools::ElementObserver_ABC< Reports >
                     , public tools::ElementObserver_ABC< Report >
{
    Q_OBJECT
public:
    //! @name Constructors/Destructor
    //@{
             ReportListView( QWidget* pParent, kernel::Controllers& controllers, gui::DisplayExtractor& extractor );
    virtual ~ReportListView();
    //@}

    //! @name Operations
    //@{
    virtual void showEvent( QShowEvent* );
    virtual void contextMenuEvent ( QContextMenuEvent * e );
    void AddMenuItem( const QString& name, Report::E_Type type ) const;

public slots:
    //! @name Slots
    //@{
    void OnSelectionChanged();
    void OnReadTimerOut();
    void OnClearAll();
    void OnClearTrace();
    void OnRequestCenter();
    void OnToggle( QAction* action );
    void OnLinkClicked( const QString&, const QModelIndex& );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifySelected( const kernel::Entity_ABC* element );
    virtual void NotifyUpdated( const Reports& reports );
    virtual void NotifyCreated( const Report& report );
    bool ShouldUpdate( const Reports& reports );
    void MarkReportsAsRead();
    void SetFilterRegexp();
    QList< QStandardItem* > GetItems( const Report& report ) const;
    //@}

private:
    //! @name Member data
    //@{
    static unsigned int sortOrder_;
    kernel::Controllers& controllers_;
    gui::DisplayExtractor& extractor_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    kernel::ContextMenu* menu_;
    QTimer* readTimer_;
    QStandardItemModel reportModel_;
    QSortFilterProxyModel* proxyFilter_;
    gui::LinkItemDelegate* delegate_;
    std::set< Report::E_Type > toDisplay_;
    //@}
};

#endif // __ReportListView_h_
