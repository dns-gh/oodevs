// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AlarmsWidget_h_
#define __AlarmsWidget_h_

#include "clients_kernel/Observer_ABC.h"
#include "clients_kernel/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
}

class Simulation;

// =============================================================================
/** @class  AlarmsWidget
    @brief  AlarmsWidget
*/
// Created: AGE 2007-05-07
// =============================================================================
class AlarmsWidget : public QDialog
                   , public kernel::Observer_ABC
                   , public kernel::ElementObserver_ABC< Simulation >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             AlarmsWidget( QWidget* parent, kernel::Controllers& controllers, const Simulation& simulation );
    virtual ~AlarmsWidget();
    //@}

    //! @name Operations
    //@{
    //@}

private slots:
    //! @name Slots
    //@{
    void OnCreate();
    void OnDelete();
    void OnSelectionChange( QListViewItem* );
    void ShowEditor( QListViewItem* item );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AlarmsWidget( const AlarmsWidget& );            //!< Copy constructor
    AlarmsWidget& operator=( const AlarmsWidget& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const Simulation& simulation );
    bool IsAfter( QListViewItem* item, int day, const QTime& time );
    QListViewItem* Trigger( QListViewItem* item );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const Simulation& simulation_;
    QListView* list_;
    QPushButton* remove_; 
    QMessageBox* messageBox_;
    //@}
};

#endif // __AlarmsWidget_h_
