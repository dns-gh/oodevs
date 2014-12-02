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

#include "clients_kernel/SafePointer.h"
#include "gaming/AgentSelectionObserver.h"
#include "gaming/ReportsModel.h"
#include "reports/Report.h"
#include <tools/Observer_ABC.h>

namespace gui
{
    class DisplayExtractor;
    class LinkItemDelegate;
}

namespace kernel
{
    class Controllers;
    class Time_ABC;
}

class AgentsModel;
class Publisher_ABC;
class ReportFactory;
class ReportsModel;

// =============================================================================
/** @class  ReportListView
    @brief  Displays the unit reports and debug traces in a sorted listview.
*/
// Created: APE 2004-03-10
// =============================================================================
class ReportListView : public QTreeView
                     , public tools::Observer_ABC
                     , public AgentSelectionObserver
{
    Q_OBJECT
public:
    //! @name Constructors/Destructor
    //@{
             ReportListView( QWidget* pParent, kernel::Controllers& controllers, gui::DisplayExtractor& extractor,
                             const ReportFactory& factory, Publisher_ABC& publisher, ReportsModel& model,
                             const AgentsModel& agents, const kernel::Time_ABC& time );
    virtual ~ReportListView();
    //@}

    //! @name Operations
    //@{
    virtual void showEvent( QShowEvent* );
    virtual void contextMenuEvent ( QContextMenuEvent * e );
    void Purge();
    //@}

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
    void CreateItem( const ReportsModel::Message& message, const kernel::Entity_ABC& entity, bool unreadMessages );
    template< typename T >
    void Create( const T& report );
    void FillReports();
    void AddReports();
    void AddMenuItem( QMenu* menu, const QString& name, Report::E_Type type ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    gui::DisplayExtractor& extractor_;
    unsigned int selected_;
    QTimer* readTimer_;
    QStandardItemModel reportModel_;
    gui::LinkItemDelegate* delegate_;
    std::set< Report::E_Type > toDisplay_;
    const ReportFactory& factory_;
    ReportsModel& model_;
    const AgentsModel& agents_;
    const kernel::Time_ABC& time_;
    //@}
};

#endif // __ReportListView_h_
