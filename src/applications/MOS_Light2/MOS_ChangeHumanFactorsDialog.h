// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MOS_ChangeHumanFactorsDialog_h_
#define __MOS_ChangeHumanFactorsDialog_h_

#include "MOS_ASN_Types.h"
class MOS_Agent;

// =============================================================================
/** @class  MOS_ChangeHumanFactorsDialog
    @brief  ChangeHumanFactors dialog
*/
// Created: AGE 2005-09-22
// =============================================================================
class MOS_ChangeHumanFactorsDialog : public QDialog
{
    Q_OBJECT
public:
    //! @name Constructors/Destructor
    //@{
             MOS_ChangeHumanFactorsDialog( QWidget* pParent );
    virtual ~MOS_ChangeHumanFactorsDialog();
    //@}

    //! @name Operations
    //@{
    void SetAgent( const MOS_Agent& agent );
    //@}

private slots:
    //! @name Slots
    //@{
    void Validate();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MOS_ChangeHumanFactorsDialog( const MOS_ChangeHumanFactorsDialog& );            //!< Copy constructor
    MOS_ChangeHumanFactorsDialog& operator=( const MOS_ChangeHumanFactorsDialog& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void SendMessage( uint id, ASN1T_EnumUnitFatigue, ASN1T_EnumUnitMoral, ASN1T_EnumUnitExperience );
    //@}

private:
    //! @name Member data
    //@{
    const MOS_Agent* pAgent_;
    MT_ValuedComboBox< ASN1T_EnumUnitFatigue >*     pTirednessCombo_;
    MT_ValuedComboBox< ASN1T_EnumUnitMoral >*       pMoralCombo_;
    MT_ValuedComboBox< ASN1T_EnumUnitExperience >*  pExperienceCombo_;
    QCheckBox*                                      pAllUnitsCheckBox_;
    //@}
};

#endif // __MOS_ChangeHumanFactorsDialog_h_
