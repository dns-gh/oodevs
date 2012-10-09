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

#include "clients_gui/ListDisplayer.h"
#include "tools/Observer_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "tools/ElementObserver_ABC.h"
#include "gaming/AgentSelectionObserver.h"
#include "reports/Report.h"

namespace kernel
{
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
             ReportListView( QWidget* pParent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~ReportListView();
    //@}

    //! @name Operations
    //@{
    virtual void showEvent( QShowEvent* );
    virtual void contextMenuEvent ( QContextMenuEvent * e );
    void AddMenuItem( const QString& name, Report::E_Type type ) const;
    void AddContextMenu() const;

public slots:
    //! @name Slots
    //@{
    void OnRequestPopup( const QPoint& pos );
    void OnSelectionChanged();
    void OnReadTimerOut();
    void OnClearAll();
    void OnClearTrace();
    void OnRequestCenter();
    void OnToggle( QAction* action );
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
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    gui::ItemFactory_ABC& factory_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    kernel::ContextMenu* menu_;
    QTimer* readTimer_;
    QStandardItemModel reportModel_;
    QSortFilterProxyModel* proxyFilter_;
    std::set< Report::E_Type > toDisplay_;
    //@}
};

#endif // __ReportListView_h_
