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

#include "ASN_Types.h"
#include "SafePointer.h"
#include "ValuedComboBox.h"

class Agent;
class Controllers;

// =============================================================================
/** @class  ChangeHumanFactorsDialog
    @brief  ChangeHumanFactors dialog
*/
// Created: AGE 2005-09-22
// =============================================================================
class ChangeHumanFactorsDialog : public QDialog
{
    Q_OBJECT
public:
    //! @name Constructors/Destructor
    //@{
             ChangeHumanFactorsDialog( QWidget* pParent, Controllers& controllers );
    virtual ~ChangeHumanFactorsDialog();
    //@}

    //! @name Operations
    //@{
    void Show( const Agent& agent );
    //@}

private slots:
    //! @name Slots
    //@{
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
    SafePointer< Agent > agent_;
    ValuedComboBox< ASN1T_EnumUnitFatigue >*     pTirednessCombo_;
    ValuedComboBox< ASN1T_EnumUnitMoral >*       pMoralCombo_;
    ValuedComboBox< ASN1T_EnumUnitExperience >*  pExperienceCombo_;
    QCheckBox*                                   pAllUnitsCheckBox_;
    //@}
};

#endif // __ChangeHumanFactorsDialog_h_
