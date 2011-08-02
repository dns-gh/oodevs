// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AfterActionRequestList_h_
#define __AfterActionRequestList_h_

#include "tools/ElementObserver_ABC.h"
#include "clients_gui/ListDisplayer.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Controllers;
}

class IndicatorRequest;
class IndicatorPlotFactory;
class Simulation;

// =============================================================================
/** @class  AfterActionRequestList
    @brief  AfterActionRequestList
*/
// Created: AGE 2007-09-25
// =============================================================================
class AfterActionRequestList : public Q3VBox
                             , public tools::Observer_ABC
                             , public tools::ElementObserver_ABC< IndicatorRequest >
                             , public tools::ElementObserver_ABC< Simulation >
                             , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             AfterActionRequestList( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, IndicatorPlotFactory& plotFactory );
    virtual ~AfterActionRequestList();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnDoubleClicked( Q3ListViewItem* );
    void OnRequestPopup( Q3ListViewItem* pItem, const QPoint& pos );
    void OnRemoveItem();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyCreated( const IndicatorRequest& request );
    virtual void NotifyUpdated( const IndicatorRequest& request );
    virtual void NotifyUpdated( const Simulation& simulation );
    void Display( const IndicatorRequest& request, gui::ValuedListItem* item );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    gui::ItemFactory_ABC& factory_;
    IndicatorPlotFactory& plotFactory_;
    gui::ListDisplayer< AfterActionRequestList >* requests_;
    QPixmap pendingPixmap_;
    QPixmap donePixmap_;
    QPixmap failedPixmap_;
    Q3PopupMenu popupMenu_;
    //@}
};

#endif // __AfterActionRequestList_h_
