// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __InhabitantChangeAlertedStateDialog_h_
#define __InhabitantChangeAlertedStateDialog_h_

#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace gui
{
    class DecimalSpinBoxAndSlider;
}

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class Inhabitant_ABC;
    class Profile_ABC;
    class Time_ABC;
}

namespace actions
{
    class ActionsModel;
}

class StaticModel;
class TeamsModel;

// =============================================================================
/** @class  InhabitantChangeAlertedStateDialog
    @brief  InhabitantChangeAlertedStateDialog
*/
// Created: ABR 2011-01-25
// =============================================================================
class InhabitantChangeAlertedStateDialog : public QDialog
                                       , public tools::Observer_ABC
                                       , public kernel::ContextMenuObserver_ABC< kernel::Inhabitant_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             InhabitantChangeAlertedStateDialog( QWidget* pParent, kernel::Controllers& controllers, const StaticModel& staticModel, actions::ActionsModel& actionsModel, const kernel::Time_ABC& simulation, const kernel::Profile_ABC& profile );
    virtual ~InhabitantChangeAlertedStateDialog();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const kernel::Inhabitant_ABC& entity, kernel::ContextMenu& menu );
    //@}

private slots:
    //! @name Slots
    //@{
    void Show();
    void Validate();
    void Reject();
    void closeEvent( QCloseEvent * e );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    InhabitantChangeAlertedStateDialog( const InhabitantChangeAlertedStateDialog& );            //!< Copy constructor
    InhabitantChangeAlertedStateDialog& operator=( const InhabitantChangeAlertedStateDialog& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const StaticModel& static_;
    actions::ActionsModel& actionsModel_;
    const kernel::Time_ABC& simulation_;
    const kernel::Profile_ABC& profile_;
    kernel::SafePointer< kernel::Inhabitant_ABC > selected_;
    QCheckBox* pAlertedCheckBox_;
    //@}
};

#endif // __InhabitantChangeAlertedStateDialog_h_
