//****************************************************************************
//
// $Created:  NLD 2002-01-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ChangeDiplomacyDialog.h $
// $Author: Age $
// $Modtime: 6/04/05 15:17 $
// $Revision: 1 $
// $Workfile: MOS_ChangeDiplomacyDialog.h $
//
//*****************************************************************************

#ifndef __MOS_ChangeDiplomacyDialog_h_
#define __MOS_ChangeDiplomacyDialog_h_

#include "MOS_Types.h"
#include <QDialog.h>

class QComboBox;
class MOS_Team;

//=============================================================================
// Created:  NLD 2002-01-03 
//=============================================================================
class MOS_ChangeDiplomacyDialog : public QDialog
{
    Q_OBJECT
    MT_COPYNOTALLOWED( MOS_ChangeDiplomacyDialog );

public:
     MOS_ChangeDiplomacyDialog( QWidget* pParent = 0 );
    ~MOS_ChangeDiplomacyDialog();

    void Initialize( MOS_Team& team );

private slots:
    /** @name Main methods */
    //@{
    void Validate();
    void Reject  ();
    //@}

private:
    //! @name 
    //@{
    const MOS_Team& GetSelectedTeam ( const QComboBox& comboBox ) const;
          void      FillArmyComboBox( QComboBox& comboBox, MOS_Team* pSelected = 0 );
    //@}

  
private:
    QComboBox*     pArmy1ComboBox_;
    QComboBox*     pArmy2ComboBox_;
    QComboBox*     pDiplomacyComboBox_;
};

#include "MOS_ChangeDiplomacyDialog.inl"

#endif // __MOS_ChangeDiplomacyDialog_h_
