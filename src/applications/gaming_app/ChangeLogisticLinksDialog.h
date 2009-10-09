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
#include "tools/ElementObserver_ABC.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_gui/ValuedComboBox.h"

namespace kernel
{
    class Controllers;
    class Automat_ABC;
    class Profile_ABC;
}

class Publisher_ABC;

// =============================================================================
/** @class  ChangeLogisticLinksDialog
    @brief  ChangeLogisticLinksDialog
*/
// $$$$ SBO 2006-06-30: gérer les teams
// Created: SBO 2006-06-30
// =============================================================================
class ChangeLogisticLinksDialog : public QDialog
                                , public tools::Observer_ABC
                                , public tools::ElementObserver_ABC< kernel::Automat_ABC >
                                , public kernel::ContextMenuObserver_ABC< kernel::Automat_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ChangeLogisticLinksDialog( QWidget* parent, kernel::Controllers& controllers, Publisher_ABC& publisher, const kernel::Profile_ABC& profile );
    virtual ~ChangeLogisticLinksDialog();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyCreated( const kernel::Automat_ABC& agent );
    virtual void NotifyDeleted( const kernel::Automat_ABC& agent );
    virtual void NotifyContextMenu( const kernel::Automat_ABC& agent, kernel::ContextMenu& menu );
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
    kernel::Controllers& controllers_;
    Publisher_ABC& publisher_;
    const kernel::Profile_ABC& profile_;

    gui::ValuedComboBox< const kernel::Automat_ABC* >* tc2Combo_;
    gui::ValuedComboBox< const kernel::Automat_ABC* >* maintenanceCombo_;
    gui::ValuedComboBox< const kernel::Automat_ABC* >* medicalCombo_;
    gui::ValuedComboBox< const kernel::Automat_ABC* >* supplyCombo_;
    kernel::SafePointer< kernel::Automat_ABC > selected_;
    //@}
};

#endif // __ChangeLogisticLinksDialog_h_
