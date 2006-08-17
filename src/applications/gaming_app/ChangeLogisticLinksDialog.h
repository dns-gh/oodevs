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

#include "clients_kernel/SafePointer.h"
#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_gui/ValuedComboBox.h"

class Controllers;
class Agent_ABC;
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
                                , public ElementObserver_ABC< Agent_ABC >
                                , public ContextMenuObserver_ABC< Agent_ABC >
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
    virtual void NotifyCreated( const Agent_ABC& agent );
    virtual void NotifyDeleted( const Agent_ABC& agent );
    virtual void NotifyContextMenu( const Agent_ABC& agent, ContextMenu& menu );
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
    ValuedComboBox< const Agent_ABC* >* tc2Combo_;
    ValuedComboBox< const Agent_ABC* >* maintenanceCombo_;
    ValuedComboBox< const Agent_ABC* >* medicalCombo_;
    ValuedComboBox< const Agent_ABC* >* supplyCombo_;
    SafePointer< Agent_ABC > selected_;
    //@}
};

#endif // __ChangeLogisticLinksDialog_h_
