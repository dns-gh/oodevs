// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __InhabitantChangeConfinedStateDialog_h_
#define __InhabitantChangeConfinedStateDialog_h_

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

class TeamsModel;

// =============================================================================
/** @class  InhabitantChangeConfinedStateDialog
    @brief  InhabitantChangeConfinedStateDialog
*/
// Created: BCI 2011-02-22
// =============================================================================
class InhabitantChangeConfinedStateDialog : public QDialog
                                         , public tools::Observer_ABC
                                         , public kernel::ContextMenuObserver_ABC< kernel::Inhabitant_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             InhabitantChangeConfinedStateDialog( QWidget* pParent, kernel::Controllers& controllers, actions::ActionsModel& actionsModel, const kernel::Time_ABC& simulation, const kernel::Profile_ABC& profile );
    virtual ~InhabitantChangeConfinedStateDialog();
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
    InhabitantChangeConfinedStateDialog( const InhabitantChangeConfinedStateDialog& );            //!< Copy constructor
    InhabitantChangeConfinedStateDialog& operator=( const InhabitantChangeConfinedStateDialog& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    actions::ActionsModel& actionsModel_;
    const kernel::Time_ABC& simulation_;
    const kernel::Profile_ABC& profile_;
    kernel::SafePointer< kernel::Inhabitant_ABC > selected_;
    QCheckBox* pConfinedCheckBox_;
    //@}
};

#endif // __InhabitantChangeConfinedStateDialog_h_
