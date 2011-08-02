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

#include "tools/Observer_ABC.h"
#include "tools/ElementObserver_ABC.h"

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
                   , public tools::Observer_ABC
                   , public tools::ElementObserver_ABC< Simulation >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             AlarmsWidget( QWidget* parent, kernel::Controllers& controllers, const Simulation& simulation );
    virtual ~AlarmsWidget();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnCreate();
    void OnDelete();
    void OnSelectionChange( Q3ListViewItem* );
    void ShowEditor( Q3ListViewItem* item );
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
    bool IsAfter( Q3ListViewItem* item, const QDateTime& date );
    Q3ListViewItem* Trigger( Q3ListViewItem* item );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const Simulation& simulation_;
    Q3ListView* list_;
    QPushButton* remove_;
    QMessageBox* messageBox_;
    //@}
};

#endif // __AlarmsWidget_h_
