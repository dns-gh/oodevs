//****************************************************************************
//
// $Created:  NLD 2002-01-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_ChangeDiplomacyDialog.h $
// $Author: Nld $
// $Modtime: 14/02/05 15:56 $
// $Revision: 2 $
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

    void Initialize();

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
          void      FillArmyComboBox( QComboBox& comboBox );
    //@}

  
private:
    QComboBox*     pArmy1ComboBox_;
    QComboBox*     pArmy2ComboBox_;
    QComboBox*     pDiplomacyComboBox_;
};

#include "MOS_ChangeDiplomacyDialog.inl"

#endif // __MOS_ChangeDiplomacyDialog_h_
