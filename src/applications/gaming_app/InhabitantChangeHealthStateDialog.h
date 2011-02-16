// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __InhabitantChangeHealthStateDialog_h_
#define __InhabitantChangeHealthStateDialog_h_

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

// =============================================================================
/** @class  InhabitantChangeHealthStateDialog
    @brief  InhabitantChangeHealthStateDialog
*/
// Created: ABR 2011-01-25
// =============================================================================
class InhabitantChangeHealthStateDialog : public QDialog
                                        , public tools::Observer_ABC
                                        , public kernel::ContextMenuObserver_ABC< kernel::Inhabitant_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             InhabitantChangeHealthStateDialog( QWidget* pParent, kernel::Controllers& controllers, actions::ActionsModel& actionsModel, const kernel::Time_ABC& simulation, const kernel::Profile_ABC& profile );
    virtual ~InhabitantChangeHealthStateDialog();
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
    void OnValuesChanged( int newValue = 0 );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    InhabitantChangeHealthStateDialog( const InhabitantChangeHealthStateDialog& );            //!< Copy constructor
    InhabitantChangeHealthStateDialog& operator=( const InhabitantChangeHealthStateDialog& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    actions::ActionsModel& actionsModel_;
    const kernel::Time_ABC& simulation_;
    const kernel::Profile_ABC& profile_;

    kernel::SafePointer< kernel::Inhabitant_ABC > selected_;

    QSpinBox* healthySpinBox_;
    QSpinBox* woundedSpinBox_;
    QSpinBox* deadSpinBox_;

    QLabel* newTotalLabel_;
    QLabel* originalTotalLabel_;
    //@}
};

#endif // __InhabitantChangeHealthStateDialog_h_
