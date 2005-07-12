//****************************************************************************
//
// $Created:  NLD 2002-01-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_OrderConduiteMissionDialog.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 4 $
// $Workfile: MOS_OrderConduiteMissionDialog.h $
//
//*****************************************************************************

#ifndef __MOS_OrderConduiteMissionDialog_h_
#define __MOS_OrderConduiteMissionDialog_h_

#include "MOS_Types.h"

#include <qdialog.h>

class MOS_Agent;

class QComboBox;
class QCheckBox;

//=============================================================================
// Created:  NLD 2002-01-03 
//=============================================================================
class MOS_OrderConduiteMissionDialog : public QDialog
{
    Q_OBJECT
    MT_COPYNOTALLOWED( MOS_OrderConduiteMissionDialog );

public:
     MOS_OrderConduiteMissionDialog( QWidget* pParent = 0 );
    ~MOS_OrderConduiteMissionDialog();

    //-------------------------------------------------------------------------
    /** @name Init */
    //-------------------------------------------------------------------------
    //@{
    virtual void Initialize( MOS_Agent& agent );
    virtual void Cleanup   ();
    //@}

private slots:
    //-------------------------------------------------------------------------
    /** @name Main methods */
    //-------------------------------------------------------------------------
    //@{
    void SlotValidate();
    void SlotCancel  ();
    //@}

private:
    //-------------------------------------------------------------------------
    /** @name */
    //-------------------------------------------------------------------------
    //@{
    void FillComboBoxes();
    //@}
    
private:
    MOS_Agent* pAgent_;

    QComboBox*  pOrderConduiteComboBox_;
};

#ifdef MOS_USE_INLINE
#   include "MOS_OrderConduiteMissionDialog.inl"
#endif

#endif // __MOS_OrderConduiteMissionDialog_h_
