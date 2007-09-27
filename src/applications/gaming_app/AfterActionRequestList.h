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

#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_gui/ListDisplayer.h"

namespace kernel
{
    class Controllers;
}

class AfterActionRequest;
class AfterActionPlot;
class Publisher_ABC;

// =============================================================================
/** @class  AfterActionRequestList
    @brief  AfterActionRequestList
*/
// Created: AGE 2007-09-25
// =============================================================================
class AfterActionRequestList : public QVBox
                             , public kernel::Observer_ABC
                             , public kernel::ElementObserver_ABC< AfterActionRequest > 
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             AfterActionRequestList( QWidget* parent, QMainWindow* mainWindow, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, Publisher_ABC& publisher );
    virtual ~AfterActionRequestList();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnDoubleClicked( QListViewItem* );
    //@}    

private:
    //! @name Copy/Assignment
    //@{
    AfterActionRequestList( const AfterActionRequestList& );            //!< Copy constructor
    AfterActionRequestList& operator=( const AfterActionRequestList& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const AfterActionRequest& request );
    virtual void NotifyUpdated( const AfterActionRequest& request );
    void Display( const AfterActionRequest& request, gui::ValuedListItem* item );
    AfterActionPlot* CreateNewPlot();
    //@}

private:
    //! @name Member data
    //@{
    QMainWindow* mainWindow_;
    kernel::Controllers& controllers_;
    gui::ItemFactory_ABC& factory_;
    Publisher_ABC& publisher_;
    gui::ListDisplayer< AfterActionRequestList >* requests_;
    QPixmap pendingPixmap_;
    QPixmap donePixmap_;
    QPixmap failedPixmap_;
    //@}
};

#endif // __AfterActionRequestList_h_
