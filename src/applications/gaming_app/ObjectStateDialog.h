// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ObjectStateDialog_h_
#define __ObjectStateDialog_h_

#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class ContextMenu;
    class Controllers;
    class Object_ABC;
    class Profile_ABC;
    class Time_ABC;
}

namespace actions
{
    class ActionsModel;
}

class StaticModel;

// =============================================================================
/** @class  ObjectStateDialog
    @brief  ObjectStateDialog
*/
// Created: NPT 2012-09-18
// =============================================================================
class ObjectStateDialog : public QDialog
                        , public tools::Observer_ABC
                        , public kernel::ContextMenuObserver_ABC< kernel::Object_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ObjectStateDialog( QWidget* pParent, kernel::Controllers& controllers, const StaticModel& staticModel, actions::ActionsModel& actionsModel, const kernel::Time_ABC& simulation, const kernel::Profile_ABC& profile );
    virtual ~ObjectStateDialog();
    //@}

protected slots:
    //! @name Slots
    //@{
    void OnOk();
    void OnCancel();
    void Show();
    //@}

public:
    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const kernel::Object_ABC& object, kernel::ContextMenu& menu );
    void UpdateDialog();
    //@}

protected:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const StaticModel& static_;
    actions::ActionsModel& actionsModel_;
    const kernel::Time_ABC& simulation_;
    const kernel::Profile_ABC& profile_;
    kernel::SafePointer< kernel::Object_ABC > selected_;
    QSpinBox* constructionSpinBox_;
    QSpinBox* miningSpinBox_;
    QSpinBox* bypassSpinBox_;
    //@}
};

#endif // __ObjectStateDialog_h_
