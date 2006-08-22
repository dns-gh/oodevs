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

#include "gaming/ASN_Types.h"
#include "clients_gui/ValuedComboBox.h"
#include "clients_kernel/SafePointer.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"

namespace kernel
{
    class Agent_ABC;
    class Controllers;
}

class Publisher_ABC;

// =============================================================================
/** @class  ChangeHumanFactorsDialog
    @brief  ChangeHumanFactors dialog
*/
// Created: AGE 2005-09-22
// =============================================================================
class ChangeHumanFactorsDialog : public QDialog
                               , public kernel::Observer_ABC
                               , public kernel::ContextMenuObserver_ABC< kernel::Agent_ABC >
{
    Q_OBJECT
public:
    //! @name Constructors/Destructor
    //@{
             ChangeHumanFactorsDialog( QWidget* pParent, kernel::Controllers& controllers, Publisher_ABC& publisher );
    virtual ~ChangeHumanFactorsDialog();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const kernel::Agent_ABC& agent, kernel::ContextMenu& menu );
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
    void SendMessage( uint id, ASN1T_EnumUnitFatigue, ASN1T_EnumUnitMoral, ASN1T_EnumUnitExperience );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    Publisher_ABC& publisher_;
    kernel::SafePointer< kernel::Agent_ABC > selected_;
    gui::ValuedComboBox< E_UnitFatigue >*     pTirednessCombo_;
    gui::ValuedComboBox< E_UnitMoral >*       pMoralCombo_;
    gui::ValuedComboBox< E_UnitExperience >*  pExperienceCombo_;
    QCheckBox*                                   pAllUnitsCheckBox_;
    //@}
};

#endif // __ChangeHumanFactorsDialog_h_
