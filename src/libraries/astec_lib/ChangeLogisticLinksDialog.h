// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ChangeLogisticLinksDialog_h_
#define __ChangeLogisticLinksDialog_h_

#include "ValuedComboBox.h"
#include "SafePointer.h"
#include "ElementObserver_ABC.h"
#include "ContextMenuObserver_ABC.h"
#include <qdialog.h>

class Controllers;
class Agent;
class Publisher_ABC;

// =============================================================================
/** @class  ChangeLogisticLinksDialog
    @brief  ChangeLogisticLinksDialog
*/
// $$$$ SBO 2006-06-30: gérer les teams
// Created: SBO 2006-06-30
// =============================================================================
class ChangeLogisticLinksDialog : public QDialog
                                , public Observer_ABC
                                , public ElementObserver_ABC< Agent >
                                , public ContextMenuObserver_ABC< Agent >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ChangeLogisticLinksDialog( QWidget* parent, Controllers& controllers, Publisher_ABC& publisher );
    virtual ~ChangeLogisticLinksDialog();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyCreated( const Agent& agent );
    virtual void NotifyDeleted( const Agent& agent );
    virtual void NotifyContextMenu( const Agent& agent, QPopupMenu& menu );
    //@}

private slots:
    //! @name Slots
    //@{
    void Show();
    void Validate();
    void Reject();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ChangeLogisticLinksDialog( const ChangeLogisticLinksDialog& );            //!< Copy constructor
    ChangeLogisticLinksDialog& operator=( const ChangeLogisticLinksDialog& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    Controllers& controllers_;
    Publisher_ABC& publisher_;
    ValuedComboBox< const Agent* >* tc2Combo_;
    ValuedComboBox< const Agent* >* maintenanceCombo_;
    ValuedComboBox< const Agent* >* medicalCombo_;
    ValuedComboBox< const Agent* >* supplyCombo_;
    SafePointer< Agent > selected_;
    //@}
};

#endif // __ChangeLogisticLinksDialog_h_
