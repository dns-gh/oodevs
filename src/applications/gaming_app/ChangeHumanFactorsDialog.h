// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ChangeHumanFactorsDialog_h_
#define __ChangeHumanFactorsDialog_h_

#include "clients_gui/ValuedComboBox.h"
#include "clients_kernel/SafePointer.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/NullDisplayer.h"

namespace kernel
{
    class Agent_ABC;
    class Automat_ABC;
    class Controllers;
    class Entity_ABC;
    class Formation_ABC;
    class Profile_ABC;
    class Team_ABC;
    class Time_ABC;
}

namespace actions
{
    class ActionsModel;
}

class StaticModel;
class Simulation;

// =============================================================================
/** @class  ChangeHumanFactorsDialog
    @brief  ChangeHumanFactors dialog
*/
// Created: AGE 2005-09-22
// =============================================================================
class ChangeHumanFactorsDialog : public QDialog
                               , public tools::Observer_ABC
                               , public kernel::ContextMenuObserver_ABC< kernel::Agent_ABC >
                               , public kernel::ContextMenuObserver_ABC< kernel::Automat_ABC >
                               , public kernel::ContextMenuObserver_ABC< kernel::Formation_ABC >
                               , public kernel::ContextMenuObserver_ABC< kernel::Team_ABC >
                               , public kernel::NullDisplayer
                               , public tools::Caller< E_UnitTiredness >
                               , public tools::Caller< E_UnitMorale >
                               , public tools::Caller< E_UnitExperience >
                               , public tools::Caller< E_UnitStress >
{
    Q_OBJECT
public:
    //! @name Constructors/Destructor
    //@{
             ChangeHumanFactorsDialog( QWidget* pParent, kernel::Controllers& controllers, const StaticModel& staticModel, actions::ActionsModel& actionsModel, const kernel::Time_ABC& simulation, const kernel::Profile_ABC& profile );
    virtual ~ChangeHumanFactorsDialog();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const kernel::Agent_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Automat_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Formation_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Team_ABC& entity, kernel::ContextMenu& menu );
    virtual QSize sizeHint() const;
    //@}

private slots:
    //! @name Slots
    //@{
    void Show();
    void Validate();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ChangeHumanFactorsDialog( const ChangeHumanFactorsDialog& );            //!< Copy constructor
    ChangeHumanFactorsDialog& operator=( const ChangeHumanFactorsDialog& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void DoContextMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu );
    void SendAction( const kernel::Entity_ABC& entity, E_UnitTiredness, E_UnitMorale, E_UnitExperience, E_UnitStress );
    void SendMessage( const kernel::Entity_ABC& entity, E_UnitTiredness, E_UnitMorale, E_UnitExperience, E_UnitStress );
    virtual void Call( const E_UnitTiredness& fatigue );
    virtual void Call( const E_UnitMorale& morale );
    virtual void Call( const E_UnitExperience& experience );
    virtual void Call( const E_UnitStress& stress );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const StaticModel& static_;
    actions::ActionsModel& actionsModel_;
    const kernel::Time_ABC& simulation_;
    const kernel::Profile_ABC& profile_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    gui::ValuedComboBox< E_UnitTiredness >* pTirednessCombo_;
    gui::ValuedComboBox< E_UnitMorale >* pMoralCombo_;
    gui::ValuedComboBox< E_UnitExperience >* pExperienceCombo_;
    gui::ValuedComboBox< E_UnitStress >* pStressCombo_;
    //@}
};

#endif // __ChangeHumanFactorsDialog_h_
