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
#include "clients_kernel/Observer_ABC.h"
#include "gaming/AgentSelectionObserver.h"
#include "clients_kernel/ElementObserver_ABC.h"

class Controllers;
class Report_ABC;
class ReportFilterOptions;
class Reports;

// =============================================================================
/** @class  ReportListView
    @brief  Displays the unit reports and debug traces in a sorted listview.
*/
// Created: APE 2004-03-10
// =============================================================================
class ReportListView : public ListDisplayer< ReportListView >
                     , public Observer_ABC
                     , public AgentSelectionObserver
                     , public ElementObserver_ABC< Reports > 
                     , public ElementObserver_ABC< Report_ABC >
{
    Q_OBJECT
public:
    //! @name Constructors/Destructor
    //@{
             ReportListView( QWidget* pParent, Controllers& controllers, const ReportFilterOptions& filter, ItemFactory_ABC& factory );
    virtual ~ReportListView();
    //@}

    //! @name Operations
    //@{
    virtual void showEvent( QShowEvent* );
    void Display( const Report_ABC* report, Displayer_ABC& displayer, ValuedListItem* item );
    //@}

public slots:
    //! @name Slots
    //@{
    void OnOptionsChanged();
    //@}

private:
    //! @name Slots
    //@{
    virtual void NotifySelected( const Entity_ABC* element );

    virtual void NotifyUpdated( const Reports& reports );
    virtual void NotifyCreated( const Report_ABC& report );

    bool ShouldUpdate( const Reports& reports );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ReportListView( const ReportListView& );
    ReportListView& operator=( const ReportListView& );
    //@}

    //! @name Member data
    //@{
    Controllers&               controllers_;
    ItemFactory_ABC&           factory_;
    const ReportFilterOptions& filter_;
    const Entity_ABC*           selected_; // $$$$ AGE 2006-04-20: watch deletions !
    //@}
};

#endif // __ReportListView_h_
