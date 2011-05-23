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
    class Entity_ABC;
    class Automat_ABC;
    class Formation_ABC;
    class Profile_ABC;
    class Time_ABC;
    class LogisticLevel;
}

namespace actions
{
    class ActionsModel;
}

class StaticModel;

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
                                , public tools::ElementObserver_ABC< kernel::Formation_ABC >
                                , public kernel::ContextMenuObserver_ABC< kernel::Automat_ABC >
                                , public kernel::ContextMenuObserver_ABC< kernel::Formation_ABC >

{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ChangeLogisticLinksDialog( QWidget* parent, kernel::Controllers& controllers, actions::ActionsModel& actionsModel, const StaticModel& staticModel, const kernel::Time_ABC& simulation, const kernel::Profile_ABC& profile );
    virtual ~ChangeLogisticLinksDialog();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyCreated( const kernel::Automat_ABC& agent );
    virtual void NotifyDeleted( const kernel::Automat_ABC& agent );
    virtual void NotifyContextMenu( const kernel::Automat_ABC& agent, kernel::ContextMenu& menu );
    virtual void NotifyCreated( const kernel::Formation_ABC& agent );
    virtual void NotifyDeleted( const kernel::Formation_ABC& agent );
    virtual void NotifyContextMenu( const kernel::Formation_ABC& agent, kernel::ContextMenu& menu );
    //@}

private slots:
    //! @name Slots
    //@{
    void Show();
    void Validate();
    void Reject();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ChangeLogisticLinksDialog( const ChangeLogisticLinksDialog& );            //!< Copy constructor
    ChangeLogisticLinksDialog& operator=( const ChangeLogisticLinksDialog& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    actions::ActionsModel& actionsModel_;
    const StaticModel& static_;
    const kernel::Time_ABC& simulation_;
    const kernel::Profile_ABC& profile_;

    gui::ValuedComboBox< const kernel::Entity_ABC* >* nominalSuperiorCombo_;
    gui::ValuedComboBox< const kernel::Entity_ABC* >* currentSuperiorCombo_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    const kernel::LogisticLevel* selectedLevel_;
    //@}
};

#endif // __ChangeLogisticLinksDialog_h_
