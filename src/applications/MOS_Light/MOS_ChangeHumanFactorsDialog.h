//****************************************************************************
//
// $Created:  NLD 2002-01-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_ChangeHumanFactorsDialog.h $
// $Author: Nld $
// $Modtime: 30/11/04 10:35 $
// $Revision: 1 $
// $Workfile: MOS_ChangeHumanFactorsDialog.h $
//
//*****************************************************************************

#ifndef __MOS_ChangeHumanFactorsDialog_h_
#define __MOS_ChangeHumanFactorsDialog_h_

#include "MOS_Types.h"
#include <QDialog.h>

class QComboBox;
class MOS_Agent;

//=============================================================================
// Created:  NLD 2002-01-03 
//=============================================================================
class MOS_ChangeHumanFactorsDialog : public QDialog
{
    Q_OBJECT
    MT_COPYNOTALLOWED( MOS_ChangeHumanFactorsDialog );

public:
     MOS_ChangeHumanFactorsDialog( QWidget* pParent = 0 );
    ~MOS_ChangeHumanFactorsDialog();

    void SetAgent( const MOS_Agent& agent );

private slots:
    //-------------------------------------------------------------------------
    /** @name Main methods */
    //-------------------------------------------------------------------------
    //@{
    void Validate();
    void Reject  ();
    //@}
   
private:
    const MOS_Agent* pAgent_;

    QComboBox* pMoraleComboBox_;
    QComboBox* pTirednessComboBox_;
    QComboBox* pExperienceComboBox_;    
};

#include "MOS_ChangeHumanFactorsDialog.inl"

#endif // __MOS_ChangeHumanFactorsDialog_h_
