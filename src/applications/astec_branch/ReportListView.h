// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-03-10 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ReportListView.h $
// $Author: Ape $
// $Modtime: 4/10/04 15:42 $
// $Revision: 2 $
// $Workfile: ReportListView.h $
//
// *****************************************************************************

#ifndef __ReportListView_h_
#define __ReportListView_h_

#include "ListDisplayer.h"
#include "Observer_ABC.h"
#include "AgentSelectionObserver.h"
#include "ElementObserver_ABC.h"

class Agent_ABC;
class Report_ABC;
class ActionContext;
class SelectedElement;
class ReportFilterOptions;
class Reports;

class Controllers;

// =============================================================================
/** @class  ReportListView
    @brief  Displays the unit reports and debug traces in a sorted listview.
*/
// Created: APE 2004-03-10
// =============================================================================
class ReportListView : public ListDisplayer< ReportListView >
                     , private Observer_ABC
                     , public AgentSelectionObserver
                     , public ElementObserver_ABC< Reports > 
                     , public ElementObserver_ABC< Report_ABC >
{
    Q_OBJECT
public:
    //! @name Constructors/Destructor
    //@{
             ReportListView( QWidget* pParent, Controllers& controllers, const ReportFilterOptions& filter );
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
    virtual void NotifySelected( const Agent_ABC* element );

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
    const ReportFilterOptions& filter_;
    const Agent_ABC*           selected_;
    //@}
};

#endif // __ReportListView_h_
