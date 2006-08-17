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

#include "astec_gaming/ASN_Types.h"
#include "astec_gui/ValuedComboBox.h"
#include "astec_kernel/SafePointer.h"
#include "astec_kernel/ContextMenuObserver_ABC.h"

class Agent_ABC;
class Controllers;
class Publisher_ABC;

// =============================================================================
/** @class  ChangeHumanFactorsDialog
    @brief  ChangeHumanFactors dialog
*/
// Created: AGE 2005-09-22
// =============================================================================
class ChangeHumanFactorsDialog : public QDialog
                               , public Observer_ABC
                               , public ContextMenuObserver_ABC< Agent_ABC >
{
    Q_OBJECT
public:
    //! @name Constructors/Destructor
    //@{
             ChangeHumanFactorsDialog( QWidget* pParent, Controllers& controllers, Publisher_ABC& publisher );
    virtual ~ChangeHumanFactorsDialog();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const Agent_ABC& agent, ContextMenu& menu );
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
    Controllers& controllers_;
    Publisher_ABC& publisher_;
    SafePointer< Agent_ABC > selected_;
    ValuedComboBox< ASN1T_EnumUnitFatigue >*     pTirednessCombo_;
    ValuedComboBox< ASN1T_EnumUnitMoral >*       pMoralCombo_;
    ValuedComboBox< ASN1T_EnumUnitExperience >*  pExperienceCombo_;
    QCheckBox*                                   pAllUnitsCheckBox_;
    //@}
};

#endif // __ChangeHumanFactorsDialog_h_
