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

#include <tools/ElementObserver_ABC.h>

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
class AfterActionRequestList : public QWidget
                             , public tools::Observer_ABC
                             , public tools::ElementObserver_ABC< IndicatorRequest >
                             , public tools::ElementObserver_ABC< Simulation >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             AfterActionRequestList( QWidget* parent, kernel::Controllers& controllers, IndicatorPlotFactory& plotFactory );
    virtual ~AfterActionRequestList();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnDoubleClicked();
    void OnRequestPopup( const QPoint& pos );
    void OnRemoveItem();
    void OnRename();
    void ClearList();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyCreated( const IndicatorRequest& request );
    virtual void NotifyUpdated( const IndicatorRequest& request );
    virtual void NotifyUpdated( const Simulation& simulation );
    void Display( const IndicatorRequest& request, QTreeWidgetItem* item );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    IndicatorPlotFactory& plotFactory_;
    QTreeWidget* requests_;
    QPixmap pendingPixmap_;
    QPixmap donePixmap_;
    QPixmap failedPixmap_;
    //@}
};

#endif // __AfterActionRequestList_h_
