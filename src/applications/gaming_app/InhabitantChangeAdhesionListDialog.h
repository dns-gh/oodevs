// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __InhabitantChangeAdhesionListDialog_h_
#define __InhabitantChangeAdhesionListDialog_h_

#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"

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

// =============================================================================
/** @class  InhabitantChangeAdhesionListDialog
    @brief  InhabitantChangeAdhesionListDialog
*/
// Created: ABR 2011-01-25
// =============================================================================
class InhabitantChangeAdhesionListDialog : public QDialog
                                         , public tools::Observer_ABC
                                         , public kernel::ContextMenuObserver_ABC< kernel::Inhabitant_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             InhabitantChangeAdhesionListDialog( QWidget* pParent, kernel::Controllers& controllers, const StaticModel& staticModel, actions::ActionsModel& actionsModel, const kernel::Time_ABC& simulation, const kernel::Profile_ABC& profile );
    virtual ~InhabitantChangeAdhesionListDialog();
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
    InhabitantChangeAdhesionListDialog( const InhabitantChangeAdhesionListDialog& );            //!< Copy constructor
    InhabitantChangeAdhesionListDialog& operator=( const InhabitantChangeAdhesionListDialog& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
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
    //@}
};

#endif // __InhabitantChangeAdhesionListDialog_h_
