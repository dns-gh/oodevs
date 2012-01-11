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

namespace kernel
{
    class Controllers;
}

class Report;
class ReportFilterOptions;
class Reports;

// =============================================================================
/** @class  ReportListView
    @brief  Displays the unit reports and debug traces in a sorted listview.
*/
// Created: APE 2004-03-10
// =============================================================================
class ReportListView : public gui::ListDisplayer< ReportListView >
                     , public tools::Observer_ABC
                     , public AgentSelectionObserver
                     , public tools::ElementObserver_ABC< Reports >
                     , public tools::ElementObserver_ABC< Report >
{
    Q_OBJECT
public:
    //! @name Constructors/Destructor
    //@{
             ReportListView( QWidget* pParent, kernel::Controllers& controllers, const ReportFilterOptions& filter, gui::ItemFactory_ABC& factory );
    virtual ~ReportListView();
    //@}

    //! @name Operations
    //@{
    virtual void showEvent( QShowEvent* );
    void Display( const Report* report, kernel::Displayer_ABC& displayer, gui::ValuedListItem* item );
    void Display( const Report& report, kernel::Displayer_ABC& displayer, gui::ValuedListItem* item );
    //@}

public slots:
    //! @name Slots
    //@{
    void OnOptionsChanged();
    void OnRequestPopup( Q3ListViewItem*, const QPoint&, int );
    void OnSelectionChanged();
    void OnReadTimerOut();
    void OnClearAll();
    void OnClearTrace();
    void OnRequestCenter();
    //@}

private:
    //! @name Slots
    //@{
    virtual void NotifySelected( const kernel::Entity_ABC* element );

    virtual void NotifyUpdated( const Reports& reports );
    virtual void NotifyCreated( const Report& report );

    bool ShouldUpdate( const Reports& reports );

    void MarkReportsAsRead();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ReportListView( const ReportListView& );
    ReportListView& operator=( const ReportListView& );
    //@}

    //! @name Member data
    //@{
    kernel::Controllers&       controllers_;
    gui::ItemFactory_ABC&      factory_;
    const ReportFilterOptions& filter_;

    kernel::SafePointer< kernel::Entity_ABC > selected_;
    kernel::ContextMenu*                               menu_;
    QTimer*                                   readTimer_;
    //@}
};

#endif // __ReportListView_h_
