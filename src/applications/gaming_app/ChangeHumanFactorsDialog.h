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

#include "game_asn/Simulation.h"
#include "clients_gui/ValuedComboBox.h"
#include "clients_kernel/SafePointer.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/NullDisplayer.h"

namespace kernel
{
    class Controllers;
    class Profile_ABC;
    class Entity_ABC;
    class Agent_ABC;
    class Automat_ABC;
    class Formation_ABC;
    class Team_ABC;
}

class Publisher_ABC;

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
{
    Q_OBJECT
public:
    //! @name Constructors/Destructor
    //@{
             ChangeHumanFactorsDialog( QWidget* pParent, kernel::Controllers& controllers, Publisher_ABC& publisher, const kernel::Profile_ABC& profile );
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
    //! @name Copy/Assignement
    //@{
    ChangeHumanFactorsDialog( const ChangeHumanFactorsDialog& );            //!< Copy constructor
    ChangeHumanFactorsDialog& operator=( const ChangeHumanFactorsDialog& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void DoContextMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu );
    void SendMessage( uint id, ASN1T_EnumUnitTiredness, ASN1T_EnumUnitMorale, ASN1T_EnumUnitExperience );
    void SendMessage( const kernel::Entity_ABC& entity, ASN1T_EnumUnitTiredness, ASN1T_EnumUnitMorale, ASN1T_EnumUnitExperience );
    virtual void Call( const E_UnitTiredness& fatigue );
    virtual void Call( const E_UnitMorale& morale );
    virtual void Call( const E_UnitExperience& experience );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    Publisher_ABC& publisher_;
    const kernel::Profile_ABC& profile_;

    kernel::SafePointer< kernel::Entity_ABC > selected_;
    gui::ValuedComboBox< E_UnitTiredness >*     pTirednessCombo_;
    gui::ValuedComboBox< E_UnitMorale >*       pMoralCombo_;
    gui::ValuedComboBox< E_UnitExperience >*  pExperienceCombo_;
    //@}
};

#endif // __ChangeHumanFactorsDialog_h_
