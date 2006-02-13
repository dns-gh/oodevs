//****************************************************************************
//
// $Created:  NLD 2002-01-03 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ChangeDiplomacyDialog.h $
// $Author: Age $
// $Modtime: 6/04/05 15:17 $
// $Revision: 1 $
// $Workfile: ChangeDiplomacyDialog.h $
//
//*****************************************************************************

#ifndef __ChangeDiplomacyDialog_h_
#define __ChangeDiplomacyDialog_h_

#include "Types.h"
#include <QDialog.h>

class QComboBox;
class Team;

//=============================================================================
// Created:  NLD 2002-01-03 
//=============================================================================
class ChangeDiplomacyDialog : public QDialog
{
    Q_OBJECT
    MT_COPYNOTALLOWED( ChangeDiplomacyDialog );

public:
     ChangeDiplomacyDialog( QWidget* pParent = 0 );
    ~ChangeDiplomacyDialog();

    void Initialize( Team& team );

private slots:
    /** @name Main methods */
    //@{
    void Validate();
    void Reject  ();
    //@}

private:
    //! @name 
    //@{
    const Team& GetSelectedTeam ( const QComboBox& comboBox ) const;
          void      FillArmyComboBox( QComboBox& comboBox, Team* pSelected = 0 );
    //@}

  
private:
    QComboBox*     pArmy1ComboBox_;
    QComboBox*     pArmy2ComboBox_;
    QComboBox*     pDiplomacyComboBox_;
};

#endif // __ChangeDiplomacyDialog_h_
